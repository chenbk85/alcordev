/*
 *  basic_unit.h
 *  urg04lx
 *
 *  Created by andera on 11/10/08.
 *  Copyright 2008 Sapienza, Università di Roma. All rights reserved.
 *
 */

#ifndef UOR_BASIC_UNIT_H_INCLUDED__
#define  UOR_BASIC_UNIT_H_INCLUDED__

namespace uor  {
	
	//
	struct mt_t {};
	struct cm_t {};
	struct mm_t {};
	//
	static const mt_t  mt = mt_t();
	static const cm_t	cm = cm_t();
	static const mm_t	mm = mm_t();
	
	template < typename T  > struct  unit_traits {};
	
	template < mt_t > 
	struct unit_traits
	{
		typedef double value_type;
		double si_factor = 1.0;
	};
	
	template < cm_t > 
	struct unit_traits
	{
		typedef double value_type;
		double si_factor = 0.1;
	};
	
	template < mm_t > 
	struct unit_traits
	{
		typedef double value_type;
		double si_factor = 0.01;
	};

	
}

#endif //UOR_BASIC_UNIT_H_INCLUDED__