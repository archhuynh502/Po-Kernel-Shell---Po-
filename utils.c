#include "utils.h"

void itoa(int value, char* str, int base){
    char buf[32];
    int i=0, j, sign=0;

    if(value==0){ str[0]='0'; str[1]=0; return; }

    if(value<0 && base==10){
        sign=1;
        value=-value;
    }

    while(value!=0){
        int rem = value % base;
        buf[i++] = (rem>9)? (rem-10)+'A' : rem+'0';
        value/=base;
    }

    if(sign) buf[i++]='-';

    buf[i]=0;

    // reverse string
    for(j=0;j<i;j++) str[j]=buf[i-j-1];
    str[i]=0;
}

int strlen(const char* str){
    int len=0;
    while(str[len]) len++;
    return len;
}

void strcpy(char* dest, const char* src){
    while(*src) *dest++ = *src++;
    *dest=0;
}

