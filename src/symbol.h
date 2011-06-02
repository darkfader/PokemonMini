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
	Symbol(const char *name);
	void WriteSymbols(FILE *f);
};

/*
 * Prototypes
 */
Symbol *FindSymbol(const char *name);
void SetSymbolValue(const char *name, ValueType value);
ValueType GetSymbolValue(const char *name);
void SetSymbolExpression(char *name, const char *expr, const char **next = NULL);
void UnsetSymbol(char *name);
void WriteSymbols(const char *filename);

#endif	// _SYMBOL_H
