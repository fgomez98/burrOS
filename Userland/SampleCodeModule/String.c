
#include "String.h"

int strcmp(char string1[], char string2[] ){
    for (int i = 0; ; i++)
    {
        if (string1[i] != string2[i])
        {
            return string1[i] < string2[i] ? -1 : 1;
        }

        if (string1[i] == '\0')
        {
            return 0;
        }
    }
}

int itos(char* str, int num){
    int length = numlen(num);
    int i;
    int numlength = length;

    if(num < 0){
        str[0] = '-';
        num = -num;
        numlength--;
        
    }
    for(i=numlength; i>0; i){
        str[i] = (num%10 + '0');
        num /= 10;
    }
    str[length] = '\0';
    return length;
}

int numlen(int num){
    int length = 1;

    if(num<0){
        length++;
        num = -num;
    }

    while(num/10 != 0){
        length++;
        num = num/10;

    }
    return length;
}

int strlen(const char* str){
    int length = 0;
    while(*(str++) != '\0'){
        length ++;
    }
    return length;
}
