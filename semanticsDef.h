#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include "symTable.h"

struct codeLine{
	char line[100];
	struct codeLine *next;
};

struct codeBlock
{
	struct codeLine *top;
	struct codeLine *bot;
};


struct semNode{

	struct semNode *child;
	struct semNode *parent;
	struct semNode *sibling;
	tokenInfo terminal;
	char NonTerminal[40];
	int rule;
	int tag;

	bool visited;
	struct element *ele;
	struct symbolTable *scope;

	struct codeBlock *blk;

	char label[10];
	char temporary[5];

};

struct semNode* sem_node;
FILE *writeSN;
