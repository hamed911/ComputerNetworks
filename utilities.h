#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <string>
#include <string.h>
#include <dirent.h>     
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <arpa/inet.h>
#include <fstream>

#define STDINFD 0
#define STDOUTFD 1
#define STDERRFD 2
#define MAX_STR_SIZE 2000
#define MAX_ARRAY_SIZE 20
#define ADMIN_USER "admin"
#define ADMIN_PASS "admin"
/*
#define GETMAX(X,Y) ((X) > (Y) ? (X) : (Y))
#define GETMIN(X,Y) ((Y) > (X) ? (X) : (Y))
*/
using namespace std;


struct linux_dirent {
	long           d_ino;
	off_t          d_off;
	unsigned short d_reclen;
	char           d_name[];
};

bool writeLine(string,int);
vector<string> read_available_ports();
int found_in(string , vector<string> );
vector<string> mytokenizer(string, string);
int clear_buff(char* , int );
void showClientCommands();
bool checkClientCommandValidation(vector<string> args,char* name);

/*int strlength(char str[MAX_STR_SIZE]);
void tokenizer(char str[MAX_STR_SIZE], char delim[MAX_STR_SIZE], int* num_of_tokens, char res[MAX_ARRAY_SIZE][MAX_STR_SIZE]);

int mystrcmp(char const*p, char const *q);
void print(char str[MAX_STR_SIZE]);
void int_to_str(int val, char res[20], int base);
int days_in_month(int month, int year);
void get_date(char time_str [30]);
void list_files(char pathname[MAX_STR_SIZE], char file_names[MAX_ARRAY_SIZE][MAX_STR_SIZE], int* file_num);
*/
#endif
