#include <mpi.h>
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include "seqComb.h"
#include "mpiCreate.h"
#include <cstring>
#include<string.h>
#include "consts.h"
#include "cFunctions.h"
#include "cuda.h"




 

int main(int argc, char *argv[]) {

   	  int rank;
      seqCombType seqType;
      int numOfProcs=NUM_OF_PROCS;
      int startTime = omp_get_wtime();
      InitMPI(&argc, &argv, &rank, &numOfProcs, &seqType);
      MPI_Status status;

    


      if (rank==0)
      {
       

        char chars_matrix[NUM_CHARS*NUM_CHARS];
        float scoreMat[NUM_CHARS*NUM_CHARS];

      
        fileData* data=(fileData*)malloc(sizeof(fileData));

        //Read from file & and fill the score matrix by the weights from file
        readDetFromFile(FILE_NAME,data);
        fillMat(chars_matrix);

        fillScoreMat(data->weights,chars_matrix,scoreMat);
    
    //calculation of scores     
    for(int i=0;i<4;i++)
    {
        int* results=(int*)calloc(RESULT_ARR,sizeof(int)); //alcote to the result & best score 
        float* bestScore=(float*)calloc(1,sizeof(float));
        
        computeOnGPU(data->seq1,data->seqList[i],scoreMat,results,bestScore); // compute the best score on gpu 
        printResults(i+1,bestScore,results);  

        //Send the results to proc'1 
        MPI_Send(results,RESULT_ARR,MPI_INT,1,0,MPI_COMM_WORLD);
        MPI_Send(bestScore,1,MPI_FLOAT,1,1,MPI_COMM_WORLD);

        free(results);
        free(bestScore);
    }


printf("process %d finished work after - %.2f seconds\n", rank, omp_get_wtime() - startTime);	
  
}

else
{

openFile(OUTPUT_FILE);
int countOfSeq=0;

//get the results from pro' 0 and write to file 
while (countOfSeq<4)
{
  int* results=(int*)calloc(RESULT_ARR,sizeof(int)); //alcote to the result & best score 
  float* bestScore=(float*)calloc(1,sizeof(float));
  MPI_Recv(results,RESULT_ARR,MPI_INT,0,0,MPI_COMM_WORLD,&status);
  MPI_Recv(bestScore,1,MPI_FLOAT,0,1,MPI_COMM_WORLD,&status);
  writeToFile(OUTPUT_FILE,results,bestScore,countOfSeq);
  countOfSeq++;
}




	printf("process %d finished work after - %.2f seconds\n", rank, omp_get_wtime() - startTime);	

     
}

    MPI_Finalize();

    return 0;
}











