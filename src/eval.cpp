/****************************************************************************\
 eval.cpp
 Expression evaluator
 by Rafael Vuijk (aka DarkFader)

 Operators and priorities
 ------------------------
	( )
	! ~ - +		UNARY
	* / %		10
	+ -			9
	>> <<		8
	> < <= >=	7
	== !=		6
	&			5
	^			4
	|			3
	&&			2
	||			1

\****************************************************************************/

/*
 * Includes
 */
#include <stdio.h>
#include <stdlib.h>
#include "eval.h"
#include "stack.h"
#include "misc.h"
#include "pmas.h"

/*
 * Defines
 */
#define	UNARY		128
#define SEPERATOR	-1
#define D(x)		-x						// special encoding of operators (double-character, unary). should fit into CharacterStack type!

/*
 * Variables
 */
CharacterStack characterStack;
ValueStack valueStack;

/****************************************************************************\
 Evaluate
\****************************************************************************/

/*
 * Precedence
 */
int Precedence(char o, bool unary)
{
	if (unary) switch (o)
	{
		case '!':
		case '~':
		case D('+'):
		case D('-'): return UNARY;
	}
	switch (o)
	{
		// multiply/divide/modulo
		case '*':
		case '/':
		case '%': return 10;

		// add/subtract
		case '+':
		case '-': return 9;

		// shift
		case D('>'):
		case D('<'): return 8;

		// relational
		case '>':
		case '<':
		case D('}'):
		case D('{'): return 7;

		// equality
		case D('!'):
		case D('='): return 6;

		// bitwise
		case '&': return 5;
		case '^': return 4;
		case '|': return 3;

		// logical
		case D('&'): return 2;
		case D('|'): return 1;

		default: return 0;
	}
}

/*
 * EvaluateExpression
 */
ValueType EvaluateExpression(char *input, char **next)
{
EEKS{printf("EvaluateExpression '%s'\n", input);}
	if (next) *next = 0;

	/*
	 * Translate infix to postfix
	 */

	characterStack.empty();

	static char postfix[TMPSIZE];
	char *ppostfix = postfix;
	
	bool unary = true;		// allow unary operator at start
	
	char *p = input;
	while (*p)
	{
		// endline
		if (isendline(*p))
		{
			break;
		}

		// next expression
		if (*p == ',')
		{
			if (next)
			{
				*next = p+1;
				while (isspace2(*(*next))) (*next)++;	// remove spaces
				if (isendline(**next)) *next = 0;	// end of line?				
EEKS{printf("next = '%s'\n", *next);}
			}
			break;
		}

		// pass strings unchanged
		if ((*p == '\'') || (*p == '"'))
		{
			unary = false;
			char delim = *p; /**p = '"';*/ *ppostfix++ = *p++;
			while (*p)
			{
				if (*p == '\\') *ppostfix++ = *p++; else if (*p == delim) break;
				*ppostfix++ = *p++;
			}
			if (!*p) { eprintf("Missing closing quote.\n"); return ValueType::zero; }
			/**p = '"';*/ *ppostfix++ = *p++;
			continue;
		}

		CharacterStack::T c = *p++;
		CharacterStack::T c2 = *p++;

		// fix multi-character operators
		     if ((c == '>') && (c2 == '>')) c = D('>');	// ">>"
		else if ((c == '<') && (c2 == '<')) c = D('<');	// "<<"
		else if ((c == '=') && (c2 == '=')) c = D('=');	// "=="
		else if ((c == '!') && (c2 == '=')) c = D('!');	// "!="
		else if ((c == '>') && (c2 == '=')) c = D('}');	// "<="
		else if ((c == '<') && (c2 == '=')) c = D('{');	// ">="
		else if ((c == '&') && (c2 == '&')) c = D('&');	// "&&"
		else if ((c == '|') && (c2 == '|')) c = D('|');	// "||"
		else p--;	// oops, assumed too much

		// differentiate plus/minus from add/subtract
		if ((c == '+') && unary) c = D('+');
		if ((c == '-') && unary) c = D('-');

EEKS{printf("%c(%d)", c, c);}
		
		int prec = Precedence(c, unary);
		if (prec)
		{
			unary = true;
			if (prec < UNARY)
			{
				*ppostfix++ = SEPERATOR;	// seperator for number parser
			  	while (characterStack.something())
				{
			     	CharacterStack::T o = characterStack.peek();
			     	if (o == '(') break;
		        	if (Precedence(o, true) < prec) break;
					*ppostfix++ = characterStack.pop();
				}
			}
			characterStack.push(c);
		}
		else if (!isspace2(c))
		{
			unary = false;
			switch (c)
			{
				case '(':
					characterStack.push(c);
					unary = true;
					break;
				case ')':	// output number
					while (characterStack.something())
					{
				     	CharacterStack::T c = characterStack.pop();
				     	if (c == '(') break;
				        *ppostfix++ = c;
					}
					break;
				default:
					*ppostfix++ = c;
					break;
			}
		}
	}

	while (characterStack.something())
	{
		*ppostfix++ = characterStack.pop();
	}

	*ppostfix = 0;

	/*
 	 * Evaluate postfix
 	 */

	valueStack.empty();

EEKS{printf("postfix='%s'\n", postfix);}

	p = postfix;
	while (*p)
	{
//EEKS{printf("p='%s'\n", p);}

		if (*p == '\"')								// string
		{
			char s[TMPSIZE];	//, *q = s;
			//char *where = p++;
			
			ParseString(s, p);
			
			/*while (*p && (*p != '\"'))
			{
				*q++ = ParseStringChar(p);
			}
			p++;	// skip delim
			*q++ = 0;*/
			valueStack.push(ValueType(s));
		}
		else if (*p == '\'')						// ascii values
		{
			char *where = p++;
			ValueType n(ValueType::zero);
			while (*p && (*p != '\''))
			{
				n = n << 8; n = n | (unsigned char)ParseStringChar(p);
			}
			p++;	// skip delim
			valueStack.push(n);
		}
		else if ((*p == '$') || (*p == '@'))		// alternative hexadecimal prefix
		{
			ValueType n(strtol(p+1,&p,16));
			valueStack.push(n);
		}
		else if ((*p == '0') && (*(p+1) == 'b'))	// binary prefix
		{
			ValueType n(strtol(p+2,&p,2));
			valueStack.push(n);
		}
		else if (isdigit2(*p))						// decimal or hexadecimal
		{
			ValueType n(strtol(p,&p,0));
			valueStack.push(n);
		}
		else if (isidentifier(*p, true))			// identifier
		{
EEKS{printf("isidentifier(%s)\n", p);}
			char id[TMPSIZE], *i = id;
			while (isidentifier(*p, i == id)) *i++ = *p++;
			*i = 0;
EEKS{printf("id(%s)\n", id);}
			valueStack.push(GetSymbolValue(id));
		}
		else switch (*p++)							// operator
		{
			// seperators
			case SEPERATOR:
			case ' ':
			case '\t': break;

			// unary operators
	#define case_unary_break(c,o)		case c: valueStack.push(o valueStack.pop()); break;
			case_unary_break('!',!);
			case_unary_break('~',~);
			case_unary_break(D('-'),-);
			case_unary_break(D('+'),+);

			// binary operators
	#define case_binary_break(c,o)		case c: { ValueType r = valueStack.pop(); if ((c == '/') && ((long)r == 0)) { eprintf("Division by zero.\n"); return ValueType::zero; }; valueStack.push(valueStack.pop() o r); break; }
			case_binary_break('*',*);
			case_binary_break('/',/);
			case_binary_break('%',%);
			case_binary_break('+',+);
			case_binary_break('-',-);
			case_binary_break(D('>'),>>);
			case_binary_break(D('<'),<<);
			case_binary_break('>',>);
			case_binary_break('<',<);
			case_binary_break(D('}'),>=);
			case_binary_break(D('{'),<=);
			case_binary_break(D('='),==);
			case_binary_break(D('!'),!=);
			case_binary_break('&',&);
			case_binary_break('^',^);
			case_binary_break('|',|);
			case_binary_break(D('&'),&&);
			case_binary_break(D('|'),||);

/*--------------------------------------------------------------------------*/
			case '.':		// current address
			{
				extern unsigned int addr;
				valueStack.push(addr);
				break;
			}
/*--------------------------------------------------------------------------*/

			default:
				eprintf("Invalid operator '%c'.\n", *(p-1));
				return ValueType::zero;
		}
	}

EEKS{printf("pop\n");}
	ValueType r(valueStack.pop());
	if (valueStack.something())
	{
		eprintf("Expression error.\n");
		return ValueType::zero;
	}
EEKS{printf("pop2\n");}
	return r;
}
