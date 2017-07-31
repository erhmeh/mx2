#include <stdio.h>
#include <math.h>

int main()
{
  long long n;
  printf("Enter number: ");
  scanf("%lld", &n);
  printf("%d", myFunction(n));
  return 0;
}

int myFunction(long long n)
{
  int dn = 0, i = 0, remainder;
  while (n!=0)
  {
      remainder = n%10;
      n /= 10;
      dn += remainder*pow(2,i);
      ++i;
    }
    return dn;
}
