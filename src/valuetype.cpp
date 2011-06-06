#include <string.h>
#include <stdlib.h>
#include "pmas.h"
#include "valuetype.h"
#include "eval.h"

ValueType::ValueType()
{
	_s = NULL;
	_i = 0;
	_defined = false;
}

ValueType::ValueType(const ValueType &rhs)
{
	_s = rhs._s ? strdup(rhs._s) : NULL;
	_i = rhs._i;
	_defined = rhs._defined;
}

ValueType::ValueType(const char *s)
{
	_s = strdup(s);
	_i = 0;
	_defined = true;
}

ValueType::ValueType(long i)
{
	_s = NULL;
	_i = i;
	_defined = true;
}

ValueType& ValueType::operator = (const ValueType &rhs) 
{
	if (this == &rhs) { eprintf("Assign to myself!\n"); eexit(); }
	Free();
	_s = rhs._s ? strdup(rhs._s) : NULL;
	_i = rhs._i;
	_defined = rhs._defined;
  	return *this;
}

void ValueType::Free()
{
	if (_s != NULL)
	{
		_s[0] = '0';
		free(_s);
		_s = NULL;
	}
	_i = 0;
	_defined = false;
}

ValueType::~ValueType()
{
	Free();
}

/*
 * operator long
 * evaluate expression to number
 */
ValueType::operator long () const
{
	if (_s != NULL)
	{
		ValueType n = EvaluateExpression(_s);
		if (n.getString() != NULL) eprintf("Invalid string operation.\n"); return 0;
		return (long)n;
	}
	return _i;
}

/*
 * getString
 * returns NULL when it's a number
 */
const char * ValueType::getString() const
{
	return _s;
}

/*
 * undefine
 */
void ValueType::undefine()
{
	Free();
}

/*
 * defined
 */
bool ValueType::defined()
{
	return _defined;
}

/*
 * operator +
 */
ValueType ValueType::operator + (const ValueType &rhs)
{
	if (!_defined || !rhs._defined) { eprintf("Trying to add with undefined value!\n"); eexit(); }
	if (_s && rhs._s)		// concatenate strings
	{
		char tmp[TMPSIZE];
		strcpy(tmp, _s);
		strcat(tmp, rhs._s);
		return ValueType(tmp);
	}
	else if (_s && !rhs._s)	// string + number
	{
		char tmp[TMPSIZE];
		strcpy(tmp, _s);
		sprintf(tmp, "%ld", rhs._i);
		return ValueType(tmp);
	}
	else
	{
		return ValueType(_i + rhs);
	}
}

/*
 * string/number compare operators
 */
#define COMPARE_OPERATION(op)	\
	ValueType ValueType::operator op (const ValueType &rhs) const	\
	{	\
		if (!_defined || !rhs._defined) { eprintf("Trying to compare with undefined value!\n"); eexit(); }	\
		if (_s && rhs._s)		\
			return ValueType(strcmp(_s, rhs._s) op 0);	\
		else	\
			return ValueType(_i op (long)rhs);	\
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
	if (!_defined)
		printf("value=undefined\n");
	else if (_s != NULL)
		printf("value='%s'\n", _s);
	else
		printf("value=%ld\n", _i);
}

ValueType ValueType::zero((long)0);

