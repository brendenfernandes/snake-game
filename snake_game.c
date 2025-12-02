#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main()
{
    int snake_x[500];   //array for snake position x-axis
    int snake_y[500];   //array for snake position y-axis
    int snake_length = 3;   //starting snake length
    int score = 0;  //holds score
    int ch; //holds the key input
    int max_x;  //max screen size on the x-axis
    int max_y;  //max screen size on the y-axis
    int direction = 3;  //this starts the snake moving towards the right
    int food_x; //food position on the x-axis
    int food_y; //food position on the x-axis
    int food_exists = 0;    //if the food currently exits
    int base_speed = 120000;
    int paused = 0;     //pause flag

    srand(time(NULL));  //seeds random nummbers

    initscr();  //starts ncurses
    noecho();   //stops echoing keys
    cbreak();   //instantaneous key input
    keypad(stdscr, TRUE);   //enables special keys
    curs_set(0);    //hides the cursor

    //setup for colors
    if(has_colors())
    {
        start_color();  //starts color mode
        use_default_colors();   //allows the transparent backgrounds

        init_pair(1, COLOR_GREEN, -1);  //snake color
        init_pair(2, COLOR_YELLOW, -1); //food color
        init_pair(3, COLOR_RED, -1);    //border color
    }

    getmaxyx(stdscr, max_y, max_x); //gets the size of the terminal

    //ensures minimum terminal window size is 20 x 20
    if(max_y < 20 || max_x < 20)
    {
        endwin();   //exit ncurses
        printf("ERROR: Terminal window must be at least 20x20.\n"); //print error message
        return 1;   //exit program
    }

    //win condition (half the border's size)
    int win_length = (2 * (max_x + max_y)) / 2;

    int quit_game = 0;  //main loop flag

    //oouter loop, restarting game
    while(!quit_game)
    {
        clear();    //clears screen

        //title screen
        move(max_y/2 - 6, (max_x/2) - 19); addstr(" ####   #    #    ##    #   #  ######");
        move(max_y/2 - 5, (max_x/2) - 19); addstr("#       ##   #   #  #   #  #   #     ");
        move(max_y/2 - 4, (max_x/2) - 19); addstr("#       # #  #  #    #  # #    #     ");
        move(max_y/2 - 3, (max_x/2) - 19); addstr(" ###    #  # #  ######  #      ##### ");
        move(max_y/2 - 2, (max_x/2) - 19); addstr("    #   #   ##  #    #  # #    #     ");
        move(max_y/2 - 1, (max_x/2) - 19); addstr("     #  #    #  #    #  #  #   #     ");
        move(max_y/2 + 0, (max_x/2) - 19); addstr("####    #    #  #    #  #   #  ######");

        //press any key input to start game
        move(max_y/2 + 2, (max_x/2) - 10);
        addstr("Press any key to start");

        refresh();  //updates display
        getch();    //waits for user to enter key to start gam
        clear();    //clear screen for gameplay

        //starts snake in center
        snake_x[0] = max_x / 2;
        snake_y[0] = max_y / 2;

        //positions the body of the snake behind the head
        for(int i = 1; i < snake_length; i++) 
        {
            snake_x[i] = snake_x[0] - i;    //places boy to the left of head
            snake_y[i] = snake_y[0];    //same row
        }

        int game_over = 0;  //game loop flag

        //loop for the game
        while(!game_over)
        {
            clear();    //clear screen for redraw

            //draws the borders for the top and bottom of the play area
            for (int i = 0; i < max_x; i++) 
            {
                attron(COLOR_PAIR(3));  //red border
                move(0, i); 
                addch('#'); //top border
                move(max_y - 1, i); 
                addch('#'); //bottom border
                attroff(COLOR_PAIR(3)); //end the red color
            }

            //draws the borders for the left and right sides of the play area
            for (int i = 0; i < max_y; i++) 
            {
                attron(COLOR_PAIR(3));  //red border
                move(i, 0); 
                addch('#'); //left border
                move(i, max_x - 1); 
                addch('#'); //right border
                attroff(COLOR_PAIR(3)); //end the red color
            }

            //draws instructions at top border
            const char* instructions = "Snake Game (WASD to move, Q to quit, P to pause)";
            move(0, (max_x - strlen(instructions)) / 2);
            addstr(instructions);   //add text

            //draw score at bottom border
            char score_text[32];
            sprintf(score_text, "Score: %d", score);
            move(max_y - 1, (max_x - strlen(score_text)) / 2);
            addstr(score_text); //writes out score

            //spawns food
            if (!food_exists) 
            {
                food_x = (rand() % (max_x - 2)) + 1;    //random x placement
                food_y = (rand() % (max_y - 2)) + 1;    //random y placement
                food_exists = 1;    //food is active
            }

            //draws food
            attron(COLOR_PAIR(2));  //start yellow color
            move(food_y, food_x);
            addch('*'); //food character
            attroff(COLOR_PAIR(2)); //end color yellow

            //draws the snake
            for (int i = 0; i < snake_length; i++) 
            {
                move(snake_y[i], snake_x[i]);   //moves cursor to body

                attron(COLOR_PAIR(1));  //green color start

                if(i==0)
                {
                    addch('@'); //adds head
                }
                else
                {
                    addch('O');    //adds body
                }
                attroff(COLOR_PAIR(1)); //ends green color
            }

            refresh();  //updates screen

            //player's input
            nodelay(stdscr, TRUE);  //non-blocking input
            ch = getch();   //read input

            //quit game
            if (ch == 'q' || ch == 'Q') 
            { 
                quit_game = 1; 
                break; 
            }

            //pause game
            if (ch == 'p' || ch == 'P')
            {
                paused = 1;
            }

            //pause screen
            while(paused)
            {
                const char* paused_msg = "PAUSED - Press P to Resume";
                move(max_y/2, (max_x - strlen(paused_msg)) / 2);
                addstr(paused_msg); //display pause message
                refresh();  //update

                int key = getch();  //check input

                if(key == 'p' || key == 'P')
                {
                    paused = 0; //unpause
                    move(max_y/2, (max_x - strlen(paused_msg)) / 2);
                    addstr("                           ");  //erases the paused text
                    refresh();
                }
            }

            if (ch == 'w' && direction != 1)
            {
                direction = 0;  //moves up
            }
            
            if (ch == 's' && direction != 0)
            {
                direction = 1;  //moves down
            } 

            if (ch == 'a' && direction != 3)
            {
                direction = 2;  //moves left
            }

            if (ch == 'd' && direction != 2)
            {
                direction = 3;  //moves right
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
                snake_y[0]--;   //moves up
            }

            if (direction == 1)
            {
                snake_y[0]++;   //moves down
            }

            if (direction == 2) 
            {
                snake_x[0]--;   //moves left
            }

            if (direction == 3) 
            {
                snake_x[0]++;   //moves right
            }

            //handles collision with walls
            if (snake_x[0] <= 0 || snake_x[0] >= max_x - 1 || snake_y[0] <= 0 || snake_y[0] >= max_y - 1)
            {
                game_over = 1;  //end game
            }

            //handles collision with itself
            for (int i = 1; i < snake_length; i++) 
            {
                if (snake_x[0] == snake_x[i] && snake_y[0] == snake_y[i]) 
                {
                    game_over = 1;  //end game
                    break;
                }
            }

            //win condition check
            if(snake_length >= win_length)
            {
                nodelay(stdscr, FALSE);
                clear();    //clear screen
                mvprintw(max_y/2, (max_x/2)-4, "YOU WIN!");
                mvprintw(max_y/2 + 2, (max_x/2)-11, "Press any key to exit.");
                refresh();
                getch();    //waits
                quit_game = 1;  //exit game fully
                break;
            }

            //handles eating food
            if (snake_x[0] == food_x && snake_y[0] == food_y) 
            {
                snake_length++; //grows snake
                score++;    //increment score
                food_exists = 0;    //respawns food
            }

            usleep(120000); //speed control
        }

        if(quit_game) 
        {
            break;
        }

        //game over screen
        clear();
        mvprintw(max_y/2 - 1, (max_x/2) - 5, "GAME OVER");
        mvprintw(max_y/2 + 1, (max_x/2) - 12, "Press R to restart");
        mvprintw(max_y/2 + 2, (max_x/2) - 11, "Press Q to quit game");
        refresh();

        nodelay(stdscr, FALSE);

        //restart/quit loop
        while(1)
        {
            int choice = getch();   //awaits input

            if(choice == 'q' || choice == 'Q') 
            { 
                quit_game = 1;  //exits outer loop
                break; 
            }

            if(choice == 'r' || choice == 'R') 
            {
                snake_length = 3;   //resets snake
                score = 0;  //resets score
                direction = 3;  //resets direction
                food_exists = 0;    //clears food
                break;  //restarts game
            }
        }
    }

    endwin();   //closes ncurses
    return 0;   //exits program
}
