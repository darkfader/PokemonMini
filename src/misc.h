#ifndef _MISC_H
#define _MISC_H

/*
 * Defines
 */
#define TMPSIZE				1024
#define FREE(x)				{ if (x) free(x); x = 0; }
#ifdef DEBUG
	#define EEKS				if (1)
#else
	#define EEKS				if (0)
#endif
//#include <stdio.h>

/*
 * Variables
 */
extern const char endline_chars[];
extern const char delim_chars[];

/*
 * Prototypes
 */
char *strskipspace(char *s);
int strword(const char *s, const char *lookfor);
int strreplace(char *s, char *from, char *to);
int HexValue(char c);
char ParseStringChar(const char *&p);
void ParseString(char *q, const char *&p);

/*
 * isalpha2
 */
inline int isalpha2(int c)
{
	return ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'));
}

/*
 * isspace2
 */
inline bool isspace2(char c)
{
	return (c == ' ') || (c == '\t') || (c == '\r') || (c == '\n');
}

/*
 * isdigit2
 */
inline int isdigit2(int c)
{
	return (c >= '0') && (c <= '9');
}

/*
 * toupper2
 */
inline int toupper2(int c)
{
	return ((c >= 'a') && (c <= 'z')) ? c - 'a' + 'A' : c;
}

/*
 * isidentifier
 */
inline int isidentifier(char c, bool firstchar)
{
	return (c == '_') || isalpha2(c) || (!firstchar && isdigit2(c));
}

/*
 * isendline
 */
inline bool isendline(char c)
{
	return (c == '#') || (c == ';') || (c == '\r') || (c == '\n') || (c == 0);
}

#endif	// _MISC_H
