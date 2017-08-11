#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 10
#define COLUMNS 10

int main()
{
	int row,column;

	srand((unsigned)time(NULL));

	for(row=0;row<ROWS;row++)
	{
		for(column=0;column<COLUMNS;column++)
			printf("%2d\t",rand() % 100);
		putchar('\n');
	}

	return(0);
}
