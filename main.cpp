#include <stdio.h>
#include "UnAlz.h"


void Usage()
{
	printf("\n");
#ifdef _UNALZ_ICONV
		printf("USAGE : unalz [-utf8 | -cp949] sourcefile.alz [dest path] \n");
#	ifdef _UNALZ_UTF8
		printf("        -utf8  : convert filename's codepage to UTF-8 (default)\n");
		printf("        -cp949 : convert filename's codepage to CP949\n");
		printf("        -euc-kr: convert filename's codepage to EUC-KR\n");
#	else
		printf("        -utf8  : convert filename's codepage to UTF-8\n");
		printf("        -cp949 : convert filename's codepage to CP949 (default)\n");
		printf("        -euc-kr: convert filename's codepage to EUC-KR\n");
#	endif // _UNALZ_UTF8
#else		// no iconv
	printf("USAGE : unalz sourcefile.alz [dest path] \n");
#endif // _UNALZ_ICONV
}

void UnAlzCallback(const char* szMessage, INT64 nCurrent, INT64 nRange, void* param, BOOL* bHalt)
{
	// progress
	static char szFileName[1024]={0};
	INT64	percent;
	static  INT64 nPrevPercent = -1;

	// 파일명 출력..
	if(szMessage)
	{
		printf("\n");
#ifdef _WIN32
		sprintf(szFileName, "unalziiiing : %s (%I64dbytes) ", szMessage, nRange);
#else
		sprintf(szFileName, "unalziiiing : %s (%lldbytes) ", szMessage, nRange);
#endif
		printf("%s", szFileName);
		fflush(stdout);
		nPrevPercent = -1;
		return;
	}

	percent = nCurrent*100/nRange;

	if(nPrevPercent/10==percent/10) return; 	// 너무 잦은 업데이트 방지..
	nPrevPercent = percent;

	printf(".");
	fflush(stdout);

	/*
	char	buf[1024];
#ifdef _WIN32
	sprintf(buf, "\r%s %I64d/%I64d (%I64d%%)", szFileName, nCurrent, nRange, percent);
#else
//	sprintf(buf, "\r%s %d/%d (%d%%)", szMessage, (int)nCurrent, (int)nRange, (int)percent);		// int64 를 출력할라면 어찌해야 되는지?
	sprintf(buf, "\r%s %lld/%lld (%lld%%)", szFileName, nCurrent, Range, percent);
#endif
	puts(buf);
	fflush(stdout);
	*/
}


int main(int argc, char* argv[])
{
//	printf("unalz v0.20 (2004/10/22) \n");
//	printf("unalz v0.22 (2004/10/27) \n");
	printf("unalz v0.23 (2004/10/30) \n");
	printf("copyright(C) 2004 http://www.kipple.pe.kr\n");

	if(argc<2)
	{
		Usage();
		return 0;
	}

	CUnAlz	unalz;
	char* source=NULL;
	char* destpath=".";
	char* destcodepage=NULL;
	int   count=1;

	// utf8 옵션 처리
#ifdef _UNALZ_ICONV
	if(strcmp(argv[count], "-utf8")==0)
	{
		destcodepage = "UTF-8";				// utf-8 support
		count++;
	}
	else if(strcmp(argv[count], "-cp949")==0)
	{
		destcodepage = "CP949";				// cp959 
		count++;
	}
	else if(strcmp(argv[count], "-euc-kr")==0)
	{
		destcodepage = "EUC-KR";			// EUC-KR
		count++;
	}
	if(count>=argc)	{Usage();return 0;}		// 옵션만 쓰면 어쩌라고..

	if(destcodepage) unalz.SetDestCodepage(destcodepage);
#endif

	// 소스 파일
	source=argv[count];						
	count++;

	// 대상 경로
	if(count<argc)							
	{
		destpath = argv[count];
		count++;
	}


	// 파일 열기
	if(unalz.Open(source)==FALSE)
	{
		printf("file open error : %s\n", source);
		printf("err code(%d) (%s)\n", unalz.GetLastErr(), unalz.GetLastErrStr());
		return 0;
	}

	printf("\nExtract %s to %s\n", source, destpath);


	// callback 함수 세팅
	unalz.SetCallback(UnAlzCallback, (void*)NULL);
	if(unalz.ExtractAll(destpath)==FALSE)
	{
		printf("extract %s to %s failed.\n", source, destpath);
		printf("err code(%d) (%s)\n", unalz.GetLastErr(), unalz.GetLastErrStr());
	}
	printf("\ndone..\n");

	return 0;
}

