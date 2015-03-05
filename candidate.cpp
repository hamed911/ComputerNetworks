#include "candidate.h"
#include <errno.h>

Candidate::Candidate(string n,int id){
	name = n;
	code = id;
	voteNumber = 0;
}

bool Candidate::saveCandidate(){
	string path = "./DB/candidates/";
	ostringstream convert;
	convert<<code;
	path+=convert.str();
	cerr << "path is :"<< path << endl;
	int fd = open(path.c_str(),O_WRONLY|O_RDONLY|O_CREAT);

    if(-1 == fd)
    {
        printf("\n open() failed with error [%s]\n",strerror(errno));
        return 1;
    }
    else
    {
    	chmod(path.c_str(), S_IRUSR | S_IWUSR );
        write(fd,name.c_str() ,strlen(name.c_str()));
        write(fd,"\n",sizeof("\n"));
    	close(fd);

    	ofstream file(path.c_str());
		if(file.is_open()){
			file <<name << endl;
			file << voteNumber << endl;
			file.close();	
		}
		else{
			cerr << "Error in saving candidate " << name << endl;
			return false;
		}
		cout<< "candidate "<< name << " added successfully!" << endl;
    
    }
    
    int fd_to_save_cname = open("./DB/candidates_names.txt", O_APPEND | O_RDWR);
	cerr<<"uname is: "<<name<< " in " <<name.length()<<endl;
	char* cstr = new char [name.length()+1];
	strcpy(cstr, name.c_str());
	int dumm = write(fd_to_save_cname, cstr, strlen(cstr));
	dumm = write(fd_to_save_cname, "\n", strlen("\n"));
	if(dumm <0)
	write(STDOUTFD, "Error while writing port\n", sizeof("Error while writing port\n")); 
	close(fd_to_save_cname);
	

	/*if(file_exist(path.c_str()))
		return false;
	ofstream file(path.c_str());
	if(file.is_open()){
		file <<name << endl;
		file << voteNumber << endl;
		file.close();	
		cerr << "candidate "<< name <<" saved successfully!" << endl;
	}
	else{
		cerr << "Error in saving candidate " << name << endl;
		return false;
	}*/
	return true;
}
void Candidate::setVoteNumber(int d){
	voteNumber = d;
}
void Candidate::setCode(int i){
	code = i;
}
void Candidate::setName(string n){
	name =n;
} 