#include "utilities.h"

vector<string> read_available_ports()
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

int found_in(string str, vector<string> arr)
{
	for(int i=0; i<arr.size(); i++)
	{
		if(str == arr[i])
			return i;
	}
	return -1;
}
bool checkClientCommandValidation(vector<string> args,char* name){
	if(args.size()<=0)
		return false;
	if(args[0]=="Connect"){
		if(args.size()!=4){
			cout <<"insufficient number of argument!\tcorrect form=>\tConnect Server 2000 " << name << endl;
			return false;
		}
		else
			return true;
	}else if(args[0]=="Register"){
		if(args.size()!=5){
			cout <<"insufficient number of argument!\tcorrect form=>\tRegister 123456789 " << name << " ***** *****" << endl;
			return false;
		}else{
			if(args[3]!=args[4]){
				cout << "Passwords are not the same!" << endl;
				return false;
			}
		}
		
	}
	return true;
}
void showClientCommands(){
	cout<<"\nThese are the valid commands:\n\n";
	char comm[][MAX_STR_SIZE] = 
	{ "Connect\t\t->\tConnect voter to Server\n"
		, "Disconnect\t\t->\tDisconnect from the server by admin\n"
			, "Register\t->\tRegister yourself to certificate authority\n"
			, "Get Stocks List\t->\tgGets latest stocks list\n"
			, "Vote\t\t->\tVote to specific candidate\n"
			, "ShowCandidates\t->\tShow all the available candidates\n"
			, "SetVotingTime\t->\tset voting time\n"
			, "ExtendVotingTime\t->\textendVotingTime\n"
			, "ShowServers\t->\tShow server port\n"
			, "ShowCA\t->\tShow CA's port\n"
			, "Exit\t\t->\tExit the Program\n\n"};
	for(int p = 0; p < 8; p++)
		cout<< comm[p];
}

bool writeLine(string fileName,int port){
	ofstream file;
	file.open(fileName.c_str(),ios::app|ios::out);
	if(!file.is_open())
		return false;
	file << port << endl;
	file.close();
	return true;
}
/*int strlength(char str[MAX_STR_SIZE])
{
	int i;
	for(i = 0; i < GETMAX(MAX_STR_SIZE, sizeof(str)); i++)
	{
		if(str[i] == '\0') break;
	}
	return i;
}*/
/*void int_to_str(int val, char res[20], int base)
{
	clear_buff(res, 20);
	char tmp[20];
	clear_buff(tmp, 20);
	if(val == 0)
	{
		strcpy(res, "0");
		return;
	}
	int index = 0;
	while (val > 0)
	{
		char digit = '0';
		digit = (char) ((val % base) + (int) digit);
		tmp[index] = digit;
		val /= base;
		index ++;
	}
	tmp = tmp + "\0";
	int i;
	int size = strlength(tmp);

	for(i = 0; i<size; i++)
	{
		res[i] = tmp[size-1-i];
	}
	strcat(res, "\0");
}*/
/*void print(char str[MAX_STR_SIZE])
{
	write(STDOUTFD, str, strlength(str));
}*/
/*void tokenizer(char str[MAX_STR_SIZE], char delim[MAX_STR_SIZE], int* num_of_tokens, char res[MAX_STR_SIZE][MAX_STR_SIZE])
{
	char tmp[MAX_STR_SIZE];
	int i, tmp_index = 0, res_index = 0;
	for(i = 0; i <= strlength(str); i++)
	{
		int cut = 0;
		int j;
		for(j = 0; j < strlength(delim); j++)
		{
			if (str[i] == delim[j] || str[i] == '\0' )
			{
				cut = 1;
				break;
			}
		}
		if(cut == 0)
		{
			tmp[tmp_index] = str[i];
			tmp_index ++;
		}
		else
		{
			int k;
			for(k = 0; k < tmp_index; k++)
				res[res_index][k] = tmp[k];
			res[res_index][k] = '\0';
			res_index ++;
			tmp_index = 0;
		}
	}
	*num_of_tokens = res_index;
}*/
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
			res.push_back(tmp);
			tmp = "";
			//res[res_index][k] = '\0';
			//res_index ++;
			//tmp_index = 0;
		}
	}
	for(int i=0; i<res.size(); i++)
		cout<<res[i]<<"  ";
	cout<<endl;
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
/*int mystrcmp(char const*p, char const *q)
{
	int i = 0;
	for(i=0; q[i]!='\0'; i++)
	{
		if(p[i] != q[i])
			return -1;
	}
	return 0;
}*/

/*int days_in_month(int month, int year)
{
	if(month == 2)
		return 28 + (year % 4 == 0);
	if(month  == 4 || month == 6 || month == 9 || month == 11)
		return 30;
	return 31;
}*/
/*void get_date(char time_str [30])
{
	clear_buff(time_str, 30);

	time_t my_time = time(NULL);
	my_time += 3 * 3600 + 30 * 60;
	int second = my_time % 60;
	int minute = (my_time % 3600) / 60;
	int hour = (my_time % (3600 * 24)) / 3600;
	int days = my_time / (3600 * 24);
	int month = 1, year = 1970;
	while (days > days_in_month(month, year))
	{
		days -= days_in_month(month, year);
		month ++;
		if(month == 13)
		{
			year ++;
			month = 1;
		}
	}

	char tmp[20];
	char str[20];
	clear_buff(tmp, 20);
	clear_buff(str, 20);
	int_to_str(month, tmp, 10);
	if(month < 10)
	{
		strcpy(str, "0");
		strcat(str, tmp);
		clear_buff(tmp, 20);
		strcpy(tmp, str);
	}
	strncpy(time_str, tmp, strlength(tmp));
	strcat(time_str, "/");//month

	clear_buff(tmp, 20);
	clear_buff(str, 20);
	int_to_str(days, tmp, 10);
	if(days < 10)
	{
		strcpy(str, "0");
		strcat(str, tmp);
		clear_buff(tmp, 20);
		strcpy(tmp, str);
	}
	strncat(time_str, tmp, strlength(tmp));
	strcat(time_str, "/");//day

	clear_buff(tmp, 20);
	int_to_str(year, tmp, 10);
	strncat(time_str, tmp, strlength(tmp));
	strcat(time_str, ",");//year

	clear_buff(tmp, 20);
	clear_buff(str, 20);
	int_to_str(hour, tmp, 10);
	if(hour < 10)
	{
		strcpy(str, "0");
		strcat(str, tmp);
		clear_buff(tmp, 20);
		strcpy(tmp, str);
	}
	strncat(time_str, tmp, strlength(tmp));
	strcat(time_str, ":");//hour

	clear_buff(tmp, 20);
	clear_buff(str, 20);
	int_to_str(minute, tmp, 10);
	if(minute < 10)
	{
		strcpy(str, "0");
		strcat(str, tmp);
		clear_buff(tmp, 20);
		strcpy(tmp, str);
	}
	strncat(time_str, tmp, strlength(tmp));
	strcat(time_str, ":");//minute

	clear_buff(tmp, 20);
	clear_buff(str, 20);
	int_to_str(second, tmp, 10);
	if(second < 10)
	{
		strcpy(str, "0");
		strcat(str, tmp);
		clear_buff(tmp, 20);
		strcpy(tmp, str);
	}
	strncat(time_str, tmp, strlength(tmp));//second

	strcat(time_str, "\0");//null at the end of string
}*/
/*void list_files(char pathname[MAX_STR_SIZE], char file_names[MAX_ARRAY_SIZE][MAX_STR_SIZE], int* file_num)
{
	int fd, nread;
	char buf[MAX_STR_SIZE];
	struct linux_dirent *d;
	int bpos;
	char d_type;
	fd = open(pathname, O_RDONLY | O_DIRECTORY);
	print("\nat 1\n");
	int file_num_int = 0;
	while(1)
	{
		nread = syscall(SYS_getdents, fd, buf, MAX_STR_SIZE);
		if (nread == 0){
			break;
		}
		for (bpos = 0; bpos < nread;) 
		{
			d = (struct linux_dirent *) (buf + bpos);
			print(d->d_name);
			print("\n");
			clear_buff(file_names[file_num_int], MAX_STR_SIZE);
			strcpy(file_names[file_num_int], d->d_name);
			bpos += d->d_reclen;
			(file_num_int) ++;
		}
		break;

	}
	*file_num = file_num_int;
}*/
