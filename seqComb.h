#ifndef SEQ_H_
#define SEQ_H_

#define SEQ1_MAX 5000
#define SEQ2_MAX 3000
#define WEIGHTS_LENGTH 4
#define NUM_OF_ATTRIBUTES 3



//sequences combinations (used only for checking results )
typedef struct SeqComb
{
   char     seq1[SEQ1_MAX];
   char     seq2[SEQ2_MAX];
   float    weights[WEIGHTS_LENGTH];
   char     mutant[SEQ2_MAX];
   int      offset;
} SeqComb;


//sequences combinations data type(used only for checking results )
typedef struct seqCombType
{
   int coord[2];
   MPI_Datatype seqDataType;
   MPI_Comm comm;
} seqCombType;

//helper struct to read file details
typedef struct fileData
{
   float    weights[WEIGHTS_LENGTH];
   int      numOfSeq2;
   char*    seq1;
   char**   seqList; 
}fileData;




void     buildSeq(SeqComb* val,char seq1[SEQ1_MAX],char seq2[SEQ2_MAX],float weights[WEIGHTS_LENGTH]);
void     printSeq(SeqComb* seq);
int      readWeightsFromFile(FILE* fp,fileData* data);
void     readDetFromFile(char *fileName ,fileData* data);
int      readseq1(FILE* fp,fileData* data);
int      readSeq2(FILE* fp,fileData* data);
void     printResults(int seqNum,float* score,int* results);















#endif
