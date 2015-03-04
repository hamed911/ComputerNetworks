#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "utilities.h"
#include "cryptogram.h"

int main(int argn, char** args){

	if(argn != 3){
		cout<<"Valid format is:\n./client voter_name ssn"<<endl;
		return 0;
	}
	showClientCommands();
	RSA* keypair=NULL;
	//Connect to Certification Authorizer
	char input_buffer[MAX_STR_SIZE];
	clear_buff(input_buffer, MAX_STR_SIZE);
	int got_ca = 0;
	while(read(STDINFD, input_buffer, MAX_STR_SIZE) > 0)
	{			
		vector<string> input_tokens;
		input_tokens = mytokenizer(input_buffer, " \n");
		
		if(input_tokens[0] == "Connect")// don't forget to check reconnect!
		{
			int n, m;//return value of read/write calls
			int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			int port_no = atoi(input_tokens[1].c_str());

			struct sockaddr_in serv_addr;
			serv_addr.sin_family = AF_INET;
			//serv_addr.sin_addr.s_addr = inet_addr(input_tokens[1].c_str());
			serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

			serv_addr.sin_port = htons(port_no);

			int status = connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
			if(status < 0)
			{
				write(STDOUTFD,"Error on connecting\n", sizeof("Error on connecting\n"));
				continue;
			}
			else 
				write(STDOUTFD, "connecting successful\n", sizeof("connecting successful\n"));

			//sending identity to server			
			char iden_buff[MAX_STR_SIZE];
			clear_buff(iden_buff, MAX_STR_SIZE);
			strcat(iden_buff, "voter ");
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
				if(temptkns[0]!="Disconnect" && temptkns[0]!="Exit")
				{
					if(temptkns[0]!="Register"){
						cout << "you must register your self to Certificate Authorizer!"<< endl;
						continue;
					}
					else {
						if(! checkClientCommandValidation(temptkns))
							continue;
						//create public key
						char publicPath[MAX_STR_SIZE];
						char privatePath[MAX_STR_SIZE];
						clear_buff(publicPath,MAX_STR_SIZE);
						clear_buff(privatePath,MAX_STR_SIZE);
						strcat( publicPath,"./DB/");
						struct stat st;
						if (stat(publicPath, &st) == -1) {
						    mkdir(publicPath, 0700);
						}
						strcat( publicPath,"public/");

						if (stat(publicPath, &st) == -1) {
						    mkdir(publicPath, 0700);
						}
						strcat(publicPath, temptkns[1].c_str());
						strcat(privatePath,"./DB/private/");
						if (stat(privatePath, &st) == -1) {
						    mkdir(privatePath, 0700);
						}
						strcat(privatePath,temptkns[1].c_str());
						if( !create_RSA_key(publicPath,privatePath)) {
							cout<<"error in creating key" << endl;
							continue;
						}/*
						char temp_buff[MAX_STR_SIZE];
						clear_buff(temp_buff,MAX_STR_SIZE);
						string information = temptkns[1]+"#"+temptkns[2]+"#"+temptkns[3]+"#";
						cout << information << endl;
						strcat(temp_buff, information.c_str());
						int* encrypt_len = new int(0);
						int* public_key_length = new int (0);
						char* encrypted_massage =  encrypt_massage_with_private_key(keypair,temp_buff,encrypt_len);
						cerr << "encrypted_lenght is" << *encrypt_len << endl;
						//create protocol
						*/
						char msg_buff[MAX_STR_SIZE];
						clear_buff(msg_buff,MAX_STR_SIZE);
						
						strcat(msg_buff,"Register ");
						strcat(msg_buff,temptkns[1].c_str());
						strcat(msg_buff," ");
						strcat(msg_buff,temptkns[2].c_str());

						/*
						char int_to_char[100];
						clear_buff(int_to_char, MAX_STR_SIZE);
						sprintf(int_to_char,"%d",*encrypt_len);
						strcat(msg_buff,int_to_char);
						strcat(msg_buff,"#");
						strcat(msg_buff,encrypted_massage);
						strcat(msg_buff,"#");
						
						char* pub_key = get_public_key(keypair,public_key_length);

						char testtt[MAX_STR_SIZE];
						clear_buff(testtt,MAX_STR_SIZE);						
						public_key_decrypt((unsigned char *) encrypted_massage,*encrypt_len,(unsigned char *) pub_key, (unsigned char *)testtt);
						cerr << "###### decrepted massage is: \n" << testtt << endl;
						cerr<< "public key length is "<< *public_key_length << endl;
						clear_buff(int_to_char, MAX_STR_SIZE);
						sprintf(int_to_char,"%d",*public_key_length);
						strcat(&msg_buff[256],int_to_char);
						strcat(&msg_buff[256+numDigits(*public_key_length)+1],"#");
						strcat(&msg_buff[256+ numDigits(*public_key_length)+2],pub_key);
						strcat(&msg_buff[256+ numDigits(*public_key_length)+2+*public_key_length],"\0");*/
						int bytes_written = write(fd, msg_buff, MAX_STR_SIZE);
						if(bytes_written < 0)
						{
							write(STDOUTFD,"Error on writing\n", sizeof("Error on writing\n"));
							continue;
						}
						//get response from server
						char res_buff[MAX_STR_SIZE];
						clear_buff(res_buff, MAX_STR_SIZE);
						int read_status = read(fd, res_buff, MAX_STR_SIZE);
						if(strncmp (res_buff,"OK",2)==0 )
							got_ca = 1;//!

						//show the response to client
						write(STDOUTFD, res_buff, strlen(res_buff));
						}
					
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
		else if(input_tokens[0] == "Exit")
		{
			return 0;
		}
		else if(input_tokens[0]!="Connect" && input_tokens[0]!="Exit")
		{
			write(STDOUTFD, "You Should connect to a Certification Authorizer First!\n", sizeof("You Should connect to a Certification Authorizer First!\n"));
		}
		if(got_ca > 0)
			break;
	}

	//show server available ports
	cout<<"\nThese are available port(s) to connect to servers\n"<<endl;
	vector<string> avail_srvr_ports = read_available_srvr_ports();
	print_myvector(avail_srvr_ports);
	sugg_comm_to_voter();

	//Connect to Server to Vote
	while(read(STDINFD, input_buffer, MAX_STR_SIZE) > 0)
	{			
		vector<string> input_tokens;
		input_tokens = mytokenizer(input_buffer, " \n");
		
		if(input_tokens[0] == "Connect")// don't forget to check reconnect!
		{
			if(!checkClientCommandValidation(input_tokens))
				continue;
			int n, m;//return value of read/write calls
			int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			int port_no = atoi(input_tokens[2].c_str());

			struct sockaddr_in serv_addr;
			serv_addr.sin_family = AF_INET;
			serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
			serv_addr.sin_port = htons(port_no);

			int status = connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
			if(status < 0)
			{
				write(STDOUTFD,"Error on connecting\n", sizeof("Error on connecting\n"));
				continue;
			}
			else 
				write(STDOUTFD, "connecting successful\n", sizeof("connecting successful\n"));

			//sending identity to server			
			char iden_buff[MAX_STR_SIZE];
			clear_buff(iden_buff, MAX_STR_SIZE);
			strcat(iden_buff, "voter ");
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
				if(temptkns[0]!="Disconnect" && temptkns[0]!="Exit")
				{
					int bytes_written = write(fd, request, strlen(request));
					if(bytes_written < 0)
					{
						write(STDOUTFD,"Error on writing\n", sizeof("Error on writing\n"));
						continue;
					}

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
