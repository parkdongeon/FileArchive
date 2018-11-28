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

struct node **Heap;			
							
char Code[30] = {};
int Code_Last_Idx = -1;

char Save_Code[ASCII][30] = {};		
						
					
int Last_Idx_Heap = 0;		


void Scan_File(unsigned char *Input)
{
	FILE *IFile = fopen("Input.txt", "r");		

	int i = 0;

	while (feof(IFile) == 0)					
	{
		Input[i] = fgetc(IFile);				
		i++;
	}
	fclose(IFile);
}

void Check_Freq(unsigned char *Input, int *Freq)
{
	for (int i = 0; i < strlen((char*)Input) - 1; i++)		
	{
		Freq[(int)Input[i]]++;						
													
													
													
	}
}

void Show_Freq(int* Freq)
{
	for (int i = 0; i < ASCII; i++)
	{
		if (Freq[i] > 0)
		{
			if (i == 10)	
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

	for (int i = 0; i < ASCII; i++)		
	{
		if (Freq[i] > 0)					
		{
			cnt++;								
		}
	}
	return cnt;
}



void Make_Heap(int *Freq)
{
	int Non_Zero_Char = Count_Non_Zero(Freq);

	Heap = (struct node **)malloc((Non_Zero_Char + 1) * sizeof(struct node *));		

	memset(Heap, 0, Non_Zero_Char * sizeof(struct node *));							
																					
}

void add_To_Heap(struct node * Heap_element)
{
	Last_Idx_Heap++;							

	Heap[Last_Idx_Heap] = Heap_element;			
												

	int children = Last_Idx_Heap;				
												

	int parents = children / 2;					

	while (children != 1)						
	{
		if (Last_Idx_Heap == 1)							
		{
			return;											
		}
		else											
		{
			if (Heap[children]->freq <= Heap[parents]->freq)		
			{													
				struct node *Temp;
				Temp = Heap[parents];
				Heap[parents] = Heap[children];
				Heap[children] = Temp;									

				children = parents;										

				parents = children / 2;									
			}
			else												
			{
				break;													
			}
		}
	}
}

void Make_Min_Heap(int *Freq)
{
	for (int i = 0; i < ASCII; i++)			
	{
		if (Freq[i] > 0)					
		{
			struct node *Heap_element = (struct node *)malloc(sizeof(struct node));			
			Heap_element->Character = (char)i;												
			Heap_element->freq = Freq[i];													
			Heap_element->next_L = 0;														
			Heap_element->next_R = 0;														
																							

			add_To_Heap(Heap_element);			
		}
	}
}

struct node *Delete_From_Heap()
{
	if (Last_Idx_Heap < 1)
	{
		return 0;
	}

	struct node *Output_Node;
	Output_Node = Heap[1];

	Heap[1] = Heap[Last_Idx_Heap];
	Last_Idx_Heap--;

	int New_First_Node = 1;
	int Left_Children = 2 * New_First_Node;
	int Right_Children = 2 * New_First_Node + 1;


	while (1)
	{
		if (Last_Idx_Heap < Left_Children)		
		{
			break;
		}
		else if (Last_Idx_Heap < Right_Children)		
		{
			struct node *Temp;
			Temp = Heap[New_First_Node];
			Heap[New_First_Node] = Heap[Left_Children];
			Heap[Left_Children] = Temp;

			New_First_Node = Left_Children;
			Left_Children = 2 * New_First_Node;
			Right_Children = 2 * New_First_Node + 1;
		}
		else if (Last_Idx_Heap >= Right_Children)	
		{
			int Smaller;

			if (Heap[Left_Children]->freq <= Heap[Right_Children]->freq)
			{
				Smaller = Left_Children;
			}
			else
			{
				Smaller = Right_Children;
			}

			struct node *Temp;
			Temp = Heap[New_First_Node];
			Heap[New_First_Node] = Heap[Smaller];
			Heap[Smaller] = Temp;

			New_First_Node = Smaller;
			Left_Children = 2 * New_First_Node;
			Right_Children = 2 * New_First_Node + 1;

		}
	}
	return Output_Node;
}







struct node* Make_tree()
{
	struct node *first = 0;
	struct node *second = 0;

	while (1)
	{
		first = Delete_From_Heap();
		second = Delete_From_Heap();


		if (second == 0)
		{
			return first;
		}

		struct node* Sum_Node = (struct node*)malloc(sizeof(struct node));
		Sum_Node->Character = '+';
		Sum_Node->freq = first->freq + second->freq;
		Sum_Node->next_L = first;
		Sum_Node->next_R = second;

		add_To_Heap(Sum_Node);

	}

}

void traverseCoding(struct node *me, char _code)
{
	Code[++Code_Last_Idx] = _code;
	if (me->next_L == 0 && me->next_R == 0)
	{

		strcpy(&(Save_Code[(unsigned char)me->Character][0]), Code);


		Code[Code_Last_Idx--] = 0;
		return;
	}
	traverseCoding(me->next_L, '0');
	traverseCoding(me->next_R, '1');
	Code[Code_Last_Idx--] = 0;
}

void Find_Huff_Code(struct node* Tree_Root)
{
	traverseCoding(Tree_Root->next_L, '0');
	traverseCoding(Tree_Root->next_R, '1');
}

void Show_Huff_Code()
{
	printf("\n허프만 코드 테이블로 출력\n>>\n");
	int i = 0;
	while (i < ASCII)
	{
		if (Save_Code[i][0] != 0)
		{
			printf("%c : %s\n", i, &(Save_Code[i][0]));
		}
		i++;
	}
}

int Encoding(unsigned char* Input, unsigned char *After_Encoding)
{
	int Input_idx = 0;
	int Input_length = 0;
	unsigned char Input_buff = 0;
	int Code_idx = 0;
	int Code_length = 0;

	int After_Encoding_idx = 0;

	int Shift = 0;

	int After_Encoding_bit_length = 0;

	int Code_8bit_cnt = 0;


	Input_length = strlen((char*)Input) - 1;


	while (Input_length > 0)
	{
		Code_length = strlen(&(Save_Code[(int)Input[Input_idx]][0]));

		while (1)
		{
			if (Code_length == 0)
			{
				Input_idx++;
				Code_idx = 0;
				break;
			}
			else
			{
				if (Save_Code[(int)Input[Input_idx]][Code_idx] == '1')
				{
					if (Code_8bit_cnt == 8)
					{
						Code_8bit_cnt = 0;
						After_Encoding_idx++;
						continue;
					}
					else
					{
						Input_buff = 0x01 << (7 - Shift);
						After_Encoding[After_Encoding_idx] |= Input_buff;

						Shift = (Shift + 1) % 8;
						Code_length--;
						Code_idx++;
						Code_8bit_cnt++;
						After_Encoding_bit_length++;
					}
				}
				else if (Save_Code[(int)Input[Input_idx]][Code_idx] == '0')
				{
					if (Code_8bit_cnt == 8)
					{
						Code_8bit_cnt = 0;
						After_Encoding_idx++;
						Shift = 0;
						continue;

					}
					else
					{
						Input_buff = 0x00 << (7 - Shift);
						After_Encoding[After_Encoding_idx] |= Input_buff;

						Shift = (Shift + 1) % 8;
						Code_length--;
						Code_idx++;
						Code_8bit_cnt++;
						After_Encoding_bit_length++;
					}
				}
			}
		}

		Input_length--;
	}

	return After_Encoding_bit_length;
}

void Print_bin_After_Encoding(unsigned char* After_Encoding, int Used_Bit_Count)
{
	FILE *pFile = fopen("After_Eecoding.bin", "wb");

	int will_use_byte = 0;

	if (Used_Bit_Count % 8 == 0)
	{
		will_use_byte = Used_Bit_Count / 8;
	}
	else
	{
		will_use_byte = (Used_Bit_Count / 8) + 1;
	}

	fwrite(After_Encoding, will_use_byte + 1, 1, pFile);
	fclose(pFile);
}

void Read_bin_File(unsigned char* Before_Decoding)
{
	FILE *pFile = fopen("After_Eecoding.bin", "rb");

	int i = 0;

	while (feof(pFile) == 0)
	{
		Before_Decoding[i] = fgetc(pFile);
		i++;
	}
	fclose(pFile);
}










int main()
{
	unsigned char Input[L] = {};
	Scan_File(Input);

	int Freq[ASCII] = {};
	Check_Freq(Input, Freq);
	Show_Freq(Freq);
	Make_Heap(Freq);

	Make_Min_Heap(Freq);


	struct node *Tree_Root = Make_tree();


	Find_Huff_Code(Tree_Root);


	Show_Huff_Code();


	unsigned char After_Encoding[L] = {};
	unsigned char Before_Decoding[L] = {};
	int Used_Bit_Count = 0;
	Used_Bit_Count = Encoding(Input, After_Encoding);

	Print_bin_After_Encoding(After_Encoding, Used_Bit_Count);

}
