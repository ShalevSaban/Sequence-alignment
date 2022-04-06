#ifndef __CFUNCS_H__
#define __CFUNCS_H__



int     iSemiConse(char c1,char c2);
int     isConse(char c1,char c2);
char    getSign(char c1,char c2);
void    printMat(char* mat);
void    fillMat(char* chars_matrix);
void    fillScoreMat(float weights[WEIGHTS_LENGTH],char* chars_matrix,float* scoreMat);
float   getSignScore(char sign,float weights[WEIGHTS_LENGTH]);
void    printScoreMat(float* mat);
void    openFile(char* fileName);
void    writeToFile(char* fileName, int* results,float* score,int seqNum);






 

#endif
