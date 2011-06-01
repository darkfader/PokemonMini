/****************************************************************************\
 Instruction
\****************************************************************************/

/*
 * Includes
 */
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include "pmas.h"
#include "mem.h"
#include "eval.h"
#include "instruction.h"

/*
 * Variables
 */
Instruction *instructions = 0;

/*
 * AddInstruction
 */
void AddInstruction(Instruction *instruction)
{
	instruction->next = instructions;
	instructions = instruction;
}

/*
 * FreeInstructions
 */
void FreeInstructions()
{
	while (instructions)
	{
		Instruction *instruction = instructions;
		instructions = instruction->next;
		delete instruction;
	}
}

/*
 * PlaceInstruction
 * pass == 2
 */
void PlaceInstruction(Instruction *instruction, unsigned int &addr, char args[2][TMPSIZE])
{
	unsigned int addr_instruction = addr;

//printf("%s\n", instruction->fmt);

//addr = addr_instruction + instruction->size;
//return;

	/*int args_evaluated[2];		// evaluated arguments
	for (int a=0; a<instruction->argnum; a++)
	{
		EEKS{printf("arg %d %s\n", a, args[a]);}
		args_evaluated[a] = ;
	}*/

	unsigned int opcode = instruction->fixed;

	//if (instructions[i].extended) reloc_data[addr++] = instructions[i].extended;
	//reloc_data[addr++] = instructions[i].opcode;

	//bool outofrange = false;

	for (unsigned int j=0; j<instruction->argnum; j++)
	{
		int argflags = instruction->argInfo[j].flags;
		int arg = EvaluateExpression(args[j]);	//argflags & ARGFLAG_SRCMASK];

//{printf("[%d] = %02X\n", j, arg);}

		if (argflags & ARGFLAG_REL)		// PC relative
		{
//printf("relative\n");
			arg -= addr_instruction + instruction->size - 1;
		}

//printf("%d bits\n", (argflags & ARGFLAG_BITS));
		unsigned int invbitmask = 0xFFFFFFFF << (argflags & ARGFLAG_BITS);
		unsigned int arg_rangecheck = (argflags & ARGFLAG_S) ? (arg + (1 << ((argflags & ARGFLAG_BITS)-1))) : arg;

//printf("%08X %08X\n", arg_rangecheck, invbitmask);
		if (arg_rangecheck & invbitmask)		// out of range
		{
			if ((argflags & ARGFLAG_REL) || option_range)	// give error?
			{
				//outofrange = true; break;
	
				const char *sign = "unsigned";
				if (argflags & ARGFLAG_S) sign = "signed";
				eprintf("Operand(s) out of range. Expected %s %d bits value.\n", sign, argflags & ARGFLAG_BITS);
				addr += instruction->size;
				return;
			}
		}

		if (argflags & ARGFLAG_HIGH) arg >>= 8;

		opcode |= (arg & ~invbitmask) << instruction->argInfo[j].shift;
	}

	for (unsigned int j=0; j<instruction->size; j++)
	{
		reloc_data[addr++] = opcode; opcode >>= 8;
	}
}

/*
 * TryInstruction
 */
bool TryInstruction(Instruction *instruction, char *line, int &skip, bool place, int *matchlevel)
{
	*matchlevel = 0;
	int incskip = 1;	// we can skip comparing until first space (arguments lengths are unknown)
	
	static char args[MAX_ARGS][TMPSIZE];			// operand arguments
	unsigned int argnumfound = 0;

	char *p = line + skip;
	char *fmt = instruction->fmt + skip;

	while (1)
	{
		if (*p == ' ') incskip = 0;

		if ((*fmt == 0) && (isspace2(*p) || (*p == 0)))		// end of instruction
		{
			if (argnumfound != instruction->argnum)
			{
				eprintf("Argument number mismatch (%d != %d).\n", argnumfound, instruction->argnum);
				eexit();
			}
			
			if (place)
			{
				if (pass == 1)
				{
					addr += instruction->size;
				}
				else	// if (pass == 2)
				{
					PlaceInstruction(instruction, addr, args);
				}
			}
			return true;
		}

		else if (*p == '\t')							// tab to space
		{
			*p = ' ';
			continue;
		}
		else if (isspace2(*p) && isspace2(*(p+1)))		// remove excess spaces
		{
			p++;
			continue;
		}

		else if (isspace2(*p) && (*fmt == '+'))		// mov a,[nn    +$xx]
		{
			p++;
			continue;
		}
		else if (isspace2(*p) && (*(fmt-1) == '+'))		// mov a,[nn+    $xx]
		{
			p++;
			continue;
		}

		else if (isspace2(*p) && (*fmt == ','))		// mov a ,1
		{
			p++;
			continue;
		}
		else if (isspace2(*p) && (*(fmt-1) == ','))		// mov a, 1
		{
			p++;
			continue;
		}

		else if ((*fmt == '~') && (*p != '['))
		{
			fmt++;
			if ((*fmt >= '0') && (*fmt <= '9'))
			{
				int argnum = *fmt++ - '0';
				
				char *endexpr;
				if (*(fmt - 3) == 'R')		// register
				{
					strtoul(p, &endexpr, 10);
				}
				else
				{
					endexpr = strpbrk(p, "],;#");			// TODO: allow comments characters in string-like values
				}

				if (!endexpr) endexpr = p + strlen(p);
				if (isendline(*endexpr)) *endexpr = 0;		// strip trailing comments
				// copy string for later use
				strncpy(args[argnum], p, endexpr-p);
				args[argnum][endexpr-p] = 0;
//EEKS{printf("[%d] := '%s'\n", argnum, args[argnum]);}

				argnumfound++;

				p = endexpr;
				continue;
			}
		}
		else if (toupper2(*p) == *fmt)
		{
			p++;
			fmt++;
			skip += incskip;
			(*matchlevel)++;
			continue;
		}
		else if (isspace2(*p) && (*fmt == 'B') && (*(fmt+1) == ' '))		// forgot 'B' suffix?	(matches before 'W' suffix)
		{
			fmt++;
			if (instruction->flags & FLAG_REL)
			{
//printf("relative B %s\n", line);

				if ((strskipspace(p)[0] == '_') && option_localjump)
				{
//printf("local\n");
					continue;
				}
				else if (option_farjump)
				{
//printf("option_farjump = 1\n");
					break;
				}

			}
//else printf("non-relative\n");
			continue;
		}
		else if (isspace2(*p) && (*fmt == 'W') && (*(fmt+1) == ' '))		// forgot 'W' suffix?
		{
			fmt++;
			if (instruction->flags & FLAG_REL)
			{
//printf("relative W %s\n", line);

				if ((strskipspace(p)[0] == '_') && option_localjump)
				{
//printf("local\n");
					break;
				}
				else if (option_farjump)
				{
//printf("option_farjump = 1\n");
					continue;
				}
			}
//else printf("non-relative\n");
			continue;
		}

EEKS{printf("%c %s\n", *p, fmt);}
		break;		// character mismatch                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
	}	// while

	return false;
}

/*
 * TryInstructions
 */
bool TryInstructions(char *line)
{
	int skip = 0;
	
	Instruction *bestintruction = 0;
	int bestmatchlevel = 0;
	
	for (Instruction *instruction = instructions; instruction; instruction = instruction->next)
	{
		//if (instruction->skip < skip) skip = instruction->skip;
		skip = 0;
		int matchlevel;
		if (TryInstruction(instruction, line, skip, false, &matchlevel))
		{
			//printf("%s %d\n", instruction->fmt, matchlevel);
			
			//!bestintruction || (instruction->argnum < bestintruction->argnum))
			if (matchlevel > bestmatchlevel)
			{
				bestintruction = instruction;
				bestmatchlevel = matchlevel;
			}
		}
	}
	
	if (bestintruction)
	{
		skip = 0;
		int matchlevel;
		TryInstruction(bestintruction, line, skip, true, &matchlevel);
		return true;
	}

	return false;
}
