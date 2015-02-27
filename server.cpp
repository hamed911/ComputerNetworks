#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "utilities.h"


int main(int argn, char** args){

	if (argn != 4){
		char argErr[] = "Valid format is :\n./Company company_name stocks_num price\n";
		write(STDOUTFD,argErr , strlen(argErr));
		return 0;
	}
	char sugst[] = "\nThese are the valid commands:\n\n";
	char comm[][MAX_STR_SIZE] = 
	{ "Connect\t\t->\tConnect to Server\n"
		, "DC\t\t->\tDisconnect from the server\n"
			, "Introduction\t->\tIntroduce new company to server\n"
			, "Get Status\t->\tGets latest company status from server\n"
			, "Exit\t\t->\tExit the Program\n\n"};

	write(STDOUTFD, sugst, sizeof(sugst));
	int p;
	for(p = 0; p < 5; p++)
		write(STDOUTFD, comm[p], sizeof(comm[p]));

	//char input_buffer[MAX_STR_SIZE];
	//clear_buff(input_buffer, MAX_STR_SIZE);
	string input_buffer;
	char* cstr = new char [input_buffer.length()+1];
	strcpy(cstr, input_buffer.c_str());
	getline(cin, input_buffer);
	cin.clear();
	input_buffer += "\n";

	vector <string> input_tokens = mytokenizer(input_buffer, " ");

	while(read(STDINFD, cstr, MAX_STR_SIZE) > 0)
	{
		int input_tokens_num = input_tokens.size();
		if(input_tokens[0]== "Connect")// don't forget to check reconnect!!
		{
			int n, m;//return value of read/write calls
			int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			int port_no = atoi(input_tokens[2].c_str());

			struct sockaddr_in serv_addr;
			serv_addr.sin_family = AF_INET;
			serv_addr.sin_addr.s_addr = inet_addr(input_tokens[1].c_str());
			serv_addr.sin_port = htons(port_no);

			int status = connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
			if(status < 0)
			{
				write(STDOUTFD,"Error on connecting\n", sizeof("Error on connecting\n"));
				continue;
				//exit
			}
			else write(STDOUTFD, "connecting successful\n", sizeof("connecting successful\n"));
			//sending identity to server
			char iden_buff[MAX_STR_SIZE];
			clear_buff(iden_buff, MAX_STR_SIZE);
			
			strcat(iden_buff, "company ");
			strcat(iden_buff, args[1]);
			strcat(iden_buff, " \0");

			while(1)
			{
				//read command
				char in_buff[MAX_STR_SIZE];
				clear_buff(in_buff, MAX_STR_SIZE);
				int status = read(STDINFD, in_buff, MAX_STR_SIZE);
				char request[MAX_STR_SIZE];
				strcpy(request, iden_buff);
				strcat(request, in_buff);

				int tokens_num;
				//char temptkns[MAX_ARRAY_SIZE][MAX_STR_SIZE];
				vector<string> temptkns = mytokenizer(in_buff, " \n");

				//send command for server
				if(temptkns[0]!="DC" && temptkns[0] != "Exit")
				{
					int bytes_written = write(fd, request, strlen(request));
					if(bytes_written < 0)
						write(STDOUTFD,"Error on writing\n", sizeof("Error on writing\n"));


					//get response from server
					char res_buff[MAX_STR_SIZE];
					clear_buff(res_buff, MAX_STR_SIZE);
					int read_status = read(fd, res_buff, MAX_STR_SIZE);

					//show the response to client
					write(STDOUTFD, res_buff, strlen(res_buff));
				}
				else if(temptkns[0] == "DC")
				{
					int bytes_written = write(fd, "DC", strlen("DC"));
					if(bytes_written < 0)
						write(STDOUTFD,"Error on writing\n", sizeof("Error on writing\n"));
					break;
				}
				else if(temptkns[0] == "Exit")
				{
					int bytes_written = write(fd,"DC", strlen("DC"));
					if(bytes_written < 0)
						write(STDOUTFD,"Error on writing\n", sizeof("Error on writing\n"));
					close(fd);
					return 0;
				}
			}
			close(fd);
		}
		else if(input_tokens[0] == "Exit")
		{
			return 0;
		}
		else if(input_tokens[0]!="Connect" && input_tokens[0]!="Exit")
		{
			write(STDOUTFD, "You Should Connect to Server First!\n", sizeof("You Should Connect to Server First!\n"));
		}
	}

	//free allocated memories
	return 0;
}
