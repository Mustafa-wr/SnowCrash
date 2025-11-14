#include <stdio.h>

int main() 
{
    char c;
    int pos = 0;
    
    while ((c = getchar()) != EOF) 
    {
        if (c == '\n') break;
        putchar(c - pos); 
        pos++;
    }
    putchar('\n');
    return 0;
}