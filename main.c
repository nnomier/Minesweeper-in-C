#include <stdio.h>
#include <stdlib.h>
#define SIZE 9
#define BOMBS 10

typedef enum { false, true } bool;

struct cells
{
    bool isOpened;
    bool isBomb;
    bool isFlagged;
    int neighbor_bomb_count;
};
typedef struct cells cell;
cell array[SIZE][SIZE];

void initialize_board()
{
    //initialize board to zeros
    int i,j;
    for(i=0; i<SIZE; i++)
    {
        for(j=0; j<SIZE; j++)
        {
            array[i][j].isBomb=0;
            array[i][j].isFlagged=0;
            array[i][j].isOpened=0;
            array[i][j].neighbor_bomb_count=0;
        }
    }
}

void fill_mines()
{
    int i,randRow,randColumn;
    for(i=0; i<=BOMBS; i++)
    {
        randRow=rand()%SIZE;
        randColumn=rand()%SIZE;
        array[randRow][randColumn].isBomb=1;
    }
}

void count_mines()
{
    int row,column,i,j;
    for(row=0; row<SIZE; row++)
    {
        for(column=0; column<SIZE; column++)
        {
            for(i=-1; i<=1; i++)
            {
                for(j=-1; j<=1; j++)
                {
                    if(i==0&&j==0 )
                        continue;
                    if(row+i>=0 && row+i<SIZE
                            && column+j>=0 && column+j<SIZE
                            && array[row+i][column+j].isBomb==1)
                        array[row][column].neighbor_bomb_count++;
                }
            }
        }
    }
}

void construct_game()
{
    initialize_board();
    fill_mines();
    count_mines();
}


//Print board to screen
void print_board()
{
    int i,j;
    for(j=0; j<SIZE; j++)
        printf("%d \t",  j);
    printf("\n");
    for(j=0; j<SIZE; j++)
        printf("---------");
    printf("\n\n");

    for(i=0; i<SIZE; i++)
    {
        for(j=0; j<SIZE; j++)
        {
            if(array[i][j].isBomb==1&&array[i][j].isOpened==1)
                printf("*");
            else if (array[i][j].isFlagged==1)
                printf("f");
            else if (array[i][j].isOpened==1&&array[i][j].isBomb==0)
                printf("%d",array[i][j].neighbor_bomb_count);
            else
                printf("-");

            printf("\t");
        }
        printf("\t %d ", i);

        printf("\n\n");
    }
}

void print_initial_board()
{
    int i,j;
    for(j=0; j<SIZE; j++)
        printf("%d \t",  j);
    printf("\n");
    for(j=0; j<SIZE; j++)
        printf("---------");

    printf("\n\n");

    for(i=0; i<SIZE; i++)
    {
        for(j=0; j<SIZE; j++)
        {
            if(array[i][j].isBomb==1&&array[i][j].isFlagged==0)
                printf("*");
            else if(array[i][j].isFlagged==1)
                printf("f");
            else
                printf("%d",array[i][j].neighbor_bomb_count);
            printf("\t");
        }

        printf("\t %d ", i);
        printf("\n");
        printf("\n");
    }
}

bool check_lose(int i,int j)
{
    bool isLose;
    if(array[i][j].isBomb==1)
        isLose=1;
    else
        isLose=0;
    return isLose;
}

void open_cell(int i,int j)
 {
    int k,l;
    if (!array[i][j].isOpened)
    {
        array[i][j].isOpened=1;
    }
    else
    {
        return;
    }

    if (array[i][j].neighbor_bomb_count==0)
    {
        for(k=-1; k<=1; k++)
        {
            for(l=-1; l<=1; l++)
            {
                if(k==0&&l==0 )
                    continue;
                if(i+k>=0 && i+k<SIZE && j+l>=0 && j+l<SIZE)
                    open_cell(i+k, j+l);
            }
        }
    }
}


bool checkWin()
{
    int counter=0,i,j;
    bool isWin,allOpened;

    for(i=0; i<SIZE; i++)
    {
        for(j=0; j<SIZE; j++)
        {
            if(array[i][j].isBomb==1&&array[i][j].isFlagged==1)
                counter++;
        }
    }
    for(i=0;i<SIZE;i++)
    {
        for(j=0;j<SIZE;j++)
        {
            if(array[i][j].isOpened==0&array[i][j].isFlagged==0)
                allOpened=0;

        }
    }

    if(counter==10&&allOpened==1)
        isWin=1;
    else
        isWin=0;

    return isWin;
}

bool unflag(int i,int j)
{
    bool proceed;
    if(array[i][j].isFlagged==1)
    {
        proceed=1;
    }
    else
        proceed=0;
    return proceed;
}

int main()
{
    construct_game();
    print_board();
    int row,column,option;
    bool isLose,isWin,proceed;
    while(1)
    {
        printf("Enter cell :\n");
        scanf("%d%d",&row,&column);
        printf("Enter option : \n");
        printf("[1] Open\n[2] Flag\n[3] Unflag\n");
        scanf("%d",&option);
        switch(option)
        {
        case(1):

            isLose=check_lose(row,column);
            isWin=checkWin();
            if(isLose==1)
            {
                printf("YOU LOST \n");
                print_initial_board();
                exit(1);
            }
            else if(isWin==1)
            {
                printf("\nYOU WON\n");
                exit(1);
            }
            else
            {
                open_cell(row,column);
                print_board();
            }
            break;

        case(2):
            array[row][column].isFlagged=1;
            isWin=checkWin();
            if(isWin==1)
            {
                printf("\nYOU WON\n");
                exit(1);
            }
            else
                print_board();
            break;

        case(3):
            proceed=unflag(row,column);
            if(proceed==1)
                {
                    array[row][column].isFlagged=0;
                    print_board();
                }
            if(proceed==0)
                {
                    printf("This cell wasn't flagged, try another one\n");
                    print_board();
                }
            break;
        }
    }
    return 0;
}
