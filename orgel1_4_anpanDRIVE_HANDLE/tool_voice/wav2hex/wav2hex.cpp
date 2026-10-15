//#define SHDBG		//ﾃﾞﾊﾞｸﾞﾛｸﾞ(log.log)を出力するときに宣言する

//【実行形態】
//Win32ｺﾝｿｰﾙｱﾌﾟﾘｹｰｼｮﾝ
//
//【機能】
//①ﾘﾆｱPCM形式WAVﾌｧｲﾙからｲﾝﾃﾙHEXﾌｧｲﾙを生成する｡
//②WAVﾌｧｲﾙのﾍｯﾀﾞ情報を見て､自動的に指定されたｻﾝﾌﾟﾘﾝｸﾞﾚｰﾄと量子化ﾋﾞｯﾄ数に変換する｡
//　ch数は1ch(ﾓﾉﾗﾙ)となる｡
//③ｻﾝﾌﾟﾘﾝｸﾞﾚｰﾄやch数の変換では複数のｻﾝﾌﾟﾙの平均値を出力する｡
//④最大のﾀﾞｲﾅﾐｯｸﾚﾝｼﾞになる(量子化ﾋﾞｯﾄ数での最小値から最大値まで振らせる)ように加工する｡
//　但し､WAVﾌｧｲﾙ名の末尾が'#'(例えばabc#.wav)のときはﾀﾞｲﾅﾐｯｸﾚﾝｼﾞの加工をしない｡
//⑤複数ﾌｧｲﾙを入力し､順に1個のｲﾝﾃﾙHEXﾌｧｲﾙに出力する｡
//⑥上記⑤の処理で､入力ﾌｧｲﾙ毎にorgelの音声ﾃﾞｰﾀ(SPIﾌﾗｯｼｭ)のｲﾝﾃﾞｯｸｽを設定するvos_macの
//　ｽｸﾘﾌﾟﾄを出力する｡
//　vos_mac 開始ｱﾄﾞﾚｽ[ﾊﾞｲﾄ],ｻﾝﾌﾟﾙ数　;WAVﾌｧｲﾙ名
//⑦64Mﾊﾞｲﾄ分までのHEXﾌｧｲﾙが生成可能｡
//⑧指定されたｻﾝﾌﾟﾘﾝｸﾞﾚｰﾄで生成する｡但し､入力ﾌｧｲﾙのｻﾝﾌﾟﾘﾝｸﾞﾚｰﾄと整数比でない場合は
//　近しい整数個のｻﾝﾌﾟﾙを取り纏めるので､誤差が生じる｡
//⑨指定された量子化ﾋﾞｯﾄでHEXﾌｧｲﾙに変換する｡
//・8ﾋﾞｯﾄでは､HEXﾌｧｲﾙの各ﾊﾞｲﾄに各ｻﾝﾌﾟﾙを格納する｡
//・12ﾋﾞｯﾄでは､HEXﾌｧｲﾙの3ﾊﾞｲﾄに2ｻﾝﾌﾟﾙを格納する｡
//　格納の順は､若番ｱﾄﾞﾚｽに先行ｻﾝﾌﾟﾙの下位8ﾋﾞｯﾄを､中間ｱﾄﾞﾚｽの上位に先行ｻﾝﾌﾟﾙの上位4ﾋﾞｯﾄ､
//　下位に後行ｻﾝｵｳﾙの下位4ﾋﾞｯﾄを､老番のｱﾄﾞﾚｽに後行ｻﾝﾌﾟﾙの上位8ﾋﾞｯﾄを格納する｡
//　(若番ｱﾄﾞﾚｽMSB)先行ｻﾝﾌﾟﾙの上位8ﾋﾞｯﾄ･先行ｻﾝﾌﾟﾙの下位4ﾋﾞｯﾄ･後行ｻﾝﾌﾟﾙの下位4ﾋﾞｯﾄ･後行ｻﾝﾌﾟﾙの上位8ﾋﾞｯﾄ(老番ｱﾄﾞﾚｽLSB)
//・16ﾋﾞｯﾄでは､HEXﾌｧｲﾙの2ﾊﾞｲﾄに1ｻﾝﾌﾟﾙを格納する｡
//　若番ｱﾄﾞﾚｽに下位8ﾋﾞｯﾄ､老番ｱﾄﾞﾚｽに上位8ﾋﾞｯﾄを格納する｡
//・量子化ﾋﾞｯﾄ数に関わらずPCM値はunsignedで､ﾚﾍﾞﾙ0は中央値となる｡
//
//【入力ﾊﾟﾗﾒｰﾀ(起動ﾊﾟﾗﾒｰﾀ)】
//P1:出力ﾌｧｲﾙ(ｲﾝﾃﾙHEX)のﾊﾟｽ名
//P2:出力ﾌｧｲﾙ(vos_macｽｸﾘﾌﾟﾄ)のﾊﾟｽ名
//P3:入力ﾌｧｲﾙ(WAVﾌｧｲﾙﾊﾟｽ名ﾘｽﾄ)のﾊﾟｽ名
//P4:変換後のｻﾝﾌﾟﾘﾝｸﾞﾚｰﾄ[sps](1000～44100)
//P5:変換後の量子化ﾋﾞｯﾄ数(7～16)
//
//【改変履歴】
//2018.9.29	新規作成


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
//ﾛｸﾞ編集関数(%ld表示)
//======================================
static void SHDBGld(
	char *file,
	int line,
	char *name,
	long value)
{
	char s[128];

	sprintf_s(s,sizeof(s),"%.32s %d:%s=%ld\n",file,line,name,value);
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
//ﾛｸﾞ編集関数(%lu表示)
//======================================
static void SHDBGlu(
	char *file,
	int line,
	char *name,
	unsigned long value)
{
	char s[128];

	sprintf_s(s,sizeof(s),"%.32s %d:%s=%lu\n",file,line,name,value);
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
//大域ﾃﾞｰﾀ
//======================================
static char *o_fname;					//出力ﾌｧｲﾙ(ｲﾝﾃﾙHEX)ﾊﾟｽ名
static char *c_fname;					//出力ﾌｧｲﾙ(vos_macｽｸﾘﾌﾟﾄ)ﾊﾟｽ名
static char *i_fname;					//入力ﾌｧｲﾙ(WAVﾌｧｲﾙﾊﾟｽﾘｽﾄ)ﾊﾟｽ名
static char w_fname[256];				//入力ﾌｧｲﾙ(WAV)ﾊﾟｽ名
static int w_fname_no;					//入力ﾌｧｲﾙ(WAV)番号
static FILE *o_fp,*c_fp,*i_fp,*w_fp;
static union
	{
		unsigned char u_char[4];
		unsigned char s_char[4];
		unsigned short u_short[2];
		unsigned short s_short[2];
		unsigned int u_int;
		unsigned int s_int;
	} buf;
static unsigned short max,min;			//ﾃﾞｰﾀの最大値と最小値
static unsigned int data_n;
static unsigned short smp_byte,matome;
static unsigned short cnv_bits;			//変換後の量子化ﾋﾞｯﾄ数
static unsigned int cnv_rate;			//変換後のｻﾝﾌﾟﾘﾝｸﾞﾚｰﾄ
static unsigned int hex_ad;
static unsigned char hex_buf[0x4000000];


//======================================
//指定ﾊﾞｲﾄ数を読込む
//======================================
void getbyte(
	unsigned char bytes)				//読込むﾊﾞｲﾄ数
{
//	unsigned char n;


	//Hs("getbyte()入る");
	//Hd(bytes);
	if(fread(&buf,bytes,1,w_fp)!=1)
	{
		if(ferror(w_fp))
		{
			printf("入力(WAV)ﾌｧｲﾙのfread()ｴﾗｰ fname:%s errno:%d\n",w_fname,errno);
			exit(1);
		}
	}
//	for(n=0;n<bytes;n++)
//	{
//		buf.u_char[n]=fgetc(w_fp);
//		if(ferror(w_fp))
//		{
//			printf("入力(WAV)ﾌｧｲﾙのfgetc()ｴﾗｰ fname:%s errno:%d\n",w_fname,errno);
//			exit(1);
//		}
//	}
	//Hdump(&buf,bytes);
	//Hs("getbyte()出る");
}


//======================================
//ｻﾝﾌﾟﾙを取出す
//======================================
void getsmp(void)
{


	if(smp_byte==1)						//8ﾋﾞｯﾄのとき
	{
		getbyte(1);						//1ﾊﾞｲﾄを取出して
		buf.u_char[1]=buf.u_char[0];	//　16ﾋﾞｯﾄに変換する
	}
	else								//16ﾋﾞｯﾄのとき
	{
		getbyte(2);						//2ﾊﾞｲﾄを取出して
		buf.s_short[0]+=0x8000;			//unsinedに変換する
	}
}


//======================================
//ﾃﾞｰﾀの纏め値の最大値と最小値を取得する
//======================================
void data_minmax(void)
{
	unsigned int n,smp;
	unsigned short m;


	Hs("data_minmax()入る");
	Hx(data_n);
	Hu(matome);
	max=0;								//最大値の初期値
	min=0xffff;							//最小値の初期値
	for(n=0;n<data_n;n++)				//全ﾃﾞｰﾀを処理する
	{
		smp=0;							//ｻﾝﾌﾟﾙ値の初期値
		for(m=0;m<matome;m++)			//纏め数だけ回す
		{
			getsmp();
			smp+=buf.u_short[0];
		}
		smp/=matome;					//平均値を計算する
		if(max<smp) max=smp;			//最大値を求める
		if(min>smp) min=smp;			//最小値を求める
	}
	Hx(min);
	Hx(max);
	Hs("data_minmax()出る");
}


//======================================
//HEXﾃﾞｰﾀを生成する
//======================================
void make_hex(
	unsigned char no_vol)
{
	unsigned int n,smp;
	unsigned short m;
	unsigned short hex0,hex1,hex2;


	Hs("make_hex()入る");
	Hx(data_n);
	Hu(matome);
	for(n=0;n<data_n;n++)				//全ﾃﾞｰﾀを処理する
	{
		smp=0;							//ｻﾝﾌﾟﾙ値の初期値
		for(m=0;m<matome;m++)			//纏め数だけ回す
		{
			getsmp();
			Hx(buf.u_short[0]);
			smp+=buf.u_short[0];
		}
		smp=(smp+(matome/2))/matome;	//平均値を計算する
		Hx(smp);
		if(no_vol)
		{
			buf.u_short[0]=(smp-min)*0x10000/(max-min+1);
										//ﾀﾞｲﾅﾐｯｸﾚﾝｼﾞを16ﾋﾞｯﾄで100%にする
		}
		else
		{
			buf.u_short[0]=smp;			//ﾀﾞｲﾅﾐｯｸﾚﾝｼﾞを変更しない
		}
		Hx(buf.u_short[0]);
		Hx(buf.u_char[1]);
		if(cnv_bits==16)				//変換後の量子化ﾋﾞｯﾄ数が16のとき
		{
			hex_buf[hex_ad++]=buf.u_char[0];
										//HEXﾊﾞｯﾌｧの若番ｱﾄﾞﾚｽに下位8ﾋﾞｯﾄを格納する
			hex_buf[hex_ad++]=buf.u_char[1];
										//HEXﾊﾞｯﾌｧの老番ｱﾄﾞﾚｽに上位8ﾋﾞｯﾄを格納する
		}
		else if(cnv_bits==8)			//変換後の量子化ﾋﾞｯﾄ数が8のとき
		{
			hex_buf[hex_ad++]=buf.u_char[1];
										//HEXﾊﾞｯﾌｧに格納する
		}
		else							//変換後の量子化ﾋﾞｯﾄ数が12のとき
		{
			if(n&1)						//後行ｻﾝﾌﾟﾙのとき
			{
				hex2=buf.u_char[1];		//最後ﾊﾞｲﾄに後行ｻﾝﾌﾟﾙの上位8ﾋﾞｯﾄを格納する
				hex1|=buf.u_char[0]>>4;
										//中間ﾊﾞｲﾄの下位に後行ｻﾝﾌﾟﾙの下位4ﾋﾞｯﾄを格納する
				hex_buf[hex_ad++]=hex0;	//HEXﾊﾞｯﾌｧに格納する
				hex_buf[hex_ad++]=hex1;
				hex_buf[hex_ad++]=hex2;
			}
			else						//先行ｻﾝﾌﾟﾙのとき
			{
				hex0=buf.u_char[1];		//先頭ﾊﾞｲﾄに先行ｻﾝﾌﾟﾙの上位8ﾋﾞｯﾄを格納する
				hex1=buf.u_char[0]&0xf0;//中間ﾊﾞｲﾄの上位に先行ｻﾝﾌﾟﾙの下位4ﾋﾞｯﾄを格納する
			}
		}
	}
	Hs("make_hex()出る");
}


//======================================
//HEXﾌｧｲﾙを書出す
//======================================
void put_hex(void)
{
	unsigned int n;
	unsigned char m,chksum;


	Hx(hex_ad);
	n=hex_ad;
	while(n%16) hex_buf[n++]=0xff;		//ﾃﾞｰﾀの最後から16ﾊﾞｲﾄ境界まで0xffを埋める
	for(n=0;n<hex_ad;)
	{
		buf.u_int=n;
		if(buf.u_short[0]==0)			//ｱﾄﾞﾚｽが65536ﾊﾞｲﾄ境界のとき
		{
			chksum=0x02+0x04+buf.u_char[3]+buf.u_char[2];
										//ﾁｪｯｸｻﾑを計算する
			if(fprintf(o_fp,":02000004%4.4X%2.2X\n",buf.u_short[1],(256-chksum)&0xff)<0)
										//ｾｸﾞﾒﾝﾄﾚｺｰﾄﾞ
			{
				printf("出力(txt)ﾌｧｲﾙのfprintf()ｴﾗｰ fname:%s errno:%d\n",o_fname,errno);
				exit(1);
			}
		}
		if(fprintf(o_fp,":10%4.4X00",buf.u_short[0])<0)
										//ﾃﾞｰﾀｱﾄﾞﾚｽ
		{
			printf("出力(txt)ﾌｧｲﾙのfprintf()ｴﾗｰ fname:%s errno:%d\n",o_fname,errno);
			exit(1);
		}
		chksum=0x10+buf.u_char[1]+buf.u_char[0];
										//ﾁｪｯｸｻﾑの初期値
		for(m=0;m<16;m++)				//16ﾊﾞｲﾄ分の処理
		{
			if(fprintf(o_fp,"%2.2X",hex_buf[n])<0)
										//ﾃﾞｰﾀ
			{
				printf("出力(txt)ﾌｧｲﾙのfprintf()ｴﾗｰ fname:%s errno:%d\n",o_fname,errno);
				exit(1);
			}
			chksum+=hex_buf[n++];		//ﾁｪｯｸｻﾑを計算する
		}
		if(fprintf(o_fp,"%2.2X\n",(256-chksum)&0xff)<0)
										//ﾁｪｯｸｻﾑﾃﾞｰﾀ
		{
			printf("出力(txt)ﾌｧｲﾙのfprintf()ｴﾗｰ fname:%s errno:%d\n",o_fname,errno);
			exit(1);
		}
	}
	if(fprintf(o_fp,":00000001FF\n")<0)	//ｴﾝﾄﾞﾚｺｰﾄﾞ
	{
		printf("出力(txt)ﾌｧｲﾙのfprintf()ｴﾗｰ fname:%s errno:%d\n",o_fname,errno);
		exit(1);
	}
}


//======================================
//ﾒｲﾝ処理
//======================================
int main(int argc,char *argv[])
{
	unsigned int n,fmt_len,smp_rate,data_len,data_seek;
	unsigned char fmt_sw=0;
	unsigned short ch_su;
	unsigned char no_vol;
	char s[256];


	//ﾊﾟﾗﾒｰﾀ取り込み
	/*DBG*/Hd(argc);
	if(argc<5)
	{
		printf("【ﾊﾟﾗﾒｰﾀ】\n"
			"P1:出力ﾌｧｲﾙ(ｲﾝﾃﾙHEX)のﾊﾟｽ名\n"
			"P2:出力ﾌｧｲﾙ(vos_macｽｸﾘﾌﾟﾄ)のﾊﾟｽ名\n"
			"P3:入力ﾌｧｲﾙ(WAVﾌｧｲﾙﾊﾟｽ名ﾘｽﾄ)のﾊﾟｽ名\n"
			"P4:変換後のｻﾝﾌﾟﾘﾝｸﾞﾚｰﾄ[sps](1000～44100)\n"
			"P5:変換後の量子化ﾋﾞｯﾄ数(8･12･16の何れか)\n");
		return(9);
	}

	//P1:出力ﾌｧｲﾙ(ｲﾝﾃﾙHEX)のﾊﾟｽ名
	o_fname=argv[1];
	/*DBG*/Hs(o_fname);

	//P2:出力ﾌｧｲﾙ(vos_macｽｸﾘﾌﾟﾄ)のﾊﾟｽ名
	c_fname=argv[2];
	/*DBG*/Hs(c_fname);

	//P3:入力ﾌｧｲﾙ(WAVﾌｧｲﾙﾊﾟｽ名ﾘｽﾄ)のﾊﾟｽ名
	i_fname=argv[3];
	/*DBG*/Hs(i_fname);

	//P4:変換後のｻﾝﾌﾟﾘﾝｸﾞﾚｰﾄ[ksps]
	cnv_rate=atoi(argv[4]);
	/*DBG*/Hu(cnv_rate);
	if((cnv_rate<1000)||(44100<cnv_rate))
	{
		printf("変換後のｻﾝﾌﾟﾘﾝｸﾞﾚｰﾄ[sps]が(1000～44100)の範囲でない P4:%u\n",cnv_rate);
		exit(1);
	}

	//P5:変換後の量子化ﾋﾞｯﾄ数(7～16)
	cnv_bits=atoi(argv[5]);
	/*DBG*/Hu(cnv_bits);
	if((cnv_bits!=8)&&(cnv_bits!=12)&&(cnv_bits!=16))
	{
		printf("変換後の量子化ﾋﾞｯﾄ数が8･12･16の何れかでない P5:%u\n",cnv_bits);
		exit(1);
	}

	//出力ﾌｧｲﾙ(ｲﾝﾃﾙHEX)を書出しopenする
	if((o_fp=fopen(o_fname,"w"))==NULL)
	{
		printf("出力ﾌｧｲﾙ(ｲﾝﾃﾙHEX)のfopen()ｴﾗｰ fname:%s errno:%d\n",o_fname,errno);
		exit(1);
	}
	//出力ﾌｧｲﾙ(vos_mac)を書出しopenする
	if((c_fp=fopen(c_fname,"w"))==NULL)
	{
		printf("出力ﾌｧｲﾙ(vos_macｽｸﾘﾌﾟﾄ)のfopen()ｴﾗｰ fname:%s errno:%d\n",c_fname,errno);
		exit(1);
	}
	//入力ﾌｧｲﾙ(WAVﾌｧｲﾙﾊﾟｽ名ﾘｽﾄ)を読込みopenする
	if((i_fp=fopen(i_fname,"r"))==NULL)
	{
		printf("入力ﾌｧｲﾙ(WAVﾌｧｲﾙﾊﾟｽ名ﾘｽﾄ)のfopen()ｴﾗｰ fname:%s errno:%d\n",i_fname,errno);
		exit(1);
	}

	for(;;)
	{
		//WAVﾌｧｲﾙﾊﾟｽ名ﾘｽﾄの読み込み
		if(fgets(w_fname,sizeof(w_fname)-1,i_fp)==NULL)
		{
			if(ferror(i_fp))
			{
				printf("入力ﾌｧｲﾙ(WAVﾌｧｲﾙﾊﾟｽ名ﾘｽﾄ)のfgets()ｴﾗｰ fname:%s errno:%d\n",i_fname,errno);
				exit(1);
			}
			break;
		}
		Hdump(w_fname,16);
		for(n=0;n<sizeof(w_fname);n++)
		{
			if(w_fname[n]=='\n')
			{
				w_fname[n]='\0';
				break;
			}
		}
		Hs(w_fname);
		if(n==0) continue;
		no_vol=1;
		if(n>4)
		{
			if(w_fname[n-5]=='#') no_vol=0;
		}
		else no_vol=1;
		if(w_fname[0]=='/') break;

		//入力ﾌｧｲﾙ(WAV)をﾊﾞｲﾅﾘopenする
		if((w_fp=fopen(w_fname,"rb"))==NULL)
		{
			printf("入力ﾌｧｲﾙ(WAV)のfopen()ｴﾗｰ fname:%s errno:%d\n",w_fname,errno);
			return(1);
		}
		Hx(w_fp);

		//RIFFﾍｯﾀﾞの確認
		getbyte(4);						//RIFFﾍｯﾀﾞ
		if(memcmp(buf.u_char,"RIFF",4))
		{
			printf("入力ﾌｧｲﾙ(WAV)のRIFFﾍｯﾀﾞｴﾗｰ fname:%s\n",w_fname);
			exit(1);
		}

		getbyte(4);						//これ以降のﾌｧｲﾙｻｲｽﾞ

		//WAVEFﾍｯﾀﾞの確認
		getbyte(4);						//WAVEﾍｯﾀﾞ
		if(memcmp(buf.u_char,"WAVE",4))
		{
			printf("入力ﾌｧｲﾙ(WAV)のWAVEﾍｯﾀﾞｴﾗｰ fname:%s\n",w_fname);
			exit(1);
		}

		//ｻﾌﾞﾁｬﾝｸの確認
		for(;;)
		{
			Hm("ｻﾌﾞﾁｬﾝｸ");
			getbyte(4);
			if(memcmp(buf.u_char,"fmt ",4)==0)
			{
				Hm("fmt");
				//fmtﾁｬﾝｸの確認
				getbyte(4);				//fmtﾁｬﾝｸﾊﾞｲﾄ数
				fmt_len=buf.u_int;
				if((buf.u_int!=16)&&(buf.u_int!=18))
				{
					printf("入力ﾌｧｲﾙ(WAV)のfmtﾁｬﾝｸﾊﾞｲﾄ数が18(ﾘﾆｱPCM)でない fname:%s\n",w_fname);
					exit(1);
				}
				getbyte(2);				//ﾌｫｰﾏｯﾄID
				if(buf.u_short[0]!=1)
				{
					printf("入力ﾌｧｲﾙ(WAV)のﾌｫｰﾏｯﾄIDが1(ﾘﾆｱPCM)でない fname:%s\n",w_fname);
					exit(1);
				}
				getbyte(2);				//ﾁｬﾝﾈﾙ数
				if(buf.u_short[0]==1) ch_su=1;
				else if(buf.u_short[0]==2) ch_su=2;
				else
				{
					printf("入力ﾌｧｲﾙ(WAV)のﾁｬﾝﾈﾙ数が1(ﾓﾉﾗﾙ)か2(ｽﾃﾚｵ)でない fname:%s\n",w_fname);
					exit(1);
				}
				Hu(ch_su);
				getbyte(4);				//ｻﾝﾌﾟﾘﾝｸﾞﾚｰﾄ
				if((smp_rate=buf.u_int)==0)
				{
					printf("入力ﾌｧｲﾙ(WAV)のｻﾝﾌﾟﾘﾝｸﾞﾚｰﾄが0である fname:%s\n",w_fname);
					exit(1);
				}
				else if(smp_rate>100000)
				{
					printf("入力ﾌｧｲﾙ(WAV)のｻﾝﾌﾟﾘﾝｸﾞﾚｰﾄが100kを超えている fname:%s\n",w_fname);
					exit(1);
				}
				Hu(smp_rate);
				getbyte(4);				//ﾃﾞｰﾀ速度
				getbyte(2);				//ﾌﾞﾛｯｸｻｲｽﾞ
				if(fmt_len>16) getbyte(4);
										//ｻﾝﾌﾟﾙ当たりﾋﾞｯﾄ数
				else getbyte(2);
				if(buf.u_short[0]==8) smp_byte=1;
				else if(buf.u_short[0]==16) smp_byte=2;
				else
				{
					printf("入力ﾌｧｲﾙ(WAV)のｻﾝﾌﾟﾙ当たりﾋﾞｯﾄ数が8(ﾋﾞｯﾄ)または16(ﾋﾞｯﾄ)でない fname:%s\n",w_fname);
					exit(1);
				}
				Hu(smp_byte);
				fmt_sw=1;
			}
			else if(memcmp(buf.u_char,"fact",4)==0)
			{
				Hm("fact");
				getbyte(4);				//ﾊﾞｲﾄ数
				n=buf.u_int;
				if(fseek(i_fp,n,SEEK_CUR))
				{
					printf("入力ﾌｧｲﾙ(WAV)のfseek()ｴﾗｰ fname:%s errno:%d\n",w_fname,errno);
					exit(1);
				}
			}
			else if(memcmp(buf.u_char,"data",4)==0)
			{
				Hm("data");
				getbyte(4);				//波形ﾃﾞｰﾀﾊﾞｲﾄ数
				if((data_len=buf.u_int)==0)
				{
					printf("入力ﾌｧｲﾙ(WAV)の波形ﾃﾞｰﾀﾊﾞｲﾄ数が0である fname:%s\n",w_fname);
					exit(1);
				}
				Hx(data_len);
				if(fmt_sw==0)
				{
					printf("入力ﾌｧｲﾙ(WAV)のdataﾁｬﾝｸの前にfmtﾁｬﾝｸが無い fname:%s\n",w_fname);
					exit(1);
				}
				if(smp_rate<cnv_rate)
				{
					printf("入力ﾌｧｲﾙ(WAV)のｻﾝﾌﾟﾘﾝｸﾞﾚｰﾄが変換後のｻﾝﾌﾟﾘﾝｸﾞﾚｰﾄ指定値より小さい fname:%s\n",w_fname);
					exit(1);
				}
				matome=(smp_rate+cnv_rate/2)/cnv_rate*ch_su;
				if((data_seek=ftell(w_fp))==0xffffffff)
				{
					printf("入力ﾌｧｲﾙ(WAV)のftell()ｴﾗｰ fname:%s errno:%d\n",w_fname,errno);
					exit(1);
				}
				Hx(data_seek);

				//ﾃﾞｰﾀ数を計算する
				data_n=data_len/smp_byte/matome;
				if((cnv_bits>8)&&(cnv_bits<=12)) data_n&=0xfffffffe;
										//変換後の量子化ﾋﾞｯﾄ数が9～12のときはﾃﾞｰﾀ数を偶数に丸める
				Hx(data_n);

				//vos_macの記述を出力する
				sprintf_s(s,sizeof(s),"\tvos_mac(0x%06x,0x%06x,%u)\t//%d:%s",hex_ad,data_n,cnv_rate,++w_fname_no,w_fname);
				if(fprintf(c_fp,"%s\n",s)<0)
				{
					printf("出力ﾌｧｲﾙ(vosmacｽｸﾘﾌﾟﾄ)のfprintf()ｴﾗｰ fname:%s errno:%d\n",c_fname,errno);
					exit(1);
				}
				printf("%s\n",s);

				if((hex_ad+data_n)>0x1000000)
				{
					printf("合計ﾊﾞｲﾄ数が0x1000000を超えた\n");
					exit(1);
				}

				//HEXﾃﾞｰﾀを生成する
				data_minmax();
				if(fseek(w_fp,data_seek,SEEK_SET))
				{
					printf("入力ﾌｧｲﾙ(WAV)のfseek()ｴﾗｰ fname:%s errno:%d\n",w_fname,errno);
					exit(1);
				}
				make_hex(no_vol);
				break;
			}
			else if(memcmp(buf.u_char,"LIST",4)==0)
			{
				Hm("LIST");
				getbyte(4);				//ﾊﾞｲﾄ数
				n=buf.u_int;
				if(fseek(w_fp,n,SEEK_CUR))
				{
					printf("入力ﾌｧｲﾙ(WAV)のfseek()ｴﾗｰ fname:%s errno:%d\n",w_fname,errno);
					exit(1);
				}
			}
			else
			{
				printf("入力ﾌｧｲﾙ(WAV)のﾁｬﾝｸ識別子が規定外である fname:%s\n",w_fname);
				exit(1);
			}
		}
		//入力ﾌｧｲﾙ(WAV)をcloseする
		if(fclose(w_fp))
		{
			printf("入力ﾌｧｲﾙ(WAV)のclose()ｴﾗｰ fname:%s errno:%d\n",w_fname,errno);
			exit(1);
		}
	}

	//HEXﾌｧｲﾙを書出す
	put_hex();

	if(fclose(o_fp))
	{
		printf("出力ﾌｧｲﾙ(ｲﾝﾃﾙHEX)のclose()ｴﾗｰ fname:%s errno:%d\n",o_fname,errno);
		exit(1);
	}
	if(fclose(c_fp))
	{
		printf("出力ﾌｧｲﾙ(vos_macｽｸﾘﾌﾟﾄ)のclose()ｴﾗｰ fname:%s errno:%d\n",c_fname,errno);
		exit(1);
	}
	printf("HEXﾌｧｲﾙに%uﾊﾞｲﾄ出力しました\n",hex_ad);
	if(fclose(i_fp))
	{
		printf("入力ﾌｧｲﾙ(WAVﾌｧｲﾙﾊﾟｽ名ﾘｽﾄ)のclose()ｴﾗｰ fname:%s errno:%d\n",o_fname,errno);
		exit(1);
	}
	exit(0);
}
