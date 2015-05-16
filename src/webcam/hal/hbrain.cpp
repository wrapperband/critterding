#include "hbrain.h"

brain::brain()
{
	processed = false;
}

void brain::resetMotors()
{
      for ( unsigned int i=0; i < motorCount.size(); i++ )
		motorCount[i] = 0;
}

// void brain::updateMotorCount()
// {
//       for ( unsigned int i=0; i < motors.size(); i++ )
// 		if ( br->presence )
// 			motorCount[i] = 0;
// }


brain::~brain()
{
}

