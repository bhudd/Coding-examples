#include <stdio.h>
#include <string.h>

void stringcopy(char* dest, char* src)
{
    memcpy(dest, src, strlen(src)*sizeof(char));
}

void stringcopy2(char* dest, char* src)
{
    for(int i = 0; i < strlen(src); ++i)
    {
        dest[i] = src[i];
    }
}

void stringcopy3(char* dest, char* src)
{
    while((*dest++ = *src++) != '\0');
}

int main(void)
{
    char str1[] = "what the fuck.";
    char str2[40];
    char str3[40];
    char str4[40];
    stringcopy(str2, str1);
    stringcopy2(str3, str1);
    stringcopy3(str4, str1);
    printf("str1: %s str2: %s\n", str1, str2);
    printf("str3: %s str4: %s\n", str3, str4);
}