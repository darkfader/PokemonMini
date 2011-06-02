#ifndef _STRINGLIST_H
#define _STRINGLIST_H

#include <string.h>

struct StringList
{
	char *string;
	StringList *next;

	StringList(const char *string)
	{
		this->string = strdup(string);
		next = 0;
	}
	
	void Add(const char *string)
	{
		StringList *p = this;
		while (p->next) p = p->next;
		p->next = new StringList(string);
	}
};

#endif	// _STRINGLIST_H
