#include "es.h"


// LECTURE

int read_Elf32_Addr (FILE *f, int endianess)
{
	if (endianess == 1)
	{
		int c;
		//Elf32_Addr = 4
		fread(&c, sizeof(Elf32_Addr), 1, f);
		return c;
	}
	else
	{
		int c1 = read_Elf32_Half(f, endianess);
		int c2 = read_Elf32_Half(f, endianess);
		return c1*256*256+c2;
	}
}

int read_Elf64_Addr (FILE *f, int endianess)
{
	if (endianess == 1)
	{
		int c;
		//Elf64_Addr = 8
		fread(&c, sizeof(Elf64_Addr), 1, f);
		return c;
	}
	else
	{
		int c1 = read_Elf32_Addr(f, endianess);
		int c2 = read_Elf32_Addr(f, endianess);
		return c1*256*256*256+c2;
	}
}

int read_Elf32_Half (FILE *f, int endianess)
{
	if (endianess == 1)
		{
		int c;
		//Elf32_Half = 2
		fread(&c, sizeof(Elf32_Half), 1, f);
		return c;
	}
	else
	{
		int c1 = read_unsigned_char(f);
		int c2 = read_unsigned_char(f);
		return c1*256+c2;
	}
}

int read_Elf32_Off (FILE *f, int endianess)
{
	if (endianess == 1)
	{
		int c;
		//Elf32_Off = 4
		fread(&c, sizeof(Elf32_Off), 1, f);
		return c;
	}
	else
	{
		int c1 = read_Elf32_Half(f, endianess);
		int c2 = read_Elf32_Half(f, endianess);
		return c1*256*256+c2;
	}
}

int read_Elf64_Off (FILE *f, int endianess)
{
	if (endianess == 1)
	{
		int c;
		//Elf64_Off = 8
		fread(&c, sizeof(Elf64_Off), 1, f);
		return c;
	}
	else
	{
		int c1 = read_Elf32_Off(f, endianess);
		int c2 = read_Elf32_Off(f, endianess);
		return c1*256*256*256+c2;
	}
}

int read_Elf32_Sword (FILE *f, int endianess)
{
	if (endianess == 1)
	{
		int c;
		//Elf32_Sword = 4
		fread(&c, sizeof(Elf32_Sword), 1, f);
		return c;
	}
	else
	{
		int c1 = read_Elf32_Half(f, endianess);
		int c2 = read_Elf32_Half(f, endianess);
		return c1*256*256+c2;
	}
}

int read_Elf32_Word (FILE *f, int endianess)
{
	if (endianess == 1)
	{
		int c;
		//Elf32_Word = 4
		fread(&c, sizeof(Elf32_Word), 1, f);
		return c;
	}
	else
	{
		int c1 = read_Elf32_Half(f, endianess);
		int c2 = read_Elf32_Half(f, endianess);
		return c1*256*256+c2;
	}
}

int read_Elf64_Word (FILE *f, int endianess)
{
	if (endianess == 1)
	{
		int c;
		//Elf64_Word = 8
		fread(&c, sizeof(Elf64_Word), 1, f);
		return c;
	}
	else
	{
		int c1 = read_Elf32_Word(f, endianess);
		int c2 = read_Elf32_Word(f, endianess);
		return c1*256*256*256+c2;
	}
}

int read_unsigned_char (FILE *f)
{
	int c;
	//unsigned char = 1
	fread(&c, sizeof(unsigned char), 1, f);
	c = c & 0x00FF;
	return c;
}

// ECRITURE


void write_quarter_word (int c, FILE *f)
{
	fwrite(&c, 1, 1, f);
}

void write_half_word (int c, FILE *f, int endianess)
{
	if (endianess == 1)
		fwrite(&c, 2, 1, f);
	else 
	{
		write_quarter_word ((c & 0xFF00) >> 8, f);
		write_quarter_word (c & 0x00FF, f);
	}
}
void write_word (int c, FILE *f, int endianess)
{
	if (endianess == 1)
		fwrite(&c, 4, 1, f);
	else 
	{
		write_half_word ((c & 0xFFFF0000) >> 16, f, endianess);
		write_half_word (c & 0x0000FFFF, f, endianess);
	}
}
void write_double_word (int c, FILE *f, int endianess)
{
	if (endianess == 1)
		fwrite(&c, 8, 1, f);
	else 
	{
		write_word ((c & 0xFFFFFFFF00000000) >> 32, f, endianess);
		write_word (c & 0x00000000FFFFFFFF, f, endianess);
	}
}








