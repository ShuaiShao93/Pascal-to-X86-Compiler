#include "compiler.h"

//符号表
extern struct table tab[TXMAX];
extern int ntab;
extern struct btable btab[BTXMAX];
extern int nbtab;
extern struct atable atab[ATXMAX];
extern int natab;

//四元式
extern struct siyuans siyuanshi[SYSMAX];
extern int nsys;
extern struct lab label[LBMAX];
extern int nlab;
extern int ntemp;

//字符串表
extern char zfcb[STRMAX][STRNUM];
extern int nstr;

struct huibian hb[HBMAX];
int nhb = 0;
int curblktab;

char p[10];
char retadr[STRMAX];
char strtemp[STRMAX];

int handlesys(int syscount);
char* addr(int tabidx);
int search(char* name);
int genhb(char* ins, char* dst, char* src);

int shengchenghuibian(){
	FILE* fileout;
	int syscount;
	int i;
	int labelcount = 0;

	if((fileout = fopen("11061077_asm.asm", "w")) == NULL)
		printf("Failed to open the out-file!\n");

	fprintf(fileout, ".386\n");
    fprintf(fileout, ".model flat, stdcall\n");
	fprintf(fileout, "\n");
    fprintf(fileout, "option casemap:none\n");
	fprintf(fileout, "\n");
    fprintf(fileout, "includelib E:\\masm32\\lib\\kernel32.lib\n");
    fprintf(fileout, "includelib E:\\masm32\\lib\\msvcrt.lib\n");
	fprintf(fileout, "\n");
    fprintf(fileout, "include E:\\masm32\\include\\msvcrt.inc\n");
    fprintf(fileout, "include E:\\masm32\\include\\kernel32.inc\n");
	fprintf(fileout, "\n");
    fprintf(fileout, ".DATA\n");
    fprintf(fileout, "print_int    DB \"%%d\",0\n");
    fprintf(fileout, "print_char   DB \"%%c\",0\n");
    fprintf(fileout, "print_string DB \"%%s\",0\n");
    fprintf(fileout, "scanf_int    DB \"%%d\",0\n");
    fprintf(fileout, "scanf_char   DB \"%%c\",0\n");
	strcpy(strtemp, "_str");
	for(i = 0; i < nstr; i ++){
		strcpy(&(strtemp[4]), itoa(i, p, 10));
		fprintf(fileout, "%s   DB \"%s\",0\n", strtemp, zfcb[i]);
	}
	fprintf(fileout, "\n");
    fprintf(fileout, ".CODE\n");
	fprintf(fileout, "\n");

	for(syscount = 0; syscount < nsys; syscount ++){
		while(labelcount < nlab && label[labelcount].numsys == syscount){
			strcpy(hb[nhb].ins, label[labelcount].name);
			if(label[labelcount].labeltyp == LABELTYP_NRM){
				strcat(hb[nhb].ins, ":");
			}

			nhb ++;

			if(label[labelcount].labeltyp == LABELTYP_SPE){
				curblktab = label[labelcount].tabidx;
				genhb("mov", "EBP", "ESP");
				genhb("sub", "ESP", itoa(tab[btab[tab[curblktab].ref].last].adr, p, 10));
			}

			labelcount ++;
		}

		handlesys(syscount);
	}


	for(i = 0; i < nhb; i ++){
		if(hb[i].dst[0] == '\0' && hb[i].src[0] == '\0' &&strcmp(hb[i].ins, "ret") != 0 &&strcmp(hb[i].ins, "cdq") != 0)
		    fprintf(fileout, "%s %s %s\n", hb[i].ins, hb[i].dst, hb[i].src);
		else if(hb[i].dst[0] == '\0'|| hb[i].src[0] == '\0')
			fprintf(fileout, "\t%s %s%s\n", hb[i].ins, hb[i].dst, hb[i].src);
		else
			fprintf(fileout, "\t%s %s, %s\n", hb[i].ins, hb[i].dst, hb[i].src);
	}

	printf("Compiling Ended!!! The codes have been output into \"11061077_asm.asm\"\n");

	fclose(fileout);

	return 0;
}

int handlesys(int syscount){
	int i;
	char adr[STRMAX];

	if(strcmp(siyuanshi[syscount].ins, "ASSIGN") == 0){
		genhb("mov", "EAX", addr(search(siyuanshi[syscount].rt)));
		genhb("mov", addr(search(siyuanshi[syscount].rs)), "EAX");
	}
	else if(strcmp(siyuanshi[syscount].ins, "ASSIGNI") == 0){
		genhb("mov", "EAX", siyuanshi[syscount].rt);
		genhb("mov", addr(search(siyuanshi[syscount].rs)), "EAX");
	}
	else if(strcmp(siyuanshi[syscount].ins, "SW") == 0){
		genhb("lea", "EAX", addr(search(siyuanshi[syscount].rs)));
		genhb("sub", "EAX",addr(search(siyuanshi[syscount].rt)));
		genhb("mov", "EDX", addr(search(siyuanshi[syscount].rd)));
		genhb("mov", "DWORD PTR [EAX]", "EDX");
	}
	else if(strcmp(siyuanshi[syscount].ins, "LW") == 0){
		genhb("lea", "EAX", addr(search(siyuanshi[syscount].rs)));
		genhb("sub", "EAX", addr(search(siyuanshi[syscount].rt)));
		genhb("mov", "EDX", "DWORD PTR [EAX]");
		genhb("mov", addr(search(siyuanshi[syscount].rd)), "EDX");
	}
	else if(strcmp(siyuanshi[syscount].ins, "ADD") == 0){
		genhb("mov", "EAX", addr(search(siyuanshi[syscount].rs)));
		genhb("mov", "EDX", addr(search(siyuanshi[syscount].rt)));
		genhb("add", "EAX", "EDX");
		genhb("mov", addr(search(siyuanshi[syscount].rd)), "EAX");
	}
	else if(strcmp(siyuanshi[syscount].ins, "ADDI") == 0){
		genhb("mov", "EAX", addr(search(siyuanshi[syscount].rs)));
		genhb("add", "EAX", siyuanshi[syscount].rt);
		genhb("mov", addr(search(siyuanshi[syscount].rd)), "EAX");
	}
	else if(strcmp(siyuanshi[syscount].ins, "MINU") == 0){
		genhb("mov", "EAX", addr(search(siyuanshi[syscount].rs)));
		genhb("mov", "EDX", addr(search(siyuanshi[syscount].rt)));
		genhb("sub", "EAX", "EDX");
		genhb("mov", addr(search(siyuanshi[syscount].rd)), "EAX");
	}
	else if(strcmp(siyuanshi[syscount].ins, "MINUI") == 0){
		genhb("mov", "EAX", addr(search(siyuanshi[syscount].rs)));
		genhb("sub", "EAX", siyuanshi[syscount].rt);
		genhb("mov", addr(search(siyuanshi[syscount].rd)), "EAX");
	}
	else if(strcmp(siyuanshi[syscount].ins, "MULT") == 0){
		genhb("mov", "EAX", addr(search(siyuanshi[syscount].rs)));
		genhb("mov", "ECX", addr(search(siyuanshi[syscount].rt)));
		genhb("cdq", NULL, NULL);
		genhb("imul", "ECX", NULL);
		genhb("mov", addr(search(siyuanshi[syscount].rd)), "EAX");
	}
	else if(strcmp(siyuanshi[syscount].ins, "MULTI") == 0){
		genhb("mov", "EAX", addr(search(siyuanshi[syscount].rs)));
		genhb("mov", "ECX", siyuanshi[syscount].rt);
		genhb("cdq", NULL, NULL);
		genhb("imul", "ECX", NULL);
		genhb("mov", addr(search(siyuanshi[syscount].rd)), "EAX");
	}
	else if(strcmp(siyuanshi[syscount].ins, "DIV") == 0){
		genhb("mov", "EAX", addr(search(siyuanshi[syscount].rs)));
		genhb("mov", "ECX", addr(search(siyuanshi[syscount].rt)));
		genhb("cdq", NULL, NULL);
		genhb("idiv", "ECX", NULL);
		genhb("mov", addr(search(siyuanshi[syscount].rd)), "EAX");
	}
	else if(strcmp(siyuanshi[syscount].ins, "JUMP") == 0){
		genhb("jmp", siyuanshi[syscount].rs, NULL);
	}
	else if(strcmp(siyuanshi[syscount].ins, "BGE") == 0){
		genhb("mov", "EAX", addr(search(siyuanshi[syscount].rs)));
		genhb("mov", "EDX", addr(search(siyuanshi[syscount].rt)));
		genhb("cmp", "EAX", "EDX");
		genhb("jge", siyuanshi[syscount].rd, NULL);
	}
	else if(strcmp(siyuanshi[syscount].ins, "BGT") == 0){
		genhb("mov", "EAX", addr(search(siyuanshi[syscount].rs)));
		genhb("mov", "EDX", addr(search(siyuanshi[syscount].rt)));
		genhb("cmp", "EAX", "EDX");
		genhb("jg", siyuanshi[syscount].rd, NULL);
	}
	else if(strcmp(siyuanshi[syscount].ins, "BLE") == 0){
		genhb("mov", "EAX", addr(search(siyuanshi[syscount].rs)));
		genhb("mov", "EDX", addr(search(siyuanshi[syscount].rt)));
		genhb("cmp", "EAX", "EDX");
		genhb("jle", siyuanshi[syscount].rd, NULL);
	}
	else if(strcmp(siyuanshi[syscount].ins, "BLT") == 0){
		genhb("mov", "EAX", addr(search(siyuanshi[syscount].rs)));
		genhb("mov", "EDX", addr(search(siyuanshi[syscount].rt)));
		genhb("cmp", "EAX", "EDX");
		genhb("jl", siyuanshi[syscount].rd, NULL);
	}
	else if(strcmp(siyuanshi[syscount].ins, "BNE") == 0){
		genhb("mov", "EAX", addr(search(siyuanshi[syscount].rs)));
		genhb("mov", "EDX", addr(search(siyuanshi[syscount].rt)));
		genhb("cmp", "EAX", "EDX");
		genhb("jne", siyuanshi[syscount].rd, NULL);
	}
	else if(strcmp(siyuanshi[syscount].ins, "BEQ") == 0){
		genhb("mov", "EAX", addr(search(siyuanshi[syscount].rs)));
		genhb("mov", "EDX", addr(search(siyuanshi[syscount].rt)));
		genhb("cmp", "EAX", "EDX");
		genhb("je", siyuanshi[syscount].rd, NULL);
	}
	else if(strcmp(siyuanshi[syscount].ins, "PUSH") == 0){
		genhb("mov", "EAX", addr(search(siyuanshi[syscount].rs)));
		genhb("push", "EAX", NULL);
	}
	else if(strcmp(siyuanshi[syscount].ins, "PUSHP") == 0){
		genhb("lea", "EAX", addr(search(siyuanshi[syscount].rs)));
		genhb("push", "EAX", NULL);
	}
	else if(strcmp(siyuanshi[syscount].ins, "LAP") == 0){
		genhb("lea", "EAX", addr(search(siyuanshi[syscount].rs)));
		genhb("sub", "EAX", addr(search(siyuanshi[syscount].rt)));
		genhb("push", "EAX", NULL);
	}
	else if(strcmp(siyuanshi[syscount].ins, "POP") == 0){
		genhb("pop", "EAX", NULL);
		genhb("mov", addr(search(siyuanshi[syscount].rs)), "EAX");
	}
	else if(strcmp(siyuanshi[syscount].ins, "CALL") == 0){
		if(tab[search(&(siyuanshi[syscount].rs[1]))].lev > tab[curblktab].lev){
			for(i = 1; i < tab[curblktab].lev + 1; i ++){
			    strcpy(adr, "DWORD PTR [EBP+");
			    strcat(adr, itoa((tab[curblktab].lev + 2 - i)*sizeof(int), p, 10));
			    strcat(adr, "]");
		        genhb("mov", "EAX", adr);
			    genhb("push", "EAX", NULL);
		    }
			genhb("push", NULL, "EBP");
		}
		else{
		    for(i = 1; i < tab[search(&(siyuanshi[syscount].rs[1]))].lev + 1; i ++){
			    strcpy(adr, "DWORD PTR [EBP+");
			    strcat(adr, itoa((tab[curblktab].lev + 2 - i)*sizeof(int), p, 10));
			    strcat(adr, "]");
		        genhb("mov", "EAX", adr);
			    genhb("push", "EAX", NULL);
		    }
		}
		genhb("push", NULL, "EBP");
		
		genhb("call", siyuanshi[syscount].rs, NULL);

		genhb("pop", NULL, "EBP");

		genhb("add", "ESP", itoa(sizeof(int)*(tab[search(&(siyuanshi[syscount].rs[1]))].lev + btab[tab[search(&(siyuanshi[syscount].rs[1]))].ref].parnum), p, 10));

		genhb("push", "EAX", NULL);
	}
	else if(strcmp(siyuanshi[syscount].ins, "RETURN") == 0){
		genhb("mov", "ESP", "EBP");
		genhb("sub", "ESP", itoa(sizeof(int), p, 10));  //让esp向下移一格，指向返回值
		genhb("pop", NULL, "EAX");
		curblktab = btab[atoi(siyuanshi[syscount].rs)].lastpar - btab[atoi(siyuanshi[syscount].rs)].parnum;
		genhb("ret", NULL, NULL);
		if(strcmp(siyuanshi[syscount].rt, "0") == 0){
			strcpy(adr, "END ");
			strcat(adr, tab[atoi(siyuanshi[syscount].rt)].name);
		}
		else{
		    strcpy(adr, "_");
		    strcat(adr, tab[atoi(siyuanshi[syscount].rt)].name);
		    strcat(adr, " ENDP");
		}
		genhb(adr, NULL, NULL);
	}
	else if(strcmp(siyuanshi[syscount].ins, "READ") == 0){
			genhb("lea", "EAX", addr(search(siyuanshi[syscount].rs)));
			genhb("push", "EAX", NULL);
			if(tab[search(siyuanshi[syscount].rs)].typ == TYP_CHAR)
			    genhb("push offset", "scanf_char", NULL);
			else
				genhb("push offset", "scanf_int", NULL);
			genhb("call", "crt_scanf", NULL);
			genhb("add", "ESP", itoa(2*sizeof(int), p, 10));
	}
	else if(strcmp(siyuanshi[syscount].ins, "WRITE") == 0){
			genhb("mov", "EAX", addr(search(siyuanshi[syscount].rs)));
			genhb("push", "EAX", NULL);
			if(tab[search(siyuanshi[syscount].rs)].typ == TYP_CHAR)
			    genhb("push offset", "print_char", NULL);
			else
				genhb("push offset", "print_int", NULL);
			genhb("call", "crt_printf", NULL);
			genhb("add", "ESP", itoa(2*sizeof(int), p, 10));
	}
	else if(strcmp(siyuanshi[syscount].ins, "WRITES") == 0){
		strcpy(&(strtemp[4]), siyuanshi[syscount].rs);
		genhb("push offset", strtemp, NULL);
		genhb("push offset", "print_string", NULL);
		genhb("call", "crt_printf", NULL);
		genhb("add", "ESP", itoa(2*sizeof(int), p, 10));
	}
	else{
		fprintf(stderr, "INS:%s not supported!!!!\n", siyuanshi[syscount].ins);
		system("pause");
		exit(0);
	}


	return 0;
}

char* addr(int tabidx){
	int lev = tab[tabidx].lev;
	int offset = tab[tabidx].adr;
	int curlev = tab[curblktab].lev + 1;
	char address[STRMAX];

	if(tabidx == -1){
		system("pause");
		exit(0);
	}

	if(tab[tabidx].obj == OBJ_CONST){
		strcpy(retadr, itoa(tab[tabidx].adr, p ,10));

		return retadr;
	}
	else if(tab[tabidx].obj == OBJ_FUNC){ 
		if(tabidx == curblktab){
		    strcpy(address, "[EBP-");
		    strcat(address, itoa(1*sizeof(int), p ,10));
		    strcat(address, "]");
		}
		else if(curlev - lev > 1){
			strcpy(address, "DWORD PTR [EBP+");
		    strcat(address, itoa((curlev - lev)*sizeof(int), p, 10));
		    strcat(address, "]");
		    genhb("mov", "EBX", address);
			strcpy(address, "[EBX-");
		    strcat(address, itoa(1*sizeof(int), p ,10));
		    strcat(address, "]");
		}
		else{
			fprintf(stderr, "ILLEGAL IDENTIFICATION: %s", tab[tabidx].name);
			system("pause");
			exit(0);
		}
	}
	else if(lev == curlev){
		if(tab[tabidx].nrm == NRM_FALSE && tab[tabidx].ref == -1){
		    strcpy(address, "DWORD PTR [EBP+");
		    strcat(address,  itoa(offset, p, 10));
	    	strcat(address, "]");
		    genhb("mov", "EBX", address);

		    strcpy(address, "[EBX]");
	    }
		else if(tab[tabidx].nrm == NRM_FALSE){
			strcpy(address, "[EBP+");
		    strcat(address,  itoa(offset, p, 10));
	    	strcat(address, "]");
		}
		else{
		    strcpy(address, "[EBP-");
		    strcat(address, itoa(offset, p, 10));
		    strcat(address, "]");
		}
	}
	else if(lev < curlev){
		strcpy(address, "DWORD PTR [EBP+");
		strcat(address, itoa((curlev - lev + 1)*sizeof(int), p, 10));
		strcat(address, "]");
		genhb("mov", "EBX", address);

		if(tab[tabidx].nrm == NRM_FALSE && tab[tabidx].ref == -1){
		    strcpy(address, "DWORD PTR [ECX+");
		    strcat(address,  itoa(offset, p, 10));
	    	strcat(address, "]");
		    genhb("mov", "EBX", address);

		    strcpy(address, "[EBX]");
	    }
		else if(tab[tabidx].nrm == NRM_FALSE){
			strcpy(address, "[EBX+");
		    strcat(address,  itoa(offset, p, 10));
	    	strcat(address, "]");
		}
		else{
		    strcpy(address, "[EBX-");
		    strcat(address, itoa(offset, p, 10));
		    strcat(address, "]");
		}
	}

	strcpy(retadr, "DWORD PTR ");

	strcat(retadr, address);

	return retadr;
}

int search(char* name){
	int i;
	int lev = tab[curblktab].lev + 1;

	for(i = btab[tab[curblktab].ref].last; i > 0 ; i = tab[i].link){
		if(strcmp(tab[i].name, name) == 0){
			return i;
		}
	}

	while((--lev) > 0){
		for(i = btab[btab[tab[curblktab].ref].display[lev]].last; i > 0 ; i = tab[i].link){
			if(strcmp(tab[i].name, name) == 0){
			    return i;
		    }
		}
	}

	fprintf(stderr, "ILLEGAL IDENTIFICATION: %s", name);

	return -1;
}

int genhb(char* ins, char* dst, char* src){
	strcpy(hb[nhb].ins, ins);
	if(dst == NULL)
		hb[nhb].dst[0] = '\0';
	else
		strcpy(hb[nhb].dst, dst);
	if(src == NULL)
		hb[nhb].src[0] = '\0';
	else
		strcpy(hb[nhb].src, src);
	
	nhb ++;

	return 0;
}