#ifndef _EVAL_H
#define _EVAL_H

/*
 * Includes
 */
#include "valuetype.h"
#include "misc.h"

/*
 * User defined error handling
 */
void eprintf(const char *fmt, ...);

/*
 * Typedefs
 */
struct Symbol;

/*
 *
 */
Symbol *FindSymbol(char *name);
ValueType GetSymbolValue(char *name);
void SetSymbolValue(char *name, ValueType value);
void SetSymbolExpression(char *name, char *expr, char **next = 0);
ValueType EvaluateExpression(char *input, char **next = 0);
unsigned char ParseString(char *&p);
void WriteSymbols(char *filename);

#endif	// _EVAL_H
