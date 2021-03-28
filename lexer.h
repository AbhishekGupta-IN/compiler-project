#include "lexerDef.h"
/*
ID: 2017A7PS0136P Name: VITTHAL BHANDARI
ID: 2017A7PS0180P Name: IESHAAN SAXENA
ID: 2017A7PS0181P Name: NAMAN SINGHAL
ID: 2017A7PS0233P Name: DARSHAN AGRAWAL
ID: 2017A7PS0236P Name: ABHISHEK GUPTA
*/

void removeComments(char * testcaseFile , char * cleanFile,int print);
void printCleanFile();
void ignoreComments();
void createSymbolTable();
int computeHashValue(char *name);
tokenInfo *newNode(char *lexeme_name,char * token_name);
void insert(char *lexeme_name,char * token_name);
void initializeSymbolTable();
char * receiveToken(char * lexeme);
tokenInfo * getNextToken(FILE* fp);

tokenInfo * getNextToken2(FILE* fp);
void tokenCheck(FILE* fp);