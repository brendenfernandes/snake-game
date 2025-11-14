#include <ncurses.h>
#include <unistd.h>

int main()
{
    int x = 10; //starting positone of the snake head on x
    int y = 10; //starting positone of the snake head on y
    int ch; //stores the keyboard input
    

    initscr();  //initialized the screen, starts ncurses
    noecho();   //stops typed characters from being shown on screen

    //while loop for the main game
    while(1)
    {
        clear();    //fresh state of the game

        move(0,0);  //cursor starts at the top left of the frame
        addstr("Snake Game (WASD to move, q to quit)"); //displays game name and instructions

        move(y,x);  //cursor moves to place the head of the snake position
        addstr("@");    //draws the head of the snake

        refresh();  //update the terminal with new state

        ch = getch();   //waits for keyboard input

        //quits game if 'q' is pressed
        if(ch=='q')
        {
            break;
        }

        //moves snake up
        if(ch == 'w')
        {
            y--;
        }

        //moves snake left
        if(ch == 'a')
        {
            x--;
        }

        //moves snake down
        if(ch == 's')
        {
            y++;
        }

        //moves snake right
        if(ch == 'd')
        {
            x++;
        }

        endwin();   //ends the ncurses and returns to normal terminal
        return 0;   //returns 0 and ends program
    }
}