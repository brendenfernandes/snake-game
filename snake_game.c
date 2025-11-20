#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int main()
{
    int x = 10; //starting positone of the snake head on x
    int y = 10; //starting positone of the snake head on y
    int ch; //stores the keyboard input
    int max_x;  //max terminal width
    int max_y;  //max terminal height
    int food_x; //food position x axis
    int food_y; //food position y axis
    int food_exists = 0;    //if food is on screen
    
    srand(time(NULL));

    initscr();  //initialized the screen, starts ncurses
    noecho();   //stops typed characters from being shown on screen
    cbreak();   //gets input
    keypad(stdscr, TRUE);   //allows special keys

    getmaxyx(stdscr, max_y, max_x); //gets the height and width of the window

    //while loop for the main game
    while(1)
    {
        clear();    //fresh state of the game

        //draws the top and bottom border frames of play area
        for(int i = 0; i<max_x; i++)
        {
            move(0,i);
            addch('#');

            move(max_y - 1,i);
            addch('#');
        }

        //draws the left and right border frames of play area
        for(int i = 0; i<max_y; i++)
        {
            move(i,0);
            addch('#');

            move(i, max_x -1);
            addch('#');
        }

        move(0,2);  //cursor starts at the top left of the frame
        addstr("Snake Game (WASD to move, q to quit)"); //displays game name and instructions


        //spwans food into within play field if food is not existing
        if(!food_exists)
        {
            food_x = (rand() % (max_x - 2)) + 1;
            food_y = (rand() % (max_y - 2)) + 1;
            food_exists = 1;
        }

        move(food_y, food_x);   //cursor moves to place food
        addch('*'); //draws the food

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

        //keeps snake within the borders
        if(x<=1)
        {
            x=1;
        }
        if(x>=max_x -2)
        {
            x=max_x -2;
        }
        if(y<=1)
        {
            y=1;
        }
        if(y>=max_y -2)
        {
            y=max_y -2;
        }

        //checks if snake has eaten food
        if(x == food_x && y == food_y)
        {
            food_exists = 0;    //causes food to spawn in next loop
        }

        
    }
    
    endwin();   //ends the ncurses and returns to normal terminal
    return 0;   //returns 0 and ends program
}