#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include"lexer.h"

/*
ID: 2017A7PS0136P Name: VITTHAL BHANDARI
ID: 2017A7PS0180P Name: IESHAAN SAXENA
ID: 2017A7PS0181P Name: NAMAN SINGHAL
ID: 2017A7PS0233P Name: DARSHAN AGRAWAL
ID: 2017A7PS0236P Name: ABHISHEK GUPTA
*/
#define EMPTY "EMPTY"
#define BUFFER_LENGTH 255
#define NO_OF_KEYWORDS 33
#define BUFF_SIZE 40
#define HASH_TABLE_SIZE 107



struct rules{
	char lhs[40];			// LHS
	struct rules* next;		// RHS
};

/*
#ifndef _values
#define _values
typedef union {
    double value_rnum;  // FOR RNUM
    int value_num;      // FOR NUM
    bool value_bool;    // FOR BOOLEAN
}values;
#endif

#ifndef _tokenInfo
#define _tokenInfo
typedef struct
{
    char lexeme[25];
    char token[25];
    int lineNumber;
     values value;
    struct tokenInfo *next;
}tokenInfo;
#endif
*/

struct Node{
	char NonTerminal[40];
	tokenInfo terminal;
	struct Node *child;
	struct Node *sibling;
	struct Node *parent;
	int tag;
	int rule;
	int visited;
	char scope[100];
};

struct grammar{
	struct rules decode[102];
};

struct set{
	char terminal[40];
	struct set* next;
	int rule_no;
	//struct rules* the_rule;
};

struct addFF{
	char rule[40];					// LHS
	struct set* first;	// First Sets
	struct set* follow;	// Follow Sets
};

struct FirstAndFollow{
	struct addFF ff[55];
};


struct stack{
	struct Node* node;
	struct stack* next;
};

static char *LEXEMES[57] =  {"integer", "real", "boolean", "of", "array", "start", "end" ,"declare", "module", "driver", "program" ,"record", "tagged", "union", "get_value", "print" ,"use", "with" ,"parameters" ,"true" ,"false" ,"takes", "input", "returns", "AND", "OR", "for", "in", "switch", "case", "break", "default", "while","+","-","*","/","<","<=",">=",">","==","!=","<<","<<<",">>>",">>",":","..",";",",",":=","[","]","(",")","**"};
static char *TOKENS[57] = {"INTEGER","REAL","BOOLEAN","OF","ARRAY","START","END","DECLARE","MODULE","DRIVER","PROGRAM","RECORD","TAGGED","UNION","GET_VALUE","PRINT","USE","WITH","PARAMETERS","TRUE","FALSE","TAKES","INPUT","RETURNS","AND","OR","FOR","IN","SWITCH","CASE","BREAK","DEFAULT","WHILE","PLUS","MINUS","MUL","DIV","LT","LE","GE","GT","EQ","NE","DEF","DRIVERDEF","DRIVERENDDEF","ENDDEF","COLON","RANGEOP","SEMICOL","COMMA","ASSIGNOP","SQBO","SQBC","BO","BC","COMMENTMARK"};

tokenInfo *token_table[HASH_TABLE_SIZE];

extern struct grammar derivations;
extern struct FirstAndFollow description;
int **parseTable;
int countTerminals;
int countNonTerminals;
struct Node *root;

struct set* onlyTerminals;

char Emp[10];

//int NO_OF_TOKENS;
FILE* writeFile;
struct stack *parser_stack;