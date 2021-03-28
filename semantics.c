#include "semantics.h"

void printNode(struct semNode* temp){
	if(temp->tag == 1){
		printf("lexeme: %s token: %s lineNumber : %d \n",temp->terminal.lexeme,temp->terminal.token,temp->terminal.lineNumber);
		if(strcmp(temp->terminal.token,"ID")==0){
			printf("Ele : %u\n",temp->ele);
			printf("Scope : %u\n",temp->scope);
		}
	}
	else{
		printf("NT : %s\n",temp->NonTerminal);
	}

	return;
}

struct semNode* makeSemNode(struct astNode* root){
	struct semNode* node = (struct semNode*) malloc (sizeof(struct semNode));
	node->tag = root->tag;
	strcpy(node->NonTerminal, root->NonTerminal);
	node->terminal = root->terminal;
	node->child = NULL;
	node->sibling = NULL;
	node->parent = NULL;
	node->rule = root->rule;
	node->scope = NULL;
	node->ele = NULL;
	node->visited = false;
	//printf("Scope(semNode) : %s\n",root->scope);
	if(node->tag == 1 && strcmp(node->terminal.token,"ID")==0){
		for(int i = 0; i<allScopes; i++){
			if(strcmp(root->scope, allExistingTables[i]->name)==0){
				node->scope = allExistingTables[i];
				break;
			}
		}
		struct symbolTable *table = node->scope;
		if(table!=NULL){
			int hash = hashVal(node->terminal.lexeme);
			struct element *entry = table->places[hash];
			while(entry!=NULL){
				if(strcmp(entry->lexeme,node->terminal.lexeme)==0){
					node->ele = entry;
					break;
				}
				entry = entry->next;
			}
			if(node->ele==NULL){
				//printNode(node);
				//printf("ERROR\n");
			}
			//printf("Entry Assigned: %s\t%s\t%s\t%u\n",node->terminal.lexeme,node->scope->name, node->ele->lexeme,node->ele);
		}
		else{

			//printNode(node);

			int lno = node->terminal.lineNumber;
			struct symbolTable* respectiveTable=NULL;
			for(int i = allScopes - 1; i >= 0 ; i--){
				//printf("Check: %s Low: %d high: %d\n",allExistingTables[i]->name, allExistingTables[i]->low,allExistingTables[i]->high);
				if(lno<allExistingTables[i]->high && lno>allExistingTables[i]->low){
					//printf("Checking for scope : %s\n",allExistingTables[i]->name);
					respectiveTable = lookupForChecking(allExistingTables[i], node->terminal.lexeme);
					if(respectiveTable!=NULL){
						//printf("Found Scope : %s", respectiveTable->name);
						break;
					}
				}
			}
			node->scope = respectiveTable;
			if(respectiveTable!=NULL)
			{
				int hash = hashVal(node->terminal.lexeme);
				struct element *entry = respectiveTable->places[hash];
				while(entry!=NULL){
					if(strcmp(entry->lexeme,node->terminal.lexeme)==0){
						node->ele = entry;
						break;
					}
					entry = entry->next;
				}
				if(node->ele==NULL){
					printNode(node);
					printf("ERROR\n");
				}
			}
			//printNode(node);
			//printf("\n\n");
		}
	}
	//printf("Assigned\n");
	//printNode(node);
	//printf("\n");
	return node;
}


struct semNode* GenerateSN(struct astNode* root){
	if(root == NULL) return NULL;
	struct semNode* node = makeSemNode(root);
	struct astNode* c = root->child;
	node->child = GenerateSN(c);
	struct astNode* s = root->sibling;
	node->sibling = GenerateSN(s);
	struct semNode* temp = node->child;
	while(temp != NULL){
		temp->parent = node;
		temp = temp->sibling;
	}
	return node;
}

void printSN_Tree(struct semNode * n,struct semNode* parent){
    if(n==NULL)
    return;
	if(parent == NULL){
		fprintf(writeSN, "***********INORDER TRAVERSAL*************\n\n");
	}
    struct semNode* c = n->child;
    printSN_Tree(c,n);

        if(n->tag==1){
        	//printf("\n");
        	//printNode(n);
        	//printf("\n");
            if(strcmp(n->terminal.token,"RNUM") == 0 || strcmp(n->terminal.token,"NUM") == 0){
                if(parent==NULL) fprintf(writeSN, "%s\t%d\t%s\t%s\t%s\t%s\t%s\n",n->terminal.lexeme,n->terminal.lineNumber,n->terminal.token,n->terminal.lexeme,"----","YES","----");
                else fprintf(writeSN, "%s\t%d\t%s\t%s\t%s\t%s\t%s\n",n->terminal.lexeme,n->terminal.lineNumber,n->terminal.token,n->terminal.lexeme,n->parent->NonTerminal,"YES","----");
            }
            else{
                if(parent==NULL) fprintf(writeSN, "%s\t%d\t%s\t%s\t%s\t%s\t%s\n",n->terminal.lexeme,n->terminal.lineNumber,n->terminal.token,"NA","----","YES","----");
                else fprintf(writeSN, "%s\t%d\t%s\t%s\t%s\t%s\t%s\n",n->terminal.lexeme,n->terminal.lineNumber,n->terminal.token,"NA",n->parent->NonTerminal,"YES","----");

            }
        }
        else{
            if(parent==NULL) fprintf(writeSN, "%s\t%s\t%s\t%s\t%s\t%s\t%s\n","----","----","----","----","----","No",n->NonTerminal);
            else fprintf(writeSN, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%d\n","----","----","----","----",n->parent->NonTerminal,"No",n->NonTerminal, n->rule);
			
        }
    if(c!=NULL){
        while(c->sibling!=NULL){
            printSN_Tree(c->sibling,n);
            c = c->sibling;
        }
    }
    return;
}


