#include "img.h"

Img::Img()
{
// 	dirlayout = Dirlayout::Instance();
}

void Img::copyTo( vector<float>& img )
{
	img.clear();
	for ( int i = 0; i < v.size(); i++ )
		img.push_back(v[i]);
}

void Img::copyTo( Img& img )
{
	if ( img.v.size() > 0 )
	{
		for ( int i = 0; i < v.size(); i++ )
			img.v[i] = v[i];
	}
	else
	{
		for ( int i = 0; i < v.size(); i++ )
			img.v.push_back(v[i]);
	}
}

// void Img::loadFromFile( int imgid )
// {
// 	stringstream buf;
// 	buf << dirlayout->imagesdir << "/" << imgid << ".img";
// // 	cerr << buf.str() << endl;
// 	
// 	string content;
// 	fileH.open( buf.str().c_str(), content ); 
// 
// 	string n;
// 	unsigned int count = 0;
// 	unsigned int pstartpos = 0;
// 	unsigned int csize = content.size();
// 	unsigned int numb;
// 
// 	while ( pstartpos+1 < csize )
// 	{
// 		size_t stoppos = content.find_first_of( ';', pstartpos );
// 		if ( stoppos != string::npos )
// 			n = content.substr( pstartpos, stoppos-pstartpos );
// 
// 		pstartpos += n.size() + 1;
// 	  
// 		if(EOF == sscanf(n.c_str(), "%u", &numb))
// 			cerr << "error reading number " << n << endl;
// // 		v[count++] = (float)numb/256;
// 		v.push_back( (float)numb/256 );
// 	}
// }

Img::~Img()
{
}
