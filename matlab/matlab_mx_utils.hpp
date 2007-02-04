#pragma once

#include "alcor/matlab/detail/matlab_tags.hpp"
#include <vector>
//-----------------------------------------------------------------------++
namespace all{ namespace matlab {
//---------------------------------------------------------------------++
template<typename T>
struct buffer2array 
{

//---------------------------------------------------------------------++
static mxArray* create_from_planar(typename T::const_ptr _src
				,matlab::row_major_t//src data ordering
				,int _height
				,int _width
				,int _channels=3)
{
 ///
mwSize dims[] = {_height, _width, _channels};
///
int ndimensions = ( (_channels > 2 ) ? (3) : (2) );
///
mxArray* mxresult = mxCreateNumericArray(ndimensions, dims, T::tag, mxREAL);
///
T::ptr start_pr = static_cast<T::ptr>(mxGetData(mxresult));
//fill using start_pr ...
    //
	int kb = 0;
    //
    int row_inc = _width;
    //
	int planar_inc = _height*_width;

	int column_inc=0;	

	///channel's stride ..
    //TODO: remove it from here?
	std::vector<int> _channel_stride;
	for (int i = _channels-1 ; i >= 0; --i)
		{	
		_channel_stride.push_back((i)*planar_inc);
		}

    //total number of pixels (without taking care of the pixel's depth..)
    int idx = planar_inc-1;
  //printf("buffer2array loop IN\n");
	//Loop inversion!
	for(int coli =_width ; coli; --coli)
        {	
          //printf("col n: %d\n", coli);
		column_inc = (coli-1);//*_channels; 
		for(int rowi=_height; rowi; --rowi, --idx)
			{
        //printf("row n: %d\n", rowi);
			//
			kb= (row_inc*(rowi-1)) + column_inc;
            //TODO: check this code ...          
      //printf("loop n: %d\n", idx);
			for(int ich = _channels-1 ; ich >= 0; --ich)
				{

				start_pr[idx + _channel_stride[ich]] = _src[kb+_channel_stride[ich]];
				}
			}
		}  
  //printf("buffer2array loop OUT\n");
	return mxresult;
}

//static mxArray* create_from_planar(typename T::const_pointer _src
//        ,matlab::detail::column_major_t//src data ordering
//				,int _height
//				,int _width
//				,int _channels=3)
//{
//   ///
//int dims[] = {_height, _width, _channels};
/////
//int ndimensions = ( (_channels > 2 ) ? (3) : (2) );
/////
//mxArray* mxresult = mxCreateNumericArray(ndimensions, dims, T::tag, mxREAL);
/////
//T::pointer start_pr = static_cast<T::pointer>(mxGetData(mxresult));
//
/////
//std::memcpy(start_ptr,_src,( _height*_width*T::size*_channels));
//
/////
//return mxresult;
//
//
//}
//---------------------------------------------------------------------++
///
static mxArray* create_from_interleaved(typename T::const_ptr _src
					,matlab::row_major_t//src ordering
					,const int& _height
					,const int& _width
					,const int& _channels=3)//depth==num of channels/component
{
    //
	mwSize dims[] = {_height, _width, _channels};
    ///TODO: mmmh  
	int ndimensions =( (_channels > 2 ) ? (3) : (2) );
    //
	mxArray* mxresult = mxCreateNumericArray(ndimensions, dims, T::tag, mxREAL);
    //
	T::ptr start_pr = static_cast<T::ptr>(mxGetData(mxresult));
    //
	int kb = 0;
    //
	int pixel_inc = _width*_channels;
    //
	int planar_inc = _height*_width;
    //int planar_inc_2 = planar_inc*2;
	int column_inc=0;	

	///channel's stride ..
    //TODO: remove it from here?
	std::vector<int> _channel_stride;
	for (int i = _channels-1 ; i >= 0; --i)
		{	
		_channel_stride.push_back((i)*planar_inc);
		}

    //total number of pixels (without taking care of the pixel's depth..)
    int idx = planar_inc;

	//Loop inversion!
	for(int coli =_width ; coli; --coli)
  {
		column_inc = (coli-1)*_channels; 
		for(int rowi=_height; rowi; --rowi, --idx)
			{
			//
			kb= (pixel_inc*(rowi-1))+column_inc;
            //TODO: check this code ...
			for(int ich = _channels-1 ; ich >= 0; --ich)
				{
				start_pr[idx + _channel_stride[ich]] = _src[kb+ich];
				}
			}
  }
	return mxresult;
}

};
//---------------------------------------------------------------------++
	}}//namespaces
//----------------------------------------------------------------------++//inline 
