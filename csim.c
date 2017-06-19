#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
	int valid;
	int tag;
	unsigned long long int address;
	int rankLRU;
} line;

typedef struct
{
	int s;
	int b;
	int E;
	int t;
	int S;
	int B;
	line** sets;
} cache;

int main(int argc, char **argv)
{

	cache* c = malloc(sizeof(cache));
	int h;
	int v;
	char* t;

	h = 0;
	v = 0;
	c -> s = -1;
	c -> E = -1;
	c -> b = -1;
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-h") == 0)
		{
			h = 1;
		}
		else if (strcmp(argv[i], "-v") == 0)
		{
			v = 1;
		}
		else if (strcmp(argv[i], "-s") == 0 && i < argc - 1)
		{
			c -> s = atoi((argv[i+1]));
			i++;
		}
		else if (strcmp(argv[i], "-E") == 0 && i < argc - 1)
		{
			c -> E = atoi((argv[i+1]));
			i++;
		}
		else if (strcmp(argv[i], "-b") == 0 && i < argc - 1)
		{
			c -> b = atoi((argv[i+1]));
			i++;
		}
		else if (strcmp(argv[i], "-t") == 0 && i < argc - 1)
		{
			t = (argv[i+1]);
			i++;
		}
	}
	if (h == 1)
	{
		printf("Usage: ./csim-ref [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n");
		exit(0);
	}
	c -> B = 1 << c -> b;
	c -> S = 1 << c -> s;
	c -> t = 64 - c -> b - c -> s;
	c -> sets = (line **) malloc(sizeof(line *) * c -> S);
	for (int i = 0; i < c -> S ; i++)
	{
		c -> sets[i] = (line *)malloc(sizeof(line) * c -> E);
	}

	FILE *f = fopen(t, "r");

	char *instruction = malloc(sizeof(char));
	unsigned long int *address =  malloc(sizeof(long long int));
	int *size = malloc(sizeof(int));
	int misses = 0;
	int hits = 0;
	int evictions = 0;
	while(fscanf(f, "%c %lx,%i", instruction, address, size) > 0){
		if(*instruction == 'L' || *instruction == 'M' || *instruction == 'S'){
			int tag = (*address >> c -> b) >> c -> s;
			int set = ((*address << c -> t) >> c -> t) >> c -> b;
			int lineNum = -1;
			int replacementType = -1;
			int hit = 0;
			int currentrankLRU = 0;
			if(v == 1)
			{
				printf("%c, %lx, %d \n", *instruction, *address, *size);
			}
			for (int i = 0; i < c -> E; i++)
			{
				line curLine = c -> sets[set][i];
				if (curLine.valid == 1 && curLine.tag == tag)
				{
					lineNum = i;
					replacementType = 1;
					hit = 1;
				}
				else if ((replacementType == -1 || replacementType == 2)&& curLine.valid == 1 && curLine.rankLRU > currentrankLRU)
				{
					lineNum = i;
					replacementType = 2;
					currentrankLRU = curLine.rankLRU;
				}
				else if (curLine.valid != 1 && replacementType != 1 && replacementType != 3)
				{
					lineNum = i;
					replacementType = 3;
				}
			}
			if (hit == 0)
				{
					misses++;
					if (replacementType == 2)
					{
						evictions++;
					}
					printf("\n");
					c ->sets[set][lineNum].valid = 1;
					c ->sets[set][lineNum].tag = tag;
					c ->sets[set][lineNum].rankLRU = 1;
					for (int i = 0; i < c -> E; i++)
					{
						if (c -> sets[set][i].tag != tag)
						{
							c -> sets[set][i].rankLRU++;
						}
					}
				}
				else
				{
					c ->sets[set][lineNum].rankLRU = 1;
					for (int i = 0; i < c -> E; i++)
					{
						if (c -> sets[set][i].tag != tag)
						{
							c -> sets[set][i].rankLRU++;
						}
					}
					hits++;
				}
				if(*instruction == 'M')
				{
					hits++;
				}
		}
		
	}
	fclose(f);
	for (int i = 0; i < c -> S ; i++)
	{
		free(c -> sets[i]);
	}
	free(c -> sets);
	free(c);
	
	printSummary(hits, misses, evictions);
}
