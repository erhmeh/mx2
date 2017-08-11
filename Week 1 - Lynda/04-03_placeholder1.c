#include <stdio.h>

int main()
{
	float a;
    float b = 5.0;

	printf("Input a number: ");
	scanf("%f",&a);
	printf("%.1f / %.1f = %.1f\n",a,b,a/b);

	return(0);
}
