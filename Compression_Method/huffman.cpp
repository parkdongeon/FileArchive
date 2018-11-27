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


int main()
{
	unsigned char Input[L] = {};			
	Scan_File(Input);

	int Freq[ASCII] = {};		
	Check_Freq(Input, Freq);	
	Show_Freq(Freq);
}
