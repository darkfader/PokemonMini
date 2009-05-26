//#include <stdio.h>
#include "stack.h"

int pushes = 0;
#define PUSHES(i)		//printf("pushes=%d\n", (pushes += i));
#define STACKCHECK()	//if (!something()) { eprintf("Stack is empty!\n"); eexit(); }

template<> void CharacterStack::push(CharacterStack::T x)
{
	*p++ = x;
}

template<> CharacterStack::T CharacterStack::pop()
{
	STACKCHECK();
	return *(--p);
}

template<> void ValueStack::push(ValueStack::T x)
{
	*p++ = x;
	PUSHES(+1);
}

template<> ValueStack::T ValueStack::pop()
{
	PUSHES(-1);
	STACKCHECK();
	ValueStack::T r = *(--p);
	p->Free();
	return r;
}
