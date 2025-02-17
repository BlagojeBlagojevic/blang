#ifndef UTILS_STRINGS
#define UTILS_STRINGS
#include<string.h>
#include "arena.h"

//#define DEVICE
#ifdef LOG_COMPILE
	#define printC(...) fprintf(stdout, __VA_ARGS__)
#endif
#ifndef LOG_COMPILE
	#define printC(...) 
#endif

#define DEVICE
#ifdef DEVICE
	#include "device.h"
#endif
#ifndef DEVICE
	#include "bvm_type.h"
#endif
enum {KEYWORD_VAR = 0, KEYWORD_PRINT, KEYWORD_PRINTCHAR, KEYWORD_PRINTFLOAT, KEYWORD_PRINTSTRING,
      KEYWORD_PRINTSTACK, KEYWORD_IF, KEYWORD_END, KEYWORD_ELSE, KEYWORD_PUSHSPMEM, KEYWORD_SET,
      KEYWORD_DUP, KEYWORD_LET, KEYWORD_DROP, KEYWORD_SETSP, KEYWORD_OVER, KEYWORD_ROT, KEYWORD_SWAP,
      KEYWORD_WHILE, KEYWORD_ENDLOOP, KEYWORD_BREAKLOOP, KEYWORD_PTR, KEYWORD_ARR, KEYWORD_LETARR,
      KEYWORD_PTRVAL,KEYWORD_SHR,KEYWORD_SHL, KEYWORD_OR, KEYWORD_AND,KEYWORD_BNOT,
      KEYWORD_INC, KEYWORD_HALT,	KEYWORD_WRITE, KEYWORD_OPEN, KEYWORD_CLOSE, KEYWORD_DUPF, KEYWORD_DUP2,
      KEYWORD_EXIT, KEYWORD_TRUNC, KEYWORD_ISATTY, KEYWORD_READ, KEYWORD_SLEEP,
      KEYWORD_SYSTEM, KEYWORD_POP
     };
static const char* keywords[] = { "var", "print","charprint", "floatprint", "printstring", "printstack",
                                  "if", "end", "else", "SP", "SET",
                                  "dup", "?", "drop", "setsp", "over","rot","swap",
                                  "while","endloop","breakloop",
                                  "&", "arr", "??", "@","shr","shl","or","and","bnot",
                                  "inc", "halt", "write", "open", "close", "dupF", "dup2",
                                  "exit", "truncate", "isatty", "read", "sleep",
                                  "system", "pop",
                                };
static const char* endScriptToken = "endscript";
enum {LOGIC_G, LOGIC_L, LOGIC_E, LOGIC_I};
static const char* logicOperators[] = {">", "<", "=", "!"};
//we hear nead to put wat will sep our code
static inline char isEndChar(char c) {
	const char temp = (
	                    (c == ' ')  || (c == '+') || (c == '-')
	                    || (c == '*')  || (c == '/') || (c == ',')
	                    || (c == ';')  || (c == '%') || (c == '>')
	                    || (c == '<')  || (c == '=') || (c == '(')
	                    || (c == ')')  || (c == '[') || (c == ']')
	                    || (c == '{')  || (c == '}') || (c == '\n')
	                    || (c == '\t') || (c == '!') || (c == '\0'));
	return temp;
	}

static inline char isOperator(char c) {
	return (c == '+' || c == '-' || c == '*'
	        || c == '/' || c == '>' || c == '<'
	        || c == '=' || c == '%'   || c == '!');
	}

//FOR NOW ONLY >, <, =
static inline char isLogicOperator(char c) {

	return (c == '>' || c == '<' || c == '=');


	}


static inline char isDigit(char c) {
	return (c == '0' || c == '1' || c == '2'
	        || c == '3' || c == '4' || c == '5'
	        || c == '6' || c == '7' || c == '8'
	        || c == '9'   );
	}


static inline char isValidVar(char* str) {
	return (str[0] != '0' && str[0] != '1' && str[0] != '2'
	        && str[0] != '3' && str[0] != '4'
	        && str[0] != '5' && str[0] != '6'
	        && str[0] != '7' && str[0] != '8'
	        && str[0] != '9' && !isEndChar(str[0]));
	}

//HEAR PUT ALL KEYWORDS IN LEUNGUE

static inline char isKeyword(char* str) {

	for (size_t i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
		if (strcmp(str, keywords[i]) == 0) {
			return 1;
			}
		}
	return 0;
	}
#ifdef DEVICE
	static inline char isDevice(char* str){
		for (size_t i = 0; i < sizeof(device_name) / sizeof(device_name[0]); i++) {
			if (strcmp(str, device_name[i]) == 0) {
				return 1;
				}
			}
		return 0;
	}
#endif
#ifndef DEVICE
	static inline char isDevice(char* str){
		(void*)str;
		return 0;
	}
#endif
//TD: ADD CHECKING FOR FLOAT AND MAYBE BRACES
static inline char isInteger(char* str) {
	if (str == NULL || *str == '\0') {
		return 0;
		}
	int i = 0;
	while (isDigit(str[i])) {
		i++;
		}
	return str[i] == '\0';
	}

static inline char isFloat(char* str) {
	if (str == NULL || *str == '\0' || strtod(str, NULL) == 0.0F) {
		return 0;
		}
	return 1;
	}


static inline char isStrEqual(const char *comp,char *str) {
	if(!strcmp(comp, str)) {
		return 1;
		}
	else {
		return 0;
		}
	}


static inline char* returnSubstring(char* str, int start, int end, Arena *mainArena) {
	//int length = strlen(str);
	int subLength = end - start + 1;
	char* subStr = arena_alloc(mainArena, (subLength + 1) * sizeof(char));
	//memset(subStr, '\0', (subLength + 1) * sizeof(char));
	//char* subStr = malloc((subLength + 1) * sizeof(char));

	strncpy(subStr, str + start, subLength);
	//system("pause");
	subStr[subLength] = '\0';
	return subStr;
	}

static inline int ValueToNum(char *str) {
	long int num;
	char *end;
	num = strtol(str, &end, 10);
	//printf("Num is %d", (int)num);
	return (int)num;
	}

static inline int isValidString(char *str) {
	if(str[0] != '"')
		return 0;
	int counter = 1;
	while(str[counter] != '\0') {
		if(str[counter] == '"' && str[counter + 1] == '\0')
			return 1;
		counter++;
		}
	return 0;
	}

#define ENCODE_OPERAND(first, second) \
	(first) |          \
	(second << 6)


#define FIRST(operand) (operand & 0x3f)


#define SECOND(operand) ((operand & 0xfc0) >> 6)





#endif
