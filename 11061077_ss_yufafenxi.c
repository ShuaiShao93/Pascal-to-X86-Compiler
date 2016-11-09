#include"compiler.h"

struct symbol sym;

int get_sym();
int cl, cs;
extern int nerr;
FILE* cffilein;
char p[10];
int numofpar;

//符号表
struct table tab[TXMAX];
int ntab = 0;
struct btable btab[BTXMAX];
int nbtab = 0;
struct atable atab[ATXMAX];
int natab = 0;

//四元式
struct siyuans siyuanshi[SYSMAX];
int nsys = 0;
struct lab label[LBMAX];
int nlab = 0;
int ntemp = 0;

//字符串表
char zfcb[STRMAX][STRNUM];
int nstr = 0;

void chengxu();
void fenchengxu(int tabidx, int lev, int curblkidx);
void changliangshuomingbufen(struct curcondition* curcon);
void changliangdingyi(struct curcondition* curcon);
void changliang();
void bianliangshuomingbufen(struct curcondition* curcon);
void bianliangshuoming(struct curcondition* curcon);
void leixing(struct curcondition* curcon);
void guochengshuomingbufen(struct curcondition* curcon);
void hanshushuomingbufen(struct curcondition* curcon);
int guochengshoubu(struct curcondition* curcon);
int hanshushoubu(struct curcondition* curcon);
void xingshicanshubiao(struct curcondition* curcon);
void xingshicanshuduan(struct curcondition* curcon);
void yuju(struct curcondition* curcon);
void fuzhiyuju(struct curcondition* curcon);
int biaodashi(struct curcondition* curcon);
int xiang(struct curcondition* curcon);
int yinzi(struct curcondition* curcon);
int hanshudiaoyongyuju(struct curcondition* curcon);
void shizaicanshubiao(int tabidx, struct curcondition* curcon);
int tiaojian(struct curcondition* curcon);
void tiaojianyuju(struct curcondition* curcon);
void dangxunhuanyuju(struct curcondition* curcon);
void forxunhuanyuju(struct curcondition* curcon);
void guochengdiaoyongyuju(struct curcondition* curcon);
void fuheyuju(struct curcondition* curcon);
void duyuju(struct curcondition* curcon);
void xieyuju(struct curcondition* curcon);

int tabsearch(char *name, struct curcondition* curcon);


int yufafenxi(){
	int i, j = 0;

	if((cffilein = fopen("11061077_token.txt", "r")) == NULL)
		printf("Failed to open the in-file!");

	if(get_sym() < 0)
		printf("Failed to get symbol!");

	chengxu();
	
	if(nerr == 0){
		printf("TABEL:\n");
	    for(i = 0; i < ntab; i ++){
		    printf("%d %s %d %d %d %d %d %d %d\n", i, tab[i].name, tab[i].link, tab[i].obj, tab[i].typ, tab[i].ref, tab[i].nrm, tab[i].lev, tab[i].adr);
	    }
		printf("\n");
	    printf("BTABLE:\n");
	    for(i = 0; i < nbtab; i ++){
			printf("%d %d %d\n", i, btab[i].last, btab[i].lastpar, btab[i].parnum);
	    }
		printf("\n");
	    printf("ATABLE:\n");
	    for(i = 0; i < natab; i ++){
		    printf("%d %d %d\n", i, atab[i].eltyp, atab[i].high);
	    }
		printf("\n");
	    printf("QUADRUPLE:\n");
	    for(i = 0; i < nlab; i ++){
		    while(j < label[i].numsys){
		        printf("\t%d %s %s %s %s\n", j, siyuanshi[j].ins, siyuanshi[j].rs, siyuanshi[j].rt, siyuanshi[j].rd);
			    j++;
		    }
		    printf("%s\n", label[i].name);
	    }
	    while(j < nsys){
		    printf("\t%d %s %s %s %s\n",  j, siyuanshi[j].ins, siyuanshi[j].rs, siyuanshi[j].rt, siyuanshi[j].rd);
	        j++;
    	}
	    printf("\n");
	}

	fclose(cffilein);

	return 0;
}

int get_sym(){
	char c;

	int i = 0;

	fscanf(cffilein, "%d%d%s", &cl, &cs, sym.name);
	fgetc(cffilein);
	while((c = fgetc(cffilein)) != '\n'){
		if(c == EOF){
			system("pause");
			exit(0);
		}
		sym.content[i] = c;
		i ++;
	}
	sym.content[i] = '\0';

	return 0;
}

void chengxu(){
	tab[0].link = 0;
	tab[0].obj = OBJ_PROCE;
	tab[0].lev = 0;
	strcpy(tab[0].name, "start");
	tab[0].nrm = NRM_TRUE;
	tab[0].ref = nbtab;
	tab[0].typ = TYP_NOTYP;
	tab[0].adr = 0;

	ntab ++;
	nbtab ++;

	btab[0].lastpar = 0;

	fenchengxu(0,1,0);
	if(strcmp(sym.name, "PERIOD") != 0){
			error(cl, cs, "Here should be a \'.\'");
	}
}

void fenchengxu(int tabidx, int lev, int curblkidx){  
	struct curcondition curc;
	struct curcondition* curcon = &curc;
	int i;

	if(tabidx == ntab - 1)
		curcon -> link = 0;
	else{
		tab[tabidx + 1].link = 0;
		for(i = tabidx + 2; i < ntab; i ++){
			tab[i].link = i - 1;
		}
	    curcon->link = ntab - 1;
	}
	curcon->lev = lev;
	curcon->undefiden = 0;
	curcon->curblkidx = curblkidx;

	if(strcmp(sym.name, "CONSTTK") != 0 && strcmp(sym.name, "VARTK") != 0 && strcmp(sym.name, "PROCETK") != 0 && strcmp(sym.name, "FUNCTK") != 0 && strcmp(sym.name, "BEGINTK") != 0){
		error(cl,cs, "Here should be a \"const\" or \"var\" or \"procedure\" or\"function\" or \"begin\".");
	    while(strcmp(sym.name, "CONSTTK") != 0 && strcmp(sym.name, "VARTK") != 0 && strcmp(sym.name, "PROCETK") != 0 && strcmp(sym.name, "FUNCTK") != 0 && strcmp(sym.name, "BEGINTK") != 0){
		    get_sym();
		}
	}
	if(strcmp(sym.name, "CONSTTK") == 0){
		changliangshuomingbufen(curcon);
	}
	if(strcmp(sym.name, "VARTK") != 0 && strcmp(sym.name, "PROCETK") != 0 && strcmp(sym.name, "FUNCTK") != 0 && strcmp(sym.name, "BEGINTK") != 0){
		error(cl,cs, "Here should be a \"var\" or \"procedure\" or\"function\" or \"begin\".");
	    while(strcmp(sym.name, "VARTK") != 0 && strcmp(sym.name, "PROCETK") != 0 && strcmp(sym.name, "FUNCTK") != 0 && strcmp(sym.name, "BEGINTK") != 0){
		    get_sym();
		}
	}
	if(strcmp(sym.name, "VARTK") == 0){
		bianliangshuomingbufen(curcon);
	}
	if(strcmp(sym.name, "PROCETK") != 0 && strcmp(sym.name, "FUNCTK") != 0 && strcmp(sym.name, "BEGINTK") != 0){
		error(cl,cs, "Here should be a \"procedure\" or\"function\" or \"begin\".");
	    while(strcmp(sym.name, "PROCETK") != 0 && strcmp(sym.name, "FUNCTK") != 0 && strcmp(sym.name, "BEGINTK") != 0){
		    get_sym();
		}
	}
    while(strcmp(sym.name, "PROCETK") == 0 || strcmp(sym.name, "FUNCTK") == 0){
		if(strcmp(sym.name, "PROCETK") == 0){
			guochengshuomingbufen(curcon);
		}
		else{
			hanshushuomingbufen(curcon);
		}
		if(strcmp(sym.name, "PROCETK") != 0 && strcmp(sym.name, "FUNCTK") != 0 && strcmp(sym.name, "BEGINTK") != 0){
	    	error(cl,cs, "Here should be a \"procedure\" or\"function\" or \"begin\".");
	        while(strcmp(sym.name, "PROCETK") != 0 && strcmp(sym.name, "FUNCTK") != 0 && strcmp(sym.name, "BEGINTK") != 0){
		        get_sym();
		    }
	    }
	}

	label[nlab].numsys = nsys;
	label[nlab].labeltyp = LABELTYP_SPE;
	if(strcmp(tab[tabidx].name, "start") != 0){
	    label[nlab].name[0] = '_';
	    strcpy(&(label[nlab].name[1]), tab[tabidx].name);
		strcat(label[nlab].name, " PROC");
	}
	else{
		strcpy(label[nlab].name, tab[tabidx].name);
		strcat(label[nlab].name, ":");
	}
	label[nlab].tabidx = tabidx;
	tab[tabidx].adr = nlab;
	nlab ++;

	if(strcmp(sym.name, "BEGINTK") != 0){
		error(cl,cs, "Here should be a \"begin\".");
	    while(strcmp(sym.name, "BEGINTK") != 0){
		    get_sym();
		}
	}
	fuheyuju(curcon);

	if(strcmp(tab[tabidx].name, "start") == 0){
		strcpy(siyuanshi[nsys].ins, "RETURN");
		strcpy(siyuanshi[nsys].rs, itoa(curcon->curblkidx, p,10));
	    strcpy(siyuanshi[nsys].rt, itoa(tabidx, p,10));

	    nsys ++;
	}


}

void changliangshuomingbufen(struct curcondition* curcon){
	if(strcmp(sym.name, "CONSTTK") != 0){
		error(cl, cs, "Here should be a \'const\'.");
	}
	else{
		get_sym();
		changliangdingyi(curcon);
		while(strcmp(sym.name, "COMMA") == 0){
			get_sym();
			changliangdingyi(curcon);
		}
		if(strcmp(sym.name, "SEMICN") != 0){
			error(cl, cs, "Here should be a \';\'.");
		}
		else
			get_sym();
	}
}

void changliangdingyi(struct curcondition* curcon){
	if(strcmp(sym.name, "IDEN") != 0){
		error(cl, cs, "Here should be an identifier.");
		while(strcmp(sym.name, "COMMA") != 0 && strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
			get_sym();
		}
	}
	else{
		tab[ntab].link = curcon->link;
		tab[ntab].lev = curcon->lev;
		strcpy(tab[ntab].name, sym.content);
		tab[ntab].obj = OBJ_CONST;
		tab[ntab].ref = 0;
		tab[ntab].nrm = NRM_FALSE;

		btab[curcon->curblkidx].last = ntab;

		curcon->link = ntab;
		ntab ++;

		get_sym();
	    if(strcmp(sym.name, "EQL") != 0){
		    error(cl, cs, "Here should be a \'=\'.");
			while(strcmp(sym.name, "COMMA") != 0 && strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
			    get_sym();
		    }
		}
	    else{
		    get_sym();
		    changliang();
	    }
    }
}

void changliang(){
	if(strcmp(sym.name, "CHARCON") == 0){
		tab[ntab-1].typ = TYP_CHAR;
		tab[ntab-1].adr = sym.content[0];

		get_sym();
	}
	else{ 
	    if(strcmp(sym.name, "PLUS") == 0){
		    get_sym();
		    if(strcmp(sym.name, "INTCON") != 0){
			    error(cl, cs, "Here should be an integer.");
				while(strcmp(sym.name, "COMMA") != 0 && strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
			        get_sym();
		        }
		    }
			else{
				tab[ntab-1].typ = TYP_INT;
				tab[ntab-1].adr = atoi(sym.content);

				get_sym();
			}
	    }
		else if(strcmp(sym.name, "MINU") == 0){
		    get_sym();
		    if(strcmp(sym.name, "INTCON") != 0){
			    error(cl, cs, "Here should be an integer.");
				while(strcmp(sym.name, "COMMA") != 0 && strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		        	get_sym();
		        }
		    }
			else{
				tab[ntab-1].typ = TYP_INT;
				tab[ntab-1].adr = -atoi(sym.content);

				get_sym();
			}
	    }
		else{
			if(strcmp(sym.name, "INTCON") != 0){
				error(cl, cs, "Here should be an integer.");
				while(strcmp(sym.name, "COMMA") != 0 && strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		        	get_sym();
		        }
			}
			else{
				tab[ntab-1].typ = TYP_INT;
				tab[ntab-1].adr = atoi(sym.content);

				get_sym();
			}
		}
	}
}

void bianliangshuomingbufen(struct curcondition* curcon){
	if(strcmp(sym.name, "VARTK") != 0){
		error(cl, cs, "Here should be a \"var\".");
	}
	else{
		get_sym();
		bianliangshuoming(curcon);
		if(strcmp(sym.name, "SEMICN") != 0){
			error(cl, cs, "Here should be a \';\'.");
		}
		else{
			get_sym();
			while(strcmp(sym.name, "IDEN") == 0){
				bianliangshuoming(curcon);
				if(strcmp(sym.name, "SEMICN") != 0){
			        error(cl, cs, "Here should be a \';\'.");
		        }
				else{
					get_sym();
				}
			}
		}
	}
}

void bianliangshuoming(struct curcondition* curcon){
	int link;

	if(strcmp(sym.name, "IDEN") != 0){
		error(cl, cs, "Here should be an identifier.");
		while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
			get_sym();
		}
	}
	else{
		tab[ntab].link = curcon->link;
		tab[ntab].lev = curcon->lev;
		strcpy(tab[ntab].name, sym.content);
		tab[ntab].obj = OBJ_VAR;
		tab[ntab].ref = 0;
		tab[ntab].nrm = NRM_TRUE;
		link = tab[ntab].link;
		if(tab[link].nrm == NRM_FALSE || (tab[link].obj != OBJ_VAR && link != 0)){
			do{
				link = tab[link].link;
			}while(tab[link].nrm == NRM_FALSE || (tab[link].obj != OBJ_VAR && link != 0));
		}
		if(link == 0){
			tab[ntab].adr = 2 * sizeof(int);
		}
		else if(tab[link].typ == TYP_ARRAY){
			tab[ntab].adr = tab[link].adr + atab[tab[link].ref].size;
		}
		else{
			tab[ntab].adr = tab[link].adr + sizeof(int);
		}

		curcon->undefiden ++;

		btab[curcon->curblkidx].last = ntab;

		curcon->link = ntab;
		ntab ++;

		get_sym();
		if(strcmp(sym.name, "COMMA") != 0 && strcmp(sym.name, "COLON") != 0 && strcmp(sym.name, "SEMICN") != 0){
	    	error(cl,cs, "Here should be a \',\' or \':\' or \';\'.");
	        while(strcmp(sym.name, "COMMA") != 0 && strcmp(sym.name, "COLON") != 0 && strcmp(sym.name, "SEMICN") != 0){
		        get_sym();
		    }
	    }
	    while(strcmp(sym.name, "COMMA") == 0){
		    get_sym();
		    if(strcmp(sym.name, "IDEN") != 0){
		        error(cl, cs, "Here should be an identifier.");
				while(strcmp(sym.name, "COLON") != 0  && strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
			        get_sym();
		        }
			}
	        else{
				tab[ntab].link = curcon->link;
		        tab[ntab].lev = curcon->lev;
		        strcpy(tab[ntab].name, sym.content);
		        tab[ntab].obj = OBJ_VAR;
		        tab[ntab].nrm = NRM_TRUE;
				link = tab[ntab].link;
		        if(tab[link].nrm == NRM_FALSE || (tab[link].obj != OBJ_VAR && link != 0)){
			        do{
				        link = tab[link].link;
			        }while(tab[link].nrm == NRM_FALSE || (tab[link].obj != OBJ_VAR && link != 0));
		        }
		        if(tab[ntab].link == 0){
			        tab[ntab].adr = 2 * sizeof(int);
		        }
		        else if(tab[link].typ == TYP_ARRAY){
			        tab[ntab].adr = tab[link].adr + atab[tab[link].ref].size;
		        }
		        else{
			        tab[ntab].adr = tab[link].adr + sizeof(int);
		        }

		        curcon->undefiden ++;

				btab[curcon->curblkidx].last = ntab;

				curcon->link = ntab;
		        ntab ++;

		        get_sym();
	        }
		}
	    if(strcmp(sym.name, "COLON") != 0){
		    error(cl, cs, "Here should be a \':\'.");
			while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
			    get_sym();
		    }
	    }
	    else{
		    get_sym();
		    leixing(curcon);
	    }
    }
}

void leixing(struct curcondition* curcon){
    int i;

	if(strcmp(sym.name, "ARRAYTK") == 0){
		natab += curcon->undefiden;

		get_sym();
		if(strcmp(sym.name, "LBRACK") != 0){
			error(cl, cs, "Here should be a \'[\'.");
			while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		    	get_sym();
		    }
		}
		else{
			get_sym();
			if(strcmp(sym.name, "INTCON") != 0){
				error(cl, cs, "Here should be an integer.");
				while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		        	get_sym();
		        }
			}
			else{
				for(i = curcon->undefiden; i > 0; i --){
					tab[ntab - i].typ = TYP_ARRAY;
					atab[natab - i].high = atoi(sym.content) - 1;
					tab[ntab-i].ref = natab - i;
				}
				get_sym();
				if(strcmp(sym.name, "RBRACK") != 0){
					error(cl, cs, "Here should be a \']\'.");
					while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		            	get_sym();
		            }
				}
				else{
					get_sym();
					if(strcmp(sym.name, "OFTK") != 0){
						error(cl, cs, "Here should be an \"of\".");
						while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		                	get_sym();
		                }
					}
					else{
						get_sym();
						if(strcmp(sym.name, "INTTK") == 0){
							for(i = curcon->undefiden; i > 0; i --){
								atab[natab - i].eltyp = ELTYP_INT;
								atab[natab - i].elsize = sizeof(int);
								atab[natab - i].size = atab[natab - i].elsize * (atab[natab - i].high + 1);
							}
							get_sym();
						}
						else if(strcmp(sym.name, "CHARTK") == 0){
							for(i = curcon->undefiden; i > 0; i --){
								atab[natab - i].eltyp = ELTYP_CHAR;
								atab[natab - i].elsize = sizeof(int);
								atab[natab - i].size = atab[natab - i].elsize * (atab[natab - i].high + 1);
							}
							get_sym();
						}
						else{
							error(cl, cs, "Here should be an \"integer\" or a \"char\".");
							while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		    	                get_sym();
							}
						}
					}
				}
			}
		}
	}
	else if(strcmp(sym.name, "INTTK") == 0){
		for(i = curcon->undefiden; i > 0; i --){
			tab[ntab-i].typ = TYP_INT;
			tab[ntab].ref = 0;
		}
		get_sym();
	}
	else if(strcmp(sym.name, "CHARTK") == 0){
		for(i = curcon->undefiden; i > 0; i --){
			tab[ntab-i].typ = TYP_CHAR;
			tab[ntab].ref = 0;
		}
		get_sym();
	}
	else{
		error(cl, cs, "Here should be an \"integer\" or a \"char\".");
		while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
	    	get_sym();
		}
	}

	curcon->undefiden = 0;
}

void guochengshuomingbufen(struct curcondition* curcon){
	int tabidx;
	int i;

	btab[nbtab].last = 0;
	tabidx = guochengshoubu(curcon);
	btab[nbtab].lastpar = ntab - 1;
	for(i = 0; i < btab[nbtab].lastpar - tabidx; i ++){
		tab[btab[nbtab].lastpar - i].adr = (1 + tab[tabidx].lev + i + 1)*sizeof(int);
	}
	for(i = 1; i < curcon->lev; i ++){
		btab[nbtab].display[i] = btab[curcon->curblkidx].display[i];
	}
	btab[nbtab].display[curcon->lev] = curcon->curblkidx; 
	nbtab ++;
	fenchengxu(tabidx, curcon->lev + 1, nbtab-1);

	strcpy(siyuanshi[nsys].ins, "RETURN");
	strcpy(siyuanshi[nsys].rs, itoa(curcon->curblkidx, p,10));
	strcpy(siyuanshi[nsys].rt, itoa(tabidx, p,10));

	nsys ++;

	if(strcmp(sym.name, "SEMICN") != 0)
		error(cl, cs, "Here should be a \';\'.");
	else{
		get_sym();
		while(strcmp(sym.name, "PROCETK") == 0){
			btab[nbtab].last = 0;
			tabidx = guochengshoubu(curcon);
			btab[nbtab].lastpar = ntab - 1;
			for(i = 0; i < btab[nbtab].lastpar - tabidx; i ++){
	        	tab[btab[nbtab].lastpar - i].adr = (tab[tabidx].lev + 1 - 1 + i + 1)*sizeof(int);
	        }
			for(i = 1; i < curcon->lev; i ++){
		        btab[nbtab].display[i] = btab[curcon->curblkidx].display[i];
	        }
	        btab[nbtab].display[curcon->lev] = curcon->curblkidx; 
			nbtab ++;
			fenchengxu(tabidx, curcon->lev + 1, nbtab-1);

			strcpy(siyuanshi[nsys].ins, "RETURN");
			strcpy(siyuanshi[nsys].rs, itoa(curcon->curblkidx, p,10));
	        strcpy(siyuanshi[nsys].rt, itoa(tabidx, p,10));

	        nsys ++;

			if(strcmp(sym.name, "SEMICN") != 0){
				error(cl, cs, "Here should be a \';\'.");
			}
			else
				get_sym();
		}
	}
}

void hanshushuomingbufen(struct curcondition* curcon){
	int tabidx;
	int i;

	btab[nbtab].last = 0;
	tabidx = hanshushoubu(curcon);
	btab[nbtab].lastpar = ntab - 1;
	for(i = 0; i < btab[nbtab].lastpar - tabidx; i ++){
		tab[btab[nbtab].lastpar - i].adr = (1 + tab[tabidx].lev + i + 1)*sizeof(int);
	}
	for(i = 1; i < curcon->lev; i ++){
		btab[nbtab].display[i] = btab[curcon->curblkidx].display[i];
	}
	btab[nbtab].display[curcon->lev] = curcon->curblkidx; 
	nbtab ++;
	fenchengxu(tabidx, curcon->lev + 1, nbtab - 1);

	strcpy(siyuanshi[nsys].ins, "RETURN");
	strcpy(siyuanshi[nsys].rs, itoa(curcon->curblkidx, p,10));
	strcpy(siyuanshi[nsys].rt, itoa(tabidx, p,10));

	nsys ++;

	if(strcmp(sym.name, "SEMICN") != 0)
		error(cl, cs, "Here should be a \';\'.");
	else{
		get_sym();
		while(strcmp(sym.name, "FUNCTK") == 0){
			btab[nbtab].last = 0;
			tabidx = hanshushoubu(curcon);
			btab[nbtab].lastpar = ntab - 1;
			for(i = 0; i < btab[nbtab].lastpar - tabidx; i ++){
	        	tab[btab[nbtab].lastpar - i].adr = (1 + tab[tabidx].lev + i + 1)*sizeof(int);
	        }
			for(i = 1; i < curcon->lev; i ++){
		        btab[nbtab].display[i] = btab[curcon->curblkidx].display[i];
	        }
	        btab[nbtab].display[curcon->lev] = curcon->curblkidx;
	        nbtab ++;
			fenchengxu(tabidx, curcon->lev + 1, nbtab - 1);

	        strcpy(siyuanshi[nsys].ins, "RETURN");
			strcpy(siyuanshi[nsys].rs, itoa(curcon->curblkidx, p,10));
	        strcpy(siyuanshi[nsys].rt, itoa(tabidx, p,10));

	        nsys ++;

			if(strcmp(sym.name, "SEMICN") != 0){
				error(cl, cs, "Here should be a \';\'.");
			}
			else
				get_sym();
		}
	}
}

int guochengshoubu(struct curcondition* curcon){  //返回值为该分程序所属过程或函数的入口对应的符号表的索引
	int tabidx;
	int link;

	if(strcmp(sym.name, "PROCETK") != 0)
		error(cl, cs, "Here should be a \"procedure\".");
	else{
		get_sym();
		if(strcmp(sym.name, "IDEN") != 0){
			error(cl, cs, "Here should be an identifier.");
			while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		    	get_sym();
		    }
		}
		else{
			tab[ntab].link = curcon->link;
		    tab[ntab].lev = curcon->lev;
		    strcpy(tab[ntab].name, sym.content);
		    tab[ntab].obj = OBJ_PROCE;
		    tab[ntab].nrm = NRM_TRUE;
			tab[ntab].typ = TYP_NOTYP;

			tab[ntab].ref = nbtab;
			btab[nbtab].parnum = 0;

			btab[curcon->curblkidx].last = ntab;

			tabidx = ntab;
			curcon->link = ntab;
		    ntab ++;

			get_sym();

		    if(strcmp(sym.name, "LPARENT") == 0){
			    xingshicanshubiao(curcon);
		    }
		    if(strcmp(sym.name, "SEMICN") != 0)
			    error(cl, cs, "Here should be a \';\'.");
		    else
			    get_sym();
		}
	}

	return tabidx;
}

int hanshushoubu(struct curcondition* curcon){ //返回值为该分程序所属过程或函数的入口对应的符号表的索引
	int tabidx;
	int link;

	if(strcmp(sym.name, "FUNCTK") != 0)
		error(cl, cs, "Here should be an \"function\".");
	else{
		get_sym();
		if(strcmp(sym.name, "IDEN") != 0){
			error(cl, cs, "Here should be an identifier.");
			while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		    	get_sym();
		    }
		}
		else{
			tab[ntab].link = curcon->link;
		    tab[ntab].lev = curcon->lev;
		    strcpy(tab[ntab].name, sym.content);
		    tab[ntab].obj = OBJ_FUNC;
		    tab[ntab].nrm = NRM_TRUE;

	        tab[ntab].ref = nbtab;
			btab[nbtab].parnum = 0;

			btab[curcon->curblkidx].last = ntab;

			tabidx = ntab;
			curcon->link = ntab;
		    ntab ++;

		    get_sym();
		    if(strcmp(sym.name, "LPARENT") == 0){
			    xingshicanshubiao(curcon);
		    }
		    if(strcmp(sym.name, "COLON") != 0){
			    error(cl, cs, "Here should be a \':\'.");
				get_sym();
				while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		    	    get_sym();
		        }
			}
		    else{
			    get_sym();
			    if(strcmp(sym.name, "INTTK") == 0){
					tab[tabidx].typ = TYP_INT;

				    get_sym();
				    if(strcmp(sym.name, "SEMICN") != 0)
			            error(cl, cs, "Here should be a \';\'.");
		            else
			            get_sym();
			    }
			    else if(strcmp(sym.name, "CHARTK") == 0){
					tab[tabidx].typ = TYP_CHAR;

				    get_sym();
				    if(strcmp(sym.name, "SEMICN") != 0)
			            error(cl, cs, "Here should be a \';\'.");
		            else
			            get_sym();
			    }
			    else{
				    error(cl, cs, "Here should be an \"integer\" or an \"char\".");
					while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		    	        get_sym();
		            }
				}
			}
		}
	}

	return tabidx;
}

void xingshicanshubiao(struct curcondition* curcon){
	if(strcmp(sym.name, "LPARENT") != 0)
		error(cl, cs, "Here should be a \'(\'.");
	else{
		get_sym();
		xingshicanshuduan(curcon);
		if(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "RPARENT") != 0 && strcmp(sym.name, "ENDTK") != 0){
	    	error(cl,cs, "Here should be a \';\' or \')\'.");
	        while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "RPARENT") != 0 && strcmp(sym.name, "ENDTK") != 0){
		        get_sym();
		    }
	    }
	    while(strcmp(sym.name, "SEMICN") == 0){
		    get_sym();
		    xingshicanshuduan(curcon);
		}
	}
	if(strcmp(sym.name, "RPARENT") != 0){
		error(cl, cs, "Here should be a \')\'.");
		while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		  	get_sym();
        }
	}
	else
		get_sym();
}

void xingshicanshuduan(struct curcondition* curcon){
	int varflag = 0;
	int i;

	if(strcmp(sym.name, "VARTK") == 0){
		get_sym();
		varflag = 1;
	}
	if(strcmp(sym.name, "IDEN") != 0){
		error(cl, cs, "Here should be an identifier.");
		while(strcmp(sym.name, "RPARENT") != 0 && strcmp(sym.name, "COLON") != 0 && strcmp(sym.name, "ENDTK") != 0){
	    	get_sym();
		}
	}
	else{
		tab[ntab].link = curcon->link;
		tab[ntab].lev = curcon->lev + 1;
		strcpy(tab[ntab].name, sym.content);
		tab[ntab].obj = OBJ_VAR;
		if(varflag == 1)
		    tab[ntab].ref = -1;
		else
			tab[ntab].ref = 0;
		tab[ntab].nrm = NRM_FALSE;

		curcon->undefiden ++;

		btab[nbtab].last = ntab;
		btab[nbtab].parnum ++;

		ntab ++;

		get_sym();

		if(strcmp(sym.name, "COMMA") != 0 && strcmp(sym.name, "RPARENT") != 0 && strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "COLON") != 0){
	    	error(cl,cs, "Here should be a \',\' or \')\' or \':\'.");
	        while(strcmp(sym.name, "COMMA") != 0 && strcmp(sym.name, "RPARENT") != 0 && strcmp(sym.name, "SEMICN") != 0  && strcmp(sym.name, "COLON") != 0){
		        get_sym();
		    }
	    }

		while(strcmp(sym.name, "COMMA") == 0){
			get_sym();
			if(strcmp(sym.name, "IDEN") != 0){
				error(cl, cs, "Here should be an identifier.");
				while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "COLON") != 0 && strcmp(sym.name, "ENDTK") != 0){
		        	get_sym();
		        }
			}
			else{
				tab[ntab].link = curcon->link;
		        tab[ntab].lev = curcon->lev + 1;
		        strcpy(tab[ntab].name, sym.content);
		        tab[ntab].obj = OBJ_VAR;
		        if(varflag == 1)
					tab[ntab].ref = -1;
				else
					tab[ntab].ref = 0;
		        tab[ntab].nrm = NRM_FALSE;

		        curcon->undefiden ++;

		        btab[nbtab].last = ntab;
				btab[nbtab].parnum ++;

		        ntab ++;

				get_sym();
			}
		}
		if(strcmp(sym.name, "COLON") != 0){
			error(cl, cs, "Here should be a \':\'.");
			while(strcmp(sym.name, "RPARENT") != 0){
		    	get_sym();
		    }
		}
		else{
			get_sym();
			if(strcmp(sym.name, "INTTK") == 0){
				for(i = curcon->undefiden; i > 0; i --){
			        tab[ntab-i].typ = TYP_INT;
		    }

			    get_sym();
			}
			else if(strcmp(sym.name, "CHARTK") == 0){
				for(i = curcon->undefiden; i > 0; i --){
			        tab[ntab-i].typ = TYP_CHAR;
		    }

				get_sym();
			}
			else{
				error(cl, cs, "Here should be an \"integer\" or an \"char\".");
				while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		    	    get_sym();
		        }
			}
		}

		curcon->undefiden = 0;
	}
}

void yuju(struct curcondition* curcon){
	if(strcmp(sym.name, "IDEN") != 0 && strcmp(sym.name, "IFTK") != 0 && strcmp(sym.name, "WHILETK") != 0 && strcmp(sym.name, "BEGINTK") != 0 && strcmp(sym.name, "READTK") != 0 && strcmp(sym.name, "WRITETK") != 0 && strcmp(sym.name, "FORTK") != 0 && strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
	    error(cl,cs, "Here should be a statement.");
	    while(strcmp(sym.name, "IDEN") != 0 && strcmp(sym.name, "IFTK") != 0 && strcmp(sym.name, "WHILETK") != 0 && strcmp(sym.name, "BEGINTK") != 0 && strcmp(sym.name, "READTK") != 0 && strcmp(sym.name, "WRITETK") != 0 && strcmp(sym.name, "FORTK") != 0 && strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
	        get_sym();
	    }
    }
	if(strcmp(sym.name, "IDEN") == 0){
		if(tab[tabsearch(sym.content, curcon)].obj == OBJ_PROCE)
			guochengdiaoyongyuju(curcon);
		else
			fuzhiyuju(curcon);
	}
	else if(strcmp(sym.name, "IFTK") == 0){
		tiaojianyuju(curcon);
	}
	else if(strcmp(sym.name, "WHILETK") == 0){
		dangxunhuanyuju(curcon);
	}
	else if(strcmp(sym.name, "BEGINTK") == 0){
		fuheyuju(curcon);
	}
	else if(strcmp(sym.name, "READTK") == 0){
		duyuju(curcon);
	}
	else if(strcmp(sym.name, "WRITETK") == 0){
		xieyuju(curcon);
	}
	else if(strcmp(sym.name, "FORTK") == 0){
		forxunhuanyuju(curcon);
	}
}

void fuzhiyuju(struct curcondition* curcon){
	char iden[ALFALENGTH];
	char tempiden[ALFALENGTH] = "%TEMP";
    char tempiden2[ALFALENGTH] = "%TEMP";
    char labeliden[ALFALENGTH] = "_LABEL";
	int i;

	if(strcmp(sym.name, "IDEN") == 0){
		strcpy(iden, sym.content);
		if(tab[tabsearch(sym.content, curcon)].obj == OBJ_VAR && tab[tabsearch(sym.content, curcon)].typ != TYP_ARRAY){
		    get_sym();
		    if(strcmp(sym.name, "ASSIGN") != 0){
			    error(cl, cs, "Here should be a \";=\".");
				while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		    	    get_sym();
		        }
			}
		    else{
			    get_sym();
			    strcpy(&(tempiden[5]), itoa(biaodashi(curcon), p, 10)) ;

		        strcpy(siyuanshi[nsys].ins, "ASSIGN");
				strcpy(siyuanshi[nsys].rs, iden);
				strcpy(siyuanshi[nsys].rt, tempiden);
				nsys ++;

		    }
		}
		else if(tab[tabsearch(sym.content, curcon)].obj == OBJ_FUNC){
			if(tab[tabsearch(iden, curcon)].ref != curcon->curblkidx){
				for(i = curcon->lev - 1; i > 0; i --){
					if(btab[curcon->curblkidx].display[i] == tab[tabsearch(iden, curcon)].ref)
						break;
					if(i == 1)
						error(cl, cs, "This function is not available here.");
				}
			}
			get_sym();
			if(strcmp(sym.name, "ASSIGN") != 0){
				error(cl, cs, "Here should be a \";=\".");
				while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		    	    get_sym();
		        }
			}
			else{
				get_sym();
				strcpy(&(tempiden[5]), itoa(biaodashi(curcon), p, 10)) ;

		        strcpy(siyuanshi[nsys].ins, "ASSIGN");
				strcpy(siyuanshi[nsys].rs, iden);
				strcpy(siyuanshi[nsys].rt, tempiden);
				nsys ++;
			}
		}
		else if(tab[tabsearch(sym.content, curcon)].obj == OBJ_VAR && tab[tabsearch(sym.content, curcon)].typ == TYP_ARRAY){
			get_sym();
			if(strcmp(sym.name, "LBRACK") != 0){
				error(cl, cs, "Here should be a \'[\'.");
				while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		        	get_sym();
		        }
			}
			else{
				get_sym();
				strcpy(&(tempiden2[5]), itoa(biaodashi(curcon), p, 10));

				if(strcmp(sym.name, "RBRACK") != 0){
					error(cl, cs, "Here should be a \']\'.");
					while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		            	get_sym();
		            }
				}
				else{
					get_sym();
					if(strcmp(sym.name, "ASSIGN") != 0){
						error(cl, cs, "Here should be a \";=\".");
						while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		    	            get_sym();
		                }
					}
					else{
						get_sym();
						strcpy(&(tempiden[5]), itoa(biaodashi(curcon), p, 10));

						strcpy(siyuanshi[nsys].ins, "MULTI");
				        strcpy(siyuanshi[nsys].rs, tempiden2);
						strcpy(siyuanshi[nsys].rt, "4");
						strcpy(siyuanshi[nsys].rd, tempiden2);
						nsys ++;

						strcpy(siyuanshi[nsys].ins, "SW");    //SW指令是指将rd的内容存到基为rs,偏移为rt的地址处
				        strcpy(siyuanshi[nsys].rs, iden);
						strcpy(siyuanshi[nsys].rt, tempiden2);
						strcpy(siyuanshi[nsys].rd, tempiden);
						nsys ++;
					}
				}
			}
		}
	}
	else{
		error(cl, cs, "This identifier is not available here.");
		while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		    get_sym();
		}
	}
}

int biaodashi(struct curcondition* curcon){  //返回temp数组中对应的下标
	int fuhao = FUHAO_Z;
	int link;
	char tempiden[ALFALENGTH] = "%TEMP";
    char tempiden2[ALFALENGTH] = "%TEMP";
    char labeliden[ALFALENGTH] = "_LABEL";

	if(strcmp(sym.name, "PLUS") == 0){
		get_sym();
		fuhao = FUHAO_Z;
	}
	else if(strcmp(sym.name, "MINU") == 0){
		get_sym();
		fuhao = FUHAO_F;
	}
	strcpy(&(tempiden[5]), itoa(xiang(curcon), p, 10));
	if(fuhao == FUHAO_F){
		strcpy(siyuanshi[nsys].ins, "MINU");
		strcpy(siyuanshi[nsys].rs, "0");
	    strcpy(siyuanshi[nsys].rt, tempiden);

	    strcpy(&(tempiden[5]), itoa(ntemp, p, 10));
		tab[ntab].link = curcon->link;
		tab[ntab].lev = curcon->lev;
		strcpy(tab[ntab].name, tempiden);
		tab[ntab].obj = OBJ_VAR;
		tab[ntab].typ = TYP_INT;
		tab[ntab].ref = 0;
		tab[ntab].nrm = NRM_TRUE;
		link = tab[ntab].link;
		if(tab[link].nrm == NRM_FALSE || (tab[link].obj != OBJ_VAR && link != 0)){
			do{
				link = tab[link].link;
			}while(tab[link].nrm == NRM_FALSE || (tab[link].obj != OBJ_VAR && link != 0));
		}
		if(link == 0){
			tab[ntab].adr = 2 * sizeof(int);
		}
		else if(tab[link].typ == TYP_ARRAY){
			tab[ntab].adr = tab[link].adr + atab[tab[link].ref].size;
		}
		else{
			tab[ntab].adr = tab[link].adr + sizeof(int);
		}

		btab[curcon->curblkidx].last = ntab;

		curcon->link = ntab;
		ntab ++;

	    ntemp ++;
	    strcpy(siyuanshi[nsys].rd, tempiden);

	    nsys ++;
	}

	while(strcmp(sym.name, "PLUS") == 0 || strcmp(sym.name, "MINU") == 0){
		if(strcmp(sym.name, "PLUS") == 0){
			fuhao = FUHAO_Z;
		}
		else{
			fuhao = FUHAO_F;
		}
		get_sym();
		strcpy(&(tempiden2[5]), itoa(xiang(curcon), p, 10));
		if(fuhao == FUHAO_Z){
		    strcpy(siyuanshi[nsys].ins, "ADD");
	    }
	    else{
		    strcpy(siyuanshi[nsys].ins, "MINU");
	    }

		strcpy(siyuanshi[nsys].rs, tempiden);
	    strcpy(siyuanshi[nsys].rt, tempiden2);

		if(tab[tabsearch(tempiden, curcon)].typ == TYP_CHAR && tab[tabsearch(tempiden2, curcon)].typ == TYP_CHAR)
		    tab[ntab].typ = TYP_CHAR;
		else
			tab[ntab].typ = TYP_INT;

	    strcpy(&(tempiden[5]), itoa(ntemp, p, 10));
		tab[ntab].link = curcon->link;
		tab[ntab].lev = curcon->lev;
		strcpy(tab[ntab].name, tempiden);
		tab[ntab].obj = OBJ_VAR;
		tab[ntab].ref = 0;
		tab[ntab].nrm = NRM_TRUE;
		link = tab[ntab].link;
		if(tab[link].nrm == NRM_FALSE || (tab[link].obj != OBJ_VAR && link != 0)){
			do{
				link = tab[link].link;
			}while(tab[link].nrm == NRM_FALSE || (tab[link].obj != OBJ_VAR && link != 0));
		}
		if(link == 0){
			tab[ntab].adr = 2 * sizeof(int);
		}
		else if(tab[link].typ == TYP_ARRAY){
			tab[ntab].adr = tab[link].adr + atab[tab[link].ref].size;
		}
		else{
			tab[ntab].adr = tab[link].adr + sizeof(int);
		}

		btab[curcon->curblkidx].last = ntab;

		curcon->link = ntab;
		ntab ++;

	    ntemp ++;
	    strcpy(siyuanshi[nsys].rd, tempiden);

	    nsys ++;
	}

	return ntemp - 1;
}

int xiang(struct curcondition* curcon){  //返回temp数组中对应的下标
	int chengchu;
	int link;
	char tempiden[ALFALENGTH] = "%TEMP";
    char tempiden2[ALFALENGTH] = "%TEMP";
    char labeliden[ALFALENGTH] = "_LABEL";

	strcpy(&(tempiden[5]), itoa(yinzi(curcon), p, 10));
	while(strcmp(sym.name, "MULT") == 0 || strcmp(sym.name, "DIV") == 0){
		if(strcmp(sym.name, "MULT") == 0){
			chengchu = CHENGCHU_CHENG;
		}
		else{
			chengchu = CHENGCHU_CHU;
		}
		get_sym();
		strcpy(&(tempiden2[5]), itoa(yinzi(curcon), p, 10));

		if(chengchu == CHENGCHU_CHENG){
		    strcpy(siyuanshi[nsys].ins, "MULT");
	    }
	    else{
		    strcpy(siyuanshi[nsys].ins, "DIV");
	    }

		strcpy(siyuanshi[nsys].rs, tempiden);
	    strcpy(siyuanshi[nsys].rt, tempiden2);
		if(tab[tabsearch(tempiden, curcon)].typ == TYP_CHAR && tab[tabsearch(tempiden2, curcon)].typ == TYP_CHAR)
		    tab[ntab].typ = TYP_CHAR;
		else
			tab[ntab].typ = TYP_INT;

	    strcpy(&(tempiden[5]), itoa(ntemp, p, 10));
		tab[ntab].link = curcon->link;
		tab[ntab].lev = curcon->lev;
		strcpy(tab[ntab].name, tempiden);
		tab[ntab].obj = OBJ_VAR;
		tab[ntab].ref = 0;
		tab[ntab].nrm = NRM_TRUE;
		link = tab[ntab].link;
		if(tab[link].nrm == NRM_FALSE || (tab[link].obj != OBJ_VAR && link != 0)){
			do{
				link = tab[link].link;
			}while(tab[link].nrm == NRM_FALSE || (tab[link].obj != OBJ_VAR && link != 0));
		}
		if(link == 0){
			tab[ntab].adr = 2 * sizeof(int);
		}
		else if(tab[link].typ == TYP_ARRAY){
			tab[ntab].adr = tab[link].adr + atab[tab[link].ref].size;
		}
		else{
			tab[ntab].adr = tab[link].adr + sizeof(int);
		}

		btab[curcon->curblkidx].last = ntab;

		curcon->link = ntab;
		ntab ++;

	    ntemp ++;
	    strcpy(siyuanshi[nsys].rd, tempiden);

	    nsys ++;
	}

	return ntemp - 1;
}

int yinzi(struct curcondition* curcon){  //返回temp数组中对应的下标
	int link;
	char iden[ALFALENGTH];
	char tempiden[ALFALENGTH] = "%TEMP";
    char tempiden2[ALFALENGTH] = "%TEMP";
    char labeliden[ALFALENGTH] = "_LABEL";

	if(strcmp(sym.name, "IDEN") == 0){
		strcpy(iden, sym.content);
		if(tabsearch(sym.content, curcon) < 0){
			get_sym();
		}
		else{
		if((tab[tabsearch(sym.content, curcon)].obj == OBJ_VAR && tab[tabsearch(sym.content, curcon)].typ != TYP_ARRAY) || tab[tabsearch(sym.content, curcon)].obj == OBJ_CONST){
			strcpy(siyuanshi[nsys].ins, "ASSIGN");
			strcpy(siyuanshi[nsys].rt, iden);

			strcpy(&(tempiden[5]), itoa(ntemp, p, 10));
			tab[ntab].link = curcon->link;
		    tab[ntab].lev = curcon->lev;
    		strcpy(tab[ntab].name, tempiden);
	    	tab[ntab].obj = OBJ_VAR;
			tab[ntab].typ = tab[tabsearch(iden, curcon)].typ;
	    	tab[ntab].ref = 0;
	    	tab[ntab].nrm = NRM_TRUE;
		    link = tab[ntab].link;
	    	if(tab[link].nrm == NRM_FALSE || (tab[link].obj != OBJ_VAR && link != 0)){
	    		do{
	    			link = tab[link].link;
	    		}while(tab[link].nrm == NRM_FALSE || (tab[link].obj != OBJ_VAR && link != 0));
	    	}
	    	if(link == 0){
	    		tab[ntab].adr = 2 * sizeof(int);
	    	}
	    	else if(tab[link].typ == TYP_ARRAY){
	    		tab[ntab].adr = tab[link].adr + atab[tab[link].ref].size;
	    	}
	    	else{
	    		tab[ntab].adr = tab[link].adr + sizeof(int);
	    	}

	    	btab[curcon->curblkidx].last = ntab;

	    	curcon->link = ntab;
	    	ntab ++;

			ntemp ++;
			strcpy(siyuanshi[nsys].rs, tempiden);

			nsys ++;

			get_sym();

			return ntemp - 1;
		}
		else if(tab[tabsearch(sym.content, curcon)].obj == OBJ_VAR && tab[tabsearch(sym.content, curcon)].typ == TYP_ARRAY){
			get_sym();
			if(strcmp(sym.name, "LBRACK") != 0){
				error(cl, cs, "Here should be a \'[\'.");
				while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "THENTK") != 0 && strcmp(sym.name, "DOTK") != 0 && strcmp(sym.name, "TOTK") != 0 && strcmp(sym.name, "DOWNTOTK") != 0 && strcmp(sym.name, "LSS") != 0 && strcmp(sym.name, "LEQ") != 0 && strcmp(sym.name, "GRE") != 0 && strcmp(sym.name, "GEQ") != 0 && strcmp(sym.name, "EQL") != 0 && strcmp(sym.name, "NEQ") != 0){
		        	get_sym();
		        }
			}
			else{
				get_sym();
			    strcpy(&(tempiden2[5]), itoa(biaodashi(curcon), p, 10));

				strcpy(siyuanshi[nsys].ins, "MULTI");
				strcpy(siyuanshi[nsys].rs, tempiden2);
				strcpy(siyuanshi[nsys].rt, "4");
				strcpy(siyuanshi[nsys].rd, tempiden2);
				nsys ++;

				strcpy(siyuanshi[nsys].ins, "LW");    //LW指令是指将基为rt,偏移为rt的地址处的内容取出存到rd中
				strcpy(siyuanshi[nsys].rs, iden);
				strcpy(siyuanshi[nsys].rt, tempiden2);

				strcpy(&(tempiden[5]), itoa(ntemp, p, 10));
				tab[ntab].link = curcon->link;
	        	tab[ntab].lev = curcon->lev;
	        	strcpy(tab[ntab].name, tempiden);
	        	tab[ntab].obj = OBJ_VAR;
				tab[ntab].typ = atab[tab[tabsearch(iden, curcon)].ref].eltyp;
	        	tab[ntab].ref = 0;
	        	tab[ntab].nrm = NRM_TRUE;
	        	link = tab[ntab].link;
	        	if(tab[link].nrm == NRM_FALSE || (tab[link].obj != OBJ_VAR && link != 0)){
	        		do{
	        			link = tab[link].link;
	        		}while(tab[link].nrm == NRM_FALSE || (tab[link].obj != OBJ_VAR && link != 0));
	        	}
	        	if(link == 0){
	        		tab[ntab].adr = 2 * sizeof(int);
	        	}
	        	else if(tab[link].typ == TYP_ARRAY){
	        		tab[ntab].adr = tab[link].adr + atab[tab[link].ref].size;
	        	}
		        else{
	        		tab[ntab].adr = tab[link].adr + sizeof(int);
	        	}

	        	btab[curcon->curblkidx].last = ntab;

	        	curcon->link = ntab;
	        	ntab ++;

			    ntemp ++;
			    strcpy(siyuanshi[nsys].rd, tempiden);

				nsys ++;

			    if(strcmp(sym.name, "RBRACK") != 0){
				    error(cl, cs, "Here should be a \']\'.");
					get_sym();
					while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "THENTK") != 0 && strcmp(sym.name, "DOTK") != 0 && strcmp(sym.name, "TOTK") != 0 && strcmp(sym.name, "DOWNTOTK") != 0 && strcmp(sym.name, "ENDTK") != 0 && strcmp(sym.name, "LSS") != 0 && strcmp(sym.name, "LEQ") != 0 && strcmp(sym.name, "GRE") != 0 && strcmp(sym.name, "GEQ") != 0 && strcmp(sym.name, "EQL") != 0 && strcmp(sym.name, "NEQ") != 0){
		            	get_sym();
		            }
				}
			    else{
				    get_sym();

					return ntemp - 1;
				}
			}
		}
		else if(tab[tabsearch(sym.content, curcon)].obj == OBJ_FUNC){
			return hanshudiaoyongyuju(curcon);
		}
		}
	}
	else if(strcmp(sym.name, "INTCON") == 0){
		strcpy(siyuanshi[nsys].ins, "ASSIGNI");

		strcpy(siyuanshi[nsys].rt, sym.content);

	    strcpy(&(tempiden[5]), itoa(ntemp, p, 10));
		tab[ntab].link = curcon->link;
		tab[ntab].lev = curcon->lev;
		strcpy(tab[ntab].name, tempiden);
		tab[ntab].obj = OBJ_VAR;
		tab[ntab].typ = TYP_INT;
		tab[ntab].ref = 0;
		tab[ntab].nrm = NRM_TRUE;
		link = tab[ntab].link;
		if(tab[link].nrm == NRM_FALSE || (tab[link].obj != OBJ_VAR && link != 0)){
			do{
				link = tab[link].link;
			}while(tab[link].nrm == NRM_FALSE || (tab[link].obj != OBJ_VAR && link != 0));
		}
		if(link == 0){
			tab[ntab].adr = 2 * sizeof(int);
		}
		else if(tab[link].typ == TYP_ARRAY){
			tab[ntab].adr = tab[link].adr + atab[tab[link].ref].size;
		}
		else{
			tab[ntab].adr = tab[link].adr + sizeof(int);
		}

		btab[curcon->curblkidx].last = ntab;

		curcon->link = ntab;
		ntab ++;

	    ntemp ++;
	    strcpy(siyuanshi[nsys].rs, tempiden);

	    nsys ++;

		get_sym();

		return ntemp - 1;
	}
	else if(strcmp(sym.name, "LPARENT") == 0){
		get_sym();
		biaodashi(curcon);
		if(strcmp(sym.name, "RPARENT") != 0){
			error(cl, cs, "Here should be a \')\'.");
			get_sym();
			while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "THENTK") != 0 && strcmp(sym.name, "DOTK") != 0 && strcmp(sym.name, "TOTK") != 0 && strcmp(sym.name, "DOWNTOTK") != 0 && strcmp(sym.name, "ENDTK") != 0 && strcmp(sym.name, "LSS") != 0 && strcmp(sym.name, "LEQ") != 0 && strcmp(sym.name, "GRE") != 0 && strcmp(sym.name, "GEQ") != 0 && strcmp(sym.name, "EQL") != 0 && strcmp(sym.name, "NEQ") != 0){
		           	get_sym();
		    }
		}
		else{
			get_sym();

			return ntemp - 1;
		}
	}
	else{
		error(cl, cs, "This symbol should not be here.");
		while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "THENTK") != 0 && strcmp(sym.name, "DOTK") != 0 && strcmp(sym.name, "TOTK") != 0 && strcmp(sym.name, "DOWNTOTK") != 0 && strcmp(sym.name, "ENDTK") != 0 && strcmp(sym.name, "LSS") != 0 && strcmp(sym.name, "LEQ") != 0 && strcmp(sym.name, "GRE") != 0 && strcmp(sym.name, "GEQ") != 0 && strcmp(sym.name, "EQL") != 0 && strcmp(sym.name, "NEQ") != 0 && strcmp(sym.name, "RPARENT") != 0){
		    get_sym();
		}
	}
}

int hanshudiaoyongyuju(struct curcondition* curcon){  //返回temp数组中对应的下标
	int link;
	char iden[ALFALENGTH];
	char tempiden[ALFALENGTH] = "%TEMP";
    char tempiden2[ALFALENGTH] = "%TEMP";
    char labeliden[ALFALENGTH] = "_";

	if(strcmp(sym.name, "IDEN") != 0 || !(tab[tabsearch(sym.content, curcon)].obj == OBJ_FUNC)){
		error(cl, cs, "Here should be an identifier of function.");
		while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		    get_sym();
        }

	}
	else{
		strcpy(iden, sym.content);
		get_sym();
		if(strcmp(sym.name, "LPARENT") == 0){
			shizaicanshubiao(tabsearch(iden, curcon), curcon);
		}

		strcpy(siyuanshi[nsys].ins, "CALL");

		strcpy(&(labeliden[1]), iden);
		strcpy(siyuanshi[nsys].rs, labeliden);

		nsys ++;

		strcpy(siyuanshi[nsys].ins, "POP");

		strcpy(&(tempiden[5]), itoa(ntemp, p, 10));
		tab[ntab].link = curcon->link;
		tab[ntab].lev = curcon->lev;
		strcpy(tab[ntab].name, tempiden);
		tab[ntab].obj = OBJ_VAR;
		if(tab[tabsearch(iden, curcon)].typ == TYP_CHAR)
			tab[ntab].typ = TYP_CHAR;
		else
			tab[ntab].typ = TYP_INT;
		tab[ntab].ref = 0;
		tab[ntab].nrm = NRM_TRUE;
		link = tab[ntab].link;
		if(tab[link].nrm == NRM_FALSE || (tab[link].obj != OBJ_VAR && link != 0)){
			do{
				link = tab[link].link;
			}while(tab[link].nrm == NRM_FALSE || (tab[link].obj != OBJ_VAR && link != 0));
		}
		if(link == 0){
			tab[ntab].adr = 2 * sizeof(int);
		}
		else if(tab[link].typ == TYP_ARRAY){
			tab[ntab].adr = tab[link].adr + atab[tab[link].ref].size;
		}
		else{
			tab[ntab].adr = tab[link].adr + sizeof(int);
		}

		btab[curcon->curblkidx].last = ntab;

		curcon->link = ntab;
		ntab ++;

	    ntemp ++;
	    strcpy(siyuanshi[nsys].rs, tempiden);

	    nsys ++;

		return ntemp - 1;
	}
}

void shizaicanshubiao(int tabidx, struct curcondition* curcon){
	int k = 1;
	char iden[ALFALENGTH];
	char tempiden[ALFALENGTH] = "%TEMP";
    char tempiden2[ALFALENGTH] = "%TEMP";
    char labeliden[ALFALENGTH] = "_LABEL";

	numofpar = 0;

	if(strcmp(sym.name, "LPARENT") != 0){
		error(cl, cs, "Here should be a \'(\'.");
		while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		    get_sym();
        }
	}
	else{
		get_sym();
		if(tab[tabidx + k].nrm == NRM_TRUE || tab[tabidx + k].ref == 0){
		    strcpy(&(tempiden[5]), itoa(biaodashi(curcon), p, 10));
			strcpy(siyuanshi[nsys].ins, "PUSH");
		    strcpy(siyuanshi[nsys].rs, tempiden);

		    nsys ++;
		}
		else{
			if(strcmp(sym.name, "IDEN") != 0){
				error(cl, cs, "Here should be an identifier.");
				while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "COMMA") != 0 && strcmp(sym.name, "ENDTK") != 0){
		    	    get_sym();
		        }
			}
			else{
		        strcpy(iden, sym.content);
		        if(tab[tabsearch(sym.content, curcon)].obj == OBJ_VAR && tab[tabsearch(sym.content, curcon)].typ != TYP_ARRAY){

					strcpy(siyuanshi[nsys].ins, "PUSHP");
		            strcpy(siyuanshi[nsys].rs, iden);

		            nsys ++;

					get_sym();
				}
				else if(tab[tabsearch(sym.content, curcon)].obj == OBJ_VAR && tab[tabsearch(sym.content, curcon)].typ == TYP_ARRAY){
					get_sym();
		            if(strcmp(sym.name, "LBRACK") != 0){
			            error(cl, cs, "Here should be a \'[\'.");
						while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "COMMA") != 0 && strcmp(sym.name, "ENDTK") != 0){
		    	            get_sym();
		                }
					}
		            else{
			            get_sym();
		                strcpy(&(tempiden2[5]), itoa(biaodashi(curcon), p, 10));

			            strcpy(siyuanshi[nsys].ins, "MULTI");
			            strcpy(siyuanshi[nsys].rs, tempiden2);
			            strcpy(siyuanshi[nsys].rt, "4");
			            strcpy(siyuanshi[nsys].rd, tempiden2);
			            nsys ++;

			            strcpy(siyuanshi[nsys].ins, "LAP");    //LAP指令是指将基为rs,偏移为rt的地址取出并且push
			            strcpy(siyuanshi[nsys].rs, iden);
			            strcpy(siyuanshi[nsys].rt, tempiden2);

		            	nsys ++;
					}

		            if(strcmp(sym.name, "RBRACK") != 0){
		        	    error(cl, cs, "Here should be a \']\'.");
						get_sym();
						while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "COMMA") != 0 && strcmp(sym.name, "ENDTK") != 0){
		    	            get_sym();
		                }
					}
		            else{
			            get_sym();
			        }
				}
				else{
					error(cl, cs, "This identifier is not available here.");
					while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "COMMA") != 0 && strcmp(sym.name, "ENDTK") != 0){
		    	        get_sym();
		            }
				}
			}
		}

		numofpar ++;

		if(strcmp(sym.name, "COMMA") != 0 && strcmp(sym.name, "RPARENT") != 0){
	    	error(cl,cs, "Here should be a \',\' or \')\'.");
	        while(strcmp(sym.name, "COMMA") != 0 && strcmp(sym.name, "RPARENT") != 0){
		        get_sym();
		    }
	    }

		while(strcmp(sym.name, "COMMA") == 0){
		    get_sym();
			numofpar ++;
		    if(tab[tabidx + k].nrm == NRM_TRUE || tab[tabidx + k].ref == 0){
	            strcpy(&(tempiden[5]), itoa(biaodashi(curcon), p, 10));
			    strcpy(siyuanshi[nsys].ins, "PUSH");
		        strcpy(siyuanshi[nsys].rs, tempiden);

		        nsys ++;
			}
		    else{
			    if(strcmp(sym.name, "IDEN") != 0){
				    error(cl, cs, "Here should be an identifier.");
					while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "RPARENT") != 0 && strcmp(sym.name, "ENDTK") != 0){
		    	        get_sym();
		            }
				}
			    else{
	                strcpy(iden, sym.content);
	                if(tab[tabsearch(sym.content, curcon)].obj == OBJ_VAR && tab[tabsearch(sym.content, curcon)].typ != TYP_ARRAY){
						
				    	strcpy(siyuanshi[nsys].ins, "PUSHP");
		                strcpy(siyuanshi[nsys].rs, iden);

		                nsys ++;

			    		get_sym();
			    	}
					else if(tab[tabsearch(sym.content, curcon)].obj == OBJ_VAR && tab[tabsearch(sym.content, curcon)].typ == TYP_ARRAY){
					    get_sym();
		                if(strcmp(sym.name, "LBRACK") != 0){
			                error(cl, cs, "Here should be a \'[\'.");
							while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "RPARENT") != 0 && strcmp(sym.name, "ENDTK") != 0){
		    	                get_sym();
		                    }
						}
		                else{
			                get_sym();
		                    strcpy(&(tempiden2[5]), itoa(biaodashi(curcon), p, 10));

			                strcpy(siyuanshi[nsys].ins, "MULTI");
			                strcpy(siyuanshi[nsys].rs, tempiden2);
			                strcpy(siyuanshi[nsys].rt, "4");
			                strcpy(siyuanshi[nsys].rd, tempiden2);
			                nsys ++;

			                strcpy(siyuanshi[nsys].ins, "LAP");    //LAP指令是指将基为rs,偏移为rt的地址取出并且push
			                strcpy(siyuanshi[nsys].rs, iden);
			                strcpy(siyuanshi[nsys].rt, tempiden2);

		                	nsys ++;
				    	}

		                if(strcmp(sym.name, "RBRACK") != 0){
		        	        error(cl, cs, "Here should be a \']\'.");
							get_sym();
							while(strcmp(sym.name, "SEMICN" ) != 0 && strcmp(sym.name, "RPARENT") != 0 && strcmp(sym.name, "ENDTK") != 0){
		    	                get_sym();
		                    }
						}
		                else{
			                get_sym();
			            }
			    	}
				    else{
					    error(cl, cs, "This identifier is not available here.");
						while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "RPARENT") != 0 && strcmp(sym.name, "ENDTK") != 0){
		    	            get_sym();
		                }
					}
			    }
		    }
		}
	}
	if(strcmp(sym.name, "RPARENT") != 0){
		error(cl, cs, "Here should be a \')\'.");
		get_sym();
		while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		     get_sym();
		}
	}
	else{
		get_sym();
		if(numofpar != btab[tab[tabidx].ref].parnum)
			error(cl, cs, "The num of parameter is incorrect!\n");
	}
}

int tiaojian(struct curcondition* curcon){   //返回值为跳转指令在四元式表中的下标
	char tempiden[ALFALENGTH] = "%TEMP";
    char tempiden2[ALFALENGTH] = "%TEMP";
    char labeliden[ALFALENGTH] = "_LABEL";

	strcpy(&(tempiden[5]), itoa(biaodashi(curcon), p, 10));
	if(strcmp(sym.name, "LSS") == 0){
		get_sym();
		strcpy(&(tempiden2[5]), itoa(biaodashi(curcon), p, 10));
		strcpy(siyuanshi[nsys].ins, "BGE");
		strcpy(siyuanshi[nsys].rs, tempiden);
		strcpy(siyuanshi[nsys].rt, tempiden2);

	    nsys ++;

		return nsys - 1;
	}
	else if(strcmp(sym.name, "LEQ") == 0){
		get_sym();
		strcpy(&(tempiden2[5]), itoa(biaodashi(curcon), p, 10));
		strcpy(siyuanshi[nsys].ins, "BGT");
		strcpy(siyuanshi[nsys].rs, tempiden);
		strcpy(siyuanshi[nsys].rt, tempiden2);

	    nsys ++;

		return nsys - 1;
	}
	else if(strcmp(sym.name, "GRE") == 0){
		get_sym();
		strcpy(&(tempiden2[5]), itoa(biaodashi(curcon), p, 10));
		strcpy(siyuanshi[nsys].ins, "BLE");
		strcpy(siyuanshi[nsys].rs, tempiden);
		strcpy(siyuanshi[nsys].rt, tempiden2);

	    nsys ++;

		return nsys - 1;
	}
	else if(strcmp(sym.name, "GEQ") == 0){
		get_sym();
		strcpy(&(tempiden2[5]), itoa(biaodashi(curcon), p, 10));
		strcpy(siyuanshi[nsys].ins, "BLT");
		strcpy(siyuanshi[nsys].rs, tempiden);
		strcpy(siyuanshi[nsys].rt, tempiden2);

	    nsys ++;

		return nsys - 1;
	}
	else if(strcmp(sym.name, "EQL") == 0){
		get_sym();
		strcpy(&(tempiden2[5]), itoa(biaodashi(curcon), p, 10));
		strcpy(siyuanshi[nsys].ins, "BNE");
		strcpy(siyuanshi[nsys].rs, tempiden);
		strcpy(siyuanshi[nsys].rt, tempiden2);

	    nsys ++;

		return nsys - 1;
	}
	else if(strcmp(sym.name, "NEQ") == 0){
		get_sym();
		strcpy(&(tempiden2[5]), itoa(biaodashi(curcon), p, 10));
		strcpy(siyuanshi[nsys].ins, "BEQ");
		strcpy(siyuanshi[nsys].rs, tempiden);
		strcpy(siyuanshi[nsys].rt, tempiden2);

	    nsys ++;

		return nsys - 1;
	}
	else
		error(cl, cs, "Here should be a relational operator.");
}

void tiaojianyuju(struct curcondition* curcon){
	int sysidx, sysidx2;
	char tempiden[ALFALENGTH] = "%TEMP";
    char tempiden2[ALFALENGTH] = "%TEMP";
    char labeliden[ALFALENGTH] = "_LABEL";

	if(strcmp(sym.name, "IFTK") != 0){
		error(cl, cs, "Here should be a \"if\".");
		while(strcmp(sym.name, "THENTK") != 0 && strcmp(sym.name, "DOTK") != 0){
		    get_sym();
		}
	}
	else{
		get_sym();
		sysidx = tiaojian(curcon);
		if(strcmp(sym.name, "THENTK") != 0){
			error(cl, cs, "Here should be a \"then\".");
			while(strcmp(sym.name, "THENTK") != 0 && strcmp(sym.name, "DOTK") != 0){
		        get_sym();
		    }
		}
		else{
			get_sym();
			yuju(curcon);

			if(strcmp(sym.name, "ELSETK") == 0){
				strcpy(siyuanshi[nsys].ins, "JUMP");
		        sysidx2 = nsys;

				nsys ++;

				label[nlab].numsys = nsys;
	            label[nlab].labeltyp = LABELTYP_NRM;
     			strcpy(&(labeliden[6]), itoa(nlab, p, 10));
				strcpy(label[nlab].name, labeliden);
	    		nlab ++;
		    	strcpy(siyuanshi[sysidx].rd, labeliden);

				get_sym();
				yuju(curcon);

				label[nlab].numsys = nsys;
	            label[nlab].labeltyp = LABELTYP_NRM;
     			strcpy(&(labeliden[6]), itoa(nlab, p, 10));
				strcpy(label[nlab].name, labeliden);
	    		nlab ++;
		    	strcpy(siyuanshi[sysidx2].rs, labeliden);
			}
			else{
				label[nlab].numsys = nsys;
	            label[nlab].labeltyp = LABELTYP_NRM;
     			strcpy(&(labeliden[6]), itoa(nlab, p, 10));
				strcpy(label[nlab].name, labeliden);
	    		nlab ++;
		    	strcpy(siyuanshi[sysidx].rd, labeliden);
			}
		}
	}
}

void dangxunhuanyuju(struct curcondition* curcon){
	int sysidx;
	int labidx;
	char tempiden[ALFALENGTH] = "%TEMP";
    char tempiden2[ALFALENGTH] = "%TEMP";
    char labeliden[ALFALENGTH] = "_LABEL";

	if(strcmp(sym.name, "WHILETK") != 0){
		error(cl, cs, "Here should be a \"while\".");
		while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		     get_sym();
		}
	}
	else{
		label[nlab].numsys = nsys;
        label[nlab].labeltyp = LABELTYP_NRM;
     	strcpy(&(labeliden[6]), itoa(nlab, p, 10));
		strcpy(label[nlab].name, labeliden);
		labidx = nlab;
		nlab ++;

		get_sym();
		sysidx = tiaojian(curcon);
		if(strcmp(sym.name, "DOTK") != 0){
			error(cl, cs, "Here should be a \"do\".");
			while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		        get_sym();
		    }
		}
		else{
			get_sym();
			yuju(curcon);

			strcpy(siyuanshi[nsys].ins, "JUMP");
			strcpy(&(labeliden[6]), itoa(labidx, p, 10));
			strcpy(siyuanshi[nsys].rs, labeliden);

			nsys ++;

			label[nlab].numsys = nsys;
	        label[nlab].labeltyp = LABELTYP_NRM;
     		strcpy(&(labeliden[6]), itoa(nlab, p, 10));
			strcpy(label[nlab].name, labeliden);
	    	nlab ++;
		    strcpy(siyuanshi[sysidx].rd, labeliden);
		}
	}
}

void forxunhuanyuju(struct curcondition* curcon){
	int toORdownto;
	int sysidx;
	int labidx;
	char iden[ALFALENGTH];
	char tempiden[ALFALENGTH] = "%TEMP";
    char tempiden2[ALFALENGTH] = "%TEMP";
    char labeliden[ALFALENGTH] = "_LABEL";

	if(strcmp(sym.name, "FORTK") != 0){
		error(cl, cs, "Here should be a \"for\".");
	}
	else{
		get_sym();
		if(strcmp(sym.name, "IDEN") != 0 || tab[tabsearch(sym.content, curcon)].obj != OBJ_VAR || tab[tabsearch(sym.content, curcon)].typ == TYP_ARRAY){
			error(cl, cs, "Here should be an identifier of variable.");
			while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		        get_sym();
		    }
		}
		else{
			strcpy(iden, sym.content);

			get_sym();
			if(strcmp(sym.name, "ASSIGN") != 0){
				error(cl, cs, "Here should be a \";=\".");
				while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		            get_sym();
		        }
			}
			else{
				get_sym();
				strcpy(&(tempiden[5]), itoa(biaodashi(curcon), p, 10)) ;

		        strcpy(siyuanshi[nsys].ins, "ASSIGN");
				strcpy(siyuanshi[nsys].rs, iden);
				strcpy(siyuanshi[nsys].rt, tempiden);
				nsys ++;

				label[nlab].numsys = nsys;
	            label[nlab].labeltyp = LABELTYP_NRM;
     			strcpy(&(labeliden[6]), itoa(nlab, p, 10));
				strcpy(label[nlab].name, labeliden);
		        labidx = nlab;
		        nlab ++;

				if(strcmp(sym.name, "TOTK") == 0){
					get_sym();
					toORdownto = TOORDOWNTO_TO;
					strcpy(&(tempiden2[5]), itoa(biaodashi(curcon), p, 10));
		            strcpy(siyuanshi[nsys].ins, "BGT");
		            strcpy(siyuanshi[nsys].rs, iden);
					strcpy(siyuanshi[nsys].rt, tempiden2);

					sysidx = nsys;
	                nsys ++;
				}
				else if(strcmp(sym.name, "DOWNTOTK") == 0){
					get_sym();
					toORdownto = TOORDOWNTO_DOWNTO;
					strcpy(&(tempiden2[5]), itoa(biaodashi(curcon), p, 10));
		            strcpy(siyuanshi[nsys].ins, "BLT");
		            strcpy(siyuanshi[nsys].rs, iden);
					strcpy(siyuanshi[nsys].rt, tempiden2);

					sysidx = nsys;
	                nsys ++;
				}
				else{
					error(cl, cs, "Here should be a \"to\" or a \"downto\".");
					while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "DOTK") != 0 && strcmp(sym.name, "ENDTK") != 0){
		                get_sym();
		            }
				}

				if(strcmp(sym.name, "DOTK") != 0){
					error(cl, cs, "Here should be a \"do\".");
					while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		                get_sym();
		            }
				}
				else{
					get_sym();
					yuju(curcon);

					if(toORdownto == TOORDOWNTO_TO){
					    strcpy(siyuanshi[nsys].ins, "ADDI");
			            strcpy(siyuanshi[nsys].rs, iden);
						strcpy(siyuanshi[nsys].rt, "1");
						strcpy(siyuanshi[nsys].rd, iden);

			            nsys ++;
					}
					else{
						strcpy(siyuanshi[nsys].ins, "MINUI");
			            strcpy(siyuanshi[nsys].rs, iden);
						strcpy(siyuanshi[nsys].rt, "1");
						strcpy(siyuanshi[nsys].rd, iden);

			            nsys ++;
					}

					strcpy(siyuanshi[nsys].ins, "JUMP");
			        strcpy(&(labeliden[6]), itoa(labidx, p, 10));
			        strcpy(siyuanshi[nsys].rs, labeliden);

			        nsys ++;

			        label[nlab].numsys = nsys;
	                label[nlab].labeltyp = LABELTYP_NRM;
     			    strcpy(&(labeliden[6]), itoa(nlab, p, 10));
				    strcpy(label[nlab].name, labeliden);
	    	        nlab ++;
		            strcpy(siyuanshi[sysidx].rd, labeliden);
				}
			}
		}
	}
}

void guochengdiaoyongyuju(struct curcondition* curcon){
	char iden[ALFALENGTH];
	char tempiden[ALFALENGTH] = "%TEMP";
    char tempiden2[ALFALENGTH] = "%TEMP";
    char labeliden[ALFALENGTH] = "_";

	if(strcmp(sym.name, "IDEN") != 0 || !(tab[tabsearch(sym.content, curcon)].obj == OBJ_PROCE)){
		error(cl, cs, "Here should be an identifier of procedure.");
		while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		    get_sym();
	    }
	}
	else{
		strcpy(iden, sym.content);
		get_sym();
		if(strcmp(sym.name, "LPARENT") == 0){
			shizaicanshubiao(tabsearch(iden, curcon), curcon);
		}

		strcpy(siyuanshi[nsys].ins, "CALL");

		strcpy(&(labeliden[1]), iden);
		strcpy(siyuanshi[nsys].rs, labeliden);

		nsys ++;
	}
}

void fuheyuju(struct curcondition* curcon){
	if(strcmp(sym.name, "BEGINTK") != 0){
		error(cl, cs, "Here should be a \"begin\".");
		while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0 && strcmp(sym.name, "BEGINTK") != 0){
		    get_sym();
        }
	}
	else{
		get_sym();
		yuju(curcon);
		if(strcmp(sym.name, "ENDTK") != 0 && strcmp(sym.name, "SEMICN") != 0){
	    	error(cl,cs, "Here should be a \';\' or \"end\".");
	        while(strcmp(sym.name, "ENDTK" ) != 0 && strcmp(sym.name, "SEMICN") != 0){
		        get_sym();
		    }
	    }
		while(strcmp(sym.name, "SEMICN") == 0){
			get_sym();
			yuju(curcon);
			if(strcmp(sym.name, "ENDTK") != 0 && strcmp(sym.name, "SEMICN") != 0){
	    	    error(cl,cs, "Here should be a \';\' or \"end\".");
	            while(strcmp(sym.name, "ENDTK" ) != 0 && strcmp(sym.name, "SEMICN") != 0){
		            get_sym();
		        }
	        }
		}
		if(strcmp(sym.name, "ENDTK") != 0){
			error(cl, cs, "Here should be a \"end\".");
			while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		        get_sym();
		    }
		}
		else{
			get_sym();
		}
	}
}

void duyuju(struct curcondition* curcon){
	char iden[ALFALENGTH];

	if(strcmp(sym.name, "READTK") != 0){
		error(cl, cs, "Here should be a \"read\".");
		while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
            get_sym();
        }
	}
	else{
		get_sym();
		if(strcmp(sym.name, "LPARENT") != 0){
			error(cl, cs, "Here should be a \')\'.");
			while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		        get_sym();
		    }
		}
		else{
			get_sym();
			if(strcmp(sym.name, "IDEN") != 0 || tab[tabsearch(sym.content, curcon)].typ == TYP_ARRAY || (tab[tabsearch(sym.content, curcon)].obj != OBJ_VAR && tab[tabsearch(sym.content, curcon)].obj != OBJ_FUNC)){
				error(cl, cs, "Here should be an identifier of variable.");
				while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		            get_sym();
		        }
			}
			else{
				strcpy(iden, sym.content);

				strcpy(siyuanshi[nsys].ins, "READ");
				strcpy(siyuanshi[nsys].rs, iden);

				nsys ++;

				get_sym();

				if(strcmp(sym.name, "COMMA") != 0 && strcmp(sym.name, "RPARENT") != 0){
	    	        error(cl,cs, "Here should be a \',\' or \')\'.");
	                while(strcmp(sym.name, "COMMA") != 0 && strcmp(sym.name, "RPARENT") != 0){
		                get_sym();
		            }
	            }

				while(strcmp(sym.name, "COMMA") == 0){
					get_sym();
					if(strcmp(sym.name, "IDEN") != 0 || tab[tabsearch(sym.content, curcon)].typ == TYP_ARRAY || (tab[tabsearch(sym.content, curcon)].obj != OBJ_VAR && tab[tabsearch(sym.content, curcon)].obj != OBJ_FUNC)){
						error(cl, cs, "Here should be an identifier of variable.");
						while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "RPARENT") != 0 && strcmp(sym.name, "ENDTK") != 0){
		                    get_sym();
		                }
					}
					else{
						strcpy(iden, sym.content);

				        strcpy(siyuanshi[nsys].ins, "READ");
				        strcpy(siyuanshi[nsys].rs, iden);

				        nsys ++;

						get_sym();
					}
				}
				if(strcmp(sym.name, "RPARENT") != 0){
					error(cl, cs, "Here should be a \')\'.");
					get_sym();
					while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		                get_sym();
		            }
				}
				else
					get_sym();
			}
		}
	}
}

void xieyuju(struct curcondition* curcon){
	char tempiden[ALFALENGTH] = "%TEMP";
    char tempiden2[ALFALENGTH] = "%TEMP";
    char labeliden[ALFALENGTH] = "_LABEL";

	if(strcmp(sym.name, "WRITETK") != 0){
		error(cl, cs, "Here should be a \"write\".");
		while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
			get_sym();
        }
	}
	else{
		get_sym();
		if(strcmp(sym.name, "LPARENT") != 0){
			error(cl, cs, "Here should be a \'(\'.");
			while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		        get_sym();
		    }
		}
		else{
			get_sym();
			if(strcmp(sym.name, "STRCON") == 0){
				strcpy(siyuanshi[nsys].ins, "WRITES");
				strcpy(zfcb[nstr], sym.content);
				strcpy(siyuanshi[nsys].rs, itoa(nstr, p, 10));
				nstr ++;

				nsys ++;

				get_sym();
				if(strcmp(sym.name, "COMMA") == 0){
					get_sym();
					strcpy(&(tempiden[5]), itoa(biaodashi(curcon), p, 10));

					strcpy(siyuanshi[nsys].ins, "WRITE");
				    strcpy(siyuanshi[nsys].rs, tempiden);

				    nsys ++;

					if(strcmp(sym.name, "RPARENT") != 0){
						error(cl, cs, "Here should be a \')\'.");
						get_sym();
						while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		                    get_sym();
		                }
					}
					else{
						get_sym();
					}
				}
				else if(strcmp(sym.name, "RPARENT") == 0){
					get_sym();
				}
				else{
					error(cl, cs, "Here should be a \',\' or a \')\'.");
					while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		                get_sym();
		            }
				}
			}
			else{
				strcpy(&(tempiden[5]), itoa(biaodashi(curcon), p, 10));

				strcpy(siyuanshi[nsys].ins, "WRITE");
				strcpy(siyuanshi[nsys].rs, tempiden);

				nsys ++;

				if(strcmp(sym.name, "RPARENT") != 0){
					error(cl, cs, "Here should be a \')\'.");
					get_sym();
					while(strcmp(sym.name, "SEMICN") != 0 && strcmp(sym.name, "ENDTK") != 0){
		                get_sym();
		            }
				}
				else
					get_sym();
			}
		}
	}
}

int tabsearch(char *name, struct curcondition* curcon){
	int i;
	int lev = tab[btab[curcon->curblkidx].lastpar -  btab[curcon->curblkidx].parnum].lev + 1;

	for(i = btab[curcon->curblkidx].last; i > 0 ; i = tab[i].link){
		if(strcmp(tab[i].name, name) == 0){
			return i;
		}
	}

	while((--lev) > 0){
		for(i = btab[btab[curcon->curblkidx].display[lev]].last; i > 0 ; i = tab[i].link){
			if(strcmp(tab[i].name, name) == 0){
			    return i;
		    }
		}
	}
	error(cl, cs, "This identifier is not available here.");
	return -1;
}
