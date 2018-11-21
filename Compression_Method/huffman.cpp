#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define L 10000
#define ASCII 256


struct node
{
	char Character;
	int freq;
	struct node *next_L;
	struct node *next_R;
};



void Scan_File(unsigned char *Input)
{
	FILE *IFile = fopen("Input.txt", "r");		//Input.txt를 "r"로 읽어온다.

	int i = 0;

	while (feof(IFile) == 0)					//파일의 끝이 될 때까지 반복
	{
		Input[i] = fgetc(IFile);				//Input.txt의 내용을 Input배열에 넣는다.
		i++;
	}
	fclose(IFile);
}

void Check_Freq(unsigned char *Input, int *Freq)
{
	for (int i = 0; i < strlen((char*)Input) - 1; i++)		//Input배열의 길이 만큼 반복 (==모든 글자를 한번씩 확인해본다)
	{
		Freq[(int)Input[i]]++;						//Freq[K] = Input배열의 i번쨰 글자의 ASCII값이 K이며 K가 반복된 횟수
													//K = Input배열의 i번쨰 글자의 ASCII값
													//
													//따라서 Freq[(int)Input[i]]를 1씩 증가시켜준다.
	}
}

void Show_Freq(int* Freq)
{
	for (int i = 0; i < ASCII; i++)
	{
		if (Freq[i] > 0)
		{
			if (i == 10)	//줄바꿈표시라 출력이 직관적이지 않음
			{
				printf("ASCII값 = %3d < \\n> %3d회\n", i, Freq[i]);
			}
			else
			{
				printf("ASCII값 = %3d < %c > %3d회\n", i, i, Freq[i]);
			}
		}
	}
}


int Count_Non_Zero(int *Freq)
{
	int cnt = 0;

	for (int i = 0; i < ASCII; i++)		//모든 글자를 확인해본다.
	{
		if (Freq[i] > 0)					//만약 그 글자가 1번 이상 나타났을 시
		{
			cnt++;								//cnt를 1 증가 시켜준다.
		}
	}
	return cnt;
}



int main()
{
	unsigned char Input[L] = {};			//.txt내용이 저장될 배열
	Scan_File(Input);

	int Freq[ASCII] = {};		//Input의 빈도수를 ASCII에 의해 체크해주는 배열
	Check_Freq(Input, Freq);	//빈도수를 체크해주는 함수
	Show_Freq(Freq);
}
