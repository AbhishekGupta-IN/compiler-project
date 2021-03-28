#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

/*
ID: 2017A7PS0136P Name: VITTHAL BHANDARI
ID: 2017A7PS0180P Name: IESHAAN SAXENA
ID: 2017A7PS0181P Name: NAMAN SINGHAL
ID: 2017A7PS0233P Name: DARSHAN AGRAWAL
ID: 2017A7PS0236P Name: ABHISHEK GUPTA
*/
#define BUFFER_LENGTH 255
#define NO_OF_KEYWORDS 33
#define BUFF_SIZE 40
#define HASH_TABLE_SIZE 107

//#ifndef _values
//#define _values
typedef union {
    double value_rnum;  // FOR RNUM
    int value_num;      // FOR NUM
    bool value_bool;    // FOR BOOLEAN
}values;
//#endif

//#ifndef _tokeninfo
//#define _tokeninfo
typedef struct
{
    char lexeme[25];
    char token[25];
    int lineNumber;
    values value;
    struct tokenInfo *next;
    char scopeName[100];
}tokenInfo;
//#endif



static char *keywords[NO_OF_KEYWORDS] =  {"integer", "real", "boolean", "of", "array", "start", "end" ,"declare", "module", "driver", "program" ,"record", "tagged", "union", "get_value", "print" ,"use", "with" ,"parameters" ,"true" ,"false" ,"takes", "input", "returns", "AND", "OR", "for", "in", "switch", "case", "break", "default", "while"};
static char *keywords_tokens[NO_OF_KEYWORDS] = {"INTEGER","REAL","BOOLEAN","OF","ARRAY","START","END","DECLARE","MODULE","DRIVER","PROGRAM","RECORD","TAGGED","UNION","GET_VALUE","PRINT","USE","WITH","PARAMETERS","TRUE","FALSE","TAKES","INPUT","RETURNS","AND","OR","FOR","IN","SWITCH","CASE","BREAK","DEFAULT","WHILE"};

tokenInfo *symbol_table[HASH_TABLE_SIZE];
extern FILE *fp;

extern int tt;
FILE *fp1,*copy;
char buffer[BUFF_SIZE],buffer2[BUFF_SIZE];
extern int idCount;
extern int numCount;
extern int lineNo,lineNo2;
extern bool eofState,eofState2;
char * Buffer[100];

extern int noOfErrors;