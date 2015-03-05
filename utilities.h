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
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <sstream>  

#define STDINFD 0
#define STDOUTFD 1
#define STDERRFD 2
#define MAX_STR_SIZE 2048
#define MAX_ARRAY_SIZE 20
#define ADMIN_USER "admin"
#define ADMIN_PASS "admin"

using namespace std;


struct linux_dirent {
	long           d_ino;
	off_t          d_off;
	unsigned short d_reclen;
	char           d_name[];
};


struct my_time {
	int min;
	int hour;
};

int create_directories(char path_name[MAX_STR_SIZE]);
vector<string> read_available_ca_ports();
vector<string> read_available_srvr_ports();
int found_in(string str, vector<string> arr);
void save_ca_port(string portno);
void save_srvr_port(string portno);
vector<string> mytokenizer(string str, string delim);
int clear_buff(char* c, int size);
void print_myvector(vector<string> v);
void sugg_comm_to_voter();
void sugg_comm_to_server();
void showClientCommands();
bool checkClientCommandValidation(vector<string> args);
void show_server_commands();
int numDigits(int );
vector<string> mytokenizer_char(char* ,string );
int show_candidates_name(char res[MAX_STR_SIZE]);
vector<string> read_available_unames();
int save_vote_comm(string uname);
int voting(vector<string> v);
bool in_time(my_time start_time, my_time stop_time);
bool file_exist(const char *name);

#endif
