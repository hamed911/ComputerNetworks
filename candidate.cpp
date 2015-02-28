#include "candidate.h"

Candidate::Candidate(string n,int id){
	name = n;
	code = id;
	voteNumber = 0;
}
void Candidate::saveCandidate(){
	string path = "./candidate/"+code;
	ofstream file;
	file.open(path.c_str(), ios::out | ios::trunc);
	if(file.is_open()){
		file <<name << endl;
		file << voteNumber << endl;
		file.close();	
		cerr << "candidate "<< name <<" saved successfully!" << endl;
	}
	else
		cerr << "Error in saving candidate " << name << endl;
	
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