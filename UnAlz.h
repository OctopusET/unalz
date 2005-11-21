/*

  COPYRIGHT(C) 2004-2005  hardkoder , http://www.kipple.pe.kr

  저작권 정보 : ( BSD License )
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
	            - 2GB 이상의 파일 처리 지원 (WINDOWS ONLY)
	2004/10/22	- 다중 플랫폼(BSD/LINUX)지원을 위한 수정
				  ( BSD/LINUX 의 경우 2GB 이하의 파일만 지원 )
				- unalz 0.20
	2004/10/23	- by xxfree86 : DARWIN 컴파일 지원, 경로명에 "\\" 포함시 문제점 수정
	2004/10/24	- by aqua0125 : 코드페이지 변환처리, 64bit 파일 처리
				- 빅엔디안, 코드페이지 변환 관련 소스 정리
	2004/10/25	- by yongari : __LP64__ , 빅엔디안(le64toh/le132oh/le16toh) 관련 이슈 수정
	2004/10/26	- BSD/LINUX : byte-order, libiconv 이슈 정리 
				- unalz 0.22
	2004/10/30	- 정리 & 정리.. 
				- unalz 0.23
	2004/11/14	- by xxfree86 : 암호 걸린 파일 처리 추가
				- unalz 0.30
	2004/11/27	- cygwin에서 컴파일 되도록 수정
	            - 암호처리 부분에 일부 사용된 GPL 의 CZipArchive 코드를 "ZIP File Format Specification version 4.5" 문서를 참고해서 다시 코딩 & 정리
				- 암호걸린 파일과 안걸린 파일 섞였을때 처리
				- 파일의 뒷부분이 잘려서 손상된 파일도 멀쩡한 부분까지는 압축을 풀도록 수정
				- unalz 0.31
	2005/01/08	- 암호 잘못 입력시 한번 체크후 두번째는 정상 암호를 입력해서 풀지 못하게 되던 버그 수정
	2005/02/05	- 압축 해제후 deflate 의 파일 CRC 확인 기능 추가
	2005/03/07	- bzip2, raw 파일에 대한 파일 CRC 확인 기능 추가
	2005/03/13	- ALZ 파일이 아닐경우 에러 코드(ERR_NOT_ALZ_FILE) 추가
	2005/06/16	- GetFileList() 함수 버그 수정(리턴타입 변경)
	2005/06/18	- by goweol : utf-8 사용시 파일이름에서 버퍼 오버플로우 발생하던 버그 수정
				- unalz 0.4
	2005/06/22	- by goweol : -l 옵션으로 파일 리스팅 기능 추가
				- UnAlzUtils.cpp/h 파일을 프로젝트에 추가 
	2005/06/29	- by xxfree86 : MacOSX 10.4.1  gcc 4.0 에서 iconv 관련 컴파일 에러 수정
				- 빅엔디안에서 CRC 체크시 에러 발생하는 문제점 수정(?)
	2005/07/02	- unalz 커맨드 라인 방식 변경, 압축풀 대상 파일 지정 기능 추가..
				- 압축 해제된 파일시간을 원래 시간으로 세팅하는 코드 추가 - from unalz_wcx_01i.zip
	2005/07/09	- unalz 0.5
	2005/07/24	- -d 로 대상 경로를 "/" 로 시작되는 절대경로로 지정하면 프로그램이 죽던 버그 수정(Pavel Roskin)
				- pipemode 추가 - 메시지없이 파이프로 출력한다(Pavel Roskin)
				- 리스트 모드에서 파일 날자/시간을 시스템 로케일 설정에 따라서 표시(Pavel Roskin)
				- 커맨드라인에서 -pwd 옵션으로 암호 지정기능 추가
				- unalz 0.51
	2005/07/27	- main() 에 setlocale() 추가
				- unalz 0.52
	2005/10/15	- NetBSD 에서 컴파일 되도록 수정 ( by minskim@bawi )
				- unalz 0.53
	2005/11/21	- 파일명 처리쪽에 있는 Buffer overflow 문제 수정 ( by Ulf Harnhammar <metaur@telia.com> )
				- unalz 0.54

  
  기능 :
	- alz 파일의 압축 해제 (deflate/변형 bzip2/raw)
	- 분할 압축 파일 지원 (alz, a00, a01.. )
	- 다양한 플래폼 지원 (Win32/POSIX(BSD/LINUX/DARWIN))
	- 암호걸린 파일의 압축 해제
	- 뒷부분이 잘린 파일도 멀쩡한 부분까지 압축 해제 가능
	- CRC 체크기능


  컴파일 옵션 ( -DXXXX )
	- _WIN32 : WIN32 
	- _UNALZ_ICONV : iconv 를 사용해서 code 페이지 변환 지원
	- _UNALZ_UTF8 : _UNALZ_ICONV 를 사용할 경우 기본 코드페이지를 "UTF-8" 로 지정

*/


#ifndef _UNALZ_H_
#define _UNALZ_H_

#include <vector>
using namespace std;


#ifndef INT64
#ifdef _WIN32
#	define INT64 __int64
#else
#	define INT64 long long
#endif
#endif

#ifndef UINT64
#ifdef _WIN32
#	define UINT64 unsigned __int64
#else
#	define UINT64 unsigned long long
#endif
#endif

#ifndef UINT32
	typedef unsigned int		UINT32;
#endif

#ifndef UINT16
	typedef unsigned short		UINT16;
#endif

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
#ifndef ULONG
	typedef unsigned long ULONG;   // same as DWORD? i don't know.
#endif
#ifndef BOOL
#	ifndef BOOL_DEFINED		// 이미 BOOL 이 DEFINE 되어 있으면 BOOL_DEFINED 를 define 해서 컴파일 에러를 막을 수 있다.
	typedef int BOOL;
#	endif
#endif
#ifndef FALSE
#	define FALSE               0
#endif
#ifndef TRUE
#	define TRUE                1
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
//#	define ASSERT(x) assert(x)
#	define ASSERT(x) {printf("unalz assert at file:%s line:%d\n", __FILE__, __LINE__);}
#endif




namespace UNALZ
{

#ifdef _WIN32
#	pragma pack(push, UNALZ, 1)			// structure packing 
#else
#	pragma pack(1)
#endif

static const char UNALZ_VERSION[]   = "CUnAlz0.53";
static const char UNALZ_COPYRIGHT[] = "Copyright(C) 2004-2005 by hardkoder ( http://www.kipple.pe.kr ) ";

enum		{ENCR_HEADER_LEN=12}; // xf86
// 맨 파일 앞..
struct SAlzHeader
{
	UINT32	unknown;			// ??
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
	COMP_UNKNOWN = 3,					// unknown!
};

enum ALZ_FILE_ATTRIBUTE
{
	ALZ_FILEATTR_READONLY	= 0x1,
	ALZ_FILEATTR_HIDDEN		= 0x2,
	ALZ_FILEATTR_DIRECTORY	= 0x10,
	ALZ_FILEATTR_FILE		= 0x20,			
};

enum ALZ_FILE_DESCRIPTOR
{
	ALZ_FILE_DESCRIPTOR_ENCRYPTED			= 0x01,		// 암호 걸린 파일
	ALZ_FILE_DESCRIPTOR_FILESIZEFIELD_1BYTE = 0x10,		// 파일 크기 필드의 크기
	ALZ_FILE_DESCRIPTOR_FILESIZEFIELD_2BYTE = 0x20,
	ALZ_FILE_DESCRIPTOR_FILESIZEFIELD_4BYTE = 0x40,
	ALZ_FILE_DESCRIPTOR_FILESIZEFIELD_8BYTE = 0x80,
};

struct _SLocalFileHeaderHead			///<  고정 헤더.
{
	SHORT	fileNameLength;
	BYTE    fileAttribute;			    // from http://www.zap.pe.kr, enum FILE_ATTRIBUE 참고
	UINT32  fileTimeDate;				// dos file time
	
	BYTE	fileDescriptor;				///<  파일 크기 필드의 크기 : 0x10, 0x20, 0x40, 0x80 각각 1byte, 2byte, 4byte, 8byte.
										///<  fileDescriptor & 1 == 암호걸렸는지 여부
	BYTE	unknown2[1];				///<  ???

	/*
	SHORT	versionNeededToExtract;
	_UGeneralPurposeBitFlag	generalPurposeBitFlag;
	SHORT	compressionMethod;
	SHORT	lastModFileTime;
	SHORT	lastModFileDate;
	UINT32	crc32;
	UINT32	compressedSize;
	UINT32	uncompressedSize;
	SHORT	fileNameLength;
	SHORT	extraFieldLength;
	*/
};

/*
struct _SDataDescriptor
{
	UINT32	crc32;
	UINT32	compressed;
	UINT32	uncompressed;
};
*/

struct SLocalFileHeader
{
	SLocalFileHeader() { memset(this, 0, sizeof(*this)); }
	//~SLocalFileHeader() { if(fileName) free(fileName); if(extraField) free(extraField); }
	void Clear() { if(fileName) free(fileName); fileName=NULL; if(extraField) free(extraField);extraField=NULL; }
	_SLocalFileHeaderHead	head;

	BYTE					compressionMethod;			///< 압축 방법 : 2 - deflate, 1 - 변형 bzip2, 0 - 압축 안함.
	BYTE					unknown;
	UINT32					fileCRC;					///< 파일의 CRC, 최상위 바이트는 암호 체크용으로도 사용된다.

	INT64					compressedSize;
	INT64					uncompressedSize;

	CHAR*					fileName;
	BYTE*					extraField;
//	_SDataDescriptor		dataDescriptor;
	INT64					dwFileDataPos;				///<  file data 가 저장된 위치..
	
	BYTE					encChk[ENCR_HEADER_LEN];	// xf86
};

struct _SCentralDirectoryStructureHead
{
	UINT32	dwUnknown;						///<  항상 NULL 이던데..
	UINT32	dwUnknown2;						///<  아마도 crc
	UINT32	dwCLZ03;						///<  "CLZ0x03" - 0x035a4c43 끝을 표시하는듯.
	/*
	SHORT	versionMadeBy;
	SHORT	versionNeededToExtract;
	_UGeneralPurposeBitFlag	generalPurposeBitFlag;
	SHORT	compressionMethod;
	SHORT	lastModFileTime;
	SHORT	lastModFileDate;
	UINT32	crc32;
	UINT32	compressedSize;
	UINT32	uncompressedSize;
	SHORT	fileNameLength;
	SHORT	extraFieldLength;
	SHORT	fileCommentLength;
	SHORT	diskNumberStart;
	SHORT	internalFileAttributes;
	UINT32	externalFileAttributes;
	UINT32	relativeOffsetOfLocalHeader;
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
	UINT32	sizeOfTheCentralDirectory;
	UINT32	offsetOfStartOfCentralDirectoryWithREspectoTotheStartingDiskNumber;
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
#	pragma pack(pop, UNALZ)		///<  PACKING 원상 복구
#else
#	ifdef __LP64__				// 64bit 는 패킹이 8byte 맞나 ? 잘모르겠다.....
#		pragma pack(8)
#	else
#		pragma pack(4)
#	endif
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
	BOOL	ExtractCurrentFileToBuf(BYTE* pDestBuf, int nBufSize);		// pDestBuf=NULL 일 경우 테스트만 수행한다.
	BOOL	ExtractAll(const char* szDestPathName);
	void	SetCallback(_UnAlzCallback* pFunc, void* param=NULL);
	void	SetPipeMode(BOOL bPipeMode) {m_bPipeMode=bPipeMode;}

	void	setPassword(char *passwd) { if(strlen(passwd) == 0) return; strcpy(m_szPasswd, passwd); };  // xf86
	BOOL	chkValidPassword();			// xf86
	BOOL	IsEncrypted() { return m_bIsEncrypted; };

#ifdef _UNALZ_ICONV
	void	SetDestCodepage(const char* szToCodepage) { strcpy(m_szToCodepage, szToCodepage); }
#endif

public :			///<  WIN32 전용 ( UNICODE 처리용 )

#ifdef _WIN32
#ifndef __GNUWIN32__
#ifndef LPCWSTR
	typedef const wchar_t* LPCWSTR;
#endif
	BOOL	Open(LPCWSTR szPathName);
	BOOL	SetCurrentFile(LPCWSTR szFileName);
	static BOOL		IsFolder(LPCWSTR szPathName);
#endif // __GNUWIN32__	
#endif // _WIN32

public :
	typedef vector<SLocalFileHeader>		FileList;					///<  파일 목록.
	FileList*			GetFileList() { return &m_fileList; };			///<  file 목록 리턴
	void				SetCurrentFile(FileList::iterator newPos);		///< low level 접근..
	FileList::iterator	GetCurFileHeader() { return m_posCur; };		///<  현재 (SetCurrentFile() 로 세팅된) 파일 정보

public :
	enum ERR							///< 에러 코드 - 정리 필요..
	{
		ERR_NOERR,
		ERR_CANT_OPEN_FILE,				///< 소스 파일 열기 실패
		ERR_CANT_OPEN_DEST_FILE,		///< 대상 파일 열기 실패
//		ERR_CANT_CREATE_DEST_PATH,		///< 대상 경로 만들기 실패
		ERR_CORRUPTED_FILE,				///< 깨진 파일?
		ERR_NOT_ALZ_FILE,				///< ALZ 파일이 아니다.
		ERR_CANT_READ_SIG,				///< signature 읽기 실패
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
		ERR_BZIP2_FAILED,
		ERR_INVALID_FILE_CRC,	
		ERR_UNKNOWN_COMPRESSION_METHOD,

		ERR_ICONV_CANT_OPEN,
		ERR_ICONV_INVALID_MULTISEQUENCE_OF_CHARACTERS,
		ERR_ICONV_INCOMPLETE_MULTIBYTE_SEQUENCE,
		ERR_ICONV_NOT_ENOUGH_SPACE_OF_BUFFER_TO_CONVERT,
		ERR_ICONV_ETC,

		ERR_PASSWD_NOT_SET,
		ERR_INVALID_PASSWD,
		ERR_USER_ABORTED,

	};
	ERR		GetLastErr(){return m_nErr;}
	const char* GetLastErrStr(){return LastErrToStr(m_nErr);}
	const char* LastErrToStr(ERR nERR);

	enum SIGNATURE							///<  zip file signature - little endian
	{
		SIG_ERROR							= 0x00,
		SIG_EOF								= 0x01,
		SIG_ALZ_FILE_HEADER					= 0x015a4c41,	///<  ALZ 0x01
		SIG_LOCAL_FILE_HEADER				= 0x015a4c42,	///<  BLZ 0x01
		SIG_CENTRAL_DIRECTORY_STRUCTURE		= 0x015a4c43,	///<  CLZ 0x01
		SIG_ENDOF_CENTRAL_DIRECTORY_RECORD	= 0x025a4c43,	///<  CLZ 0x02
	};

public :
	static BOOL			DigPath(const CHAR* szPathName);
	static BOOL			IsFolder(const CHAR* szPathName);
	static const char*	GetVersion() { return UNALZ_VERSION; }
	static const char*	GetCopyright() { return UNALZ_COPYRIGHT; }
	BOOL				IsHalted() { return m_bHalt; }		// by xf86

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
		UINT32		bufsize;		///<  ET_MEM 일 경우 대상 버퍼의 크기
		UINT32		bufpos;			///<  ET_MEM 일 경우 대상 버퍼에 쓰고 있는 위치
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

private :		// 분할 압축 파일 처리를 위한 래퍼(lapper^^?) 클래스
	BOOL		FOpen(const char* szPathName);
	void		FClose();
	INT64		FTell();
	BOOL		FEof();
	BOOL		FSeek(INT64 offset);
	BOOL		FRead(void* buffer, UINT32 nBytesToRead, int* pTotRead=NULL);

	BOOL		IsDataDescr() { return m_bIsDataDescr; };   // xf86
	int			getPasswordLen() { return strlen(m_szPasswd); };

	enum		{MAX_FILES=1000};								///< 처리 가능한 분할 압축 파일 수.
	enum		{MULTIVOL_TAIL_SIZE=16,MULTIVOL_HEAD_SIZE=8};	///< 분할 압축시 꼴랑지, 헤더 크기 
	struct SFile												///< 분할 파일 정보
	{
		HANDLE	fp;
		INT64	nFileSize;
		int		nMultivolHeaderSize;
		int		nMultivolTailSize;
	};

	SFile		m_files[MAX_FILES];					///< 분할 파일 저장 array - 무식한가?
	int			m_nCurFile;							///< m_files 에서 현재 처리중인 파일의 위치.
	int			m_nFileCount;						///< 분할 파일 갯수..
	INT64		m_nVirtualFilePos;					///< 멀티볼륨에서의 가상의 위치
	INT64		m_nCurFilePos;						///< 현재 파일의 물리적 위치.
	BOOL		m_bIsEOF;							///< 파일의 끝까지 (분할 파일 포함해서) 왔나?

	BOOL		m_bIsEncrypted;		// xf86
	BOOL		m_bIsDataDescr;
	char		m_szPasswd[512];
	BOOL		m_bPipeMode;						///< pipemode - 메시지 출력없이 stdout 으로만 출력

private :
	/*			from CZipArchive
	void		CryptDecodeBuffer(UINT32 uCount, CHAR *buf);
	void		CryptInitKeys();
	void		CryptUpdateKeys(CHAR c);
	BOOL		CryptCheck(CHAR *buf);
	CHAR		CryptDecryptCHAR();
	void		CryptDecode(CHAR &c);
	UINT32		CryptCRC32(UINT32 l, CHAR c);
	*/

private :		// encryption 처리
	BOOL		IsEncryptedFile(BYTE fileDescriptor);
	BOOL		IsEncryptedFile();
	void		InitCryptKeys(const CHAR* szPassword);
	void		UpdateKeys(BYTE c);
	BOOL		CryptCheck(const BYTE* buf);
	BYTE		DecryptByte();
	void		DecryptingData(int nSize, BYTE* data);
	UINT32		CRC32(UINT32 l, BYTE c);
	UINT32		m_key[3];

private :
	FileList			m_fileList;					///< 압축파일 내의 파일 목록
	ERR					m_nErr;
	FileList::iterator	m_posCur;					///< 현재 파일
	_UnAlzCallback*		m_pFuncCallBack;
	void*				m_pCallbackParam;
	BOOL				m_bHalt;

#ifdef _UNALZ_ICONV
	char				m_szToCodepage[256];		///< codepage 
	char				m_szFromCodepage[256];		///< "CP949"
#endif
};
}

using namespace UNALZ;

#endif
