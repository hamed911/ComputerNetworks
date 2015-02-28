#ifndef CANDIDATE_H_
#define CANDIDATE_H_

#include "utilities.h"

class Candidate{
private:
	string name;
	int code;
	int voteNumber;
public:
	Candidate(string , int);
	void saveCandidate();
	void setVoteNumber(int);
	void setCode(int);
	void setName(string);
};

#endif
