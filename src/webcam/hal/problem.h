#ifndef PROBLEM_H
#define PROBLEM_H

#include <iostream>
#include <vector>
#include "img.h"

using namespace std;

	struct question
	{
		int q;
		int a;
	};

	class Problem
	{
		public:
			Problem();
			~Problem();
			
// 			void copyTo(Problem& p);

// 			vector<float> image;
			Img image;
// 			vector<question> questions;
			
			question currentquestion;

		private:
	};

#endif
