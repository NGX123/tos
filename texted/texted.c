/* EXTRA INFO
RAW MODE FLAGS
It dispables ECHO mode so characters are not displayed
It disables ICANON so we don't press enter but(it reads bytes not line)w
It disables ISIG so ctrl-c and ctrl-z won't work
It disables IXON so ctrl-s and ctrl-q won't work
It disables IEXTEN so ctrl-c and ctrl-v won't work
It disables ICRNL so ctrl-m won't work
It disables OPOST so output won't be processed
It disables alot of other flags most of which are not even on in modern terminals 

SETTINGS
VIMIN is set to 0 - minimum amout of bytes needed before read can return(0 - soon as posible)
VTIME is set to 1 - maximum amount of time to wait for read, if times out returns value of c 

\x1b - 1 bytes, escape character, [2J - 3 bytes, J - clear the screen, 2 - argument 
\x1b[H - moves cursor to the start of the screen

*/
/*** includes ***/
#include <unistd.h> //Used for taking input from devices
#include <termios.h>  //Used to manipulate terminal 
#include <stdlib.h> //used for some standart functions(e.g. atexit)
#include <ctype.h> //Used for iscntrl
#include <stdio.h> //standard i/o library
#include <errno.h> //used for error handling
#include <sys/ioctl.h> //terminal input output control
#include <string.h> //extended strings functionality




/*** defines ***/
//ctrl+char values are bytes fro 1-26, we define a macro to make control-chars
#define CTRL_KEY(k) ((k) & 0x1f)




/*** data ***/
//Stroing configuration
struct editorConfig {
    int screenrows; //amount of rows
    int screencols; //amount of collums
    struct termios orig_termios; //original settings
};

//Structure variable
struct editorConfig E;




/*** terminal ***/
void die(const char *s) {
    //Used to write where it finished and the error

    //Escape codes to clear the screen and put cursor in the start
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);

    //Prints the error
    perror(s);
    exit(1);
}

void disableRawMode() {
    //Disables the raw mode for later use of terminal

    //tcsetattr - writes the original terminal config back into structure
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1)
    die("tcsetattr");
}

void enableRawMode() {
    //Turns on Raw mode by turning off flags and settings

    //Terminal config is read into the structure
    if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);

    //A structure for changing is cloned from terminal config
    struct termios raw = E.orig_termios;

    //Flags are turned off and chnaged for recieving only raw input
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    //Writes the changed structure to terminal config
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

char editorReadKey() {
    //Records a key press

    int nread;
    char c;

    //Read - read(standart_input, file, bytes), reads one byte from stdin 
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if (nread == -1 && errno != EAGAIN) die("read");
    }

    return c;
}

int getCursorPosition(int *rows, int *cols) {
    char buf[32];
    unsigned int i = 0;

    if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return -1;

    while (i < sizeof(buf) - 1) {
        if (read(STDIN_FILENO, &buf[i], 1) != 1) break;
        if (buf[i] == 'R') break;
        i++;
    }

    buf[i] = '\0';

    //We check if there is the escape sequnce and command in buffer
    if (buf[0] != '\x1b' || buf[1] != '[') return -1;

    //We get the two numbers seperated by ; and put them into rows and cols
    if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) return -1;

    return 0;
}

int getWindowSize(int *rows, int *cols) {
    //Gets the size of the window in rows and columns

    //Structure for storing window details
    struct winsize ws;

    //ioctl with TIOCGWINSZ gives the size of  the output window into strcucture
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return -1;
        return getCursorPosition(rows, cols);
    } 
    else {
        //Saves the value of structure in pointers
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    }
}




/*** append buffer ***/
struct abuf{
    char *b; //adress in memmory
    int len; //length
};

#define ABUF_INIT {NULL, 0}

void abAppend(struct abuf *ab, const char *s, int len){
    char *new = realloc(ab->b, ab->len + len);

    if (new == NULL) return;
    memcpy(&new[ab->len], s, len);
    ab->b = new;
    ab->len += len;
}

void abFree(struct abuf *ab){
    free(ab->b);
}




/*** output ***/
void editorDrawRows() {
    //Prints ~ to fill all the lines that are not in use on the screen

    int y;
    for (y = 0; y < E.screenrows; y++) {
        write(STDOUT_FILENO, "~", 1);

        //As a separate statement so ~ is printed on the last line
        if (y < E.screenrows - 1){
            write(STDOUT_FILENO, "\r\n", 2);
        }
    }
}

void editorRefreshScreen() {
    //Cleans the screan and moves the cursor to the start

    //Write - (destination, bytes combination, bytes), writes an escape sequence to the, there are different escape sequences
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);

    //Draws the lines
    editorDrawRows();

    //Clears screan
    write(STDOUT_FILENO, "\x1b[H", 3);
}




/*** input ***/
void editorProcessKeypress() {
    //Uses the editorReadKey to record the keypress and checks for shortcuts

    //Records Keypress
    char c = editorReadKey();

    //Shortcut combinations
    switch (c) {
    case CTRL_KEY('z'):
        write(STDOUT_FILENO, "\x1b[2J", 4);
        write(STDOUT_FILENO, "\x1b[H", 3);
        exit(0);
        break;
    }
}




/*** init ***/
void initEditor() {
    //Gets the windows size
    if (getWindowSize(&E.screenrows, &E.screencols) == -1) die("getWindowSize");
}

int main() {
    enableRawMode();
    initEditor();

    while (1) {
        editorRefreshScreen();
        editorProcessKeypress();
    }

    return 0;
}