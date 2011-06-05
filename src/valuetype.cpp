#include <string.h>
#include <stdlib.h>
#include "pmas.h"
#include "valuetype.h"
#include "eval.h"

char undefined[] = "undefined";

ValueType::ValueType() { s = 0; i = 0; }

ValueType::ValueType(const ValueType &rhs)
{
	s = rhs.s ? strdup(rhs.s) : 0;
	i = rhs.i;
}

ValueType::ValueType(const char *s) { this->s = strdup(s); i=0; }
ValueType::ValueType(long i) { s = 0; this->i = i; }


ValueType& ValueType::operator = (const ValueType &rhs) 
{
	if (this == &rhs) { eprintf("Assign to myself!\n"); eexit(); }
	Free();
	s = rhs.s ? strdup(rhs.s) : 0;
	i = rhs.i;
  	return *this;
}

void ValueType::Free() { if (s) { s[0] = '0'; if (s != undefined) free(s); s = 0; } }
ValueType::~ValueType() { Free(); }

/*
 * operator long
 * evaluate expression to number
 */
ValueType::operator long () const
{
	if (s)
	{
		ValueType n = EvaluateExpression(s);
		if (n.getString() != NULL) eprintf("Invalid string operation.\n"); return 0;
		return (long)n;
	}
	return i;
}

/*
 * getString
 * returns NULL when it's a number
 */
const char * ValueType::getString() const
{
	return s;
}

/*
 * undefine
 */
void ValueType::undefine()
{
	Free();
	s = undefined;
	i = 0;
}

/*
 * defined
 */
bool ValueType::defined()
{
	return (s != undefined);
}

/*
 * operator +
 */
ValueType ValueType::operator + (const ValueType &rhs)
{
	if (s && rhs.s)		// concatenate strings
	{
		char tmp[TMPSIZE];
		strcpy(tmp, s);
		strcat(tmp, rhs.s);
		return ValueType(tmp);
	}
	else if (s && !rhs.s)	// string + number
	{
		char tmp[TMPSIZE];
		strcpy(tmp, s);
		sprintf(tmp, "%ld", rhs.i);
		return ValueType(tmp);
	}
	else
	{
		return ValueType(i + rhs);
	}
}

/*
 * string/number compare operators
 */
#define COMPARE_OPERATION(op)	\
	ValueType ValueType::operator op (const ValueType &rhs) const	\
	{	\
		if (s && rhs.s)		\
			return ValueType(strcmp(s, rhs.s) op 0);	\
		else	\
			return ValueType(i op (long)rhs);	\
	}

COMPARE_OPERATION(!=);
COMPARE_OPERATION(==);
COMPARE_OPERATION(>);
COMPARE_OPERATION(<);
COMPARE_OPERATION(>=);
COMPARE_OPERATION(<=);

/*
 * print
 */
void ValueType::print() const
{
	if (!s)
		printf("value=%ld\n", i);
	else
		printf("value='%s'\n", s);
}

ValueType ValueType::zero((long)0);
