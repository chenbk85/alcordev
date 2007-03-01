#pragma once

#include "Aria.h"

class ArActionFollowTarget: public ArAction
{
public:
  // constructor, sets myMaxSpeed and myStopDistance
  ArActionFollowTarget();

  // destructor. does not need to do anything
  ~ArActionFollowTarget(void) {};

  //
  void set_target(double mt_distance, double degree_offset);

  // called by the action resolver to obtain this action's requested behavior
  ArActionDesired *fire(ArActionDesired currentDesired);

  // store the robot pointer, and it's ArSonarDevice object, or deactivate this action if there is no sonar.
  void setRobot(ArRobot *robot);

  enum eSector
  {
  FRONT,//front sector
  LEFT,  
  RIGHT,
  SIDE
  };

  //depending on sector ...
  //try to adjust speed ...
  enum eRange
  {
  FAR,  //go fast 
  CLOSE,//go medium
  NEAR  //stop
  };

private:
  // what the action wants to do; used by the action resover after fire()
  ArActionDesired myDesired;

  ///
  bool    kill_Movement;
  ///
  double  front_sector;//??

  ///
  double m_rel_distance ;
  ///
  double m_rel_offset   ;
  
  ///
  double  calcvclose(double dist);
  ///
  double  calcvfar  (double dist);

  ///
  float dnear;  
  ///
  float dclose;
  ///
  float dfar;

  ///millimetri
  double vclose ;
  ///
  double vfar;

  ///

  ///
  eSector mySector;
  ///
  eRange myDistance;
  ///
  ArMutex action_mutex;
};
//###################################################################
//###################################################################
//###################################################################
//
inline ArActionFollowTarget::ArActionFollowTarget()
{
  //myMaxSpeed = maxSpeed;
  //myStopDistance = stopDistance;
  front_sector  = 30.0;//degrees

  kill_Movement = false;
//some defaul
  ///metri
  dnear   = 1.0;  
  ///metri
  dclose  = 2.0;
  ///
  dfar    = 2.8;

  ///
  vclose  = ;
  ///
  vfar    = ;

}

//
inline void ArActionFollowTarget::set_target(double rel_distance, double rel_offset)
{
  m_rel_distance = rel_distance;
  m_rel_offset   = rel_offset;
}

void ArActionFollowTarget::setRobot(ArRobot *robot)
{
  ArAction::setRobot(robot);
}

ArActionDesired *ArActionFollowTarget::fire(ArActionDesired currentDesired)
{
  mySector = (::fabs(rel_offset) < front_sector)? (FRONT):(SIDE);

  double tempvel;
  double temprotvel;

  if(m_rel_distance > dnear)
  {
    //target is safe
    if(m_rel_distance < dclose)
    {
    //target is close
    tempvel = calcvclose(m_rel_distance);
    }
    else
    {
      //target is far
    tempvel = calcvfar(m_rel_distance);
    }
  }
  else
  {
    //brake
    tempvel = 0;
  }

  return &myDesired;
}

  ///
double  ArActionFollowTarget::calcvclose(double dist)
{
  double numer = (m_rel_distance - dnear)*(m_rel_distance - dnear);
  double denom = (dclose-dnear)*(dclose-dnear);
  return (vclose * (numer/denom) );
}
  ///
double  ArActionFollowTarget::calcvfar  (double dist);
{

}
