#ifndef _MACROLIST_H
#define _MACROLIST_H

/*
 * Includes
 */
#include "stringlist.h"

/*
 * MacroList
 */
struct MacroList
{
	char *name;
	StringList *parameters;
	StringList *lines;
	MacroList *next;

	MacroList(const char *name);
	void AddParameter(char *string);
	void AddLine(char *string);
};

/*
 * Variables
 */
extern MacroList *current_macro;
extern int macro_id;		// reset to 0 for each pass

/*
 * Prototypes
 */
MacroList *FindMacro(const char *name);
MacroList *NewMacro(const char *name);
bool MacroExecute(const char *line);

#endif	// _MACROLIST_H
