#ifndef _SYMBOL_H
#define _SYMBOL_H

#include <stdio.h>
#include "valuetype.h"

/*
 * Symbol
 */
struct Symbol
{
	char *name;
	Symbol *left, *right;
	ValueType value;

	~Symbol() { Free(); }	
	void Free();
	Symbol(char *name);
	void WriteSymbols(FILE *f);
};

/*
 * Prototypes
 */
void SetSymbolValue(char *name, ValueType value);
void SetSymbolExpression(char *name, char *expr, char **next);
void UnsetSymbol(char *name);

#endif	// _SYMBOL_H
