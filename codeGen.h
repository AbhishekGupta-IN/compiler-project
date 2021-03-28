
int printCodeBlock(codeBlock * blk);
int printToFile(codeBlock * blk, FILE * fp);
struct codeBlock * createCodeBlock();
int addCodeLine(char * buffer, codeBlock * cb) ;
int getLabel();
int getTemporary();