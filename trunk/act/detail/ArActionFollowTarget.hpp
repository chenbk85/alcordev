#pragma once

#include "Aria.h"

class ArActionFollowTarget: public : public ArAction
{
public:
  // constructor, sets myMaxSpeed and myStopDistance
  ArActionFollowTarget(double maxSpeed, double stopDistance);

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
  RIGHT 
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
  double myMaxSpeed;
  double myStopDistance;
  bool  killMovement;
  ///
  double m_rel_distance ;
  double m_rel_offset   ;
  ///
  eSector mySector;
  ///
  eRange myDistance;
  ///
  ArMutex action_mutex;
};

//
inline ArActionFollowTarget::ArActionFollowTarget(double maxSpeed, double stopDistance)
{
myMaxSpeed = maxSpeed;
myStopDistance = stopDistance;
killMovement = false;
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

  return &myDesired;
}
