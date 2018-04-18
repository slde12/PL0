#define norw 15 /* number of reserved words */
#define imax 32767 /* maximum integer value */
#define cmax 11 /* maximum number of chars for idents */
#define nestmax 5 /* maximum depth of block nesting */
#define strmax 256 /* maximum length of strings */
#define cmax_1 12

#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3
#define MAX_REGISTER_SIZE 16

#define MAX_SYMBOL_TABLE_SIZE 4096
#define MAX_CODE_LENGTH 500

typedef enum{
	nulsym = 1, identsym,   numbersym, 
	plussym,    minussym,   multsym, 
	slashsym,   oddsym,     eqsym, 
	neqsym,     lessym,     leqsym,
    gtrsym,     geqsym,     lparentsym, 
    rparentsym, commasym,   semicolonsym,
	periodsym, 	becomessym, beginsym, 
	endsym, 	ifsym, 		thensym,
	whilesym, 	dosym, 		callsym, 
	constsym, 	varsym, 	procsym, 
	writesym, 	readsym , 	elsesym
}token_type;

static const char OpName[22][4] = {
	"LIT\0", "RTN\0", "LOD\0", "STO\0", "CAL\0", "INC\0",
	"JMP\0", "JPC\0", "SIO\0", "NEG\0", "ADD\0", "SUB\0", "MUL\0",
	"DIV\0", "ODD\0", "MOD\0", "EQL\0", "NEQ\0", "LSS\0", "LEQ\0",
	"GTR\0", "GEQ\0" };

typedef struct lexeme{
	char lexeme[cmax_1];
	int tokenType;
	struct lexeme *next;
}lexemeToken;

typedef struct symbol{
	int kind; // const = 1, var = 2, proc = 3
	char name[12]; // name up to 11 chars
	int val; // number (ASCII value)
	int level; // L level
	int addr; // M address
}symbol;

typedef struct instruction{
	int op;
	int r;
	int l;
	int m;
}instruction;


int findType(char *lexeme);
void addLexeme(lexemeToken *myToken, char lexeme[]);
void printTokenTable(lexemeToken *myToken);
void printTokenList(lexemeToken *myToken);
void printTokenListSymbolic(lexemeToken *myToken);
void printTokens(lexemeToken *myToken);
lexemeToken* lexer(char const *fileName);

void getToken();
void program();
void block();
void statement();
void condition();
void expression();
void term();
void factor();
void emit(int op, int r, int l, int m);
void printAssembly();
instruction* parser(lexemeToken* token);

int base(int l, int base);
void init();
void readInput();
void loop();
void fetchCycle();
void codePrint();
void executeCycle();
void printExecute();
void printRegister();
void printResult();
void printResultLater();
void printStack();
void LIT();
void RTN();
void LOD();
void STO();
void CAL();
void INC();
void JMP();
void JPC();
void SIO();
void NEG();
void ADD();
void SUB();
void MUL();
void DIV();
void ODD();
void MOD();
void EQL();
void NEQ();
void LSS();
void LEQ();
void GTR();
void GEQ();
void printVMTracing();
void printOutPut();
void virtualMachine();