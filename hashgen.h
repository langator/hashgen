int arg_error(char *msg, char *msg2);
int opt_error(char *msg, char msg2);
int is_file(char arg[], int first, int one);
int small_help(void);
char * hashgen(unsigned char args[]);
char * fhashgen(FILE *stream);
int bufhash(char arg[]);
int test_arg(char arguments[]);


