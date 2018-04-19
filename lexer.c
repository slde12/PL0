#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "structure.h"

char const *token_type_represent[] = {
	"nulsym", 	  "identsym",   "numbersym", 
	"plussym",    "minussym",   "multsym", 
	"slashsym",   "oddsym",     "eqsym", 
	"neqsym",     "lessym",     "leqsym",
    "gtrsym",     "geqsym",     "lparentsym", 
    "rparentsym", "commasym",   "semicolonsym",
	"periodsym",  "becomessym", "beginsym", 
	"endsym", 	  "ifsym", 		"thensym",
	"whilesym",   "dosym", 		"callsym", 
	"constsym",   "varsym", 	"procsym", 
	"writesym",   "readsym", 	"elsesym"
};




int findType(char *lexeme){
	//default, identsym
	int type = identsym;

	//null
	if(strcmp(lexeme, "null") == 0)
		type = nulsym;
	
	//number
	else if(isdigit(lexeme[0]))
		type = numbersym;
	
	// plus
	else if(strcmp(lexeme, "+") == 0)
		type = plussym;

	//minus
	else if(strcmp(lexeme, "-") == 0)
		type = minussym;

	//multiple
	else if(strcmp(lexeme, "*") == 0)
		type = multsym;

	//slash
	else if(strcmp(lexeme, "/") == 0)
		type = slashsym;

	// odd
	else if(strcmp(lexeme, "odd") == 0)
		type = oddsym;

	// equal
	else if(strcmp(lexeme, "=") == 0)
		type = eqsym;

	//not equal to
	else if(strcmp(lexeme, "<>") == 0)
		type = neqsym;

	//less than
	else if(strcmp(lexeme, "<") == 0)
		type = lessym;

	//less than or equal to
	else if(strcmp(lexeme, "<=") == 0)
		type = leqsym;

	//greater than
	else if(strcmp(lexeme, ">") == 0)
		type = gtrsym;

	//greater than or equal to
	else if(strcmp(lexeme, ">=") == 0)
		type = geqsym;

	//left parentheses
	else if(strcmp(lexeme, "(") == 0)
		type = lparentsym;

	//right parentheses
	else if(strcmp(lexeme, ")") == 0)
		type = rparentsym;

	//comma
	else if(strcmp(lexeme, ",") == 0)
		type = commasym;

	//semicolon
	else if(strcmp(lexeme, ";") == 0)
		type = semicolonsym;

	//period
	else if(strcmp(lexeme, ".") == 0)
		type = periodsym;

	//becomes
	else if(strcmp(lexeme, ":=") == 0)
		type = becomessym;

	//begin
	else if(strcmp(lexeme, "begin") == 0)
		type = beginsym;

	// end
	else if(strcmp(lexeme, "end") == 0)
		type = endsym;

	// if
	else if(strcmp(lexeme, "if") == 0)
		type = ifsym;

	//then
	else if(strcmp(lexeme, "then") == 0)
		type = thensym;

	//while
	else if(strcmp(lexeme, "while") == 0)
		type = whilesym;

	//do
	else if(strcmp(lexeme, "do") == 0)
		type = dosym;

	//call
	else if(strcmp(lexeme, "call") == 0)
		type = callsym;

	//constant
	else if(strcmp(lexeme, "const") == 0)
		type = constsym;

	//var keyword
	else if(strcmp(lexeme, "var") == 0)
		type = varsym;

	//procedure
	else if(strcmp(lexeme, "procedure") == 0)
		type = procsym;

	//write
	else if(strcmp(lexeme, "write") == 0)
		type = writesym;

	//read
	else if(strcmp(lexeme, "read") == 0)
		type = readsym;

	// else
	else if(strcmp(lexeme, "else") == 0)
		type = elsesym;

	return type;
}

void addLexeme(lexemeToken *myToken, char lexeme[]){
	lexemeToken *newToken = malloc(sizeof(lexemeToken));
	strcpy(newToken -> lexeme, lexeme);
	newToken -> tokenType = findType(lexeme);
	//printf("%s  %d\n", newToken -> lexeme, newToken -> tokenType);
	newToken->next = NULL;
	lexemeToken *p = myToken;
	while(p -> next != NULL){
		p = p ->next;
	}
	p -> next = newToken;
}

void printTokenTable(lexemeToken *myToken){
	printf("\nLexeme Table:\n");
	printf("lexeme\t\ttoken type\n");
	lexemeToken *p = myToken;
	p = p-> next;
	while(p != NULL){
		printf("%s\t\t%d\n", p->lexeme, p->tokenType);
		p = p -> next;
	}
}

void printTokenList(lexemeToken *myToken){
	printf("\nInternal Representation:\n");
	lexemeToken *p = myToken;
	p = p -> next;
	while(p != NULL){
		printf("%d ", p->tokenType);
		if(p->tokenType == identsym)
			printf("%s ", p->lexeme);
		else if(p->tokenType == numbersym)
			printf("%d ", atoi(p->lexeme));
		p = p -> next;
	}
}

void printTokenListSymbolic(lexemeToken *myToken){
	printf("\nSymbolic Representation:\n");
	lexemeToken *p = myToken;
	p = p-> next;
	while(p != NULL){
		printf("%s ", token_type_represent[p->tokenType - 1]);
		if(p->tokenType == 2)
			printf("%s ", p->lexeme);
		else if(p->tokenType == 3)
			printf("%d ", atoi(p->lexeme));
		p = p -> next;
	}
}

void printTokens(lexemeToken *myToken){
	printf("-------------------------------------------\n");
	printf("LIST OF LEXEMES/TOKENS:\n");

	printTokenList(myToken);
	printf("\n");
	printTokenListSymbolic(myToken);
	printf("\n\n");
}


lexemeToken* lexer(char const *fileName){

	FILE *fp = fopen(fileName, "r");

	if(fp == NULL){
		printf("Cannot open the file \"%s \"\n", fileName);
		return NULL;
	}
	else{
		//print input file header
		//printf("Source Program:%s\n", fileName);

		//store the whole file into buffer
		int bufferSize = 0;
		char c;
		while((c = fgetc(fp)) != EOF)
		{	
			bufferSize++;
		}
		rewind(fp);

		char fileBuffer[bufferSize];

		int i = 0;
		//read and print input code character by character
		while((c = fgetc(fp)) != EOF)
		{	
			fileBuffer[i++] = c;
			//printf("%c", c);
		}

		//printf("\n");

		lexemeToken *myToken = malloc(sizeof(lexemeToken));
		myToken -> next = NULL;

		char token[cmax_1];
		memset(token, '\0', cmax_1);

		i = 0;
		int identifierLength = 0;
		int halt = 0;
		while(i < bufferSize && halt == 0){
			c = fileBuffer[i];
			//printf("%c\n", c);
			//number or id or reserved words
			if(isdigit(c) || isalpha(c)){
				//id or reserved words
				if(isalpha(c)){
					token[identifierLength++] = c;
					i++;

					c = fileBuffer[i];
					while(isalpha(c) || isdigit(c)){
						token[identifierLength++] = c;
						i++;

						c = fileBuffer[i];
						if(identifierLength == 12){
							printf("Error: Identifier too long.\n");
							halt = 1;
							break;
						}
					}

					addLexeme(myToken, token);
					memset(token, '\0', cmax_1);
					identifierLength = 0;
				}//number
				else{
					token[identifierLength++] = c;
					i++;

					c = fileBuffer[i];
					if(isalpha(c)){
						printf("Error: Invalid symbol.\n");
						halt = 1;
					}
					while(isdigit(c)){
						token[identifierLength++] = c;
						i++;
						c = fileBuffer[i];
					}
					if (atoi(token) > imax){
						printf("Error: Number too large.\n");
						halt = 1;
					}
					addLexeme(myToken, token);
					memset(token, '\0', cmax_1);
					identifierLength = 0;
				}
			}
			//skip comments, keep divide
			else if(c == '/'){
				token[identifierLength++] = c;
				i++;
				
				c = fileBuffer[i];
				//comments
				if(c == '*'){
					i++;

					c = fileBuffer[i];
					//look ahead
					char d = fileBuffer[i+1];
					while(c != '*' || d != '/'){
						i++;

						c = fileBuffer[i];
						d = fileBuffer[i+1];
					}
					i += 2;
				}
				//divide
				else{
					addLexeme(myToken, token);
				}
				memset(token, '\0', cmax_1);
				identifierLength = 0;
			}
			//skip space, tab and newline
			else if(c == 0x20 || c == 0x9 || c == 0xA || c == 0xD){
				i++;
			}
			//special symbol
			else if(c == '+' || c == '-' || c == '*' || c == '(' || c == ')' || c == ',' || c == '.' || c == '<' || c == '>' || c == ';' || c == ':' || c == '='){
				// :=
				if(c == ':'){
					token[identifierLength++] = c;
					i++;
					c = fileBuffer[i];
					if(c == '='){
						token[identifierLength++] = c;
						addLexeme(myToken, token);
						memset(token, '\0', cmax_1);
						identifierLength = 0;
						i++;
					}else{
						//error
					}
				}
				// <, <=
				else if(c == '<'){
					token[identifierLength++] = c;
					i++;
					c = fileBuffer[i];
					if(c == '='){
						token[identifierLength++] = c;
						i++;
					}
					else if(c == '>'){
						token[identifierLength++] = c;
						i++;
					}
					addLexeme(myToken, token);
					memset(token, '\0', cmax_1);
					identifierLength = 0;
				}
				// >, >=
				else if(c == '>'){
					token[identifierLength++] = c;
					i++;
					c = fileBuffer[i];
					if(c == '='){
						token[identifierLength++] = c;
						i++;
					}
					addLexeme(myToken, token);
					memset(token, '\0', cmax_1);
					identifierLength = 0;
				}
				else{
					token[identifierLength++] = c;
					addLexeme(myToken, token);
					memset(token, '\0', cmax_1);
					identifierLength = 0;
					i++;
				}
			}
			else{
				printf("%x Error: Invalid symbol.\n", c);
				halt = 1;
			}
		}
		if(halt == 0){
			return myToken;
		}else{
			return NULL;
		}
		//printTokenTable(myToken);
		//printf("test3\n");
		//printTokenList(myToken);
	}
}