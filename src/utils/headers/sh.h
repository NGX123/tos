// File: sh.h
// Description: includes defines for the sh.c



// Handles errors
void error_handler(int error_type);

// Concatentates strings
char* concat(const char *s1, const char *s2);

// Formats the user input
static void format_input(char* input);


// Exits the shell
int shell_exit(char **args);

// Changes drectory of the current shell
int shell_cd(char **args);

// Executes the binary
int shell_exec(char **args);


// Main loop of the shell
void shell_loop(void);

// Launches external programs
int shell_launch(char **args);