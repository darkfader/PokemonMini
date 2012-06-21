#ifndef _STACK_H
#define _STACK_H

#include "valuetype.h"
#include "misc.h"
#include "pmas.h"

/*
 * Typedefs
 */
template <typename _T> struct Stack
{
	typedef _T T;
	T _top[TMPSIZE];
	T *_p;

	Stack() { _p = _top; }
	~Stack() { empty(); }
	void empty() { while (something()) pop(); }
	bool something() { return _p > _top; }
	void push(T x);
	T peek();
	T pop();
};

typedef Stack<ValueType> ValueStack;
typedef Stack<int> CharacterStack;			// char works too, int is probably faster

#endif	// _STACK_H
