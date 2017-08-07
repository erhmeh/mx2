#include <stdio.h>
int PrintData(int number)

{

    printf("%d", number);

    return number + 1;

}

int main()

{

    for(int x = 0; x<3; x++)

    {

        PrintData(x);

    }

    return(0);

}
