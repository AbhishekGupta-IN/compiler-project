int LABEL = 1;
int TEMP_COUNT = 1;
int TEMP_LOCAL = 1;

char TEMP_BUF[5];
char LABEL_BUF[10];

char buffer[100];

int getTemporary() {
	memset(TEMP_BUF, 0, 5);
	sprintf(TEMP_BUF, "ri%d", TEMP_COUNT++);
}


int getLabel() {
	memset(LABEL_BUF, 0, 10);
	sprintf(LABEL_BUF, "label%d", LABEL++);
}

struct codeBlock * createCodeBlock() {

	codeBlock * new = (codeBlock *)malloc(sizeof(codeBlock));
	new->top = NULL;
	new->bot = NULL;

	return new;
}

int addCodeLine(char * buffer, codeBlock * cb) {

	codeLine * new = malloc(sizeof(codeLine));
	new->next = NULL;
	memset(new->line, 0, 100);
	strcpy(new->line, buffer);

	if(cb->top == NULL && cb->bot == NULL) {
		cb->top = new;
		cb->bot = new;
	}
	else {
		cb->bot->next = new;
		cb->bot = new;
	}
}

int printCodeBlock(codeBlock * blk) {

	codeLine * ln = blk->top;

	while(ln != NULL) {
		printf("%s", ln->line);
		ln = ln->next;
	}
}

int printToFile(codeBlock * blk, FILE * fp) {

	codeLine * ln = blk->top;

	while(ln != NULL) {
		fprintf(fp, "%s", ln->line);
		ln = ln->next;
	}
}

int codeGenIO(struct Node * ptr) {

	struct codeBlock * blk;
	struct Node* var;
	
	if( strcmp(ptr->child->terminal.token, "GET_VALUE") == 0 ) {
		
		blk = createCodeBlock();

		addCodeLine("\t; getting a value into a variable\n", blk);
		// setup stack frame 
		addCodeLine("\tpush rbp\n", blk); 
		// move printing format
		addCodeLine("\tmov rdi, s_fmt\n", blk);
		addCodeLine("\tmov rax, 0\n", blk);
		

		// handle arrays

		
		memset(buffer, 0, 100);
		sprintf(buffer, "\tmov rsi, %s\n", ptr->childL->next->se->temporary);
		addCodeLine(buffer, blk);
		
		addCodeLine("\tcall scanf\n", blk);
		addCodeLine("\tpop rbp\n", blk);

	}
	else if( strcmp(ptr->child->terminal.token, "PRINT") == 0 ) {
		
		blk = createCodeBlock();

		addCodeLine("\t; printing a value\n", blk);
		// setup stack frame 
		addCodeLine("\tpush rbp\n", blk); 
		// move printing format
		addCodeLine("\tmov rdi, p_fmt\n", blk);
		addCodeLine("\tmov rax, 0\n", blk);
		
		if( strcmp(ptr->child->sibling->sibling->NonTerminal, "<var>") == 0 ) {
			var = ptr->child->sibling->sibling;
			if( strcmp(var->child->terminal.token, "ID") == 0 ) {
				if(var->childL->se->isArray == 0) {
					memset(buffer, 0, 100);
					sprintf(buffer, "\tmov rsi, [%s]\n", var->childL->se->temporary);
					addCodeLine(buffer, blk);
				}
				else {

					if( strcmp(var->childL->next->childL->id->val, "NUM") == 0 ) {
						memset(buffer, 0, 100);
						sprintf(buffer, "\tmov ebx, dword %s\n", var->childL->next->childL->tptr->val);
						addCodeLine(buffer, blk);							
					}
					else {
						memset(buffer, 0, 100);
						sprintf(buffer, "\tmov ebx, [%s]\n", var->childL->next->childL->se->temporary);
						addCodeLine(buffer, blk);							
					}

					memset(buffer, 0, 100);
					sprintf(buffer, "\tmov rsi, [%s + 4*ebx]\n", var->childL->se->temporary);
					addCodeLine(buffer, blk);	
				}
			}
			else if( strcmp(var->childL->id->val, "RNUM") == 0 ) {
				memset(buffer, 0, 100);
				sprintf(buffer, "\tmov rsi, dword %s\n", var->childL->tptr->val);
				addCodeLine(buffer, blk);	
			}
			else if( strcmp(var->childL->id->val, "NUM") == 0 ) {
				memset(buffer, 0, 100);
				sprintf(buffer, "\tmov rsi, dword %s\n", var->childL->tptr->val);
				addCodeLine(buffer, blk);
			}
		}

		addCodeLine("\tcall printf\n", blk);
		addCodeLine("\tpop rbp\n", blk);


	}
	else {
		// error in AST.
	}

	ptr->blk = blk;
}

