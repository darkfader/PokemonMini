//#include <stdio.h>
#include "stack.h"

int pushes = 0;
#define PUSHES(i)		//printf("pushes=%d\n", (pushes += i));
#define STACKCHECK()	//if (!something()) { eprintf("Stack is empty!\n"); eexit(); }

void CharacterStack::push(CharacterStack::T x)
{
	*p++ = x;
}

CharacterStack::T CharacterStack::pop()
{
	STACKCHECK();
	return *(--p);
}

void ValueStack::push(ValueStack::T x)
{
	*p++ = x;
	PUSHES(+1);
}

ValueStack::T ValueStack::pop()
{
	PUSHES(-1);
	STACKCHECK();
	ValueStack::T r = *(--p);
	p->Free();
	return r;
}
