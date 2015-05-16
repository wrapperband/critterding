#include "bodyowner.h"

Bodyowner::Bodyowner() {}
Bodyowner::~Bodyowner() {}


float Bodyowner::beEaten(float amount)
{
  if(amount > energyLevel)
    amount = energyLevel;
  energyLevel -= amount;

  if(energyLevel <= 0.0f)
    deathreason = "being eaten";

  return amount;
}


