#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "utilities.h"
#include "cryptogram.h"
int main(int argn, char** args){
	bool isAdmin = false;
	if(argn != 2){
		char argErr[]  = "Valid format is:\n./client voter_name\n";
		write(STDOUTFD, argErr, strlen(argErr));
		return 0;
	}
	RSA* keypair = create_RSA_key();
	//show CA available ports
	cout<< "These are available port(s) to connect to CAs\n";
	//read available ports
	vector<string> avail_ports = read_available_ports();
	for(int i=0; i<avail_ports.size(); i++)
		cout<<avail_ports[i]<<endl;

	showClientCommands();

	char input_buffer[MAX_STR_SIZE];
	clear_buff(input_buffer, MAX_STR_SIZE);
	while(read(STDINFD, input_buffer, MAX_STR_SIZE) > 0)
	{			
		//parsing input_buffer
		int input_tokens_num;		
		vector<string> input_tokens;
		string ord(input_buffer);
		input_tokens = mytokenizer(input_buffer, " ");
		
		if(input_tokens[0] == "Connect")// don't forget to check reconnect!!!!!!!!!!!!!!!!!!!!!!!!!
		{
			// if(!isAdmin){
			// 	cout << "you must be admin to connect server" << endl;
			// 	continue;
			// }
			if( checkClientCommandValidation(input_tokens,args[1]) )
				continue;
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
			}
			else write(STDOUTFD, "connecting successful\n", sizeof("connecting successful\n"));

			//sending identity to server			
			while(1)
			{
				//read command
				char in_buff[MAX_STR_SIZE];
				clear_buff(in_buff, MAX_STR_SIZE);
				int status = read(STDINFD, in_buff, MAX_STR_SIZE);
				char request[MAX_STR_SIZE];
				strcpy(request, in_buff);
				strcat(request, in_buff);		

				int tokens_num;
				//char temptkns[MAX_ARRAY_SIZE][MAX_STR_SIZE];
				vector<string> temptkns = mytokenizer(in_buff, " \n");
	
				//send command for server
				if(temptkns[0]!="Disconnect" && temptkns[0]!="Exit")
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
				else if(temptkns[0] == "Disconnect")
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
		else if(input_tokens[0]=="Register"){
			if( checkClientCommandValidation(input_tokens,args[1]) )
				continue;
		}
		else if(input_tokens[0] == "Exit")
		{
			return 0;
		}
		else if(input_tokens[0]!="Register" && input_tokens[0]!="Exit")
		{
			write(STDOUTFD, "You Should Register to Certificate authority First!\n", sizeof("You Should Register to Certificate authority First!\n"));
		}
	}

	//free allocated memories
	return 0;
}
