#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pmas.h"
#include "symbol.h"
#include "eval.h"

/*
 *
 */
Symbol *symbols = 0;
Symbol *symbolsicase = 0;
Symbol *macrosymbols = 0;
const char *SymTypes[12] = {
	"",    "",
	"DEF", "Def",
	"RAM", "Ram",
	"LAB", "Lab",
	"LOC", "Loc",
	"MAC", "Mac"
};

/*
 *
 */
Symbol::Symbol(const char *name, int type)
{
	this->name = strdup(name);
	this->type = type;
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
	extern int option_symoutput;
	if (left) left->WriteSymbols(f);
	if (type != SYM_SYS) {
		if (option_symoutput == 1) {
			if (!value.getString())	// Number
				fprintf(f, "%s $%08X %s\n", SymTypes[type], (int)value, name);
			else			// String
				fprintf(f, "%s \"%s\" %s\n", SymTypes[type], value.getString(), name);
		} else {
			if (!value.getString()) fprintf(f, "%08X %s\n", (int)value, name);
		}
	}
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
	if (symbolsicase) symbolsicase->WriteSymbols(f);
	fclose(f);
}

/*
 * FindSymbol
 */
Symbol *FindSymbolFrom(const char *name, Symbol *s, bool icase)
{
	int d;
	while (s)
	{
		if (icase) {
			d = strcasecmp(name, s->name);
		} else {
			d = strcmp(name, s->name);
		}
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
 * FindSymbol
 */
Symbol *FindSymbol(const char *name)
{
	extern int inside_macro;
	Symbol *s;
	if (inside_macro) {
		s = FindSymbolFrom(name, macrosymbols, false);
		if (s) return s;
	}
	s = FindSymbolFrom(name, symbols, false);
	if (s) return s;
	s = FindSymbolFrom(name, symbolsicase, true);
	return s;
}

/*
 * IsSymbolDefined
 */
bool IsSymbolDefined(const char *name)
{
	Symbol *s = FindSymbol(name);
	if (s == NULL) return false;
	return s->value.defined();
}

/*
 * CreateSymbol
 */
Symbol *CreateSymbol(const char *name, int type)
{
	Symbol *s = symbols;
	int d;
	if (type == SYM_MAC) {
		s = macrosymbols;
	} else if (type & 1) {
		s = symbolsicase;
	}
	if (!s)
	{
		if (type == SYM_MAC) {
			return macrosymbols = new Symbol(name, type);
		} else if (type & 1) {
			return symbolsicase = new Symbol(name, type);
		} else {
			return symbols = new Symbol(name, type);
		}
	}
	else while (1)
	{
		if (s->type & 1) {
			d = strcasecmp(name, s->name);
		} else {
			d = strcmp(name, s->name);
		}
		if (d < 0)
		{
			if (s->left) s = s->left; else { s->left = new Symbol(name, type); return s->left; }
		}
		else
		{
			if (s->right) s = s->right; else { s->right = new Symbol(name, type); return s->right; }
		}
	}
}

/*
 * GetSymbolValue
 */
ValueType GetSymbolValue(const char *name)
{
	extern int pass;

	if (name[0] == '_')	// try local labels first
	{
		int locallabel = 1;
		if (name[1] == '_')	// or special label
		{
			ValueType out;
			if (SpecialSymbols(name, out)) return out;
		}
		if (locallabel)
		{
			char name2[TMPSIZE];
			extern char locallabelprefix[];
			strcpy(name2, locallabelprefix);
			strcat(name2, name);
EEKS{printf("local label? %s\n", name2);}
			Symbol *s2 = FindSymbol(name2);
			if (s2) return s2->value;
		}
	}

	Symbol *s = FindSymbol(name);

	if (!s && (pass != PASS_ASM)) return ValueType::zero;		// assume forward reference is a number

	if (!s) { eprintf("Symbol '%s' not defined.\n", name); return ValueType::zero; }
	if (!s->value.defined()) { eprintf("Symbol '%s' not defined.\n", name); return ValueType::zero; }

EEKS{printf("GetSymbolValue %s, ", name); s->value.print();}

	return s->value;
}

/*
 * SetSymbolValue
 */
void SetSymbolValue(const char *name, ValueType value, int type)
{
EEKS{printf("SetSymbolValue %s, ", name); value.print();}

	Symbol *s = FindSymbol(name);
	if (!s) s = CreateSymbol(name, type);
	s->value = value;
	s->type = type;
}

/*
 * SetSymbolExpression
 */
void SetSymbolExpression(char *name, const char *expr, int type, const char **next)
{
EEKS{printf("SetSymbolExpression %s, %s\n", name, expr);}
	ValueType n(EvaluateExpression(expr, next));
EEKS{printf("after eval: "); n.print();}
	SetSymbolValue(name, n, type);
}

/*
 * UnsetSymbol
 */
void UnsetSymbol(char *name)
{
EEKS{printf("UnsetSymbol %s, ", name);}
	Symbol *s = FindSymbol(name);
	if (s)
	{
		if (s->type == SYM_SYS)
		{
			eprintf("Cannot unset reserved symbol '%s'.\n", name);
		}
		else
		{
			s->value.undefine();
		}
	}
}
