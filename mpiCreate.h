#ifndef MPIC_H_
#define MPIC_H_


void    InitMPI(int *argc, char **argv[], int *rank, int *numOfProcs, seqCombType* dataT);
void    createSeqCombType(MPI_Datatype* seqType);

#endif
