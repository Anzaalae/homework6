/** 
 * postfix.c
 *
 * School of Computer Science, 
 * Chungbuk National University
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

/* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum{
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];	/* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];		/* 계산을 위해 필요한 스택 */

int postfixStackTop = -1;  /* postfixStack용 top */
int evalStackTop = -1;	   /* evalStack용 top */

int evalResult = 0;	   /* 계산 결과를 저장 */

// postfixStack 스택 자료구조용
void postfixpush(char x);
char postfixPop();
// evalStack 스택 자료구조용
void evalPush(int x);
int evalPop();
// infix 배열
void getInfix();
precedence getToken(char symbol);
precedence getPriority(char x);
// postfix에 문자열을 추가하기 위한 함수
void charCat(char* c);
// postfix 만드는 함수
void toPostfix();
// 현재 상황을 배열과 수택을 출력해서 알려줌
void debug();
// 모든 배열, 스택 리셋
void reset();
// 계산 수행하는 함수
void evaluation();

int main()
{
	printf("[----- [Kim Yejun]  [2020025051] -----]\n\n");


	char command;

	do{
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'i': case 'I':
			getInfix();
			break;
		case 'p': case 'P':
			toPostfix();
			break;
		case 'e': case 'E':
			evaluation();
			break;
		case 'd': case 'D':
			debug();
			break;
		case 'r': case 'R':
			reset();
			break;
		case 'q': case 'Q':
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

// postfix 스택 Push, Pop
void postfixPush(char x)
{
    postfixStack[++postfixStackTop] = x;
}
char postfixPop()
{
    char x;
    if(postfixStackTop == -1)
        return '\0';
    else {
    	x = postfixStack[postfixStackTop--];
    }
    return x;
}

// evaluation 스택 Push, Pop
void evalPush(int x)
{
    evalStack[++evalStackTop] = x;
}
int evalPop()
{
    if(evalStackTop == -1)
        return -1;
    else
        return evalStack[evalStackTop--];
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix()
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp);
}
// 우선순위를 수치화
precedence getToken(char symbol)
{
	switch(symbol) {
	case '(' : return lparen;
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '/' : return divide;
	case '*' : return times;
	default : return operand;
	}
}
precedence getPriority(char x)
{
	return getToken(x);
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c)
{
	if (postfixExp == '\0')
		strncpy(postfixExp, c, 1);
	else
		strncat(postfixExp, c, 1);
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{
	/* infixExp의 문자 하나씩을 읽기위한 포인터 */
	char *exp = infixExp;
	char x; /* 문자하나를 임시로 저장하기 위한 변수 */

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while(*exp != '\0')
	{
		// isdigit함수는 *exp값이 숫자 또는 문자이면 1 반환
		if(isdigit(*exp))
			charCat(exp);
		//() 괄호 안 문자들 전부 Push 이후 ( 까지 전부 Pop
		else if(*exp== '(')
			postfixPush(*exp);
		else if(*exp== ')') {
			while((x= postfixPop())!= '(')
				charCat(&x);
		}
		// +, -, *, /가 나오면 이전의 숫자들 전부 pop한 이후 연산자 Push
		else {
			while(postfixStackTop!= -1 && getPriority(postfixStack[postfixStackTop])>= getPriority(*exp)){
				x= postfixPop();
				charCat(&x);
			}
			postfixPush(*exp);
		}
		exp++;
	}

	// 스택에 남은 연산자 전부 Pop
	while(postfixStackTop!= -1){
		x= postfixPop();
		charCat(&x);
	}

}

void debug()
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);
	printf("postExp =  %s\n", postfixExp);
	printf("eval result = %d\n", evalResult);

	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);

	printf("\n");

}

void reset()
{	// 모든 배열, 스택 자료구조 전부 리셋
	infixExp[0] = '\0';
	postfixExp[0] = '\0';

	for(int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0';
         
	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;
}

void evaluation()
{
	/* postfixExp, evalStack을 이용한 계산 */
	char *eval= postfixExp;
	int cal1, cal2;
	char x;

	while(*eval!= '\0'){
		// 연산할 대상 숫자들을 저장 후 switch 조건문을 통해 연산
		// 문자를 숫자로 바꿔주는 atoi 함수를 사용하고 싶었지만 *exp는 const char변수가 아님
		if(isdigit(*eval)) evalPush(*eval- '0');
		else{
			cal1= evalPop();
			cal2= evalPop();
			// 연산 후 연산 결과를 스택에 Push, 이후 다음 연산에 Pop하여 사용
			switch(*eval){
				case '+':
					evalPush(cal2 + cal1);
					break;
				case '-':
					evalPush(cal2 - cal1);
					break;
				case '*':
					evalPush(cal2 * cal1);
					break;
				case '/':
					evalPush(cal2 / cal1);
					break;
			}
		}
		eval++;
	}

	evalResult= evalPop();
}