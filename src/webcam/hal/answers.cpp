#include "answers.h"

Answers* Answers::Instance () 
{
	static Answers t;
	return &t; // _instance isn't needed in this case
}

Answers::Answers()
{
	// init mutexes
	pthread_mutex_init (&answers_m, NULL);
	
	dirlayout = Dirlayout::Instance();
	loadQuestionsFromDisk();
	loadAnswersFromDisk();
}

// void Answers::add(Problem* p)
// {
// // 	pthread_mutex_lock( &answers_m );
// 	// 	cerr << "adding answer" << endl;
// 		list.push_back(Problem());
// 		Problem* np = &list[list.size()-1];
// 		
// 		p->image.copyTo(np->image);
// 		np->questions.clear();
// 		np->questions.push_back(question());
// 		np->questions[0].q = p->questions[0].q;
// 		np->questions[0].a = p->questions[0].a;
// 
// 		cerr << list.size() << "asnwers so far" << endl;
// // 	pthread_mutex_unlock( &answers_m );
// }

void Answers::save(Problem* p)
{
	stringstream imgstr;
	for ( unsigned int i=0; i < p->image.v.size(); i++ )
		imgstr << p->image.v[i]*256 << ";";
// 	cerr << imgstr.str() << endl;

	vector<string> files;
	dirH.listContentsFull(dirlayout->imagesdir, files);

// 	stringstream imgname;
// 	imgname << Timer::Instance()->sdl_now;

	stringstream buf;

// 	buf << dirlayout->imagesdir << "/" << imgname.str() << ".img";
	buf << dirlayout->imagesdir << "/" << (files.size()+1) << ".img";
	
	fileH.save(buf.str(), imgstr.str());

	stringstream qa;
	qa << dirlayout->answersdir << "/" << p->currentquestion.q << "-" << p->currentquestion.a << "-" << (files.size()+1) << ".a";
	fileH.save(qa.str(), "");

/*	stringstream buf;
	for ( unsigned int i=0; i < list.size(); i++ )
	{
		buf << list[0]
	}*/
}

void Answers::loadQuestionsFromDisk()
{
	questions.clear();

	vector<string> files;
	dirH.listContentsFull(dirlayout->questionsdir, files);

	for ( unsigned int i = 0; i < files.size(); i++ )
	{
		if ( parseH->Instance()->endMatches( ".q", files[i] ) )
		{
			string content;
			fileH.open( files[i], content ); 

			diskquestion q;
			q.name = files[i];

			//q.id
			string qid = Parser::Instance()->returnUntillStrip( ";", content );
			if(EOF == sscanf(qid.c_str(), "%d", &q.id))
				cerr << "reading question id " << q.name << endl;
			
			q.question = content.substr(0, content.size()-1);
			questions.push_back(q);
		}
	}
}

void Answers::loadAnswersFromDisk()
{
	answers.clear();
  
	vector<string> files;
	dirH.listContents(dirlayout->answersdir, files);

	for ( unsigned int i = 0; i < files.size(); i++ )
	{
		if ( parseH->Instance()->endMatches( ".a", files[i] ) )
		{
			string content;
			fileH.open( files[i], content ); 

			diskanswer a;

			string qid = Parser::Instance()->returnUntillStrip( "-", files[i] );
			if(EOF == sscanf(qid.c_str(), "%d", &a.q))
				cerr << "reading question id " << qid << endl;
			string aid = Parser::Instance()->returnUntillStrip( "-", files[i] );
			if(EOF == sscanf(aid.c_str(), "%d", &a.a))
				cerr << "reading answer id " << aid << endl;

			// strip .a from imgid
			string imgid = files[i].substr(0, files[i].size()-2);
			if(EOF == sscanf(imgid.c_str(), "%d", &a.img))
				cerr << "reading answer id " << imgid << endl;
			
// 			cerr << qid << " " << aid << " " << imgid << " " << endl;
			answers.push_back(a);
		}
	}
// 	cerr << "there are " << answers.size() << " answers" << endl;
}

Answers::~Answers()
{
}
