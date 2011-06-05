#ifndef _PMAS_H
#define _PMAS_H

/*
 * Includes
 */
#include <stdio.h>
#include "misc.h"

/*
 * Typedefs
 */
struct File
{
	FILE *fi;
	int line_num;
	char *filename;
	char origline[TMPSIZE];		// set in ParseLine, used to print error line
};

/*
 * Variables
 */
extern char locallabelprefix[];		// prefix for macro/local labels

extern int pass;					// pass 1..2
extern int option_range;			// range checking of immediate values (default = 0)
extern unsigned int option_base;	// start address of output file (default = 0)
extern int option_localjump;		// jumps to local labels default to short jumps? (default = 1)
extern int option_farjump;			// jumps to non-local labels and without suffix default to far jumps? (default = 1)
extern int option_word;				// *NOT WORKING YET* non-jumps without B or W suffix default to word? (default = 0)
extern int option_fill;				// byte to fill uninitialized data with

extern File *file;

/*
 * Prototypes
 */
void ParseFile(const char *filename);
void ParseLine(const char *file_line);
void eprintf(const char *fmt, ...);
void eexit();
void MacroLine(const char *file_line);

#endif	// _PMAS_H
