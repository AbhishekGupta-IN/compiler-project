#include "symTable.h"

struct symbolTable *allExistingTables[100];
int countUpLevelTables = 0;
int allScopes = 0;

int hashVal(char *lexeme){
	int val = 0;
	for(int i = 0 ; i<strlen(lexeme) ; i++){
		val+=lexeme[i];
	}
	return val % SYMTABSIZE;
}

struct symbolTable *createSymTable(struct symbolTable* parent, char *TableName){
	struct symbolTable *curr=malloc(sizeof(struct symbolTable));
	curr->parent = parent;
	curr->depth = 0;
	curr->child = NULL;
	curr->sibling = NULL;
	curr->offset = 0;
	curr->iterative = 0;
	curr->caseStmt = 0;

	//printf("%s %u\n",TableName, curr->sibling);
	if(parent==NULL){
		curr->depth = 1;
	}
	else{
		curr->depth = parent->depth + 1;
	}

	strcpy(curr->name,TableName);
	for(int i = 0 ; i < SYMTABSIZE ; i++){
		curr->places[i]=NULL;
	}

		if(parent!=NULL){
			if(parent->child==NULL){
				//printf("Parent: %s Child: %s",)
				parent->child = curr;		
			}
			else{
				struct symbolTable* ch = parent->child;
				//printf("Parent : %s , Name : %s, Current : %s Sib:%u\n",parent->name,ch->name, TableName,ch->sibling);
				while(ch->sibling!=NULL){
					ch = ch->sibling;
					//printf("Name : %s\n",ch->name);
					//sleep(1);
				}
				ch->sibling = curr;
			}
		}
	return curr;
}

// try to make a get type function 



void* arrayInitializer(int low, int high, enum type T){

	if(T == boolean){
		bool *ptr = malloc(sizeof(bool)*(high-low+1));
		return ptr;
	}

	if(T == real){
		float *ptr = malloc(sizeof(float)*(high-low+1));
		return ptr;
	}
	if(T == integer){
		int *ptr = malloc(sizeof(int)*(high-low+1));
		return ptr;
	}
	return NULL;
}
void copyIntoNode(struct element* ptr, struct element* node){
	ptr->width = node->width;
	ptr->offset = node->offset;
	ptr->isInput = node->isInput;
	ptr->T = node->T;
	if(node->arrayTag==1){
		ptr->arrayTag =1;
		ptr->details.isStatic = node->details.isStatic;
		ptr->details.T = node->details.T;
		ptr->details.low = node->details.low;
		ptr->details.high = node->details.high;
		ptr->details.maintainPointer = node->details.maintainPointer;
	}

	return;
}

struct element* returnDetails(char* lexeme, char* scope){
	char *ptr = malloc(sizeof(char)*100);
	//struct element* req = malloc(sizeof(struct element));
	for(int i = 0; i < allScopes; i++){
		if(strcmp(allExistingTables[i]->name,scope)==0){
			struct element * holder = searchTable(allExistingTables[i],lexeme);
			if(holder!=NULL){
				return holder;
			}
			else{
				return NULL;
			}
		}
	}
	return NULL;
}

struct element* searchTable(struct symbolTable *curr, char* lexeme){
	
	for(int j = 0; j < SYMTABSIZE; j++){
			if(curr->places[j]!=NULL){
				struct element * ptr = curr->places[j];
				while(ptr!=NULL){
					if(strcmp(ptr->lexeme,lexeme)==0){
						return ptr;
					}
					ptr = ptr->next;
				}
			}
		}	
	return NULL;
}


int lookUpForReDeclaration(struct symbolTable *curr,char* lexeme, struct element* node){
	int hash = hashVal(lexeme);
	
	struct element *ptr;
	// first check current table
	// then check for parents
	ptr = curr->places[hash];
	while(ptr!=NULL){
			if(strcmp(ptr->lexeme,lexeme)==0){
				if(ptr->isInput==0)
					return 1;
				else
					{
						//printf("\t(%u)Lexeme : %s, newType : %s\n\n",ptr,lexeme,getTypeName(node->T));
						copyIntoNode(ptr,node);
						//printf("\t(%u)Lexeme : %s, newType : %s, Input : %d\n\n",ptr,ptr->lexeme,getTypeName(node->T),ptr->isInput);
						return 2;
					}
			}
			ptr = ptr->next;
		}
	return 0;
}
struct symbolTable* lookupForChecking(struct symbolTable* curr, char* lexeme){
	int hash = hashVal(lexeme);
	
	struct element *ptr;
	// first check current table
	// then check for parents
	while(curr!=NULL){
		ptr = curr->places[hash];
		if(ptr==NULL && curr->parent ==NULL){
			return NULL;
		}
		if(ptr!=NULL){
			while(ptr!=NULL){
				if(strcmp(ptr->lexeme,lexeme)==0){
					return curr;
				}
				ptr = ptr->next;
			}
		}
		curr = curr->parent;
	}
	return NULL;
}

void printArrays(){
	printf("scope(module_name)\tscope(line_numbers)\tvariable_name\tstatic_or_dynamic\trange_lexemes\ttype_of_element\n");
	for(int i = 0; i < allScopes; i++){
		char *ptr2 = strtok(allExistingTables[i]->name, ">");
		struct symbolTable * table = allExistingTables[i];

		int lowScope = table->low;
		int highScope = table->high;
		for(int j = 0; j < SYMTABSIZE; j++){
			if(table->places[j]!=NULL){
				struct element * ptr= table->places[j];
				while(ptr!=NULL && ptr->arrayTag == 1){
					char tempArr[5];

						strcpy(tempArr,"yes");
						if(ptr->details.isStatic==1){
							printf("%s\t\t",ptr2);
							printf("%d-%d\t\t",lowScope,highScope);
							printf("%s\t\t",ptr->lexeme);
							printf("static\t\t");
							printf("[%d,%d]\t\t",ptr->details.low,ptr->details.high);
							printf("%s\n",getTypeName(ptr->T));
						}
						else{
							printf("%s\t\t",ptr2);
							printf("%d-%d\t\t",lowScope,highScope);
							printf("%s\t\t",ptr->lexeme);
							printf("dynamic\t\t");
							printf("[%s,%s]\t\t",ptr->details.lowVar->terminal.lexeme,ptr->details.highVar->terminal.lexeme);
							printf("%s\n",getTypeName(ptr->T));
						}	
					ptr = ptr->next;
				}			
			}
		}
	}
}

void insertEntry(struct symbolTable* current, int pos,char *lexeme,struct Node* type, int input, struct Node* idNode){

	struct element *ptr = malloc(sizeof(struct element));
	strcpy(ptr->lexeme,lexeme);
	ptr->next = NULL;
	ptr->isInput = input;
	strcpy(idNode->terminal.scopeName,current->name);
	strcpy(idNode->scope,current->name);
	//printf("SCOPE : %s\n",idNode->scope );
	//enum type T;
	if(strcmp(type->child->terminal.token,"INTEGER")==0){

	//printf("INT: %s:%s\n",lexeme,type->child->terminal.token);
		ptr->T = integer;
		ptr->arrayTag = 0;
		ptr->width = INT_WIDTH;
		ptr->offset = current->offset;


		//printf("(Integer)Width:\t%d, Table Offset:\t%d Type:\t%s\n",ptr->width,ptr->offset,getTypeName(ptr->T));
		current->offset += INT_WIDTH;
	}

	else if(strcmp(type->child->terminal.token,"REAL")==0){

	//printf("REAL: %s:%s\n",lexeme,type->child->terminal.token);
		ptr->T = real;
		ptr->arrayTag = 0;
		ptr->width = REAL_WIDTH;
		ptr->offset = current->offset;

		//printf("(Real)Width:\t%d, Table Offset:\t%d\n",ptr->width,ptr->offset);
		current->offset += REAL_WIDTH;
	}
	
	else if(strcmp(type->child->terminal.token,"BOOLEAN")==0){

	//printf("BOOL: %s:%s\n",lexeme,type->child->terminal.token);
		ptr->T = boolean;
		ptr->arrayTag = 0;
		ptr->width = BOOL_WIDTH;
		ptr->offset = current->offset;

		//printf("(Bool)Width:\t%d, Table Offset:\t%d\n",ptr->width,ptr->offset);
		current->offset += BOOL_WIDTH;
	}
	
	else{

	//printf("ELSE: %s:%s.\n",lexeme,type->child->terminal.token);
		ptr->arrayTag = 1;
		struct Node* info = type->child;
		struct Node* rangeArrays = info->sibling->sibling;
		struct Node* index1 = rangeArrays->child;
		struct Node* index2 = index1->sibling->sibling;
		struct Node* aType = rangeArrays->sibling->sibling->sibling;

		if(strcmp(aType->child->terminal.token,"BOOLEAN")==0)
				ptr->T = boolean;

		else if(strcmp(aType->child->terminal.token,"INTEGER")==0)
				ptr->T = integer;

		else if(strcmp(aType->child->terminal.token,"REAL")==0)
				ptr->T = real;



		if(strcmp(index1->child->terminal.token,"ID")==0){
			ptr->details.isStatic = 0;
			ptr->width = 1;
			ptr->details.lowVar = index1->child;
			//printf("Low Lexeme : %s\n",ptr->details.lowVar->terminal.lexeme);

			if(strcmp(index2->child->terminal.token,"NUM")==0){
				ptr->details.high = atoi(index2->child->terminal.lexeme);
			}
		}
		if(strcmp(index2->child->terminal.token,"ID")==0){
			ptr->details.isStatic = 0;
			ptr->width = 1;
			ptr->details.highVar = index2->child;

			//printf("High Lexeme : %s\n",ptr->details.highVar->terminal.lexeme);

			if(strcmp(index1->child->terminal.token,"NUM")==0){
				ptr->details.low = atoi(index1->child->terminal.lexeme);
			}
		}
		ptr->details.maintainPointer = NULL;
		if(strcmp(index1->child->terminal.token,"NUM")==0 && strcmp(index2->child->terminal.token,"NUM")==0){
			ptr->details.isStatic = 1;
			ptr->details.low = atoi(index1->child->terminal.lexeme);
			ptr->details.high = atoi(index2->child->terminal.lexeme);

			//printf("%s Bounds: %s, %s (%d, %d)\n",ptr->lexeme,index1->child->terminal.lexeme,index2->child->terminal.lexeme, ptr->details.low,ptr->details.high);
			if(ptr->T == boolean){
				ptr->details.maintainPointer = arrayInitializer(ptr->details.low,ptr->details.high,ptr->T);
			}
			else if(ptr->T == integer){
				ptr->details.maintainPointer = arrayInitializer(ptr->details.low,ptr->details.high,ptr->T);
			}
			else if(ptr->T == real){
				ptr->details.maintainPointer = arrayInitializer(ptr->details.low,ptr->details.high,ptr->T);	
			}

			if(ptr->isInput == 1){
				ptr->width = 1 + 2*sizeof(int);
			}
			else{
				if(ptr->T == boolean)
					ptr->width = 1 + (ptr->details.high - ptr->details.low + 1) * BOOL_WIDTH;

				else if(ptr->T == integer)
					ptr->width = 1 + (ptr->details.high - ptr->details.low + 1) * INT_WIDTH;

				else if(ptr->T == real)
					ptr->width = 1 + (ptr->details.high - ptr->details.low + 1) * REAL_WIDTH;
			}



		}

		ptr->offset = current->offset;
		current->offset = ptr->width;
		//printf("Low : %d\t High : %d\tStatic : %d\n",ptr->details.low,ptr->details.high,ptr->details.isStatic);

	}

		// Assign according to type
	//printf("ptr assigned\n");

	// if(current==NULL){
	// 	//printf("table null\n");
	// }
	int ret = lookUpForReDeclaration(current,lexeme,ptr);
	if(ret==1){
		printf("REDECLARATION ERROR: THIS(%s) IS ALREADY DECLARED IN CURRENT(%s) SCOPE\n",lexeme,current->name);
		noOfErrors++;
		return;
	}
	else if(ret==2){
		return;
	}


	if(current->places[pos]==NULL){
		//printf("NULL case\n");
		current->places[pos] = ptr;
		//printf("Inserted at NULL position\n");
	}
	else{
			//printf("NOT NULL case\n");
			struct element* p = current->places[pos];
        	while(p->next!=NULL){
        		p=p->next;
        	}
            p->next = ptr;

	}
	//printf("SymbolTable populated\n" );

}

void printActivationRecordSize(){
	for(int i = 0; i < countUpLevelTables; i++){
		int size = 0;
		struct symbolTable* table = allTables[i];
		size += returnTableSize(table);
		table = table->child;
		while(table!=NULL){
			size += returnTableSize(table);
			table = table->sibling;
		}
		printf("%s\t\t\t\t%d\n\n",allTables[i]->name,size);
	}
}

int returnTableSize(struct symbolTable * table){
	int size = 0;
	for(int j = 0; j < SYMTABSIZE; j++){
			if(table->places[j]!=NULL){
				struct element * ptr= table->places[j];
				while(ptr!=NULL){
					size+= ptr->width;
					ptr = ptr->next;
				}			
			}
		}
	return size;
}

void insertInST(struct Node *head, struct symbolTable* current){


	//<declareStmt> case
	if(strcmp(head->NonTerminal,"<declareStmt>")==0){
		//printf("\n\nInside <declareStmt> of scope : %s\n",current->name);

			// currently head is "DECLARE"
		struct Node* ch = head->child;
		//printf("Declare : %s\n",ch->terminal.lexeme);
			// type
		struct Node* type = ch->sibling->sibling->sibling;
		//printf("Type : %s\n",type->child->terminal.token);
			// idList node
		struct Node* list = ch->sibling;
		//printf("List : %s\n",list->NonTerminal);
			// id
		struct Node* id = list->child;
		//printf("Id : %s\n",id->terminal.lexeme);
			// <idList*>
		list = list->child->sibling;
		int hash = hashVal(id->terminal.lexeme);

		insertEntry(current, hash,id->terminal.lexeme,type,0, id);

		//printf("\t\t Lexeme : %s\n",id->terminal.lexeme);
		//printf("Inserted\n");
		while(strcmp(list->child->terminal.token,"EMPTY")!=0){
			hash = hashVal(list->child->sibling->terminal.lexeme);
			//printf("\t\t Lexeme : %s\n",list->child->sibling->terminal.lexeme);
			insertEntry(current,hash,list->child->sibling->terminal.lexeme,type,0, list->child->sibling);
			list = list->child->sibling->sibling;
		}
		//printf("\n\n" );
	}

	//<input_plist> || <output_plist> case
	else if(strcmp(head->NonTerminal,"<input_plist>")==0 ){

		//printf("\n\nInside <input_plist> of scope : %s\n",current->name);
		//printf("\tcurrentNode : %s\n",head->NonTerminal);
		// id
		struct Node* id = head->child;		
		//printf("currentNode : %s\n",id->terminal.lexeme);
		// type 
		struct Node* type = id->sibling->sibling;
		//printf("\tcurrentNode : %s\n",type->child->terminal.token);
		//<input_plist *> OR <output_plist*>
		struct Node* plist = type->sibling;
		//printf("\tcurrentNode : %s, token val of child: %s\n",plist->NonTerminal,plist->child->terminal.token);

		int hash = hashVal(id->terminal.lexeme);

		insertEntry(current,hash,id->terminal.lexeme,type,1,id);
		//printf("Inserted\n");

		//printf("\t\t Lexeme : %s\n",id->terminal.lexeme);

		while(strcmp(plist->child->terminal.token,"EMPTY")!=0){
			hash = hashVal(plist->child->sibling->terminal.lexeme);
			//printf("\t\t Lexeme : %s\n", plist->child->sibling->terminal.lexeme);

			insertEntry(current,hash,plist->child->sibling->terminal.lexeme,plist->child->sibling->sibling->sibling,1,plist->child->sibling);
			plist = plist->child->sibling->sibling->sibling->sibling;
		}	
		//printf("\n\n");

	}

	else if(strcmp(head->NonTerminal,"<output_plist>")==0){

		//printf("\n\nInside <output_plist> of scope : %s\n",current->name);

		//printf("\tcurrentNode : %s\n",head->NonTerminal);
		// id
		struct Node* id = head->child;		
		//printf("currentNode : %s\n",id->terminal.lexeme);
		// type 
		struct Node* type = id->sibling->sibling;
		//printf("\tcurrentNode : %s\n",type->child->terminal.token);
		//<input_plist *> OR <output_plist*>
		struct Node* plist = type->sibling;
		//printf("\tcurrentNode : %s, token val of child: %s\n",plist->NonTerminal,plist->child->terminal.token);

		int hash = hashVal(id->terminal.lexeme);


		//printf("\t\t Lexeme : %s\n", id->terminal.lexeme );
		insertEntry(current,hash,id->terminal.lexeme,type,0,id);
		//printf("Inserted\n");

		while(strcmp(plist->child->terminal.token,"EMPTY")!=0){
			hash = hashVal(plist->child->sibling->terminal.lexeme);
			//printf("\t\t Lexeme : %s\n", plist->child->sibling->terminal.lexeme);
			insertEntry(current,hash,plist->child->sibling->terminal.lexeme,plist->child->sibling->sibling->sibling,0,plist->child->sibling);
			plist = plist->child->sibling->sibling->sibling->sibling;
		}	
		//printf("\n\n");
	}

}


void startCreatingScopes(struct Node *head, struct symbolTable* parent, struct symbolTable* current, int d){

	if(head==NULL)
		return;

	if(head->visited == 1){
		return;
	}
	head->visited = 1;

	// if(head->tag == 0){
	// 	printf("\t\tCurrent Node : %s\n\n",head->NonTerminal);
	// }

	// else if(head->tag == 1){
	// 	printf("\t\tCurrent Node : %s , Line : %d\n\n",head->terminal.token,head->terminal.lineNumber);
	// }
	d++;
	/*
	if(head->tag == 1 ){
		printf("\tTerminal lexeme : %s, Depth : %d\n",head->terminal.lexeme,d);
	}
	else{
		printf("\tNonTerminal : %s, Depth : %d\n",head->NonTerminal,d);
	}
	*/


	//startCreatingScopes(head,parent,current);

	if (head->tag==1 && strcmp(head->terminal.token,"EMPTY")==0)
		return;
	if (head->tag==0){
		//NON-TERMINAL CHECKING
		if(strcmp(head->NonTerminal,"<driverModule>")==0){
			struct symbolTable* dm = createSymTable(NULL,"driver");

			allTables[countUpLevelTables++] = dm;
			allExistingTables[allScopes++] = dm;
			//printf("\n\tsymbolTable for driverModule \n\n");
			dm->low = head->child->sibling->sibling->sibling->sibling->child->terminal.lineNumber;
			dm->high = head->child->sibling->sibling->sibling->sibling->child->sibling->sibling->terminal.lineNumber;
			startCreatingScopes(head->child->sibling->sibling->sibling->sibling,NULL,dm,d);

		}
		else if(strcmp(head->NonTerminal,"<declareStmt>")==0){
			//printf("\nrunning declareStmt\n\n");
			insertInST(head, current);
			//return;
		}
		else if(strcmp(head->NonTerminal,"<otherModules>")==0){
			//struct symbolTable *om = createSymTable(NULL);

			startCreatingScopes(head->child,NULL,NULL,d);
		}
		else if(strcmp(head->NonTerminal,"<module>")==0){
			struct symbolTable *mod = createSymTable(NULL,head->child->sibling->sibling->terminal.lexeme);

			allTables[countUpLevelTables++] = mod;
			allExistingTables[allScopes++] = mod;
			//printf("\n\tsymbolTable for module :%s\n\n", head->NonTerminal);
			//changing it to <input_plist>
			struct Node *input_plist = head->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling;
			//printf("running input_plist : %s\n",input_plist->NonTerminal);
			insertInST(input_plist,mod); 
			struct Node *ret = input_plist->sibling->sibling->sibling;
			//printf("running output_plist\n");

			//printf("NT : %s\n",ret->NonTerminal);

			
			//printf("token : %s\n",ret->child->terminal.token);
			if(strcmp(ret->child->terminal.token,"EMPTY")!=0){
				insertInST(ret->child->sibling->sibling,mod);
			}

			mod->low = ret->sibling->child->terminal.lineNumber;
			mod->high = ret->sibling->child->sibling->sibling->terminal.lineNumber;

			startCreatingScopes(ret->sibling,NULL,mod,d);
			//return;

		}
		else if(strcmp(head->NonTerminal,"<iterativeStmt>")==0){

			//
			char name[100];
			int t = sprintf(name,"%s>iterative%d",current->name,current->iterative);
			current->iterative++;
			//printf("%s\n",name);

			struct symbolTable *iter = createSymTable(current,name);
			allExistingTables[allScopes++] = iter;
			// check for for and while
			// if(current->child == NULL){
			// 	current->child = iter;
			// }
			// else{
			// 	struct symbolTable *ch = current->child;
			// 	while(ch->sibling!=NULL){
			// 		ch = ch->sibling;
			// 	}
			// 	ch->sibling = iter;
			// }
			struct Node* check = head->child;
			struct Node* stmts;

			//printf("\n\tsymbolTable for iterativeStmt \n\n");
			if(strcmp(check->terminal.token,"FOR")==0){
				stmts = check->sibling->sibling->sibling->sibling->sibling->sibling->sibling;
				//printf("Inside Iterative For : %s\n", stmts->NonTerminal);
				iter->low = check->sibling->sibling->sibling->sibling->sibling->sibling->terminal.lineNumber;
				iter->high = stmts->sibling->terminal.lineNumber;
				startCreatingScopes(stmts,current,iter,d);
				//printf("Returned from For \n");
				//return;
			}
			else{
				stmts = check->sibling->sibling->sibling->sibling->sibling;
				//printf("Inside Iterative while : %s, %s\n", stmts->NonTerminal, stmts->terminal.token);
				iter->low = check->sibling->sibling->sibling->sibling->terminal.lineNumber;
				iter->high = stmts->sibling->terminal.lineNumber;
				startCreatingScopes(stmts,current,iter,d);
				//printf("Returned from while \n");
				//return;
			}
			//return;
		}
		else if(strcmp(head->NonTerminal,"<caseStmt>")== 0 || strcmp(head->NonTerminal,"<caseStmt*>")== 0 ){


			//printf("\n\tsymbolTable for caseStmt \n\n");
			if(strcmp(head->child->terminal.token,"EMPTY")==0){
				//return;
				}
			else{
				char name[100];
				int t = sprintf(name,"%s>case%d",current->name,current->caseStmt);
				current->caseStmt++;

				//printf("%s\n",name);

				struct symbolTable *caseStmt = createSymTable(current,name);
				
				allExistingTables[allScopes++] = caseStmt;
				//printf("Inserted Case Table\n");
				// if(current->child == NULL){
				// 	current->child = caseStmt;
				// 	}
				// else{
				// 		struct symbolTable *ch = current->child;
				// 		while(ch->sibling!=NULL){
				// 			ch = ch->sibling;
				// 			}
				// 		ch->sibling = caseStmt;
				// 	}

				struct Node* stmts = head->child->sibling->sibling->sibling;

				caseStmt->low = head->child->terminal.lineNumber;
				caseStmt->high = stmts->sibling->sibling->sibling->child->terminal.lineNumber - 1;

				startCreatingScopes(stmts,current,caseStmt,d);
				}
				if (strcmp(head->NonTerminal,"<caseStmt*>")==0){
				//	return;
				}
		}

	} 

	struct Node* ch = head->child;
	startCreatingScopes(ch,parent,current,d);
	if(ch!=NULL){
		while(ch->sibling!=NULL){
			startCreatingScopes(ch->sibling, parent, current,d);
			ch = ch->sibling;
		}
	}
}

char* getTypeName(enum type T) 
{
   switch (T) 
   {
      case boolean: return "Boolean";
      case integer: return "Integer";
      case real: return "Real";
      default: return "";
   }
   return "";
}

void printSymbolTables(){
	for(int i = 0; i < countUpLevelTables; i++){
		struct symbolTable* upLevel = allTables[i];
		printf("Current : %s\n",upLevel->name);
		for(int j = 0; j < SYMTABSIZE; j++){
			if(upLevel->places[j]!=NULL){
				struct element * ptr = upLevel->places[j];
				while(ptr!=NULL){
					printf("(%u)Lexeme : %s , Type : %s\n",ptr,ptr->lexeme, getTypeName(ptr->T));
					ptr = ptr->next;
				}
			}
		}
		printf("\n");
		printChildren(upLevel,0);
	}
}

void printChildren(struct symbolTable *table,int tag){
	if(table==NULL)
		return;
	//printf("\tInside Print Children\n\n");
	struct symbolTable* c = table->child;
	printChildren(c,1);
	if(tag==0){
		//printf("NOT PRINTING : %s\n",table->name);
		//
	}
	else{
		if (table!=NULL){
			printf("Table Name : %s\n",table->name);
		for(int j = 0; j < SYMTABSIZE; j++){
			if(table->places[j]!=NULL){
				struct element * ptr= table->places[j];
				while(ptr!=NULL){
					printf("(%u)Lexeme : %s , Type : %s\n",ptr,ptr->lexeme,getTypeName(ptr->T));
					ptr = ptr->next;
				}			
			}
		}
		printf("\n");
		}
	}

	if(c!=NULL)
    {	//printf("Parent : %s Child: %s\n",table->name,c->name);
        while(c->sibling!=NULL)
        {
        	//printf("Parent : %s Child: %s\n",table->name,c->sibling->name);
            printChildren(c->sibling,1);
            c=c->sibling;
        }
    }
    printf("\n\n");
}
void checker(){
	printf("%d\n",allScopes);
	for(int i = 0; i < allScopes; i++){
		//if(allExistingTables[i]!=NULL)
		printf("%d : %s\n",i+1,allExistingTables[i]->name);
	}
}


void printSymbolTableProc(){
	printf("variable_name\tscope(module_name)\tscope(line_numbers)\twidth\tisArray\tstatic_or_dynamic\trange_lexemes\ttype_of_element\toffset\tnesting_level\n");
	for(int i = 0; i < allScopes; i++){
		char *ptr2 = strtok(allExistingTables[i]->name, ">");
		struct symbolTable * table = allExistingTables[i];

		int lowScope = table->low;
		int highScope = table->high;
		for(int j = 0; j < SYMTABSIZE; j++){
			if(table->places[j]!=NULL){
				struct element * ptr= table->places[j];
				while(ptr!=NULL){
					char tempArr[5];

					if(ptr->arrayTag==0){
						strcpy(tempArr,"no");
						printf("%s\t\t",ptr->lexeme);
						printf("%s\t\t",ptr2);
						printf("%d-%d\t\t",lowScope,highScope);
						printf("%d\t\t",ptr->width);
						printf("%s\t\t",tempArr);
						printf("%s\t\t","----");
						printf("%s\t\t","----");
						printf("%s\t\t",getTypeName(ptr->T));
						printf("%d\t\t",ptr->offset);
						printf("%d\n",table->depth);
					}
					else{
						strcpy(tempArr,"yes");
						if(ptr->details.isStatic==1){
							printf("%s\t\t",ptr->lexeme);
							printf("%s\t\t",ptr2);
							printf("%d-%d\t\t",lowScope,highScope);
							printf("%d\t\t",ptr->width);
							printf("%s\t\t",tempArr);
							printf("static\t\t");
							printf("[%d,%d]\t\t",ptr->details.low,ptr->details.high);
							printf("%s\t\t",getTypeName(ptr->T));
							printf("%d\t\t",ptr->offset);
							printf("%d\n",table->depth);
						}
						else{
							printf("%s\t\t",ptr->lexeme);
							printf("%s\t\t",ptr2);
							printf("%d-%d\t\t",lowScope,highScope);
							printf("%d\t\t",ptr->width);
							printf("%s\t\t",tempArr);
							printf("dynamic\t\t");
							printf("[%s,%s]\t\t",ptr->details.lowVar->terminal.lexeme,ptr->details.highVar->terminal.lexeme);
							printf("%s\t\t",getTypeName(ptr->T));
							printf("%d\t\t",ptr->offset);
							printf("%d\n",table->depth);
						}	
					}
					ptr = ptr->next;
				}			
			}
		}
	}
}