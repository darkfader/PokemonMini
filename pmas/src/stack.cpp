#include "stack.h"
#include "pmas.h"

template<> CharacterStack::T CharacterStack::peek()
{
	if (!something()) { eprintf("Cannot peek in empty stack!\n"); eexit(); }
	return *(_p-1);
}

template<> ValueStack::T ValueStack::peek()
{
	if (!something()) { eprintf("Cannot peek in empty stack!\n"); eexit(); }
	return *(_p-1);
}

template<> void CharacterStack::push(CharacterStack::T x)
{
	*_p++ = x;
}

template<> void ValueStack::push(ValueStack::T x)
{
	*_p++ = x;
}

template<> CharacterStack::T CharacterStack::pop()
{
	if (!something()) { eprintf("Cannot pop from empty stack!\n"); eexit(); }
	T r = *(--_p);
	return r;
}

template<> ValueStack::T ValueStack::pop()
{
	if (!something()) { eprintf("Cannot pop from empty stack!\n"); eexit(); }
	T r = *(--_p);
	_p->Free();
	return r;
}

