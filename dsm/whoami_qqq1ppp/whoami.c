/* 
    2017-08-29 Made By qqq1ppp
    h@ck wargame project
    category : Reversing

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUF_LEN 14

int init_buf(char* dest)
{
    char buf[] = {'M','V','\x1e','\x11','4','7','+','\x04','\x05','D',',','\x04',']','\0'};
    strncpy(dest, buf, BUF_LEN);
    return 0;
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        printf("usage : %s your_name\n", argv[0]);
        exit(0);
    }
    char buf[BUF_LEN];
    int i, j;
    char whoami[BUF_LEN];

    init_buf(whoami);

    printf("Are you an 4ws0me_h@cker?\n");

    strncpy(buf, argv[1], BUF_LEN);
    for(i=0; i<BUF_LEN-1; i++)
    {
        j=BUF_LEN-i-2;
	// printf("%c %c\n", buf[i], whoami[j]);
        buf[i] ^= whoami[j];
	// printf("%c\n", buf[i]);
    }

    // printf("You're an %s !\n", buf);
    if(strncmp(buf, "4ws0me_h@cker", BUF_LEN) != 0)
    {
        printf("You're not..\n");
    }else{
        printf("You are!\n");
    }
    
    return 0;
}