#include <stdio.h>

void evaluate(int x);

int main()
{
	int i;

	printf("Type an integer value: ");
	scanf("%d",&i);
	evaluate(i);

	return(0);
}

void evaluate(int x)
{
	if(x > 10){
		printf("X is greater than 10");
	}
	if(x < 10){
		printf("X is less than 10");
	}
	if(x == 10){
		printf("X is equal to 10");
	}
	return;
}
