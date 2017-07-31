#include <stdio.h>

int main(){
  int a, b, result;
  printf("Enter number a: ");
  scanf("%d",&a);

  printf("Enter number b: ");
  scanf("%d",&b);

  result = p(a,b);

  printf("For a = %d and b = %d the result is %d",a,b,result);
  return 0;
}

int p(int a, int b){
  if (b != 0){
    return (a*p(a,b-1));
  }
  else{
    return 1;
  }
}
