/****************************************************************************\
Pika Macro DISassembler
by Rafael Vuijk (aka DarkFader)
http://darkfader.net/pm/

TODO:
	read a symbol file
	uh... macro's? :)
\****************************************************************************/

/*
 * Includes
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pmas.h"
#include "instruction.h"

/*
 * Variables
 */
Instruction instructions_00[256];
Instruction instructions_CE[256];
Instruction instructions_CF[256];
unsigned char *memory;
unsigned int size;

/*
 * tolower2
 */
inline int tolower2(int c)
{
	return (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c;
}

/*
 * InitInstructions
 */
void InitInstructions()
{
	const Instruction instructions[] =
	{
		#include "mindx.h"
	};
	
	#define INSTRUCTIONS		(sizeof(instructions) / sizeof(instructions[0]))

	memset(instructions_00, 0, sizeof(instructions_00));
	memset(instructions_CE, 0, sizeof(instructions_CE));
	memset(instructions_CF, 0, sizeof(instructions_CF));

	for (int i=0; i<INSTRUCTIONS; i++)
	{
		unsigned char extended = 0x00;
		if (instructions[i].flags & FLAG_EXTENDED) extended = instructions[i].fixed & 0xFF;
		switch (extended)
		{
			case 0x00: instructions_00[instructions[i].fixed & 0xFF] = instructions[i]; break;
			case 0xCE: instructions_CE[instructions[i].fixed >> 8 & 0xFF] = instructions[i]; break;
			case 0xCF: instructions_CF[instructions[i].fixed >> 8 & 0xFF] = instructions[i]; break;
			default: exit(1);
		}
	}
}

/*
 * ReadMiniFile
 */
int ReadMiniFile(char *filename)
{	
	FILE *fi = fopen(filename, "rb");
	if (!fi) { fprintf(stderr, "Cannot open input file!\n"); return -1; }
	fseek(fi, 0, SEEK_END);
	size = ftell(fi);
	memory = new unsigned char [size];
	fseek(fi, 0, SEEK_SET);
	fread(memory, 1, size, fi);
	fclose(fi);
	return 0;
}

/*
 * Disassemble
 */
unsigned int Disassemble(char *q, unsigned char *memory, unsigned int addr)
{
	char *qp = q; *qp = 0;
	unsigned int opcode_addr = addr;
	Instruction *instruction;
	switch (memory[addr])
	{
		case 0xCE: addr++; instruction = instructions_CE + memory[addr++]; break;
		case 0xCF: addr++; instruction = instructions_CF + memory[addr++]; break;
		default: instruction = instructions_00 + memory[addr++]; break;
	}

	if (instruction->fmt)
	{
		//unsigned char argbytes[MAX_ARGS];
		//for (unsigned int j=0; j<instruction->argbytes; j++) argbytes[j] = memory[addr++];
		
		char *s = instruction->fmt;
		while (*s)
		{
			if (*s == '~')
			{
				s++;
				int argsrc = *s++ - '0';
				unsigned int argflags = instruction->argInfo[argsrc].flags;
				unsigned int tmp = *(unsigned int *)(memory + addr);
				unsigned int bitmask = ~(0xFFFFFFFF << (argflags & ARGFLAG_BITS));
				int value = tmp & bitmask;

				if (argflags & ARGFLAG_HIGH) value <<= 8;
				
				if (argflags & ARGFLAG_S)
				{
					if (value & ((argflags & ARGFLAG_BITS)-1)) value -= (1 << (argflags & ARGFLAG_BITS));
				}

				if (argflags & ARGFLAG_REL)
				{
					value += instruction->size - 1;
					qp += sprintf(qp, ".%+d ", value);
					value += opcode_addr - 1;
					qp += sprintf(qp, ";@%06X", value);
				}
				else if ((argflags & ARGFLAG_BITS) >= 16)
				{
					qp += sprintf(qp, "$%04X", value);
				}
				else
				{
					qp += sprintf(qp, "$%02X", value);
				}
			}
			else
			{
				*qp++ = tolower2(*s++);
				*qp = 0;
			}
		}

		if (!*s) return opcode_addr + instruction->size;
	}

	qp = q;
	qp += sprintf(qp, ".db $%02X", memory[opcode_addr]);
	return opcode_addr + 1;
}

#ifdef VERSION

/*
 * main
 */
int main(int argc, char *argv[])
{
	InitInstructions();

	fprintf(stderr,
		"Pika Macro DISassembler "VERSION" (build " __DATE__ ") by Rafael Vuijk. http://darkfader.net/pm/\n"
	);
	
	if (argc < 3)
	{
		fprintf(stderr,
			"Syntax: pmdis input.min output.s\n"	//[symbols.sym]
			"Use '-' for output filename to use stdout.\n"
		);
		exit(0);
	}
	
	if (ReadMiniFile(argv[1]) < 0) exit(1);

	FILE *fo = strcmp(argv[2], "-") ? fopen(argv[2], "wt") : stdout;
	if (!fo) { fprintf(stderr, "Cannot open output file!\n"); exit(1); }
	fprintf(fo, "; disassembly of %s\n\n", argv[1]);
	unsigned int addr = 0;
	while (addr < size)
	{
		char s[256];
		fprintf(fo, "L%06X: ", addr);
		addr = Disassemble(s, memory, addr);
		fprintf(fo, "%s\n", s);
	}
	fclose(fo);

	return 0;
}

#endif
