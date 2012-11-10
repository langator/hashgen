/* main.c */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <mhash.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#define MAXSTR 1024
#define BUFFERSIZE  4096


int args_parser(char *arg[], int argc);
int small_help(void);
char * hashgen(unsigned char args[]);
char * hashgen_stdin(void);
char * fhashgen(FILE *stream);
int bufhash(char arg[]);

int main(int argc, char *argv[]) { 
int c, i;
FILE *fd;
char string[MAXSTR];
char a[MAXSTR];
struct stat st;

// arg parser
//args_parser(argv, argc);


// input from stdin
/*
 *   for create hash string without '\n' tap Ctrl-D 2 time 
 */
if ( argc == 1) {
	printf("%s\n", fhashgen(stdin));
	return 0;
}
if ( (argc == 2) && ((strcmp(argv[1],"-")) == 0) ) {
        printf("%s\n",fhashgen(stdin));
	return 0;
}

//add -f option support
if ( strcmp(argv[1],"-f") == 0 ) {
	for (i=2; i<argc; i++)
		if ( (fd = fopen(argv[i],"r")) ) {
                	printf("%s\n",fhashgen(fd));
			fclose(fd);
			}
		else {
			fprintf(stderr,"Error open file: %s\n", argv[i]);
			return 1;
		}
		
		return 0;
}

// read files by line and create hash
if ( strcmp(argv[1],"-l") == 0 ) {
	for (i=2; i<argc; i++)
		if ( (fd=fopen(argv[i],"r")) ) { 
			while(fgets(string,sizeof(string),fd)) {
				c=0;
				while (string[c]!='\n' && string[c]!='\0') {
					a[c]=string[c];
					c++;
					if (c==(MAXSTR-1) && string[c]!='\n')
						fprintf(stderr, "ERROR: line more than %d char\n",MAXSTR);
				}
			
			a[c]='\0';
			printf("%s\n",hashgen(a));
			}
		
		fclose(fd);
		}
                else {
                        fprintf(stderr,"Error open file: %s\n", argv[i]);
                        return 1;
                }

                return 0;
}

//test count args
if (argc != 1) {
// full help
	if ( ((strcmp(argv[1],"--help"))==0) || ((strcmp(argv[1],"-h"))==0) ) {
        	help();
		return 0;
	}
}

// create hash summ of strings
if ( ((strcmp(argv[1],"-s")) == 0) ) {
	if (argc == 2) 
		small_help();

	for (i=2; i<argc; i++) 
		printf("%s\n",hashgen(argv[i]));
	return 0;
	}

// create hash by line, use buffer in reading file
for (c=1; c<argc; c++)
	bufhash(argv[c]);
return 0;

}

/***************************
 *	functions 
 ***************************/

//*********************************
// *	BUG: function work 1.6 time slow then sha256sum
// ********************************

//buffered read and hashgen
int bufhash(char arg[]) {
	char buf[BUFFERSIZE], str[BUFFERSIZE];
	int n_char, in_fd, c, i=0;
	if ( (in_fd=open(arg, O_RDONLY)) == -1 ) {
		fprintf(stderr, "Cannot open file: %s\n", arg);
		return 1;
	}
	while ((n_char=read(in_fd, buf, BUFFERSIZE))>0){
		for(c=0; c<n_char; c++) {
			if (buf[c]!='\n') {
				str[i]=buf[c];
				i++; 
				}
			else {
				str[i]='\0';
				i++;
				
				if ( str[0] != '\0')  {
                                        printf("%s\n", hashgen(str));
                                        i=0;
                                        }
                                else
                                        i=0;

			}
		}
	}
	
	if (n_char == -1) {
		fprintf(stderr, "Cannot read %s\n", str);
		return 1;
	}

	close(in_fd);
	return 0;
}



//small help
int small_help(void) {
	printf("Usage: hashgen <file>\n");
	printf("Try 'hashgen --help' for more information.\n");
	return 0;
	}

//hash generator from args
char* hashgen(unsigned char args[]) {
       int i;
       MHASH td;
       unsigned char hash[32]; /* enough size for SHA256 */
       static unsigned char buffer[64]; /* for mhash to string */
       
       td = mhash_init(MHASH_SHA256);
       
       if (td == MHASH_FAILED) exit(1);
       
       mhash(td, args, strlen(args));
       
       mhash_deinit(td, hash);
       
       for (i = 0; i < mhash_get_block_size(MHASH_SHA256); i++) {
       		sprintf(buffer + (i * 2), "%02x", hash[i]); // convert mhash hash to mortal human string
       }
       
return buffer;
}

// for stream only
//**********************************************************
//BUG: then run without arg, have "\n" in end string
//**********************************************************
char* fhashgen(FILE *stream) {
               int i, n;
               MHASH td;
	       static unsigned char buffer[BUFFERSIZE];
               unsigned char hash[32]; /* enough size for SHA256 */

               td = mhash_init(MHASH_SHA256);

               if (td == MHASH_FAILED) exit(1);

	        if (stream != stdin)
               		while ( (n=fread(&buffer, sizeof(char), BUFFERSIZE, stream)) > 0) {
                       		mhash(td, &buffer, n);
               		}
		else
			while (fread(&buffer, 1, 1, stream) == 1) {
	                       mhash(td, &buffer, 1);
                        }

               mhash_deinit(td, hash);

               for (i = 0; i < mhash_get_block_size(MHASH_SHA256); i++) {
                        sprintf(buffer + (i * 2), "%02x", hash[i]); // convert openssl hash to mortal human string
               }

               return buffer;
        }

// Help
int help(void) {
	printf("Usage: hashgen [OPTION] [FILE]\n\n");
	printf("Options:\n");
	printf("  -f <files>\n");
	printf("  -s <strings>\n");
	printf("  -l <files>\n");
	printf("  -j jobs\n");
	printf("  -h, --help\n");
	printf("  -c <config file>\n");
	printf("  -D debug\n");
	printf("  -t type hash\n");
	printf("  -v verbose\n");
	printf("  -o output\n");
	printf("  -F format of string\n\n");
	printf("If no FILE, or when FILE is -, read standard input.\n");
	return 0;
}

//function for parse args
int args_parser(char *arg[], int argc) {
	int i,c;
	
	//DEBUG
	printf("Count args: %d\n", argc);
	printf("Run comang:");
	for (i=0; i<argc; i++)
		printf(" %s", arg[i]);
	putchar('\n');

	// -t type hash
	// -j jobs
	// -l hash lines of file 
	// -o output
}

//function for generate value
