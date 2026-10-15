//#define SHDBG		//ﾃﾞﾊﾞｸﾞﾛｸﾞ(log.log)を出力するときに宣言する

//【実行形態】
//Win32ｺﾝｿｰﾙｱﾌﾟﾘｹｰｼｮﾝ
//
//【機能】
//①1行1ﾃﾞｰﾀのﾃｷｽﾄﾌｧｲﾙからCｿｰｽｺｰﾄﾞを生成する
//②入力ﾃﾞｰﾀの最小値と最大値を検査して､最小値を0へｼﾌﾄし､ﾀﾞｲﾅﾐｯｸﾚﾝｼﾞを変更する
//
//【制約】
//ｻﾝﾌﾟﾙ数の最大値は #define MAX_DATA で定める
//
//【ﾊﾟﾗﾒｰﾀ】
//P1:入力ﾌｧｲﾙ(txt形式)のﾊﾟｽ名
//P2:出力ﾌｧｲﾙ(txt形式)のﾊﾟｽ名
//P3:処理ﾃﾞｰﾀ数
//P4:ﾀﾞｲﾅﾐｯｸﾚﾝｼﾞ変換値[%]
//　0=変換しない
//　1～100=指定%に変換する
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
static int dyn_n;		//ﾀﾞｲﾅﾐｯｸﾚﾝｼﾞ変換値[%](0=変換しない､1～100=指定%に変換する)
static int min,max;		//ｻﾝﾌﾟﾙの最小値､最大値

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
		printf("出力(Cｿｰｽ)ﾌｧｲﾙのfopen()ｴﾗｰ fname:%s errno:%d\n",o_fname,errno);
		return(1);
	}
	for(n=0;n<data_n;n++)
	{
		if(fprintf(o_fp,"\t%d,\n",data[n]>>8)<0)
		{
			printf("出力(Cｿｰｽ)ﾌｧｲﾙのfprintf()ｴﾗｰ fname:%s errno:%d\n",o_fname,errno);
			return(9);
		}
	}
	if(fclose(o_fp))
	{
		printf("出力(Cｿｰｽ)ﾌｧｲﾙのfclose()ｴﾗｰ fname:%s errno:%d\n",o_fname,errno);
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
	if(argc<=4)
	{
		printf("【ﾊﾟﾗﾒｰﾀ】\n"
			"P1:入力ﾌｧｲﾙ(wav形式)のﾊﾟｽ名\n"
			"P2:出力ﾌｧｲﾙ(txt形式)のﾊﾟｽ名\n"
			"P3:処理ﾃﾞｰﾀ数\n"
			"P46:ﾀﾞｲﾅﾐｯｸﾚﾝｼﾞ変換値[%]\n"
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

	//P4:ﾀﾞｲﾅﾐｯｸﾚﾝｼﾞ変換値
	dyn_n=atoi(argv[4]);
	/*DBG*/Hd(dyn_n);
	if(dyn_n<0)
	{
		printf("P7:ﾀﾞｲﾅﾐｯｸﾚﾝｼﾞ変換値は 0以上 で指定すること\n");
		return(9);
	}

	//入力ﾌｧｲﾙ(txt形式)のﾛｰﾄﾞ
	if(wav_load()) return(9);

	//ﾀﾞｲﾅﾐｯｸﾚﾝｼﾞの伸長
	if(dyn_n) dyn_ext();

	//出力ﾌｧｲﾙ(txt形式)の生成
	if(tbl_out()) return(9);

	printf("%dﾃﾞｰﾀ処理しました\n",data_n);
	return(0);
}

