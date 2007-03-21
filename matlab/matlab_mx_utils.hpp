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
static mxArray* create_from_planar(typename matlab::traits<T>::const_ptr _src
				,matlab::row_major_t//src data ordering
				,size_t _height
				,size_t _width
				,int _channels=3)
{
   ///
  mwSize dims[] = {_height, _width, _channels};
  ///
  int ndimensions = ( (_channels > 2 ) ? (3) : (2) );
  ///
  mxArray* mxresult = mxCreateNumericArray(ndimensions, 
                                            dims, 
                                            matlab::traits<T>::tag, 
                                            mxREAL);
  ///
  matlab::traits<T>::ptr start_pr = 
      static_cast<matlab::traits<T>::ptr>(mxGetData(mxresult));
//fill using start_pr ...
    //
  size_t kb = 0;
  //
  size_t row_inc = _width;
  //
  size_t planar_inc = _height*_width;
  //
  size_t column_inc=0;	

	///channel's stride ..
    //TODO: remove it from here?
	std::vector<size_t> _channel_stride;

	for (size_t i = _channels ; i; --i)
		{	
		_channel_stride.push_back((i-1)*planar_inc);
		}

    //total number of pixels (without taking care of the pixel's depth..)
    size_t idx = planar_inc-1;
  //printf("buffer2array loop IN\n");
	//Loop inversion!
	for(size_t coli =_width ; coli; --coli)
        {	
          //printf("col n: %d\n", coli);
		column_inc = (coli-1);//*_channels; 
		for(size_t rowi=_height; rowi; --rowi, --idx)
			{
        //printf("row n: %d\n", rowi);
			//
			kb= (row_inc*(rowi-1)) + column_inc;
            //TODO: check this code ...          
      //printf("loop n: %d\n", idx);
			for(size_t ich = _channels ; ich; --ich)
				{
				start_pr[idx + _channel_stride[ich-1]] = _src[kb+_channel_stride[ich-1]];
				}
			}
		}  
  //printf("buffer2array loop OUT\n");
	return mxresult;
}

//---------------------------------------------------------------------++
///
static mxArray* create_from_interleaved(typename matlab::traits<T>::const_ptr _src
					,matlab::row_major_t//src ordering
					,size_t _height
					,size_t _width
					,size_t _channels=3)//depth==num of channels/component
{
  //
	mwSize dims[] = {_height, _width, _channels};
    ///TODO: mmmh  
	int ndimensions =( (_channels > 2 ) ? (3) : (2) );
    //
	mxArray* mxresult = mxCreateNumericArray(ndimensions, dims, matlab::traits<T>::tag, mxREAL);
    //
	matlab::traits<T>::ptr start_pr = 
      static_cast<matlab::traits<T>::ptr>(mxGetData(mxresult));
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
	for (int i = _channels ; i ; --i)
		{	
		_channel_stride.push_back((i-1)*planar_inc);
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
