#include <stdio.h>
#include "UnAlz.h"


void Usage()
{
	printf("\n");
	printf("USAGE : unalz sourcefile.alz [dest path] \n");
}

void UnAlzCallback(const char* szMessage, INT64 nCurrent, INT64 nRange, void* param, BOOL* bHalt)
{
	// progress
	char	buf[1000];
	INT64	percent;
	static  INT64 nPrevPercent = -1;
	int		buflen;
	int		i;

	// 파일명 출력..
	if(szMessage)
	{
		printf("\n");
		printf("Extracting : %s      ", szMessage);
		nPrevPercent = -1;
		return ;
	}


	percent = nCurrent*100/nRange;

	if(nPrevPercent==percent) return; 	// 너무 잦은 업데이트 방지..
	nPrevPercent = percent;
#ifdef _WIN32
	sprintf(buf, "%I64d/%I64d (%I64d%%)", nCurrent, nRange, percent);
#else
	sprintf(buf, "%d/%d (%d%%)", (int)nCurrent, (int)nRange, (int)percent);
#endif
	printf("%s", buf);
	buflen = strlen(buf);
	fflush(stdout);

	for(i=0;i<buflen;i++)
		printf("\b");

}

int main(int argc, char* argv[])
{
	printf("unalz v0.20 (2004/10/22) \n");
	printf("copyright(C) 2004 http://www.kipple.pe.kr\n");


	if(argc<2)
	{
		Usage();
		return 0;
	}


	char* source = argv[1];
	char* dest = argv[2];

	if(argc<3) dest=".";	// current dir
	else dest = argv[2];


	CUnAlz	unalz;

	if(unalz.Open(source)==FALSE)
	{
		printf("file open error : %s\nerr code(%d)\n", source,unalz.GetLastErr());
		return 0;
	}

	printf("\nExtract %s to %s\n", source, dest);


	// callback 함수 세팅
	unalz.SetCallback(UnAlzCallback, (void*)NULL);
	if(unalz.ExtractAll(dest)==FALSE)
	{
		printf("extract %s to %s failed.\n", source, dest);
	}
	printf("\ndone..\n");

	return 0;
}

