#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "utilities.h"
#include "candidate.h"
#include "cryptogram.h"


struct my_time start_time,stop_time;

int process_command(char command[MAX_STR_SIZE], int cfd)
{
	int num_of_tokens;
	//char tokens[MAX_ARRAY_SIZE][MAX_STR_SIZE];
	vector<string> tokens = mytokenizer(command, " \n");
	cerr<<tokens[0]<<"DDDDDDDDDDDDDDDDDDD"<<endl;
	//if(num_of_tokens < 0)
		//return -1;
	char result[MAX_STR_SIZE];
	clear_buff(result, MAX_STR_SIZE);
	if(tokens[0] == "Show" && strncmp(tokens[1].c_str(),"Candidates", 10)==0)
	{
		if (tokens.size()!=2)
		{
			//cout <<"incorrect number of argument!\tcorrect form=>\tShow Candidates" << endl;
cerr<<"\n\n inja gir mide \n"<<command<<" is incorrect show candidates\n\n"<<endl;
			
			return write(cfd, "incorrect number of argument!\tcorrect form=>\tShow Candidates\n", sizeof("incorrect number of argument!\tcorrect form=>\tShow Candidates\n"));
		}
		if(strncmp(tokens[1].c_str(), "Candidates", 10)!=0)
		{
			//cout <<"incorrect form \tcorrect form=>\tShow Candidates" << endl;
			return write(cfd, "incorrect form \tcorrect form=>\tShow Candidates\n", sizeof("incorrect form \tcorrect form=>\tShow Candidates\n"));
		}
		cerr<<"FAHMIDE client E"<<endl;
		show_candidates_name(result);
		return write(cfd, result, sizeof(result));
		//return process_server_comm(tokens, num_of_tokens, res, cfd);
	}
	else if(tokens[0] == "Vote")
	{
		if(certificate_is_valid(command))
		{
			//write(cfd, "you has certificated successfully\n", strlen("you has certificated successfully\n"));
			if(voting(tokens)>0)
			{
				if(in_time(start_time,stop_time))
				{
					int fd_to_save_uname = open("./DB/voters.txt", O_APPEND | O_RDWR);
					//cerr<<"uname is: "<<uname<< " in " <<uname.length()<<endl;
					char* cstr = new char [tokens[1].length()+1];
					strcpy(cstr, tokens[1].c_str());
					int dumm = write(fd_to_save_uname, cstr, strlen(cstr));
					dumm = write(fd_to_save_uname, "\n", strlen("\n"));
					if(dumm <0)
					write(STDOUTFD, "Error while writing port\n", sizeof("Error while writing port\n")); 
					close(fd_to_save_uname);
	
					return write(cfd, "your vote has saved\n", sizeof("your vote has saved\n"));
				}
				else
					return write(cfd, "Voting time is expired\n", sizeof("your vote has saved\n"));
			}
			else
				return write(cfd, "your vote hasn't saved\n", sizeof("your vote hasn't saved\n"));
			 
		}else{
			return write(cfd, "you must certificate first\n", strlen("you must certificate first\n"));
		}
			
	}
	return -1;
}

void show_results(){
		int fd, nread;
		char buf[MAX_STR_SIZE];
		struct linux_dirent *d;
		int bpos;
		char d_type;
		string t ="./DB/candidates";
		cout << "candidate code\tcondidate name\t all vote" << endl;
		fd = open(t.c_str(), O_RDONLY | O_DIRECTORY);
		while(1)
		{
			nread = syscall(SYS_getdents, fd, buf, MAX_STR_SIZE);
			if (nread == 0){
				break;
			}
			for (bpos = 0; bpos < nread;) 
			{
				d = (struct linux_dirent *) (buf + bpos);
				if( strncmp (d->d_name,".",1)!=0  && strncmp (d->d_name,"..",2)!=0){
					char cmp_path[MAX_STR_SIZE];
					clear_buff(cmp_path, MAX_STR_SIZE);
					strcpy(cmp_path, t.c_str());
					strcat(cmp_path,"/");
					strcat(cmp_path, d->d_name);

					ifstream file (cmp_path);
					if(!file.is_open()){
						cout << "error in opening candidates file" << endl;
						continue;
					}else{
						string name;
						int vote;
						file >> name >> vote;
						cout << d->d_name << "\t\t" << name << "\t\t" << vote << endl;
					}

				}	
				bpos += d->d_reclen;
			}
			break;

		}


}

vector<int> convert_string_to_time(string t){
	cerr << "in convert time ..." << endl;
	vector<int> result;
	vector<string> r_string = mytokenizer(t,":\n");
	if(r_string.size()==2){
		for(int i=0; i<2 ; i++){
			result.push_back( atoi(r_string[i].c_str()) );
			cerr << r_string[i]<< ":";
		}
		cerr<<endl;
	}
	return result;
}

bool process_server_command(vector<string> args)
{
	if(args.size()==0)
		return false;
	if(args[0]=="Add"){
		if(args.size()!=3){
			cout <<"insufficient number of argument!\tcorrect form=>\tAdd Dr.rahimi 12" << endl;
			return false;
		}
		else
			return true;
	}else if(args[0]=="Show"){
		if(args.size()!=3){
			cout <<"insufficient number of argument!\tcorrect form=>\tShow All Results" << endl;
			return false;
		}else{
			if(args[1]!="All" || args[2]!="Results"){
				cout << "correct form=>\t Show All Results" << endl;
				return false;
			}
		}
		
	}else if (args[0]=="Set"){
		if(args.size()!=5){
			cout <<"insufficient number of argument!\tcorrect form=>\t‫‪Set‬‬ ‫‪Voting‬‬ ‫‪Time‬‬ ‫‪8:00‬‬ ‫‪20:00‬‬" << endl;
			return false;
		}else{
			if(args[1]!="Voting" || args[2]!="Time" ){
				cout << "correct form=>\t‫‪Set‬‬ ‫‪Voting‬‬ ‫‪Time‬‬ ‫‪8:00‬‬ ‫‪20:00‬‬" << endl;
				return false;
			}else if( convert_string_to_time(args[3]).size()!=2 || convert_string_to_time(args[4]).size()!=2 ){
				cout << "Erorr in etering time:\tcorrect form=>\t‫‪Set‬‬ ‫‪Voting‬‬ ‫‪Time‬‬ ‫‪8:00‬‬ ‫‪20:00‬‬" << endl;
				return false;
			}
		}
	}else if(args[0]=="Extend"){
		if(args.size()!=4){
			cout <<"insufficient number of argument!\tcorrect form=>\t‫‪Extend‬‬ ‫‪Voting‬‬ ‫‪Time‬‬ ‫‪23:00‬‬" << endl;
			return false;
		}else{
			if(args[1]!="Voting" && args[2]!="Time"){
				cout << "correct form=>\t‫‪Extend‬‬ ‫‪Voting‬‬ ‫‪Time‬‬ ‫‪23:00‬‬" << endl;
				return false;
			}
			vector <int > temp =convert_string_to_time( args[3]);  
			if ( temp.size()!=2 ){
				cout << "Erorr in etering time:\tcorrect form=>\t‫‪Extend‬‬ ‫‪Voting‬‬ ‫‪Time‬‬ ‫‪23:00‬‬" << endl;
				return false;
			}
		}
	}else return false;
	
	return true;
	
}

int main(int argn, char** args){
	//struct my_time start_time,stop_time;
	if (argn != 2){
		cout<<"Valid format is :\n./server server_port"<<endl;
		return 0;
	}
	show_server_commands();

	//connect to one of CAs to get its public key
	char input_buffer[MAX_STR_SIZE];
	clear_buff(input_buffer, MAX_STR_SIZE);	
	int got_pub_key = 0;
	while(read(STDINFD, input_buffer, MAX_STR_SIZE) > 0)
	{
		//int input_tokens_num = input_tokens.size();
		vector <string> input_tokens = mytokenizer(input_buffer, " \n");
		if(input_tokens[0]== "Connect")// don't forget to check reconnect!!
		{
			int n, m;//return value of read/write calls
			int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			int port_no = atoi(input_tokens[1].c_str());

			struct sockaddr_in serv_addr;
			serv_addr.sin_family = AF_INET;
			serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
			serv_addr.sin_port = htons(port_no);

			int status = connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
			if(status < 0)
			{
				write(STDOUTFD,"Error on connecting\n", sizeof("Error on connecting\n"));
				clear_buff(input_buffer, MAX_STR_SIZE);
				continue;
				//exit
			}
			else 
				write(STDOUTFD, "connecting successful\n", sizeof("connecting successful\n"));
			//sending identity to server
			char iden_buff[MAX_STR_SIZE];
			clear_buff(iden_buff, MAX_STR_SIZE);
			
			strcat(iden_buff, "server ");
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
					got_pub_key = 1;

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
			write(STDOUTFD, "You Should Connect to a Certification Authorizer First!\n", sizeof("You Should Connect to a Certification Authorizer First!\n"));
		}
		if(got_pub_key > 0)
			break;
	}

	//from here this can be server for voters and response
	//##################################################################################################################################
	int port_number = atoi(args[1]);
	//save_srvr_port(args[1]);
	if(found_in(args[1], read_available_srvr_ports()) < 0)
	{
		int fd_to_save_port = open("./DB/servers.txt", O_APPEND | O_RDWR);
			cerr<<"args[1] is: "<<args[1]<< " in " <<strlen(args[1])<<endl;
			int dumm = write(fd_to_save_port, args[1], strlen(args[1]));
			dumm = write(fd_to_save_port, "\n", strlen("\n"));
			if(dumm <0)
					write(STDOUTFD, "Error while writing port\n", sizeof("Error while writing port\n")); 
			close(fd_to_save_port);
	}

	const int num_of_connection = 5;
	
	//creating socket to be server for voters
	int server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port_number);

	struct sockaddr_in client_addr;
	//binding 
	int binding_st = bind(server_fd,(struct sockaddr*) &server_addr, sizeof(server_addr));
	if(binding_st == -1)
	{
		write(STDOUTFD, "binding error\n", sizeof("binding error\n"));
		return -1;
		//exit
	}
	else 	
		write(STDOUTFD, "binding successful\n", sizeof("binding successful\n"));
	//listenning to voters
	int listening_st = listen(server_fd, num_of_connection);
	if(listening_st == -1)
		write(STDOUTFD, "listening error\n", sizeof("listening error"));
	else
		write(STDOUTFD, "listening successful\n", sizeof("listening successful"));

	fd_set read_fdset, temp_fdset;
	struct timeval tv;
	int ret_val;
	int new_sock_fd, it_fd;

	// Watch stdin (fd 0) to see when it has input.
	FD_ZERO(&read_fdset);
	FD_SET(server_fd, &read_fdset);
	FD_SET(STDINFD, &read_fdset);

	// Wait up to five seconds. 
	tv.tv_sec = 10 * 60;
	tv.tv_usec = 0;

	unsigned int size_of_client_addr = sizeof(client_addr);

	int status;
	while(1)
	{
		memcpy(&temp_fdset, &read_fdset, sizeof(temp_fdset));
		status = select(FD_SETSIZE, &temp_fdset, (fd_set *)0, (fd_set *)0, (struct timeval*) &tv);
		if(status < 1)
		{
			write(STDOUTFD, "Select Error\n", sizeof("Select error\n"));
			break;
		}

		for(it_fd = 0; it_fd<FD_SETSIZE; it_fd++)
		{
			if(FD_ISSET(it_fd, &temp_fdset))
			{
				if(it_fd == STDINFD)
				{
					char buff_read [MAX_STR_SIZE], response_buff[MAX_STR_SIZE];
					clear_buff(buff_read, MAX_STR_SIZE);
					clear_buff(response_buff, MAX_STR_SIZE);

					int sts = read(it_fd, buff_read, MAX_STR_SIZE-1);
					if(sts <0 ){
						cout << "error in reading from commandline" << endl;
					}
					vector <string> args = mytokenizer(buff_read," \n");
					if( !process_server_command(args) )
					{
						clear_buff(buff_read, MAX_STR_SIZE);
						continue;
					}
					if(args[0]=="Add"){
						Candidate* temp_cand = new Candidate(args[1],atoi(args[2].c_str()));
						temp_cand->saveCandidate();
						clear_buff(buff_read, MAX_STR_SIZE);
						continue;
					}else if(args[0]=="Set"){
						vector<int> st = convert_string_to_time(args[3]);
						start_time.hour = st[0];
						start_time.min = st[1];
						st = convert_string_to_time(args[4]);
						stop_time.hour=st[0];
						stop_time.min=st[1];
						cout << "start time is: " <<start_time.hour << ":" << start_time.min << 
							" and stop time is: " << stop_time.hour << ":" << stop_time.min << endl;
							clear_buff(buff_read, MAX_STR_SIZE);
							continue;
					}else if (args[0]=="Extend"){
						vector<int> st = convert_string_to_time(args[3]);
						stop_time.hour = st[0];
						stop_time.min = st[1];
						cout << "stop time extends till: " <<stop_time.hour << ":" << stop_time.min << endl;
						clear_buff(buff_read, MAX_STR_SIZE);
						continue;
					}
					else if(args[0]=="Show"){
						show_results();
						continue;
					}else
					{
						cout << "wrong input command" << endl;
						continue;
					}
				}
				if(it_fd == server_fd)
				{	
					new_sock_fd = accept(server_fd, NULL, NULL);
					if(new_sock_fd < 0)
					{
						write(STDOUTFD,"Error on accepting\n", sizeof("Error on accepting\n"));
						break;
					}
					else write(STDOUTFD,"accepting successful\n", sizeof("accepting successful\n"));
					FD_SET(new_sock_fd, &read_fdset);
				}
				else
				{
					int n, m;
					char buff_read [MAX_STR_SIZE], response_buff[MAX_STR_SIZE];
					clear_buff(buff_read, MAX_STR_SIZE);
					clear_buff(response_buff, MAX_STR_SIZE);

					n = read(it_fd, buff_read, MAX_STR_SIZE-1);
					if(n == 0)
					{
						close(it_fd);
						FD_CLR(it_fd, &read_fdset);
						write(STDOUTFD, "Removing One Client_fd\n", sizeof("Removing One Client_fd\n"));
					}
					else if(n < 0)
					{
						write(STDOUTFD, "Error on reading\n", sizeof("Error on reading\n"));
					}
					//after reading successfully
					else
					{
						if(buff_read != "DC")
						{
							cerr<<"Varede dasture GHEIRE DC shode ~"<<endl;
							cerr<<"command is: "<<buff_read<<endl;
							if(process_command(buff_read, it_fd) < 0)
							{
								int st = write(it_fd, "Invalid command\n", sizeof("Invalid command\n"));
								if(st < 0) 
									write(STDOUTFD, "Error on writing\n", sizeof("Error on writing\n"));
								continue;
							}
							//if(s < 0) write(STDOUTFD, "Error on writing\n", sizeof("Error on writing\n"));
						}
						else //if(buff_read == "DC")
						{
							cerr<<"varede dasture DC DC DC shode"<<endl;
							cerr<<"command is: "<<buff_read<<endl;
							write(it_fd, "Disconnecting in Progress ...\n",
									sizeof("Disconnecting in Progress ...\n"));
							close(it_fd);
							FD_CLR(it_fd, &read_fdset);
							write(STDOUTFD, "Removing One Client_fd\n", sizeof("Removing One Client_fd\n"));
						}
					}
				}
			}
		}
	}
	close(server_fd);
	
	//free allocated memories
	return 0;
}
