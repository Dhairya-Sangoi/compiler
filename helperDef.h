//Batch Number : 67
//Dhairya Sangoi - 2012B4A7640P
//Gautam Singhania - 2012B4A7495P

#ifndef HELPERDEF_H_INCLUDED
#define HELPERDEF_H_INCLUDED

//structure definition for each token
typedef struct token{
    char *name;
    int isTerminal;
    int index;
    int firstRule;
    int lastRule;
}token;

//structure definition for each token in tokenlist returned by lexer
typedef struct tokenize{
    char *name;
    int lineno;
    int index;
    char *errmsg;
}tokenize;

//structure definition for token list returned by lexer
typedef struct tokenlist{
    tokenize tok;
    struct tokenlist *next;
}tokenlist;



#endif // HELPERDEF_H_INCLUDED
