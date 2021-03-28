#include "lexer.h"
/*
ID: 2017A7PS0136P Name: VITTHAL BHANDARI
ID: 2017A7PS0180P Name: IESHAAN SAXENA
ID: 2017A7PS0181P Name: NAMAN SINGHAL
ID: 2017A7PS0233P Name: DARSHAN AGRAWAL
ID: 2017A7PS0236P Name: ABHISHEK GUPTA
*/

int tt=SEEK_CUR;
int lineNo = 1,lineNo2 = 1;
bool eofState = false,eofState2 = false;
FILE *fp;
int noOfErrors = 0;

void removeComments(char * testcaseFile , char * cleanFile, int print){
    FILE * fp1;
    FILE * fp2;
    char c1,c2;
    int flag = 0;
    fp1 = fopen(testcaseFile,"r");
    fp2 = fopen(cleanFile,"w");

    if(fp1 == NULL){
        return;
    }
    while(1){
        c1 = getc(fp1);
        if(c1 == EOF){
            break;
        }
        if(c1=='\n'){
            fputc('\n',fp2);
        }
        else if(c1=='\t'){
            fputc('\t',fp2);
        }
        else if(c1 != '*'){
            fputc(c1,fp2);
        }
        else{
            //fputc(' ',fp2);
            c2 = fgetc(fp1);
            if(c2 == EOF){
                fputc('*',fp2);
                break;
            }
            if(c2 == '*'){
                fputc(' ',fp2);
                fputc(' ',fp2);
                while(1){
                    c1 = fgetc(fp1);
                    if(c1 == EOF){
                        flag = 1;
                        break;
                    }
                    if(c1 == '*'){
                        fputc(' ',fp2);
                        c2 = getc(fp1);
                        if(c2 == EOF){
                            flag = 1;
                            break;
                        }
                        if(c2 =='*'){
                            fputc(' ',fp2);
                            break;
                        }
                        else{
                            fputc(' ',fp2);
                        }
                    }
                    else{
                        if(c1=='\n'){
                            fputc('\n',fp2);
                        }
                        else if(c1=='\t'){
                            fputc('\t',fp2);
                        }
                        else{
                            fputc(' ',fp2);
                        }

                    }
                }
                if(flag == 1){
                    break;
                }
            }
            else{
                fputc(c1,fp2);
                fputc(c2,fp2);
            }
        }
    }
    fclose(fp1);
    fclose(fp2);
    if(print == 1){
     printCleanFile();
 }
}

void printCleanFile()
{
    FILE* cf=fopen("cleanfile.txt","r");
    char c= fgetc(cf);
    while(c!=EOF)
    {
        printf("%c",c);
        c= fgetc(cf);
    }
}


void ignoreComments()
{
    char c1;
    while(1)
    {
        c1=fgetc(fp);
        if(c1==EOF)
        {
            eofState=true;
            break;
        }
        if(c1=='*')
        {
            c1=fgetc(fp);
            if(c1==EOF)
            {
                eofState=true;
                break;
            }
            if(c1=='*')
            {
                break;
            }

        }
        if(c1=='\n')
        {
           lineNo++;
        }

    }

}

void createSymbolTable(){
    for(int i = 0; i < HASH_TABLE_SIZE ; i++){
        symbol_table[i] = NULL;
    }
}

int computeHashValue(char *name){
    int i = 0;
    int hash = 0;
    while(i < strlen(name)){
    hash += name[i];
    i++;
    }
    return hash % HASH_TABLE_SIZE;
}

tokenInfo *newNode(char *lexeme_name,char * token_name){
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

void insert(char *lexeme_name,char * token_name){
    tokenInfo *ptr;
    ptr = newNode(token_name,lexeme_name);
    int val = computeHashValue(ptr->lexeme);

    if (symbol_table[val] == NULL)
        {
            symbol_table[val] = ptr;
        }
    else
        {
            ptr->next = (tokenInfo *)symbol_table[val];
            symbol_table[val] = ptr;
        }
}

void initializeSymbolTable(){
    // Initializing Hash Table with keywords
    for(int i = 0; i <NO_OF_KEYWORDS ; i++)
        {
            insert(keywords_tokens[i],keywords[i]);

        }
}

char * receiveToken(char * lexeme){

    int symbolTableIndex = computeHashValue(lexeme);
    tokenInfo *ptr;
    ptr = symbol_table[symbolTableIndex];

    while(ptr != NULL){
        int c = strcmp(lexeme,ptr->lexeme);
        if(c == 0){
            return ptr->token;
        }
        else{
            ptr = (tokenInfo *)ptr->next;
        }
    }
    return "ID";
}

tokenInfo * getNextToken(FILE* fp)
{
    int bi = 0;
    tokenInfo *tf=(tokenInfo *)malloc(sizeof(tokenInfo));
    tf->next=NULL;
    buffer[bi]=fgetc(fp);
    bi++;
    if(buffer[bi-1]==EOF)
    {
        eofState=true;
        strcpy(tf->token,"$");
        tf->lineNumber=lineNo;
        return tf;
    }
    int con=0;
        switch(buffer[bi-1])
    {
        case '\n':
            memset(buffer,0,sizeof(buffer));
            bi=0;
            lineNo++;
            return getNextToken(fp);

        case '\r':
            memset(buffer,0,sizeof(buffer));
            bi=0;
            return getNextToken(fp);

        case '\t':
            memset(buffer,0,sizeof(buffer));
            bi=0;
            return getNextToken(fp);

        case ' ':
            memset(buffer,0,sizeof(buffer));
            bi=0;
            return getNextToken(fp);

        case '<':
            buffer[bi]=fgetc(fp);
            bi++;
            if(buffer[bi-1]=='=')
            {
                memcpy(tf->lexeme,buffer,bi);
                tf->lexeme[bi]='\0';
                tf->lineNumber=lineNo;
                strcpy(tf->token,"LE");
                memset(buffer,0,sizeof(buffer));
                bi=0;
                return tf;
            }

            else if(buffer[bi-1]=='<')
            {
                buffer[bi]=fgetc(fp);
            	bi++;
				if(buffer[bi-1]=='<')
            	{
					memcpy(tf->lexeme,buffer,bi);
	                tf->lexeme[bi]='\0';
	                tf->lineNumber=lineNo;
	                strcpy(tf->token,"DRIVERDEF");
	                memset(buffer,0,sizeof(buffer));
	                bi=0;
	                return tf;
				}
				else
				{
					memcpy(tf->lexeme,buffer,bi-1);
	                tf->lexeme[bi]='\0';
	                tf->lineNumber=lineNo;
	                strcpy(tf->token,"DEF");
	                if(buffer[bi-1]!=EOF)
                     	{
                    		fseek(fp,-1,tt);
                     	}
	                memset(buffer,0,sizeof(buffer));
	                bi=0;
	                return tf;
				}

            }

            else
            {
                memcpy(tf->lexeme,buffer,bi-1);
                tf->lexeme[bi]='\0';
                tf->lineNumber=lineNo;
                strcpy(tf->token,"LT");
                if(buffer[bi-1]!=EOF)
                {
                	fseek(fp,-1,tt);
                }
                memset(buffer,0,sizeof(buffer));
                bi=0;
                return tf;
            }


        case '>':
                buffer[bi]=fgetc(fp);
                bi++;
                if(buffer[bi-1]=='=')
                {
                    memcpy(tf->lexeme,buffer,bi);
                    tf->lexeme[bi]='\0';
                    tf->lineNumber=lineNo;
                    strcpy(tf->token,"GE");
                    memset(buffer,0,sizeof(buffer));
                    bi=0;
                    return tf;
                }

                else if(buffer[bi-1]=='>')
                {
                    buffer[bi]=fgetc(fp);
            		bi++;
					if(buffer[bi-1]=='>')
            		{
						memcpy(tf->lexeme,buffer,bi);
		                tf->lexeme[bi]='\0';
		                tf->lineNumber=lineNo;
		                strcpy(tf->token,"DRIVERENDDEF");
		                memset(buffer,0,sizeof(buffer));
		                bi=0;
		                return tf;
					}

					else
					{     
                        // CHECK LEXEME
						memcpy(tf->lexeme,buffer,bi-1);
	                    tf->lexeme[bi]='\0';
	                    tf->lineNumber=lineNo;
	                    strcpy(tf->token,"ENDDEF");
	                    if(buffer[bi-1]!=EOF)
                     	{
                    		fseek(fp,-1,tt);
                     	}
	                    memset(buffer,0,sizeof(buffer));
	                    bi=0;
	                    return tf;
                	}
                }

                else
                {
                    memcpy(tf->lexeme,buffer,bi-1);
                    tf->lexeme[bi]='\0';
                    tf->lineNumber=lineNo;
                    strcpy(tf->token,"GT");
                     if(buffer[bi-1]!=EOF)
                     {
                    	fseek(fp,-1,tt);
                     }
                    memset(buffer,0,sizeof(buffer));
                    bi=0;
                    return tf;
                }

        case '=':
            buffer[bi]=fgetc(fp);
                bi++;
                if(buffer[bi-1]=='=')
                {
                    memcpy(tf->lexeme,buffer,bi);
                    tf->lexeme[bi]='\0';
                    tf->lineNumber=lineNo;
                    strcpy(tf->token,"EQ");
                    memset(buffer,0,sizeof(buffer));
                    bi=0;
                    return tf;
                }
                else
                {
                    memcpy(tf->lexeme,buffer,bi-1);
                    tf->lexeme[bi]='\0';
                    tf->lineNumber=lineNo;

                    strcpy(tf->token,"LEXICAL ERROR");

                    if(buffer[bi-1]!=EOF)
                        fseek(fp,-1,tt);
                    memset(buffer,0,sizeof(buffer));
                    bi=0;
                    return tf;
                }
        case '!':
             buffer[bi]=fgetc(fp);
                bi++;
                if(buffer[bi-1]=='=')
                {
                    memcpy(tf->lexeme,buffer,bi);
                    tf->lexeme[bi]='\0';
                    tf->lineNumber=lineNo;
                    strcpy(tf->token,"NE");
                    memset(buffer,0,sizeof(buffer));
                    bi=0;
                    return tf;
                }
                else
                {
                    memcpy(tf->lexeme,buffer,bi-1);
                    tf->lexeme[bi]='\0';
                    tf->lineNumber=lineNo;

                    strcpy(tf->token,"LEXICAL ERROR");
                    if(buffer[bi-1]!=EOF)
                        fseek(fp,-1,tt);
                    memset(buffer,0,sizeof(buffer));
                    bi=0;
                    return tf;
                }

        case ':':
                  buffer[bi]=fgetc(fp);
                bi++;
                if(buffer[bi-1]=='=')
                {
                    memcpy(tf->lexeme,buffer,bi);
                    tf->lexeme[bi]='\0';
                    tf->lineNumber=lineNo;

                    strcpy(tf->token,"ASSIGNOP");
                    memset(buffer,0,sizeof(buffer));
                    bi=0;
                    return tf;
                }

                else
                {
                    memcpy(tf->lexeme,buffer,bi-1);
                    tf->lexeme[bi]='\0';
                    tf->lineNumber=lineNo;

                    strcpy(tf->token,"COLON");
                    if(buffer[bi-1]!=EOF)
                        fseek(fp,-1,tt);
                    memset(buffer,0,sizeof(buffer));
                    bi=0;
                    return tf;
                }



        case '.':
                 buffer[bi]=fgetc(fp);
                bi++;
                if(buffer[bi-1]=='.')
                {
                    memcpy(tf->lexeme,buffer,bi);
                    tf->lexeme[bi]='\0';
                    tf->lineNumber=lineNo;

                    strcpy(tf->token,"RANGEOP");
                    memset(buffer,0,sizeof(buffer));
                    bi=0;
                    return tf;
                }
                else
                {
                    memcpy(tf->lexeme,buffer,bi-1);
                    tf->lexeme[bi]='\0';
                    tf->lineNumber=lineNo;

                    strcpy(tf->token,"LEXICAL ERROR");
                    if(buffer[bi-1]!=EOF)
                        fseek(fp,-1,tt);
                    memset(buffer,0,sizeof(buffer));
                    bi=0;
                    return tf;
                }

        case '+':
                memcpy(tf->lexeme,buffer,bi);
                tf->lexeme[bi]='\0';
                tf->lineNumber=lineNo;
                strcpy(tf->token,"PLUS");
                memset(buffer,0,sizeof(buffer));
                bi=0;
                return tf;

        case '-':
                memcpy(tf->lexeme,buffer,bi);
                tf->lexeme[bi]='\0';
                tf->lineNumber=lineNo;

                strcpy(tf->token,"MINUS");
                memset(buffer,0,sizeof(buffer));
                bi=0;
                return tf;

        case '*':
                buffer[bi]=fgetc(fp);
                bi++;
                if(buffer[bi-1]=='*')
                {
                    tf->lineNumber=lineNo;
                   ignoreComments();

                   strcpy(tf->lexeme,"Comment");
                    strcpy(tf->token,"COMMENT");

                   memset(buffer,0,sizeof(buffer));
                    bi=0;

                  return tf;
                }

                else
                {
                    memcpy(tf->lexeme,buffer,bi-1);
                    tf->lexeme[bi]='\0';
                    tf->lineNumber=lineNo;
                    strcpy(tf->token,"MUL");
                    if(buffer[bi-1]!=EOF)
                        fseek(fp,-1,tt);
                    memset(buffer,0,sizeof(buffer));
                    bi=0;
                    return tf;
                }

        case '/':
                memcpy(tf->lexeme,buffer,bi);
                tf->lexeme[bi]='\0';
                tf->lineNumber=lineNo;
                strcpy(tf->token,"DIV");
                memset(buffer,0,sizeof(buffer));
                bi=0;
                return tf;

        case '(':
                memcpy(tf->lexeme,buffer,bi);
                tf->lexeme[bi]='\0';
                tf->lineNumber=lineNo;

                strcpy(tf->token,"BO");
                memset(buffer,0,sizeof(buffer));
                bi=0;
                return tf;
        case ')':
            memcpy(tf->lexeme,buffer,bi);
                tf->lexeme[bi]='\0';
                tf->lineNumber=lineNo;

                strcpy(tf->token,"BC");
                memset(buffer,0,sizeof(buffer));
                bi=0;
                return tf;
        case '[':
            memcpy(tf->lexeme,buffer,bi);
                tf->lexeme[bi]='\0';
                tf->lineNumber=lineNo;

                strcpy(tf->token,"SQBO");
                memset(buffer,0,sizeof(buffer));
                bi=0;
                return tf;
        case ']':
            memcpy(tf->lexeme,buffer,bi);
                tf->lexeme[bi]='\0';
                tf->lineNumber=lineNo;

                strcpy(tf->token,"SQBC");
                memset(buffer,0,sizeof(buffer));
                bi=0;
                return tf;
        case ';':
            memcpy(tf->lexeme,buffer,bi);
                tf->lexeme[bi]='\0';
                tf->lineNumber=lineNo;
                strcpy(tf->token,"SEMICOL");
                memset(buffer,0,sizeof(buffer));
                bi=0;
                return tf;
        case ',':
                memcpy(tf->lexeme,buffer,bi);
                tf->lexeme[bi]='\0';
                tf->lineNumber=lineNo;

                strcpy(tf->token,"COMMA");
                memset(buffer,0,sizeof(buffer));
                bi=0;
                return tf;

        default:
                if(isalpha(buffer[bi-1]))
                {
                    buffer[bi]=fgetc(fp);
                    bi++;
                    int i = 0;
                    while(((buffer[bi-1] >= 'A' && buffer[bi-1]<='Z') || (buffer[bi-1] >= 'a' && buffer[bi-1]<='z') || (buffer[bi-1] >= '0' && buffer[bi-1]<='9') || buffer[bi-1]=='_'))// && i<20)
                    {
                        buffer[bi]=fgetc(fp);
                        bi++;
                        i++;
                    }

                    if(i>=20)
                    {
                        strcpy(tf->lexeme,"Length Overflow");
                        strcpy(tf->token,"LEXICAL ERROR");
                        fseek(fp,-1,tt);
                        tf->lineNumber=lineNo;
                        return tf;
                    }
                    else
                    {
                        memcpy(tf->lexeme,buffer,bi-1);
                        tf->lexeme[bi-1]='\0';
                        strcpy(tf->token,receiveToken(tf->lexeme));
                        tf->lineNumber=lineNo;
                        char c = fgetc(fp);
                        if(c != EOF){
                            fseek(fp,-2,tt);
                        }
                        else if(c==EOF){

                            fseek(fp,-1,tt);
                            c = fgetc(fp);
                            if(c != buffer[bi-2]){
                                fseek(fp,-1,tt);
                            }

                            eofState = true;
                        }

                        memset(buffer,0,sizeof(buffer));
                        bi=0;
                        return tf;
                    }

                }


            else if(isdigit(buffer[bi-1]))
            {
                buffer[bi]=fgetc(fp);
                while(isdigit(buffer[bi++]))
                {
                    buffer[bi]=fgetc(fp);
                }
                if(buffer[bi-1]=='.')
                {
                    buffer[bi]=fgetc(fp);

                    if(isdigit(buffer[bi++]))
                    {
                        buffer[bi]=fgetc(fp);

                        while(isdigit(buffer[bi++]))
                        {
                            buffer[bi]=fgetc(fp);
                        }
                        if((buffer[bi-1]=='e')||(buffer[bi-1]=='E'))
                        {
                            buffer[bi++]=fgetc(fp);

                            if(buffer[bi-1]=='+'||buffer[bi-1]=='-')
                            {
                                buffer[bi]=fgetc(fp);

                                if(isdigit(buffer[bi++]))
                                {
                                    buffer[bi]=fgetc(fp);
                                    while(isdigit(buffer[bi++]))
                                    {
                                        buffer[bi]=fgetc(fp);
                                    }
                                    memcpy(tf->lexeme,buffer,bi-1);
                                    tf->lexeme[bi]='\0';
                                    tf->lineNumber=lineNo;
                                    strcpy(tf->token,"RNUM");
                                    if(buffer[bi-1]!=EOF)
                                        fseek(fp,-1,tt);
                                    memset(buffer,0,sizeof(buffer));
                                    bi=0;
                                    return tf;
                                }
                                else
                                {
                                    //check for errors
                                    memcpy(tf->lexeme,buffer,bi-3);
                                    tf->lexeme[bi]='\0';
                                    tf->lineNumber=lineNo;

                                    strcpy(tf->token,"RNUM");
                                    if(buffer[bi-1]==EOF)
                                        fseek(fp,-2,tt);
                                    else
                                        fseek(fp,-3,tt);
                                    memset(buffer,0,sizeof(buffer));
                                    bi=0;
                                    return tf;
                                }
                            }
                            else if(isdigit(buffer[bi-1]))
                            {
                                buffer[bi]=fgetc(fp);
                                while(isdigit(buffer[bi++]))
                                    {
                                        buffer[bi]=fgetc(fp);
                                    }
                                    memcpy(tf->lexeme,buffer,bi-1);
                                    tf->lexeme[bi]='\0';
                                    tf->lineNumber=lineNo;
                                    if(buffer[bi-1]!=EOF)
                                        fseek(fp,-1,tt);
                                    strcpy(tf->token,"RNUM");
                                    memset(buffer,0,sizeof(buffer));
                                    bi=0;
                                    return tf;
                            }
                            else
                            {
                                //check for error
                                memcpy(tf->lexeme,buffer,bi-2);
                                tf->lexeme[bi]='\0';
                                tf->lineNumber=lineNo;
                                if(buffer[bi-1]==EOF)
                                    fseek(fp,-1,tt);
                                else
                                    fseek(fp,-2,tt);
                                strcpy(tf->token,"RNUM");
                                memset(buffer,0,sizeof(buffer));
                                bi=0;

                                return tf;
                            }

                        }
                        else
                        {
                            memcpy(tf->lexeme,buffer,bi-1);
                            tf->lexeme[bi]='\0';
                            tf->lineNumber=lineNo;

                            strcpy(tf->token,"RNUM");
                            if(buffer[bi-1]!=EOF)
                                fseek(fp,-1,tt);
                            memset(buffer,0,sizeof(buffer));
                            bi=0;
                            return tf;
                        }
                    }
                    else
                    {
                         //check for errors
                        memcpy(tf->lexeme,buffer,bi-2);
                        tf->lexeme[bi]='\0';
                        tf->lineNumber=lineNo;

                        strcpy(tf->token,"NUM");
                        if(buffer[bi-1]!=EOF)
                            fseek(fp,-2,tt);
                        else
                            fseek(fp,-1,tt);
                        memset(buffer,0,sizeof(buffer));
                        bi=0;

                        return tf;
                    }

                }
                else
                {
                    memcpy(tf->lexeme,buffer,bi-1);
                    tf->lexeme[bi]='\0';
                    tf->lineNumber=lineNo;

                    strcpy(tf->token,"NUM");
                    if(buffer[bi-1]!=EOF)
                        fseek(fp,-1,tt);
                    memset(buffer,0,sizeof(buffer));
                    bi=0;
                    return tf;
                }
            }
            else
            {
                memcpy(tf->lexeme,buffer,bi);
                printf("%d : %d\n",buffer[bi-1],bi);
                tf->lexeme[bi]='\0';
                tf->lineNumber=lineNo;
                strcpy(tf->token,"LEXICAL ERROR");
                memset(buffer,0,sizeof(buffer));
                bi=0;
                return tf;
            }
    }
    strcpy(tf->token,"BROKEN");
    return tf;
}

void tokenCheck(FILE* fp){

	tokenInfo * tok = getNextToken(fp);
	while(tok!=NULL && strcmp(tok->token,"$")!=0){
		printf("%d\t%s\t\t%s \n",tok->lineNumber,tok->token,tok->lexeme);
		tok = getNextToken(fp);
	}
	if(tok==NULL){
		printf("Ended because of NULL\n");
	}
	else{
		printf("End of File\n");
	}
}

tokenInfo * getNextToken2(FILE* fp)
{
    int bi = 0;
    tokenInfo *tf=(tokenInfo *)malloc(sizeof(tokenInfo));
    tf->next=NULL;
    buffer2[bi]=fgetc(fp);
    bi++;
    if(buffer2[bi-1]==EOF)
    {
        eofState2=true;
        strcpy(tf->token,"$");
        tf->lineNumber=lineNo2;
        return tf;
    }
    int con=0;
        switch(buffer2[bi-1])
    {
        case '\n':
            memset(buffer2,0,sizeof(buffer2));
            bi=0;
            lineNo2++;
            return getNextToken2(fp);

        case '\r':
            memset(buffer2,0,sizeof(buffer2));
            bi=0;
            return getNextToken2(fp);

        case '\t':
            memset(buffer2,0,sizeof(buffer2));
            bi=0;
            return getNextToken2(fp);

        case ' ':
            memset(buffer2,0,sizeof(buffer2));
            bi=0;
            return getNextToken2(fp);

        case '<':
            buffer2[bi]=fgetc(fp);
            bi++;
            if(buffer2[bi-1]=='=')
            {
                memcpy(tf->lexeme,buffer2,bi);
                tf->lexeme[bi]='\0';
                tf->lineNumber=lineNo2;
                strcpy(tf->token,"LE");
                memset(buffer2,0,sizeof(buffer2));
                bi=0;
                return tf;
            }

            else if(buffer2[bi-1]=='<')
            {
                buffer2[bi]=fgetc(fp);
                bi++;
                if(buffer2[bi-1]=='<')
                {
                    memcpy(tf->lexeme,buffer2,bi);
                    tf->lexeme[bi]='\0';
                    tf->lineNumber=lineNo2;
                    strcpy(tf->token,"DRIVERDEF");
                    memset(buffer2,0,sizeof(buffer2));
                    bi=0;
                    return tf;
                }
                else
                {
                    memcpy(tf->lexeme,buffer2,bi-1);
                    tf->lexeme[bi]='\0';
                    tf->lineNumber=lineNo2;
                    strcpy(tf->token,"DEF");
                    if(buffer2[bi-1]!=EOF)
                        {
                            fseek(fp,-1,tt);
                        }
                    memset(buffer2,0,sizeof(buffer2));
                    bi=0;
                    return tf;
                }

            }

            else
            {
                memcpy(tf->lexeme,buffer2,bi-1);
                tf->lexeme[bi]='\0';
                tf->lineNumber=lineNo2;
                strcpy(tf->token,"LT");
                if(buffer2[bi-1]!=EOF)
                {
                    fseek(fp,-1,tt);
                }
                memset(buffer2,0,sizeof(buffer2));
                bi=0;
                return tf;
            }


        case '>':
                buffer2[bi]=fgetc(fp);
                bi++;
                if(buffer2[bi-1]=='=')
                {
                    memcpy(tf->lexeme,buffer2,bi);
                    tf->lexeme[bi]='\0';
                    tf->lineNumber=lineNo2;
                    strcpy(tf->token,"GE");
                    memset(buffer2,0,sizeof(buffer2));
                    bi=0;
                    return tf;
                }

                else if(buffer2[bi-1]=='>')
                {
                    buffer2[bi]=fgetc(fp);
                    bi++;
                    if(buffer2[bi-1]=='>')
                    {
                        memcpy(tf->lexeme,buffer2,bi);
                        tf->lexeme[bi]='\0';
                        tf->lineNumber=lineNo2;
                        strcpy(tf->token,"DRIVERENDDEF");
                        memset(buffer2,0,sizeof(buffer2));
                        bi=0;
                        return tf;
                    }

                    else
                    {     
                        // CHECK LEXEME
                        memcpy(tf->lexeme,buffer2,bi-1);
                        tf->lexeme[bi]='\0';
                        tf->lineNumber=lineNo2;
                        strcpy(tf->token,"ENDDEF");
                        if(buffer2[bi-1]!=EOF)
                        {
                            fseek(fp,-1,tt);
                        }
                        memset(buffer2,0,sizeof(buffer2));
                        bi=0;
                        return tf;
                    }
                }

                else
                {
                    memcpy(tf->lexeme,buffer2,bi-1);
                    tf->lexeme[bi]='\0';
                    tf->lineNumber=lineNo2;
                    strcpy(tf->token,"GT");
                     if(buffer2[bi-1]!=EOF)
                     {
                        fseek(fp,-1,tt);
                     }
                    memset(buffer2,0,sizeof(buffer2));
                    bi=0;
                    return tf;
                }

        case '=':
            buffer2[bi]=fgetc(fp);
                bi++;
                if(buffer2[bi-1]=='=')
                {
                    memcpy(tf->lexeme,buffer2,bi);
                    tf->lexeme[bi]='\0';
                    tf->lineNumber=lineNo2;
                    strcpy(tf->token,"EQ");
                    memset(buffer2,0,sizeof(buffer2));
                    bi=0;
                    return tf;
                }
                else
                {
                    memcpy(tf->lexeme,buffer2,bi-1);
                    tf->lexeme[bi]='\0';
                    tf->lineNumber=lineNo2;

                    strcpy(tf->token,"LEXICAL ERROR");

                    if(buffer2[bi-1]!=EOF)
                        fseek(fp,-1,tt);
                    memset(buffer2,0,sizeof(buffer2));
                    bi=0;
                    return tf;
                }
        case '!':
             buffer2[bi]=fgetc(fp);
                bi++;
                if(buffer2[bi-1]=='=')
                {
                    memcpy(tf->lexeme,buffer2,bi);
                    tf->lexeme[bi]='\0';
                    tf->lineNumber=lineNo2;
                    strcpy(tf->token,"NE");
                    memset(buffer2,0,sizeof(buffer2));
                    bi=0;
                    return tf;
                }
                else
                {
                    memcpy(tf->lexeme,buffer2,bi-1);
                    tf->lexeme[bi]='\0';
                    tf->lineNumber=lineNo2;

                    strcpy(tf->token,"LEXICAL ERROR");
                    if(buffer2[bi-1]!=EOF)
                        fseek(fp,-1,tt);
                    memset(buffer2,0,sizeof(buffer2));
                    bi=0;
                    return tf;
                }

        case ':':
                  buffer2[bi]=fgetc(fp);
                bi++;
                if(buffer2[bi-1]=='=')
                {
                    memcpy(tf->lexeme,buffer2,bi);
                    tf->lexeme[bi]='\0';
                    tf->lineNumber=lineNo2;

                    strcpy(tf->token,"ASSIGNOP");
                    memset(buffer2,0,sizeof(buffer2));
                    bi=0;
                    return tf;
                }

                else
                {
                    memcpy(tf->lexeme,buffer2,bi-1);
                    tf->lexeme[bi]='\0';
                    tf->lineNumber=lineNo2;

                    strcpy(tf->token,"COLON");
                    if(buffer2[bi-1]!=EOF)
                        fseek(fp,-1,tt);
                    memset(buffer2,0,sizeof(buffer2));
                    bi=0;
                    return tf;
                }



        case '.':
                 buffer2[bi]=fgetc(fp);
                bi++;
                if(buffer2[bi-1]=='.')
                {
                    memcpy(tf->lexeme,buffer2,bi);
                    tf->lexeme[bi]='\0';
                    tf->lineNumber=lineNo2;

                    strcpy(tf->token,"RANGEOP");
                    memset(buffer2,0,sizeof(buffer2));
                    bi=0;
                    return tf;
                }
                else
                {
                    memcpy(tf->lexeme,buffer2,bi-1);
                    tf->lexeme[bi]='\0';
                    tf->lineNumber=lineNo2;

                    strcpy(tf->token,"LEXICAL ERROR");
                    if(buffer2[bi-1]!=EOF)
                        fseek(fp,-1,tt);
                    memset(buffer2,0,sizeof(buffer2));
                    bi=0;
                    return tf;
                }

        case '+':
                memcpy(tf->lexeme,buffer2,bi);
                tf->lexeme[bi]='\0';
                tf->lineNumber=lineNo2;
                strcpy(tf->token,"PLUS");
                memset(buffer2,0,sizeof(buffer2));
                bi=0;
                return tf;

        case '-':
                memcpy(tf->lexeme,buffer2,bi);
                tf->lexeme[bi]='\0';
                tf->lineNumber=lineNo2;

                strcpy(tf->token,"MINUS");
                memset(buffer2,0,sizeof(buffer2));
                bi=0;
                return tf;

        case '*':
                buffer2[bi]=fgetc(fp);
                bi++;
                if(buffer2[bi-1]=='*')
                {
                    tf->lineNumber=lineNo2;
                   ignoreComments();

                   strcpy(tf->lexeme,"Comment");
                    strcpy(tf->token,"COMMENT");

                   memset(buffer2,0,sizeof(buffer2));
                    bi=0;

                  return tf;
                }

                else
                {
                    memcpy(tf->lexeme,buffer2,bi-1);
                    tf->lexeme[bi]='\0';
                    tf->lineNumber=lineNo2;
                    strcpy(tf->token,"MUL");
                    if(buffer2[bi-1]!=EOF)
                        fseek(fp,-1,tt);
                    memset(buffer2,0,sizeof(buffer2));
                    bi=0;
                    return tf;
                }

        case '/':
                memcpy(tf->lexeme,buffer2,bi);
                tf->lexeme[bi]='\0';
                tf->lineNumber=lineNo2;
                strcpy(tf->token,"DIV");
                memset(buffer2,0,sizeof(buffer2));
                bi=0;
                return tf;

        case '(':
                memcpy(tf->lexeme,buffer2,bi);
                tf->lexeme[bi]='\0';
                tf->lineNumber=lineNo2;

                strcpy(tf->token,"BO");
                memset(buffer2,0,sizeof(buffer2));
                bi=0;
                return tf;
        case ')':
            memcpy(tf->lexeme,buffer2,bi);
                tf->lexeme[bi]='\0';
                tf->lineNumber=lineNo2;

                strcpy(tf->token,"BC");
                memset(buffer2,0,sizeof(buffer2));
                bi=0;
                return tf;
        case '[':
            memcpy(tf->lexeme,buffer2,bi);
                tf->lexeme[bi]='\0';
                tf->lineNumber=lineNo2;

                strcpy(tf->token,"SQBO");
                memset(buffer2,0,sizeof(buffer2));
                bi=0;
                return tf;
        case ']':
            memcpy(tf->lexeme,buffer2,bi);
                tf->lexeme[bi]='\0';
                tf->lineNumber=lineNo2;

                strcpy(tf->token,"SQBC");
                memset(buffer2,0,sizeof(buffer2));
                bi=0;
                return tf;
        case ';':
            memcpy(tf->lexeme,buffer2,bi);
                tf->lexeme[bi]='\0';
                tf->lineNumber=lineNo2;
                strcpy(tf->token,"SEMICOL");
                memset(buffer2,0,sizeof(buffer2));
                bi=0;
                return tf;
        case ',':
                memcpy(tf->lexeme,buffer2,bi);
                tf->lexeme[bi]='\0';
                tf->lineNumber=lineNo2;

                strcpy(tf->token,"COMMA");
                memset(buffer2,0,sizeof(buffer2));
                bi=0;
                return tf;

        default:
                if(isalpha(buffer2[bi-1]))
                {
                    buffer2[bi]=fgetc(fp);
                    bi++;
                    int i = 0;
                    while(((buffer2[bi-1] >= 'A' && buffer2[bi-1]<='Z') || (buffer2[bi-1] >= 'a' && buffer2[bi-1]<='z') || (buffer2[bi-1] >= '0' && buffer2[bi-1]<='9') || buffer2[bi-1]=='_'))// && i<20)
                    {
                        buffer2[bi]=fgetc(fp);
                        bi++;
                        i++;
                    }

                    if(i>=20)
                    {
                        strcpy(tf->lexeme,"Length Overflow");
                        strcpy(tf->token,"LEXICAL ERROR");
                        fseek(fp,-1,tt);
                        tf->lineNumber=lineNo2;
                        return tf;
                    }
                    else
                    {
                        memcpy(tf->lexeme,buffer2,bi-1);
                        tf->lexeme[bi-1]='\0';
                        strcpy(tf->token,receiveToken(tf->lexeme));
                        tf->lineNumber=lineNo2;
                        char c = fgetc(fp);
                        if(c != EOF){
                            fseek(fp,-2,tt);
                        }
                        else if(c==EOF){

                            fseek(fp,-1,tt);
                            c = fgetc(fp);
                            if(c != buffer2[bi-2]){
                                fseek(fp,-1,tt);
                            }

                            eofState2 = true;
                        }

                        memset(buffer2,0,sizeof(buffer2));
                        bi=0;
                        return tf;
                    }

                }


            else if(isdigit(buffer2[bi-1]))
            {
                buffer2[bi]=fgetc(fp);
                while(isdigit(buffer2[bi++]))
                {
                    buffer2[bi]=fgetc(fp);
                }
                if(buffer2[bi-1]=='.')
                {
                    buffer2[bi]=fgetc(fp);

                    if(isdigit(buffer2[bi++]))
                    {
                        buffer2[bi]=fgetc(fp);

                        while(isdigit(buffer2[bi++]))
                        {
                            buffer2[bi]=fgetc(fp);
                        }
                        if((buffer2[bi-1]=='e')||(buffer2[bi-1]=='E'))
                        {
                            buffer2[bi++]=fgetc(fp);

                            if(buffer2[bi-1]=='+'||buffer2[bi-1]=='-')
                            {
                                buffer2[bi]=fgetc(fp);

                                if(isdigit(buffer2[bi++]))
                                {
                                    buffer2[bi]=fgetc(fp);
                                    while(isdigit(buffer2[bi++]))
                                    {
                                        buffer2[bi]=fgetc(fp);
                                    }
                                    memcpy(tf->lexeme,buffer2,bi-1);
                                    tf->lexeme[bi]='\0';
                                    tf->lineNumber=lineNo2;
                                    strcpy(tf->token,"RNUM");
                                    if(buffer2[bi-1]!=EOF)
                                        fseek(fp,-1,tt);
                                    memset(buffer2,0,sizeof(buffer2));
                                    bi=0;
                                    return tf;
                                }
                                else
                                {
                                    //check for errors
                                    memcpy(tf->lexeme,buffer2,bi-3);
                                    tf->lexeme[bi]='\0';
                                    tf->lineNumber=lineNo2;

                                    strcpy(tf->token,"RNUM");
                                    if(buffer2[bi-1]==EOF)
                                        fseek(fp,-2,tt);
                                    else
                                        fseek(fp,-3,tt);
                                    memset(buffer2,0,sizeof(buffer2));
                                    bi=0;
                                    return tf;
                                }
                            }
                            else if(isdigit(buffer2[bi-1]))
                            {
                                buffer2[bi]=fgetc(fp);
                                while(isdigit(buffer2[bi++]))
                                    {
                                        buffer2[bi]=fgetc(fp);
                                    }
                                    memcpy(tf->lexeme,buffer2,bi-1);
                                    tf->lexeme[bi]='\0';
                                    tf->lineNumber=lineNo2;
                                    if(buffer2[bi-1]!=EOF)
                                        fseek(fp,-1,tt);
                                    strcpy(tf->token,"RNUM");
                                    memset(buffer2,0,sizeof(buffer2));
                                    bi=0;
                                    return tf;
                            }
                            else
                            {
                                //check for error
                                memcpy(tf->lexeme,buffer2,bi-2);
                                tf->lexeme[bi]='\0';
                                tf->lineNumber=lineNo2;
                                if(buffer2[bi-1]==EOF)
                                    fseek(fp,-1,tt);
                                else
                                    fseek(fp,-2,tt);
                                strcpy(tf->token,"RNUM");
                                memset(buffer2,0,sizeof(buffer2));
                                bi=0;

                                return tf;
                            }

                        }
                        else
                        {
                            memcpy(tf->lexeme,buffer2,bi-1);
                            tf->lexeme[bi]='\0';
                            tf->lineNumber=lineNo2;

                            strcpy(tf->token,"RNUM");
                            if(buffer2[bi-1]!=EOF)
                                fseek(fp,-1,tt);
                            memset(buffer2,0,sizeof(buffer2));
                            bi=0;
                            return tf;
                        }
                    }
                    else
                    {
                         //check for errors
                        memcpy(tf->lexeme,buffer2,bi-2);
                        tf->lexeme[bi]='\0';
                        tf->lineNumber=lineNo2;

                        strcpy(tf->token,"NUM");
                        if(buffer2[bi-1]!=EOF)
                            fseek(fp,-2,tt);
                        else
                            fseek(fp,-1,tt);
                        memset(buffer2,0,sizeof(buffer2));
                        bi=0;

                        return tf;
                    }

                }
                else
                {
                    memcpy(tf->lexeme,buffer2,bi-1);
                    tf->lexeme[bi]='\0';
                    tf->lineNumber=lineNo2;

                    strcpy(tf->token,"NUM");
                    if(buffer2[bi-1]!=EOF)
                        fseek(fp,-1,tt);
                    memset(buffer2,0,sizeof(buffer2));
                    bi=0;
                    return tf;
                }
            }
            else
            {
                memcpy(tf->lexeme,buffer2,bi);
                printf("%d : %d\n",buffer2[bi-1],bi);
                tf->lexeme[bi]='\0';
                tf->lineNumber=lineNo2;
                strcpy(tf->token,"LEXICAL ERROR");
                memset(buffer2,0,sizeof(buffer2));
                bi=0;
                return tf;
            }
    }
    strcpy(tf->token,"BROKEN");
    return tf;
}

/*
void main(){

    removeComments("testfile.txt","cleanfile.txt");
   
}
*/