#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <cstring>
#include<string.h>


#include "seqComb.h"
#include "mpiCreate.h"
#include "consts.h"
#include "cFunctions.h"




//Read details from file and put in "fileData"
void readDetFromFile(char *fileName ,fileData* data)
{

    FILE* fp;  
        if ((fp = fopen(fileName, "r")) == 0)
    {
        printf("cannot open file %s for reading\n", fileName);
        exit(0);
    }

    if(!(readWeightsFromFile(fp,data)))
    {
      exit(0);
    }


    if(!(readseq1(fp,data)))
    {
     printf("Eror reading seq1\n");
    }

    fscanf(fp,"%d",&data->numOfSeq2); 

    if(!readSeq2(fp,data))
     exit(0);
    
}

void buildSeq(SeqComb* val,char seq1[SEQ1_MAX],char seq2[SEQ2_MAX],float weights[WEIGHTS_LENGTH])
{   
    strcpy(val->seq1,seq1);
    strcpy(val->seq2,seq2);
    memcpy(val->weights,weights,sizeof(float)*WEIGHTS_LENGTH);

}


void printSeq(SeqComb* seq)
{
    printf("seq1 : %s  \n seq2:%s \n   weight[2]: %f \n",seq->seq1,seq->seq2,seq->weights[2]);
}




int readWeightsFromFile(FILE* fp,fileData* data)
{
float weights[4];
for (int i=0;i<WEIGHTS_LENGTH;i++)
{
    if(!(fscanf(fp,"%f",&weights[i])))
    {
     printf("Eror reading weights");
     return 0;
    }
}
memcpy(data->weights,weights,sizeof(int)*WEIGHTS_LENGTH);
return 1;
}


int readseq1(FILE* fp,fileData* data)
{
    char seq1[SEQ1_MAX];
    if(!(fscanf(fp,"%s",seq1)))
     return 0;

    data->seq1=(char*)malloc(sizeof(char)*strlen(seq1));

    strcpy(data->seq1,seq1);


    return 1;
}



int readSeq2(FILE* fp,fileData* data)
{
 int numOfSeq=data->numOfSeq2;
 data->seqList=(char**)malloc(numOfSeq*sizeof(char*)); 

for(int i=0;i<numOfSeq;i++){
    char tempSeq[SEQ2_MAX];
    if(!(fscanf(fp,"%s",tempSeq)))
    {
        printf("Eror reading seq2");
        return 0;
    }
    data->seqList[i]=(char*)malloc(sizeof(char)*strlen(tempSeq));
    strcpy(data->seqList[i],tempSeq);
}
return 1;    
}



void printResults(int seqNum,float* score,int* results)
{
 printf("seq %d :   offset  %d   mutant [%d,%d] score : %f  \n",seqNum,results[0],results[1],results[2],score[0]); 

}

































