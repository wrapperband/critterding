#ifndef ANSWERS_H
#define ANSWERS_H

#include <iostream>
#include "problem.h"
#include "src/utils/parser.h"
#include "src/utils/file.h"
#include "src/utils/timer.h"
#include "src/utils/dirlayout.h"

using namespace std;


struct diskquestion
{
	string name;
	int id;
	string question;
};

struct diskanswer
{
	int q;
	int a;
	int img;
};

class Answers
{
	public:
		static Answers* Instance();

		vector<diskquestion> questions;
		vector<diskanswer> answers;

// 		void add(Problem* p);
		void save(Problem* p);

// 		vector<Problem> list;

		void loadQuestionsFromDisk();
		void loadAnswersFromDisk();

		pthread_mutex_t		answers_m;
	protected:
		Answers();
		~Answers();
	private:
		static Answers* _instance;

		Parser*			parseH;
		Dirlayout*		dirlayout;
		File			fileH;
		Dir			dirH;
};

#endif
