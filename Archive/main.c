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


// 파일 추가 함수
int append(PARCHIVE archive, char *filename)
{
	int ret = 0; // 함수가 성공했는지 실패했는지 반환

	FILE *fp = fopen(filename, "rb");
	if (fp == NULL)
	{
		printf("%s 파일이 없습니다.\n", filename);
		return -1; // 함수 종료. 실패
	}


	uint8_t *buffer;
	uint32_t size;

	size = getFIleSize(fp); // 추가할 파일의 크기
	buffer = malloc(size);

	// 추가할 파일의 내용 읽음
	if (fread(buffer, size, 1, fp) < 1)
	{
		printf("%s 파일 읽기 실패\n", filename);
		ret = -1;	// -1은 실패\n
		goto Error1;	// fp를 닫고 buffer를 해제하는 에러 처리로 이동
	}

	//새 파일 정보 생성
	PFILE_DESC desc = malloc(sizeof(FILE_DESC));
	memset(desc, 0, sizeof(FILE_DESC));
	strcpy(desc->name, filename);
	desc->size = size;

	fseek(archive->fp, sizeof(ARCHIVE_HEADER), SEEK_SET);

	desc->dataOffset = ftell(archive->fp) + sizeof(FILE_DESC);

	if(fwrite(desc, sizeof(FILE_DESC), 1, archive->fp) < 1)
	{
		printf("파일 정보 쓰기 실패\n");
		ret = -1;
		goto Error2;
	}

	if(fwrite(buffer, size, 1, archive->fp) < 1)
	{
		printf("파일 데이터 쓰기 실패\n");
		ret == -1;
		goto Error2;
	}

	printf("%s" 파일 추가 성공\n크기 : %d\n", filename, size);
}

Error2:
	free(desc);

Error1:
	free(buffer);
	fclose(fp);

	return ret;
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

		// 새 아카이브 헤더 생성
		archive->header.magic = 'AF'		//매직넘버 AF 저장
		archive->header.version = 1;		//파일버전 1 저장

		// 아카이브 파일에 아카이브 헤더 저장
		if (fwrite(&archive->header, sizeof(ARCHIVE_HEADER), 1, fp) < 1)
		{
			printf("아카이브 헤더 쓰기 실패\n");
			fclose(fp);
			return -1;
		}

		archive->fp = fp;		//아카이브파일 포인터 저장

		append(archive,"hello.txt");		//hello.txt 파일 추가
	}

	fclose(fp); // 아카이브 파일 포인터 닫기

	free(archive); // 동적 메모리 해제 
	
	return 0;
}



