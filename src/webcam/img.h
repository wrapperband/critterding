#ifndef IMG_H
#define IMG_H

#include <vector>
#include <iostream>
// #include "src/utils/file.h"
// #include "src/utils/dirlayout.h"
// #include "src/utils/parser.h"

using namespace std;

	class Img
	{
		public:
			Img();
			~Img();

			vector<float> v;
			void copyTo( vector<float>& img );
			void copyTo( Img& img );
// 			void loadFromFile( int imgid );
		private:
// 			Dirlayout*	dirlayout;
// 			File		fileH;
// 			Parser*		parseH;
	};

#endif
