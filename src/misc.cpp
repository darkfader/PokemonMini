/*
 * Includes
 */
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "misc.h"

/*
 * Variables
 */
const char endline_chars[] = "#;\r\n";			// match with 'isendline'. Note: the expression evaluator checks for it too and handles strings correctly.
const char delim_chars[] = ", \t\r\n";			// use with strtok

/*
 * strskipspace
 * return next word
 */
char *strskipspace(char *s)
{
	while (*s && !isspace2(*s)) s++;	// skip current word
	while (isspace2(*s)) s++;	// skip spaces
	return s;
}

/*
 * strword
 * check begin of word
 */
int strword(const char *s, const char *lookfor)
{
	while (*s && *lookfor && (*s == *lookfor)) { s++; lookfor++; }
	return ((*lookfor == 0) && (isspace2(*s) || (*s == 0)));
}

/*
 * strreplace
 * replace once
 */
int strreplace(char *s, char *from, char *to)
{
	char *where = strstr(s, from);
	if (where)	// found?
	{
		char tmp[TMPSIZE];
		strcpy(tmp, where + strlen(from));
//printf("tmp='%s'\n", tmp);
		strcpy(where, to);
		strcat(where, tmp);
//printf("result='%s'\n", s); eexit();
		return 1;
	}
	return 0;
}

/*
 * HexValue
 */
int HexValue(char c)
{
	const char hexChars[] = "0123456789ABCDEF";
	return strchr(hexChars, toupper2(c)) - hexChars;
}

/*
 * ParseStringChar
 * returns next character value
 * handles few escape sequences including hexadecimal notation
 */
char ParseStringChar(const char *&p)
{
	if (*p == '\\')		// special character
	{
		p++;
		switch (*p)
		{
			case 'r': p++; return '\r';
			case 'n': p++; return '\n';
			case 't': p++; return '\t';
			case '0': p++; return '\0';
			case 'x':
			{
				p++;
				int c = HexValue(*p++) << 4;
				return c | HexValue(*p++);
			}
			default: return *p++;
		}
	}
	else
	{
		return *p++;
	}
}

/*
 * ParseString
 */
char *ParseString(const char *&p)
{
	char *s = (char *)malloc(TMPSIZE), *q = s;
	const char *delims = delim_chars;
	if (*p == '"') { delims = "\""; p++; }
	while (*p && !strchr(delims, *p))
	{
		*q++ = ParseStringChar(p);
	}
	*q++ = 0;		// terminate
	if (*p) p++;	// skip delim
	return s;
}

