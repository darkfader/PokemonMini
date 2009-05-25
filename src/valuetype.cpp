#include <string.h>
#include <stdlib.h>
#include "pmas.h"
#include "valuetype.h"
#include "eval.h"

int hmmm = 0;
#define HMMM(i)			//printf("hmmmmm=%d\n", (hmmm += i));


ValueType::ValueType() { /*printf("ValueType()\n");*/ s = 0; i = 0; }

ValueType::ValueType(const ValueType &rhs)
{
	//Free();
	//printf("ValueType(const ValueType &rhs)\n");
	/*if (rhs.s == undefined) this->s = undefined;
	else if (rhs.s == 0) { this->s = 0; this->i = rhs.i; }
	else*/
	//if (rhs.s) printf("strdup %s\n", rhs.s);
	
	//if (s) exit(42);
	//Free();
	
	s = rhs.s ? strdup(rhs.s) : 0;
	i = rhs.i;
	if (s) HMMM(+1);
	
	//print();
}

ValueType::ValueType(char *s) { this->s = strdup(s); i=0; HMMM(+1); }
ValueType::ValueType(long i) { s = 0; this->i = i; }


ValueType& ValueType::operator = (const ValueType &rhs) 
{
	if (this == &rhs) { eprintf("Assign to myself!\n"); eexit(); }
	//if (s) exit(42);
	
	Free();
	
	//printf("operator =\n");
	
	/*if (rhs.s == undefined) this->s = undefined;
	else if (rhs.s == 0) { this->s = 0; this->i = rhs.i; }
	else this->s = strdup(rhs.s);*/

	//Free();
	//if (rhs.s) printf("strdup2 %s\n", rhs.s);

	s = rhs.s ? strdup(rhs.s) : 0;
	i = rhs.i;
	if (s) HMMM(+1);
	
	//print();
	//ValueType(rhs);

  	return *this;
}

//ValueType(ValueType &other) { Free(); this->s = strdup(other.s); this->i = other.i; }

//ValueType & operator = (char *s) { ValueType(s); return *this; }
//ValueType & operator = (long i) { ValueType(i); return *this; }

//void setString(char *s) { this->s = strdup(s); i=0; }

void ValueType::Free() { if (s) { HMMM(-1); /*printf("deleting %08X '%s'\n", s, s); */ s[0] = '0'; free(s); s = 0; } }
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
		if (n.getString()) eprintf("Invalid string operation.\n"); return 0;
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
		sprintf(tmp, "%d", rhs.i);
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
		printf("value=%d\n", i);
	else
		printf("value='%s'\n", s);
}

ValueType ValueType::zero((long)0);
