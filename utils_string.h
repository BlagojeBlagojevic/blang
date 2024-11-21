#ifndef UTILS_STRINGS
#define UTILS_STRINGS
//#include<string.h>

enum {KEYWORD_VAR = 0, KEYWORD_PRINT,KEYWORD_PRINTCHAR, KEYWORD_IF, KEYWORD_END,
      KEYWORD_ELSE, KEYWORD_DUP, KEYWORD_LET, KEYWORD_DROP,
      KEYWORD_WHILE, KEYWORD_ENDLOOP, KEYWORD_BREAKLOOP, KEYWORD_PTR, KEYWORD_ARR, KEYWORD_LETARR,
      KEYWORD_PTRVAL, KEYWORD_POP
     };
static const char* keywords[] = { "var", "print","charprint", "if", "end", "else",
                           "dup", "let", "drop","while","endloop","breakloop",
                           "ptr", "arr", "letarr", "ptrval", "pop",
                         };
static const char* endScriptToken = "endscript";
enum {LOGIC_G, LOGIC_L, LOGIC_E};
static const char* logicOperators[] = {">", "<", "="};
//we hear nead to put wat will sep our code
static char isEndChar(char c) {
	const char temp = (
	                    (c == ' ') || (c == '+') || (c == '-')
	                    || (c == '*') || (c == '/') || (c == ',')
	                    || (c == ';') || (c == '%') || (c == '>')
	                    || (c == '<') || (c == '=') || (c == '(')
	                    || (c == ')') || (c == '[') || (c == ']')
	                    || (c == '{') || (c == '}') || (c == '\n')
	                    || (c == '\t') || (c == '\0'));
	return temp;
	}

static char isOperator(char c) {
	return (c == '+' || c == '-' || c == '*'
	        || c == '/' || c == '>' || c == '<'
	        || c == '=' || c == '%');
	}

//FOR NOW ONLY >, <, =
static char isLogicOperator(char c) {

	return (c == '>' || c == '<' || c == '=');


	}


static char isDigit(char c) {
	return (c == '0' || c == '1' || c == '2'
	        || c == '3' || c == '4' || c == '5'
	        || c == '6' || c == '7' || c == '8'
	        || c == '9'   );
	}


static char isValidVar(char* str) {
	return (str[0] != '0' && str[0] != '1' && str[0] != '2'
	        && str[0] != '3' && str[0] != '4'
	        && str[0] != '5' && str[0] != '6'
	        && str[0] != '7' && str[0] != '8'
	        && str[0] != '9' && !isEndChar(str[0]));
	}

//HEAR PUT ALL KEYWORDS IN LEUNGUE

static char isKeyword(char* str) {

	for (size_t i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
		if (strcmp(str, keywords[i]) == 0) {
			return 1;
			}
		}
	return 0;
	}
//TD: ADD CHECKING FOR FLOAT AND MAYBE BRACES
static char isInteger(char* str) {
	if (str == NULL || *str == '\0') {
		return 0;
		}
	int i = 0;
	while (isDigit(str[i])) {
		i++;
		}
	return str[i] == '\0';
	}

static char* returnSubstring(char* str, int start, int end) {
	//int length = strlen(str);
	int subLength = end - start + 1;
	char* subStr = malloc((subLength + 1) * sizeof(char));
	//memset(subStr, '\0', (subLength + 1) * sizeof(char));
	strncpy(subStr, str + start, subLength);
	subStr[subLength] = '\0';
	return subStr;
	}

static int ValueToNum(char *str) {
	long int num;
	char *end;
	num = strtol(str, &end, 10);
	//printf("Num is %d", (int)num);
	return (int)num;
	}


#endif
