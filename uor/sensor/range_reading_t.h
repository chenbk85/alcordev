/*
 *  range_reading_t.h
 *  vf_rbpf_slam2d
 *
 *  Created by andera on 28/09/08.
 *  Copyright 2008 Sapienza, Università di Roma. All rights reserved.
 *
 */

#ifndef UOR_RANGE_READING_H_INCLUDED__
#define UOR_RANGE_READING_H_INCLUDED__

#include <boost/enable_shared_from_this.hpp>
#include <vector>

#include "sensor/beam_t.h"
#include "core/oriented_point2d_t.h"

namespace uor  {
	
	///
	class range_reading_t : public boost::enable_shared_from_this<range_reading_t>
		{
		//
		public:
			///
			typedef boost::shared_ptr<range_reading_t> ptr;
			
			///
			static ptr create(size_t nbeams)
			{
				return ptr (new range_reading_t (nbeams) );
			}
			
			///
			size_t nbeams() const {return cartesian_.size();}
			
		public:
			

		//[views]	
		public:
			///
			point2d_vec		cartesian_;	
			
		private:
			///
			range_reading_t(size_t nbeams) : cartesian_(nbeams) {};
		
		};
	
	///
	typedef range_reading_t::ptr range_reading_ptr;
}

#endif //UOR_RANGE_READING_H_INCLUDED__