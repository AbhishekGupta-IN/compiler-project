#include "semanticsDef.h"

struct semNode* makeSemNode(struct astNode*);
struct semNode* GenerateSN(struct astNode*);
void printNode(struct semNode* node);
void printSN_Tree(struct semNode*, struct semNode*);
