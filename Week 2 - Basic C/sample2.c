#include <stdio.h>

int main(){
  printf("Enter a sentence: ");
  myFunction();

  return 0;
}

void myFunction(){
  char c;
  scanf("%c",&c);

  if(c !='\n')
  {
    myFunction();
    printf("%c",c);
  }
}
