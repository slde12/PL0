#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structure.h"

static instruction* Code;
static int Stack[MAX_STACK_HEIGHT];
static int Register[MAX_REGISTER_SIZE];
static int LEX = 0;

static int SP;
static int BP;
static int PC;
static int HALT;
static int PC_LAST;
static instruction IR;
static FILE *fp;
static FILE *fp2;
static int resultPrint;
static int result;

void (*FUNC[22])() = {
	LIT, RTN, LOD, STO, CAL, INC,
	JMP, JPC, SIO, NEG, ADD, SUB, MUL,
	DIV, ODD, MOD, EQL, NEQ, LSS, LEQ,
	GTR, GEQ
};

void init(){
	SP = 0;
	BP = 1;
	PC = 0;
	HALT = 0;
	resultPrint = 0;
	IR = (instruction){0, 0, 0, 0};
//	memset(ActivitionRecord, 0, MAX_STACK_HEIGHT);
//	ActivitionRecord[ar] = BP;
//	ar++;
	memset(Stack, 0, MAX_STACK_HEIGHT);
	memset(Register, 0 , MAX_REGISTER_SIZE);
}

void loop(){
	while(1){
		if(HALT == 1) break;
		fetchCycle();
		executeCycle();
		printExecute();
		printRegister();
	}
}

void fetchCycle(){
	IR = Code[PC];
	PC_LAST = PC;
	PC = PC + 1;
}
	
void codePrint(int codeLength){
	fprintf(fp, "Op Printout:\n");
	for(int i = 0; i < codeLength; i++){
		fprintf(fp, "%2d %s %2d %2d %2d\n", i, OpName[Code[i].op - 1], Code[i].r, Code[i].l, Code[i].m);
	}
}

//op r l m
void executeCycle(){
	(*FUNC[IR.op - 1])();
}

void printExecute(){
	//printf("%2d %s %2d %2d %2d ", PC_LAST, OpName[IR.op - 1], IR.r, IR.l, IR.m);
	//printf("%s%4d%3d%3d ", OpName[IR.op - 1], IR.r, IR.l, IR.m);
	//printf("[%3d%3d%3d]   ", PC, BP, SP);
	fprintf(fp, "%-3d %-4s %3d %3d %3d %3d %3d %3d ", PC_LAST, OpName[IR.op - 1], IR.r, IR.l, IR.m, PC, BP, SP);
	printStack(SP, BP, LEX);
	fprintf(fp, "\n");
}


void printStack(int sp, int bp, int lex){
     int i;
     if (bp == 1) {
     	if (lex > 0) {
	   fprintf(fp, "|");
	   }
     }	   
     else {
     	  //Print the lesser lexical level
     	  printStack(bp-1, Stack[bp + 2], lex-1);
	  printf("|");
     }
     //Print the stack contents - at the current level
     for (i = bp; i <= sp; i++) {
     	 fprintf(fp, "%3d ", Stack[i]);	
     }
}



void printRegister(){
	fprintf(fp, "RF:");
	for (int i = 0; i < 8; i++)
	{
		fprintf(fp, "%3d ", Register[i]);
	}
	fprintf(fp, "\n");
}

void printResult(int _result){
	resultPrint = 1;
	result = _result;
	fprintf(fp, "%d\n", _result);
}

void printResultLater(){
	if(resultPrint == 1){
		fprintf(fp, "Result is:\n");
		fprintf(fp, "%d\n", result);
	}
}

// l stand for L in the instruction format
int base(int l, int base) {
	int b1; //find base L levels down
	b1 = base;
	while (l > 0){
		b1 = Stack[b1 + 1];
		l--;
	}
	return b1;
}

void LIT(){
	Register[IR.r] = IR.m;
}
void RTN(){
	SP = BP - 1;
	BP = Stack[SP + 3];
	PC = Stack[SP + 4];

//	ActivitionRecord[ar] = 0;
//	ar--;
	LEX--;
}
void LOD(){
	Register[IR.r] = Stack[base(IR.l, BP) + IR.m];
}
void STO(){
	Stack[base(IR.l, BP) + IR.m] = Register[IR.r];
}
void CAL(){
	Stack[SP + 1] = 0;
	Stack[SP + 2] = base(IR.l, BP);
	Stack[SP + 3] = BP;
	Stack[SP + 4] = PC;
	BP = SP + 1;
	PC = IR.m;

	//Not in the description
	SP = SP + 4;

//	ActivitionRecord[ar] = BP;
//	ar++;
	LEX++;
}
void INC(){
	SP = SP + IR.m;
}
void JMP(){
	PC = IR.m;
}
void JPC(){
	if(Register[IR.r] == 0){
		PC = IR.m;
	}
}
void SIO(){
	if(IR.m == 1){
		fprintf(fp, "OUTPUT: %d\n", Register[IR.r]);
		fprintf(fp2, "OUTPUT: %d\n", Register[IR.r]);
		//printResult(Register[IR.r]);
	}
	if(IR.m == 2){
		scanf("%d", &Register[IR.r]);
	}
	if(IR.m == 3){
		HALT = 1;
	}
}
void NEG(){
	Register[IR.r] = Register[IR.r] * -1;
}
void ADD(){
	Register[IR.r] = Register[IR.l] + Register[IR.m];
}
void SUB(){
	Register[IR.r] = Register[IR.l] - Register[IR.m];
}
void MUL(){
	Register[IR.r] = Register[IR.l] * Register[IR.m];
}
void DIV(){
	Register[IR.r] = Register[IR.l] / Register[IR.m];
}
void ODD(){
	Register[IR.r] = Register[IR.r] % 2;
}
void MOD(){
	Register[IR.r] = Register[IR.l] % Register[IR.m];
}
void EQL(){
	Register[IR.r] = Register[IR.l] == Register[IR.m] ? 1 : 0;	
}
void NEQ(){
	Register[IR.r] = Register[IR.l] != Register[IR.m] ? 1 : 0;
}
void LSS(){
	Register[IR.r] = Register[IR.l] < Register[IR.m] ? 1 : 0;
}
void LEQ(){
	Register[IR.r] = Register[IR.l] <= Register[IR.m] ? 1 : 0;
}
void GTR(){
	Register[IR.r] = Register[IR.l] > Register[IR.m] ? 1 : 0;
}
void GEQ(){
	Register[IR.r] = Register[IR.l] >= Register[IR.m] ? 1 : 0;
}

void printVMTracing(){
	fp = fopen("stackTracing.log", "r");
	char buffer[500];
	while(fgets(buffer, 500, fp) != NULL){
		printf("%s", buffer);
	}
	fclose(fp);
}

void printOutPut(){
	if(Code != NULL){
		fp2 = fopen("outPut.log", "r");
		char buffer[500];
		printf("\n");
		printf("-------------------------------------------\n");
		printf("PROGRAM INPUT/OUTPUT:\n");
		while(fgets(buffer, 500, fp2) != NULL){
			printf("%s", buffer);
		}
		fclose(fp2);
	}else{
		printf("\n");
		printf("-------------------------------------------\n");
		printf("PROGRAM INPUT/OUTPUT:\n");
	}
}


void virtualMachine(instruction* assembly){
	Code = assembly;
	if(Code != NULL){
		init();
		fp = fopen("stackTracing.log", "w");
		fp2 = fopen("outPut.log", "w");

		fprintf(fp, "-------------------------------------------\n");
		fprintf(fp, "VIRTUAL MACHINE TRACE:\n");
		fprintf(fp, "Initial Values:\n"); 
		fprintf(fp, "PC	BP	SP	Stack\n");
		fprintf(fp, "0	1	0	0 \n");
		fprintf(fp, "Stack Trace:\n");
		fprintf(fp, "         R  L  M  PC BP SP  Stack\n");
		loop();
		
		fclose(fp2);
		fclose(fp);
	}
}

