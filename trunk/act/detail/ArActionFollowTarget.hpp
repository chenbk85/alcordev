#pragma once

#include "Aria.h"
#include <cmath>

inline int my_sign(double x) {return (x<0)? (-1) : (1) ;};

class ArActionFollowTarget: public ArAction
{
public:
  // constructor, sets myMaxSpeed and myStopDistance
  ArActionFollowTarget(double speed_limit);

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
  isFAR,  //go fast 
  isCLOSE,//go medium
  isNEAR  //stop
  };

private:
  // what the action wants to do; used by the action resover after fire()
  ArActionDesired myDesired;

  ///
  bool    kill_Movement;
  ///
  double  front_sector;//??

  ///
  volatile double m_rel_distance ;
  ///
  volatile double m_rel_offset   ;
  
  ///
  double  calcvclose();
  ///
  double  calcvfar  ();

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
  double speed_dump;
  double m_speed_limit;

  ///
  ///gradi
  double  th_near;    
  ///gradi
  double  th_close;  
  //gradi
   double  deltah_close;
  ///gradi
   double  deltah_max  ;
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
inline ArActionFollowTarget::ArActionFollowTarget(double speed_limit):  ArAction("FH")
{
  ///
  kill_Movement = false;

  ///
  m_speed_limit = speed_limit;

  //some defaul
  ///metri
  dnear   = 1.0;  
  ///metri
  dclose  = 2.0;
  ///
  dfar    = 3.0;

  //some defaul
  ///gradi
  th_near   = 1.0;  
  ///gradi
  th_close  = 25.0;

  //gradi
  deltah_close  = 8.0;
  ///gradi
  deltah_max    = 10.0;

  ///
  vclose  = 120;
  ///
  vfar    = 250;
  ///
  speed_dump = 0.7;

  m_rel_distance = 0;
  m_rel_offset   = 0;
}

//
inline void ArActionFollowTarget::set_target(double rel_distance, double rel_offset)
{
  printf("setting: %f %f\n", rel_distance,rel_offset);
  m_rel_distance = rel_distance;
  m_rel_offset   = rel_offset;
}

inline void ArActionFollowTarget::setRobot(ArRobot *robot)
{
  ArAction::setRobot(robot);
}

inline ArActionDesired *ArActionFollowTarget::fire(ArActionDesired currentDesired)
{
  double tempvel;
  //double temprotvel;

  // reset the actionDesired (must be done), to clear
  // its previous values.
  //myDesired.reset();

  if(m_rel_distance > dnear)
  {
    //target is safe
    if(m_rel_distance < dclose)
    {
    //target is close
    tempvel = calcvclose();
    }
    else
    {
      //target is far
    tempvel = calcvfar();
    }
  }
  else
  {
    //brake
    tempvel = 0;
  }

  tempvel = (tempvel > m_speed_limit)? (m_speed_limit) : (tempvel) ;

  double absoffset = ::fabs(m_rel_offset);

  if ( absoffset  > th_near)
  {
    if(absoffset  < th_close )
    {
      //myDesired.setRotVel();
      double numer = (m_rel_offset - th_near)*(m_rel_offset - th_near);
      double denom = (th_close - th_near)* (th_close - th_near);
      myDesired.setDeltaHeading (my_sign(m_rel_offset)*deltah_close * (numer/denom) );  
    }
    else
    {
      tempvel *= speed_dump;
      myDesired.setDeltaHeading( my_sign(m_rel_offset)*deltah_max);
    }
  }
  else
  {
    myDesired.setDeltaHeading(0);      
  }

  myDesired.setVel(tempvel);

  printf("Desired: VEL: %f dHeading: %f \n", myDesired.getVel()
    ,myDesired.getDeltaHeading());
  return &myDesired;
}


  ///
inline double  ArActionFollowTarget::calcvclose()
{
  double numer = (m_rel_distance - dnear)*(m_rel_distance - dnear);
  double denom = (dclose-dnear)*(dclose-dnear);
  return (vclose * (numer/denom) );
}
  ///
inline double  ArActionFollowTarget::calcvfar  ()
{
  double numer = (m_rel_distance-dclose)*(m_rel_distance-dclose);
  double denom = (dfar-dclose)*(dfar-dclose);
  double fraction = - numer/denom;
  return ( ((vclose-vfar)* fraction ) + vclose );
}
