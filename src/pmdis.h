#ifndef _PMDIS_H
#define _PMDIS_H

extern unsigned char *memory;
extern unsigned int size;

void InitInstructions();
unsigned int Disassemble(char *q, unsigned char *memory, unsigned int addr);
int ReadMiniFile(char *filename);

#endif	// _PMDIS_H
