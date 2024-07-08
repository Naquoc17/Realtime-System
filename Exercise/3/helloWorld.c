#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Extract the arguments
    char *text = argv[1];
    int num1 = atoi(argv[2]); // Convert string to integer
    int num2 = atoi(argv[3]); // Convert string to integer

    int result = num1 * num2;

    printf("%s %d * %d = %d\n", text, num1, num2, result);
    return 0;
}
