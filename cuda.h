#pragma once






int computeOnGPU(char* seq1,char* seq2,float* scoreMat,int* resultArr,float* bestScore);


//MACRO FUNCTIONS 

#define MALLOC(pointer,size){\
    cudaError_t err = cudaSuccess;\
    err=cudaMalloc((void **)&pointer,size);\
    if (err != cudaSuccess){\
        fprintf(stderr, "Failed to allocate device memory - %s\n", cudaGetErrorString(err));\
        exit(EXIT_FAILURE);\
    }\
}

#define COPY(pointer,data,size){\
      cudaError_t err = cudaSuccess;\
      err=cudaMemcpy(pointer,data,size,cudaMemcpyHostToDevice);\
      if (err!=cudaSuccess){\
       fprintf(stderr, "Failed to copy data from host to device - %s\n", cudaGetErrorString(err));\
       exit(EXIT_FAILURE);\
      }\
}


#define COPY_HOST(data,pointer,size){\
      cudaError_t err = cudaSuccess;\
      err=cudaMemcpy(data,pointer,size,cudaMemcpyDeviceToHost);\
      if (err!=cudaSuccess){\
       fprintf(stderr, "Failed to copy data from host to device - %s\n", cudaGetErrorString(err));\
       exit(EXIT_FAILURE);\
      }\
}

#define FREE(arr){\
    if (cudaFree(arr) != cudaSuccess) {\
        fprintf(stderr, "Failed to free device data - %s\n", cudaGetErrorString(err));\
        exit(EXIT_FAILURE);\
    }\
}



