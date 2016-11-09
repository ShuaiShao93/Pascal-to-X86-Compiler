#include "compiler.h"

struct err errtab[ERRMAX];
int nerr = 0;

extern char filepath[100];

int error(int cl, int cs, char* reason){
	FILE* filein;
	char l[MAXLLENGTH];
	int i;

	if(cl != errtab[nerr-1].l || cs != errtab[nerr-1].s){
		errtab[nerr].l = cl;
		errtab[nerr].s = cs;
		nerr ++;

		filein = fopen(filepath, "r");

	    for(i = 0; i < cl; i ++)
		    fgets(l, MAXLLENGTH, filein);
	    fprintf(stderr, "ERROR!¡¡LINE: %d SYMBOL: %d REASON: %s\n", cl, cs, reason);
	    fprintf(stderr, "CONTENT: %s\n", l);

		fclose(filein);
	}

	return 0;
}