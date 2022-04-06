#include <mpi.h>
#include "seqComb.h"
#include "mpiCreate.h"
#include <stdio.h>
#include "consts.h"
#include "cFunctions.h"
#include <math.h>
#include <cstring>

char conservatives[CONSERVATIVE_NUM][CONSERVATIVE_MAX_LEN] = { "NDEQ", "NEQK", "STA", "MILV", "QHRK", "NHQK", "FYW", "HY", "MILF" };
char semiConservatives[SEMI_CONSERVATIVE_NUM][SEMI_CONSERVATIVE_MAX_LEN] = { "SAG", "ATV", "CSA", "SGND", "STPA", "STNK", "NEQHRK", "NDEQHK", "SNDEQK", "HFY", "FVLIM" };



// Fill the matrix of letters combination with the signs via omp 

void fillMat(char* chars_matrix)
{
    char c1, c2;

#pragma omp parallel for 
    for (int i = 0; i < NUM_CHARS; i++)
    {
        c1 = FIRST_CHAR + i;
#pragma omp parallel for 
        for(int j=0;j<NUM_CHARS;j++)                   
        {
            c2 = FIRST_CHAR + j;
            chars_matrix[i*NUM_CHARS+j] = getSign(c1, c2);
        }
    }
}

// fill the score of letter combination  via omp 
void fillScoreMat(float weights[WEIGHTS_LENGTH],char* chars_matrix,float* scoreMat)
{//change this name**
   #pragma omp parallel for 
    for (int i = 0; i < NUM_CHARS; i++)
    {
#pragma omp parallel for 
        for(int j=0;j<NUM_CHARS;j++)                   
         scoreMat[i*NUM_CHARS+j]=getSignScore(chars_matrix[i*NUM_CHARS+j],weights);
    } 
}


//get the score for each sign (helper function to fill score mat)
float getSignScore(char sign,float weights[WEIGHTS_LENGTH])
{
    if(sign == ASTERISK)  
     return weights[STAR_LOC];
    if(sign == DOT)   
     return -weights[POINT_LOC];
    if(sign == COLON) 
     return -weights[COLON_LOC];
    else
     return -weights[SPACE_LOC];
}


void printMat(char* mat)
{
    printf("  ");
    for (int i=0;i<NUM_CHARS;i++)
    {
        printf("%c ",FIRST_CHAR+i);
    }
    printf("\n");
    for(int j=0;j<NUM_CHARS;j++)
    {
      printf("%c ",FIRST_CHAR+j);
      for(int k=0;k<NUM_CHARS;k++)
      {
       printf("%c ",mat[j*NUM_CHARS+k]);  
      }
    printf("\n"); 
    }
}


void printScoreMat(float* mat)
{
    printf("  ");
    for (int i=0;i<NUM_CHARS;i++)
    {
        printf("%c ",FIRST_CHAR+i);
    }
    printf("\n\n");
    for(int j=0;j<NUM_CHARS;j++)
    {
      printf("%c ",FIRST_CHAR+j);
      for(int k=0;k<NUM_CHARS;k++)
      {
       printf("%f ",mat[j*NUM_CHARS+k]);  
      }
    printf("\n"); 
    }
}
//get the sign for each 2 letters  (helper function to fill mat)
char getSign(char c1,char c2)
{
    if (c1==c2)
     return ASTERISK;
    else if (isConse(c1,c2))
     return COLON;
    else if (iSemiConse(c1,c2))
     return DOT;
    
   return SPACE;
}




//check if 2 letters combination is conservative
int isConse(char c1,char c2)
{
    for (int i=0;i<CONSERVATIVE_NUM;i++)
        if(strchr(conservatives[i],c1)&& strchr(conservatives[i],c2))
         return TRUE;
    return FALSE;
}

//check if 2 letters combination is semi  conservative
int iSemiConse(char c1,char c2)
{
for (int i=0;i<SEMI_CONSERVATIVE_NUM;i++)
        if(strchr(semiConservatives[i],c1)&& strchr(semiConservatives[i],c2))
         return TRUE;
    return FALSE;    
}



void writeToFile(char* fileName, int* results,float* score,int seqNum)
{
	FILE* fp;
	
	if ((fp = fopen(fileName, "a")) == 0) // open file
	{
		printf("cannot open file  %sfor writing\n", fileName);
		exit(0);
	}
	
    fprintf(fp,"  seq %d :   offset  %d   mutant [%d,%d] score : %f  \n",seqNum+1,results[0],results[1],results[2],score[0]); 	
	fclose(fp);
}

void openFile(char* fileName)
{
	FILE* fp;
	
	if ((fp = fopen(fileName, "w")) == 0) // open file
	{
		printf("cannot open file  %sfor writing\n", fileName);
		exit(0);
	}
	
    fprintf(fp,"Results : \n"); 	
	fclose(fp);
}

