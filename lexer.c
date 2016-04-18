//Batch Number : 67
//Dhairya Sangoi - 2012B4A7640P
//Gautam Singhania - 2012B4A7495P

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "trieDef.h"
#include "trie.h"
#include "rulesDef.h"
#include "rules.h"
#include "parseTreeDef.h"
#include "parseTree.h"
#include "helperDef.h"
#include "stackDef.h"
#include "stack.h"
#include "helper.h"
#include "lexerDef.h"



FILE *getStream(FILE *fp, char* B, int k){
//This function takes the  input from the file pointed to by 'fp'.
//This file is the source code written in the given language.
//The function uses efficient technique to bring the fixed sized piece (of size k)
//of source code into the buffer B  for processing so as to avoid intensive I/O operations
//mixed with CPU intensive tasks. The function also maintains the file pointer
//after every access so that it can get more data into the memory on demand.
	//B = (char*) malloc((k+1)*sizeof(char));
	int i;
    for(i = 0; i<=k && (B[i]=fgetc(fp)) != EOF ; i++);
    B[i] = '\0';
    return fp;
}

State addToken(tokenlist* ctr, int *isAdded, int *next_pos, trie* t, State* x, char* name_of_token, int lineno){
	ctr->tok.name = strdup(name_of_token);
	ctr->tok.lineno = lineno;
	*isAdded = 1;
	(*next_pos)--;//To re-read the character

	char err_to_be[100] = "";
	ctr->tok.errmsg = NULL;
	switch(*x){
        case SYMUNDERSCORE:
        case SYMHASH:
        case SYMLTDASHTWO:
        case SYMEXCLAIM:
        case SYMEQUAL:
	case ERRTONOTEALPHANUM:
        case SYMAMPERSCENT:
        case SYMAMPERSCENTTWO:
        case SYMATTHERATE:
        case SYMATTHERATETWO:
        case DIGITDOT:
        case DIGITDOTDIGIT:
		case ERRTONOTESYM:
			ctr->tok.index = (searchTrie(t, "TK_ERR"))->dataIndex;
			sprintf(err_to_be, "ERROR_3:  Unknown pattern <%s> at line number <%d>.", name_of_token, lineno);
            ctr->tok.errmsg = strdup(err_to_be);
			break;
        case ERRUNKNOWN:
            ctr->tok.index = (searchTrie(t, "TK_ERR"))->dataIndex;
            sprintf(err_to_be,
                    "ERROR_2: Unknown Symbol <%c> at line <%d>",
                    name_of_token[0], lineno);
            ctr->tok.errmsg = strdup(err_to_be);
            break;
        case SYMGT:
            ctr->tok.index = (searchTrie(t, "TK_GT"))->dataIndex;
			break;
        case SYMLT:
            ctr->tok.index = (searchTrie(t, "TK_LT"))->dataIndex;
        break;
        case SYMLTDASH:
            ctr->tok.name = strdup("<");
            ctr->tok.index = (searchTrie(t, "TK_LT"))->dataIndex;
            *isAdded = 1;
			ctr->tok.lineno = lineno;
			ctr->next = (tokenlist*) malloc(sizeof(tokenlist));
			ctr = ctr->next;
			ctr->tok.name = strdup("-");
			ctr->tok.index = (searchTrie(t, "TK_MINUS"))->dataIndex;
			ctr->tok.lineno = lineno;
		case SYMUNDERSCOREM:
		case SYMUNDERSCOREMA:
		case SYMUNDERSCOREMAI:
		case FUNCTIONID:
		case FUNCTIONIDDIGITS:
		    if(strlen(ctr->tok.name)>30){
               ctr->tok.index = (searchTrie(t, "TK_ERR"))->dataIndex;
               sprintf(err_to_be,
                       "ERROR_1: Identifier at line <%d> is longer than the prescribed length of 30 characters.",
                       lineno);
                ctr->tok.errmsg = strdup(err_to_be);
		    }
			else
                ctr->tok.index = (searchTrie(t, "TK_FUNID"))->dataIndex;
			break;
		case MAIN:
			ctr->tok.index = (searchTrie(t, "TK_MAIN"))->dataIndex;
			break;
		case RECORDID:
			ctr->tok.index = (searchTrie(t, "TK_RECORDID"))->dataIndex;
			break;
		case DIGIT:
			ctr->tok.index = (searchTrie(t, "TK_NUM"))->dataIndex;
			break;
		case DIGITDOTDIGITTWO:
			ctr->tok.index = (searchTrie(t, "TK_RNUM"))->dataIndex;
			break;
		case ID:
		case IDCHAR:
		case IDCHARDIGIT:
		    if(strlen(ctr->tok.name)>20){
               ctr->tok.index = (searchTrie(t, "TK_ERR"))->dataIndex;
               sprintf(err_to_be,
                       "ERROR_1: Identifier at line <%d> is longer than the prescribed length of 20 characters.",
                       lineno);
                ctr->tok.errmsg = strdup(err_to_be);
		    }
			else
                ctr->tok.index = (searchTrie(t, "TK_ID"))->dataIndex;
			break;
        case BORD:
        case C:
        case CA:
        case CAL:
        case E:
        case EL:
        case ELS:
        case EN:
        case ENDI:
        case ENDR:
        case ENDRE:
        case ENDREC:
        case ENDRECO:
        case ENDRECOR:
        case ENDW:
        case ENDWH:
        case ENDWHI:
        case ENDWHIL:
        case G:
        case GL:
        case GLO:
        case GLOB:
        case GLOBA:
        case I:
        case IN:
        case INP:
        case INPU:
        case L:
        case LI:
        case LIS:
        case O:
        case OU:
        case OUT:
        case OUTP:
        case OUTPU:
        case P:
        case PA:
        case PAR:
        case PARA:
        case PARAM:
        case PARAME:
        case PARAMET:
        case PARAMETE:
        case R:
        case RE:
        case REA:
        case REC:
        case RECO:
        case RECOR:
        case RET:
        case RETU:
        case RETUR:
        case T:
        case TH:
        case THE:
        case TY:
        case TYP:
        case W:
        case WH:
        case WHI:
        case WHIL:
        case WI:
        case WIT:
        case WR:
        case WRI:
        case WRIT:
		case FIELDID:
			ctr->tok.index = (searchTrie(t, "TK_FIELDID"))->dataIndex;
			break;
		case CALL:
			ctr->tok.index = (searchTrie(t, "TK_CALL"))->dataIndex;
			break;
        case IF:
			ctr->tok.index = (searchTrie(t, "TK_IF"))->dataIndex;
			break;
		case ELSE:
			ctr->tok.index = (searchTrie(t, "TK_ELSE"))->dataIndex;
			break;
		case END:
			ctr->tok.index = (searchTrie(t, "TK_END"))->dataIndex;
			break;
		case ENDIF:
			ctr->tok.index = (searchTrie(t, "TK_ENDIF"))->dataIndex;
			break;
		case ENDRECORD:
			ctr->tok.index = (searchTrie(t, "TK_ENDRECORD"))->dataIndex;
			break;
		case ENDWHILE:
			ctr->tok.index = (searchTrie(t, "TK_ENDWHILE"))->dataIndex;
			break;
		case GLOBAL:
			ctr->tok.index = (searchTrie(t, "TK_GLOBAL"))->dataIndex;
			break;
		case INPUT:
			ctr->tok.index = (searchTrie(t, "TK_INPUT"))->dataIndex;
			break;
		case INT:
			ctr->tok.index = (searchTrie(t, "TK_INT"))->dataIndex;
			break;
		case LIST:
			ctr->tok.index = (searchTrie(t, "TK_LIST"))->dataIndex;
			break;
		case OUTPUT:
			ctr->tok.index = (searchTrie(t, "TK_OUTPUT"))->dataIndex;
			break;
		case PARAMETER:
			ctr->tok.index = (searchTrie(t, "TK_PARAMETER"))->dataIndex;
			break;
		case PARAMETERS:
			ctr->tok.index = (searchTrie(t, "TK_PARAMETERS"))->dataIndex;
			break;
		case RECORD:
			ctr->tok.index = (searchTrie(t, "TK_RECORD"))->dataIndex;
			break;
		case REAL:
			ctr->tok.index = (searchTrie(t, "TK_REAL"))->dataIndex;
			break;
		case READ:
			ctr->tok.index = (searchTrie(t, "TK_READ"))->dataIndex;
			break;
		case RETURN:
			ctr->tok.index = (searchTrie(t, "TK_RETURN"))->dataIndex;
			break;
		case THEN:
			ctr->tok.index = (searchTrie(t, "TK_THEN"))->dataIndex;
			break;
		case TYPE:
			ctr->tok.index = (searchTrie(t, "TK_TYPE"))->dataIndex;
			break;
		case WHILE:
			ctr->tok.index = (searchTrie(t, "TK_WHILE"))->dataIndex;
			break;
		case WITH:
			ctr->tok.index = (searchTrie(t, "TK_WITH"))->dataIndex;
			break;
		case WRITE:
			ctr->tok.index = (searchTrie(t, "TK_WRITE"))->dataIndex;
			break;
        default:
            ctr->tok.index = (searchTrie(t, "TK_ERR"))->dataIndex;
            ctr->tok.errmsg = strdup("Don't know how this came!!");
			break;
	}
	return INIT;
}

tokenlist* getNextToken(char B[], State* state, int* lineno, trie *t, char *name_of_token){
//This function reads the input character stream and uses efficient mechanism to recognize lexemes.
//The function tokenizes the lexeme appropriately and returns all relevant information it collects
//in this phase (lexical analysis phase) encapsulated as  tokenInfo.
//The function also displays lexical errors appropriately (list of errors to be uploaded soon).
//The input parameters are made flexible and students can select appropriate input parameters.
//The function also takes care of ignoring the white spaces and comments.
	int next_pos = 0, tokens_added = 0, isAdded = 0;
	int temp;
	char read_char = B[next_pos++];
	tokenlist *cur, *ctr, *last_added_token;

	cur = (tokenlist*) malloc(sizeof(tokenlist));
	ctr = cur;
	while(read_char != '\0'){
		isAdded = 0;

		//Delimiters
		if(read_char == ' '||read_char == '\n'||read_char == '%' ||read_char == '\t'||read_char == '\r'){

			if(*state!=INIT&&*state!=COMMENT){
				*state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
				next_pos++;//Since we don't need to re-read this character
			}
			else	isAdded = -1;

			if (read_char == '\n'){
				(*lineno)++;
				if(*state == COMMENT)	*state = INIT;
			}
			else if(read_char == '%')	*state = COMMENT;
		}

		//Non-delimiters
		else{
			switch(*state){
				case COMMENT:
					isAdded = -1;
					break;
				case ERRTONOTEALPHANUM:
					if(!(isalnum(read_char))){
						*state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
					}
					break;
				case ERRTONOTESYM:
					if(isalnum(read_char)){
						*state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
					}
					break;

				case INIT:
					switch(read_char){
					    case ',':ctr->tok.name = strdup(",");
								 ctr->tok.index = (searchTrie(t, "TK_COMMA"))->dataIndex;
								 ctr->tok.lineno = *lineno;
								 isAdded = 1;
								 break;
						case '[':ctr->tok.name = strdup("[");
								 ctr->tok.index = (searchTrie(t, "TK_SQL"))->dataIndex;
								 ctr->tok.lineno = *lineno;
								 isAdded = 1;
								 break;
						case ']':ctr->tok.name = strdup("]");
								 ctr->tok.index = (searchTrie(t, "TK_SQR"))->dataIndex;
								 ctr->tok.lineno = *lineno;
								 isAdded = 1;
								 break;
						case ';':ctr->tok.name = strdup(";");
								 ctr->tok.index = (searchTrie(t, "TK_SEM"))->dataIndex;
								 ctr->tok.lineno = *lineno;
								 isAdded = 1;
								 break;
						case ':':ctr->tok.name = strdup(":");
								 ctr->tok.index = (searchTrie(t, "TK_COLON"))->dataIndex;
								 ctr->tok.lineno = *lineno;
								 isAdded = 1;
								 break;
						case '.':ctr->tok.name = strdup(".");
								 ctr->tok.index = (searchTrie(t, "TK_DOT"))->dataIndex;
								 ctr->tok.lineno = *lineno;
								 isAdded = 1;
								 break;
						case '(':ctr->tok.name = strdup("(");
								 ctr->tok.index = (searchTrie(t, "TK_OP"))->dataIndex;
								 ctr->tok.lineno = *lineno;
								 isAdded = 1;
								 break;
						case ')':ctr->tok.name = strdup(")");
								 ctr->tok.index = (searchTrie(t, "TK_CL"))->dataIndex;
								 ctr->tok.lineno = *lineno;
								 isAdded = 1;
								 break;
						case '+':ctr->tok.name = strdup("+");
								 ctr->tok.index = (searchTrie(t, "TK_PLUS"))->dataIndex;
								 ctr->tok.lineno = *lineno;
								 isAdded = 1;
								 break;
						case '-':ctr->tok.name = strdup("-");
								 ctr->tok.index = (searchTrie(t, "TK_MINUS"))->dataIndex;
								 ctr->tok.lineno = *lineno;
								 isAdded = 1;
								 break;
						case '*':ctr->tok.name = strdup("*");
								 ctr->tok.index = (searchTrie(t, "TK_MUL"))->dataIndex;
								 ctr->tok.lineno = *lineno;
								 isAdded = 1;
								 break;
						case '/':ctr->tok.name = strdup("/");
								 ctr->tok.index = (searchTrie(t, "TK_DIV"))->dataIndex;
								 ctr->tok.lineno = *lineno;
								 isAdded = 1;
								 break;
						case '~':ctr->tok.name = strdup("~");
								 ctr->tok.index = (searchTrie(t, "TK_NOT"))->dataIndex;
								 ctr->tok.lineno = *lineno;
								 isAdded = 1;
								 break;
						case '_':*state = SYMUNDERSCORE;
								 break;
						case '#':*state = SYMHASH;
								 break;
						case '<':*state = SYMLT;
								 break;
						case '>':*state = SYMGT;
								 break;
						case '!':*state = SYMEXCLAIM;
								 break;
						case '=':*state = SYMEQUAL;
								 break;
						case '&':*state = SYMAMPERSCENT;
								 break;
						case '@':*state = SYMATTHERATE;
								 break;
						case 'b':
						case 'd':*state = BORD;
								 break;
						case 'c':*state = C;
								 break;
						case 'e':*state = E;
								 break;
						case 'g':*state = G;
								 break;
						case 'i':*state = I;
								 break;
						case 'l':*state = L;
								 break;
						case 'o':*state = O;
								 break;
						case 'p':*state = P;
								 break;
						case 'r':*state = R;
								 break;
						case 't':*state = T;
								 break;
						case 'w':*state = W;
								 break;
						default:if(read_char>='0'&&read_char<='9')
									*state = DIGIT;
								else if(read_char>='a'&&read_char<='z')
									*state = FIELDID;
								else if(read_char>='A'&&read_char<='Z')
									*state = ERRTONOTEALPHANUM;
								else{
                                    *state = ERRUNKNOWN;
                                    name_of_token[0] = read_char;
                                    name_of_token[1] = '\0';
								    *state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
								    next_pos++;
                                }
					}
					break;
				case SYMUNDERSCORE:{
					if(read_char=='m'){
						*state = SYMUNDERSCOREM;
					}
					else if(isalpha(read_char)){
						*state = FUNCTIONID;
					}
					else if(isdigit(read_char)){
						*state = ERRTONOTEALPHANUM;
					}
					else{
						*state = ERRTONOTESYM;
					}
				}
					break;

				case SYMUNDERSCOREM:{
					if(read_char=='a'){
						*state = SYMUNDERSCOREMA;
					}
					else if(isalpha(read_char)){
						*state = FUNCTIONID;
					}
					else if(read_char>='0'&&read_char<='9'){
						*state = FUNCTIONIDDIGITS;
					}
					else{
						*state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
					}
				}
					break;

				case SYMUNDERSCOREMA:{
					if(read_char=='i'){
						*state = SYMUNDERSCOREMAI;
					}
					else if(isalpha(read_char)){
						*state = FUNCTIONID;
					}
					else if(read_char>='0'&&read_char<='9'){
						*state = FUNCTIONIDDIGITS;
					}
					else{
						*state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
					}
				}
					break;

				case SYMUNDERSCOREMAI:{
					if(read_char=='n'){
						*state = MAIN;
					}
					else if(isalpha(read_char)){
						*state = FUNCTIONID;
					}
					else if(read_char>='0'&&read_char<='9'){
						*state = FUNCTIONIDDIGITS;
					}
					else{
						*state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
					}
				}
					break;

				case MAIN:{
					if(isalpha(read_char)){
						*state = FUNCTIONID;
					}
					else if(read_char>='0'&&read_char<='9'){
						*state = FUNCTIONIDDIGITS;
					}
					else{
						*state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
					}
				}
					break;

				case FUNCTIONID:{
					if(read_char>='0'&&read_char<='9'){
						*state = FUNCTIONIDDIGITS;
					}
					else if(!(isalpha(read_char))){
						*state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
					}
				}
					break;

				case FUNCTIONIDDIGITS:{
					if(isalpha(read_char)){
						*state = ERRTONOTEALPHANUM;
					}
					else if(!(read_char>='0'&&read_char<='9')){
						*state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
					}
				}
					break;

				case SYMHASH:{
					if(read_char>='a'&&read_char<='z'){
						*state = RECORDID;
					}
					else{
						*state = ERRTONOTESYM;
						if(isalnum(read_char)){
							*state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
						}
					}
				}
					break;

				case RECORDID:{
					if(!(isalnum(read_char))){
						*state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
					}
					else if(!(read_char>='a'&&read_char<='z')){
						*state = ERRTONOTEALPHANUM;
					}
				}
					break;

				case SYMLT:{
					switch(read_char){
						case '-':*state = SYMLTDASH;
								 break;
						case '=':ctr->tok.name = strdup("<=");
								 ctr->tok.index = (searchTrie(t, "TK_LE"))->dataIndex;
								 ctr->tok.lineno = *lineno;
								 isAdded = 1;
								 break;
						default: *state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
					}
				}
					break;

				case SYMLTDASH:{
					if(read_char=='-'){
						*state = SYMLTDASHTWO;
					}
					else{
						ctr->tok.name = strdup("<");
						ctr->tok.index = (searchTrie(t, "TK_LT"))->dataIndex;
						isAdded = 1;
						ctr->tok.lineno = *lineno;
						ctr->next = (tokenlist*) malloc(sizeof(tokenlist));
						ctr = ctr->next;
						tokens_added++;
						ctr->tok.name = strdup("-");
						ctr->tok.index = (searchTrie(t, "TK_MINUS"))->dataIndex;
						ctr->tok.lineno = *lineno;
					}
				}
					break;

				case SYMLTDASHTWO:{
					if(read_char=='-'){
						ctr->tok.name = strdup("<---");
						ctr->tok.index = (searchTrie(t, "TK_ASSIGNOP"))->dataIndex;
						ctr->tok.lineno = *lineno;
						isAdded = 1;
					}
					else{
						*state = ERRTONOTESYM;
						if(isalnum(read_char))
							*state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
					}
				}
					break;

				case SYMGT:{
					if(read_char=='='){
						ctr->tok.name = strdup(">=");
						ctr->tok.index = (searchTrie(t, "TK_GE"))->dataIndex;
						ctr->tok.lineno = *lineno;
						isAdded = 1;
					}
					else{
						*state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
					}
				}
					break;

				case SYMEXCLAIM:{
					if(read_char=='='){
						ctr->tok.name = strdup("!=");
						ctr->tok.index = (searchTrie(t, "TK_NE"))->dataIndex;
						ctr->tok.lineno = *lineno;
						isAdded = 1;
					}
					else{
						*state = ERRTONOTESYM;
						if(isalnum(read_char))
							*state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
					}
				}
					break;

				case SYMEQUAL:{
					if(read_char=='='){
						ctr->tok.name = strdup("==");
						ctr->tok.index = (searchTrie(t, "TK_EQ"))->dataIndex;
						ctr->tok.lineno = *lineno;
						isAdded = 1;
					}
					else{
						*state = ERRTONOTESYM;
						if(isalnum(read_char))
							*state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
					}
				}
					break;

				case SYMAMPERSCENT:{
					if(read_char=='&'){
						*state = SYMAMPERSCENTTWO;
					}
					else{
						*state = ERRTONOTESYM;
						if(isalnum(read_char))
							*state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
					}
				}
					break;

				case SYMAMPERSCENTTWO:{
					if(read_char=='&'){
						ctr->tok.name = strdup("&&&");
						ctr->tok.index = (searchTrie(t, "TK_AND"))->dataIndex;
						ctr->tok.lineno = *lineno;
						isAdded = 1;
					}
					else{
						*state = ERRTONOTESYM;
						if(isalnum(read_char))
							*state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
					}
				}
					break;

				case SYMATTHERATE:{
					if(read_char=='@'){
						*state = SYMATTHERATETWO; // changed here previous was SYMAMPERSCENTTWO
					}
					else{
						*state = ERRTONOTESYM;
						if(isalnum(read_char))
							*state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
					}
				}
					break;

				case SYMATTHERATETWO:{
					if(read_char=='@'){
						ctr->tok.name = strdup("@@@"); // changed from &&& to @@@
						ctr->tok.index = (searchTrie(t, "TK_OR"))->dataIndex;
						ctr->tok.lineno = *lineno;
						isAdded = 1;
					}
					else{
						*state = ERRTONOTESYM;
						if(isalnum(read_char))
							*state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
					}
				}
					break;

				case DIGIT:{
					if(isalpha(read_char)){
						*state = ERRTONOTEALPHANUM;
					}
					else if(read_char == '.'){
						*state = DIGITDOT;
					}
					else if(!(isdigit(read_char))){
						*state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
					}
				}
					break;

				case DIGITDOT:{
					if(isdigit(read_char)){
						*state = DIGITDOTDIGIT;
					}
					else{
						*state = ERRTONOTEALPHANUM;
						if(!(isalpha(read_char)))
							*state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
					}
				}
					break;

				case DIGITDOTDIGIT:{
					if(isdigit(read_char)){
						*state = DIGITDOTDIGITTWO;
					}
					else{
						if(!(isalpha(read_char)))
							*state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
						else
							*state = ERRTONOTEALPHANUM;
					}
				}
					break;

				case DIGITDOTDIGITTWO:{
					if(isalnum(read_char)){
						*state = ERRTONOTEALPHANUM;
					}
					else{
						*state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
					}
				}
					break;

				case C:
				case BORD:{
					if(read_char>='2'&&read_char<='7'){
						*state = ID;
					}
					else if(read_char == 'a'&&*state == C){
						*state = CA;
					}
					else if(read_char>='a'&&read_char<='z'){
						*state = FIELDID;
					}
					else if(isalnum(read_char)){
						*state = ERRTONOTEALPHANUM;
					}
					else{
						*state = FIELDID;
						*state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
					}
				}
					break;

				case ID:{
					if(read_char>='b'&&read_char<='d'){
						*state = IDCHAR;
					}
					else if (read_char>='2'&&read_char<='7'){
						*state = IDCHARDIGIT;
					}
					else if(isalnum(read_char)){
						*state = ERRTONOTEALPHANUM;
					}
					else{
						*state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
					}
				}
					break;

				case IDCHAR:{
					if (read_char>='2'&&read_char<='7'){
						*state = IDCHARDIGIT;
					}
					else if(isalnum(read_char) && !(read_char>='b'&&read_char<='d')){
						*state = ERRTONOTEALPHANUM;
					}
					else if(!(read_char>='b'&&read_char<='d')){
						*state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
					}
				}
					break;


				case IDCHARDIGIT:{
					if(isalnum(read_char) && !(read_char>='2'&&read_char<='7')){
						*state = ERRTONOTEALPHANUM;
					}
					else if(!(read_char>='2'&&read_char<='7')){
						*state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
					}
				}
					break;

				case END:
                    if(*state==END){
                        if(read_char=='i'){
                            *state = ENDI;
                            break;
                        }
                        else if(read_char=='r'){
                            *state = ENDR;
                            break;
                        }
                        else if(read_char=='w'){
                            *state = ENDW;
                            break;
                        }
                    }
				case PARAMETER:
					if(read_char=='s'&&*state==PARAMETER){
						*state = PARAMETERS;
						break;
					}

				case CALL:
				case ELSE:
				case ENDIF:
				case ENDRECORD:
				case ENDWHILE:
				case GLOBAL:
                case IF:
				case INPUT:
				case INT:
				case LIST:
				case OUTPUT:
				case PARAMETERS:
				case RECORD:
				case REAL:
				case READ:
				case RETURN:
				case THEN:
				case TYPE:
				case WHILE:
				case WITH:
				case WRITE:{
					if(read_char>='a'&&read_char<='z'){
						*state = FIELDID;
					}
					else if(isalnum(read_char)){
						*state = ERRTONOTEALPHANUM;
					}
					else{
						*state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
					}
				}
					break;

				case FIELDID:{
					if((read_char>='A'&&read_char<='Z')||(read_char>='0'&&read_char<='9')){
						*state = ERRTONOTEALPHANUM;
					}
					else if (!(isalnum(read_char))){
						*state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
					}
				}
					break;

				//deals with other forms of fieldid which may become some a specific field
				//e.g. CAL belongs to fieldid, but CALL is a specific field
				default:{
					switch(read_char){
						case 'a':{
							switch(*state){
								case GLOB:	*state=GLOBA; break;
								case RE:	*state=REA; break;
								case P:		*state=PA; break;
								case PAR:	*state=PARA; break;
								default:	*state=FIELDID;
							}
						}
							break;

						case 'b':{
							switch(*state){
								case GLO:	*state=GLOB; break;
								default:	*state=FIELDID;
							}
						}
							break;

						case 'c':{
							switch(*state){
								case ENDRE:	*state=ENDREC; break;
								case RE:	*state=REC; break;
								default:	*state=FIELDID;
							}
						}
							break;

						case 'd':{
							switch(*state){
								case EN:	*state=END; break;
								case ENDRECOR:*state=ENDRECORD; break;
								case RECOR:	*state=RECORD; break;
								case REA:	*state=READ; break;
								default:	*state=FIELDID;
							}
						}
							break;

						case 'e':{
							switch(*state){
								case ELS:	*state=ELSE; break;
								case ENDR:	*state=ENDRE; break;
								case ENDWHIL:*state=ENDWHILE; break;
								case R:		*state=RE; break;
								case PARAM:	*state=PARAME; break;
								case PARAMET:*state=PARAMETE; break;
								case TH:	*state=THE; break;
								case TYP:	*state=TYPE; break;
								case WHIL:	*state=WHILE; break;
								case WRIT:	*state=WRITE; break;
								default:	*state=FIELDID;
							}
						}
							break;

						case 'f':{
							switch(*state){
								case ENDI:	*state=ENDIF; break;
								case I:		*state=IF; break;
								default:	*state=FIELDID;
							}
						}
							break;

						case 'h':{
							switch(*state){
								case ENDW:	*state=ENDWH; break;
								case T:		*state=TH; break;
								case W:		*state=WH; break;
								case WIT:	*state=WITH; break;
								default:	*state=FIELDID;
							}
						}
							break;

						case 'i':{
							switch(*state){
								case ENDWH:	*state=ENDWHI; break;
								case L:		*state=LI; break;
								case W:		*state=WI; break;
								case WH:	*state=WHI; break;
								case WR:	*state=WRI; break;
								default:	*state=FIELDID;
							}
						}
							break;

						case 'l':{
							switch(*state){
								case CA:	*state=CAL; break;
								case CAL:	*state=CALL; break;
								case E:		*state=EL; break;
								case ENDWHI:*state=ENDWHIL; break;
								case G:		*state=GL; break;
								case GLOBA:	*state=GLOBAL; break;
								case REA:	*state=REAL; break;
								case WHI:	*state=WHIL; break;
								default:	*state=FIELDID;
							}

						}
							break;

						case 'm':{
							switch(*state){
								case PARA:	*state=PARAM; break;
								default:	*state=FIELDID;
							}
						}
							break;

						case 'n':{
							switch(*state){
								case E:		*state=EN; break;
								case I:		*state=IN; break;
								case RETUR:	*state=RETURN; break;
								case THE:	*state=THEN; break;
								default:	*state=FIELDID;
							}
						}
							break;

						case 'o':{
							switch(*state){
								case ENDREC:*state=ENDRECO; break;
								case GL:	*state=GLO; break;
								case REC:	*state=RECO; break;
								default:	*state=FIELDID;
							}
						}
							break;

						case 'p':{
							switch(*state){
								case IN:	*state=INP; break;
								case OUT:	*state=OUTP; break;
								case TY:	*state=TYP; break;
								default:	*state=FIELDID;
							}
						}
							break;

						case 'r':{
							switch(*state){
								case ENDRECO:*state=ENDRECOR; break;
								case RECO:	*state=RECOR; break;
								case RETU:	*state=RETUR; break;
								case PA:	*state=PAR; break;
								case PARAMETE:*state=PARAMETER; break;
								case W:		*state=WR; break;
								default:	*state=FIELDID;
							}
						}
							break;

						case 's':{
							switch(*state){
								case EL:	*state=ELS; break;
								case LI:	*state=LIS; break;
								default:	*state=FIELDID;
							}
						}
							break;

						case 't':{
							switch(*state){
								case IN:	*state=INT; break;
								case INPU:	*state=INPUT; break;
								case LIS:	*state=LIST; break;
								case RE:	*state=RET; break;
								case OU:	*state=OUT; break;
								case OUTPU:	*state=OUTPUT; break;
								case PARAME:*state=PARAMET; break;
								case WI:	*state=WIT; break;
								case WRI:	*state=WRIT; break;
								default:	*state=FIELDID;
							}
						}
							break;

						case 'u':{
							switch(*state){
								case INP:	*state=INPU; break;
								case RET:	*state=RETU; break;
								case O:		*state=OU; break;
								case OUTP:	*state=OUTPU; break;
								default:	*state=FIELDID;
							}
						}
							break;

						case 'y':{
							switch(*state){
								case T:		*state=TY; break;
								default:	*state=FIELDID;
							}
						}
							break;

						default:
							if(read_char>='a'&&read_char<='b'){
								*state = FIELDID;
							}
							else if(isalnum(read_char)){
								*state = ERRTONOTEALPHANUM;
							}
							else{
								*state = FIELDID;
								*state = addToken(ctr, &isAdded, &next_pos, t, state, name_of_token, *lineno);
							}
					}
				}
			}
		}

		if(isAdded==1){
			ctr->next = (tokenlist*) malloc(sizeof(tokenlist));
			last_added_token = ctr;
                if(ctr->tok.index != (searchTrie(t, "TK_ERR"))->dataIndex){
                    ctr->tok.errmsg = NULL;
                }

//            tokenlist* temp2 = cur;
//            while(temp2!=NULL){
//                printf("\n%s ", temp2->tok.name);
//            }
			ctr = ctr->next;
			tokens_added++;
			strcpy(name_of_token, "");
			*state = INIT;
		}
		else if(isAdded==0){
			temp = strlen(name_of_token);
			name_of_token[temp] = read_char;
			name_of_token[temp+1] = '\0';
		}
		read_char = B[next_pos++];
	}

	if(tokens_added>0){
		free(ctr);
		last_added_token->next = NULL;
		return cur;
	}
	else{
		free(cur);
		return NULL;
	}
}

tokenlist* caller_function(FILE* fp, int k, trie *t){
	//Inputs the file pointer, the buffer size and the trie

	//Initializations
	tokenlist *head, *tail;
	head = (tokenlist*) malloc(sizeof(tokenlist));
	tail = head;
	char* B = (char*) malloc((k+1)*sizeof(char));
	State* b = (State*) malloc(sizeof(State));
	*b = INIT;
	int *lineno = (int*) malloc(sizeof(int));
	*lineno = 1;
	int temp3;

	char name_of_token[300] = "";

	int c;

	while ( (c=fgetc(fp)) != EOF ){
        fseek(fp, -1, SEEK_CUR);//To re-read the current read input
		fp = getStream(fp, B, k);
		tail->next = getNextToken(B, b, lineno, t, name_of_token);
//        temp2 = head->next;
//        while(temp2!=NULL){
//            printf("\n%s ", temp2->tok.name);
//        }
		while(tail->next != NULL){
			tail = tail->next;

		}
	}

	if(*b!=INIT&&*b!=COMMENT){
        tail->next = (tokenlist*) malloc(sizeof(tokenlist));
        tail = tail->next;
        *b = addToken(tail, &temp3, &temp3, t, b, name_of_token,*lineno);
        tail->tok.lineno = *lineno;
	}

    tail->next = (tokenlist*) malloc(sizeof(tokenlist));
    tail = tail->next;
    tail->tok.name = "$";
    tail->tok.index = searchTrie(t, "$")->dataIndex;
    tail->tok.lineno = *lineno;
    tail->next = NULL;

    //tail = head;
    head = head->next;
    //free(tail);

	return head;
}
