#include"compiler.h"

char word[NORW][ALFALENGTH] = 
{"const", "integer", "char",  "var",   "array", "of",    "if", 
 "then",  "else",    "do",   "while",  "for",   "to",    "downto", 
 "procedure", "function",    "read",   "write", "begin", "end"};

char wsym[NORW][10] =
{"CONSTTK", "INTTK", "CHARTK", "VARTK", "ARRAYTK", "OFTK", "IFTK",
 "THENTK", "ELSETK", "DOTK", "WHILETK", "FORTK", "TOTK", "DOWNTOTK",
 "PROCETK", "FUNCTK", "READTK", "WRITETK", "BEGINTK", "ENDTK"};

char spe[SSYMMAX]=
{'+',//0
 '-',//1
 '*',//2
 '/',//3
 '=',//4
 ';',//5
 ',',//6
 '.',//7
 '(',//8
 ')',//9
 '[',//10
 ']',//11
};

char ssym[SSYMMAX][10]=
{"PLUS", "MINU", "MULT", "DIV", "EQL", "SEMICN", 
 "COMMA", "PERIOD", "LPARENT", "RPARENT", "LBRACK", "RBRACK"};

int cc;
char filepath[100];

int getsym(char line[], struct symbol sym[], int sn);

int cifafenxi(){
	FILE *filein, *fileout;
	struct symbol sym[CIFAMAX];
	char line[MAXLLENGTH];
	int sn = 0, cs, cl;

	printf("Please input the path of file: \n");
	scanf("%s", filepath);

	while((filein = fopen(filepath, "r")) == NULL){
		printf("Failed to open the in-file!\n");
		printf("Please input the path of file: \n");
	    scanf("%s", filepath);
	}

	if((fileout = fopen("11061077_token.txt", "w")) == NULL)
		printf("Failed to open the out-file!\n");

	cl = 0;

	while(fgets(line, MAXLLENGTH, filein) != NULL){
		cc = 0;
		cs = 0;
		while(getsym(line, sym, sn) >=0){
			fprintf(fileout, "%d %d %s %s\n", cl+1, cs+1, sym[sn].name, sym[sn].content);
			sn ++;
			cs ++;
		}
		cl ++;
	}
	fclose(filein);
	fclose(fileout);

	return 0;
}

char* stosym(char ch){
	int i;
	for(i = 0; i < SSYMMAX; i ++){
		if(ch == spe[i]){
			return ssym[i];
		}
	}
	return "NOT SUPPORTED!";  //��֧�ֵ��ַ���ʾ��֧��
}

int getsym(char line[], struct symbol sym[], int sn){
	char a[STRMAX];
	int k;
	int i;

    while(line[cc] == ' '|| line[cc] == '\t'){
      cc++;
	}
	if(line[cc] == '\n' || line[cc] == '\0')
		return -1;
    else if((line[cc] >= 'A' && line[cc] <= 'Z') || (line[cc] >= 'a' && line[cc] <= 'z')){
		for(k = 0; (line[cc] >= 'A' && line[cc] <= 'Z') || (line[cc] >= 'a' && line[cc] <= 'z') || (line[cc] >= '0' && line[cc] <= '9'); cc++, k++){
			if(k < ALFALENGTH){       //�����ʶ�����ȴ��ڹ涨ֵ����ôֻ��ǰ�涨���ַ���Ч
				a[k] = line[cc];
			}
		}
		a[k] = '\0';
        for(i = 0; i < NORW; i++){
			if(strcmp(word[i], a) == 0)
				break;
		}
		if(i < NORW){
			strcpy(sym[sn].name, wsym[i]);
			strcpy(sym[sn].content, a);
		}
		else{
			strcpy(sym[sn].name, "IDEN");
			strcpy(sym[sn].content, a);
		}
	}
	else if(line[cc] >= '0' && line[cc] <= '9'){
		for(k = 0; line[cc] >= '0' && line[cc] <= '9'; k++, cc++){
			if(k < NMAX){     //������ֳ��ȴ��ڹ涨ֵ����ôֻ��ǰ�涨���ַ���Ч
				a[k] = line[cc];
			}
		}
        a[k] = '\0';
		strcpy(sym[sn].name, "INTCON");
		strcpy(sym[sn].content, a);
	}
	else if(line[cc] == '\''){
		cc++;  //��ʱccָ�����ţ���Ҫ��������һ��
		for(k = 0; line[cc] != '\''; k++, cc++){
			if(k < STRMAX){     //������ȴ��ڹ涨ֵ����ôֻ��ǰ�涨���ַ���Ч
				a[k] = line[cc];
			}
		}
		cc++;  //��ʱccָ�����ţ���Ҫ��������һ��
        a[k] = '\0';
		strcpy(sym[sn].name, "CHARCON");
		strcpy(sym[sn].content, a);
	}
	else if(line[cc] == '\"'){
		cc++;  //��ʱccָ��˫���ţ���Ҫ��������һ��
		for(k = 0; line[cc] != '\"'; k++, cc++){
			if(k < STRMAX){     //������ȴ��ڹ涨ֵ����ôֻ��ǰ�涨���ַ���Ч
				a[k] = line[cc];
			}
		}
		cc++;  //��ʱccָ��˫���ţ���Ҫ��������һ��
        a[k] = '\0';
		strcpy(sym[sn].name, "STRCON");
		strcpy(sym[sn].content, a);
	}
	else if(line[cc] == ':'){
		cc++;
		if(line[cc] == '='){
			strcpy(sym[sn].name,"ASSIGN");
			strcpy(sym[sn].content,":=");
			cc++;
		}
		else{
		strcpy(sym[sn].name, "COLON");
		strcpy(sym[sn].content, ":");
		}
	}
	else if(line[cc] == '<'){
		cc++;
		if(line[cc] == '='){
			strcpy(sym[sn].name,"LEQ");
			strcpy(sym[sn].content,"<=");
			cc++;
		}
		else if(line[cc] == '>'){
		    strcpy(sym[sn].name, "NEQ");
		    strcpy(sym[sn].content, "<>");
		    cc++;
		}
		else{
		    strcpy(sym[sn].name, "LSS");
		    strcpy(sym[sn].content, "<");
		}
	}
	else if(line[cc] == '>'){
		cc++;
		if(line[cc] == '='){
			strcpy(sym[sn].name,"GEQ");
			strcpy(sym[sn].content,">=");
			cc++;
		}
		else{
		    strcpy(sym[sn].name, "GRE");
		    strcpy(sym[sn].content, ">");
		}
	}
	else{
		strcpy(sym[sn].name, stosym(line[cc]));   //���������ת��Ϊ��Ӧ������
		sym[sn].content[0] = line[cc];
		sym[sn].content[1] = '\0';
		cc++;
	}
    return 0;
}