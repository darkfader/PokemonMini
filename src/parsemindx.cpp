/*
 * Includes
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pmas.h"
#include "instruction.h"

/*
 *
 */

#define BUFSIZE			1024
#define MAX_OUTPUTS		1024
char *output[MAX_OUTPUTS];
int outputs = 0;

Instruction instruction;

char *rb[4] = {"A","B","L","H"};
char *rw[6] = {"BA","HL","X","Y","X1","X2"};
int rwnum[6] = {0, 1, 2, 3, 2, 3};

int line;
int extended = 0xAA;

/**
 * alphabetic sorting predicate
 */
int sort_output(const void *a, const void *b)
{
	if (!strcmp(*(char **)a, *(char **)b))
	{
		fprintf(stderr, "Duplicate item: %s", *(char **)a);
	}
	return strcmp(*(char **)a, *(char **)b);
}

/**
 * convert string to uppercase
 */
char *STRUPR(char *a)
{
	char *b = a;
	while (*b)
	{
		if (*b >= 'a' && *b <= 'z') *b -= 'a' - 'A';
		b++;
	}
	return a;
}

/**
 * convert string to lowercase
 */
char *STRLWR(char *a)
{
	char *b = a;
	while (*b)
	{
		if (*b >= 'A' && *b <= 'Z') *b += 'a' - 'A';
		b++;
	}
	return a;
}

/**
 * replace first occurrence in a string
 */
int strreplace(char *s, char *from, char *to)
{
	char tmp[BUFSIZE];
	char *p = strstr(s, from);
	if (p)
	{
		int pos = p-s;
		int len = strlen(from);
		strcpy(tmp, s);
		strcpy(tmp+pos, to);
		strcat(tmp, p+len);
		strcpy(s, tmp);
		return 1;
	}
	return 0;
}

/*
 * strreplacenum
 */
int strreplacenum(char *s, char *from, int to)
{
	char tmp[BUFSIZE];
	char *p = strstr(s, from);
	if (p)
	{
		int pos = p-s;
		int len = strlen(from);
		strcpy(tmp, s);
		sprintf(tmp+pos, "%d", to);
		strcat(tmp, p+len);
		strcpy(s, tmp);
		return 1;
	}
	return 0;
}

/*
 * strshift
 */
void strshift(char *s)
{
	char *p;
	while ((p = strstr(s, "<<")))
	{
		int a = atoi(p-1);
		int b = atoi(p+2);
		sprintf(p-1, "%02X", a<<b);
		strcpy(p+1, p+3);
	}
}

/*
 * stradd
 */
void stradd(char *s)
{
	char tmp[BUFSIZE];
	strcpy(tmp, s);
	int sum = 0;
	char *p = strtok(tmp, "+");
	while (p)
	{
		sum += strtoul(p,0,16);
		p = strtok(0, "+");
	}
	sprintf(s, "%02X", sum);
}

/*
 * strtrimtrailing
 */
void strtrimtrailing(char *s)
{
	if (!s) return;
	char *p = s + strlen(s);
	while (*(p-1) == ' ') p--;
	*p = 0;
}

/*
 * DoCalc
 */
void DoCalc(char *a, char *b)
{
	if (strstr(b, "+rw1"))
	{
		for (int i=0; i<6; i++)
		{
			char tmp_a[BUFSIZE]; strcpy(tmp_a, a); strreplace(tmp_a, "rw1", rw[i]);
			char tmp_b[BUFSIZE]; strcpy(tmp_b, b); strreplacenum(tmp_b, "rw1", rwnum[i]);
			DoCalc(tmp_a, tmp_b);
		}
	}
	else
	if (strstr(b, "+rw2"))
	{
		for (int i=0; i<6; i++)
		{
			char tmp_a[BUFSIZE]; strcpy(tmp_a, a); strreplace(tmp_a, "rw2", rw[i]);
			char tmp_b[BUFSIZE]; strcpy(tmp_b, b); strreplacenum(tmp_b, "rw2", rwnum[i]);
			DoCalc(tmp_a, tmp_b);
		}
	}
	else
	if (strstr(b, "+rw"))
	{
		for (int i=0; i<6; i++)
		{
			char tmp_a[BUFSIZE]; strcpy(tmp_a, a); strreplace(tmp_a, "rw", rw[i]);
			char tmp_b[BUFSIZE]; strcpy(tmp_b, b); strreplacenum(tmp_b, "rw", rwnum[i]);
			DoCalc(tmp_a, tmp_b);
		}
	}
	else
	if (strstr(b, "+rb1"))
	{
		for (int i=0; i<4; i++)
		{
			char tmp_a[BUFSIZE]; strcpy(tmp_a, a); strreplace(tmp_a, "rb1", rb[i]);
			char tmp_b[BUFSIZE]; strcpy(tmp_b, b); strreplacenum(tmp_b, "rb1", i);
			DoCalc(tmp_a, tmp_b);
		}
	}
	else
	if (strstr(b, "+rb2"))
	{
		for (int i=0; i<4; i++)
		{
			char tmp_a[BUFSIZE]; strcpy(tmp_a, a); strreplace(tmp_a, "rb2", rb[i]);
			char tmp_b[BUFSIZE]; strcpy(tmp_b, b); strreplacenum(tmp_b, "rb2", i);
			DoCalc(tmp_a, tmp_b);
		}
	}
	else
	if (strstr(b, "+rb"))
	{
		for (int i=0; i<4; i++)
		{
			char tmp_a[BUFSIZE]; strcpy(tmp_a, a); strreplace(tmp_a, "rb", rb[i]);
			char tmp_b[BUFSIZE]; strcpy(tmp_b, b); strreplacenum(tmp_b, "rb", i);
			DoCalc(tmp_a, tmp_b);
		}
	}
	else
	{	
	//fprintf(stderr, "1 %s %s\n", a, b);
		strshift(b);
	//fprintf(stderr, "2 %s %s\n", a, b);
		stradd(b);
	//fprintf(stderr, "3 %s %s\n", a, b);
		STRUPR(a);

		strtrimtrailing(a);
		strtrimtrailing(b);

		int numargs = 0;
		if (strreplace(a, "*", "~0")) numargs++;
		if (strreplace(a, "*", "~1")) numargs++;
		if (strreplace(a, "*", "~2")) numargs++;
		// .....

		//strreplace(b, ":", "");

		instruction.fmt = a;

		//instruction.opcode = strtoul(b,0,16);
		//instruction.fixed |= strtoul(b,0,16) << ((instruction.flags & FLAG_EXTENDED) ? 1 : 0);
		unsigned int instruction_fixed = instruction.fixed | (strtoul(b,0,16) << ((instruction.flags & FLAG_EXTENDED) ? 8 : 0));

		// no continue flag
		if (!(instruction.flags & FLAG_EXTENDED)) switch (instruction.fixed & 0xFF)
		{
			case 0xF1:
			case 0xF3:
			case 0xF4:
			case 0xF8:
			case 0xF9:
				break;

			default:
				instruction.flags |= FLAG_CONT;
		}

		char s[BUFSIZE], *p = s;

		// NOTE: leading text is added later
		// see mindx_type.h for structure fields
		p += sprintf(p, "%s\"", instruction.fmt);
		//for (int i=0; i<30-(p-s); i++) { p += sprintf(p, " "); }
		p += sprintf(p, ",0x%02X,0x%02X,%d,%d", instruction.flags, instruction_fixed, instruction.size, instruction.argnum);
		for (int i=0; i<instruction.argnum; i++)
		{
			p += sprintf(p, ",%d,%d", instruction.argInfo[i].shift, instruction.argInfo[i].flags);
		}
		p += sprintf(p, "\t; %d\n", line);


/*		p += sprintf(p, "%s\",0x%02X,0x%02X,0x%02X,%d,%d,%d,{", instruction.fmt, instruction.flags, instruction.extended, instruction.opcode, instruction.size, instruction.argnum, instruction.argbytes);
		for (int i=0; i<MAX_ARGS; i++)
		{
			p += sprintf(p, "0x%03X,", instruction.argflags[i]);
		}
		//%d,%d",  flags, numargs,
		p += sprintf(p, "}},//%d\n", line);
*/
		output[outputs++] = strdup(s);		// this is going to be sorted alphabetically
	}
}


/*
 *
 */
bool ParseLine(char *cs)
{
	if (*cs == ' ') cs++;
	if (*cs == '%') cs++;

	char s[BUFSIZE];
	strcpy(s, cs);
	
	// extended sets
	if (strstr(s, "INSTRUCTION SET") == s) extended = 0x00;
	if (strstr(s, "EXTENDED ") == s) { extended = s[9] - '0' + 0xCE; }

	if ((s[0] != '|') && (s[0] != '+')) return false;
	
	int repaired = 0;
/*
	repaired |= strreplace(s, "jc disp8", "jcb disp8");
	repaired |= strreplace(s, "jnc disp8", "jncb disp8");
	repaired |= strreplace(s, "jz disp8", "jzb disp8");
	repaired |= strreplace(s, "jnz disp8", "jnzb disp8");

	repaired |= strreplace(s, "jc disp16", "jcw disp16");
	repaired |= strreplace(s, "jnc disp16", "jncw disp16");
	repaired |= strreplace(s, "jz disp16", "jzw disp16");
	repaired |= strreplace(s, "jnz disp16", "jnzw disp16");

	repaired |= strreplace(s, "jmp disp16", "jmpw disp16");
	repaired |= strreplace(s, "call disp16", "callw disp16");
	repaired |= strreplace(s, "callc disp16", "callcw disp16");
	repaired |= strreplace(s, "callnc disp16", "callncw disp16");
	repaired |= strreplace(s, "callz disp16", "callzw disp16");
	repaired |= strreplace(s, "callnz disp16", "callnzw disp16");

	repaired |= strreplace(s, "jmp disp8", "jmpb disp8");
	repaired |= strreplace(s, "call disp8", "callb disp8");
	repaired |= strreplace(s, "callc disp8", "callcb disp8");
	repaired |= strreplace(s, "callnc disp8", "callncb disp8");
	repaired |= strreplace(s, "callz disp8", "callzb disp8");
	repaired |= strreplace(s, "callnz disp8", "callnzb disp8");
*/
	//repaired |= strreplace(s, "CE:B0+rb", "CF:B0+rb");
	//repaired |= strreplace(s, "H| ", "H | ");
	//repaired |= strreplace(s, "pop rb", "popb rb");
	//repaired |= strreplace(s, "ofL:ofH", "ofs16L:ofs16H");
	//repaired |= strreplace(s, "of16L:of16H", "ofs16L:ofs16H");
	//repaired |= strreplace(s, "dispL:dispH", "disp16L:disp16H");
	//repaired |= strreplace(s, "D0+rw", "D0+rw:imm16L:imm16H");
	//repaired |= strreplace(s, "D4+rw", "D4+rw:imm16L:imm16H");
	//repaired |= strreplace(s, "(???)", "");
	//repaired |= strreplace(s, "(BCD xchg)", "");
	//repaired |= strreplace(s, "(BCD decode)", "");
	//repaired |= strreplace(s, "(BCD encode)", "");
	//repaired |= strreplace(s, "???", "NOIDEA imm8");
	//repaired |= strreplace(s, ":??", ":imm8");
	//repaired |= strreplace(s, "dis8", "disp8");
	//repaired |= strreplace(s, "dis16", "disp16");
	//repaired |= strreplace(s, "im8", "imm8");
	//repaired |= strreplace(s, "im16", "imm16");
	//repaired |= strreplace(s, ", ", ",");

	if (repaired)
	{
		fprintf(stderr, "Repaired line %d %s", line, s);
	}

	if (extended == 0xCE) strreplace(s, "CE:", "");
	if (extended == 0xCF) strreplace(s, "CF:", "");

	char a[BUFSIZE],b[BUFSIZE],c[BUFSIZE];
	int n = sscanf(s, "| %[^|] | %[^|] | %[^|] |", a,b,c);
	if (n < 1) return false;
	if (a[0] == '|') return false;
	

	memset(&instruction, 0, sizeof(instruction));

	//fprintf(stderr, "%-10s %s\n", a, b);

	if (extended)
	{
		instruction.flags |= FLAG_EXTENDED;
		instruction.size = 2;
		instruction.fixed |= extended << 0;
	}
	else
	{
		instruction.size = 1;
	}
	//instruction.extended = extended;
	//if (extended) instruction.size++;

	//instruction.argbytes = 0;
	//instruction.argnum = 0;

	
	// disp = relative !
	
	// replace some stuff
	for (int i=0; i<2; i++)
	{
		strreplace(a, "dis8", "disp8");		strreplace(b, "dis8", "disp8");
		strreplace(a, "dis16", "disp16");	strreplace(b, "dis16", "disp16");

		//strreplace(a, "ofs8", "imm8");		strreplace(b, "ofs8", "imm8");
		//strreplace(a, "ofs16", "imm16");	strreplace(b, "ofs16", "imm16");
		
		strreplace(a, "im8", "imm8");		strreplace(b, "im8", "imm8");
		strreplace(a, "im16", "imm16");		strreplace(b, "im16", "imm16");
	}

//fprintf(stderr, "%s | %s\n", a, b);


	// NOTE: assume disp comes before imm


	if (strreplace(a, "intNB", "*"))
	{
		if (strreplace(b, ":intNB<<1", "")) {
			instruction.argInfo[instruction.argnum].flags |= ARGFLAG_INT | ARGFLAG_U | ARGFLAG_7;
			instruction.argInfo[instruction.argnum].shift = instruction.size * 8 + 1; instruction.size++;
			instruction.argnum++;
		}
		else { printf("Error at line %d: %s", line, s); exit(1); }
	}

	if (strreplace(a, "disp8", "*"))
	{
		if (strreplace(b, ":disp8", "")) {
			instruction.argInfo[instruction.argnum].flags |= ARGFLAG_S | ARGFLAG_REL | ARGFLAG_8; instruction.flags |= FLAG_REL;
			instruction.argInfo[instruction.argnum].shift = instruction.size * 8; instruction.size++;
			instruction.argnum++;
		}
		else { printf("Error at line %d: %s", line, s); exit(1); }
	}

	if (strreplace(a, "disp16", "*"))
	{
		if (strreplace(b, ":disp16L:disp16H", ""))
		{
			instruction.argInfo[instruction.argnum].flags |= ARGFLAG_S | ARGFLAG_REL | ARGFLAG_16; instruction.flags |= FLAG_REL;
			instruction.argInfo[instruction.argnum].shift = instruction.size * 8; instruction.size += 2;
			instruction.argnum++;
		}
		else if  (strreplace(b, ":disp16H", ""))
		{
			instruction.argInfo[instruction.argnum].flags |= ARGFLAG_S | ARGFLAG_REL | ARGFLAG_16 | ARGFLAG_HIGH; instruction.flags |= FLAG_REL;
			instruction.argInfo[instruction.argnum].shift = instruction.size * 8; instruction.size += 1;
			instruction.argnum++;
		}
		else { printf("Error at line %d: %s", line, s); exit(1); }
	}

	if (strreplace(a, "imm8", "*"))
	{
		if (strreplace(b, ":imm8", "")) {
			//instruction.size++; instruction.argflags[instruction.argbytes++] |= instruction.argnum | ARGFLAG_U | ARGFLAG_IMM;

			instruction.argInfo[instruction.argnum].flags |= ARGFLAG_U | ARGFLAG_IMM | ARGFLAG_8;
			instruction.argInfo[instruction.argnum].shift = instruction.size * 8; instruction.size += 1;
			instruction.argnum++;
		}
		else { printf("Error at line %d: %s", line, s); exit(1); }
	}

	if (strreplace(a, "imm16", "*"))
	{
		if (strreplace(b, ":imm16L:imm16H", ""))
		{
			//instruction.size++; instruction.argflags[instruction.argbytes++] |= instruction.argnum | ARGFLAG_U | ARGFLAG_IMM | ARGFLAG_16;
			//instruction.size++; instruction.argflags[instruction.argbytes++] |= instruction.argnum | ARGFLAG_U | ARGFLAG_IMM | ARGFLAG_16 | ARGFLAG_HIGH;

			instruction.argInfo[instruction.argnum].flags |= ARGFLAG_U | ARGFLAG_IMM | ARGFLAG_16;
			instruction.argInfo[instruction.argnum].shift = instruction.size * 8; instruction.size += 2;
			instruction.argnum++;
		}
		else if (strreplace(b, ":imm16H", ""))
		{
			//instruction.size++; instruction.argflags[instruction.argbytes++] |= instruction.argnum | ARGFLAG_U | ARGFLAG_IMM | ARGFLAG_16 | ARGFLAG_HIGH;

			instruction.argInfo[instruction.argnum].flags |= ARGFLAG_U | ARGFLAG_IMM | ARGFLAG_16 | ARGFLAG_HIGH;
			instruction.argInfo[instruction.argnum].shift = instruction.size * 8; instruction.size += 1;
			instruction.argnum++;
		}
		else { printf("Error at line %d: %s", line, s); exit(1); }
	}

	if (strreplace(a, "ofs8", "*"))
	{
		if (strreplace(b, ":ofs8", "")) {
			//instruction.size++; instruction.argflags[instruction.argbytes++] |= instruction.argnum | ARGFLAG_OFS;

			instruction.argInfo[instruction.argnum].flags |= ARGFLAG_OFS | ARGFLAG_8 | ARGFLAG_U;
			instruction.argInfo[instruction.argnum].shift = instruction.size * 8; instruction.size += 1;
			instruction.argnum++;
		}
		else { printf("Error at line %d: %s", line, s); exit(1); }
	}

	if (strreplace(a, "ofs16", "*"))
	{
		if (strreplace(b, ":ofs16L:ofs16H", ""))
		{
			//instruction.size++; instruction.argflags[instruction.argbytes++] |= instruction.argnum | ARGFLAG_OFS | ARGFLAG_16;
			//instruction.size++; instruction.argflags[instruction.argbytes++] |= instruction.argnum | ARGFLAG_OFS | ARGFLAG_16 | ARGFLAG_HIGH;

			instruction.argInfo[instruction.argnum].flags |= ARGFLAG_OFS | ARGFLAG_16 | ARGFLAG_U;
			instruction.argInfo[instruction.argnum].shift = instruction.size * 8; instruction.size += 2;
			instruction.argnum++;
		}
		else if (strreplace(b, ":ofs16H", ""))
		{
			//instruction.size++; instruction.argflags[instruction.argbytes++] |= instruction.argnum | ARGFLAG_OFS | ARGFLAG_16 | ARGFLAG_HIGH;

			instruction.argInfo[instruction.argnum].flags |= ARGFLAG_OFS | ARGFLAG_16 | ARGFLAG_HIGH | ARGFLAG_U;
			instruction.argInfo[instruction.argnum].shift = instruction.size * 8; instruction.size += 1;
			instruction.argnum++;
		}
		else { printf("Error at line %d: %s", line, s); exit(1); }
	}

	DoCalc(a,b);

	//if (line == 496) { fprintf(stderr, "%s %s\n", a, b); exit(1); }			// ******************************

	// other syntax
//	if (strstr(s, " mov")) { strreplace(s, " mov", " ld"); ParseLine(s); }
/*
	if (strstr(s, " subc")) { strreplace(s, " subc", " sbc"); ParseLine(s); }
	else if (strstr(s, " addc")) { strreplace(s, " addc", " adc"); ParseLine(s); }
	else if (strstr(s, " int ")) { strreplace(s, " int ", " cint "); ParseLine(s); }
	else if (strstr(s, " test")) { strreplace(s, " test", " tst"); ParseLine(s); }
	else if (strstr(s, " jdbnz")) { strreplace(s, " jdbnz", " djnz"); ParseLine(s); }
	else if (strstr(s, " mov")) { strreplace(s, " mov", " ld"); ParseLine(s); }
	else if (strstr(s, " xchg")) { strreplace(s, " xchg", " ex"); ParseLine(s); }
	else if (strstr(s, " call")) { strreplace(s, " call", " calr"); ParseLine(s); }
	else if (strstr(s, " jmp")) { strreplace(s, " jmp", " jp"); ParseLine(s); }
	else if (strstr(s, " not")) { strreplace(s, " not", " cpl"); ParseLine(s); }
	else if (strstr(s, " rol")) { strreplace(s, " rol", " rl"); ParseLine(s); }
	else if (strstr(s, " ror")) { strreplace(s, " ror", " rr"); ParseLine(s); }
	else if (strstr(s, " sal")) { strreplace(s, " sal", " sla"); ParseLine(s); }
	else if (strstr(s, " sar")) { strreplace(s, " sar", " sra"); ParseLine(s); }
	else if (strstr(s, "FLAGS")) { while (strreplace(s, "FLAGS", "f")) {}; ParseLine(s); }
*/	
	return true;
}

/*
 * ReadMindxTxt
 */
void ReadMindxTxt(char *filename)
{
	FILE *f = fopen(filename, "rt");
	if (!f) { fprintf(stderr, "Cannot open file!\n"); exit(1); }
	
	line = 0;
	extended = 0;

	while (1)
	{
		char s[BUFSIZE];
		if (!fgets(s, BUFSIZE-1, f)) break;
		line++;
		//printf("%d %s", line, s);

		if (!ParseLine(s)) continue;
	}

	fclose(f);
	memset(&instruction, 0, sizeof(instruction));
}

/*
 * WriteMindxH
 */
void WriteMindxH(char *filename)
{
	FILE *f = fopen(filename, "wt");
	if (!f) { fprintf(stderr, "Cannot open file!\n"); exit(1); }
	for (int i=0; i<outputs; i++)
	{
		//strreplace(output[i], "\xDF ", "B ");

		fprintf(f, ".instruction \"%s", output[i]);
	}
	fclose(f);
}

/*
 * WriteHighlight
 * Output contains duplicates!
 */
void WriteHighlight(char *filename)
{
	FILE *f = fopen(filename, "wt");
	if (!f) { fprintf(stderr, "Cannot open file!\n"); exit(1); }
	for (int i=0; i<outputs; i++)
	{
		STRLWR(output[i]);
		char *p = strpbrk(output[i], " \"");
		if (p) *p = 0; else p = strchr(output[i], 0);
		fprintf(f, "%s\n", output[i]);
		if ((*(p-1) == 'b') || (*(p-1) == 'w'))			// also make a default name without any B or W suffix
		{
			*(p-1) = 0;
			fprintf(f, "%s\n", output[i]);
		}
	}
	fclose(f);
}

/*
 *
 */
int main(int argc, char *argv[])
{
	//printf("%02X\n", stradd("10+20+30")); exit(1);
	ReadMindxTxt(argv[1]);

	/*
	 * not yet in mindx.txt... don't forget to remove when newer mindx.txt is out !
	 */

	line = 0;
	extended = 0;

	//ParseLine("| bkpt | FE | | |");
	
	extended = 0xCE;

	/*
	 * make B come after W
	 */
	/*for (int i=0; i<outputs; i++)
	{
		strreplace(output[i], "B ", "\xDF ");
	}*/

	/*
	 *
	 */
	qsort(output, outputs, sizeof(char *), sort_output);
	

	WriteMindxH(argv[2]);

	WriteHighlight(argv[3]);

	return 0;
}
