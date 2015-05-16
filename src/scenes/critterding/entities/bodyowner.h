#ifndef BODYOWNER_H
#define BODYOWNER_H

#include "body.h"


class Bodyowner
{
  public:
    Bodyowner();
    virtual ~Bodyowner();

    bool isPicked() {for (unsigned int i=0; i < body.bodyparts.size(); i++) if(body.bodyparts[i]->isPicked()) return true; return false;};
    bool isPicked(bool v) {for (unsigned int i=0; i < body.bodyparts.size(); i++) body.bodyparts[i]->isPicked(v); return v; };
    virtual   float beEaten(float amount);

    Body      body;
    float     energyLevel;
    string    deathreason;

  private:
    bool ispicked;
};



#endif // BODYOWNER_H
