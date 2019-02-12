#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char s[] = " ___       ________  ________  ___  ________          ________  ________  ________  ___  ________   ________ \n" 
"|\\  \\     |\\   __  \\|\\   __  \\|\\  \\|\\   __  \\        |\\   ____\\|\\   __  \\|\\   ____\\|\\  \\|\\   ___  \\|\\   __  \\    \n"
"\\ \\  \\    \\ \\  \\|\\  \\ \\  \\|\\  \\ \\  \\ \\  \\|\\  \\       \\ \\  \\___|\\ \\  \\|\\  \\ \\  \\___|\\ \\  \\ \\  \\\\ \\  \\ \\  \\|\\  \\   \n"
" \\ \\  \\    \\ \\   __  \\ \\   ____\\ \\  \\ \\  \\\\\\  \\       \\ \\  \\    \\ \\   __  \\ \\_____  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\\\  \\  \n"
"  \\ \\  \\____\\ \\  \\ \\  \\ \\  \\___|\\ \\  \\ \\  \\\\\\  \\       \\ \\  \\____\\ \\  \\ \\  \\|____|\\  \\ \\  \\ \\  \\\\ \\  \\ \\  \\\\\\  \\ \n"
"   \\ \\_______\\ \\__\\ \\__\\ \\__\\    \\ \\__\\ \\_______\\       \\ \\_______\\ \\__\\ \\__\\____\\_\\  \\ \\__\\ \\__\\\\ \\__\\ \\_______\\ \n"
"    \\|_______|\\|__|\\|__|\\|__|     \\|__|\\|_______|        \\|_______|\\|__|\\|__|\\_________\\|__|\\|__| \\|__|\\|_______|\n"
"                                                                            \\|_________|                         \n";

typedef struct {
    char name[100];
    int money;
}Player;

int rsp(void)
{
    srand(time(0));
    int input;
    puts("input 0~2 (0:rock, 1:scissors, 2:paper)");
    scanf("%d", &input);
    if(input < 0 || input > 2){
        printf("invalid input!\n");
        return -1;
    }
    int a = rand()%10;
    return a;
}

int main(int argc, char** argv)
{
    puts(s);
    Player* p = (Player*)malloc(sizeof(Player));
    int bet;
    p->money = 100;
    puts("input name");
    scanf("%s", p->name);
    while(p->money < 1000){

        if(p->money <= 0){
            puts("you're out of money..T^T");
            return 0;
        }

        printf("input money to bet (you have %d) : ", p->money); // 판돈 거는 부분
        scanf("%d", &bet);
        if((bet < 0)||(bet > p->money)){
            puts("you cracker!");
            return 0;
        }
        int r = rsp();
        if(r < 2){
            printf("you get %d plus!\n", bet); // 이기거나 비겼을 때
            p->money += bet;
        }else{
            printf("you get %d minus..\n", bet); // 졌을때
            p->money -= bet;
        }
    }
    printf("wow! you won the game %s.. congrats!!\n", p->name);
    free(p);
    return 0;
}