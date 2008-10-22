/*
 *  range_sensor_t.h
 *  vf_rbpf_slam2d
 *
 *  Created by andera on 26/09/08.
 *  Copyright 2008 Sapienza, Università di Roma. All rights reserved.
 *
 */

#ifndef UOR_RANGE_SENSOR_T_INCLUDED__
#define UOR_RANGE_SENSOR_T_INCLUDED__


#include "sensor/hokuyo_info.h"
#include "core/angle_t.h"
#include "sensor/range_reading_t.h"

//STL includes
#include <memory>
#include <vector>


///uor
namespace  uor {
	
	///Forward declaration
	struct  hokuyo_impl;
	
	///
	class hokuyo_lrf
		{
		//[ctor]
		public:
			///
			hokuyo_lrf();
			///
			hokuyo_lrf(const hokuyo_info& );
			///
			~hokuyo_lrf();
			///
			void set_params(const hokuyo_info& );
			
			//[the device stuff]
		public:
			///connects ans setup laser
			bool enable  ();
			///
			void disable ();
			
			///
			range_reading_ptr get();
			
		public:			
			///angular resolution
			angle_t resolution ();
			///
			hokuyo_info& params()   {return parameters_;}
			
			///should not be public
			angle_t step2angle(size_t);
			///
			size_t angle2step(const angle_t&) const;
	   
		private:
			///
			void updatebeamlookups_();
			///
			std::auto_ptr<hokuyo_impl> impl_;
			///
			beam_info_vec  beam_info_;
			///
			hokuyo_info parameters_;
		};
	
}//close namespace uor




#endif //UOR_RANGE_SENSOR_T_INCLUDED__