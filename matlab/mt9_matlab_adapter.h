#ifndef mt9_matlab_adapter_H_INCLUDED
#define mt9_matlab_adapter_H_INCLUDED
//---------------------------------------------------------------------++
#include "alcor/sense/shared_mt9_data_t.h"
#include "alcor/matlab/detail/matlab_tags.hpp"
//---------------------------------------------------------------------++
namespace all { 
	namespace matlab {
		class mt9_matlab_adapter ;
	}}
//---------------------------------------------------------------------++
class all::matlab::mt9_matlab_adapter 
{
public:
	static mxArray* create_array(const all::sense::mt9_data_t& _mt9_data)
	{
	mwSize dims[] = {1, 4};
  typedef  matlab::traits<core::single_t> singleT;

  mxArray* mt9_array = mxCreateNumericArray(2, dims, singleT::tag, mxREAL);
  singleT::ptr  start_pr = static_cast<singleT::ptr>(mxGetData(mt9_array));	

	start_pr[0]=_mt9_data.roll;
	start_pr[1]=_mt9_data.pitch;
	start_pr[2]=_mt9_data.yaw;

	start_pr[3]=static_cast<singleT::value_type>(_mt9_data.elapsed);

	return mt9_array;
	}
	/////////////////////////////////////////////////////////////////////
	static mxArray* create_struct(const all::sense::mt9_data_t& _mt9_data)
	{
	//
	const char *field_names[] = {"roll", "pitch", "yaw", "elapsed"};
	//
	mwSize dims[2] = {1, 1};

    // Create a 1-by-1 array of structs
    mxArray* mt9_struct = mxCreateStructArray(2, dims, 4, field_names);
	//
    int roll_field		= mxGetFieldNumber(mt9_struct,  "roll");
    int pitch_field		= mxGetFieldNumber(mt9_struct,  "pitch");
    int yaw_field		= mxGetFieldNumber(mt9_struct,  "yaw");
    int elapsed_field	= mxGetFieldNumber(mt9_struct,  "elapsed");

		//create mxArrays for each field (FLOATS)
	mxArray* mxRoll		= mxCreateNumericArray(2,dims, mxSINGLE_CLASS, mxREAL);
	mxArray* mxPitch	= mxCreateNumericArray(2,dims, mxSINGLE_CLASS, mxREAL);
	mxArray* mxYaw		= mxCreateNumericArray(2,dims, mxSINGLE_CLASS, mxREAL);
	mxArray* mxElapsed	= mxCreateNumericArray(2,dims, mxUINT32_CLASS, mxREAL);

    //Gather C Pointer
    float* start_roll_ptr    =	(float*)mxGetPr(mxRoll);
    float* start_pitch_ptr	 =	(float*)mxGetPr(mxPitch);
    float* start_yaw_ptr	 =	(float*)mxGetPr(mxYaw);
    int* start_elapsed_ptr   =	(int*)  mxGetPr(mxElapsed);

    ///
    start_roll_ptr[0]=_mt9_data.roll;
    start_pitch_ptr[0]=_mt9_data.pitch;
    start_yaw_ptr[0]=_mt9_data.yaw;
    start_elapsed_ptr[0]=_mt9_data.elapsed;

	//Assign each array to the specified field
	mxSetFieldByNumber(mt9_struct,0,roll_field, mxRoll);
	mxSetFieldByNumber(mt9_struct,0,pitch_field, mxPitch);
	mxSetFieldByNumber(mt9_struct,0,yaw_field, mxYaw);
	mxSetFieldByNumber(mt9_struct,0,elapsed_field, mxElapsed);
	//
	return mt9_struct;
	}
};
//---------------------------------------------------------------------++
////}}//namespaces
//---------------------------------------------------------------------++
#endif//mt9_matlab_adapter_H_INCLUDED
