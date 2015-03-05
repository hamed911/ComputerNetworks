#include "utilities.h"
#include <string>

bool file_exist(const char *name){
    if( access( name, F_OK ) != -1 )
        return true;
        //file exists
    else 
        return false;
        // file doesn't exist
}


int create_directories(char path_name[MAX_STR_SIZE])
{
	int mkdir_status = mkdir(path_name, S_IRUSR | S_IWUSR | S_IXUSR | S_IROTH | S_IWOTH | S_IXOTH);
	char vfpath[MAX_STR_SIZE];
	clear_buff(vfpath, MAX_STR_SIZE);
	strcpy(vfpath, path_name);
	strcat(vfpath, "/validators.txt");
	char path_name_cmp[MAX_STR_SIZE], path_name_cst[MAX_STR_SIZE];
	clear_buff(path_name_cmp, MAX_STR_SIZE);
	clear_buff(path_name_cst, MAX_STR_SIZE);
	strcpy(path_name_cmp, path_name);
	strcat(path_name_cmp, "/candidates");
	//strcpy(path_name_cst, path_name);
	//strcat(path_name_cst, "/Customer");
	mkdir_status = mkdir(path_name_cmp, S_IRUSR | S_IWUSR | S_IXUSR| S_IROTH | S_IWOTH | S_IXOTH);
	//mkdir_status = mkdir(path_name_cst, S_IRUSR | S_IWUSR | S_IXUSR| S_IROTH | S_IWOTH | S_IXOTH);
	return 0;
}


int numDigits(int number)
{
    int digits = 0;
    if (number < 0) digits = 1; // remove this line if '-' counts as a digit
    while (number) {
        number /= 10;
        digits++;
    }
    return digits;
}

vector<string> read_available_ca_ports()
{
	int open_fd = open("./DB/validators.txt", O_RDONLY);
	if(open_fd < 0)
		write(STDOUTFD, "can't open available ports file", sizeof("can't open available ports file"));
	char read_buff[MAX_STR_SIZE];
	clear_buff(read_buff, MAX_STR_SIZE);
	int r_st = read(open_fd, read_buff, MAX_STR_SIZE-1);
	vector<string> ports = mytokenizer(read_buff, "\n");
	return ports;
}

vector<string> read_available_srvr_ports()
{
	int open_fd = open("./DB/servers.txt", O_RDONLY);
	if(open_fd < 0)
		write(STDOUTFD, "can't open available ports file", sizeof("can't open available ports file"));
	char read_buff[MAX_STR_SIZE];
	clear_buff(read_buff, MAX_STR_SIZE);
	int r_st = read(open_fd, read_buff, MAX_STR_SIZE-1);
	vector<string> ports = mytokenizer(read_buff, "\n");
	return ports;
}

int found_in(string str, vector<string> arr)
{
	for(int i=0; i<arr.size(); i++)
	{
		if(str == arr[i])
			return i;
	}
	return -1;
}

bool checkClientCommandValidation(vector<string> args)
{
	if(args.size()<=0)
		return false;
	if(args[0]=="Connect"){
		if(args.size()!=5){
			cout <<"insufficient number of argument!\tcorrect form=>\tConnect Server 2000 Admin admin's password" << endl;
			return false;
		}
		else
			return true;
	}
	else if(args[0]=="Register"){
		if(args.size()!=5){
			cout <<"insufficient number of argument!\tcorrect form=>\tRegister 123456789 ghamar" << " ***** *****" << endl;
			return false;
		}else{
			if(args[3]!=args[4]){
				cout << "Passwords are not the same!" << endl;
				return false;
			}
		}
		
	}
	else if(args[0]=="Show" && strncmp(args[1].c_str(),"Candidates", 10)==0)
	{
		if (args.size()!=2)
		{
			cout <<"incorrect number of argument!\tcorrect form=>\tShow Candidates" << endl;
			return false;
		}
		if(strncmp(args[1].c_str(), "Candidates", 10)!=0)
		{
			cout <<"incorrect form \tcorrect form=>\tShow Candidates" << endl;
			return false;
		}
	}
	return true;
}
void show_server_commands()
{
	cout<<"*****"<<endl;
	cout<<"     At first connect to one of Certification Authorizers to get its public key"<<endl;
	cout<<"     ,then Disconnect from it"<<endl;
	cout<<"     ,then your serving socket will start working to get votes"<<endl;
	cout<<"*****"<<endl;
	//show CA available ports
	cout<<"\nThese are available port(s) to connect to Certification Authorizer\n"<<endl;
	//read available ports
	vector<string> avail_ports = read_available_ca_ports();
	print_myvector(avail_ports);
	sugg_comm_to_server();
}
void showClientCommands()
{
	cout<<"*****"<<endl;
	cout<<"     At first connect to one of Certification Authorizers to get a ca"<<endl;
	cout<<"     ,then disconnect from it"<<endl;
	cout<<"     ,then connect to one of servers to choose your candidate"<<endl;
	cout<<"*****"<<endl;
	//show CA available ports
	cout<<"\nThese are available port(s) to connect to Certification Authorizers\n"<<endl;
	//read available ports
	vector<string> avail_ca_ports = read_available_ca_ports();
	print_myvector(avail_ca_ports);

	cout<<"\nThese are the valid commands:\n\n";
	char comm[][MAX_STR_SIZE] = 
	{ "Connect\t\t->\tConnect voter to Server\n"
		, "Disconnect\t\t->\tDisconnect from the server by admin\n"
			, "Register\t->\tRegister yourself to certificate authority\n"
			, "Show Log\t->\tShow voters name to admin\n"
			, "Vote\t\t->\tVote to specific candidate\n"
			, "ShowCandidates\t->\tShow all the available candidates\n"
			, "SetVotingTime\t->\tset voting time\n"
			, "ExtendVotingTime\t->\textendVotingTime\n"
			, "ShowServers\t->\tShow server port\n"
			, "ShowCA\t->\tShow CA's port\n"
			, "Exit\t\t->\tExit the Program\n\n"};
	for(int p = 0; p < 11; p++)
		cout<< comm[p];
}



/*{
	if(found_in(portno, read_available_ca_ports()) < 0)
		{
			int fd_to_save_port = open("./DB/validators.txt", O_APPEND | O_RDWR);
			cerr<<"args[1] is: "<<portno<< " in " <<portno.length()<<endl;
			int dumm = write(fd_to_save_port, portno, portno.length());
			dumm = write(fd_to_save_port, "\n", strlen("\n"));
			if(dumm <0)
					write(STDOUTFD, "Error while writing port\n", sizeof("Error while writing port\n")); 
			close(fd_to_save_port);
		}
}*/

/*void save_srvr_port(string portno)
{
	if(found_in(portno, read_available_srvr_ports()) < 0)
	{
		int fd_to_save_port = open("./DB/servers.txt", O_APPEND | O_RDWR);
			cerr<<"args[1] is: "<<portno<< " in " <<portno.length()<<endl;
			int dumm = write(fd_to_save_port, portno.c_str(), sizeof(portno));
			dumm = write(fd_to_save_port, "\n", strlen("\n"));
			if(dumm <0)
					write(STDOUTFD, "Error while writing port\n", sizeof("Error while writing port\n")); 
			close(fd_to_save_port);
	}
}*/

vector<string> mytokenizer_char(char* str,string delim){
	vector<string> ans;
	int j=0;
	while(str[j]!='\0'){
		string temp="";
		for(int i=0; i<delim.length(); i++){
			while(str[j]!='\0' && str[j]!=delim[i]){
				temp += str[j];	
				j++;
			}
			if(str[j]!='\0')
				j++;
			cout<<"here is temp: " << temp << endl;
			ans.push_back(temp);
		}
	}
	return ans;
}

vector<string> mytokenizer(string str, string delim)
{	
	string tmp;
	vector<string> res;
	int i, tmp_index = 0, res_index = 0;
	for(i = 0; i < str.length(); i++)
	{
		int cut = 0;
		int j;
		for(j = 0; j < delim.length(); j++)
		{
			if (i==str.length()-1 || str[i] == delim[j])
			{
				cut = 1;
				break;
			}
		}
		if(cut == 0)
		{
			tmp += str[i];
			//tmp_index ++;
		}
		else
		{
			//int k;
			//for(k = 0; k < tmp_index; k++)
			//	res[res_index][k] = tmp[k];
			if(tmp=="")
				break;
			res.push_back(tmp);
			tmp = "";
			//res[res_index][k] = '\0';
			//res_index ++;
			//tmp_index = 0;
		}
	}
	return res;
}

int clear_buff(char* c, int size)
{
	if (size == 0) size = MAX_STR_SIZE;
	int i;
	for(i = 0; i < size; i++)
	{
		c[i] = 0;
	}
}

void print_myvector(vector<string> v)
{
	for(int i=0; i<v.size(); i++)
		cout<<i+1<<": "<<v[i]<<endl;
}

void sugg_comm_to_voter()
{
	string sugg = "\nThese are the valid commands:\n\nConnect\t\t->\tConnect to Server\nDC\t\t->\tDisconnect from the server\nIntroduction\t->\tIntroduce new customer to server\nGet Status\t->\tGets latest customer status from server\nUnregister\t->\tUnregister customer\nExit\t\t->\tExit the Program\n";
	cout<<sugg<<endl;
}

void sugg_comm_to_server()
{

	cout<<"\nThese are the valid commands:\n"<<endl;
	cout<<"Connect\t\t->\tConnect to Server\nDC\t\t->\tDisconnect from the server\nIntroduction\t->\tIntroduce new company to server\nGet Status\t->\tGets latest company status from server\nExit\t\t->\tExit the Program\n"<<endl;
}

int show_candidates_name(char res[MAX_STR_SIZE])
{
	int open_fd = open("./DB/candidates_names.txt", O_RDONLY);
	if(open_fd < 0)
		write(STDOUTFD, "can't open available ports file", sizeof("can't open available ports file"));
	return read(open_fd, res, MAX_STR_SIZE-1);
}

vector<string> read_available_unames()
{
	int open_fd = open("./DB/voters.txt", O_RDONLY);
	if(open_fd < 0)
		write(STDOUTFD, "can't open available ports file", sizeof("can't open available ports file"));
	char read_buff[MAX_STR_SIZE];
	clear_buff(read_buff, MAX_STR_SIZE);
	int r_st = read(open_fd, read_buff, MAX_STR_SIZE-1);
	vector<string> unames = mytokenizer(read_buff, "\n");
	return unames;
}

int save_vote_comm(string uname)
{
	if(found_in(uname, read_available_unames()) < 0)
	{
		int fd_to_save_uname = open("./DB/voters.txt", O_APPEND | O_RDWR);
		cerr<<"uname is: "<<uname<< " in " <<uname.length()<<endl;
		char* cstr = new char [uname.length()+1];
		strcpy(cstr, uname.c_str());
		int dumm = write(fd_to_save_uname, cstr, strlen(cstr));
		dumm = write(fd_to_save_uname, "\n", strlen("\n"));
		if(dumm <0)
		write(STDOUTFD, "Error while writing port\n", sizeof("Error while writing port\n")); 
		close(fd_to_save_uname);
		return dumm;
	}
	else
	{
		cout<<"Any voter can't vote more than once"<<endl;
		return -1;
	}
}

int voting(vector<string> v)
{
	char fpath[MAX_STR_SIZE];
	clear_buff(fpath, MAX_STR_SIZE);
	strcpy(fpath, "./DB/candidates/");
	strcat(fpath, v[2].c_str());
	cerr<<"\n\t ### fpath is: "<<fpath<< " in " <<strlen(fpath)<<endl;
	int fd_to_save_vote = open(fpath, O_RDWR);
	if(fd_to_save_vote < 0)
	{
			write(STDOUTFD, "can't open available ports file", sizeof("can't open available ports file"));
			return -1;
	}
	
	char read_buff[MAX_STR_SIZE];
	clear_buff(read_buff, MAX_STR_SIZE);
	int r_st = read(fd_to_save_vote, read_buff, MAX_STR_SIZE-1);
	vector<string> data = mytokenizer(read_buff, "\n");
	
	int tedaderay = atoi(data[1].c_str());
	tedaderay ++;
	
	string Result = static_cast<ostringstream*>( &(ostringstream() << tedaderay) )->str();

/*
	char hichi[MAX_STR_SIZE];
	clear_buff(hichi, MAX_STR_SIZE);
	int dumm = write(fd_to_save_vote, hichi, strlen(hichi));
	close(fd_to_save_vote);
	fd_to_save_vote = open(fpath, O_APPEND | O_RDWR);
	if(fd_to_save_vote < 0)
	{
			write(STDOUTFD, "can't open available ports file", sizeof("can't open available ports file"));
			return -1;
	}*/

	char chi[MAX_STR_SIZE];
	clear_buff(chi, MAX_STR_SIZE);
	strcpy(chi, data[0].c_str());
	strcat(chi, "\n");
	strcat(chi, Result.c_str());
	close(fd_to_save_vote);

	fstream f;
	f.open(fpath, std::fstream::out | std::fstream::trunc);
	f<<"";
	f.close();

	fd_to_save_vote = open(fpath, O_RDWR);
	if(fd_to_save_vote < 0)
	{
			write(STDOUTFD, "can't open available ports file", sizeof("can't open available ports file"));
			return -1;
	}

	cerr<<"fpath is: "<<fpath<< " in " <<strlen(fpath)<<endl;
	//char* cstr = new char [uname.length()+1];
	//strcpy(cstr, uname.c_str());
	// char hichi[MAX_STR_SIZE];
	// clear_buff(hichi, MAX_STR_SIZE);
	// dumm = write(fd_to_save_vote, hichi, strlen(hichi));
	int dumm = write(fd_to_save_vote, chi, strlen(chi));
	dumm = write(fd_to_save_vote, "\n", strlen("\n"));
	if(dumm <0)
	write(STDOUTFD, "Error while writing port\n", sizeof("Error while writing port\n")); 
	close(fd_to_save_vote);
	return dumm;
}

bool in_time(my_time start_time, my_time stop_time)
{
	time_t my_time = time(NULL);
	my_time += 3 * 3600 + 30 * 60;
	int second = my_time % 60;
	int minute = (my_time % 3600) / 60;
	int hour = (my_time % (3600 * 24)) / 3600;

	if(hour>start_time.hour && hour<stop_time.hour)
		return true;
	else if(hour==start_time.hour && minute>=start_time.min && hour<stop_time.hour)
		return true;
	else if(hour>start_time.hour && hour==stop_time.hour && minute<=stop_time.min)
		return true;
	else
		false;
}
/*int getdir (vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir("./DB/Candidates/")) == NULL) {
        cout << "Error(" << errno << ") opening $$$$$$$$$$$$$$$$$$$$$$$$$" << "./DB/Candidates" << endl;
        return errno;
    }
cerr<<"\n dar baz kardane folder ok bude\n\n"<<endl;
    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}*/