all:driver.c helper.c lexer.c parser.c parseTree.c rules.c stack.c trie.c allFunctionsHashTable.c dynamicArray.c recordsHashTable.c scopeHashTable.c quadruple.c semanticAnalyzer.c
	gcc -Wall driver.c helper.c lexer.c parser.c parseTree.c rules.c stack.c trie.c allFunctionsHashTable.c dynamicArray.c recordsHashTable.c scopeHashTable.c quadruple.c semanticAnalyzer.c -o stage1exe -lm
