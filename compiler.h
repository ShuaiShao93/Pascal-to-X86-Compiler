#include<stdio.h>
#include<stdlib.h>
#include<string.h>
 //词法分析
#define MAXLLENGTH 500
#define ALFALENGTH 40
#define NORW 20
#define SSYMMAX 12
#define NMAX 14
#define STRMAX 200
#define LMAX 5
#define CIFAMAX 3000 //词法分析结果条目
#define TXMAX 500 //符号表项
#define SYSMAX 1000 //四元式
#define LBMAX 80 //label
#define BTXMAX 20 //分程序表
#define ATXMAX 20 //数组信息向量表
#define HBMAX 2000 //汇编条数
#define STRNUM 100 //字符串表条数

struct symbol{
	char name[ALFALENGTH];
	char content[STRMAX];
};

//符号表
struct table{
	char name[ALFALENGTH];
	int link;
	int obj;
	int typ;
	int ref;
	int lev;
	int nrm;
	int adr;
};

#define OBJ_CONST 1
#define OBJ_VAR 2
#define OBJ_PROCE 3
#define OBJ_FUNC 4

#define TYP_NOTYP 0
#define TYP_INT 1
#define TYP_CHAR 2
#define TYP_ARRAY 3

#define NRM_TRUE 1
#define NRM_FALSE 2

struct btable{
	int last;
	int lastpar;
	int parnum;
	int display[LMAX];
};

struct atable{
	int eltyp;
	int high;
	int elsize;
	int size;
};

#define ELTYP_INT 1
#define ELTYP_CHAR 2

struct curcondition{
	int link;
	int lev;
	int undefiden; //未完全赋值的符号表项个数
	int curblkidx; //当前处理的变量所属的函数或过程名在btab中的index
};

//四元式
struct siyuans{
	char ins[ALFALENGTH];
	char rs[STRMAX];
	char rt[ALFALENGTH];
	char rd[ALFALENGTH];
};

#define FUHAO_Z 0
#define FUHAO_F 1

#define CHENGCHU_CHENG 0
#define CHENGCHU_CHU 1

#define TOORDOWNTO_TO 0
#define TOORDOWNTO_DOWNTO 1

struct lab{
	int numsys;
	int labeltyp;
	char name[STRMAX];
	int tabidx;
};

#define LABELTYP_NRM 0
#define LABELTYP_SPE 1

//生成汇编
struct huibian{
	char ins[ALFALENGTH];
	char dst[STRMAX];
	char src[ALFALENGTH];
};

//错误处理
#define ERRMAX 100

struct err{
	int l;
	int s;
};

int cifafenxi();

int yufafenxi();

int shengchenghuibian();

int error(int cl, int cs, char* reason);