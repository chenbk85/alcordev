/*
 *  math_utils.h
 *  vf_rbpf_slam2d
 *
 *  Created by andera on 24/09/08.
 *  Copyright 2008 Sapienza, Università di Roma. All rights reserved.
 *
 */

#ifndef UOR_MATH_UTILS_H_INCLUDED__
#define UOR_MATH_UTILS_H_INCLUDED__

#define _USE_MATH_DEFINES
#include <cmath>

namespace uor  {
	
	///
	template<typename T = double>
	struct constants
	{
	///
	static inline T pi() {return static_cast<T> (M_PI);};
	///
	static inline T pi_2() {return static_cast<T> (M_PI_2);};
	///
	static inline T pi_4() {return static_cast<T> (M_PI_4);};
	///
	static inline T two_pi() {return static_cast<T> (2.0*M_PI);};
	///
	static inline T inv_pi() {return static_cast<T> (M_1_PI);};	
	};
	
//	template <>
//	struct constants<double>
//	{
//		///
//		static inline double pi() {return static_cast<double> (M_PI);};
//		///
//		static inline double pi_2() {return static_cast<double> (M_PI_2);};
//		///
//		static inline double pi_4() {return static_cast<double> (M_PI_4);};
//		///
//		static inline double two_pi() {return static_cast<double> (2.0*M_PI);};
//		///
//		static inline double inv_pi() {return static_cast<double> (M_1_PI);};
//	};
//	
//	template <>
//	struct constants<float>
//	{
//		///
//		static inline float pi() {return static_cast<float> (M_PI);};
//		///
//		static inline float pi_2() {return static_cast<float> (M_PI_2);};
//		///
//		static inline float pi_4() {return static_cast<float> (M_PI_4);};
//		///
//		static inline float two_pi() {return static_cast<float> (2.0*M_PI);};
//		///
//		static inline float inv_pi() {return static_cast<float> (M_1_PI);};
//	};
	
	
	///useful concersions
	template<typename T = double>
	struct cvt
	{
	//metric
static inline T cm2mt (T val){ return static_cast<T> (0.01*val);}	
static inline T cm2mm (T val){ return static_cast<T> (10.0*val);}

static inline T mt2cm (T val){ return static_cast<T> (100.0*val);}
static inline T mt2mm (T val){ return static_cast<T> (1000.0*val);}	

static inline T mm2mt (T val){ return static_cast<T> (0.001*val);}	
static inline T mm2cm (T val){ return static_cast<T> (0.1*val);}	

//trig
static inline T rad2deg(T val) { return static_cast<T> ( 180.0 * constants<T>::inv_pi() * val);}
static inline T deg2rad(T val) { return static_cast<T> ( 0.0174532925 *val);}
};




typedef cvt<> convert;

} // closes namespace uor

#endif //UOR_MATH_UTILS_H_INCLUDED__