#include <stdlib.h>
#include "pmas.h"
#include "misc.h"
#include "macrolist.h"
#include "symbol.h"

MacroList::MacroList(const char *name)
{
	this->name = strdup(name);
	next = 0;
	parameters = 0;
	lines = 0;
}

void MacroList::AddParameter(const char *string)
{
EEKS{printf("AddParameters '%s'\n", string);}
	if (!parameters) parameters = new StringList(string); else parameters->Add(string);
}

void MacroList::AddLine(const char *string)
{
	if (!lines) lines = new StringList(string); else lines->Add(string);
}

MacroList *macros = 0;
MacroList *current_macro;
int macro_id;		// reset to 0 for each pass

/*
 * FindMacro
 */
MacroList *FindMacro(const char *name)
{
	MacroList *m = macros;
	while (m) 
	{		
		if (!strcmp(name, m->name)) return m;
		m = m->next;
	}
	return m;
}

/*
 * NewMacro
 */
MacroList *NewMacro(const char *name)
{
	if (!macros) return (macros = new MacroList(name));
	MacroList *m = macros;
	while (m->next) m = m->next;
	m->next = new MacroList(name);
	return m->next;
}

/*
 * MacroExecute
 * ************** needs rewrite ****************
 */
bool MacroExecute(const char *line)
{
	char *_line = strdup(line);
	
EEKS{printf("exec %s\n", _line);}
	char *name = strtok(_line, delim_chars);
	MacroList *m = FindMacro(name);
	if (m)
	{
EEKS{printf("found macro at %p\n", m);}
		// process parameters
		StringList *paramname = m->parameters;
EEKS{StringList *p = m->parameters; while (p) { printf("parameters string '%s'\n", p->string); p = p->next; }}
		char *paramvalue = strtok(0, "");	// after macro name comes the first parameter
		if (paramvalue)
		{
			if (isspace2(paramvalue[0])) paramvalue = strskipspace(paramvalue);
			if (strchr(endline_chars, *paramvalue)) paramvalue = 0;		// non-space character is an end-of-line? then it's no parameter
EEKS{printf("at %p '%s'\n", paramname, paramvalue);}
		}

		while (paramname && paramvalue)
		{
EEKS{printf("paramname: '%s', paramvalue: '%s'\n", paramname->string, paramvalue);}
			if (paramname->string[0] == '$')		// force string
			{
				char *paramvalue2 = strtok(paramvalue, delim_chars);	// take string until delimiter
EEKS{printf("string param: '%s'\n", paramvalue2);}
				paramvalue = strtok(0, "");		// continue with the rest later
				if (paramvalue && strchr(endline_chars, *paramvalue)) paramvalue = 0;		// non-space character is an end-of-line? then it's no parameter
				ValueType paramvalue3(paramvalue2);
				SetSymbolValue(paramname->string+1, paramvalue3);
			}
			else	// expression
			{
				SetSymbolExpression(paramname->string, paramvalue, (const char **)&paramvalue);
			}
			paramname = paramname->next;
		}
		if (paramname) { eprintf("Too few macro parameters.\n"); return true; }
		if (paramvalue) { eprintf("Too many macro parameters.\n"); return true; }

		// macro ID (should be same for both passes)
		char backuplabelprefix[TMPSIZE];
		strcpy(backuplabelprefix, locallabelprefix);
		sprintf(locallabelprefix, "macro%d", ++macro_id);
		
		// paste all lines
		StringList *s = m->lines;
		while (s)
		{
			char tmp[TMPSIZE];
			strcpy(tmp, s->string);
			strcpy(file->origline, s->string);
EEKS{printf("exec %s\n", tmp);}
//			file->line = tmp;
			ParseLine(tmp);
			s = s->next;
		}
		
		strcpy(locallabelprefix, backuplabelprefix);
	}
	free(_line);
	return m ? true : false;
}

