#include <helper_cuda.h>
#include <cuda_runtime.h>
#include "consts.h"
#include "cuda.h"
#include <stdio.h>
#include "cFunctions.h"



// implemntaion to atomic change of float (the given operations not valid to float )

__device__ __forceinline__ float atomicMaxFloat (float * addr, float value) {
    float old;
    old = (value >= 0) ? __int_as_float(atomicMax((int *)addr, __float_as_int(value))) :
         __uint_as_float(atomicMin((unsigned int *)addr, __float_as_uint(value)));


    return old;
}

//helper function to save the best offset , n , k
    __device__ void atomic_change(int *resultArr,int* tempResults)
    {
        atomicExch(&resultArr[BEST_OFFSET],tempResults[BEST_OFFSET]);
        atomicExch(&resultArr[BEST_K_PLACE],tempResults[BEST_M_PLACE]);
        atomicExch(&resultArr[BEST_M_PLACE], tempResults[BEST_K_PLACE]);
    }




//calculate the score of seq1 and given mutant by skip the score of locations n,k
__host__ __device__  float mutantScore(char* seq1,char* seq2 ,int seq2Len,int n,int k,float* scoreMat)
{

    int i=0;
    int j=i;
    float score=0;
    // float matScore=0;
    
    for(i=0;i<seq2Len-2;i++)
    {
        if(j==n || j==k)
            j++;
       
        score+=scoreMat[((seq1[i]-FIRST_CHAR)*NUM_CHARS)+(seq2[j]-FIRST_CHAR)];
    j++;
    }

    return score;

}

//create mutants n,k combitnations by array -for each n the fit k locate at  i + number of mutants (array[i]=n)
__host__ int* createMutants(int num_mutants)
{
	int* nkArr = (int*) malloc(num_mutants * 2 * sizeof(int));

	int n = 0, k = 1;
	for (int i = 0; i < num_mutants; i++) 
	{
		nkArr[i] = n;
		nkArr[i+num_mutants] = k;

		n++;
		if (n == k)
		{
			n = 0;
			k++;
		} 
	}
	
	return nkArr;
}


//calc the best score,mutants ,offsets for seq1,seq2 ,the score stored in pointer and the rest in "result arr"
   __global__ void GPU_bestScore(char* gpu_seq1,char* gpu_seq2,float* gpu_mat,float* score,int* resultArr,int maxOffset,int maxMutant,int seq2Len,int* mutants)
    {
        int i= blockDim.x * blockIdx.x + threadIdx.x;

        int j;
        float currentScore=0;
        float bestScore=-99999;
        int tempResults[RESULT_ARR];

    

        if(i<maxMutant)
        {
            for(j=0;j<maxOffset;j++)
            {
              currentScore=mutantScore(&gpu_seq1[j],&gpu_seq2[0],seq2Len,mutants[i],mutants[i+maxMutant],gpu_mat);

              if(bestScore<currentScore)
              {
                  bestScore=currentScore;
                  tempResults[BEST_OFFSET]=j;
                  tempResults[BEST_M_PLACE]=mutants[i];
                  tempResults[BEST_K_PLACE]=mutants[i+maxMutant];
               }
            }
        //atomic changes to avoid race condition 
        atomic_change(resultArr,tempResults); 
        atomicMaxFloat(score,bestScore);
        }            
    }




 

int computeOnGPU(char* seq1,char* seq2,float* scoreMat,int* resultArr,float* bestScore) {

    int seq2Len=strlen(seq2);
    int seq1Len=strlen(seq1);

    int maxOffset = seq1Len - (seq2Len - 2) + 1;
    int numOfMutants=seq2Len*(seq2Len-1)/2;

    int* mutantsArr=createMutants(numOfMutants);


    //alocate space in gpu 

    char* gpu_seq1;
    MALLOC(gpu_seq1,seq1Len);
    COPY(gpu_seq1,seq1,seq1Len);

    char* gpu_seq2;
    MALLOC(gpu_seq2,seq2Len);
    COPY(gpu_seq2,seq2,seq2Len);

    int matSize=(NUM_CHARS*NUM_CHARS)*sizeof(float);
    float* gpu_mat;
    MALLOC(gpu_mat,matSize);
    COPY(gpu_mat,scoreMat,matSize);

    int* gpu_mutants;
    MALLOC(gpu_mutants,numOfMutants * 2 * sizeof(int));
    COPY(gpu_mutants,mutantsArr,numOfMutants * 2 * sizeof(int));

    float* gpu_score;
    MALLOC(gpu_score,sizeof(float));
    COPY(gpu_score,bestScore,sizeof(float));

    int* result_gpu_arr;
    MALLOC(result_gpu_arr,sizeof(int)*RESULT_ARR);
    COPY(result_gpu_arr,resultArr,sizeof(int)*RESULT_ARR);

 

    // Launch the Kernel
    int threadsPerBlock = 256;
    int blocksPerGrid =(numOfMutants + threadsPerBlock - 1) / threadsPerBlock;
    cudaError_t err = cudaSuccess;
    GPU_bestScore<<<blocksPerGrid, threadsPerBlock>>>(gpu_seq1,gpu_seq2,gpu_mat,gpu_score,result_gpu_arr,maxOffset,numOfMutants,seq2Len,gpu_mutants);
    err = cudaGetLastError();
    if (err != cudaSuccess) {
        fprintf(stderr, "Failed to launch vectorAdd kernel -  %s\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    
    //copy the results to cpu 
    COPY_HOST(bestScore,gpu_score,sizeof(float));
    COPY_HOST(resultArr,result_gpu_arr,sizeof(int)*RESULT_ARR);


    FREE(gpu_score)
    FREE(result_gpu_arr)
    FREE(gpu_seq1)
    FREE(gpu_seq2)


    return 0;
   
    }

 




 


