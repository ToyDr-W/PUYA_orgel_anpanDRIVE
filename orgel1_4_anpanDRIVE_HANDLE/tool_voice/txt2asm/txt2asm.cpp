//#define SHDBG		//ﾃﾞﾊﾞｸﾞﾛｸﾞ(log.log)を出力するときに宣言する

//【実行形態】
//Win32ｺﾝｿｰﾙｱﾌﾟﾘｹｰｼｮﾝ
//
//【機能】
//①1行1ﾃﾞｰﾀのﾃｷｽﾄﾌｧｲﾙからｱｾﾝﾌﾞﾗｿｰｽｺｰﾄﾞを生成する
//②ｱｾﾝﾌﾞﾗ疑似命令の綴りとﾃﾞｰﾀ値の定義ﾀｲﾌﾟを指定できる
//③入力ﾃﾞｰﾀの最小値と最大値を検査して､最小値を0へｼﾌﾄし､ﾀﾞｲﾅﾐｯｸﾚﾝｼﾞを変更する
//
//【制約】
//ｻﾝﾌﾟﾙ数の最大値は #define MAX_DATA で定める
//
//【ﾊﾟﾗﾒｰﾀ】
//P1:入力ﾌｧｲﾙ(txt形式)のﾊﾟｽ名
//P2:出力ﾌｧｲﾙ(txt形式)のﾊﾟｽ名
//P3:処理ﾃﾞｰﾀ数
//P4:ｱｾﾝﾌﾞﾗｿｰｽﾌｧｲﾙのﾃﾞｰﾀ定義疑似命令の綴り(ex.dcb)
//P5:ﾃﾞｰﾀ定義ﾀｲﾌﾟ
//　B8=1ﾊﾞｲﾄに8ﾋﾞｯﾄで定義
//　W16=1ﾜｰﾄﾞに下位8ﾋﾞｯﾄ､上位8ﾋﾞｯﾄの順に定義
//　W14=1ﾜｰﾄﾞに下位7ﾋﾞｯﾄ､上位7ﾋﾞｯﾄの順に定義
//P6:ﾀﾞｲﾅﾐｯｸﾚﾝｼﾞ変換値[%]
//　0=変換しない
//　1～100=指定%に変換する
//P7:行末に付加する文字列(省略可)
//
//【改変履歴】
//2015.11.02	新規作成


#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <conio.h>
#include <dos.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <share.h>
#include <stdlib.h>


//======================================
//大域ﾃﾞｰﾀ
//======================================
//ﾊﾟﾗﾒｰﾀ
static char *i_fname;	//入力ﾌｧｲﾙ(wav形式)ﾊﾟｽ名
static char *o_fname;	//出力ﾌｧｲﾙ(ｱｾﾝﾌﾞﾗｿｰｽ形式)ﾊﾟｽ名
static int data_n;		//生成するﾃﾞｰﾀ数
static char *inst_s;	//ｱｾﾝﾌﾞﾗのﾃﾞｰﾀ定義疑似命令の綴り(ex.dcb)
static enum trgt_t{		//ﾃﾞｰﾀ定義ﾀｲﾌﾟ
	B8,					//　LPCﾀｲﾌﾟ
	W16,				//　PIC18Fﾀｲﾌﾟ
	W14}				//　PIC16Fﾀｲﾌﾟ
	trgt_t;
static int dyn_n;		//ﾀﾞｲﾅﾐｯｸﾚﾝｼﾞ変換値[%](0=変換しない､1～100=指定%に変換する)
static int min,max;		//ｻﾝﾌﾟﾙの最小値､最大値
static char *matu;		//行末に付加する文字列

//ﾃﾞｰﾀﾊﾞｯﾌｧ
#define MAX_DATA 100000
static unsigned short int data[MAX_DATA+1];


////////////////////////////////////////
//ここからﾃﾞﾊﾞｸﾞ用の記述
//======================================
//ﾃﾞﾊﾞｸﾞﾛｸﾞを出力するﾏｸﾛ
//======================================
#define LOGFNAME "log.log"
#ifdef SHDBG
	#define Hm(x) {SHSHDBGm(__FILE__,__LINE__,(char *)(x));}
	#define Hd(x) {SHSHDBGd(__FILE__,__LINE__,#x,(int)(x));}
	#define Hld(x) {SHSHDBGld(__FILE__,__LINE__,#x,(long)(x));}
	#define Hu(x) {SHSHDBGu(__FILE__,__LINE__,#x,(unsigned int)(x));}
	#define Hlu(x) {SHSHDBGlu(__FILE__,__LINE__,#x,(unsigned long)(x));}
	#define Hx(x) {SHSHDBGx(__FILE__,__LINE__,#x,(unsigned int)(x));}
	#define Hlx(x) {SHSHDBGlx(__FILE__,__LINE__,#x,(unsigned long)(x));}
	#define Hs(x) {SHSHDBGs(__FILE__,__LINE__,#x,(char *)(x));}
	#define Hdump(x,l) {SHSHDBGdump(__FILE__,__LINE__,#x,(unsigned char *)(x),l);}
#else
	#define Hm(x)
	#define Hd(x)
	#define Hld(x)
	#define Hu(x)
	#define Hlu(x)
	#define Hx(x)
	#define Hlx(x)
	#define Hs(x)
	#define Hdump(x,l)
#endif

//======================================
//ﾛｸﾞﾌｧｲﾙへ出力する
//======================================
static void SHSHDBGlog(
	char *log)							//ﾛｸﾞ内容
{
	int fd;

//	if((fd=open(LOGFNAME,O_RDWR|O_APPEND|O_CREAT,07666))==-1)
	if((_sopen_s(&fd,LOGFNAME,O_RDWR|O_APPEND|O_CREAT,_SH_DENYNO,_S_IREAD|_S_IWRITE))==-1)
										//ﾛｸﾞﾌｧｲﾙをｵｰﾌﾟﾝする
	{
		return;
	}
	_write(fd,log,strlen(log));			//ﾛｸﾞを書出す
	_close(fd);
}

//======================================
//ﾛｸﾞ編集(ﾒｯｾｰｼﾞ表示のみ)
//======================================
static void SHSHDBGm(
	char *file,
	int line,
	char *msg)
{
	char s[128];

	sprintf_s(s,sizeof(s),"%.32s %d:%.64s\n",file,line,msg);
	SHSHDBGlog(s);
}

//======================================
//ﾛｸﾞ編集関数(%d表示)
//======================================
static void SHSHDBGd(
	char *file,
	int line,
	char *name,
	int value)
{
	char s[128];

	sprintf_s(s,sizeof(s),"%.32s %d:%s=%d\n",file,line,name,value);
	SHSHDBGlog(s);
}

//======================================
//ﾛｸﾞ編集関数(%u表示)
//======================================
static void SHSHDBGu(
	char *file,
	int line,
	char *name,
	unsigned int value)
{
	char s[128];

	sprintf_s(s,sizeof(s),"%.32s %d:%s=%u\n",file,line,name,value);
	SHSHDBGlog(s);
}

//======================================
//ﾛｸﾞ編集関数(%x表示)
//======================================
static void SHSHDBGx(
	char *file,
	int line,
	char *name,
	unsigned int value)
{
	char s[128];

	sprintf_s(s,sizeof(s),"%.32s %d:%s=0x%x\n",file,line,name,value);
	SHSHDBGlog(s);
}

//======================================
//ﾛｸﾞ編集関数(%s表示)
//======================================
static void SHSHDBGs(
	char *file,
	int line,
	char *name,
	char *value)
{
	char s[192];

	sprintf_s(s,sizeof(s),"%.32s %d:%s=%.128s\n",file,line,name,value);
	SHSHDBGlog(s);
}

//======================================
//ﾛｸﾞ編集関数(16進ﾀﾞﾝﾌﾟ表示)
//======================================
static void SHSHDBGdump(
	char *file,
	int line,
	char *name,
	unsigned char *data,
	int len)
{
	char s[128],*p;
	int n,m,l;

	sprintf_s(s,sizeof(s),"%.32s %d:%s(%x)%d\n",file,line,name,data,len);
	SHSHDBGlog(s);
	for(n=0;n<len;n+=16)
	{
		sprintf_s(s,sizeof(s),"[%04x]",n);
		p=s+6;
		if((m=len-n)>16) m=16;
		for(l=0;l<m;l++)
		{
			if(l==8) *p++='-';
			else if(l) *p++=' ';
			sprintf_s(p,3,"%02x",*data++);
			p+=2;
		}
		*p++='\n';
		*p='\0';
		SHSHDBGlog(s);
	}
}


////////////////////////////////////////
//ここからｱﾌﾟﾘｹｰｼｮﾝの記述
//======================================
//元ﾌｧｲﾙのﾛｰﾄﾞ処理
//======================================
static int wav_load(void)
{
	FILE *i_fp;
	char buf[128];
	int	c,n;

	/*DBG*/Hm("wav_load入る");
	if((i_fp=fopen(i_fname,"r"))==NULL)
	{
		printf("入力10進txt形式)ﾌｧｲﾙのfopen()ｴﾗｰ fname:%s errno:%d\n",i_fname,errno);
		return(1);
	}
	min=65535;
	max=0;
	for(n=0;n<MAX_DATA;n++)
	{
		/*DBG*/Hd(n);
		if(fgets(buf,sizeof(buf)-1,i_fp)==NULL)
		{
			if(ferror(i_fp))
			{
				printf("入力(10進txt形式)ﾌｧｲﾙのfgets()ｴﾗｰ fname:%s errno:%d\n",i_fname,errno);
				return(9);
			}
			if(feof(i_fp)) break;
		}
		/*DBG*/Hs(buf);
		c=atoi(buf);
		/*DBG*/Hd(c);
		data[n]=c;
		if(min>c) min=c;
		if(max<c) max=c;
	}
	data_n=n;
	/*DBG*/Hd(data_n);
	data[data_n]=data[0];
	if(fclose(i_fp))
	{
		printf("入力(10進txt形式)ﾌｧｲﾙのfclose()ｴﾗｰ fname:%s errno:%d\n",i_fname,errno);
		return(9);
	}
	/*DBG*/Hdump(data,data_n*2);
	/*DBG*/Hm("wav_load出る");
	return(0);
}


//======================================
//ﾀﾞｲﾅﾐｯｸﾚﾝｼﾞの変換処理
//======================================
static void dyn_ext(void)
{
	int n;
	unsigned int a,b;

	/*DBG*/Hm("dyn_ext入る");
	/*DBG*/Hd(min);
	/*DBG*/Hd(max);
	b=max-min;
	/*DBG*/Hd(b);

	//変換処理
	for(n=0;n<data_n;n++)
	{
		/*DBG*/Hd(data[n]);
		a=(((unsigned int)data[n]-min)<<16)/b*dyn_n/100;
										//ﾀﾞｲﾅﾐｯｸﾚﾝｼﾞを指定値に変換する
		/*DBG*/Hd(a);
		if(a>65535) a=65535;			//ｵｰﾊﾞﾌﾛｰしたら最大値に矯正する
		data[n]=a;
	}
	/*DBG*/Hdump(data,data_n*2);
	/*DBG*/Hm("dyn_ext出る");
}


//======================================
//ｱｾﾝﾌﾞﾗﾌｧｲﾙの生成処理
//======================================
static int tbl_out(void)
{
	FILE *o_fp;
	int n;

	/*DBG*/Hm("tbl_out入る");
	if((o_fp=fopen(o_fname,"w"))==NULL)
	{
		printf("出力(ｱｾﾝﾌﾞﾗｿｰｽ)ﾌｧｲﾙのfopen()ｴﾗｰ fname:%s errno:%d\n",o_fname,errno);
		return(1);
	}
	switch(trgt_t)
	{
	case B8:
		for(n=0;n<data_n;n++)
		{
			if(fprintf(o_fp,"\t%s\t%d%s\n",inst_s,data[n]>>8,matu)<0)
			{
				printf("出力(ｱｾﾝﾌﾞﾗｿｰｽ)ﾌｧｲﾙのfprintf()ｴﾗｰ fname:%s errno:%d\n",o_fname,errno);
				return(9);
			}
		}
		break;
	case W16:
		for(n=0;n<data_n-1;n+=2)
		{
			if(fprintf(o_fp,"\t%s\t(%d<<8)+%dd%s\n",inst_s,data[n+1]>>8,data[n]>>8,matu)<0)
			{
				printf("出力(ｱｾﾝﾌﾞﾗｿｰｽ)ﾌｧｲﾙのfprintf()ｴﾗｰ fname:%s errno:%d\n",o_fname,errno);
				return(9);
			}
		}
		break;
	case W14:
		for(n=0;n<data_n-1;n+=2)
		{
			if(fprintf(o_fp,"\t%s\t(%d<<7)+%d%s\n",inst_s,data[n+1]>>9,data[n]>>9,matu)<0)
			{
				printf("出力(ｱｾﾝﾌﾞﾗｿｰｽ)ﾌｧｲﾙのfprintf()ｴﾗｰ fname:%s errno:%d\n",o_fname,errno);
				return(9);
			}
		}
		break;
	}
	if(fclose(o_fp))
	{
		printf("出力(ｱｾﾝﾌﾞﾗｿｰｽ)ﾌｧｲﾙのfclose()ｴﾗｰ fname:%s errno:%d\n",o_fname,errno);
		return(9);
	}
	/*DBG*/Hm("tbl_out出る");
	return(0);
}


//======================================
//ﾒｲﾝ処理
//======================================
int main(int argc,char *argv[])
{

	//ﾊﾟﾗﾒｰﾀ取り込み
	/*DBG*/Hd(argc);
	if(argc<=6)
	{
		printf("【ﾊﾟﾗﾒｰﾀ】\n"
			"P1:入力ﾌｧｲﾙ(txt形式)のﾊﾟｽ名\n"
			"P2:出力ﾌｧｲﾙ(asm形式)のﾊﾟｽ名\n"
			"P3:処理ﾃﾞｰﾀ数\n"
			"P4:ｱｾﾝﾌﾞﾗｿｰｽﾌｧｲﾙのﾃﾞｰﾀ定義疑似命令の綴り(ex.dcb)\n"
			"P5:ﾃﾞｰﾀ定義ﾀｲﾌﾟ\n"
			"　B8=1ﾊﾞｲﾄに8ﾋﾞｯﾄで定義\n"
			"　W16=1ﾜｰﾄﾞに下位8ﾋﾞｯﾄ､上位8ﾋﾞｯﾄの順に定義\n"
			"　W14=1ﾜｰﾄﾞに下位7ﾋﾞｯﾄ､上位7ﾋﾞｯﾄの順に定義\n"
			"P6:ﾀﾞｲﾅﾐｯｸﾚﾝｼﾞ変換値[%]\n"
			"　0=変換しない\n"
			"　1～100=指定%に変換する\n");
		return(9);
	}

	//P1:入力ﾌｧｲﾙ(wav形式)のﾊﾟｽ名
	i_fname=argv[1];
	/*DBG*/Hs(i_fname);

	//P2:出力ﾌｧｲﾙ(ｱｾﾝﾌﾞﾗｿｰｽ形式)のﾊﾟｽ名
	o_fname=argv[2];
	/*DBG*/Hs(o_fname);

	//P3:処理ﾃﾞｰﾀ数
	data_n=atoi(argv[3]);
	/*DBG*/Hd(data_n);
	if(data_n<1 || MAX_DATA<data_n)
	{
		printf("P3:wavﾌｧｲﾙのﾃﾞｰﾀ数は 1～%ld の値を指定すること\n",MAX_DATA);
		return(9);
	}

	//P4:ｱｾﾝﾌﾞﾗｿｰｽﾌｧｲﾙのﾃﾞｰﾀ定義疑似命令の綴り
	inst_s=argv[4];
	/*DBG*/Hs(inst_s);

	//P5:ﾃﾞｰﾀ定義ﾀｲﾌﾟ
	if(strcmp(argv[5],"B8")==0) trgt_t=B8;
	else if(strcmp(argv[5],"W16")==0) trgt_t=W16;
	else if(strcmp(argv[5],"W14")==0) trgt_t=W14;
	else
	{
		printf("P6:ﾃﾞｰﾀ定義ﾀｲﾌﾟは B8/W16/W14 の何れかを指定すること\n");
		return(9);
	}
	/*DBG*/Hd(trgt_t);

	//P6:ﾀﾞｲﾅﾐｯｸﾚﾝｼﾞ変換値
	dyn_n=atoi(argv[6]);
	/*DBG*/Hd(dyn_n);
	if(dyn_n<0)
	{
		printf("P7:ﾀﾞｲﾅﾐｯｸﾚﾝｼﾞ変換値は 0以上 で指定すること\n");
		return(9);
	}

	//P7:行末に付加する文字列
	if(argc>7) matu=argv[7];
	else matu="";

	//入力ﾌｧｲﾙ(txt形式)のﾛｰﾄﾞ
	if(wav_load()) return(9);

	//ﾀﾞｲﾅﾐｯｸﾚﾝｼﾞの伸長
	if(dyn_n) dyn_ext();

	//出力ﾌｧｲﾙ(txt形式)の生成
	if(tbl_out()) return(9);

	printf("%dﾃﾞｰﾀ処理しました\n",data_n);
	return(0);
}

