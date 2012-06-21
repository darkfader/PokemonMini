#ifndef _SYMBOL_H
#define _SYMBOL_H

#include <stdio.h>
#include "valuetype.h"

/*
 * Enum
 */
enum {
	SYM_SYS = 0,		// system symbol
	SYM_SYS_X = 1,
	SYM_DEF = 2,		// value or string
	SYM_DEF_ICASE = 3,	// value or string (icase)
	SYM_RAM = 4,		// ram labels
	SYM_RAM_ICASE = 5,	// ram labels (icase)
	SYM_LAB = 6,		// labels
	SYM_LAB_X = 7,
	SYM_LOC = 8,		// local labels
	SYM_LOC_X = 9,
	SYM_MAC = 10,		// macro symbol
	SYM_MAC_X = 11
};

/*
 * Symbol
 */
struct Symbol
{
	char *name;
	Symbol *left, *right;
	ValueType value;
	int type;

	~Symbol() { Free(); }
	void Free();
	Symbol(const char *name, int type);
	void WriteSymbols(FILE *f);
};

/*
 * Prototypes
 */
bool IsSymbolDefined(const char *name);
void SetSymbolValue(const char *name, ValueType value, int type);
ValueType GetSymbolValue(const char *name);
void SetSymbolExpression(char *name, const char *expr, int type, const char **next = NULL);
void UnsetSymbol(char *name);
void WriteSymbols(const char *filename);

#endif	// _SYMBOL_H
