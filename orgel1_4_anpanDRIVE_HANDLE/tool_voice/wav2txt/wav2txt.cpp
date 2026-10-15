//#define SHDBG		//ﾃﾞﾊﾞｸﾞﾛｸﾞ(log.log)を出力するときに宣言する

//【実行形態】
//Win32ｺﾝｿｰﾙｱﾌﾟﾘｹｰｼｮﾝ
//
//【機能】
//①wavﾌｧｲﾙから1ﾃﾞｰﾀ1行10進数表現のﾃｷｽﾄﾌｧｲﾙを生成する
//②wavﾌｧｲﾙの形式は様々であり､ﾌﾟﾛｸﾞﾗﾑで自動的に識別することは論理が複雑になり､
//　目標の機能を実現するためだけに実装するのは得策ではない
//　そのため､wavﾌｧｲﾙの形式はﾊﾟﾗﾒｰﾀとして与える
//③ｻﾝﾌﾟﾙ纏め数毎にｻﾝﾌﾟﾙの平均値を出力する
//
//【ﾊﾟﾗﾒｰﾀ】
//P1:入力ﾌｧｲﾙ(wav形式)のﾊﾟｽ名
//P2:出力ﾌｧｲﾙ(ﾃｷｽﾄ形式)のﾊﾟｽ名
//P3:wavﾌｧｲﾙ先頭の読み飛ばすﾊﾞｲﾄ数(10進)
//P4:wavﾌｧｲﾙ後尾の読み飛ばすﾊﾞｲﾄ数(10進)
//P5:wavﾌｧｲﾙの量子化ﾋﾞｯﾄ数(8or16)
//P6:wavﾌｧｲﾙのｻﾝﾌﾟﾙ纏め数(ch数*入力ｻﾝﾌﾟﾙﾚｰﾄ/出力ｻﾝﾌﾟﾙﾚ-ﾄ)(10進)
//P7:生成するﾃﾞｰﾀ数(10進)
//
//【改変履歴】
//2015.6.22	新規作成
//2016.11.13 後尾の読み飛ばすﾊﾞｲﾄ数の指定を可能とした


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
static int skip_n;		//wavﾌｧｲﾙ先頭の読み飛ばすﾊﾞｲﾄ数
static int leave_n;		//wavﾌｧｲﾙ後尾の読み飛ばすﾊﾞｲﾄ数
static int bit_n;		//wavﾌｧｲﾙの量子化ﾋﾞｯﾄ数(8or16)
static int set_n;		//wavﾌｧｲﾙのｻﾝﾌﾟﾙ纏め数(入力ｻﾝﾌﾟﾙﾚｰﾄ/出力ｻﾝﾌﾟﾙﾚ-ﾄ)
static int data_n;		//処理済みﾃﾞｰﾀ数


////////////////////////////////////////
//ここからﾃﾞﾊﾞｸﾞ用の記述
//======================================
//ﾃﾞﾊﾞｸﾞﾛｸﾞを出力するﾏｸﾛ
//======================================
#define LOGFNAME "log.log"
#ifdef SHDBG
	#define Hm(x) {SHDBGm(__FILE__,__LINE__,(char *)(x));}
	#define Hd(x) {SHDBGd(__FILE__,__LINE__,#x,(int)(x));}
	#define Hld(x) {SHDBGld(__FILE__,__LINE__,#x,(long)(x));}
	#define Hu(x) {SHDBGu(__FILE__,__LINE__,#x,(unsigned int)(x));}
	#define Hlu(x) {SHDBGlu(__FILE__,__LINE__,#x,(unsigned long)(x));}
	#define Hx(x) {SHDBGx(__FILE__,__LINE__,#x,(unsigned int)(x));}
	#define Hlx(x) {SHDBGlx(__FILE__,__LINE__,#x,(unsigned long)(x));}
	#define Hs(x) {SHDBGs(__FILE__,__LINE__,#x,(char *)(x));}
	#define Hdump(x,l) {SHDBGdump(__FILE__,__LINE__,#x,(unsigned char *)(x),l);}
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
static void SHDBGlog(
	char *log)							//ﾛｸﾞ内容
{
	int fd;

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
static void SHDBGm(
	char *file,
	int line,
	char *msg)
{
	char s[128];

	sprintf_s(s,sizeof(s),"%.32s %d:%.64s\n",file,line,msg);
	SHDBGlog(s);
}

//======================================
//ﾛｸﾞ編集関数(%d表示)
//======================================
static void SHDBGd(
	char *file,
	int line,
	char *name,
	int value)
{
	char s[128];

	sprintf_s(s,sizeof(s),"%.32s %d:%s=%d\n",file,line,name,value);
	SHDBGlog(s);
}

//======================================
//ﾛｸﾞ編集関数(%u表示)
//======================================
static void SHDBGu(
	char *file,
	int line,
	char *name,
	unsigned int value)
{
	char s[128];

	sprintf_s(s,sizeof(s),"%.32s %d:%s=%u\n",file,line,name,value);
	SHDBGlog(s);
}

//======================================
//ﾛｸﾞ編集関数(%x表示)
//======================================
static void SHDBGx(
	char *file,
	int line,
	char *name,
	unsigned int value)
{
	char s[128];

	sprintf_s(s,sizeof(s),"%.32s %d:%s=0x%x\n",file,line,name,value);
	SHDBGlog(s);
}

//======================================
//ﾛｸﾞ編集関数(%s表示)
//======================================
static void SHDBGs(
	char *file,
	int line,
	char *name,
	char *value)
{
	char s[192];

	sprintf_s(s,sizeof(s),"%.32s %d:%s=%.128s\n",file,line,name,value);
	SHDBGlog(s);
}

//======================================
//ﾛｸﾞ編集関数(16進ﾀﾞﾝﾌﾟ表示)
//======================================
static void SHDBGdump(
	char *file,
	int line,
	char *name,
	unsigned char *data,
	int len)
{
	char s[128],*p;
	int n,m,l;

	sprintf_s(s,sizeof(s),"%.32s %d:%s(%x)%d\n",file,line,name,data,len);
	SHDBGlog(s);
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
		SHDBGlog(s);
	}
}


////////////////////////////////////////
//ここからｱﾌﾟﾘｹｰｼｮﾝの記述
//======================================
//wavﾌｧｲﾙの変換処理
//======================================
static int wav_txt(void)
{
	FILE *i_fp,*o_fp;
	int	c,c1,c2,n,m;
	struct stat StatBuf;
	int FileSize,FileCrp;


	/*DBG*/Hm("wav_txt入る");
	if((i_fp=fopen(i_fname,"rb"))==NULL)
	{
		printf("入力(wav)ﾌｧｲﾙのfopen()ｴﾗｰ fname:%s errno:%d\n",i_fname,errno);
		return(1);
	}
	if(fstat(fileno(i_fp),&StatBuf))
	{
		printf("入力(wav)ﾌｧｲﾙのfseekfstat()ｴﾗｰ fname:%s errno:%d\n",i_fname,errno);
		return(1);
	}
    FileSize=StatBuf.st_size;
	/*DBG*/Hd(FileSize);
	if(fseek(i_fp,skip_n,SEEK_SET))
	{
		printf("入力(wav)ﾌｧｲﾙのfseek()ｴﾗｰ fname:%s errno:%d\n",i_fname,errno);
		return(1);
	}
	if((o_fp=fopen(o_fname,"w"))==NULL)
	{
		printf("出力(txt)ﾌｧｲﾙのfopen()ｴﾗｰ fname:%s errno:%d\n",o_fname,errno);
		return(1);
	}
	FileCrp=skip_n;
	/*DBG*/Hd(FileCrp);
	if((FileSize-=leave_n)<0) FileSize=0;
	/*DBG*/Hd(FileSize);
	for(n=0;n<data_n;n++)
	{
		/*DBG*/Hd(FileCrp);
		if(FileCrp>=FileSize) break;
		c=0;
		for(m=0;m<set_n;m++)
		{
			/*DBG*/Hd(m);
			if(bit_n==8)				//量子化ﾋﾞｯﾄ数が8のとき
			{
				c2=fgetc(i_fp);
				if(ferror(i_fp))
				{
					printf("入力(wav)ﾌｧｲﾙのfgetc()ｴﾗｰ fname:%s errno:%d\n",i_fname,errno);
					return(9);
				}
				if(feof(i_fp)) break;
				/*DBG*/Hx(c2);
				c+=(c2<<8);				//16ﾋﾞｯﾄの数値表現に合わせる
				/*DBG*/Hx(c);
				FileCrp++;
				/*DBG*/Hd(FileCrp);
			}
			else						//量子化ﾋﾞｯﾄ数が16のとき
			{
				c1=fgetc(i_fp);
				if(ferror(i_fp))
				{
					printf("入力(wav)ﾌｧｲﾙのfgetc()ｴﾗｰ fname:%s errno:%d\n",i_fname,errno);
					return(9);
				}
				if(feof(i_fp)) break;
				/*DBG*/Hx(c1);
				c2=fgetc(i_fp);
				if(ferror(i_fp))
				{
					printf("入力(wav)ﾌｧｲﾙのfgetc()ｴﾗｰ fname:%s errno:%d\n",i_fname,errno);
					return(9);
				}
				if(feof(i_fp)) break;
				/*DBG*/Hx(c2);
				c+=((c1+(c2<<8))+0x8000)&0x0ffff;
										//ｾﾝﾀｰ値を0x8000にする
				/*DBG*/Hx(c);
				FileCrp+=2;
				/*DBG*/Hd(FileCrp);
			}
			/*DBG*/Hd(data_n);
		}
		/*DBG*/Hd(m);
		if(m==set_n)
		{
			if(fprintf(o_fp,"%d\n",c/set_n)<0)
			{
				printf("出力(txt)ﾌｧｲﾙのfprintf()ｴﾗｰ fname:%s errno:%d\n",o_fname,errno);
				return(1);
			}
		}
		else break;
	}
	data_n=n;
	/*DBG*/Hd(data_n);
	if(fclose(o_fp))
	{
		printf("出力(txt)ﾌｧｲﾙのfclose()ｴﾗｰ fname:%s errno:%d\n",o_fname,errno);
		return(9);
	}
	if(fclose(i_fp))
	{
		printf("入力(wav)ﾌｧｲﾙのfclose()ｴﾗｰ fname:%s errno:%d\n",i_fname,errno);
		return(9);
	}
	/*DBG*/Hm("wav_txt出る");
	return(0);
}


//======================================
//ﾒｲﾝ処理
//======================================
int main(int argc,char *argv[])
{

	//ﾊﾟﾗﾒｰﾀ取り込み
	/*DBG*/Hd(argc);
	if(argc<7)
	{
		printf("【ﾊﾟﾗﾒｰﾀ】\n"
			"P1:入力ﾌｧｲﾙ(wav形式)のﾊﾟｽ名\n"
			"P2:出力ﾌｧｲﾙ(ﾃｷｽﾄ形式)のﾊﾟｽ名\n"
			"P3:wavﾌｧｲﾙ先頭の読み飛ばすﾊﾞｲﾄ数\n"
			"P4:wavﾌｧｲﾙ後尾の読み飛ばすﾊﾞｲﾄ数\n"
			"P5:wavﾌｧｲﾙの量子化ﾋﾞｯﾄ数(8or16)\n"
			"P6:wavﾌｧｲﾙのｻﾝﾌﾟﾙ纏め数(ch数*入力ｻﾝﾌﾟﾙﾚｰﾄ/出力ｻﾝﾌﾟﾙﾚ-ﾄ)(10進)\n"
			"P7:生成するﾃﾞｰﾀ数(10進)\n");
		return(9);
	}

	//P1:入力ﾌｧｲﾙ(wav形式)のﾊﾟｽ名
	i_fname=argv[1];
	/*DBG*/Hs(i_fname);

	//P2:出力ﾌｧｲﾙ(ｱｾﾝﾌﾞﾗｿｰｽ形式)のﾊﾟｽ名
	o_fname=argv[2];
	/*DBG*/Hs(o_fname);

	//P3:wavﾌｧｲﾙ先頭の読み飛ばすﾊﾞｲﾄ数
	skip_n=atoi(argv[3]);
	/*DBG*/Hd(skip_n);
	if(skip_n<0 || 64<skip_n)
	{
		printf("P3:wavﾌｧｲﾙ先頭の読み飛ばすﾊﾞｲﾄ数は 0～64 の値を指定すること\n");
		return(9);
	}

	//P4:wavﾌｧｲﾙ先頭の読み飛ばすﾊﾞｲﾄ数
	leave_n=atoi(argv[4]);
	/*DBG*/Hd(leave_n);
	if(leave_n<0 || 64<leave_n)
	{
		printf("P4:wavﾌｧｲﾙ後尾の読み飛ばすﾊﾞｲﾄ数は 0～64 の値を指定すること\n");
		return(9);
	}

	//P5:wavﾌｧｲﾙの量子化ﾋﾞｯﾄ数
	bit_n=atoi(argv[5]);
	/*DBG*/Hd(bit_n);
	if(bit_n!=8 && bit_n!=16)
	{
		printf("P5:wavﾌｧｲﾙの量子化ﾋﾞｯﾄ数は 8/16 の何れかを指定すること\n");
		return(9);
	}

	//P6:wavﾌｧｲﾙのｻﾝﾌﾟﾙ纏め数
	set_n=atoi(argv[6]);
	/*DBG*/Hd(set_n);
	if(set_n<1 || 99<set_n)
	{
		printf("P6:wavﾌｧｲﾙのｻﾝﾌﾟﾙ纏め数は 1～99 の値を指定すること\n");
		return(9);
	}

	//P7:処理ﾃﾞｰﾀ数
	data_n=atoi(argv[7]);
	/*DBG*/Hd(data_n);
	if(data_n<1)
	{
		printf("P7:wavﾌｧｲﾙのﾃﾞｰﾀ数は 1以上 の値を指定すること\n");
		return(9);
	}

	//wavﾌｧｲﾙの変換
	if(wav_txt()) return(9);


	printf("%dﾃﾞｰﾀ処理しました\n",data_n);
	return(0);
}
