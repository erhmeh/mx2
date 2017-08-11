#include <stdio.h>
#include <string.h>

int main()
{
	char first[] = "I would like to go ";
	char second[] = "from here to there\n";
	char storage[64];

	strcpy(storage,first);
	strcat(storage,second);
	printf("Here is your stringL\n%s",storage);


	return(0);
}
