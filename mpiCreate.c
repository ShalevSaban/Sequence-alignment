#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <stddef.h>

#include "seqComb.h"
#include "mpiCreate.h"
#include "consts.h"


#define NUM_OF_ATTRIBUTES 3


void InitMPI(int *argc, char **argv[], int *rank, int *numOfProcs, seqCombType* dataT)
{
    MPI_Init(argc, argv);
    MPI_Comm_rank(MPI_COMM_WORLD, rank);
    MPI_Comm_size(MPI_COMM_WORLD, numOfProcs);

    createSeqCombType(&dataT->seqDataType);
}

//create data type of seqType 
void createSeqCombType(MPI_Datatype* seqType)
{
    
    int blockLen[NUM_OF_ATTRIBUTES] ={SEQ1_MAX,SEQ2_MAX,WEIGHTS_LENGTH};
    MPI_Aint disp[NUM_OF_ATTRIBUTES];
    MPI_Datatype types[NUM_OF_ATTRIBUTES] = {MPI_CHAR, MPI_CHAR, MPI_INT};

    disp[0] = offsetof(SeqComb, seq1);
    disp[1] = offsetof(SeqComb, seq2);
    disp[2] = offsetof(SeqComb, weights);



    MPI_Type_create_struct(NUM_OF_ATTRIBUTES, blockLen, disp, types,seqType);
    MPI_Type_commit(seqType);
}



