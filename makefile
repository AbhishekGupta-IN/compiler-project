run:
	gcc -o stage1exe driver.c lexer.c parser.c ast.c symTable.c semantics.c -w

