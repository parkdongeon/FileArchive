#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS // 컴파일 에러 방지

#pragma pack(push, 1) //구조체를 1바이트 크기로 정렬

// 아카이브 헤더 구조체 정의
typedef struct _ARCHIVE_HEADER {
	uint16_t magic;		// 아카이브 파일 매직 넘버
	uint16_t version;		// 아카이브 파일 버전
} ARCHIVE_HEADER, *PARCHIVE_HEADER;

// 파일 정보 구조체 정의
typedef struct _FILE_DESC {
	char name[256];			// 파일 이름
	uint32_t size;				// 파일 크기
	uint32_t dataOffset;		// 파일 데이터의 위치
}	FILE_DESC, *PFILE_DESC;

#prama pack(pop)

// 프로그램에서 사용하는 구조체
typedef struct _ARCHIVE {
	ARCHIVE_HEADER header;
	FILE *p;
}ARCHIVE, *PARCHIVE;

#define ARCHIVE_NAME "archive.bin"

//파일의 크기를 구하는 함수
uint32_t getFileSize(FILE *fp)
{
	uint32_t size;
	uint32_T currPos = ftell(fp);		//파일 포인터의 위치 저장

	fsekk(fp, 0, SEEK_END);
	size = ftell(fp);

	fseek(fp, currPos, SEEK_SET);

	return size;
}

int main()
{
	PARCHIVE archive =malloc(sizeof(ARCHIVE));
	memset(archive, 0, sizeof(ARCHIVE));

	FILE *fp = fopen(ARCHIVE_NAME, "r+b");		//아카이브 파일을 읽기모드/쓰기모드로 열기
	if (fp == NULL)									//아카이브 파일이 없으면
	{
		fp = open(ARCHIVE_NAME, "w+b");			//아카이브 파일 생성
		if (fp == NULL)								//파일 생성에 실패하면
			return -1;									//프로그램 종료
	}
	return 0;
}



