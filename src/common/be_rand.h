#ifndef BE_RAND_H_INCLUDED
#define BE_RAND_H_INCLUDED

#include <cstdlib>
#include "common/be_timer.h"
#include "utils/settings.h"

class BeRand
{
	public:
		static BeRand* Instance();

		int get(const int minimum, const int maximum) const;
		unsigned int getSeed() const { return startseed; }
	protected:
		BeRand();
	private:
		static BeRand* _instance;

		unsigned int count;
		unsigned int startseed;
};

#endif


// #ifndef BE_RAND_H_INCLUDED
// #define BE_RAND_H_INCLUDED
// 
// #include <cstdlib>
// #include "common/be_timer.h"
// #include "utils/settings.h"
// 
// class BeRand
// {
// 	public:
// 		static BeRand* Instance();
// 
// 		const unsigned int		get(const unsigned int minimum, const unsigned int maximum) const;
// 		const unsigned int		getSeed() const { return startseed; }
// 
// 	protected:
// 		BeRand();
// 	private:
// 		static BeRand* _instance;
// 
// 		unsigned int count;
// 		unsigned int startseed;
// };
// 
// #endif
