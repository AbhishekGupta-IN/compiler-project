#include "parserDef.h"
/*
ID: 2017A7PS0136P Name: VITTHAL BHANDARI
ID: 2017A7PS0180P Name: IESHAAN SAXENA
ID: 2017A7PS0181P Name: NAMAN SINGHAL
ID: 2017A7PS0233P Name: DARSHAN AGRAWAL
ID: 2017A7PS0236P Name: ABHISHEK GUPTA
*/
int returnParseNodeCount();
void createDerivationsCopy();
void populateTerminalsArray(struct set Terminals[]);
void createParseTree(struct set TerminalsArr[]);
void printStack();
void printInorder(struct Node * n,struct Node* parent);
struct Node *createNode(char *name, int tag,int lineNumber,char *lexeme,struct Node* parent);
void printParseTable();
void createParseTable( struct set TerminalsArr[]);
int findEmptyRuleNo(char name[]);
bool isEmpty();
struct Node* pop();
void push(struct Node* node);
void printAllTerminals();
void printFollow();
void printFirst();
void printGrammar();
//struct Node *createNode(char *name, int tag,int lineNumber);
struct set* getAllTerminals(struct FirstAndFollow f);
void computeFollow();
void computeFirst();
struct set* findFollow(char node[]);
struct set* findFirst(char node[]);
struct set* add(struct set* head, char name[]);
struct set* union_func(struct set* head1, struct set* head2);
struct set* remove_empty(struct set* head);
bool contains(struct set* head, char str[]);
//struct FirstAndFollow
void createGrammar();

void createTokenTable();
tokenInfo *newNodeToken(char *lexeme_name,char * token_name);
void insertToken(char *token_name,char * lexeme_name);
void initializeTokenTable();
char * receiveLexeme(char * token);
void printTokenTable();