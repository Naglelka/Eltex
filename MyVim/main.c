#include <stdio.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <curses.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

typedef struct Coordinate
{
    int x;
    int y;
} Coordinate;

void sig_winch(int signo)
{
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
    resizeterm(size.ws_row, size.ws_col);
}

void readFile(int file)
{
    int position = lseek(file, 0, SEEK_END);
    char buffer[position];
    lseek(file, 0, SEEK_SET);

    if(read(file, buffer, sizeof(buffer)) < 0)
    {
        perror("Move error");
    }

    for(int i = 0; i < position; i++)
    {
        printw("%c", buffer[i]);
    }
    
    close(file);
}

void openFile(char *NameFile)
{
    int file;
    
    move(0, 0);
    clear();

    file = open(NameFile, O_RDWR);
    if(file < 0)
    {
        perror("Open failed on input file");
    }

    readFile(file);
}

void saveFile(char *nameFile, int size_win)
{
    FILE *file = fopen(nameFile, "w");

    for(int i = 0; i < size_win; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
            putc(mvinch(i, j) & A_CHARTEXT, file);
        }
    }
    move(0, 0);
    fclose(file);
    clear();
}

void action(WINDOW *wnd)
{
    int press_key = wgetch(wnd);
    char nameFile[25];
    Coordinate cur;
    cur.y = 0;
    cur.x = 0;

    getyx(wnd, cur.y, cur.x);
    if(press_key == ERR) 
    {
        printf("Error");
        return;
    }

     switch(press_key)
     {
        case KEY_BACKSPACE:
            getyx(wnd, cur.y, cur.x);
            mvwaddch(wnd, cur.y, cur.x, ' ');
            refresh();
            move(cur.y, cur.x);
            break;

        case KEY_UP:
            move(cur.y - 1, cur.x);
            break;

        case KEY_DOWN:
            move(cur.y + 1, cur.x);
            break;

        case KEY_LEFT:
            move(cur.y, cur.x - 1);
            break;

        case KEY_RIGHT:
            move(cur.y, cur.x + 1);
            break;

        case '\n':
            move(cur.y + 1, 0);
            break;

        case KEY_F(1):
            printw("Enter file name:");
            wgetnstr(wnd, nameFile, 25);
            clear();
            openFile(nameFile);
            break;

        case KEY_F(2): 
            int size_win_y = getmaxy(wnd);
            move(size_win_y - 1, 0);
            printw("Enter file name:");
            wgetnstr(wnd, nameFile, 25);
            saveFile(nameFile, size_win_y - 1);
            break;
        
        case KEY_F(3): 
            goto finally;
     }
    refresh();
    return;
    
    finally:

    clear();
    refresh();
    resetty();
    endwin();
    exit(EXIT_SUCCESS);
}

int main(void)
{
    WINDOW *wnd;
    wnd = initscr();
    savetty();
    signal(SIGWINCH, sig_winch);
    curs_set(TRUE);
    keypad(wnd, TRUE);
    mousemask(BUTTON1_CLICKED, NULL);
    while(1)
    {
        action(wnd);
        refresh();
    }
    curs_set(TRUE);
    clear();
    refresh();
    resetty();
    endwin();
    exit(0);
    return 0;
}