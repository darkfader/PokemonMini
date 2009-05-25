#ifndef _STACK_H
#define _STACK_H

#include "valuetype.h"
#include "misc.h"

/*
 * Typedefs
 */
template <typename _T> struct Stack
{
	typedef _T T;
	T data[TMPSIZE], *p;
	Stack() { p = data; }
	~Stack() { empty(); }
	void empty() { while (something()) pop(); }
	void push(T x);// { *p++ = x; }
	T pop();
	T peek() { return *(p-1); }
	bool something() { return p != data; }
};

typedef Stack<ValueType> ValueStack;
typedef Stack<int> CharacterStack;			// char works too, int is probably faster

#endif	// _STACK_H
