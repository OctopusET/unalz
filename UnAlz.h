/*

  COPYRIGHT(C) 2004  hardkoder@gmail.com / http://www.kipple.pe.kr
  저작권 정보 :
    - 이 소스는 자유로이 사용/수정/재배포 가능합니다.
    - 단, 당신이 만들었다고 주장하거나 거짓말하면 안됨.
    - 소스의 오류를 찾았거나, 문제점을 수정하였을 경우 이에 대한 내용을 알려주면 정말 고마울껄..
	- 자신의 프로그램에 이 소스를 사용하였을 경우 원 저작자에게 알려주면 원 저작자가 고마워 할껄..

  소스 설명 : 
	- .ALZ 의 압축을 해제하기 위한 모듈. 
	- ALZ는 BZIP2변형(변형이라고 뭐 개선한게 아니고, 헤더나 CRC 정보등을 빼서 크기를 줄인 것임)과, 
	  DEFLATE 압축 알고리즘에 ZIP과 유사한 헤더를 만들어서 씌운 포맷임.
	  ( bzip2 는 4.9x 에서 사용하였고, deflate 는 5.x 부터 사용하였음. 5.x 부터는 bzip2는 너무 느려서 사용 안함 )
	- UnAlzBz2decompress.c 와 UnAlzbzlib.c 는 원래의 bzip2 소스에서 alz 용으로 수정된 파일임
	  ( deflate 는 변형이 안되었기 때문에 그냥 zlib 를 써도 되지만.. bzip2 는 변형이 되어서 
	  원래의 bzip2 소스를 그대로 쓰면 안된다. )
	- 이 소스는 4칸 탭을 사용 하였음.

  참고 :
    - 원래 소스는 다중 플랫폼에서 사용할수 있게 하기 위해서 stdio 를 사용하였지만 
	  2GB 이상의 파일을 처리하기 위해서 어쩔수 없이 WINDOWS I/O 함수로 전부 바꾸었음.
	- 리눅스등에서 사용하기 위해서 소스 수정이 필요하다면, 공동 작업을 요청해 주셈.

  개발 순서 :
	2004/02/06	- http://www.wotsit.org/ 에서 ZIP File Format Specification version 4.5 [PKWARE Inc.] 를 
				  다운로드 받아서 분석.
	2004/02/07	- 도큐먼트로 unzip 클래스 대충 구현
	2004/02/08	- unzip 클래스를 alzip 포맷에 맞추어 변형 및 테스트
				- deflate, rawdata 압축 해제 구현.
				- 다이알로그 박스 껍데기 씌움.
	2004/02/08	- bzip2 대충 지원
	2004/03/01	- bzip2 거의 완벽 지원 乃
				- callback 구현..
	2004/03/07	- 유틸 함수 추가 ( ExtractCurrentFileToBuf() )
	2004/10/03	- 분할 압축 해제 기능 추가 ( FILE I/O 에 대한 래퍼 클래스 구현 )
	            - 2GB 이상의 파일 처리 지원

  할일 :
	- bzip2 로 압축된 파일의 압축 해제
	- UI 개선 ( PROGRESS, 에러 메시지 출력, 압축 해제 결과 )
	- 분할 압축 파일 지원

	- 암호 걸린 파일 지원
	- 손상된 파일에서의 좀더 많은 테스트
	- 파일 CRC 체크 

  할일중 한일 :
	- bzip2 로 압축된 파일의 압축 해제
	- UI 개선 ( PROGRESS, 에러 메시지 출력, 압축 해제 결과 )
	- 분할 압축 파일 지원
*/


#ifndef _UNALZ_H_
#define _UNALZ_H_

#include <vector>
#include <string>
using namespace std;


#ifndef INT64
#ifdef _WIN32
#	define INT64 __int64
#else
#	define INT64 long long
#endif
#endif

#ifndef DWORD
	typedef unsigned long       DWORD;
#endif // DWORD 
#ifndef SHORT
	typedef short SHORT;
#endif
#ifndef BYTE
	typedef unsigned char       BYTE;
#endif
#ifndef CHAR
	typedef char CHAR;
#endif
#ifndef BYTE
	typedef unsigned char BYTE;
#endif
#ifndef UINT
	typedef unsigned int UINT;
#endif
#ifndef LONG
	typedef long LONG;
#endif
#ifndef BOOL
	typedef int BOOL;
#endif
#ifndef FALSE
#	define FALSE               0
#endif
#ifndef TRUE
#	define TRUE                1
#endif
#ifndef LPCTSTR
	typedef const char* LPCSTR;
#endif
#ifndef HANDLE
#	ifdef _WIN32
	typedef void *HANDLE;
#	else
	typedef FILE *HANDLE;
#	endif
#endif
#ifndef ASSERT
#	include <assert.h>
#	define ASSERT(x) assert(x)
#endif







namespace UNALZ
{

#ifdef _WIN32
#	pragma pack(push, UNALZ, 1)			// structure packing 
#else
#	pragma pack(1)
#endif

static const char UNALZ_VERSION[] = "CUnAlz0.2";
static const char UNALZ_COPYRIGHT[] = "Copyright(C) 2004 hardkoder@gmail.com";


// 맨 파일 앞..
struct SAlzHeader
{
	DWORD	unknown;			// ??
};

/*
union _UGeneralPurposeBitFlag			// zip 에서만 사용..
{
	SHORT	data;
	struct 
	{
		BYTE bit0 : 1;
		BYTE bit1 : 1;
		BYTE bit2 : 1;
		BYTE bit3 : 1;
		BYTE bit4 : 1;
		BYTE bit5 : 1;
	};
};
*/

enum COMPRESSION_METHOD					///<  압축 방법..
{
	COMP_NOCOMP = 0,
	COMP_BZIP2 = 1,
	COMP_DEFLATE = 2,
};

struct _SLocalFileHeaderHead			///<  고정 헤더.
{
	SHORT	fileNameLength;
	BYTE	unknown[5];					///<  ???
	BYTE	fileSizeByte;				///<  파일 크기 필드의 크기 : 0x10, 0x20, 0x40, 0x80 각각 1byte, 2byte, 4byte, 8byte
	BYTE	unknown2[1];				///<  ???

	/*
	SHORT	versionNeededToExtract;
	_UGeneralPurposeBitFlag	generalPurposeBitFlag;
	SHORT	compressionMethod;
	SHORT	lastModFileTime;
	SHORT	lastModFileDate;
	DWORD	crc32;
	DWORD	compressedSize;
	DWORD	uncompressedSize;
	SHORT	fileNameLength;
	SHORT	extraFieldLength;
	*/
};

struct _SDataDescriptor
{
	DWORD	crc32;
	DWORD	compressed;
	DWORD	uncompressed;
};

struct SLocalFileHeader
{
	SLocalFileHeader() { memset(this, 0, sizeof(*this)); }
	//~SLocalFileHeader() { if(fileName) free(fileName); if(extraField) free(extraField); }
	void Clear() { if(fileName) free(fileName); fileName=NULL; if(extraField) free(extraField);extraField=NULL; }
	_SLocalFileHeaderHead	head;

	BYTE					compressionMethod;			///<  압축 방법 : 2 - deflate, 1 - bzip2(?), 0 - 압축 안함.
	BYTE					unknown3[1];				///<  ???
	DWORD					maybeCRC;					///<  아마도 crc

	INT64					compressedSize;
	INT64					uncompressedSize;

	CHAR*					fileName;
	BYTE*					extraField;
	_SDataDescriptor		dataDescriptor;
	INT64					dwFileDataPos;			///<  file data 가 저장된 위치..
};

struct _SCentralDirectoryStructureHead
{
	DWORD	dwUnknown;						///<  항상 NULL 이던데..
	DWORD	dwMaybeCRC;						///<  아마도 crc
	DWORD	dwCLZ03;						///<  "CLZ0x03" - 0x035a4c43 끝을 표시하는듯.
	/*
	SHORT	versionMadeBy;
	SHORT	versionNeededToExtract;
	_UGeneralPurposeBitFlag	generalPurposeBitFlag;
	SHORT	compressionMethod;
	SHORT	lastModFileTime;
	SHORT	lastModFileDate;
	DWORD	crc32;
	DWORD	compressedSize;
	DWORD	uncompressedSize;
	SHORT	fileNameLength;
	SHORT	extraFieldLength;
	SHORT	fileCommentLength;
	SHORT	diskNumberStart;
	SHORT	internalFileAttributes;
	DWORD	externalFileAttributes;
	DWORD	relativeOffsetOfLocalHeader;
	*/
};

struct SCentralDirectoryStructure
{
	SCentralDirectoryStructure() { memset(this, 0, sizeof(*this)); }
	//~SCentralDirectoryStructure() { if(fileName) free(fileName); if(extraField) free(extraField);if(fileComment)free(fileComment); }
	_SCentralDirectoryStructureHead	head;
	/*
	CHAR*	fileName;
	BYTE*	extraField;
	CHAR*	fileComment;
	*/
};


/*
struct _SEndOfCentralDirectoryRecordHead
{
	SHORT	numberOfThisDisk;
	SHORT	numberOfTheDiskWithTheStartOfTheCentralDirectory;
	SHORT	centralDirectoryOnThisDisk;
	SHORT	totalNumberOfEntriesInTheCentralDirectoryOnThisDisk;
	DWORD	sizeOfTheCentralDirectory;
	DWORD	offsetOfStartOfCentralDirectoryWithREspectoTotheStartingDiskNumber;
	SHORT	zipFileCommentLength;
};
*/

/*
struct SEndOfCentralDirectoryRecord
{
	SEndOfCentralDirectoryRecord() { memset(this, 0, sizeof(*this)); }
	~SEndOfCentralDirectoryRecord() { if(fileComment) free(fileComment); }
	_SEndOfCentralDirectoryRecordHead head;
	CHAR*	fileComment;
};
*/

#ifdef _WIN32
#pragma pack(pop, UNALZ)		///<  PACKING 원상 복구
#else
#pragma pack(4)
#endif


///<  PROGRESS CALLBACK FUNCTION - 압축 해제 진행 상황을 알고 싶으면 이걸 쓰면 된다.
typedef void (_UnAlzCallback)(const char* szMessage, INT64 nCurrent, INT64 nRange, void* param, BOOL* bHalt);


class CUnAlz  
{
public:
	CUnAlz();
	~CUnAlz();
	BOOL	Open(const char* szPathName);
	void	Close();
	BOOL	SetCurrentFile(const char* szFileName);
	BOOL	ExtractCurrentFile(const char* szDestPathName, const char* szDestFileName=NULL);
	BOOL	ExtractCurrentFileToBuf(BYTE* pDestBuf, int nBufSize);
	BOOL	ExtractAll(const char* szDestPathName);
	void	SetCallback(_UnAlzCallback* pFunc, void* param=NULL);

public :			///<  WIN32 전용 ( UNICODE 처리용 )
/*
#ifdef _WIN32
	BOOL	Open(LPCWSTR szPathName);
	BOOL	SetCurrentFile(LPCWSTR szFileName);
	static BOOL		IsFolder(LPCWSTR szPathName);
#endif
*/

public :
	typedef vector<SLocalFileHeader>		FileList;			///<  파일 목록.
	const FileList&	GetFileList() { return m_fileList; };		///<  file 목록 리턴
	FileList::iterator GetCurFileHeader() { return m_posCur; };	///<  현재 (SetCurrentFile() 로 세팅된) 파일 정보
//	const SLocalFileHeader* GetCurFileHeader() { return _posCur; };	///<  현재 (SetCurrentFile() 로 세팅된) 파일 정보

public :
	enum ERR							///< 에러 코드 - 정리 필요..
	{
		ERR_NOERR,
		ERR_CANT_OPEN_FILE,				///<  파일 열기 실패
		ERR_CANT_READ_SIG,				///<  signature 읽기 실패
		ERR_CANT_READ_FILE,

		ERR_AT_READ_HEADER,
		ERR_INVALID_FILENAME_LENGTH,
		ERR_INVALID_EXTRAFIELD_LENGTH,
		ERR_CANT_READ_CENTRAL_DIRECTORY_STRUCTURE_HEAD, 
		ERR_INVALID_FILENAME_SIZE,
		ERR_INVALID_EXTRAFIELD_SIZE,
		ERR_INVALID_FILECOMMENT_SIZE,
		ERR_CANT_READ_HEADER,
		ERR_MEM_ALLOC_FAILED,
		ERR_FILE_READ_ERROR,
		ERR_INFLATE_FAILED,
	};
	ERR		GetLastErr(){return m_nErr;}

	enum SIGNATURE							///<  zip file signature
	{
		SIG_ERR								= 0x00,
		SIG_EOF								= 0x01,
		SIG_ALZ_FILE_HEADER					= 0x015a4c41,	///<  ALZ 0x01
		SIG_LOCAL_FILE_HEADER				= 0x015a4c42,	///<  BLZ 0x01
		SIG_CENTRAL_DIRECTORY_STRUCTURE		= 0x015a4c43,	///<  CLZ 0x01
		SIG_ENDOF_CENTRAL_DIRECTORY_RECORD	= 0x025a4c43,	///<  CLZ 0x02
	};

public :
	static BOOL			DigPath(const char* szPathName);
	static BOOL			IsFolder(LPCSTR szPathName);
	static const char*	GetVersion() { return UNALZ_VERSION; }
	static const char*	GetCopyright() { return UNALZ_COPYRIGHT; }

private :
	SIGNATURE	ReadSignature();
	BOOL		ReadAlzFileHeader();
	BOOL		ReadLocalFileheader();
	BOOL		ReadCentralDirectoryStructure();
	BOOL		ReadEndofCentralDirectoryRecord();

private :
	enum EXTRACT_TYPE				///<   압축 해제 타입.
	{
		ET_FILE,					///<  FILE*
		ET_MEM,						///<  memory buffer
	};
	struct		SExtractDest		///<  압축 해제 대상.
	{
		SExtractDest() { memset(this, 0, sizeof(SExtractDest)); }
		EXTRACT_TYPE nType;			///<  대상이 파일인가  메모리 인가..
		FILE*		fp;				///<  ET_FILE 일 경우 대상 FILE*
		BYTE*		buf;			///<  ET_MEM 일 경우 대상 포인터
		DWORD		bufsize;		///<  ET_MEM 일 경우 대상 버퍼의 크기
		DWORD		bufpos;			///<  ET_MEM 일 경우 대상 버퍼에 쓰고 있는 위치
	};
	int			WriteToDest(SExtractDest* dest, BYTE* buf, int nSize);

private :
	BOOL		ExtractTo(SExtractDest* dest);

	//BOOL		ExtractDeflate(FILE* fp, SLocalFileHeader& file);
	//BOOL		ExtractBzip2_bak(FILE* fp, SLocalFileHeader& file); - 실패한(잘못된) 방법 
	BOOL		ExtractDeflate2(SExtractDest* dest, SLocalFileHeader& file);
	BOOL		ExtractBzip2(SExtractDest* dest, SLocalFileHeader& file);
	BOOL		ExtractRawfile(SExtractDest* dest, SLocalFileHeader& file);

private :		// bzip2 파일 처리 함수..
	typedef void MYBZFILE;
	MYBZFILE*	BZ2_bzReadOpen(int* bzerror, CUnAlz* f, int verbosity, int _small, void* unused, int nUnused);
	int			BZ2_bzread(MYBZFILE* b, void* buf, int len );
	int			BZ2_bzRead(int* bzerror, MYBZFILE* b, void* buf, int len);
	void		BZ2_bzReadClose( int *bzerror, MYBZFILE *b );

private :		// 분할 압축 파일 처리를 위한 래퍼(lapper?) 클래스
	BOOL		FOpen(const char* szPathName);
	void		FClose();
	INT64		FTell();
	BOOL		FEof();
	BOOL		FSeek(INT64 offset);
	BOOL		FRead(void* buffer, DWORD nBytesToRead, int* pTotRead=NULL);

	enum		{MAX_FILES=1000};								///< 처리 가능한 분할 압축 파일 수.
	enum		{MULTIVOL_TAIL_SIZE=16,MULTIVOL_HEAD_SIZE=8};	///< 분할 압축시 꼴랑지, 헤더 크기 
	struct SFile												///< 분할 파일 정보
	{
		HANDLE	fp;
		INT64	nFileSize;
		int		nMultivolHeaderSize;
		int		nMultivolTailSize;
	};

	SFile		m_files[MAX_FILES];								///< 분할 파일 저장 array - 무식한가?
	int			m_nCurFile;										///< m_files 에서 현재 처리중인 파일의 위치.
	int			m_nFileCount;									///< 분할 파일 갯수..
	INT64		m_nVirtualFilePos;								///< 멀티볼륨에서의 가상의 위치
	INT64		m_nCurFilePos;									///< 현재 파일의 물리적 위치.
	BOOL		m_bIsEOF;										///< 파일의 끝까지 (분할 파일 포함해서) 왔나?


private :
	FileList			m_fileList;								///< 압축파일 내의 파일 목록
	ERR					m_nErr;
	FileList::iterator	m_posCur;								///< 현재 파일
	_UnAlzCallback*		m_pFuncCallBack;
	void*				m_pCallbackParam;
	BOOL				m_bHalt;
};
}

using namespace UNALZ;

#endif
