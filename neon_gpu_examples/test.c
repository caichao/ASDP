#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

//Ne10
#include "NE10_types.h"
#include "NE10_physics.h"
#include "NE10_math.h"
#include "NE10_macros.h"
#include "NE10_init.h"
#include "NE10_imgproc.h"
#include "NE10_dsp.h"
#include "NE10.h"

//raspi_gpu_fft
#include "gpu_fft.h"
#include "mailbox.h"

#define jobs 1

float* floatdata_src = NULL;
int* intdata_src = NULL;

char Usage[] =
    "Usage: test.bin log2_N [jobs [loops]]\n"
    "log2_N = log2(FFT_length),       log2_N = 8...22\n"
    "data   = data type is float or int ,   0:float,        default 1 : int\n"
    "times_avg   =bigger , better?\n";

unsigned Microseconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec*1000000 + ts.tv_nsec/1000;
}

int main(int argc, char *argv[]) {
    int i, j, k, ret, freq, log2_N, data,times_avg, N, mb = mbox_open();
    int x;
    unsigned t[2];
    double tsq[2];

    struct GPU_FFT_COMPLEX *base;
    struct GPU_FFT *fft;

    log2_N = argc>1? atoi(argv[1]) : 12; // 8 <= log2_N <= 22
    data   = argc>2? atoi(argv[2]) : 0;  // transforms per batch
    times_avg   = argc>2? atoi(argv[3]) : 1000;
    if (argc<3) {
        printf(Usage);
        return -1;
    }

    N = 1<<log2_N; // FFT length
   	floatdata_src = (float *) malloc(sizeof(float) * N * 2); //
  	intdata_src = (int *) malloc(sizeof(int) * N * 2);
    memset(floatdata_src,-1,sizeof(float) * N * 2);
   	memset(intdata_src,-1,sizeof(int) * N * 2);
    if( data == 0){	
 /*  GPU_FFT_PROCESS  */
        printf("GPU_FFT_PROCESS:\n");	
        ret = gpu_fft_prepare(mb, log2_N, GPU_FFT_REV, 1, &fft); // call once	
        switch(ret) {
            case -1: printf("Unable to enable V3D. Please check your firmware is up to date.\n"); return -1;
            case -2: printf("log2_N=%d not supported.  Try between 8 and 22.\n", log2_N);         return -1;
            case -3: printf("Out of memory.  Try a smaller batch or increase GPU memory.\n");     return -1;
            case -4: printf("Unable to map Videocore peripherals into ARM memory space.\n");      return -1;
            case -5: printf("Can't open libbcm_host.\n");                                         return -1;
        }
	
        base = fft->in; // input buffer
        memcpy(base,floatdata_src, N*2*sizeof(float));
        usleep(1); // Yield to OS
        t[0] = Microseconds();
        for(x=0;x<times_avg;x++){
            gpu_fft_execute(fft); // call one or many times
        }
        t[1] = Microseconds();

        tsq[0]=tsq[1]=0;
        for (j=0; j<jobs; j++) {
            base = fft->out + j*fft->step; // output buffer
            freq = j+1;
            for (i=0; i<N; i++) {
                double re = cos(2*GPU_FFT_PI*freq*i/N);
                tsq[0] += pow(re, 2);
                tsq[1] += pow(re - base[i].re, 2) + pow(base[i].im, 2);
            }
        }
        printf("GPU_FFT_PROCESS: rel_rms_err = %0.2g, usecs = %d, num = %d\n",
            sqrt(tsq[1]/tsq[0]), (t[1]-t[0])/times_avg,N);
        gpu_fft_release(fft); // Videocore memory lost if not freed !
 /*  NEON_FFT_PROCESS  */
        printf("NEON_FFT_PROCESS:\n");
        if (ne10_init() != NE10_OK)
        {
            fprintf(stderr, "Failed to initialise Ne10.\n");
            return 1;
        }	
        ne10_fft_cpx_float32_t* neon_fft_src=(ne10_fft_cpx_float32_t *) malloc(sizeof(ne10_fft_cpx_float32_t) * N ); // A source array of input data
        ne10_fft_cpx_float32_t* neon_fft_dst=(ne10_fft_cpx_float32_t *) malloc(sizeof(ne10_fft_cpx_float32_t) * N ); // A destination array for the transformed data
        ne10_fft_cfg_float32_t cfg;          // An FFT "configuration structure"
        cfg = ne10_fft_alloc_c2c_float32(N);
      	memcpy(neon_fft_src,floatdata_src, N*2*sizeof(float));
      	t[0] = Microseconds();
        for(x=0;x<times_avg;x++){
            ne10_fft_c2c_1d_float32(neon_fft_dst, neon_fft_src, cfg, 0);
        }
      	t[1] = Microseconds();
      	ne10_fft_destroy_c2c_float32(cfg);
      	free(neon_fft_src);
      	free(neon_fft_dst);
      	printf("NEON_FFT_PROCESS: usecs = %d, num = %d\n", (t[1]-t[0])/times_avg,N);
 /*  NEON_MULTIPLY_PROCESS  */
        printf("NEON_MULTIPLY_PROCESS:\n");
       	ne10_float32_t* neon_mul_src=(float *) malloc(sizeof(float) * N);
        ne10_float32_t* neon_mul_dst=(float *) malloc(sizeof(float) * N);
	      memcpy(neon_mul_src,floatdata_src, N*sizeof(float));
	      t[0] = Microseconds();
        for(x=0;x<times_avg;x++){
            ne10_mul_float_c(neon_mul_dst,neon_mul_src,neon_mul_src,N);
        } 
	      t[1] = Microseconds();
	      free(neon_mul_src);
      	free(neon_mul_dst);
	      printf("NEON_MULTIPLY_PROCESS: usecs = %d, num = %d\n", (t[1]-t[0])/times_avg,N);
 /*  NORMAL_MULTIPLY_PROCESS  */	
        printf("NORMAL_MULTIPLY_PROCESS:\n");
      	float* normal_mul_dst =(float *) malloc(sizeof(float) * N);
      	t[0] = Microseconds();
        for(x=0;x<times_avg;x++){
            for(i=0;i<N;i++){
         	    	normal_mul_dst[i] = floatdata_src[i] * floatdata_src[i];
           	}
        }        	
      	t[1] = Microseconds();
       	free(normal_mul_dst);
       	printf("NORMAL_MULTIPLY_PROCESS: usecs = %d, num = %d\n", (t[1]-t[0])/times_avg,N);
    }	
    else if(data == 1){
 /*  GPU_FFT_PROCESS  */
        printf("GPU_FFT_PROCESS:\n");	
        ret = gpu_fft_prepare(mb, log2_N, GPU_FFT_REV, 1, &fft); // call once	
        switch(ret) {
            case -1: printf("Unable to enable V3D. Please check your firmware is up to date.\n"); return -1;
            case -2: printf("log2_N=%d not supported.  Try between 8 and 22.\n", log2_N);         return -1;
            case -3: printf("Out of memory.  Try a smaller batch or increase GPU memory.\n");     return -1;
            case -4: printf("Unable to map Videocore peripherals into ARM memory space.\n");      return -1;
            case -5: printf("Can't open libbcm_host.\n");                                         return -1;
        }
	
        base = fft->in; // input buffer
        memcpy(base,intdata_src, N*2*sizeof(float));
        usleep(1); // Yield to OS
        t[0] = Microseconds();
        for(x=0;x<times_avg;x++){
            gpu_fft_execute(fft); // call one or many times
        }     
        t[1] = Microseconds();

        tsq[0]=tsq[1]=0;
        for (j=0; j<jobs; j++) {
            base = fft->out + j*fft->step; // output buffer
            freq = j+1;
            for (i=0; i<N; i++) {
                double re = cos(2*GPU_FFT_PI*freq*i/N);
                tsq[0] += pow(re, 2);
                tsq[1] += pow(re - base[i].re, 2) + pow(base[i].im, 2);
            }
        }

        printf("GPU_FFT_PROCESS: rel_rms_err = %0.2g, usecs = %d, num = %d\n",
            sqrt(tsq[1]/tsq[0]), (t[1]-t[0])/times_avg,N);
        gpu_fft_release(fft); // Videocore memory lost if not freed !

        
 /*  NEON_FFT_PROCESS  */
        printf("NEON_FFT_PROCESS:\n");
        if (ne10_init() != NE10_OK)
        {
            fprintf(stderr, "Failed to initialise Ne10.\n");
            return 1;
        }	
        ne10_fft_cpx_int32_t* neon_fft_src=(ne10_fft_cpx_int32_t *) malloc(sizeof(ne10_fft_cpx_int32_t) * N ); // A source array of input data
        ne10_fft_cpx_int32_t* neon_fft_dst=(ne10_fft_cpx_int32_t *) malloc(sizeof(ne10_fft_cpx_int32_t) * N ); // A destination array for the transformed data
        ne10_fft_cfg_int32_t cfg;          // An FFT "configuration structure"
        cfg = ne10_fft_alloc_c2c_int32(N);
      	memcpy(neon_fft_src,intdata_src, N*2*sizeof(int));
      	t[0] = Microseconds();
        for(x=0;x<times_avg;x++){
            ne10_fft_c2c_1d_int32(neon_fft_dst, neon_fft_src, cfg, 0,0);//最后一个flag先设为0;
        }        
	      t[1] = Microseconds();
      	ne10_fft_destroy_c2c_int32(cfg);
      	free(neon_fft_src);
      	free(neon_fft_dst);
      	printf("NEON_FFT_PROCESS: usecs = %d, num = %d\n", (t[1]-t[0])/times_avg,N);
 /*  NEON_MULTIPLY_PROCESS  */	
        printf("NEON_MULTIPLY_PROCESS:\n");
	      ne10_int32_t* neon_mul_src=(int *) malloc(sizeof(int) * N);
	      ne10_int32_t* neon_mul_dst=(int *) malloc(sizeof(int) * N);
      	memcpy(neon_mul_src,intdata_src, N*sizeof(int));
      	t[0] = Microseconds();
        for(x=0;x<times_avg;x++){
            ne10_mul_float_c(neon_mul_dst,neon_mul_src,neon_mul_src,N);//没有 int型的函数。
        }   
	      t[1] = Microseconds();
	      free(neon_mul_src);
	      free(neon_mul_dst);
	      printf("NEON_MULTIPLY_PROCESS: usecs = %d, num = %d\n", (t[1]-t[0])/times_avg,N);
 /*  NORMAL_MULTIPLY_PROCESS  */	
        printf("NORMAL_MULTIPLY_PROCESS:\n");
	      int* normal_mul_dst =(int *) malloc(sizeof(int) * N);
	      t[0] = Microseconds();
        for(x=0;x<times_avg;x++){
     	      for(i=0;i<N;i++){
	             	normal_mul_dst[i] = intdata_src[i] * intdata_src[i];
           	}
        }                
	      t[1] = Microseconds();
	      free(normal_mul_dst);
       	printf("NORMAL_MULTIPLY_PROCESS: usecs = %d, num = %d\n", (t[1]-t[0])/times_avg,N);
    }


    free(floatdata_src);
    free(intdata_src);
    return 0;
}
