#ifndef _MEM_H
#define _MEM_H

struct RelocData
{
	unsigned char & operator [] (int i);
};

extern RelocData reloc_data;
extern int reloc_offset;
extern unsigned int addr;

void WriteOutput(const char *filename);
void UpdateMaxAddr();

#endif	// _MEM_H
