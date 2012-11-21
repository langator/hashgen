/* main.c */
// Copyright 2012 langator
// Distributed under the terms of the GNU General Public License v2 or later

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

int arg_error(char *msg, char *msg2);
int opt_error(char *msg, char msg2);
int is_file(char arg[], int first, int one);
int args_parser(char *arg[], int argc);
int small_help(void);
char * hashgen(unsigned char args[]);
char * fhashgen(FILE *stream);
int bufhash(char arg[]);
int test_arg(char arguments[]);

int main(int argc, char *argv[]) { 
int c, i, n, e;
FILE *fd;
char string[MAXSTR];
char str[MAXSTR];
char str2[MAXSTR];
char a[MAXSTR];
struct stat st;

int one=0, first=0, second=0, start_file=-1, end_file=-1, OUTPUT=-1, only_one=0;
int V=0, F=0, L=0, S=0, H=0, O=0, T=0; // for options

// arg parser
//n=args_parser(argv, argc);
//return n;

// input from stdin
/*
 *   for create hash of string without '\n' tap Ctrl-D 2 time 
 */
if ( argc == 1) {
        printf("%s\n", fhashgen(stdin));
        return 0;
}
if ( (argc == 2) && ((strcmp(argv[1],"-")) == 0) ) {
        printf("%s\n", fhashgen(stdin));
        return 0;
}


//args parser v.2, next step - function
for (i=1; i<argc; i++ ) {
	if ( (first > 1) || (second > 1) || (only_one > 0 && argc > 2) ) {
		small_help();
		return 1;
		}
	
	sprintf(str, "%s", argv[i]);

	//if first arg is file
	if ( (i == 1) && (is_file(argv[1], first, one) == 0) ) {
		str[0]='-';
		str[1]='f';
		str[2]='\0';
		i=i-1;
		}	
	//for long options (long to short)
	if (str[0]=='-' && str[1]=='-') 
		if ( strcmp(argv[i], "--help")==0 ) 
			help();
			//printf("%s\n", argv[i]);
		else {
			arg_error("Cannot args: ", argv[i]);
			return 1;
		}
	
	// start parse args
	if ( (str[0]=='-' ) && ( str[1]!='-' ) )
		for (c=1; (str[c]!='\0' && i<argc); c++) {
			switch (str[c]) {
			   // Option "v"
			   case 'v':
			   	V=1;
				break;
			   
			   //Option "h"
			   case 'h':
			   	only_one=only_one++;
				if (only_one > 1 || argc > 2 ) {
					opt_error("Option error: ", str[c]);
					return 1;
					}
				else {
					help();
					return 0;
					}
				break;
				
			   //Option "l"
			   case 'l':
			   	L=1;
				one+=1;
				first+=1;
				if ( (first > 1) || (one > 1) ) {
					opt_error("Option error: ", str[c]);
					return 1;
					}
				//check count args
				if (i+1 == argc) {
					fprintf(stderr, "Option error: \"%c\", cannot file.\n", str[c]);
				        small_help();
					return 1;
					}

				// count arg is files
				for (i=i+1; ( is_file(argv[i], first, one) == 0 ); i++) {
                        		if (start_file == -1)  
                                		start_file=i; 
                        		else
                                		end_file=i;
					if ( ((i+1) == argc) )
						break;
				}
								
				if (start_file == -1) {
					arg_error("Cannot file: ", argv[i]);
					return 1;
					}
				
				// dont remove
				if ( is_file(argv[i], first, one) != 0 )
					i--;
				break;
			   //Option "f"
			   case 'f': 
			   	F=1;
				one+=1;
				first+=1;
				if ( (first > 1) || (one > 1) ) {
					opt_error("Option error: ", str[c]);
					return 1;
					}
				//check count args
				if (i+1 == argc) {
					fprintf(stderr, "Option error: \"%c\", cannot file.\n", str[c]);
				        small_help();
					return 1;
					}

				//Нужно использовать do while, чтобы i не переполнялось 
				// count arg is files
				for (i=i+1; ( is_file(argv[i], first, one) == 0 ); i++) {
                        		if (start_file == -1)  
                                		start_file=i; 
                        		else
                                		end_file=i;
					if ( ((i+1) == argc) )
						break;
				}
								
				if (start_file == -1) {
					arg_error("Cannot file: ", argv[i]);
					return 1;
					}
				
				// dont remove
				if ( is_file(argv[i], first, one) != 0 )
					i--;

				break;
			   //Option 's'
			   case 's':
			   	S=1;
				one+=1;
				first+=1;
				if ( (first > 1) || (one > 1) )  {
					opt_error("Option error: ", str[c]);
                                        return 1;
                                        }
                                
				//check count args
				if (i+1 == argc) {
					fprintf(stderr, "Option error: \"%c\", cannot file.\n", str[c]);
				        small_help();
					return 1;
					}

				//Нужно использовать do while, чтобы i не переполнялось 
				// count arg is files
				for (i=i+1; ( is_file(argv[i], first, one) == 0 ); i++) {
                        		if (start_file == -1)  
                                		start_file=i; 
                        		else
                                		end_file=i;
					if ( ((i+1) == argc) )
						break;
				}
								
				if (start_file == -1) {
					arg_error("Cannot file: ", argv[i]);
					return 1;
					}
				
				// dont remove
				if ( is_file(argv[i], first, one) != 0 )
					i--;


				
				break;

			   //Option "o"
			   case 'o':
			   	O=1;
				one+=1;
			   	second=second++;
				if ( ((i+1) == argc) ) {
					arg_error("Cannot output file", "");
					return 1;
					}
				
				for (i=i+1; ( is_file(argv[i], second, one) == 0 ); i++) {
					OUTPUT=i;
                                        if ( ((i+1) == argc) )
                                                break;
					else
						 if ( is_file(argv[i+1], second, one) == 0 ) {
                                                	arg_error("Option error: \"-o\", uses more then one output file", "");
                                                	return 1;
                                                	}
						else
							break;  

                                }
				
				if ( OUTPUT == -1 ) {
					arg_error("Cannot output file.", "");
					return 1;
					}
				
				break; 
			   
			   
			   default:
			   	opt_error("Invalid option: ", str[c]);
				return 1;
				break;
			}
			one=0;
		   
		   //DEBUG
		   //printf("STR= %c I=%d %s\n",str[c], i, argv[i]);
		   }

// end arg parser
}

//DEBUG
//printf("ARGPARSER OUT\n");


//
// NEEDED ADD CHECK FILES argv[start_file] - argv[end_file]
//

// "-f" 
if (F==1) {
	do {
		if ( (fd = fopen(argv[start_file],"r")) == NULL ) {
                        fprintf(stderr,"Error open file: %s\n", argv[start_file]);
                        return 1;
			}
		else if (V == 1) {
			printf("%s  %s\n", fhashgen(fd), argv[start_file]);
                        fclose(fd);
                        }
                else {
			printf("%s\n", fhashgen(fd));
			fclose(fd);
			}
			
		start_file++;
	} while (start_file<(end_file+1)); 
return 0;
}

// "-s" options
if (S==1) {
        do {
                if (V == 1) {
                        printf("%s  %s\n", hashgen(argv[start_file]), argv[start_file]);
                        }
                else {
                        printf("%s\n", hashgen(argv[start_file]));
                        }

                start_file++;
        } while (start_file<(end_file+1));
return 0;
}

// "-l" options
if (L==1) {
        do {
                if (V == 1) {
                        //NEED add new arg to bufhash for verbose mode
			bufhash(argv[start_file]);
			}
                else 
		        bufhash(argv[start_file]);

                start_file++;
        } while (start_file<(end_file+1));
return 0;
}


//for stdout in file need be write function:
//int output(FILE, V) 


return 0;
//================================================================
//	EXIT MAIN
//================================================================



// input from stdin
/*
 *   for create hash of string without '\n' tap Ctrl-D 2 time 
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
						fprintf(stderr, "ERROR: line more than %d char\n", MAXSTR);
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
//for options
int opt_error(char *msg, char msg2){
	fprintf(stderr, "%s\"%c\"\n", msg, msg2);
	small_help();
	return -1;
}
// for args
int arg_error(char *msg, char *msg2){
        fprintf(stderr, "%s%s\n", msg, msg2);
        small_help();
        return -1;
}




// check arg on file
int is_file(char arg[], int first, int one ) {
	int i;
	//DEBUG
	//printf("%s\n", arg);
	if ( (first > 1) || (one > 1) ) {
		fprintf(stderr, "Error args.\n"); 
		small_help();
		return 1;
	}
	if (arg[0]!='-')
		return 0;
	else 
		return -1;
} 


//*********************************
// *	BUG: function work 1.6 time slow then sha256sum
// ********************************

//buffered read by line and hashgen
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
		sprintf(buffer + (i * 2), "%02x", hash[i]); // convert hash to mortal human string
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

struct hashv {
	char *hash;
	char *argm;
	};

//int verbose_print()



//function for parse args
int args_parser(char *arg[], int argc) {
	int i,c, testret;
	char * d;
	char str[MAXSTR];
	struct hashv s;
		
	//DEBUG
	printf("Count args: %d\n", argc);
	printf("Run comang:");
	for (i=0; i<argc; i++){
		s.argm=arg[i];
		s.hash=hashgen(arg[i]);
		printf("%s %s\n", s.hash, s.argm);
		}
		//printf(" %s", arg[i]);
	putchar('\n');

	for (i=0; i<argc; i++) {
        	sprintf(str, "%s", arg[i]);

        	if (str[0]=='-' && str[1]!='-') {
			printf("short arg: %s\n", arg[i]);
			testret=test_arg(arg[i]);
			if (testret == -1) 
				return 1;
			}

		if (str[0]=='-' &&  str[1]=='-')
			printf("long arg: %s\n", arg[i]);
		
        	for (c=0; str[c]!='\0'; c++)
			;;
	}
        putchar('\n');



	// -t type hash
	// -j jobs
	// -o output
	return 0;
}

//test arg
int test_arg(char argument[]) {
	int i, STRUCT;
	for (i=1; argument[i]!='\0'; i++) {
		switch (argument[i]) {
			case 's':
				printf("S\n");
				break;
			case 'f':
				printf("F\n");
				break;
			case 'l':
				printf("L\n");
				break;
			case 'h':
				printf("H\n");
				break;
			case 'v':
				printf("V\n");
				STRUCT=1;
				break;
			case 'o':
				printf("O\n");
				break;
			case 't':
				printf("T\n");
				break;
			case 'j':
				printf("J\n");
				break;
			default:
				fprintf(stderr, "Invalid option: '%c'\n", argument[i]);
				return -1;

		}
	}
	return 0;
}

//function for generate value
//-F option
