/****************************************************************************\
mem.cpp

Memory is divided into parts
\****************************************************************************/

#include <stdio.h>
#include <fcntl.h>			// for _O_BINARY
#include <string.h>
#include "pmas.h"
#include "mem.h"

#define MEMSIZE			2*1024*1024		// max output size
#define	PARTSIZE		256

int reloc_offset;
unsigned int addr;
unsigned int maxaddr = 0;

struct MemPart
{
	unsigned char *data;
	unsigned int *access;

	MemPart()
	{
		data = 0;
	}
	
	~MemPart()
	{
		if (data) delete [] data;
	}
	
	void alloc()
	{
		data = new unsigned char [PARTSIZE];
		memset(data, option_fill, PARTSIZE);

		access = new unsigned int [PARTSIZE / sizeof(int)];
		memset(access, 0, PARTSIZE / sizeof(int));
	}

	operator unsigned char * ()
	{
		return data;
	}

	unsigned char & operator [] (int i)
	{
		if (!data) alloc();
		if (access[i / 32] & (1 << (i % 32))) { eprintf("Memory overlap.\n"); eexit(); }
		access[i / 32] |= 1 << (i % 32);
		return data[i];
	}
};

MemPart memParts[MEMSIZE / PARTSIZE];

unsigned char & RelocData::operator [] (int i)
{
	unsigned int addr = reloc_offset + i;
	if (/*(addr < 0) ||*/ (addr >= MEMSIZE)) { eprintf("Memory location out of range.\n"); eexit(); }
	return memParts[addr / PARTSIZE][addr % PARTSIZE];
}

RelocData reloc_data;

/*
 * WriteOutput
 */
void WriteOutput(const char *filename)
{
	MemPart emptyPart;
	emptyPart.alloc();

#ifdef WIN32
	if (!strcmp(filename, "-")) setmode(fileno(stdout), _O_BINARY);		// binary stdout
#endif
	FILE *fo = strcmp(filename, "-") ? fopen(filename, "wb") : stdout;
	if (!fo) { fprintf(stderr, "Error opening output file.\n"); eexit(); }

	for (unsigned int addr=option_base; addr<maxaddr; )
	{
		int offset = addr % PARTSIZE;
		int len = PARTSIZE - offset;
		if (addr + len >= maxaddr) len = maxaddr - addr;
		unsigned char *p = memParts[addr / PARTSIZE];
		if (!p) p = emptyPart;
		fwrite(p + offset, 1, len, fo);
		addr += len;
	}
	
	fclose(fo);
}

/*
 * UpdateMaxAddr
 */
void UpdateMaxAddr()
{
	if (addr + reloc_offset > maxaddr) maxaddr = addr + reloc_offset;
}
