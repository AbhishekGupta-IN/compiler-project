#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include "ast.h"


#define SYMTABSIZE 137
#define INT_WIDTH 4
#define REAL_WIDTH 8
#define BOOL_WIDTH 2


enum type{integer,real,boolean,error,isvoid};

struct arrayDetails{

	int isStatic; // 0 = STATIC ; 1 = DYNAMIC ;
	enum type T;
	int low;
	int high;
	void *maintainPointer;
	struct Node* lowVar;
	struct Node* highVar;	
};

struct element{
	//int occ;
	int width;
	int offset;
	int isInput; 	// 0 = NOT INPUT ; 1 = INPUT; 
	char lexeme[25];
	enum type T;
	int arrayTag; 	// 0 = NOT ARRAY ; 1 = ARRAY ;
	struct arrayDetails details;
	struct element *next;
};

struct symbolTable{

	struct symbolTable *parent;	// for maintain scopes
	struct symbolTable *child;
	struct symbolTable *sibling;
	struct element *places[SYMTABSIZE];
	int offset;
	int depth;
	char name[100];
	int iterative;
	int caseStmt;
	int low;
	int high;

};


struct symbolTable *allTables[20];
struct symbolTable *allExistingTables[100];
extern int countUpLevelTables;
extern int allScopes;