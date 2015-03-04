#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <errno.h>
#include "utilities.h"
#include "cryptogram.h"

int process_voter_comm(vector<string> tokens, int num_of_tokens, char res[MAX_STR_SIZE], char* dirnm)
{
	return 0;
}
int process_server_comm(vector<string> tokens, int num_of_tokens, char res[MAX_STR_SIZE], char* dirnm)
{
	return 0;
}
int process_command(char command[MAX_STR_SIZE], char res[MAX_STR_SIZE], char* dirnm)
{
	int num_of_tokens;
	//char tokens[MAX_ARRAY_SIZE][MAX_STR_SIZE];
	vector<string> tokens = mytokenizer(command, " \n");
	cerr<<tokens[0]<<" DD voter/server DD"<<endl;
	//if(num_of_tokens < 0)
		//return -1;
	if(tokens[0] == "server"){
		cerr<<"FAHMIDE SERVER E"<<endl;
		return process_server_comm(tokens, num_of_tokens, res, dirnm);
	}
	else if(tokens[0] == "voter"){
		cerr<<"fahmide vvvvvoter e"<<endl;
		return process_voter_comm(tokens, num_of_tokens, res, dirnm);
	}
	return -1;
}

/*int process_command(char command[MAX_STR_SIZE], char res[MAX_STR_SIZE], char* dirnm)
{
	return 0;
}*/

int main(int argn, char** args)
{
	if(argn < 2){
		cout<<"Valid format is :\n./validator port_number"<<endl;
		return 0;
	}
	int port_number = atoi(args[1]);
	//save_ca_port(args[1].c_str());
	if(found_in(args[1], read_available_ca_ports()) < 0)
	{
			int fd_to_save_port = open("./DB/validators.txt", O_APPEND | O_RDWR);
			cerr<<"args[1] is: "<<args[1]<< " in " <<strlen(args[1])<<endl;
			int dumm = write(fd_to_save_port, args[1], strlen(args[1]));
			dumm = write(fd_to_save_port, "\n", strlen("\n"));
			if(dumm <0)
					write(STDOUTFD, "Error while writing port\n", sizeof("Error while writing port\n")); 
			close(fd_to_save_port);
	}

	const int num_of_connection = 5;
	
	// make directories
	char directory_name[] = "DB";
	int mkdir_status = create_directories(directory_name);
	if(mkdir_status != 0){
		cout<<"Error while creating directory"<<endl;
		return 0;
	}
	cout<<"Directory created"<<endl;
	
	//creating socket
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
		//exit
	}
	else 	
		write(STDOUTFD, "binding successful\n", sizeof("binding successful\n"));
	//listenning
	int listening_st = listen(server_fd, num_of_connection);
	if(listening_st == -1)
		write(STDOUTFD, "listening error\n", sizeof("listening error"));
	else
		write(STDOUTFD, "listening successful\n", sizeof("listening successful"));

	fd_set read_fdset, temp_fdset;
	struct timeval tv;
	int ret_val;
	int new_sock_fd, it_fd;

	/* Watch stdin (fd 0) to see when it has input. */
	FD_ZERO(&read_fdset);
	FD_SET(server_fd, &read_fdset);
	FD_SET(STDINFD, &read_fdset);

	/* Wait up to five seconds. */
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

					read(it_fd, buff_read, MAX_STR_SIZE-1);
					//process_server_command(buff_read, response_buff, directory_name);
					write(STDOUTFD, buff_read, MAX_STR_SIZE);
					write(STDOUTFD, "OK", strlen("OK"));
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
							if( strncmp (buff_read,"Register",8)==0 ){
								char publicPath[MAX_STR_SIZE];
								clear_buff(publicPath,MAX_STR_SIZE);						
								strcat( publicPath,"./DB/");
								struct stat st;
								if (stat(publicPath, &st) == -1) {
								    mkdir(publicPath, 0700);
								}
								strcat( publicPath,"certificate/");
								if (stat(publicPath, &st) == -1) {
								    mkdir(publicPath, 0700);
								}
								vector<string> rcommand = mytokenizer(buff_read," ");
								////open file
								string s = "./DB/certificate/"+ rcommand[1];
								int fd_c = creat(s.c_str(),O_RDWR| O_APPEND);
								if(fd_c > 0){
									chmod(s.c_str(),S_IRUSR | S_IWUSR);
								}
								for(int i=0; i< rcommand.size(); i++)
									cerr << rcommand[i]  << ' ';
								cerr << endl;
								//open file

							}
							int s = write(it_fd, "ok", strlen("ok"));
							if(s < 0) write(STDOUTFD, "Error on writing\n", sizeof("Error on writing\n"));
						}
						else //if(buff_read == "DC")
						{
							cerr<<"varede dasture DC DC DC shode"<<endl;
							
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
	return 0;
}

/*
void set_compn_data(char* path_name, company* this_company)
{
	int op_fd = open(path_name, O_RDWR | O_APPEND);
	if(op_fd < 0) write(STDOUTFD, "Error In Opening File to Read Data!\n", sizeof("Error In Opening File to Read Data!\n"));

	int path_tkn_num;
	//char path_tkns[MAX_ARRAY_SIZE][MAX_STR_SIZE];
	vector<string> pathtkns = mytokenizer(path_name, "./ \n");
	strcpy(this_company->name, path_tkns[path_tkn_num - 2]);

	char read_buff[MAX_STR_SIZE];
	clear_buff(read_buff, MAX_STR_SIZE);
	int rd_st = read(op_fd, read_buff, MAX_STR_SIZE-1);


	if(rd_st > 0)
	{
		int tkn_num;
		char tkns[MAX_ARRAY_SIZE][MAX_STR_SIZE];
		tokenizer(read_buff, " \n", &tkn_num, tkns);

		char tmp_stcknum[MAX_ARRAY_SIZE];
		clear_buff(tmp_stcknum, MAX_ARRAY_SIZE);
		strcpy(tmp_stcknum, tkns[3]);
		this_company->stocks_number = atoi(tmp_stcknum);//set number of company's stocks



		clear_buff(tmp_stcknum, MAX_ARRAY_SIZE);
		strncpy(tmp_stcknum, tkns[6], strlength(tkns[6])-1);
		this_company->price_of_stock = atoi(tmp_stcknum);//set price of each stock


		this_company->budget = (this_company->stocks_number)*(this_company->price_of_stock);

		int i = 7;
		this_company->invstr_num = 0;
		while(i<tkn_num && strlength(tkns[i])!=0)
		{
			strcpy(this_company->investors[(i-7)/6], tkns[i]);
			i++; i++;//passing bought
			this_company->investors_stocks_num[(i-7)/6] = atoi(tkns[i]);
			i++; i++; i++;//passing stocks on
			strcpy(this_company->investing_date[(i-7)/6], tkns[i]);
			i++;
			this_company->invstr_num++;
		}
	}
	close(op_fd);
}*/

/*void record_company_data(char path_name[], company this_company)
{
	int file_fd = open(path_name, O_RDWR);
	if(file_fd < 0) write(STDOUTFD, "Error In Opening File to Write!\n", sizeof("Error In Opening File to Write!\n"));

	int w_st = write(file_fd, "Main stocks Num ", sizeof("Main stocks Num "));
	char strtmp[20];
	clear_buff(strtmp, 20);
	int_to_str(this_company.stocks_number, strtmp, 10);
	write(file_fd, strtmp, strlength(strtmp));
	write(file_fd, "\n", sizeof("\n"));

	write(file_fd, "stocks Price ", sizeof("stocks Price "));
	clear_buff(strtmp, 20);
	int_to_str(this_company.price_of_stock, strtmp, 10);
	write(file_fd, strtmp, strlength(strtmp));
	write(file_fd, "$\n", sizeof("$\n"));

	int i;
	for(i=0; i<this_company.invstr_num; i++)
	{
		write(file_fd, this_company.investors[i], strlength(this_company.investors[i]));//investor name
		write(file_fd, " bought ", sizeof(" bought "));
		clear_buff(strtmp, 20);
		int_to_str(this_company.investors_stocks_num[i], strtmp, 10);
		write(file_fd, strtmp, strlength(strtmp));//number of stocks
		write(file_fd, " stocks on ", sizeof(" stocks on "));
		write(file_fd, this_company.investing_date[i], strlength(this_company.investing_date[i]));//date
		write(file_fd, "\n", sizeof("\n"));
	}
	close(file_fd);
}*/

/*		client* this_customer, char res[MAX_STR_SIZE], char* path_name)
{
	clear_buff(res, MAX_STR_SIZE);
	if(num_of_tokens >= 6 && mystrcmp(tokens[2], "Introduction") == 0 && mystrcmp(tokens[3], "Customer") == 0)
	{
		int cr_fd = creat(path_name, O_RDWR | O_APPEND);
		if(cr_fd > 0)
		{
			chmod(path_name,S_IRUSR | S_IWUSR);
			char w_buff[MAX_STR_SIZE];
			clear_buff(w_buff, MAX_STR_SIZE);
			strcpy(w_buff , "Main budget ");
			strcat(w_buff, tokens[5]);
			strcat(w_buff, "$\n");
			int w_st = write(cr_fd, w_buff, strlength(w_buff));
			if(w_st < 0)
			{
				write(STDOUTFD, "Error in Writing File!\n", sizeof("Error in Writing File!\n"));
				return -1;
			}
			strcpy(res, "You Registered Successfully!\n");
		}
		else 
			return -1;
		close(cr_fd);

	}
	else if( num_of_tokens >= 5 && mystrcmp(tokens[2], "Get") == 0 && 
			mystrcmp(tokens[3], "Stocks") == 0 && mystrcmp(tokens[4], "List") == 0)
	{
		int file_num;
		char file_names[MAX_ARRAY_SIZE][MAX_STR_SIZE];
		char sources_path[MAX_STR_SIZE];

		clear_buff(sources_path, MAX_STR_SIZE);
		char old_path_tokens[MAX_ARRAY_SIZE][MAX_STR_SIZE];
		int old_path_tokens_num;
		tokenizer(path_name, "/", &old_path_tokens_num, old_path_tokens);	

		strcpy(sources_path, old_path_tokens[0]);
		strcat(sources_path, "/Company/");
		print(sources_path);
		//list_files(sources_path, file_names, &file_num);
		char date_p[30];
		clear_buff(date_p, 30);
		get_date(date_p);

		strcpy(res, "------------Stocks List-------------\nTime:");
		strcat(res, date_p);
		strcat(res, "\nCompany---StocksNum-Price\n");
		int fd, nread;
		char buf[MAX_STR_SIZE];
		struct linux_dirent *d;
		int bpos;
		char d_type;
		fd = open(sources_path, O_RDONLY | O_DIRECTORY);
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
				if(mystrcmp(d->d_name, ".") != 0 && mystrcmp(d->d_name, "..")){
					company this_cmp;
					char cmp_path[MAX_STR_SIZE];
					clear_buff(cmp_path, MAX_STR_SIZE);
					strcpy(cmp_path, sources_path);
					strcat(cmp_path, d->d_name);

					set_compn_data(cmp_path, &this_cmp);			

					strcat(res, this_cmp.name);
					strcat(res, "\t");
					char stcks [MAX_STR_SIZE];
					int_to_str(this_cmp.stocks_number, stcks, 10);
					strcat(res, stcks);
					strcat(res, "\t");
					int_to_str(this_cmp.price_of_stock, stcks, 10);
					strcat(res, stcks);
					strcat(res, "\n");

				}	
				bpos += d->d_reclen;
			}
			break;

		}


	}
	else if(num_of_tokens >= 5 && mystrcmp(tokens[2], "Buy") == 0 && this_customer != NULL)
	{
		return 1;
	}
	else if(num_of_tokens >= 4 && mystrcmp(tokens[2], "Get") == 0 && mystrcmp(tokens[3], "Status") == 0
			&& this_customer != NULL)
	{
		char date_p[30];
		clear_buff(date_p, 30);
		get_date(date_p);
		strcpy(res, "---------status---------\nTime : ");
		strcat(res, date_p);
		strcat(res, "\n");
		strcat(res, "Budget : ");
		char budget_str[20];
		clear_buff(budget_str, 20);
		int_to_str(this_customer->budget, budget_str, 10);
		strcat(res, budget_str);
		strcat(res, "$\n");
		strcat(res, "Company---Stocks Num\n");
		int i;
		for(i = 0; i < this_customer -> invstd_num; i++){
			strcat(res, this_customer -> invested[i]);
			strcat(res, " ");
			char tmp[20];
			clear_buff(tmp, 20);
			int_to_str(this_customer -> invested_stocks_num[i], tmp, 10);
			strcat(res, tmp);
			strcat(res, " stocks on ");
			strcat(res, this_customer -> invested_date[i]);
			strcat(res, "\n");
		}
		strcat(res, "\0");
	}
	else if(num_of_tokens >= 3 && mystrcmp(tokens[2], "Unregister") == 0 && this_customer != NULL)
	{
		unlink(path_name);
		strcpy(res, "You Are Unregistered!\n");
	}
	else if(num_of_tokens >= 3 && mystrcmp(tokens[2], "DC") == 0)
	{
	}
	else
		return -1;
	return 0;
}*/

/*int record_customer_data(char path_name[], client this_customer)
{
	int file_fd = open(path_name, O_RDWR);
	if(file_fd < 0) write(STDOUTFD, "Error In Opening File to Write!\n", sizeof("Error In Opening File to Write!\n"));
	int w_st = write(file_fd, "Main budget ", sizeof("Main budget "));
	char strtmp[20];
	clear_buff(strtmp, 20);
	int_to_str(this_customer.budget, strtmp, 10);
	write(file_fd, strtmp, strlength(strtmp));
	write(file_fd, "$\n", sizeof("$\n"));
	int i;
	for(i=0; i<this_customer.invstd_num; i++)
	{
		write(file_fd, this_customer.invested[i], strlength(this_customer.invested[i]));//company name
		write(file_fd, " ", sizeof(" "));
		clear_buff(strtmp, 20);
		int_to_str(this_customer.invested_stocks_num[i], strtmp,10);
		write(file_fd, strtmp, strlength(strtmp));//number of stocks
		write(file_fd, this_customer.invested_date[i], strlength(this_customer.invested_date[i]));//date
		write(file_fd, "\nbudget ", sizeof("\nbudget "));
		clear_buff(strtmp, 20);
		int_to_str(this_customer.remained_budget[i], strtmp, 10);
		write(file_fd, strtmp, strlength(strtmp));//remained budget after buying above stocks
		write(file_fd, "$\n", sizeof("$\n"));
	}
	close(file_fd);
	return 0;
}*/

/*int process_customer_comm(char tokens[MAX_ARRAY_SIZE][MAX_STR_SIZE], int num_of_tokens, char res[MAX_STR_SIZE], char* dirnm){
	client this_customer;

	strcpy(this_customer.name, tokens[1]);
	char path_name[MAX_STR_SIZE];
	clear_buff(path_name, MAX_STR_SIZE);
	strcpy(path_name, dirnm);
	strcat(path_name, "/Customer/");
	strcat(path_name, tokens[1]);
	strcat(path_name, ".txt");

	int file_fd = open(path_name, O_RDWR | O_APPEND);
	if(file_fd < 0)
	{
		return (customer_response(tokens, num_of_tokens, NULL , res, path_name));
	}
	else
	{
		char read_buff[MAX_STR_SIZE];
		clear_buff(read_buff, MAX_STR_SIZE);
		int r_st = read(file_fd, read_buff, MAX_STR_SIZE-1);

		//tokenize read_file_lines
		if(r_st > 0)
		{
			int tkn_num;
			char tkns[MAX_ARRAY_SIZE][MAX_STR_SIZE];
			tokenizer(read_buff, " \n", &tkn_num, tkns);

			char tmp_budget[MAX_ARRAY_SIZE];
			clear_buff(tmp_budget, MAX_ARRAY_SIZE);
			strncpy(tmp_budget, tkns[2], strlength(tkns[2])-1);
			this_customer.budget = atoi(tmp_budget);

			int i = 3;
			this_customer.invstd_num = 0;
			while(i < tkn_num && strlength(tkns[i]) != 0)
			{
				strcpy(this_customer.invested[(i-3)/7], tkns[i]);//company name
				print("company name:");
				print (tkns[i]);
				i++;
				this_customer.invested_stocks_num[(i-3)/7] = atoi(tkns[i]);//num of stocks
				print("\n num of stocks:");
				print (tkns[i]);
				i++; i++; i++;// for passing stocks on
				strcpy(this_customer.invested_date[(i-3)/7], tkns[i]);//date
				print("\n date:");
				print (tkns[i]);
				i++; i++;//passing budget

				char tmp_budget[MAX_ARRAY_SIZE];
				clear_buff(tmp_budget, MAX_ARRAY_SIZE);
				strncpy(tmp_budget, tkns[i], strlength(tkns[i])-1);
				this_customer.remained_budget[(i-3)/7] = atoi(tmp_budget);
				print("\n budget:");
				print (tkns[i]);
				i++;
				this_customer.invstd_num++;
			}
			close(file_fd);
			int response_stat = customer_response(tokens, tkn_num, &this_customer, res, path_name);
			if(response_stat == 1)
				record_customer_data(path_name, this_customer);
			return response_stat; 
		}
		else
			close(file_fd);
	}

	return -1;
}*/

/*int company_response(char tokens[MAX_ARRAY_SIZE][MAX_STR_SIZE], int num_of_tokens,
		company* this_company, char res[MAX_STR_SIZE], char* path_name)
{		
	clear_buff(res, MAX_STR_SIZE);
	if(num_of_tokens >= 7 && mystrcmp(tokens[2], "Introduction") == 0 && mystrcmp(tokens[3], "Company") == 0)
	{
		int cr_fd = creat(path_name, O_RDWR | O_APPEND);
		if(cr_fd > 0)
		{
			chmod(path_name, S_IRUSR | S_IWUSR );
			char w_buff[MAX_STR_SIZE];
			clear_buff(w_buff, MAX_STR_SIZE);
			strcpy(w_buff, "Main stocks Num ");
			strcat(w_buff, tokens[5]);
			strcat(w_buff, "\n");
			strcat(w_buff, "stocks Price ");
			strcat(w_buff, tokens[6]);
			strcat(w_buff, "$\n");
			int w_st = write(cr_fd, w_buff, strlength(w_buff));
			if(w_st < 0) 
			{
				write(STDOUTFD, "Error in Writing File!\n", sizeof("Error in Writing File!\n"));
				return -1;
			}
			strcpy(res, "You Registered Successfully!\n");
		}
		else 
			return -1;
	}
	else if(num_of_tokens >= 4 && mystrcmp(tokens[2], "Get") == 0 && mystrcmp(tokens[3], "Status") == 0
			&& this_company != NULL)
	{
		clear_buff(res, MAX_ARRAY_SIZE);

		char date_p[30];
		clear_buff(date_p, 30);
		get_date(date_p);//get date

		strcpy(res, "------------Company Status -------------\nCompanyName : ");
		strcat(res, this_company->name);
		strcat(res, "\nStatus Time : ");
		strcat(res, date_p);
		strcat(res, "\nBudget ");

		char tmp_str[20];
		clear_buff(tmp_str, 20);
		int_to_str(this_company->budget, tmp_str, 10);

		strcat(res, tmp_str);
		strcat(res, "$\nPrice of each stock : ");

		clear_buff(tmp_str, 20);
		int_to_str(this_company->price_of_stock,tmp_str, 10);

		strcat(res, tmp_str);
		strcat(res, "$\nRemaining stocks num : ");

		clear_buff(tmp_str, 20);
		int_to_str(this_company->stocks_number, tmp_str, 10);

		strcat(res, tmp_str);
		strcat(res, "\n");

		int i;
		for(i=0; i<this_company->invstr_num; i++)
		{
			strcat(res, this_company->investors[i]);
			strcat(res, " bought ");

			clear_buff(tmp_str, 20);
			int_to_str(this_company->investors_stocks_num[i], tmp_str, 10);

			strcat(res, tmp_str);
			strcat(res, " stocks on ");
			strcat(res, this_company->investing_date[i]);
			strcat(res, "\n");
		}
		strcat(res, "\0");
	}
	else
		return -1;
	return 0;
}*/

/*int process_company_comm(char tokens[MAX_ARRAY_SIZE][MAX_STR_SIZE], int num_of_tokens, char res[MAX_STR_SIZE], char* dirnm)
{
	company this_company;

	strcpy(this_company.name, tokens[1]);
	char path_name[MAX_STR_SIZE];
	clear_buff(path_name, MAX_STR_SIZE);
	strcpy(path_name, dirnm);
	strcat(path_name, "/Company/");
	strcat(path_name, tokens[1]);
	strcat(path_name, ".txt");

	int file_fd = open(path_name, O_RDWR | O_APPEND);
	if(file_fd < 0)
	{
		return company_response(tokens, num_of_tokens, NULL, res, path_name);
	}
	else
	{
		set_compn_data(path_name, &this_company);
		return company_response(tokens, num_of_tokens, &this_company, res, path_name);
	}
}*/
/*int process_command(char command[MAX_STR_SIZE], char res[MAX_STR_SIZE], char* dirnm)
{
	int num_of_tokens;
	char tokens[MAX_ARRAY_SIZE][MAX_STR_SIZE];
	tokenizer(command, " \n", &num_of_tokens, tokens);
	if(num_of_tokens < 0)
		return -1;
	if(mystrcmp(tokens[0], "company") == 0)
		return process_company_comm(tokens, num_of_tokens, res, dirnm);
	else if(mystrcmp(tokens[0], "customer") == 0)
		return process_customer_comm(tokens, num_of_tokens, res, dirnm);
	return -1;
}*/

/*int process_server_command(char buff_read[MAX_STR_SIZE], char response_buff[MAX_STR_SIZE], char* dirnm){
	int num_of_tokens;
	clear_buff(response_buff, MAX_STR_SIZE);
	char tokens[MAX_ARRAY_SIZE][MAX_STR_SIZE];
	tokenizer(buff_read, " \n", &num_of_tokens, tokens);
	if(num_of_tokens < 4)
		return -1;
	char pathname[MAX_STR_SIZE];
	clear_buff(pathname, MAX_STR_SIZE);
	strcpy(pathname, dirnm);
	strcat(pathname, "/");
	strcat(pathname, tokens[2]);
	strcat(pathname, "/");
	strcat(pathname, tokens[3]);
	strcat(pathname, ".txt");
	if(mystrcmp(tokens[0],"Change") == 0 && mystrcmp(tokens[1], "FileOwner") == 0){
		if(chmod(pathname, S_IRUSR | S_IWUSR | S_IWOTH | S_IROTH) != 0) strcpy(response_buff, "File address Incorrect\n");
		else strcpy(response_buff, "File mode changed\n");
	}

	else if(mystrcmp(tokens[0],"Show") == 0 && mystrcmp(tokens[1], "FileOwner") == 0){
		struct stat sb;
		 if (stat(pathname, &sb) == -1)  strcpy(response_buff, "Stat can not be recognized\n");
		 else
		 {
			 char modestr[MAX_STR_SIZE] ;
			 int_to_str(sb.st_mode, modestr, 10);
			 strcpy(response_buff, modestr);
		 }
	}
	return 0;
}*/
