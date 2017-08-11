#include <stdio.h>
#include <time.h>

int main()
{
	time_t now;
	time(&now);
	printf("The current time is %ld\n",ctime(&now));

	return(0);
}
