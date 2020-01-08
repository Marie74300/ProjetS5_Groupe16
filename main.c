#include <stdio.h>
#include <math.h>
#include "bfile.h"

int main(int argc, char ** argv)
{
	if (argc != 2) {
		printf("1 argument attendu.\n"); 
		return 1;
	}
	FILE *fich_o = fopen (argv[1], "r");
	if (fich_o == NULL){
		printf("Erreur ouverture en lecture.\n"); 
		return 2;
	}

	BFILE *myFile = bstart(fich_o, "r");
	int b;
	int n=0;
	int x=0;
	int hex = 0;
	int EI_CLASS;

	while(!beof(myFile))
	{
		if (x == 0 && n == 0) printf("MAGIC NUMBER [4 bytes]\n");
		if (x == 4 && n == 0) printf("FORMAT [1 byte]\n");
		if (x == 5 && n == 0) printf("ENDIANNESS [1 byte]\n");
		if (x == 6 && n == 0) printf("ORIGINAL/CURRENT [1 byte]\n");
		if (x == 7 && n == 0) printf("TARGET OPERATING SYSTEM [1 byte]\n");
		if (x == 8 && n == 0) printf("ABI VERSION [1 byte]\n");
		if (x == 9 && n == 0) printf("UNUSED [7 bytes]\n");
		if (x == 16 && n == 0) printf("OBJECT FILE TYPE [2 bytes]\n");
		if (x == 18 && n == 0) printf("INSTRUCTION SET ARCHITECTURE [2 bytes]\n");
		if (x == 20 && n == 0) printf("ORIGINAL VERSION [4 bytes]\n");
		
		if (EI_CLASS == 1) {
			if (x == 24 && n == 0) printf("ENTRY POINT [4 bytes]\n");
			if (x == 28 && n == 0) printf("HEADER TABLE [4 bytes]\n");
			if (x == 32 && n == 0) printf("SECTION HEADER TABLE [4 bytes]\n");
			if (x == 36 && n == 0) printf("FLAGS [4 bytes]\n");
			if (x == 40 && n == 0) printf("HEADER SIZE [2 bytes]\n");
			if (x == 42 && n == 0) printf("PROGRAM HEADER TABLE ENTRY SIZE [2 bytes]\n");
			if (x == 44 && n == 0) printf("NUMBER OF ENTRIES [2 bytes]\n");
			if (x == 46 && n == 0) printf("SECTION HEADER TABLE ENTRY SIZE [2 bytes]\n");
			if (x == 48 && n == 0) printf("NUMBER OF ENTRIES IN SECTION HEADER TABLE [2 bytes]\n");
			if (x == 50 && n == 0) printf("INDEX OF SHTE [2 bytes]\n");
			if (x == 52 && n == 0) printf("END OF HEADER\n\n\n");
		}
		else if (EI_CLASS == 2) {
			if (x == 24 && n == 0) printf("ENTRY POINT [8 bytes]\n");
			if (x == 32 && n == 0) printf("HEADER TABLE [8 bytes]\n");
			if (x == 40 && n == 0) printf("SECTION HEADER TABLE [8 bytes]\n");
			if (x == 48 && n == 0) printf("FLAGS [4 bytes]\n");
			if (x == 52 && n == 0) printf("HEADER SIZE [2 bytes]\n");
			if (x == 54 && n == 0) printf("PROGRAM HEADER TABLE ENTRY SIZE [2 bytes]\n");
			if (x == 56 && n == 0) printf("NUMBER OF ENTRIES [2 bytes]\n");
			if (x == 58 && n == 0) printf("SECTION HEADER TABLE ENTRY SIZE [2 bytes]\n");
			if (x == 60 && n == 0) printf("NUMBER OF ENTRIES IN SECTION HEADER TABLE [2 bytes]\n");
			if (x == 62 && n == 0) printf("INDEX OF SHTE [2 bytes]\n");
			if (x == 64 && n == 0) printf("END OF HEADER\n\n\n");
		}


		if(n == 0)printf("%d : ", x);
		b = bitread(myFile);
		printf("%d", b);

		if (b) hex += (int)pow(2, 8-n-1);
		
		n++;
		if(n == 4) printf(" ");
		if(x == 4 && n==8) EI_CLASS = hex;
		if(n == 8) {n=0; printf("\t%X\n", hex); x++; hex = 0;}
	}



	printf("\n");
	bstop(myFile);
	return 0;
}