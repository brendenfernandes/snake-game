#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int snake_x[500];   //snake position x-axis
    int snake_y[500];   //snake position y-axis
    int snake_length = 3;   //starting snake length
    int ch; //holds the key input
    int max_x;  //max screen size on the x-axis
    int max_y;  //max screen size on the y-axis
    int direction = 3;  //this starts the snake moving towards the right
    int food_x; //food position on the x-axis
    int food_y; //food position on the x-axis
    int food_exists = 0;    //if the food currently exits

    srand(time(NULL));  //seeds random nummbers

    initscr();  //starts ncurses
    noecho();   //stops echoing keys
    cbreak();   //instantaneous key input
    keypad(stdscr, TRUE);   //enables special keys
    curs_set(0);    //hides the cursor

    getmaxyx(stdscr, max_y, max_x); //this gets the size of the terminal

    clear();

    //title screen
    move(max_y/2 - 6, (max_x/2) - 19); addstr(" ####   #    #    ##    #   #  ######");
    move(max_y/2 - 5, (max_x/2) - 19); addstr("#       ##   #   #  #   #  #   #     ");
    move(max_y/2 - 4, (max_x/2) - 19); addstr("#       # #  #  #    #  # #    #     ");
    move(max_y/2 - 3, (max_x/2) - 19); addstr(" ###    #  # #  ######  #      ##### ");
    move(max_y/2 - 2, (max_x/2) - 19); addstr("    #   #   ##  #    #  # #    #     ");
    move(max_y/2 - 1, (max_x/2) - 19); addstr("     #  #    #  #    #  #  #   #     ");
    move(max_y/2 + 0, (max_x/2) - 19); addstr("####    #    #  #    #  #   #  ######");

    move(max_y/2 + 2, (max_x/2) - 10);
    addstr("Press any key to start");

    refresh();
    getch();    //waits for user to enter key to start gam
    clear();


    //starts snake in center
    snake_x[0] = max_x / 2;
    snake_y[0] = max_y / 2;

    for(int i = 1; i < snake_length; i++) 
    {
        snake_x[i] = snake_x[0] - i;
        snake_y[i] = snake_y[0];
    }


    //loop for the game
    while (1)
    {
        clear();

        //draws the borders for the top and bottom of the play area
        for (int i = 0; i < max_x; i++) 
        {
            move(0, i); addch('#');
            move(max_y - 1, i); addch('#');
        }

        //draws the borders for the left and right sides of the play area
        for (int i = 0; i < max_y; i++) 
        {
            move(i, 0); addch('#');
            move(i, max_x - 1); addch('#');
        }

        //draws instructions
        move(0, 2);
        addstr("Snake Game (WASD to move, q to quit)");

        //spawns food
        if (!food_exists) 
        {
            food_x = (rand() % (max_x - 2)) + 1;
            food_y = (rand() % (max_y - 2)) + 1;
            food_exists = 1;
        }

        //draws food
        move(food_y, food_x);
        addch('*');

        //draws the snake
        for (int i = 0; i < snake_length; i++) 
        {
            move(snake_y[i], snake_x[i]);

            if(i==0)
            {
                addch('@'); //adds head
            }

            else
            {
                addch('O');;    //adds body
            }
            
        }

        refresh();  //updates screen

        //player's input
        nodelay(stdscr, TRUE);
        ch = getch();

        if (ch == 'q')
        {
            break;  //break statement to exit game loop
        }

        if (ch == 'w' && direction != 1)
        {
            direction = 0;  //sets direction to up
        }

        if (ch == 's' && direction != 0)
        {
            direction = 1;  //sets direction to down
        }
        
        if (ch == 'a' && direction != 3)
        {
            direction = 2;  //sets direction to left
        }

        if (ch == 'd' && direction != 2)
        {
            direction = 3;  //sets direction to right 
        }

        //moves body segments
        for (int i = snake_length - 1; i > 0; i--) 
        {
            snake_x[i] = snake_x[i - 1];    
            snake_y[i] = snake_y[i - 1];
        }

        //moves the snake's head
        if (direction == 0)
        {
            snake_y[0]--;   //moves snake's head up
        } 

        if (direction == 1)
        {
            snake_y[0]++;   //moves snake's head down
        }

        if (direction == 2)
        {
            snake_x[0]--;   //moves snake's head left
        }

        if (direction == 3)
        {
            snake_x[0]++;   //moves snake's head right
        }

        //handles collision with walls
        if (snake_x[0] <= 0 || snake_x[0] >= max_x - 1 || snake_y[0] <= 0 || snake_y[0] >= max_y - 1)
        {
            endwin();   //closes ncurses
            printf("GAME OVER\n");  //prints message
            return 0;   //exits
        }

        //handles collision with itself
        for (int i = 1; i < snake_length; i++) 
        {
            if (snake_x[0] == snake_x[i] && snake_y[0] == snake_y[i]) 
            {
                endwin();   //closes ncurses
                printf("GAME OVER\n");  //prints message
                return 0;   //exits
            }
        }

        //handles eating food
        if (snake_x[0] == food_x && snake_y[0] == food_y) 
        {
            snake_length++; //snake grows 
            food_exists = 0;    //respawns the food
        }

        usleep(120000); //speed control
    }

    endwin();
    return 0;
}
