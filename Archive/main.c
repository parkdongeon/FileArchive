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
