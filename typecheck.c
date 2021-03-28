#include "typecheck.h"

int currentLineNumber;

void printErrors(struct semNode* root){
	if(root==NULL) return;
	if(root->visited == true){
		return;
	}
	enum type n = getType(root);
	// if(root->tag==0) printf("%d\t%s\t%s\n", n, root->terminal.token, root->terminal.lexeme);
	// else printf("%d\t%s\n", n,root->NonTerminal);
	printErrors(root->child);
	printErrors(root->sibling);
	return;
}

enum type getType(struct semNode* node){
	if(node==NULL)
		return error;
	node->visited = true;
	if(node->tag==1){
		if(strcmp(node->terminal.token, "NUM")==0){return integer;currentLineNumber = node->terminal.lineNumber;}
		if(strcmp(node->terminal.token, "RNUM")==0) {return real;currentLineNumber = node->terminal.lineNumber;}
		if(strcmp(node->terminal.token, "TRUE")==0) {return boolean;currentLineNumber = node->terminal.lineNumber;}
		if(strcmp(node->terminal.token, "FALSE")==0) {return boolean;currentLineNumber = node->terminal.lineNumber;}
		if(strcmp(node->terminal.token, "ID")==0) {
			//printNode(node);
			//printf("\n");
			currentLineNumber = node->terminal.lineNumber;
			if(node->ele==NULL || node->scope == NULL){
				//printNode(node);
				printf("Line No:- %d | Error: Variable %s Not Declared.\n", node->terminal.lineNumber, node->terminal.lexeme);
				noOfErrors++;
				return error;
			}
			return node->ele->T;
		}
	}
	else{
		int rule = node->rule+1;
		if(rule==21){
			struct semNode* temp1,*temp2;
			temp1 = node->child;
			temp2 = temp1->sibling;
			enum type t1,t2;
			t1 = getType(temp1);
			t2 = getType(temp2);
			if(t1==error || t2==error) return error;
			if(t1 != integer){
					printf("Line No:- %d | Error: Range is not an integer\n", currentLineNumber);
					noOfErrors++;
				return error;
			}
			if(t2 != integer){
				if(temp2!=NULL){
					currentLineNumber = temp2->terminal.lineNumber;
					printf("Line No:- %d | Error-- Range is not an integer\n", currentLineNumber);
					noOfErrors++;
				}
				return error;
			}
			return isvoid;
		}
		if(rule==42){
			struct semNode* temp = node->child;
			enum type t1 = getType(temp);
			if(t1==error) return error;
			if(t1 != integer){
				if(temp!=NULL){
					currentLineNumber = temp->terminal.lineNumber;
					printf("Line No:- %d | Error-- Array index not integer\n", currentLineNumber);
					noOfErrors++;
				}
				return error;
			}
			return isvoid;
		}
		if(rule==46){
			struct semNode* temp = node->child;
			enum type t1 = getType(temp);
			if(t1==error) return error;
			if(temp->ele->arrayTag==0){
				enum type  t2 = getType(temp->sibling->child);
				if(t2 == error) return error;
				if(t1 != t2){
					printf("Line No:- %d | Error-- Assignment is of different Data Types\n", currentLineNumber);
					noOfErrors++;
					return error;
				} 
				return isvoid;
			}
			else{
				if(temp->ele->details.isStatic == 1){
					struct semNode* temp1,*temp2;
					temp1 = temp->sibling->child;
					temp2 = temp1->sibling;
					enum type t2 = getType(temp1);
					if(t2==error) return error;
					if(strcmp(temp1->terminal.token,"NUM")==0){
						if(((temp1->terminal.value.value_num)<(temp->ele->details.low)) || ((temp1->terminal.value.value_num)>(temp->ele->details.high))){
							printf("Line No:- %d | Error-- Index value out of bound\n", currentLineNumber);
							noOfErrors++;
							return error;
						}
					}
					if(t2 != integer){
						printf("Line No:- %d | Error-- Array index not integer\n", currentLineNumber);
						noOfErrors++;
						return error;
					}
					enum type t3 = getType(temp2);
					//printf("%d\n", t3);
					if(t3==error) return error;
					if(t1 != t3){
						printf("Line No:- %d | Error-- Assignment is of different Data Types\n", currentLineNumber);
						noOfErrors++;
						return error;
					}
					return isvoid;
			}
			}
		}
		if(rule==59){
			struct semNode* temp = node->child;
			enum type t1 = getType(temp);
			//printf("%d\t%s\t%s\t%s\n", t1, temp->terminal.token, temp->terminal.lexeme, temp->NonTerminal);
			if(t1==error) return error;
			if(temp->sibling==NULL){
				return t1;
			}
			else{
				struct semNode* temp1 = temp->sibling->sibling;
				enum type t2 = getType(temp1);
				//printf("%d\t%s\t%s\t%s\n", t1, temp->terminal.token, temp->terminal.lexeme, temp->NonTerminal);
				if(t2==error) return error;
				if((t1 != boolean) || (t2 != boolean)){
					printf("Line No:- %d | Error-- Logical Op of Non-Boolean values\n", currentLineNumber);
					noOfErrors++;
					return error;
				}
			}
			return boolean;
		}
		if(rule==63){
			struct semNode* temp = node->child;
			enum type t1 = getType(temp);
			//printf("%d\t%s\t%s\t%s\n", t1, temp->terminal.token, temp->terminal.lexeme, temp->NonTerminal);
			if(t1==error) return error;
			if(temp->sibling==NULL){
				return t1;
			}
			else{
				if(temp->sibling->child != NULL){
					struct semNode* temp1 = temp->sibling->sibling;
					enum type t2 = getType(temp1);
					//printf("%d\t%s\t%s\t%s\n", t1, temp->terminal.token, temp->terminal.lexeme, temp->NonTerminal);
					if(t2==error) return error;
					if(t1 != t2){
						printf("Line No:- %d | Error-- Assignment is of different Data Types\n", currentLineNumber);
						noOfErrors++;
						return error;
					}
					if((t1 != boolean) || (t2 != boolean)){
						printf("Line No:- %d | Error-- Logical Op of Non-Boolean values\n", currentLineNumber);
						noOfErrors++;
						return error;
					}
				}	
			}
			return boolean;
		}
		if(rule==66 || rule==69){
			struct semNode* temp = node->child;
			enum type t1 = getType(temp);
			//printf("%d\t%s\t%s\t%s\n", t1, temp->terminal.token, temp->terminal.lexeme, temp->NonTerminal);
			if(t1==error) return error;
			if(temp->sibling==NULL){
				return t1;
			}
			else{
				struct semNode* temp1 = temp->sibling->sibling;
				enum type t2 = getType(temp1);
				//printf("%d\t%s\t%s\t%s\n", t1, temp->terminal.token, temp->terminal.lexeme, temp->NonTerminal);
				if(t2==error) return error;
				if(t1 != t2){
					printf("Line No:- %d | Error-- diff data type\n", currentLineNumber);
					noOfErrors++;
					return error;
				}
				if((t1 != boolean) || (t2 != boolean)){
					printf("Line No:- %d | Error-- Logical Op of Non-Boolean values\n", currentLineNumber);
					noOfErrors++;
					return error;
				}
			}
		}
		if(rule==74 || rule==75){
			enum type t1 = getType(node->child->sibling);
			if(t1==error) return error;
			if(t1 != integer || t1 != real){
				printf("Line No:- %d | Error-- Negation/Mod of Boolean\n", currentLineNumber);
				noOfErrors++;
				return error;
			}
			return t1;
		}
		if(rule==37){
			struct semNode* temp = node->child;
			enum type t1 = getType(temp);
			//printf("%d\t%s\t%s\t%s\n", t1, temp->terminal.token, temp->terminal.lexeme, temp->NonTerminal);
			if(t1==error) return error;
			if(temp->sibling==NULL){
				return t1;
			}
			struct semNode* temp1 = temp->sibling->child;
			enum type t2 = getType(temp1);
			if(t2==error) return error;
			if(strcmp(temp1->terminal.token,"NUM")==0){
				if(((temp1->terminal.value.value_num)<(temp->ele->details.low)) || ((temp1->terminal.value.value_num)>(temp->ele->details.high))){
					printf("Line No:- %d | Error-- Index value out of bound\n", currentLineNumber);
					noOfErrors++;
					return error;
				}
			}
			if(t2 != integer){
				printf("Line No:- %d | Error-- Array index not integer\n", currentLineNumber);
				noOfErrors++;
				return error;
			}
			return t1;
		}
		if(rule==91){
			struct semNode* temp = node->child;
			enum type t1 = getType(temp);
			if(t1==error) return error;
			if(t1 != integer){
				printf("Line No:- %d | Error-- Switch Case value not an Integer\n", currentLineNumber);
				noOfErrors++;
				return error;
			}
			return isvoid;
		}
		if(rule==100){
			struct semNode* temp1 = node->child;
			enum type t1 = getType(temp1);
			if(t1==error) return error;
			if(t1 != integer){
				printf("Line No:- %d | Error-- For_Loop ID doesn't have integer data type\n", currentLineNumber);
				noOfErrors++;
				return error;
			}
			return isvoid;
		}
		if(rule==101){
			struct semNode* temp = node->child;
			enum type t1 = getType(temp);
			if(t1==error) return error;
			if(t1!=boolean){
				printf("Line No:- %d | Error-- While_Loop Condition is not Boolean\n",currentLineNumber);
				noOfErrors++;
				return error;
			}
			return isvoid;
		}
		else{
			return isvoid;
		}
	}
}
