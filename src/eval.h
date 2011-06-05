#ifndef _EVAL_H
#define _EVAL_H

/*
 * Includes
 */
#include "valuetype.h"
#include "misc.h"

/*
 * Typedefs
 */
struct Symbol;

/*
 * prototypes
 */
ValueType EvaluateExpression(const char *input, const char **next = 0);
unsigned char ParseString(char *&p);

#endif	// _EVAL_H
