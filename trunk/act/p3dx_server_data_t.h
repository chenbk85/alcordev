#ifndef p3dx_server_data_t_H_INCLUDED
#define p3dx_server_data_t_H_INCLUDED
//-------------------------------------------------------------------
#include "alcor/core/i_serializable.h"
#include "alcor/math/all_math.h"
//-------------------------------------------------------------------
using all::math::pose2d;
//-------------------------------------------------------------------
namespace all 
	{
namespace act 
	{
//-------------------------------------------------------------------
    class p3dx_position_t 
      :	 public all::math::pose2d
        ,public all::core::i_serializable
		{
		//[c'tor]
		public:
			p3dx_position_t ():pose2d()
						{

						}
      ///
			p3dx_position_t (double _x,double _y, double _th):
            pose2d(_x,_y,math::angle(_th, all::math::deg_tag))
						{
						}
      ///
			p3dx_position_t (const pose2d &pose):
					pose2d(pose)
						{
              
						}
      ///
			p3dx_position_t& operator=(const pose2d& other) 
				{
          setX(other.get_x1());
          setY(other.get_x2());
          setTh(math::angle( other.getTh()) );
			    return *this;
				}
      ///
			p3dx_position_t& operator=(const p3dx_position_t& other) 
				{
          setX(other.getX());
				  setY(other.getY());
          setTh(math::angle( other.getTh()) );
			    return *this;
				}

		public:
      ///
			void import(void* _pkt);
      ///
			void pack(void* _pkt) ;		
		};
//-------------------------------------------------------------------
	}//alcor
	}//device
//-------------------------------------------------------------------
#endif //p3dx_server_data_H_INCLUDED