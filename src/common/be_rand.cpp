#include "be_rand.h"

BeRand* BeRand::Instance () 
{
	static BeRand t;
	return &t;
}

BeRand::BeRand()
{
	// random seed
	BeTimer timer;
// 	unsigned int startseed;
	if ( Settings::Instance()->getCVar("startseed") == 0 )
	{
		timer.mark();
		unsigned int n1 = timer.getMilliSeconds();
		srand( n1 );
		unsigned int r1 = get( 2000, 20000 );
		for ( unsigned int i=0; i<r1; i++ ) srand( i );
	// 	cerr << "n1: " << n1 << endl;
	// 	cerr << "r1: " << r1 << endl;

		timer.mark();
		unsigned int n2 = timer.getMilliSeconds();
		srand( n2 );
		unsigned int r2 = get( 2000, 20000 );
		for ( unsigned int i=0; i<r2; i++ )  srand( i );
	// 	cerr << "n2: " << n2 << endl;
	// 	cerr << "r2: " << r2 << endl;

		timer.mark();
		unsigned int n3 = timer.getMilliSeconds();
		srand( n3 );
		unsigned int r3 = get( 2000, 20000 );
		for ( unsigned int i=0; i<r3; i++ )  srand( i );
	// 	cerr << "n3: " << n3 << endl;
	// 	cerr << "r3: " << r3 << endl;

		timer.mark();
		startseed = (n1*n2*n3*timer.getMilliSeconds())+r1+r2+r3+timer.getMilliSeconds();
	}
	else
	{
		// custom seed
		startseed = Settings::Instance()->getCVar("startseed"); // 420041141;
	}

	if ( Settings::Instance()->getCVar("benchmark") == 0 )
		std::cout << "RNG: seed: " << startseed << std::endl;

	srand( startseed );
	count = 0;
}

int BeRand::get(const int minimum, const int maximum) const
{
	if ( maximum > minimum )
	{
		return ((rand() % (maximum-minimum+1)) + minimum);
	}
	else
	{
		return minimum;
	}
}


// #ifdef _MINGW
// 	#include <unistd.h>
// #endif
// #include "be_rand.h"
// 
// // good rng seeds for benchmark:
// // 1063523561
// 
// BeRand* BeRand::Instance () 
// {
// 	static BeRand t;
// 	return &t;
// }
// 
// BeRand::BeRand()
// {
// 	// random seed
// 	BeTimer timer;
// 	startseed=0;
// 
// 	if ( Settings::Instance()->getCVar("startseed") == 0 )
// 	{
// 		timer.mark();
// 		const unsigned int n1 = timer.getTotalMilliSeconds();
// 		srand( n1 );
// 		const unsigned int r1 = get( 2000, 20000 );
// 		for ( unsigned int i=0; i<r1; i++ ) srand( i );
// 	// 	cerr << "n1: " << n1 << endl;
// 	// 	cerr << "r1: " << r1 << endl;
// 
// 		timer.mark();
// 		const unsigned int n2 = timer.getTotalMilliSeconds();
// 		srand( n2 );
// 		const unsigned int r2 = get( 2000, 20000 );
// 		for ( unsigned int i=0; i<r2; i++ )  srand( i );
// 	// 	cerr << "n2: " << n2 << endl;
// 	// 	cerr << "r2: " << r2 << endl;
// 
// 		timer.mark();
// 		const unsigned int n3 = timer.getTotalMilliSeconds();
// 		srand( n3 );
// 		const unsigned int r3 = get( 2000, 20000 );
// 		for ( unsigned int i=0; i<r3; i++ )  srand( i );
// 	// 	cerr << "n3: " << n3 << endl;
// 	// 	cerr << "r3: " << r3 << endl;
// 
// 		timer.mark();
// 		startseed = (n1*n2*n3*timer.getTotalMilliSeconds())+r1+r2+r3+timer.getTotalMilliSeconds();
// 	}
// 	else
// 	{
// 		// custom seed
// 		startseed = Settings::Instance()->getCVar("startseed"); // 420041141;
// 	}
// 
// 	if ( Settings::Instance()->getCVar("benchmark") == 0 )
// 		std::cout << "start seed: " << startseed << std::endl;
// 
// 	srand( startseed );
// 	count = 0;
// }
// 
// const unsigned int BeRand::get(const unsigned int minimum, const unsigned int maximum) const
// {
// // 	cerr << "requested betweed " << minimum << " and " << maximum << " got ";
// 	if ( maximum > minimum )
// 	{
// 	  // commented out for benchmark, no reseeding
// /*		if (++count > 1000)
// 		{
// 			unsigned int newseed = timer.getTotalMilliSeconds();
// 			if ( newseed > 0 )
// 				srand( newseed + (rand() % (maximum-minimum+1)) + minimum );
// 			count = 0;
// 		}*/
// 	
// 		return ((rand() % (maximum-minimum+1)) + minimum);
// 	}
// 	else
// 	{
// 		return minimum;
// 	}
// }
