#include "ast.h"

int astNodeCount = 0;

struct astNode* makeNode(struct Node * ptnode){
	struct astNode *node = (struct astNode *)malloc(sizeof(struct astNode));
	node->tag = ptnode->tag;
	strcpy(node->NonTerminal,ptnode->NonTerminal);
	node->terminal = ptnode->terminal;
	node->parent = NULL;
	node->child = NULL;
	node->sibling = NULL;
	node->rule = ptnode->rule;
	strcpy(node->scope,ptnode->scope);
	//printf("scope(ast) : %s\n", node->scope);
	astNodeCount++;
	return node;
}

int NumberOfastNode(){
	return astNodeCount;
}

struct astNode *jointToRoot(struct astNode *root, struct astNode *arr[], int len){
	int j = 0;
	while(j<len && arr[j]==NULL)
		j++;
	// printf("JOINNODES FIRST NON NULL: j = %d\n",j);
	root->child = arr[j];
	struct astNode *itr;
	for(int i = j; i < len; i++)
	{
		if(i==j)
			itr = arr[i];
		else if(arr[i]!=NULL)
		{
			itr->sibling = arr[i];
			itr = itr->sibling;	// itr = arr[i]
		}
		else
			continue;
		while(itr->sibling!=NULL)
		{
			itr->parent = root;
			itr = itr->sibling;
		}
		itr->parent = root;
	}
	return root;
}

struct astNode *nodeJoin(struct astNode *arr[], int len){
	int j = 0;
	while(j<len && arr[j]==NULL)
		j++;
	// printf("JOINNODES w/o ROOT FIRST NON NULL: j = %d\n",j);
	struct astNode *first = arr[j];
	struct astNode *itr;
	for(int i=j; i<len; i++)
	{
		if(i==j)
			itr = arr[i];
		else if(arr[i]!=NULL)
		{
			itr->sibling = arr[i];
			itr = itr->sibling;
		}
		else 
			continue;
		while(itr->sibling!=NULL)
			itr = itr->sibling;
	}
	return first;
}


struct astNode *ast_generate(struct Node *root){
	if(root==NULL)
		return NULL;
	if(root->child==NULL)
		return NULL;		
	int rule = root->rule + 1;
	
	if(rule==1)		// <program>	:	<moduleDeclarations> <otherModules> <driverModule> <otherModules>
	{
		struct astNode *program = makeNode(root);
		struct astNode **arr = (struct astNode **)malloc(4*sizeof(struct astNode *));
		arr[0] = ast_generate(root->child);
		arr[1] = ast_generate(root->child->sibling);
		arr[2] = ast_generate(root->child->sibling->sibling);
		arr[3] = ast_generate(root->child->sibling->sibling->sibling);
		return jointToRoot(program, arr, 4);
	}
	else if(rule==3 || rule==6 || rule==10 || rule==13 || rule==16 || rule==27 || rule==43 ||rule==55 || rule==58 || rule==62 || rule==65 || rule==68 || rule==71 || rule==94 || rule==99){
		// For Empty
		return NULL;
	}
	else if(rule==35 || (rule>=28 && rule<=32) || rule==36 || rule==44 || rule==45 || rule==47 || rule==48 || rule==60 || rule==73 || rule==77){
		// Ex:- <var>	:	<var_id_num>
		return ast_generate(root->child);
	}
	else if(rule==2 || rule==5 || rule==26 || rule==59 || rule==63 || rule==64 || rule==66 || rule==69){
		// Ex:- <moduleDeclarations>	:	<moduleDeclaration> <moduleDeclarations>
		struct astNode **arr = (struct astNode **)malloc(2*sizeof(struct astNode *));
		arr[0] = ast_generate(root->child);
		arr[1] = ast_generate(root->child->sibling);
		return nodeJoin(arr,2);
	}
	else if(rule==61 || rule==67 || rule==70){
		// Ex:- <expression*>	:	<logicalOp> <booleanExpr> <expression*>
		struct astNode **arr = (struct astNode **)malloc(3*sizeof(struct astNode *));
		arr[0] = ast_generate(root->child);
		arr[1] = ast_generate(root->child->sibling);
		arr[2] = ast_generate(root->child->sibling->sibling);
		return nodeJoin(arr,3);
	}
	else if((rule>=17 && rule<=19) || rule==51 || (rule>=78 && rule<=89) || rule==52 || rule==22 || rule==23 || rule==24 || rule==95 || (rule>=38 && rule<=41) || rule==96 || rule==97){
		// Ex:- <type>	:	INTEGER
		return makeNode(root->child);
	}
	else if(rule==11 || rule==14){	
		// Ex:- <input_plist>	:	ID COLON <dataType> <input_plist*>
		struct astNode *input = makeNode(root);
		struct astNode **arr = (struct astNode **)malloc(3*sizeof(struct astNode *));
		struct Node *temp = root->child;	// ID
		arr[0] = makeNode(temp);
		temp = temp->sibling->sibling;	// <dataType>
		arr[1] = ast_generate(temp);
		temp = temp->sibling;			//<input_plist*>
		arr[2] = ast_generate(temp);
		return jointToRoot(input, arr, 3);
	}
	else if(rule==90){	
		// Ex:- <declareStmt>	:	DECLARE <idList> COLON <dataType> SEMICOL
		struct astNode *declStmt = makeNode(root);
		struct astNode **arr = (struct astNode **)malloc(2*sizeof(struct astNode *));
		arr[0] = ast_generate(root->child->sibling);
		arr[1] = ast_generate(root->child->sibling->sibling->sibling);
		return jointToRoot(declStmt, arr, 2);
	}
	else if(rule==46 || rule==37 || rule==56 || rule==74 || rule==75){ 
		// Ex:- <assignmentStmt>	:	ID <whichStmt>
		struct astNode *assignStmt = makeNode(root);
		struct astNode **arr = (struct astNode **)malloc(2*sizeof(struct astNode *));
		arr[0] = makeNode(root->child);
		arr[1] = ast_generate(root->child->sibling);
		return jointToRoot(assignStmt, arr, 2);
	}
	else if(rule==8){	
		//Ex:- <module>	:	DEF MODULE ID ENDDEF TAKES INPUT SQBO <input_plist> SQBC SEMICOL <ret> <moduleDef>
		struct astNode *modl = makeNode(root);
		struct astNode **arr = (struct astNode **)malloc(4*sizeof(struct astNode *));
		struct Node *temp = root->child->sibling->sibling;	// ID
		arr[0] = makeNode(temp);
		temp = temp->sibling->sibling->sibling->sibling->sibling;	// <input_plist>
		arr[1] = ast_generate(temp);
		temp = temp->sibling->sibling->sibling;	// <ret>
		arr[2] = ast_generate(temp);
		temp = temp->sibling;	// <moduleDef>
		arr[3] = ast_generate(temp);
		return jointToRoot(modl, arr, 4);
	}
	else if(rule==25 || rule==49 || rule==54){ 
		// Ex:- <moduleDef>	:	START <statements> END
		struct astNode *md = makeNode(root);
		struct astNode **arr = (struct astNode **)malloc(1*sizeof(struct astNode *));
		arr[0] = ast_generate(root->child->sibling);	// <statements>
		return jointToRoot(md, arr, 1);
	}
	else if(rule==72 || rule==76 || rule==42){
		// Ex:- <factor>	:	BO <expression> BC
		return ast_generate(root->child->sibling);
	}
	else if(rule==57){	
		//Ex:-	<idList*>	:	COMMA ID <idList*>
		struct astNode **arr = (struct astNode **)malloc(2*sizeof(struct astNode *));
		arr[0] = makeNode(root->child->sibling);
		arr[1] = ast_generate(root->child->sibling->sibling);
		return nodeJoin(arr, 2);
	}
	else if(rule==21){	
		//Ex:- <range_arrays>	:	<index> RANGEOP <index>
		struct astNode *rangearr = makeNode(root);
		struct astNode **arr = (struct astNode **)malloc(2*sizeof(struct astNode *));
		arr[0] = ast_generate(root->child);	// <index>
		arr[1] = ast_generate(root->child->sibling->sibling);	// <index>
		return jointToRoot(rangearr, arr, 2);
	}
	else if(rule==102){	
		//Ex:- <range>	:	NUM RANGEOP NUM
		struct astNode *rang = makeNode(root);
		struct astNode **arr = (struct astNode **)malloc(2*sizeof(struct astNode *));
		arr[0] = makeNode(root->child);	// NUM
		arr[1] = makeNode(root->child->sibling->sibling);	// NUM
		return jointToRoot(rang, arr, 2);
	}
	else if(rule==20){	
		//Ex:- <dataType>	:	ARRAY SQBO <range_arrays> SQBC OF <type>
		struct astNode *datatype = makeNode(root);
		struct astNode **arr = (struct astNode **)malloc(2*sizeof(struct astNode *));
		arr[0] = ast_generate(root->child->sibling->sibling);
		arr[1] = ast_generate(root->child->sibling->sibling->sibling->sibling->sibling);
		return jointToRoot(datatype, arr, 2);
	}
	else if(rule==50){	
		//Ex:- <lvalueARRStmt>	:	SQBO <index> SQBC ASSIGNOP <expression> SEMICOL
		struct astNode *arrstmt = makeNode(root);
		struct astNode **arr = (struct astNode **)malloc(2*sizeof(struct astNode *));
		arr[0] = ast_generate(root->child->sibling);	// <index>
		arr[1] = ast_generate(root->child->sibling->sibling->sibling->sibling);	// <expression>
		return jointToRoot(arrstmt, arr, 2);
	}
	else if(rule==101){	
		//Ex:- <iterativeStmt>	:	WHILE BO <expression> BC START <statements> END
		struct astNode *iterativeStmt = makeNode(root);
		struct astNode **arr = (struct astNode **)malloc(2*sizeof(struct astNode *));
		arr[0] = ast_generate(root->child->sibling->sibling);	// <expression>
		arr[1] = ast_generate(root->child->sibling->sibling->sibling->sibling->sibling);	// <statements>
		return jointToRoot(iterativeStmt, arr, 2);
	}
	else if(rule==91){	
		//Ex:-  <condionalStmt>	:	SWITCH BO ID BC START <caseStmt> <default> END
		struct astNode *condStmt = makeNode(root);
		struct astNode **arr = (struct astNode **)malloc(3*sizeof(struct astNode *));
		struct Node *temp = root->child->sibling->sibling;	// ID
		arr[0] = makeNode(temp);
		temp = temp->sibling->sibling->sibling;	// <caseStmt>
		arr[1] = ast_generate(temp);
		temp = temp->sibling;	// <default>
		arr[2] = ast_generate(temp);
		return jointToRoot(condStmt, arr, 3);
	}
	else if(rule==53){
		//Ex:- <moduleReuseStmt>	:	<optional> USE MODULE ID WITH PARAMETERS <idList> SEMICOL
		struct astNode *mrs = makeNode(root);
		struct astNode **arr = (struct astNode **)malloc(3*sizeof(struct astNode *));
		struct Node *temp = root->child;	// <optional>
		arr[0] = ast_generate(temp);
		temp = temp->sibling->sibling->sibling;	// ID
		arr[1] = makeNode(temp);
		temp = temp->sibling->sibling->sibling;	// <idlist>
		arr[2] = ast_generate(temp);
		return jointToRoot(mrs, arr, 3);
	}
	else if(rule==92 || rule==93){
		//Ex:-  <caseStmt>	:	CASE <value> COLON <statements> BREAK SEMICOL <caseStmt*>
		struct astNode *caseStmt = makeNode(root);
		struct astNode **arr = (struct astNode **)malloc(3*sizeof(struct astNode *));
		struct Node *temp = root->child->sibling;	// <value>
		arr[0] = ast_generate(temp);
		temp = temp->sibling->sibling;	// <statements>
		arr[1] = ast_generate(temp);
		temp = temp->sibling->sibling->sibling;	// <caseStmt*>
		arr[2] = ast_generate(temp);
		return jointToRoot(caseStmt, arr, 3);
	}
	else if(rule==100){
		//Ex:- <iterativeStmt>	:	FOR BO ID IN <range> BC START <statements> END
		struct astNode *itrstmt = makeNode(root);
		struct astNode **arr = (struct astNode **)malloc(3*sizeof(struct astNode *));
		struct Node *temp = root->child->sibling->sibling;	// ID
		arr[0] = makeNode(temp);
		temp = temp->sibling->sibling;	// <range>
		arr[1] = ast_generate(temp);
		temp = temp->sibling->sibling->sibling;	// <statements>
		arr[2] = ast_generate(temp);
		return jointToRoot(itrstmt, arr, 3);
	}
	else if(rule==7){
		//Ex:- <driverModule>	:	DRIVERDEF DRIVER PROGRAM DRIVERENDDEF <moduleDef>
		struct astNode *dm = makeNode(root);
		struct astNode **arr = (struct astNode **)malloc(1*sizeof(struct astNode *));
		arr[0] = ast_generate(root->child->sibling->sibling->sibling->sibling);
		return jointToRoot(dm, arr, 1);
	}
	else if(rule==33 || rule==4){
		//Ex:- <moduleDeclaration>	:	DECLARE MODULE ID SEMICOL
		//Ex:- <ioStmt>	:	GET_VALUE BO ID BC SEMICOL
		struct astNode *mdn = makeNode(root);
		struct astNode **arr = (struct astNode **)malloc(1*sizeof(struct astNode *));
		arr[0] = makeNode(root->child->sibling->sibling);	// ID
		return jointToRoot(mdn, arr, 1);
	}
	else if(rule==34 || rule==9 || rule==98){
		//Ex:- <ret>	:	RETURNS SQBO <output_plist> SQBC SEMICOL
		struct astNode *ret = makeNode(root);
		struct astNode **arr = (struct astNode **)malloc(1*sizeof(struct astNode *));
		arr[0] = ast_generate(root->child->sibling->sibling);
		return jointToRoot(ret, arr, 1);
	}
	else if(rule==12 || rule==15){	
		//Ex:- <input_plist*>	:	COMMA ID COLON <dataType> <input_plist*>
		struct astNode *input = makeNode(root);
		struct astNode **arr = (struct astNode **)malloc(3*sizeof(struct astNode *));
		struct Node *temp = root->child->sibling;	// ID
		arr[0] = makeNode(temp);
		temp = temp->sibling->sibling;	// <dataType> 
		arr[1] = ast_generate(temp);
		temp = temp->sibling;
		arr[2] = ast_generate(temp);
		return jointToRoot(input, arr, 3);
	}
}


inline void printAstNode(struct astNode *root)
{
	if(root->tag!=0)
		printf("AST Node: %d %s %s\n",root->tag,root->terminal.lexeme, root->terminal.token);
	else
		printf("AST Node: %d %s NONTERM\n",root->tag,root->NonTerminal);
}


/*
void printAstTree(struct astNode * n,struct astNode* parent)
{
    if(n==NULL)
    	return;
	if(parent == NULL){
		printf("***********INORDER TRAVERSAL*************\n");
	}
    struct astNode* c = n->child;
    printAstTree(c,n);

        if(n->tag==1){
            if(strcmp(n->terminal.token,"RNUM") == 0 || strcmp(n->terminal.token,"NUM") == 0){
                printf("%s\t%d\t%s\t%s\t%s\t%s\t%s\n",n->terminal.lexeme,n->terminal.lineNumber,n->terminal.token,n->terminal.lexeme,n->parent->NonTerminal,"YES","----");
           	    //fprintf(writeAST,"%s\t%d\t%s\t%s\t%s\t%s\t%s\n",n->terminal.lexeme,n->terminal.lineNumber,n->terminal.token,n->terminal.lexeme,n->parent->NonTerminal,"YES","----");
            
            }
            
            else{
            	printf("%s\t%d\t%s\t%s\t%s\t%s\t%s\n",n->terminal.lexeme,n->terminal.lineNumber,n->terminal.token,"NA",n->parent->NonTerminal,"YES","----");
                //fprintf(writeAST,"%s\t%d\t%s\t%s\t%s\t%s\t%s\n",n->terminal.lexeme,n->terminal.lineNumber,n->terminal.token,"NA",n->parent->NonTerminal,"YES","----");
            }
        }
        else{
        	printf("%s\t%s\t%s\t%s\t%s\t%s\t%s\n","----","----","----","----",n->parent->NonTerminal,"No",n->NonTerminal);
			//fprintf(writeAST,"%s\t%s\t%s\t%s\t%s\t%s\t%s\n","----","----","----","----",n->parent->NonTerminal,"No",n->NonTerminal);
        }
    if(c!=NULL){
        while(c->sibling!=NULL){
            printAstTree(c->sibling,n);
            c = c->sibling;
        }
        printf("\n");
    }
    return;
}
*/ 
void printAstTree(struct astNode * n,struct astNode* parent)
{
    if(n==NULL)
    return;
	if(parent == NULL){
		fprintf(writeAST, "***********INORDER TRAVERSAL*************\n\n");
	}
    struct astNode* c = n->child;
    printAstTree(c,n);

        if(n->tag==1){
            if(strcmp(n->terminal.token,"RNUM") == 0 || strcmp(n->terminal.token,"NUM") == 0){
                if(parent==NULL) {
                	fprintf(stdout, "%s\t%d\t%s\t%s\t%s\t%s\t%s\n",n->terminal.lexeme,n->terminal.lineNumber,n->terminal.token,n->terminal.lexeme,"----","YES","----");

                	fprintf(writeAST, "%s\t%d\t%s\t%s\t%s\t%s\t%s\n",n->terminal.lexeme,n->terminal.lineNumber,n->terminal.token,n->terminal.lexeme,"----","YES","----");
                }
                else{ 
                	fprintf(stdout, "%s\t%d\t%s\t%s\t%s\t%s\t%s\n",n->terminal.lexeme,n->terminal.lineNumber,n->terminal.token,n->terminal.lexeme,n->parent->NonTerminal,"YES","----");
                	
                	fprintf(writeAST, "%s\t%d\t%s\t%s\t%s\t%s\t%s\n",n->terminal.lexeme,n->terminal.lineNumber,n->terminal.token,n->terminal.lexeme,n->parent->NonTerminal,"YES","----");
                }
            }
            else{
                if(parent==NULL){ 
                 	fprintf(stdout, "%s\t%d\t%s\t%s\t%s\t%s\t%s\n",n->terminal.lexeme,n->terminal.lineNumber,n->terminal.token,"NA","----","YES","----");
               	
                	fprintf(writeAST, "%s\t%d\t%s\t%s\t%s\t%s\t%s\n",n->terminal.lexeme,n->terminal.lineNumber,n->terminal.token,"NA","----","YES","----");
                }
                else{
                	fprintf(stdout, "%s\t%d\t%s\t%s\t%s\t%s\t%s\n",n->terminal.lexeme,n->terminal.lineNumber,n->terminal.token,"NA",n->parent->NonTerminal,"YES","----");

                	fprintf(writeAST, "%s\t%d\t%s\t%s\t%s\t%s\t%s\n",n->terminal.lexeme,n->terminal.lineNumber,n->terminal.token,"NA",n->parent->NonTerminal,"YES","----");
            	}
            }
        }
        else{
            if(parent==NULL) {
            	fprintf(stdout, "%s\t%s\t%s\t%s\t%s\t%s\t%s\n","----","----","----","----","----","No",n->NonTerminal);

            	fprintf(writeAST, "%s\t%s\t%s\t%s\t%s\t%s\t%s\n","----","----","----","----","----","No",n->NonTerminal);
            }
            else{ 
            	fprintf(stdout, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%d\n","----","----","----","----",n->parent->NonTerminal,"No",n->NonTerminal, n->rule);

            	fprintf(writeAST, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%d\n","----","----","----","----",n->parent->NonTerminal,"No",n->NonTerminal, n->rule);
			}
        }
    if(c!=NULL){
        while(c->sibling!=NULL){
        //	printf("\t%s : sibling\n",n->NonTerminal);
            printAstTree(c->sibling,n);
            c = c->sibling;
        }
        //  if(parent!=NULL)
        //  {
        //  	printf("Done : %s, parent : %s\n",n->NonTerminal,parent->NonTerminal);
        //  }
        //  else{
        // 	printf("Done : %s\n",n->NonTerminal);
        // }
    }
    return;
}

