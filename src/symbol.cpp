#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pmas.h"
#include "symbol.h"
#include "eval.h"

/*
 *
 */
Symbol *symbols = 0;

/*
 *
 */
Symbol::Symbol(char *name)
{
	this->name = strdup(name);
	left = right = 0;
}

/*
 *
 */
void Symbol::Free()
{
	if (name) { free(name); name = 0; }
}

/*
 *
 */
void Symbol::WriteSymbols(FILE *f)
{
	if (left) left->WriteSymbols(f);
	if (!value.getString()) fprintf(f, "%08X %s\n", (int)value, name);
	if (right) right->WriteSymbols(f);
}

/*
 * WriteSymbols
 */
void WriteSymbols(const char *filename)
{
	FILE *f = fopen(filename, "wt");
	if (!f) { fprintf(stderr, "Error opening output file.\n"); exit(1); }
	if (symbols) symbols->WriteSymbols(f);
	fclose(f);
}

/*
 * FindSymbol
 */
Symbol *FindSymbol(const char *name)
{
	Symbol *s = symbols;
	while (s)
	{
		int d = strcmp(name, s->name);
		if (!d)
		{
			return s;
		}
		else if (d < 0)
		{
			s = s->left;
		}
		else
		{
			s = s->right;
		}
	}
	return 0;
}

/*
 * CreateSymbol
 */
Symbol *CreateSymbol(char *name)
{
	Symbol *s = symbols;
	if (!s)
	{
		return s = symbols = new Symbol(name);
	}
	else while (1)
	{
		int d = strcmp(name, s->name);
		if (d < 0)
		{
			if (s->left) s = s->left; else { s->left = new Symbol(name); return s->left; }
		}
		else
		{
			if (s->right) s = s->right; else { s->right = new Symbol(name); return s->right; }
		}
	}
}

/*
 * GetSymbolValue
 */
ValueType GetSymbolValue(const char *name)
{
	extern int pass;

	if (name[0] == '_')		// try local labels first
	{
		char name2[TMPSIZE];
		extern char locallabelprefix[];
		strcpy(name2, locallabelprefix);
		strcat(name2, name);
EEKS{printf("local label? %s\n", name2);}
		Symbol *s2 = FindSymbol(name2);
		if (s2) return s2->value;
	}

	Symbol *s = FindSymbol(name);
	
	if (!s && (pass == 1)) return ValueType::zero;		// assume forward reference is a number
	
	if (!s) { eprintf("Symbol '%s' not defined.\n", name); return ValueType::zero; }
	if (!s->value.defined()) { eprintf("Symbol '%s' not defined.\n", name); return ValueType::zero; }

EEKS{printf("GetSymbolValue %s, ", name); s->value.print();}

	return s->value;
}

/*
 * SetSymbolValue
 */
void SetSymbolValue(char *name, ValueType value)
{
EEKS{printf("SetSymbolValue %s, ", name); value.print();}

	Symbol *s = FindSymbol(name);
	if (!s) s = CreateSymbol(name);
	s->value = value;
}

/*
 * SetSymbolExpression
 */
void SetSymbolExpression(char *name, const char *expr, const char **next)
{
EEKS{printf("SetSymbolExpression %s, %s\n", name, expr);}
	ValueType n(EvaluateExpression(expr, next));
EEKS{printf("after eval: "); n.print();}
	SetSymbolValue(name, n);
}

/*
 * UnsetSymbol
 */
void UnsetSymbol(char *name)
{
EEKS{printf("UnsetSymbol %s, ", name);}
	Symbol *s = FindSymbol(name);
	s->value.undefine();
}
