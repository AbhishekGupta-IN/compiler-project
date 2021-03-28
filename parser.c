#include "parser.h"
/*
ID: 2017A7PS0136P Name: VITTHAL BHANDARI
ID: 2017A7PS0180P Name: IESHAAN SAXENA
ID: 2017A7PS0181P Name: NAMAN SINGHAL
ID: 2017A7PS0233P Name: DARSHAN AGRAWAL
ID: 2017A7PS0236P Name: ABHISHEK GUPTA
*/
struct grammar derivations;
struct grammar derivations2;
struct FirstAndFollow description;

int idCount = 0;
int numCount = 0;
int rnumCount = 0;
int NO_OF_TOKENS = 57;
int fd = -1;
int gCount = 0;
int endArray[100];
int countEnd = 0, countEnd2= 0;
int parseTreeNodeCount = 0;



void createTokenTable(){
	for(int i = 0 ;i < HASH_TABLE_SIZE; i++){
		token_table[i] = NULL;
	}
}

void createDerivationsCopy(){
	for(int i = 0; i<102;i++){
		strcpy(derivations2.decode[i].lhs,derivations.decode[i].lhs);
		struct rules *temp = derivations.decode[i].next;
		struct rules *ptr = NULL;
		struct rules *tail = NULL;
		while(temp!=NULL){
			if(ptr==NULL){
				ptr = malloc(sizeof(struct rules));
				strcpy(ptr->lhs,temp->lhs);
				ptr->next = NULL;
				tail = ptr;
			}
			else{
				tail->next = malloc(sizeof(struct rules));
				tail = tail->next;
				strcpy(tail->lhs,temp->lhs);
				tail->next = NULL;
			}
			temp = temp->next;
		}
	derivations2.decode[i].next = ptr;
	}
}

tokenInfo *newNodeToken(char *token_name,char * lexeme_name){
    tokenInfo *node;
    node = (tokenInfo*)malloc(sizeof(tokenInfo));

    if (node==NULL){
        printf("memory error overflow\n");
    }
    strcpy(node->lexeme,lexeme_name);
    strcpy(node->token,token_name);
    node->next = NULL;
    return node;

}

void insertToken(char *token_name,char * lexeme_name){
    tokenInfo *ptr;
    ptr = newNodeToken(token_name,lexeme_name);
    int val = computeHashValue(ptr->token);

    if (token_table[val] == NULL)
        {
            token_table[val] = ptr;
        }
    else
        {	tokenInfo* p = token_table[val];
        	while(p->next!=NULL){
        		p=p->next;
        	}
            p->next = ptr;
        }
    //printf("%d:%s\n",val,ptr->lexeme);
}

void initializeTokenTable(){
    // Initializing Hash Table with keywords
    for(int i = 0; i <NO_OF_TOKENS ; i++)
        {
        	//printf("%s:%s\n",TOKENS[i],LEXEMES[i]);
            insertToken(TOKENS[i],LEXEMES[i]);

        }
}

void printTokenTable(){
	for(int i = 0; i < 107; i++){
		if(token_table[i]!=NULL){
			tokenInfo * ptr = token_table[i];
			while(ptr!=NULL){
				printf("%d:%s:%s\n",i,ptr->lexeme,ptr->token);
				ptr = ptr->next;
			}
		}
	}

}

char * receiveLexeme(char * token){

    int tokenTableIndex = computeHashValue(token);
    tokenInfo *ptr;
    ptr = token_table[tokenTableIndex];

    while(ptr != NULL){
        int c = strcmp(token,ptr->token);
        if(c == 0){
            return ptr->lexeme;
        }
        else{
            ptr = (tokenInfo *)ptr->next;
        }
    }
    return "NULL";
}

void push(struct Node* node)//,int tag, int Lno)
{

	struct stack* new_node = malloc (sizeof(struct stack));
	/*
    if(tag==0){
		new_node->node.tag = 0;
		strcpy(new_node->node.NonTerminal,push_name);
		printf("Inserting NonTerminal: %s \n",new_node->node.NonTerminal);
	}
	else{
		new_node->node.tag = 1;
		new_node->node.terminal.lineNumber = Lno;
		strcpy(new_node->node.terminal.token,push_name);
		printf("Inserting Terminal: %s \n",new_node->node.terminal.token);
	}
    */
    new_node->node = node;
	new_node->next = parser_stack;
	parser_stack = new_node;

}

bool isEmpty(){
	if(parser_stack == NULL){
	return true;
	}
	return false;
}

struct Node* pop(){
	struct Node *node = malloc(sizeof(struct Node));
	node = parser_stack->node;
	if(isEmpty()){
	printf("The stack is already empty\n");
	return NULL;
	}
	parser_stack = parser_stack->next;
	return node;
}
void createGrammar(){
	
	FILE * temp = fopen("grammar.txt","r");
	//printf("\tfd : %d\n",fd);
	gCount++;
	//fp1 = fopen("grammar.txt","r");
	if(temp==NULL){
		//fclose(fp1);
		//fp1 = fopen("grammar.txt","r");
		if(temp==NULL){
			printf("\tgCount : %d \n",gCount);
			//perror("File can't be read, Grammar");
			//exit(0);
		}
	}
	//int fd =fileno(temp);
	
	//derivations = (struct grammar *)malloc(sizeof(struct grammar)*100);

	//printf("Grammar Enter\n");

	char buff[BUFFER_LENGTH];
	memset(buff,'\0',255);
	int i = 0;
	int j = 0;
	int d;
	char *holder = malloc(sizeof(char)*40);

	while(fgets(buff,BUFFER_LENGTH,temp)){
		//printf("%s\n",buff);
		//for(int i = 0; i < strlen(buff), )
		//printf("H1\n");
		memset(holder,'\0',40);
		char *ptr;
		//printf("Hello\n");
		ptr =  strtok(buff," \t\n");
		//printf("%d ",strlen(ptr));

		strcpy(derivations.decode[i].lhs,ptr);
		//printf("%s\n",description.ff[j].rule);
		//printf("%d,%s\n",strlen(ptr));
		if(i==0){
			strcpy(description.ff[j].rule,ptr);
			description.ff[j].first = NULL;
			description.ff[j].follow = NULL;
		}

		else{

			if(strcmp(description.ff[j].rule,ptr)!=0)
			{
				j++;
				strcpy(description.ff[j].rule,ptr);
				description.ff[j].first = NULL;
				description.ff[j].follow = NULL;

			}
		}

		//printf("%s\n",derivations[i].rule);
		ptr = strtok(NULL," \t\n");
		ptr = strtok(NULL," \t\n");

		//printf("%s ",ptr);
		struct rules* maintain = &derivations.decode[i];

		while(ptr!=NULL){
			struct rules *newNode = (struct rules*) malloc(sizeof(struct rules));
			strcpy(holder,ptr);
			if(holder[strlen(holder)-1]==13){
				ptr[strlen(ptr)-1] = '\0';
			}
			strcpy(newNode->lhs,ptr);
			maintain->next = newNode;
			maintain = newNode;
			ptr = strtok(NULL," \t\n");
			//printf("%s ",ptr);

		}
		//printf("\n");
		i++;
	}

	//fclose(temp);
	//fclose(temp);
	//return description;
	//printf("Done\n");
}

bool contains(struct set* head, char str[]){
	struct set* temp = head;
	//char *holder = (char *) malloc(sizeof(char) * 40);
	//memset(holder,'\0',40);
	while(temp!=NULL){
		//printf("%ld,%s\n",strlen(temp->terminal) ,temp->terminal);
		//printf("%ld,%s\n",strlen(str), str);


		if(strcmp(temp->terminal, str)==0){
			//printf("hello\n");
			return true;
		}
		temp = temp->next;
	}
	return false;
}

struct set* remove_empty(struct set* head){
	if(strcmp(head->terminal, Emp)==0){
		head = head->next;
		return head;
	}
	struct set* temp = head;
	while(strcmp(temp->next->terminal, Emp) != 0){
		temp = temp->next;
	}
	temp->next = temp->next->next;
	return head;
}

struct set* union_func(struct set* head1, struct set* head2){
	if(head2==NULL) return head1;
	if(head1==NULL){
		head1 = head2;
		return head1;
	}
	struct set* temp_h2 = head2;
	struct set* head1_end = head1;
	while(head1_end->next != NULL){
		head1_end = head1_end->next;
	}
	while(temp_h2!=NULL){
		if(!contains(head1, temp_h2->terminal)){
			struct set* new_node = (struct set*) malloc (sizeof(struct set));
			new_node->next = NULL;
			new_node->rule_no = -1;
			strcpy(new_node->terminal, temp_h2->terminal);
			head1_end->next = new_node;
			head1_end = new_node;
		}
		temp_h2 = temp_h2->next;
	}
	return head1;
}

struct set* add(struct set* head, char name[]){
	//printf("%s\n", name);
	struct set* temp = head;
	struct set* new_node = (struct set*) malloc (sizeof(struct set));
	new_node->next = NULL;
	new_node->rule_no = -1;
	strcpy(new_node->terminal, name);
	//printf("Add:%s\n",new_node->terminal);
	if(head==NULL){
		head=new_node;
		return head;
	}
	while(temp->next != NULL){
		if(strcmp(temp->terminal, name) == 0) return head;
		temp = temp->next;
	}
	if(strcmp(temp->terminal, name) == 0) return head;
	else{
		temp->next = new_node;
	}
	return head;
}


struct set* findFirst(char node[]){
	createDerivationsCopy();
	struct set* head = malloc(sizeof(struct set));
	if(node[0] != '<'){
		struct set* new_node = (struct set*) malloc (sizeof(struct set));
		strcpy(new_node->terminal, node);
		new_node->next = NULL;
		head = new_node;
		return head;
	}
	int flag = 0;
	//struct set* update = malloc(sizeof(struct set));

	for(int i = 0; i<102; i++){
		//printf("Node: %s, Value: %s, flag : %d\n", node, derivations.decode[i].lhs, flag);
		if(strcmp(node, derivations2.decode[i].lhs)==0){
			flag=1;
			struct set* new_head = malloc(sizeof(struct set));
			struct rules* next_rule = malloc(sizeof(struct rules));
			next_rule = derivations2.decode[i].next;
			//printf("Node: %s\n",derivations.decode[i].lhs);

			while((next_rule != NULL) && (next_rule->lhs[0] == '<')){
				struct set* head_new = findFirst(next_rule->lhs);
				//printf("head_new_1:%s, head_new_2:%s\n", head_new->terminal, head_new->next->terminal);
				if(!contains(head_new, Emp)){
					//printf("-----Does not Contain Empty!!!\n");
					new_head = union_func(new_head, head_new);
					break;
				}
				else{
					//printf("-----Contains Empty!!!\n");
					struct set* no_empty_head = remove_empty(head_new);
					new_head = union_func(new_head, no_empty_head);
					//printf("new_head_1:%s, new_head_2:%s\n", new_head->terminal, new_head->next->terminal);
					next_rule = next_rule->next;
					//continue;
				}
			}
			head = union_func(head, new_head);
			//printf("head:%s, new_head:%s\n", head->terminal, new_head->terminal);
			if(next_rule==NULL){
				head = add(head, Emp);
			}
			else if((next_rule->lhs)[0] != '<'){
				//printf("Inside : %s\n",next_rule->lhs);
				head = add(head, next_rule->lhs);
				//printf("Head:%s:%s\n",head->terminal, head->next->terminal);
			}


			struct set*temp = head;
			while(temp != NULL){
				if(temp->rule_no == -1)
					temp->rule_no = i;
				//printf("%s : %d , %d, %s\n",temp->terminal,temp->rule_no, i,derivations.decode[i].lhs);
				temp = temp->next;
			}
			//printf("%s\n", derivations.decode[i].next->lhs);
			//printf("%s, %s\n",node, head->terminal);
		}
		else{
			if(flag==1) return head;
			//else continue;
		}
	}

	return head;
}


struct set* findFollow(char node[]){
	//printf("%s : Hello\n", node);
	createDerivationsCopy();
	struct set* head = NULL;
	for(int i = 0; i<102; i++){
		struct set* new_head = NULL;
		char permanent[40];
		strcpy(permanent, derivations2.decode[i].lhs);
		struct rules* next_rule = derivations2.decode[i].next;
		while(next_rule != NULL){
			if(strcmp(node, next_rule->lhs)==0){
				if(next_rule->next == NULL){
					if(strcmp(permanent, node)==0){
						break;
					}
					struct set* head_new = findFollow(permanent);
					new_head = union_func(new_head, head_new);
					break;
				}
				else{
					struct set* head_new_temp = findFirst(next_rule->next->lhs);
					if(contains(head_new_temp, Emp)){
						struct set* head_new = remove_empty(head_new_temp);
						new_head = union_func(new_head, head_new);
						struct rules* loop = next_rule->next;
						if(loop->next==NULL){
							if(strcmp(permanent, node)==0){
								break;
							}
							struct set* temp1 = findFollow(permanent);
							new_head = union_func(new_head, temp1);
							break;
						}
						loop = loop->next;
						while(loop != NULL){
							struct set* temp2 = findFirst(loop->lhs);
							if(contains(temp2, Emp)){
								struct set* temp3 = remove_empty(temp2);
								new_head = union_func(new_head, temp3);
							}
							else{
								new_head = union_func(new_head, temp2);
								break;
							}
							loop = loop->next;
						}
						if(loop==NULL){
							if(strcmp(permanent, node)==0){
								break;
							}
							struct set* temp4 = findFollow(permanent);
							new_head = union_func(new_head, temp4);
						}
					}
					else{
						new_head = union_func(new_head, head_new_temp);
					}
				}
			}
			next_rule = next_rule->next;
		}
		head = union_func(head, new_head);
	}
	if(head==NULL){
		struct set* new_node = (struct set*) malloc (sizeof(struct set));
		strcpy(new_node->terminal, "$");
		new_node->next = NULL;
		head = new_node;
	}
	return head;
}


void computeFirst(){
	for(int i = 0; i < 55; i++){
		description.ff[i].first = findFirst(description.ff[i].rule);

	}
	for(int i = 0; i < 55; i++){
		description.ff[i].first = description.ff[i].first->next;
	}
}


void computeFollow(){
	for(int i = 0; i<55; i++){
		description.ff[i].follow = findFollow(description.ff[i].rule);
	}
	for(int i = 0; i < 55 ; i++){


		if(strlen(description.ff[i].follow->terminal)==0){
			description.ff[i].follow = description.ff[i].follow->next;
		}

		//printf("%s, %d\n",description.ff[i].follow->terminal,strlen(description.ff[i].follow->terminal));


	}
}

struct set* getAllTerminals(struct FirstAndFollow f){
	// Storing terminals that occur in first and follow only

	struct set* head = (struct set*) malloc(sizeof(struct set));

	for(int i = 0; i < 55; i++){
		struct set* temp = (struct set*)malloc(sizeof(struct set));
		temp = description.ff[i].first;
		//printf("Temp: %s\n",temp->terminal);
		while(temp!=NULL){
			head = union_func(head,temp);
			temp = temp->next;
		}
		temp = description.ff[i].follow;
		while(temp!=NULL){
			head = union_func(head,temp);
			temp = temp->next;
		}

	}

	// REMOVING EMPTY
	head = remove_empty(head);


	head = head->next;
	struct set* temp = malloc(sizeof(struct set));
	temp = head;
	while(temp!=NULL){
		countTerminals++;
		temp = temp -> next;
	}
 // Empty element removal
	return head;
}


void printGrammar(){
	char *nt = (char *)malloc(sizeof(char)*40);
	char *derives = (char *)malloc(sizeof(char)*40);
	int d;
	for(int i = 0; i < 102; i++){
		memset(nt,'\0',40);
		memset(derives,'\0',40);
		d = 0;
		//printf("%s\n",derivations[i].rule);
		strcpy(nt,derivations2.decode[i].lhs);
		struct rules *temp = derivations2.decode[i].next;
		printf(" %s : ",nt);
		while(temp!=NULL){
			printf("%d:%s ",d,temp->lhs);
			temp = temp->next;
			d++;
		}
		printf("\n");
	}
}



void printFirst(){
	printf("\n↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓______FIRST______↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓\n\n");
	for(int i = 0; i < 55; i++){
		struct set* temp = description.ff[i].first;
		printf("%d : %s ->\t",i,description.ff[i].rule);
		int j = 0;
		while(temp != NULL){
			printf("%d,%s : %d\t", j,temp->terminal, temp->rule_no);
			temp = temp->next;
			j++;
		}

		printf("\n");
	}
	printf("\n↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑______FIRST______↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑\n\n");
}


void printFollow(){
	printf("\n↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓_____FOLLOW_____↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓\n\n");
	for(int i = 0; i < 55; i++){
		struct set* temp = description.ff[i].follow;
		printf("%d : %s ->\t",i,description.ff[i].rule);
		int j = 0;
		while(temp != NULL){
			printf(" %d:%s\t",j, temp->terminal);
			temp = temp->next;
			j++;
		}
		printf("\n");
	}
	printf("\n↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑_____FOLLOW_____↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑\n\n");
}

void printAllTerminals(){
	struct set* head = onlyTerminals;
	int i=0;
	while(head!=NULL){
		printf("%s,%d\n",head->terminal,i++);
		head = head->next;
	}


}

int findEmptyRuleNo(char name[]){
	for(int i = 0; i<102; i++){
		struct rules* temp = malloc(sizeof(struct rules));
		temp = derivations.decode[i].next;
		if(strcmp(temp->lhs, Emp)==0 && strcmp(derivations.decode[i].lhs,name)==0){
			return i;
		}
	}
}

void createParseTable( struct set TerminalsArr[]){

	parseTable = (int **)malloc(sizeof(int *) * countNonTerminals);
	for(int i = 0; i < countNonTerminals; i++)
		parseTable[i] = (int *)malloc(sizeof(int)* countTerminals);

	for(int i = 0; i < countNonTerminals; i++)
		for(int j = 0; j < countTerminals; j++)
			parseTable[i][j] = -1 ;

	for(int i = 0; i < countNonTerminals; i++){
		//char * ptr = (char *)malloc(sizeof(char) * 40);
		//strcpy(ptr,description.ff[i].rule);
		struct set *first = malloc(sizeof(struct set));
		first = description.ff[i].first;
		struct set *follow =description.ff[i].follow;
		//printf("%s\n",description.ff[i].rule);
		if(contains(first,Emp)){
				int rule_no = findEmptyRuleNo(description.ff[i].rule);
				//printf("\tContains Empty,with rule:%d\n",rule_no);
				while(follow!=NULL){
					for(int j = 0; j < countTerminals; j++){
						if(strcmp(TerminalsArr[j].terminal,follow->terminal)==0){
							//printf("\t\t%s",follow->terminal);
							parseTable[i][j] = rule_no;break;
						}
					}
					follow = follow->next;
				}
			}
		while(first!=NULL){

			//printf("\tWith usual firsts\n");
			for(int j = 0; j < countTerminals; j++){
				if(strcmp(first->terminal,TerminalsArr[j].terminal)==0){
					//printf("\t\tFirst:%s\n",first->terminal);
					parseTable[i][j] = first->rule_no;break;
					}
				}

			first = first->next;

			/*for(int j = 0; j < countTerminals; j++){
				if(strcmp(first->terminal,TerminalsArr[j].terminal)==0){
					//printf("%s : %s\n",description.ff[i].rule,first->terminal);
					if(contains(first,Emp)){
						//printf("\tContains Empty\n");

						while(follow!=NULL){
							int rule_no = findEmptyRuleNo(description.ff[i].rule);
							printf("%s\t%s\t%s\t%d\n",description.ff[i].rule,first->terminal,follow->terminal,rule_no);
							printf("%d,%d\n",i,j);
							parseTable[i][j] = rule_no;
							follow = follow -> next;
						}
					}
					parseTable[i][j] = first->rule_no;
				}
			}*/
			//printf("\n");
		}
	}
}

void printParseTable(){
	for(int i = 0; i < countNonTerminals; i++){
		printf("%d: ",i);
		for(int j = 0; j < countTerminals; j++){
			printf("%d ",parseTable[i][j]);
		}
		printf("\n");
	}

}
int returnParseNodeCount(){
	return parseTreeNodeCount;
}

struct Node *createNode(char *name, int tag,int lineNumber,char *lexeme, struct Node *parent){
	struct Node* newNode = malloc(sizeof(struct Node));
	newNode->tag = tag;
	newNode->parent = parent;
	parseTreeNodeCount++;
	newNode->visited = 0;
	if(tag == 0){
		strcpy(newNode->NonTerminal,name);
	}
	else{

       	newNode->terminal.lineNumber = lineNumber;

		if(strcmp(name,Emp)!=0)
			{
			strcpy(newNode->terminal.token,name);
			strcpy(newNode->terminal.lexeme,receiveLexeme(newNode->terminal.token));
			// adjustments for token and id
			/*
			if(strcmp(newNode->terminal.token,"ID\0")==0){
				tokenInfo *tok = getNextToken2(copy);
				while(strcmp(tok->token,"ID\0")!=0){
					tok = getNextToken2(copy);
				}
				idCount++;
				//printf("Found ID: %s\n",tok->lexeme);
	        	strcpy(newNode->terminal.lexeme,tok->lexeme);
			}
			else if(strcmp(newNode->terminal.token,"NUM\0")==0){
				tokenInfo *tok = getNextToken2(copy);
				while(strcmp(tok->token,"NUM\0")!=0){
					tok = getNextToken2(copy);
				}
				numCount++;
				//printf("Found NUM: %s\n",tok->lexeme);
	        	strcpy(newNode->terminal.lexeme,tok->lexeme);
			}
			else if(strcmp(newNode->terminal.token,"RNUM\0")==0){
				tokenInfo *tok = getNextToken2(copy);
				while(strcmp(tok->token,"RNUM\0")!=0){
					tok = getNextToken2(copy);
				}
				rnumCount++;
				//printf("Found RNUM: %s\n",tok->lexeme);
	        	strcpy(newNode->terminal.lexeme,tok->lexeme);
			}
			else{
        		strcpy(newNode->terminal.lexeme,receiveLexeme(newNode->terminal.token));
			}
			*/
        }
        else{
        	strcpy(newNode->terminal.token,name);
        	strcpy(newNode->terminal.lexeme,"----");
        }
	}
	//printf("Node Created: %s\n",name);
	return newNode;
}

void printInorder(struct Node * n,struct Node* parent)
{
    if(n==NULL)
    return;
    struct Node * c=n->child;
    printInorder(c,n);

        if(n->tag==1){
            if(strcmp(n->terminal.token,"RNUM") == 0 ){
            	tokenInfo *tok = getNextToken2(copy);
				while(tok!=NULL && strcmp(tok->token,"RNUM\0")!=0){
					tok = getNextToken2(copy);
				//	printf("Token(RNUM):%s\n",tok->lexeme);
				//	getchar();
				}
				rnumCount++;

				if(tok==NULL){
				//	printf("RNUM:Encountered NULL\n");
				}
				//printf("Found RNUM: %s\n",tok->lexeme);
	        	strcpy(n->terminal.lexeme,tok->lexeme);
	        	fprintf(stdout,"%s\t%d\t%s\t%s\t%s\t%s\t%s\n",n->terminal.lexeme,n->terminal.lineNumber,n->terminal.token,n->terminal.lexeme,n->parent->NonTerminal,"YES","----");

                fprintf(writeFile,"%s\t%d\t%s\t%s\t%s\t%s\t%s\n",n->terminal.lexeme,n->terminal.lineNumber,n->terminal.token,n->terminal.lexeme,n->parent->NonTerminal,"YES","----");
            }
            else if(strcmp(n->terminal.token,"NUM") == 0 ){
            	tokenInfo *tok = getNextToken2(copy);
				while(tok!=NULL && strcmp(tok->token,"NUM\0")!=0){
					tok = getNextToken2(copy);
				//	printf("Token(NUM):%s\n",tok->lexeme);
				//	getchar();
				}
				numCount++;

				if(tok==NULL){
				//	printf("NUM:Encountered NULL\n");
				}
				//printf("Found NUM: %s\n",tok->lexeme);
	        	strcpy(n->terminal.lexeme,tok->lexeme);

	        	fprintf(stdout,"%s\t%d\t%s\t%s\t%s\t%s\t%s\n",n->terminal.lexeme,n->terminal.lineNumber,n->terminal.token,n->terminal.lexeme,n->parent->NonTerminal,"YES","----");
                fprintf(writeFile,"%s\t%d\t%s\t%s\t%s\t%s\t%s\n",n->terminal.lexeme,n->terminal.lineNumber,n->terminal.token,n->terminal.lexeme,n->parent->NonTerminal,"YES","----");
            }
            
            else if(strcmp(n->terminal.token,"ID") == 0 ){
            	tokenInfo *tok = getNextToken2(copy);
				while(tok!=NULL && strcmp(tok->token,"ID\0")!=0){
					tok = getNextToken2(copy);
					//printf("Token(ID):%s\n",tok->lexeme);
					//getchar();
				}
				idCount++;
				if(tok==NULL){
					//printf("ID:Encountered NULL\n");
				}
				//printf("Found ID: %s\n",tok->lexeme);
	        	strcpy(n->terminal.lexeme,tok->lexeme);
                fprintf(stdout,"%s\t%d\t%s\t%s\t%s\t%s\t%s\n",n->terminal.lexeme,n->terminal.lineNumber,n->terminal.token,"NA",n->parent->NonTerminal,"YES","----");
                fprintf(writeFile,"%s\t%d\t%s\t%s\t%s\t%s\t%s\n",n->terminal.lexeme,n->terminal.lineNumber,n->terminal.token,"NA",n->parent->NonTerminal,"YES","----");
            }
            else{
            	if(strcmp(n->terminal.token,"END")==0){
            		n->terminal.lineNumber = endArray[countEnd2++];
            	}
            	fprintf(stdout,"%s\t%d\t%s\t%s\t%s\t%s\t%s\n",n->terminal.lexeme,n->terminal.lineNumber,n->terminal.token,"NA",n->parent->NonTerminal,"YES","----");
            	fprintf(writeFile,"%s\t%d\t%s\t%s\t%s\t%s\t%s\n",n->terminal.lexeme,n->terminal.lineNumber,n->terminal.token,"NA",n->parent->NonTerminal,"YES","----");
            }
        }
        else{
            fprintf(stdout,"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%d\n","----","----","----","----",n->parent->NonTerminal,"No",n->NonTerminal,n->rule);
            fprintf(writeFile,"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%d\n","----","----","----","----",n->parent->NonTerminal,"No",n->NonTerminal,n->rule);
        }
    if(c!=NULL)
    {

        while(c->sibling!=NULL)
        {
            printInorder(c->sibling,n);
            c=c->sibling;
        }
        fprintf(writeFile,"\n");
    }
}

void printStack(){
	struct stack *StackPointer = parser_stack;
	//printf("Printing Stack\n");
	while(StackPointer!=NULL){
		if(StackPointer->node->tag==0){
			printf("NonTerminal: %s\n",StackPointer->node->NonTerminal);
		}
		else{
		  	printf("Terminal: %s %s %d\n",StackPointer->node->terminal.token,StackPointer->node->terminal.lexeme,StackPointer->node->terminal.lineNumber);
		}
		StackPointer = StackPointer->next;
	}
	//printf("Ended\n");
}

void createParseTree(struct set TerminalsArr[]){
	tokenInfo* tok;
	//printf("%d\n",isEmpty());

	//push(description.ff[0].rule,0,-1);
	int counter = 0;

	//printf("Hello\n");

	root = createNode(description.ff[0].rule,0,-1,"",NULL);
    push(root);

	//printf("createNode\n");


	struct set* first = malloc(sizeof(struct set));
	struct set* temporary = malloc(sizeof(struct set));
	struct rules *ruleHolder = malloc(sizeof(struct rules));

	tok = getNextToken(fp);

	if(tok == NULL){
		printf("Random Shit Error\n");
	}

	//printf("Hello\n");

	//printf("%s %s %d\n",tok->lexeme,tok->token,tok->lineNumber);

	struct Node* currentNode ;// = malloc(sizeof(struct Node));
	currentNode = root;
	int rule_no;

	//printf("%d\n",isEmpty());

	while(!isEmpty()){

		ruleHolder = malloc(sizeof(struct rules));

        struct Node* header = currentNode;

        //printf("\n");
    	//printStack();
        //printf("%s %ld\n",tok->token,strlen(tok->token));
        //printf("\n");
		//createGrammar();
		createDerivationsCopy();
        if(strcmp(tok->token,"LEXICAL ERROR")==0){
            printf("%d\tLEXICAL ERROR\n",tok->lineNumber);
            tok = getNextToken(fp);
            continue;
        }

		if(parser_stack->node->tag==1){
			//printf("Terminal : %s, NonTerminal: %s\n",parser_stack->node->terminal.token,parser_stack->node->NonTerminal);
			if(strcmp(parser_stack->node->terminal.token,"EMPTY")==0){
               // printf("Empty Condition\n");
                //printf("%d %s %ld\n",isEmpty(),tok->token,strlen(tok->token));
                pop();
                //printf("Stack popped\n");
                //printStack();
                if(parser_stack!=NULL){
                currentNode = parser_stack->node;
            }
                //printf("Updated\n");
				continue;
			}
			if(strcmp(parser_stack->node->terminal.token,tok->token)==0){
                //printf("Terminal Check\n");
				tok = getNextToken(fp);

				if(strcmp(tok->token,"END")==0){
					endArray[countEnd++] = tok->lineNumber;
				}
				//printf("%s %s %d\n",tok->lexeme,tok->token,tok->lineNumber);

				pop();

                currentNode = parser_stack->node;
				continue;
			}
			else{
                printf("%d SYNTACTIC ERROR(1)\n",tok->lineNumber);
                noOfErrors++;
				if(!isEmpty())
					pop();
				else{
					printf("STACK EMPTY\n");
				}
                continue;
			}
		}

		// Retrieve first
		int i;
		for( i = 0; i < countNonTerminals; i++){
			if(strcmp(description.ff[i].rule,parser_stack->node->NonTerminal)==0){
				break;
			}
		}
		int j;
		for(j = 0;j < countTerminals; j++){
			if(strcmp(tok->token,TerminalsArr[j].terminal)==0){
				rule_no = parseTable[i][j];
				break;
			}
		}
		
		if(j==countTerminals || rule_no == -1){
            computeFollow();
            printf("%d SYNTACTIC ERROR(2)\n",tok->lineNumber);
            noOfErrors++;
			struct set* follow;// = malloc(sizeof(struct set));
			follow = description.ff[i].follow;
            //struct set* temp = description.ff[i].follow;
        
			int flag = 0;
			while(strcmp(tok->token,"$")!=0){
				if(contains(follow,tok->token)){
					pop();
					flag = 1;
					break;
				}
				else{
					tok = getNextToken(fp);
                    printf("%s %ld\n",tok->token,strlen(tok->token));
				}

			}
			if(flag==0){
				break;
			}
            continue;

		}
		

		// // Retrieve Rule No.
		// int rule_no;
		// temp = first;
		// while(temp!=NULL){
		// 	if(strcmp(temp->terminal,tok->token)==0){
		// 		rule_no = temp->rule_no;
		// 	}
		// }

		ruleHolder = derivations2.decode[rule_no].next;


		//printf("Rule for %s : %d\n",derivations.decode[rule_no].lhs,rule_no);

		struct rules *curr = malloc(sizeof(struct rules));
        struct Node* maintain[15];

        currentNode->rule = rule_no;
		curr = ruleHolder;
		int children = 0;
		while(curr!=NULL){
			//printf("Making Tree Node: %s %d\n",curr->lhs,children);

			if(children == 0){
                if(currentNode->tag==0){
                    //printf("Child of %s: %s\n",currentNode->NonTerminal,curr->lhs); 
                }
                else{
                   // printf("Child of %s: %s\n",currentNode->terminal.token,curr->lhs);
                }
				if(curr->lhs[0]=='<'){
					currentNode->child = createNode(curr->lhs,0,-1,"",currentNode);
                   //printf("CurrentNode:%s(%u), child:%s(%u)\n",currentNode->NonTerminal,currentNode,currentNode->child->NonTerminal,currentNode->child);
				}
				else{
					currentNode->child = createNode(curr->lhs,1,tok->lineNumber,tok->lexeme,currentNode);
                   // printf("CurrentNode:%s(%u), child:%s(%u)\n",currentNode->NonTerminal,currentNode,currentNode->child->terminal.token,currentNode->child);
				}
                //maintain[children] = currentNode;
				currentNode = currentNode->child;
			}
			else{
                if(currentNode->tag==0){
                   // printf("Sibling of %s: %s\n",currentNode->NonTerminal,curr->lhs); 
                }
                else{
                   // printf("Sibling of %s: %s\n",currentNode->terminal.token,curr->lhs);
                }
				if(curr->lhs[0]=='<'){
					currentNode->sibling = createNode(curr->lhs,0,-1,"",currentNode->parent);
                   // printf("CurrentNode:%s(%u), sibling:%s(%u)\n",currentNode->NonTerminal,currentNode,currentNode->sibling->NonTerminal,currentNode->sibling);
				}
				else{
					currentNode->sibling = createNode(curr->lhs,1,tok->lineNumber,tok->lexeme,currentNode->parent);
                 	// printf("CurrentNode:%s(%u), sibling:%s(%u)\n",currentNode->NonTerminal,currentNode,currentNode->sibling->terminal.token,currentNode->sibling);
				}
                //maintain[children] = currentNode;
				currentNode = currentNode->sibling;
			}
            maintain[children] = currentNode;
			children++;
			curr = curr->next;
		}

        //printf("\nTree,Children:%d\n",children);
        //printInorder(header);
        //printf("\n");

        /*
		struct rules* current; //= malloc(sizeof(struct rules));
		current = ruleHolder;
        struct rules *prev = NULL, *next = NULL;

        while (current != NULL) {
            next = current->next;
            current->next = prev;
            prev = current;
            current = next;
        }
        ruleHolder = prev;
        

        struct rules *temp = malloc(sizeof(struct rules));
        temp = ruleHolder;

        printf("Temp : %s\n", temp->lhs);
        */
        //currentNode = 
        struct Node* hold = pop();
        if(hold->tag==0){
          //  printf("Hold:%s\n",hold->NonTerminal);
        }
        else{
          //  printf("Hold:%s\n",hold->terminal.token);
        }

    //    printf("IN BETWEEN STACK\n");
      //  printStack();

        if(currentNode->tag == 0){
        	//printf("NonTerminal : %s\n", currentNode->NonTerminal);
        }
        else{
        //	printf("Terminal : %s\n",currentNode->terminal.token);
        }

        //printf("\n");

        /*
        while(temp!=NULL){
        	printf("Inserting in Stack: %s\n", temp->lhs);
        	if(temp->lhs[0]=='<')
	        	push(temp->lhs,0,-1);
	        else
	        	push(temp->lhs,1,tok->lineNumber);
        	temp = temp->next;
        }*/
        children--;
        while(children>=0){
            if(maintain[children]->tag==0){
              //  printf("Printing maintain:%s,%d\n",maintain[children]->NonTerminal,children);
            }
            else{
               // printf("Printing maintain:%s,%d\n",maintain[children]->terminal.token,children);
            }
            push(maintain[children]);
            children--;
        }

       // printf("Iteration Completed:%d\n\n",counter++);
        //printStack();
        //printf("Stack Printed\n");

        currentNode = parser_stack->node;
        if(currentNode->tag == 0){
          //  printf("Current Top of Stack : %s\n", currentNode->NonTerminal);
        }
        else{
           // printf("Current Top of Stack : %s\n",currentNode->terminal.token);
        }
       //  sleep(1);
	}
}
void populateTerminalsArray(struct set Terminals[]){
	struct set* temp = malloc(sizeof(struct set));
	temp = onlyTerminals;
	int i = 0;
	while(temp!=NULL){

		strcpy(Terminals[i].terminal,temp->terminal);
		Terminals[i].next = NULL;
		i++;
		temp = temp->next;
	}
}