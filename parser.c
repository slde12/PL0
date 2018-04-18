#include "structure.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static int tokenType;//current token type
static int tokenNum;//current token value
static char tokenId[12];//current token name, 12 can be fixed later

static lexemeToken* curToken;//current token pointer
static instruction* code;//return type, a instruction set [op, r, l, m]

static int cdx = 0;//current code index
static int level = -1;//current level
static int rdx = 0;//current register index
//static int addr = 4;//current address for var or proc

static int sdx = 0;//current symbol table index(current symbol table size)
static symbol symbolTable[MAX_SYMBOL_TABLE_SIZE];




//assume no variable conflict
int findSymbol(char name[]){
	int position = -1;
	for(int i = 0; i < sdx; i++){
		if(strcmp(symbolTable[i].name, name) == 0){
			position = i;
			break;
		}
	}
	return position;
}

//get next token in the token list
void getToken(){
	tokenType = curToken -> tokenType;
	//printf("%d ", tokenType);
	if(tokenType == identsym){
		strcpy(tokenId, curToken -> lexeme);
		//printf("%s", tokenId);
	}else if(tokenType == numbersym){
		tokenNum = atoi(curToken -> lexeme);
		//printf("%d", tokenNum);
	}
	//printf("\n");
	curToken = curToken -> next;
}

void program(){
	//emit(6, 0, 0, 7);
	getToken();
	block();
	if(tokenType != periodsym){
		printf("Error: periodsym missing\n");
	}else{
		emit(9, 0, 0, 3);
	}
}

void block(){
	level++;
	//const
	if(tokenType == constsym){
		do{
			symbolTable[sdx].kind = 1;
			getToken();
			if(tokenType != identsym){
				printf("Error: identsym missing\n");
			}else{
				strcpy(symbolTable[sdx].name, tokenId);
				getToken();
				if(tokenType != eqsym){
					printf("Error: eqsym missing\n");
				}else{
					getToken();
					if(tokenType != numbersym){
						printf("Error: numbersym missing\n");
					}else{
						//generate const value
						symbolTable[sdx].val = tokenNum;
						sdx++;
						getToken();
					}
				}
			}
		}while(tokenType == commasym);
		if(tokenType != semicolonsym){
			printf("Error: semicolonsym missing\n");
		}else{
			getToken();
		}
	}

	//var
	int addrOfVar = 4;
	if(tokenType == varsym){
		do{
			symbolTable[sdx].kind = 2;
			getToken();
			if(tokenType != identsym){
				printf("Error: identsym missing\n");
			}else{
				strcpy(symbolTable[sdx].name, tokenId);
				symbolTable[sdx].level = level;
				symbolTable[sdx].addr = addrOfVar;
				addrOfVar++;
				//addr++;
				sdx++;
				getToken();
			}
		}while(tokenType == commasym);
		if(tokenType != semicolonsym){
			printf("Error: semicolonsym missing\n");
		}else{
			getToken();
		}
	}

	emit(6, 0, 0, addrOfVar);

	//never reach here for homework3
	//procedure
	while(tokenType == procsym){
		symbolTable[sdx].kind = 3;
		getToken();
		if(tokenType != identsym){
			printf("Error: identsym missing\n");
		}else{
			strcpy(symbolTable[sdx].name, tokenId);
			symbolTable[sdx].addr = cdx;
			//addrOfVar++;
			//addr++;
			sdx++;
			getToken();
			if(tokenType != semicolonsym){
				printf("Error: semicolonsym missing\n");
			}else{
				getToken();
				block();
				if(tokenType != semicolonsym){
					printf("Error: semicolonsym missing\n");
				}else{
					getToken();
				}
			}
		}
	}


	statement();

	level--;
}


void statement(){
	//when to generate symbol address
	if(tokenType == identsym){
		int i = findSymbol(tokenId);
		if(i == -1){
			printf("Error: Undeclared identify\n");
		}else if(symbolTable[i].kind != 2){
			printf("Error: Assignment to constant or procedure is not allowed\n");
		}
		else{
			getToken();
			if(tokenType != becomessym){
				printf("Error: becomessym missing\n");
			}else{
				getToken();
				expression();
				//the final result is always in register 0???
				emit(4, 0, symbolTable[i].level, symbolTable[i].addr);
				rdx--;
			}
		}
	}else if(tokenType == callsym){
		emit(6, 0, 0, 4);
		getToken();
		if(tokenType != identsym){
			printf("Error: not identsym\n");
		}else{
			int i = findSymbol(tokenId);
			if(i == -1){
				printf("Error: Undeclared identify\n");
			}else{
				if(symbolTable[i].kind != 3){
					printf("Error: call a constant or variable\n");
				}else{
					emit(5, 0, symbolTable[i].level, symbolTable[i].addr);
					getToken();
				}
			}
		}
	}else if(tokenType == beginsym){
		getToken();
		statement();
		while(tokenType == semicolonsym){
			getToken();
			statement();
		}
		if(tokenType != endsym){
			printf("Error: endsym missing\n");
		}else{
			getToken();
		}
	}else if(tokenType == ifsym){
		getToken();
		condition();
		if(tokenType != thensym){
			printf("Error: not thensym\n");
		}else{
			getToken();
			int ctemp = cdx;
			emit(8, 0, 0, 0);
			statement();
			code[ctemp].m = cdx;
		}
	}else if(tokenType == whilesym){
		int cx1 = cdx;
		getToken();
		condition();
		int cx2 = cdx;
		emit(8, 0, 0, 0);
		if(tokenType != dosym){
			printf("Error: not dosym\n");
		}else{
			getToken();
			statement();
			emit(7, 0, 0, cx1);
			code[cx2].m = cdx;
		}
	}else if(tokenType == readsym){
		getToken();
		if(tokenType != identsym){
			printf("Error: identsym missing\n");
		}else{
			int i = findSymbol(tokenId);
			emit(9, 0, 0, 2);
			emit(9, 0, symbolTable[i].level, symbolTable[i].addr);
			getToken();
		}
	}else if(tokenType == writesym){
		getToken();
		if(tokenType != identsym){
			printf("Error: identsym missing\n");
		}else{
			int i = findSymbol(tokenId);
			emit(3, 0, symbolTable[i].level, symbolTable[i].addr);
			emit(9, 0, 0, 1);
			getToken();
		}
	}
}


void condition(){
	//not complete
	if(tokenType == oddsym){
		getToken();
		expression();
		//not sure about the register
		emit(15, 0, 0, 0);
	}else{
		expression();
		if(tokenType < 9 || tokenType > 14){
			printf("Error: not rel-op\n");
		}else{
			int relOp = tokenType;
			getToken();
			expression();
			switch(relOp){
				case 9:
					//register arrangement
					emit(17, 0, rdx-2, rdx-1);
					break;
				case 10:
					emit(18, 0, rdx-2, rdx-1);
					break;
				case 11:
					emit(19, 0, rdx-2, rdx-1);
					break;
				case 12:
					emit(20, 0, rdx-2, rdx-1);
					break;
				case 13:
					emit(21, 0, rdx-2, rdx-1);
					break;
				case 14:
					emit(22, 0, rdx-2, rdx-1);
					break;
			}
			rdx -= 2;
		}
	}
}


void expression(){
	int addop;
	if(tokenType == plussym || tokenType == minussym){
		addop = tokenType;
		getToken();
		term();
		if(addop == minussym)
			emit(10, rdx-1, 0 ,0);
	}else{
		term();
	}
	while(tokenType == plussym || tokenType == minussym){
		addop = tokenType;
		getToken();
		term();
		if(addop == plussym){
			emit(11, rdx-2, rdx-2, rdx-1);
		}else{
			emit(12, rdx-2, rdx-2, rdx-1);
		}
		rdx--;
	}
}


void term(){
	int mulop;
	factor();
	while(tokenType == multsym || tokenType == slashsym){
		mulop = tokenType;
		getToken();
		factor();
		if(mulop == multsym){
			emit(13, rdx-2, rdx-2, rdx-1);
		}else{
			emit(14, rdx-2, rdx-2, rdx-1);
		}
		rdx--;
	}
}

void factor(){
	if(tokenType == identsym){
		int i = findSymbol(tokenId);
		if(i == -1){
			printf("Error: Undeclared identify\n");
		}else if(symbolTable[i].kind == 3){
			printf("Error: expression must not contain a procedure\n");
		}else{
			if(symbolTable[i].kind == 1){
				emit(1, rdx, 0, symbolTable[i].val);
			}else{
				emit(3, rdx, symbolTable[i].level, symbolTable[i].addr);
			}
			rdx++;
			getToken();
		}
	}else if(tokenType == numbersym){
		emit(1, rdx, 0, tokenNum);
		getToken();
		rdx++;
	}else if(tokenType == lparentsym){
		getToken();
		expression();
		if(tokenType != rparentsym){
			printf("Error: right parenthesis missing\n");
		}else{
			getToken();
		}
	}
}
void emit(int op, int r, int l, int m){
	if(cdx > MAX_CODE_LENGTH){
		printf("Error: code too long\n");
	}else{
		code[cdx].op = op;
		code[cdx].r = r;
		code[cdx].l = l;
		code[cdx].m = m;
		cdx++;
	}
}

void printAssembly(){
	printf("Code is syntactically correct. Assembly code generated successfully.\n");
	printf("-------------------------------------------\n");
	printf("GENERATED INTERMEDIATE CODE:\n");
	for(int i = 0; i < cdx; ++i){
		printf("%3d %s %d %d %d\n", i, OpName[code[i].op - 1], code[i].r, code[i].l, code[i].m);
	}
	printf("\n\n");
}


instruction* parser(lexemeToken* token){
	curToken = token -> next;
	code = malloc(sizeof(instruction) * MAX_CODE_LENGTH);
	program();
	
	return code;
}