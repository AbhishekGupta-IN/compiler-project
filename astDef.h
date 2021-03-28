#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include "parser.h"

struct astNode{
	int tag;
	char NonTerminal[40];
	tokenInfo terminal;
	struct astNode *child;
	struct astNode *sibling;
	struct astNode *parent;
	char scope[100];
	int rule;
};

struct astNode * ast;
FILE *writeAST;