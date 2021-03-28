#include "astDef.h"

struct astNode *makeNode(struct Node *);
struct astNode *ast_generate(struct Node *);
void printAstTree(struct astNode *,struct astNode *);
void printAstNode(struct astNode *);
struct astNode *jointToRoot(struct astNode *, struct astNode *arr[], int);
struct astNode *nodeJoin(struct astNode *arr[], int);
int NumberOfastNode();

