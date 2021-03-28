#include "symTableDef.h"

int hashVal(char *lexeme);
struct symbolTable *createSymTable(struct symbolTable* parent, char *TableName);
void insertInST(struct Node *head, struct symbolTable* current);
void insertEntry(struct symbolTable* current, int pos,char *lexeme,struct Node* type,int input, struct Node* idNode);
void startCreatingScopes(struct Node *n, struct symbolTable* parent, struct symbolTable* current, int d);
char* getTypeName(enum type T) ;
void printSymbolTables();
void printChildren(struct symbolTable *table,int tag);
struct symbolTable* lookupForChecking(struct symbolTable* curr, char* lexeme);
int lookUpForReDeclaration(struct symbolTable *curr,char* lexeme,struct element* node);
void copyIntoNode(struct element* ptr, struct element* node);
struct element* searchTable(struct symbolTable *curr, char* lexeme);
struct element* returnDetails(char* lexeme, char* scope);
void checker();
void printSymbolTableProc();
void printActivationRecordSize();
int returnTableSize(struct symbolTable * table);
void printArrays();