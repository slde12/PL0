#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structure.h"


//void parser(char const *fileName);
//void printTokens();
//void intermediateCodeGenerater();
//void printAssembly();
//void virtualMachine();
//void printVMTracing();

//void parser(char const *fileName){}
//void printTokens(){}
//void intermediateCodeGenerater(){}
//void printAssembly(){}
//void virtualMachine(){}
//void printVMTracing(){}


int main(int argc, char const *argv[]){
	char const *fileName = argv[1];
	lexemeToken* myToken = lexer(fileName);
	//printTokens(myToken);
	
	instruction* assembly = parser(myToken);
	//printAssembly();
	
	virtualMachine(assembly);

	if(argc > 2){
		for(int i = 2; i < argc; i++){
			if(strcmp(argv[i], "-l") == 0){
				if(myToken == NULL){
					printf("\n");
					break;
				}
				printTokens(myToken);
			}
			else if(strcmp(argv[i], "-v") == 0){
				printVMTracing();
			}
			else if(strcmp(argv[i], "-a") == 0){
				printAssembly();
			}
		}
	}

	printOutPut();

	//generatorSymbolTable();
	return 0;
}