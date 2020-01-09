#include "read.h"


// LECTURE

int read_Elf32_Addr (FILE *f, int endianess)
{
	if (endianess == 1)
	{
		int current;
		//Elf32_Addr = 4
		fread(&current, sizeof(Elf32_Addr), 1, f);
		return current;
	}
	else
	{
		int current1 = read_Elf32_Half(f, endianess);
		int current2 = read_Elf32_Half(f, endianess);
		return current1*256*256+current2;
	}
}

int read_Elf64_Addr (FILE *f, int endianess)
{
	if (endianess == 1)
	{
		int current;
		//Elf64_Addr = 8
		fread(&current, sizeof(Elf64_Addr), 1, f);
		return current;
	}
	else
	{
		int current1 = read_Elf32_Addr(f, endianess);
		int current2 = read_Elf32_Addr(f, endianess);
		return current1*256*256*256+current2;
	}
}

int read_Elf32_Half (FILE *f, int endianess)
{
	if (endianess == 1)
		{
		int current;
		//Elf32_Half = 2
		fread(&current, sizeof(Elf32_Half), 1, f);
		return current;
	}
	else
	{
		int current1 = read_unsigned_char(f);
		int current2 = read_unsigned_char(f);
		return current1*256+current2;
	}
}

int read_Elf32_Off (FILE *f, int endianess)
{
	if (endianess == 1)
	{
		int current;
		//Elf32_Off = 4
		fread(&current, sizeof(Elf32_Off), 1, f);
		return current;
	}
	else
	{
		int current1 = read_Elf32_Half(f, endianess);
		int current2 = read_Elf32_Half(f, endianess);
		return current1*256*256+current2;
	}
}

int read_Elf64_Off (FILE *f, int endianess)
{
	if (endianess == 1)
	{
		int current;
		//Elf64_Off = 8
		fread(&current, sizeof(Elf64_Off), 1, f);
		return current;
	}
	else
	{
		int current1 = read_Elf32_Off(f, endianess);
		int current2 = read_Elf32_Off(f, endianess);
		return current1*256*256*256+current2;
	}
}

int read_Elf32_Sword (FILE *f, int endianess)
{
	if (endianess == 1)
	{
		int current;
		//Elf32_Sword = 4
		fread(&current, sizeof(Elf32_Sword), 1, f);
		return current;
	}
	else
	{
		int current1 = read_Elf32_Half(f, endianess);
		int current2 = read_Elf32_Half(f, endianess);
		return current1*256*256+current2;
	}
}

int read_Elf32_Word (FILE *f, int endianess)
{
	if (endianess == 1)
	{
		int current;
		//Elf32_Word = 4
		fread(&current, sizeof(Elf32_Word), 1, f);
		return current;
	}
	else
	{
		int current1 = read_Elf32_Half(f, endianess);
		int current2 = read_Elf32_Half(f, endianess);
		return current1*256*256+current2;
	}
}

int read_Elf64_Word (FILE *f, int endianess)
{
	if (endianess == 1)
	{
		int current;
		//Elf64_Word = 8
		fread(&current, sizeof(Elf64_Word), 1, f);
		return current;
	}
	else
	{
		int current1 = read_Elf32_Word(f, endianess);
		int current2 = read_Elf32_Word(f, endianess);
		return current1*256*256*256+current2;
	}
}

int read_unsigned_char (FILE *f)
{
	int current;
	//unsigned currenthar = 1
	fread(&current, sizeof(unsigned char), 1, f);
	current = current & 0x00FF;
	return current;
}

// ECRITURE


void write_quarter_word (int current, FILE *f)
{
	fwrite(&current, 1, 1, f);
}

void write_half_word (int current, FILE *f, int endianess)
{
	if (endianess == 1)
		fwrite(&current, 2, 1, f);
	else 
	{
		write_quarter_word ((current & 0xFF00) >> 8, f);
		write_quarter_word (current & 0x00FF, f);
	}
}
void write_word (int current, FILE *f, int endianess)
{
	if (endianess == 1)
		fwrite(&current, 4, 1, f);
	else 
	{
		write_half_word ((current & 0xFFFF0000) >> 16, f, endianess);
		write_half_word (current & 0x0000FFFF, f, endianess);
	}
}
void write_double_word (int current, FILE *f, int endianess)
{
	if (endianess == 1)
		fwrite(&current, 8, 1, f);
	else 
	{
		write_word ((current & 0xFFFFFFFF00000000) >> 32, f, endianess);
		write_word (current & 0x00000000FFFFFFFF, f, endianess);
	}
}








