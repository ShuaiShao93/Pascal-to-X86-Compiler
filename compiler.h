#include<stdio.h>
#include<stdlib.h>
#include<string.h>
 //�ʷ�����
#define MAXLLENGTH 500
#define ALFALENGTH 40
#define NORW 20
#define SSYMMAX 12
#define NMAX 14
#define STRMAX 200
#define LMAX 5
#define CIFAMAX 3000 //�ʷ����������Ŀ
#define TXMAX 500 //���ű���
#define SYSMAX 1000 //��Ԫʽ
#define LBMAX 80 //label
#define BTXMAX 20 //�ֳ����
#define ATXMAX 20 //������Ϣ������
#define HBMAX 2000 //�������
#define STRNUM 100 //�ַ���������

struct symbol{
	char name[ALFALENGTH];
	char content[STRMAX];
};

//���ű�
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
	int undefiden; //δ��ȫ��ֵ�ķ��ű������
	int curblkidx; //��ǰ����ı��������ĺ������������btab�е�index
};

//��Ԫʽ
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

//���ɻ��
struct huibian{
	char ins[ALFALENGTH];
	char dst[STRMAX];
	char src[ALFALENGTH];
};

//������
#define ERRMAX 100

struct err{
	int l;
	int s;
};

int cifafenxi();

int yufafenxi();

int shengchenghuibian();

int error(int cl, int cs, char* reason);