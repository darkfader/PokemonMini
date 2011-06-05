#ifndef _INSTRUCTION_H
#define _INSTRUCTION_H

/*
 * Defines
 */

#define FLAG_CONT			0x01		// can continue to next location
#define FLAG_REL			0x02		// PC relative (SHOULD THIS BE HERE?)
#define FLAG_EXTENDED		0x04		// only used in parsemindx

/*
 * Defines
 */

#define MAX_ARGS			4

// argument flags for type checking
#define ARGFLAG_U			0x00000040		// unsigned
#define ARGFLAG_S			0x00000080		// signed
#define ARGFLAG_4			0x00000004		// 4 bits
#define ARGFLAG_7			0x00000007		// 7 bits
#define ARGFLAG_8			0x00000008		// 8 bits
#define ARGFLAG_16			0x00000010		// 16 bits
#define ARGFLAG_BITS		0x0000001F

// argument flags for calculation
#define ARGFLAG_REL			0x00000100		// PC relative
#define ARGFLAG_HIGH		0x00000200		// shift argument by 8 bits

// argument flags for meta information
#define ARGFLAG_REG			0x00010000		// register
#define ARGFLAG_IMM			0x00020000		// only used in parsemindx
#define ARGFLAG_OFS			0x00040000		// only used in parsemindx
#define ARGFLAG_INT			0x00080000		// interrupt (only used in parsemindx)

/*
 * Typedefs
 */

struct ArgInfo
{
	unsigned int shift;
	unsigned int flags;
};

struct Instruction
{
	char *fmt;							// uppercase format string. delimiters ' ' and ','. arguments '~0' and '~1'
	unsigned int flags;					// instruction behaviour
	unsigned int fixed;
	unsigned int size;					// total size of instruction
	unsigned int argnum;				// number of arguments

	ArgInfo argInfo[MAX_ARGS];
	Instruction *next;
	
	~Instruction()
	{
		free(fmt);
	}
};

/*
 * Variables
 */
extern Instruction *instructions;

/*
 * Prototypes
 */
bool TryInstructions(const char *file_line);
void AddInstruction(Instruction *instruction);
void FreeInstructions();

#endif	// _INSTRUCTION_H
