#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include "typecheck.h"

/*
ID: 2017A7PS0136P Name: VITTHAL BHANDARI
ID: 2017A7PS0180P Name: IESHAAN SAXENA
ID: 2017A7PS0181P Name: NAMAN SINGHAL
ID: 2017A7PS0233P Name: DARSHAN AGRAWAL
ID: 2017A7PS0236P Name: ABHISHEK GUPTA
*/


void createCopy(char *f1,char *f2){
    FILE *fptr1 = fopen(f1,"r");
    FILE *fptr2 = fopen(f2,"w");
    if(fptr1==NULL && fptr2==NULL){
        printf("File does not exist\n");
        exit(0);
    }
    char c = fgetc(fptr1);
    while(c!=EOF){
        fputc(c,fptr2);
        c = fgetc(fptr1);
    }
    fclose(fptr1);
    fclose(fptr2);
    return;
}
//#include "lexerDef.h"
int main(int argc,char* argv[])
{
    //FILE *fp;
    countNonTerminals = 55;
    countTerminals = 0;
    createSymbolTable();
    initializeSymbolTable();
    createTokenTable();
    initializeTokenTable();
    //printTokenTable();
    createGrammar();
    strcpy(Emp,"EMPTY\0");
    createDerivationsCopy();
    //printGrammar();
    //exit(0);
    //printf("Grammar Done\n");
    computeFirst(description);
    //printFirst();
    //printf("First Done\n");
    computeFollow(description);
    //printFollow();
    //printf("Follow Done\n");
    onlyTerminals = getAllTerminals(description);
    //printf("Retrieved Terminals\n");
    struct set TerminalsArr[countTerminals];
    populateTerminalsArray(TerminalsArr);

    //printf("populateTerminalsArray Terminals\n");

     // for(int i = 0; i < countTerminals; i++){
     //     printf("%s,%d\n",TerminalsArr[i].terminal,i);
     // }

    createParseTable(TerminalsArr);
    //printParseTable();


    //tokenCheck();
   // printCleanFile();


    int i;
    clock_t    start_time, end_time;
    double total_CPU_time, total_CPU_time_in_seconds;
    printf("\tImplementation status of our work : \n");
    printf("\t\t(a) FIRST and FOLLOW set automated\n\t\t(b) Both lexical and syntax analysis modules implemented\n\t\t(c) Modules work with all 6 testcases\n");
    printf("--------------------------------------------------------------------------------------------\n");
    printf("\tPlease choose appropriate option :\n\t\t 0.) Exit console\n\t\t 1.) Lexer Output\n\t\t 2.) Parser Output.\n\t\t 3.) AST Output\n\t\t 4.) Memory\n\t\t 5.) Symbol Table\n\t\t 6.) Activation Record Size\n\t\t 7.) Static and Dynamic Arrays\n\t\t 8.) Error Reporting.\n\t\t 9.) Code Geeneration\n\n");
    
    printf("\tEnter your choice here : ");
    scanf("%d",&i);
    while(i!=0)
    {   
        lineNo = 1;
        memset(buffer,0,sizeof(buffer));
        switch(i)
        {  
        case 0: printf("\n");
                removeComments(argv[1],"cleanfile.txt",1);
                exit(1);
                break;
        case 1: 
                printf("\n");
                fp = fopen(argv[1],"r");
                tokenCheck(fp);
                fclose(fp);
                exit(1);
                break;

        case 2: printf("\n");
                removeComments(argv[1],"cleanfile.txt",0);
                createCopy("cleanfile.txt","cleanfile2.txt");
                fp = fopen("cleanfile.txt","r");
                copy = fopen("cleanfile2.txt","r");
                writeFile = fopen(argv[2],"w");
                createParseTree(TerminalsArr);

                printf("\nPrinting Parse Tree\n");
                printInorder(root,NULL);
                fclose(writeFile);
                fclose(fp);
                fclose(copy);
                exit(1);
                break;


        case 3: printf("\n");
                removeComments(argv[1],"cleanfile.txt",0);
                createCopy("cleanfile.txt","cleanfile2.txt");
                fp = fopen("cleanfile.txt","r");
                copy = fopen("cleanfile2.txt","r");
                writeFile = fopen(argv[2],"w");
                createParseTree(TerminalsArr);

                printf("\nPrinting Parse Tree\n");
                printInorder(root,NULL);
                fclose(writeFile);
                fclose(fp);
                fclose(copy);

                struct Node *temp = root;
                ast = ast_generate(temp);
                writeAST = fopen("ast.txt","w");

                printf("\e[1;1H\e[2J");
                printf("\nPrinting AST Tree\n");
                printAstTree(ast,NULL);
                fclose(writeAST);

                exit(1);
                break;

        case 4: printf("\n");
                removeComments(argv[1],"cleanfile.txt",0);
                createCopy("cleanfile.txt","cleanfile2.txt");
                fp = fopen("cleanfile.txt","r");
                copy = fopen("cleanfile2.txt","r");
                writeFile = fopen(argv[2],"w");
                createParseTree(TerminalsArr);

                printf("\nPrinting Parse Tree\n");
                printInorder(root,NULL);
                fclose(writeFile);
                fclose(fp);
                fclose(copy);

                struct Node *temp23 = root;
                //printf("\t\tStarted Creating Symbol Tables\n");
                startCreatingScopes(temp23,NULL,NULL,0);

                struct Node *temp6 = root;
                ast = ast_generate(temp6);
                writeAST = fopen("ast.txt","w");
                printf("\nPrinting AST Tree\n");
                printAstTree(ast,NULL);
                fclose(writeAST);


                printf("\e[1;1H\e[2J");
                printf("\nMemory\n\n");
                int ptCount = (int)returnParseNodeCount();
                printf("Parse tree Number of nodes = %d\n",ptCount);
                int memoryPT = ptCount * sizeof(struct Node);
                printf("Allocated Memory = %d\n",memoryPT);
                int astCount = (int)NumberOfastNode();
                printf("Abstract Syntax tree Number of nodes = %d\n",astCount);
                int memoryAST = astCount * sizeof(struct astNode);
                printf("Allocated Memory = %d\n",memoryAST);
                float compression = ((float)(memoryPT - memoryAST)/(float)(memoryPT))*100;
                printf("Compression Percentage: %f\n",compression);
                exit(1);
                break;

        case 5: printf("\n");
                removeComments(argv[1],"cleanfile.txt",0);
                createCopy("cleanfile.txt","cleanfile2.txt");
                fp = fopen("cleanfile.txt","r");
                copy = fopen("cleanfile2.txt","r");
                writeFile = fopen(argv[2],"w");
                createParseTree(TerminalsArr);

                struct Node *temp3 = root;     

                printf("\nPrinting Parse Tree\n");           
                printInorder(temp3,NULL);
                fclose(writeFile);
                fclose(fp);
                fclose(copy);


                printf("\e[1;1H\e[2J");
                struct Node *temp2 = root;
                //printf("\t\tStarted Creating Symbol Tables\n");
                startCreatingScopes(temp2,NULL,NULL,0);
                printf("\n\n");
               // printSymbolTables();
                //checker();

                printf("\nTotal No Of Errors:%d\n\n",noOfErrors);
                printSymbolTableProc();
                exit(1);
                break;


        case 6:printf("\n");
                removeComments(argv[1],"cleanfile.txt",0);
                createCopy("cleanfile.txt","cleanfile2.txt");
                fp = fopen("cleanfile.txt","r");
                copy = fopen("cleanfile2.txt","r");
                writeFile = fopen(argv[2],"w");
                createParseTree(TerminalsArr);

                struct Node *temp9 = root;     

                printf("\nPrinting Parse Tree\n");           
                printInorder(temp9,NULL);
                fclose(writeFile);
                fclose(fp);
                fclose(copy);

                printf("\e[1;1H\e[2J");
                struct Node *temp100 = root;
                //printf("\t\tStarted Creating Symbol Tables\n");
                startCreatingScopes(temp100,NULL,NULL,0);
                printf("\n\n");
               // printSymbolTables();
                //checker();

                printf("\nTotal No Of Errors:%d\n\n",noOfErrors);
                printActivationRecordSize();
                exit(1);
                break;

        case 7:printf("\n");
                removeComments(argv[1],"cleanfile.txt",0);
                createCopy("cleanfile.txt","cleanfile2.txt");
                fp = fopen("cleanfile.txt","r");
                copy = fopen("cleanfile2.txt","r");
                writeFile = fopen(argv[2],"w");
                createParseTree(TerminalsArr);

                struct Node *temp91 = root;     

                printf("\nPrinting Parse Tree\n");           
                printInorder(temp91,NULL);
                fclose(writeFile);
                fclose(fp);
                fclose(copy);

                printf("\e[1;1H\e[2J");
                struct Node *temp1001 = root;
                //printf("\t\tStarted Creating Symbol Tables\n");
                startCreatingScopes(temp1001,NULL,NULL,0);
                printf("\n\n");
               // printSymbolTables();
                //checker();

                printf("\nTotal No Of Errors:%d\n\n",noOfErrors);
                printArrays();
                exit(1);
                break;

        
        case 8: printf("\n");
                start_time = clock();
                removeComments(argv[1],"cleanfile.txt",0);
                createCopy("cleanfile.txt","cleanfile2.txt");
                fp = fopen("cleanfile.txt","r");
                copy = fopen("cleanfile2.txt","r");
                writeFile = fopen(argv[2],"w");
                createParseTree(TerminalsArr);

                if(noOfErrors > 0){
                    printf("\nTotal Syntax Errors : %d\n\nExiting\n",noOfErrors);
                    exit(1);
                    break;
                }

                printf("\nPrinting Parse Tree\n");
                printInorder(root,NULL);
                fclose(writeFile);
                fclose(fp);
                fclose(copy);

                struct Node *temp22 = root;
                //printf("\t\tStarted Creating Symbol Tables\n");
                startCreatingScopes(temp22,NULL,NULL,0);

                struct Node *temp5 = root;
                ast = ast_generate(temp5);
                writeAST = fopen("ast.txt","w");
                printf("\nPrinting AST Tree\n");
                printAstTree(ast,NULL);
                fclose(writeAST);

                struct semNode *sn = GenerateSN(ast);
                writeSN = fopen("sem_node.txt", "w");
                struct semNode *ext = sn;
                printSN_Tree(ext,NULL);
                fclose(writeSN);

                printf("\e[1;1H\e[2J");
                ext = sn;
                printErrors(ext);

                printf("\nTotal No Of Errors:%d\n\n",noOfErrors);
                end_time = clock();

                total_CPU_time  =  (double) (end_time - start_time);
                total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;

                printf("total CPU time is : %lf\n",total_CPU_time);
                printf("total CPU time in seconds is : %lf\n",total_CPU_time_in_seconds);

                exit(1);
                break;

        case 9:printf("\n");
                removeComments(argv[1],"cleanfile.txt",0);
                createCopy("cleanfile.txt","cleanfile2.txt");
                fp = fopen("cleanfile.txt","r");
                copy = fopen("cleanfile2.txt","r");
                writeFile = fopen(argv[2],"w");
                createParseTree(TerminalsArr);

                if(noOfErrors > 0){
                    printf("\nTotal Syntax Errors : %d\n\nHandled\n\n",noOfErrors);
                }

                printf("\nPrinting Parse Tree\n");
                printInorder(root,NULL);
                fclose(writeFile);
                fclose(fp);
                fclose(copy);

                struct Node *temp223 = root;
                //printf("\t\tStarted Creating Symbol Tables\n");
                startCreatingScopes(temp223,NULL,NULL,0);

                struct Node *temp53 = root;
                ast = ast_generate(temp53);
                writeAST = fopen("ast.txt","w");
                printf("\nPrinting AST Tree\n");
                printAstTree(ast,NULL);
                fclose(writeAST);

                struct semNode *sn2 = GenerateSN(ast);
                writeSN = fopen("sem_node.txt", "w");
                struct semNode *ext1 = sn2;
                printSN_Tree(ext1,NULL);
                fclose(writeSN);

                printf("\e[1;1H\e[2J");
                ext1 = sn2;
                printErrors(ext1);

                if(noOfErrors == 0){
                    printf("No errors\n");
                }
                else{
                    printf("Total No of Errors : %d\n\n",noOfErrors);
                }

                exit(1);
                break;
        
                

        default: printf("Please enter a valid option number\n");
        }

    printf("\n\n\tImplementation status of our work : \n");
    printf("\t\t(a) FIRST and FOLLOW set automated\n\t\t(b) Both lexical and syntax analysis modules implemented\n\t\t(c) Modules work with all 6 testcases\n");
    printf("--------------------------------------------------------------------------------------------\n");
    printf("\tPlease choose appropriate option :\n\t\t 0.) Exit console\n\t\t 1.) Lexer Output\n\t\t 2.) Parser Output.\n\t\t 3.) AST Output\n\t\t 4.) Memory\n\t\t 5.) Symbol Table\n\t\t 6.) Activation Record Size\n\t\t 7.) Static and Dynamic Arrays\n\t\t 8.) Error Reporting.\n\t\t 9.) Code Geeneration\n\n");
    printf("\tEnter your choice here : ");
    scanf("%d",&i);
    }
}
