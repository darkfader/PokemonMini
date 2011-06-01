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
//Symbol *FindSymbol(const char *name);
//ValueType GetSymbolValue(const char *name);
//void SetSymbolValue(const char *name, ValueType value);
//void SetSymbolExpression(const char *name, const char *expr, char **next = 0);
ValueType EvaluateExpression(const char *input, const char **next = 0);
unsigned char ParseString(char *&p);
//void WriteSymbols(const char *filename);

#endif	// _EVAL_H
