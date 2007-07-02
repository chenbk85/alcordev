#include "triclops.h"
#include "digiclops.h"
#include "pnmutils.h"
//-------------------------------------------------------------------
#include <boost/config.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <fstream>
//-------------------------------------------------------------------
#include "alcor/core/core.h"
//=============================================================================
#include <windows.h>
//=============================================================================
#include <boost/shared_ptr.hpp>
//forward ...
/**
 * This class handles saving/restoring Digiclops properties from the windows 
 * registry and/or an .ini file.
 */
class Settings  
{
public:

   /** @param context Must be an initialized Digiclops context. */
   Settings( DigiclopsContext context );
    
//    void setContext(DigiclopsContext context);
   
   virtual ~Settings();

   /** Save the current camera settings to the indicated file. */
   bool saveToFile( const char* pszPath );

   /** Load settings and set camera based on settings from a file. */
   bool loadFromFile( const char* pszPath );


protected:

   struct Property
   {
      DigiclopsCameraProperty property;
      bool  bOnePush;
      bool  bOnOff;
      bool  bAuto;
      int   iValueA;
      int   iValueB;
      char  pszName[ 64 ];
   };

   Property m_arProperties[ DIGICLOPS_PROPERTY_TOTAL ];
   
   DigiclopsContext m_context;

   bool readPropertyFromCamera( Property* pproperty );
   bool writePropertyToCamera( const Property* pproperty );


   bool readAllProperties();
   bool writeAllProperties();

   bool getPropertyString( const Property* pproperty, char* pszString );
   bool scanPropertyString( Property* pproperty, const char* pszString );

   Property* findPropertyFromName( const char* pszPropertyName );
   
   bool handleDigiclopsError( 
      DigiclopsError error, const char* pszFunction, ... ) const;
};
//=============================================================================
template <int ROWS, int COLS>
class image_indices_t
{
  public:
  BOOST_STATIC_CONSTANT(size_t, cols_times_rows=COLS*ROWS);

  unsigned int i [cols_times_rows];
  unsigned int j [cols_times_rows];

  image_indices_t()
  {
    for (unsigned int ii = 0,k =0; ii < ROWS; ++ii)
      for(unsigned int jj = 0; jj < COLS; ++jj, ++k)
      {
        i[k] = ii;
        j[k] = jj;
      }
  }
};
//=============================================================================

namespace all{ namespace sense { namespace detail {
//-------------------------------------------------------------------
typedef image_indices_t<240, 320> image_indices_320;
typedef image_indices_t<480, 640> image_indices_640;
//-------------------------------------------------------------------
class bumblebee_driver_impl {
public:
  bumblebee_driver_impl():
            valid_count_(0)
          , zbound_(10.0f)
          , is_depth16_new(false)
  {};

  bool init_digiclops_context_(DigiclopsSerialNumber, std::string& digiclopsfile
                              ,int framerate);
  bool init_triclops_context_(std::string& triclopsfile);

  bool init_grabbing_();

  void allocate_buffers_();
  ///
  void set_interleaved_();
  ///
  void set_planar_();
  ///
  bool grab_();

  bool left_color_proc_();
  bool right_color_proc_();

  float   focal_;
  float   center_row_;
  float   center_col_;
  float   base_;
  int     serial_num_;
  int		  rows_;
  int		  cols_;

  ///
  size_t planeinc;
  size_t planeinc_2;
  size_t pixelnum;
  size_t valid_count_;

  float zbound_;

  boost::shared_ptr<Settings> settings_sptr;

  //RIGHT
  core::uint8_sarr get_rgb_right_() const ;
  //LEFT
  core::uint8_sarr get_rgb_left_() const ;

  //XYZ
  boost::function<core::single_sarr (void)>  get_depthmap_buffer_;
  //  
  image_indices_320 indices320;
  //
  core::single_sarr get_depthmap_sandbox_320_planar_();
  core::single_sarr get_depthmap_sandbox_320_interleaved_();
  
  //  
  image_indices_640 indices640;
  core::single_sarr get_depthmap_sandbox_640_planar_();
  core::single_sarr get_depthmap_sandbox_640_interleaved_();

  //
  void extract_depth16_();
  //
  bool is_depth16_new;

  //data
  ///
  TriclopsInput       color_input_;
  ///
  TriclopsColorImage  left_color_;
  ///
  TriclopsColorImage  right_color_;
  ///
  TriclopsInput       stereo_input_;
  ///
  TriclopsImage16     depth_image_16_;

  ///Triclops Context
  TriclopsContext     triclops_context_;
  ///Digiclops Context
  DigiclopsContext    digiclops_context_;

  //Internal image data storage.
  ///
  all::core::uint8_sarr   left_image_sptr_;
  all::core::uint8_sarr   right_image_sptr_;
  ///
  all::core::single_sarr  depth_image_sptr_;
  //all::core::single_sarr  pcloud_ptr_;

  bool handle_digiclops_error(char*, DigiclopsError);
  bool handle_triclops_error(char*, TriclopsError);

  };
//###########################################################################
////-------------------------------------------------------------------
//-------------------------------------------------------------------
inline bool bumblebee_driver_impl::init_digiclops_context_(
  DigiclopsSerialNumber serial_num, std::string& digiclopsfile
  ,int framerate)
{
  //
  bool bok = true;

  DigiclopsError      de;
 // open the Digiclops
 de = digiclopsCreateContext( &digiclops_context_ );
 bok = bok && handle_digiclops_error( "digiclopsCreateContext()", de );

 de = digiclopsInitializeFromSerialNumber(digiclops_context_,  serial_num);
 bok = bok && handle_digiclops_error( "digiclopsInitialize()", de );

  // set the digiclops to deliver the stereo image and right (color) image
 de = digiclopsSetImageTypes( digiclops_context_,  ALL_IMAGES );
 bok = bok && handle_digiclops_error( "digiclopsSetImageTypes()", de );

		//     // set the Digiclops resolution
	de = digiclopsSetImageResolution( digiclops_context_,  DIGICLOPS_FULL);
 bok = bok && handle_digiclops_error( "digiclopsSetImageResolution()", de );

 DigiclopsFrameRate drate = DIGICLOPS_FRAMERATE_050;

 switch(framerate)
 {
 case 12:
   drate= DIGICLOPS_FRAMERATE_012;
   break;
 case 25:
   drate =DIGICLOPS_FRAMERATE_025;
   break;
 case 50:
    drate =DIGICLOPS_FRAMERATE_050;
   break;
 case 100:
    drate =DIGICLOPS_FRAMERATE_100;
   break;
 default:
    drate =DIGICLOPS_FRAMERATE_050;
   break;
 }

		//Framerate
	de= digiclopsSetFrameRate(digiclops_context_, drate);
	bok = bok && handle_digiclops_error( "digiclopsSetFrameRate()", de );

	de = digiclopsSetColorProcessing(digiclops_context_,  DIGICLOPS_EDGE_SENSING );
	bok = bok && handle_digiclops_error( "digiclopsSetColorProcessing()", de );
  
  printf("Reading Digiclops Settings from %s\n", (char*)digiclopsfile.c_str());
  settings_sptr.reset( new ::Settings(digiclops_context_) );
  settings_sptr->loadFromFile(digiclopsfile.c_str());

 return bok;
}
//-------------------------------------------------------------------
inline bool bumblebee_driver_impl::init_triclops_context_(std::string& triclopsfile)
{
  bool bok = true;
	TriclopsError  te;
	  printf("Reading Triclops Settings from %s\n", triclopsfile.c_str());
  te = triclopsGetDefaultContextFromFile(&triclops_context_, (char*)triclopsfile.c_str() );
	bok = bok && handle_triclops_error( "triclopsGetDefaultContextFromFile()", te );

	printf ("Triclops Configuration Loaded.\n");

  TriclopsBool is_subpixel_on = 1;
  triclopsGetSubpixelInterpolation(triclops_context_, &is_subpixel_on);

  if(is_subpixel_on)
  {
    te = triclopsSetSubpixelInterpolation(triclops_context_, 1);
    bok = bok && handle_triclops_error( "triclopsSetSubpixelInterpolation()", te );
  }

  te = triclopsGetFocalLength(triclops_context_, &focal_);
  bok = bok && handle_triclops_error( "triclopsGetFocalLength()", te );
  
  te = triclopsGetImageCenter(triclops_context_, &center_row_, &center_col_);
  bok = bok && handle_triclops_error( "triclopsGetImageCenter()", te );
      
  te = triclopsGetSerialNumber(triclops_context_, &serial_num_);
  bok = bok && handle_triclops_error( "triclopsGetSerialNumber()", te );
  
  te = triclopsGetBaseline(triclops_context_, &base_);
  bok = bok && handle_triclops_error( "triclopsGetBaseline()", te );

  te = triclopsGetResolution(triclops_context_, &rows_, &cols_);
  bok = bok && handle_triclops_error( "triclopsGetResolution()", te );


  //defaul is planar...
  if(rows_ == 240)
    get_depthmap_buffer_ = boost::bind(&bumblebee_driver_impl::get_depthmap_sandbox_320_planar_, this);
  else
    get_depthmap_buffer_ = boost::bind(&bumblebee_driver_impl::get_depthmap_sandbox_640_planar_, this);

  return bok;
}
//-------------------------------------------------------------------
inline bool bumblebee_driver_impl::init_grabbing_()
{
  DigiclopsError      de = digiclopsStart( digiclops_context_ );
  return handle_digiclops_error( "digiclopsStart()", de );		
}
//-------------------------------------------------------------------
inline void bumblebee_driver_impl::allocate_buffers_()
{
  left_image_sptr_.reset ( new all::core::uint8_t  [rows_*cols_*3] );
  right_image_sptr_.reset( new all::core::uint8_t  [rows_*cols_*3] );
  depth_image_sptr_.reset( new all::core::single_t [rows_*cols_*3] );
	planeinc  = rows_ * cols_;
  planeinc_2= planeinc*2;
  pixelnum =  planeinc*3;
}
//-------------------------------------------------------------------
inline void bumblebee_driver_impl::set_interleaved_()
{
  //defaul is planar...
  if(rows_ == 240)
    get_depthmap_buffer_ = boost::bind(&bumblebee_driver_impl::get_depthmap_sandbox_320_interleaved_, this);
  else
    get_depthmap_buffer_ = boost::bind(&bumblebee_driver_impl::get_depthmap_sandbox_640_interleaved_, this);
}
//-------------------------------------------------------------------
inline void bumblebee_driver_impl::set_planar_()
{
  //defaul is planar...
  if(rows_ == 240)
    get_depthmap_buffer_ = boost::bind(&bumblebee_driver_impl::get_depthmap_sandbox_320_planar_, this);
  else
    get_depthmap_buffer_ = boost::bind(&bumblebee_driver_impl::get_depthmap_sandbox_640_planar_, this);
}
//-------------------------------------------------------------------
inline bool bumblebee_driver_impl::grab_()
{
  bool b_ok = true;
  //GRAB IMAGE SET
  DigiclopsError de = digiclopsGrabImage( digiclops_context_ );
  b_ok = b_ok && ( handle_digiclops_error( "digiclopsGrabImage()", de ) );

  b_ok = b_ok && ( left_color_proc_() );
  b_ok = b_ok && ( right_color_proc_() );
  
  is_depth16_new = true;

  return b_ok;
}
//-------------------------------------------------------------------
inline bool bumblebee_driver_impl::left_color_proc_()
{
  bool b_ok = true;

  //GRAB IMAGE SET
  DigiclopsError de = digiclopsExtractTriclopsInput( digiclops_context_, LEFT_IMAGE, &color_input_ );
  b_ok = b_ok &&  ( handle_digiclops_error( "digiclopsExtractTriclopsInput()", de ) );

  TriclopsError  te = triclopsRectifyColorImage( 
    triclops_context_, TriCam_REFERENCE, &color_input_, &left_color_ );
  b_ok = b_ok && (handle_triclops_error( "triclopsRectifyColorImage()", te ) ); 

  if(!b_ok) return false;

  memcpy( &left_image_sptr_[0]           ,  left_color_.red   ,  rows_*cols_);
  memcpy( &left_image_sptr_[rows_*cols_]  , left_color_.green , rows_*cols_);
  memcpy( &left_image_sptr_[rows_*cols_*2], left_color_.blue  , rows_*cols_);

  return true;
}
//-------------------------------------------------------------------
inline bool bumblebee_driver_impl::right_color_proc_()
{
  bool b_ok = true;

  DigiclopsError de = digiclopsExtractTriclopsInput( digiclops_context_, RIGHT_IMAGE, &color_input_ );
  b_ok = b_ok && handle_digiclops_error( "digiclopsExtractTriclopsInput()", de );

  TriclopsError  te = triclopsRectifyColorImage( 
    triclops_context_, TriCam_REFERENCE, &color_input_, &right_color_ );
  b_ok = b_ok && ( handle_triclops_error( "triclopsRectifyColorImage()", te ) ); 

  if(!b_ok) return false;

  memcpy( &right_image_sptr_[0]             , right_color_.red   , rows_*cols_);
  memcpy( &right_image_sptr_[rows_*cols_]   , right_color_.green , rows_*cols_);
  memcpy( &right_image_sptr_[rows_*cols_*2] , right_color_.blue  , rows_*cols_);

  return true;
}
//-------------------------------------------------------------------
inline core::uint8_sarr bumblebee_driver_impl::get_rgb_right_() const 
{
  return right_image_sptr_;
}
//-------------------------------------------------------------------
inline core::uint8_sarr bumblebee_driver_impl::get_rgb_left_() const
{
  return left_image_sptr_;
}
//###########################################################################
//###########################################################################
//-------------------------------------------------------------------
//inline core::single_sarr bumblebee_driver_impl::get_depthmap_()
//{
//  std::ofstream logfile("beeimpl.txt", std::ios::out); 
///////////////////////////////////////////////////////////////////////////////////////
//  if(is_depth16_new)
//  {
//  extract_depth16_();
//  is_depth16_new = false;
//  }
///////////////////////////////////////////////////////////////////////////////////////
//  //
//	const int pixelinc    = depth_image_16_.rowinc/2;
//	unsigned short      disparity ;
//	float	              x, y, z; 
//	unsigned short*     row ;
//  //
//  std::memset(depth_image_sptr_.get(), 0, pixelnum * sizeof(core::single_t));
//  //
//  valid_count_ = 0;
//  int k = (rows_*cols_)-1;
//
//  int i = rows_;
//	////Outer loop: rows. Extract rows.
//	for (; i ; --i )
//    {
//		row = depth_image_16_.data + (i-1) * pixelinc;
//			//Inner Loop: cols. Extract disparities.
//		for (int j = cols_; j; --j, --k)
//        {
//          //logfile << "i: " << i << " j: " << j << " k: " << k << std::endl; 
//			  disparity = row[j-1];
//		// filter invalid points
//		    if ( disparity < 0xFF00 )
//          {
//			    // convert the 16 bit disparity value to floating point x,y,z
//			    triclopsRCD16ToXYZ
//				    ( triclops_context_, i-1, j-1, disparity, &x, &y, &z ) ;
//
//		      //if(z < zbound_)
//            //{
//              valid_count_++;
//              depth_image_sptr_[k]              = x;
//              depth_image_sptr_[k+planeinc]     = y;
//              depth_image_sptr_[k+planeinc_2]   = z;
//            //}
//
//           }//disparity
//        }//inner loop		
//    }//outer loop
//    
//  return depth_image_sptr_;
//}
//-------------------------------------------------------------------
////TODO: just a clone of the above code except for the ordering
//inline core::single_sarr bumblebee_driver_impl::get_depthmap_interleaved_()
//{
//
///////////////////////////////////////////////////////////////////////////////////////
//  if(is_depth16_new)
//  {
//  extract_depth16_();
//  is_depth16_new = false;
//  }
///////////////////////////////////////////////////////////////////////////////////////
//
//	const int pixelinc    = depth_image_16_.rowinc/2;
//
//	unsigned short      disparity ;
//	float	              x, y, z; 
//	unsigned short*     row ;
//
//    //
//  std::memset(depth_image_sptr_.get(), 0, pixelnum * sizeof(core::single_t));
//
//  //
//  valid_count_ = 0;
//  int k = (rows_*cols_*3);
//  int i = rows_;
//
//	////Outer loop: rows. Extract rows.
//	for (; i ; --i )
//    {
//		row     = depth_image_16_.data + (i-1) * pixelinc;
//			//Inner Loop: cols. Extract disparities.
//	  for (int j = cols_; j; --j, k-=3 )
//        {
//			  disparity = row[j-1];
//		// filter invalid points
//		    if ( disparity < 0xFF00 )
//          {
//			    // convert the 16 bit disparity value to floating point x,y,z
//			    triclopsRCD16ToXYZ
//				    ( triclops_context_, i-1, j-1, disparity, &x, &y, &z ) ;
//		      //if(z < 10)
//        //    {
//              valid_count_++;
//              depth_image_sptr_[k]     = x;
//              depth_image_sptr_[k+1]   = y;
//              depth_image_sptr_[k+2]   = z;
//            //}
//
//           }//disparity
//        }//inner loop		
//    }//outer loop
//    
//  return depth_image_sptr_;
//}
//------------------------------------------------------------------------
//TODO: just a clone of the above code except for the ordering
inline core::single_sarr bumblebee_driver_impl::get_depthmap_sandbox_320_planar_()
{
/////////////////////////////////////////////////////////////////////////////////////
  if(is_depth16_new)
  {
  extract_depth16_();
  is_depth16_new = false;
  }
/////////////////////////////////////////////////////////////////////////////////////
	unsigned short      disparity ;
	float	              x, y, z; 
  //
  std::memset(depth_image_sptr_.get(), 0, pixelnum * sizeof(core::single_t));
  //
  valid_count_ = 0;
  const size_t  total_floats = indices320.cols_times_rows;
  ///
  for (int i = 0, k = 0; i < total_floats ; ++i, ++k)
  {
    disparity  = depth_image_16_.data[i];
    if(disparity < 0xFF00)
    {
	    triclopsRCD16ToXYZ
        ( triclops_context_, indices320.i[i], indices320.j[i], disparity, &x, &y, &z ) ;
      //if(z < zbound_)
         //{
           valid_count_++;
           depth_image_sptr_[k]            = x;
           depth_image_sptr_[k+planeinc]   = y;
           depth_image_sptr_[k+planeinc_2] = z;


         //}
    }
  }
  return depth_image_sptr_;
}
//------------------------------------------------------------------------
//TODO: just a clone of the above code except for the ordering
inline core::single_sarr bumblebee_driver_impl::get_depthmap_sandbox_640_planar_()
{
/////////////////////////////////////////////////////////////////////////////////////
  if(is_depth16_new)
  {
  extract_depth16_();
  is_depth16_new = false;
  }
/////////////////////////////////////////////////////////////////////////////////////
	unsigned short      disparity ;
	float	              x, y, z; 
  //
  std::memset(depth_image_sptr_.get(), 0, pixelnum * sizeof(core::single_t));
  //
  valid_count_ = 0;
  ///
  for (int i = 0; i < indices640.cols_times_rows ; ++i)
  {
    disparity  = depth_image_16_.data[i];
    if(disparity < 0xFF00)
    {
	    triclopsRCD16ToXYZ
        ( triclops_context_, indices320.i[i], indices320.j[i], disparity, &x, &y, &z ) ;
      //if(z < zbound_)
         //{
           valid_count_++;
           depth_image_sptr_[i]     = x;
           depth_image_sptr_[i+1]   = y;
           depth_image_sptr_[i+2]   = z;
         //}
    }
  }
  return depth_image_sptr_;
}
//------------------------------------------------------------------------
//TODO: just a clone of the above code except for the ordering
inline core::single_sarr bumblebee_driver_impl::get_depthmap_sandbox_320_interleaved_()
{
/////////////////////////////////////////////////////////////////////////////////////
  if(is_depth16_new)
  {
  extract_depth16_();
  is_depth16_new = false;
  }
/////////////////////////////////////////////////////////////////////////////////////
	unsigned short      disparity ;
	float	              x, y, z; 
  //
  std::memset(depth_image_sptr_.get(), 0, pixelnum * sizeof(core::single_t));
  //
  valid_count_ = 0;
  const size_t  total_floats = indices320.cols_times_rows;
  ///
  for (int i = 0, k = 0; i < total_floats ; ++i, k+=3)
  {
    disparity  = depth_image_16_.data[i];
    if(disparity < 0xFF00)
    {
	    triclopsRCD16ToXYZ
        ( triclops_context_, indices320.i[i], indices320.j[i], disparity, &x, &y, &z ) ;
      //if(z < zbound_)
         //{
           valid_count_++;
           depth_image_sptr_[k]     = x;
           depth_image_sptr_[k+1]   = y;
           depth_image_sptr_[k+2]   = z;
         //}
    }
  }
  return depth_image_sptr_;
}
//------------------------------------------------------------------------
//TODO: just a clone of the above code except for the ordering
inline core::single_sarr bumblebee_driver_impl::get_depthmap_sandbox_640_interleaved_()
{
/////////////////////////////////////////////////////////////////////////////////////
  if(is_depth16_new)
  {
  extract_depth16_();
  is_depth16_new = false;
  }
/////////////////////////////////////////////////////////////////////////////////////
	//unsigned short      disparity ;
	//float	              x, y, z; 
 // //
 // std::memset(depth_image_sptr_.get(), 0, pixelnum * sizeof(core::single_t));
 // //
 // valid_count_ = 0;
 // ///
 // for (int i = 0; i < indices640.cols_times_rows ; ++i)
 // {
 //   disparity  = depth_image_16_.data[i];
 //   if(disparity < 0xFF00)
 //   {
	//    triclopsRCD16ToXYZ
 //       ( triclops_context_, indices320.i[i], indices320.j[i], disparity, &x, &y, &z ) ;
 //     //if(z < zbound_)
 //        //{
 //          valid_count_++;
 //          depth_image_sptr_[i]     = x;
 //          depth_image_sptr_[i+1]   = y;
 //          depth_image_sptr_[i+2]   = z;
 //        //}
 //   }
 // }
  return depth_image_sptr_;
}
//-------------------------------------------------------------------
  //
inline  void bumblebee_driver_impl::extract_depth16_()
{
  digiclopsExtractTriclopsInput( digiclops_context_, STEREO_IMAGE, &stereo_input_ );
  triclopsRectify( triclops_context_, &stereo_input_ );
  // stereo processing
  triclopsStereo( triclops_context_ ) ;
  // retrieve the interpolated depth image from the context
  triclopsGetImage16( 
    triclops_context_, TriImg16_DISPARITY, TriCam_REFERENCE, &depth_image_16_ );
}
//-------------------------------------------------------------------
inline bool bumblebee_driver_impl::handle_digiclops_error(char* function, 
                                                          DigiclopsError error)
{
   if( error != DIGICLOPS_OK ) 
   { 
      printf( 
	  "ERROR: %s reported %s.\n", 
	  function, 
	    digiclopsErrorToString( error ) ); 
    return false;
   } 
return true;
}
//-------------------------------------------------------------------
inline bool bumblebee_driver_impl::handle_triclops_error(char* function, 
                                                         TriclopsError error)
{
   if( error != TriclopsErrorOk ) 
   { 
      printf( 
	 "ERROR: %s reported %s.\n", 
	 function, 
	 triclopsErrorToString( error ) ); 
          return false;
   } 
   return true;
}
//-------------------------------------------------------------------
}}}//namespaces

//###########################################################################
//###########################################################################
//###########################################################################
//#define REGISTRY_KEY_TEXT "Software\\Point Grey Research, Inc.\\CameraSettings"



inline Settings::Settings( DigiclopsContext context )
{
   m_context = context;

   memset( m_arProperties, 0x0, sizeof( Property ) * DIGICLOPS_PROPERTY_TOTAL );

   m_arProperties[ DIGICLOPS_AUTO_EXPOSURE ].property = DIGICLOPS_AUTO_EXPOSURE;
   strcpy( m_arProperties[ DIGICLOPS_AUTO_EXPOSURE ].pszName, "Auto Exposure" );

   m_arProperties[ DIGICLOPS_SHUTTER ].property = DIGICLOPS_SHUTTER;
   strcpy( m_arProperties[ DIGICLOPS_SHUTTER ].pszName, "Shutter" );

   m_arProperties[ DIGICLOPS_GAIN ].property = DIGICLOPS_GAIN;
   strcpy( m_arProperties[ DIGICLOPS_GAIN ].pszName, "Gain" );

   m_arProperties[ DIGICLOPS_DELTA_GAIN_TOP ].property = DIGICLOPS_DELTA_GAIN_TOP;
   strcpy( m_arProperties[ DIGICLOPS_DELTA_GAIN_TOP ].pszName, "Delta Gain Top" );

   m_arProperties[ DIGICLOPS_DELTA_GAIN_LEFT ].property = DIGICLOPS_DELTA_GAIN_LEFT;
   strcpy( m_arProperties[ DIGICLOPS_DELTA_GAIN_LEFT ].pszName, "Delta Gain Left" );

   m_arProperties[ DIGICLOPS_AUTO_GAIN_LOW ].property = DIGICLOPS_AUTO_GAIN_LOW;
   strcpy( m_arProperties[ DIGICLOPS_AUTO_GAIN_LOW ].pszName, "Auto Gain Low" );

   m_arProperties[ DIGICLOPS_AUTO_GAIN_HIGH ].property = DIGICLOPS_AUTO_GAIN_HIGH;
   strcpy( m_arProperties[ DIGICLOPS_AUTO_GAIN_HIGH ].pszName, "Auto Gain High" );

   m_arProperties[ DIGICLOPS_AUTO_SHUTTER_LOW ].property = DIGICLOPS_AUTO_SHUTTER_LOW;
   strcpy( m_arProperties[ DIGICLOPS_AUTO_SHUTTER_LOW ].pszName, "Auto Shutter Low" );

   m_arProperties[ DIGICLOPS_AUTO_SHUTTER_HIGH ].property = DIGICLOPS_AUTO_SHUTTER_HIGH;
   strcpy( m_arProperties[ DIGICLOPS_AUTO_SHUTTER_HIGH ].pszName, "Auto Shutter High" );

   m_arProperties[ DIGICLOPS_WHITEBALANCE ].property = DIGICLOPS_WHITEBALANCE;
   strcpy( m_arProperties[ DIGICLOPS_WHITEBALANCE ].pszName, "Software White Balance" );

   m_arProperties[ DIGICLOPS_HARDWARE_WHITEBALANCE ].property = DIGICLOPS_HARDWARE_WHITEBALANCE;
   strcpy( m_arProperties[ DIGICLOPS_HARDWARE_WHITEBALANCE ].pszName, "White Balance" );
}


inline Settings::~Settings()
{
}


inline bool 
Settings::handleDigiclopsError( 
                               DigiclopsError   error, 
                               const char*      pszFunction, 
                               ... ) const
{
   if( error == DIGICLOPS_OK )
   {
      return true;
   }
   else
   {
      va_list ap;
      va_start( ap, pszFunction );
      
      char pszTemp[ 512 ];
      
      _vsnprintf( pszTemp, sizeof( pszTemp ), pszFunction, ap );
      
      //OutputDebugString( pszTemp );
      //fprintf( stderr, pszTemp );

      assert( false );
      
      va_end( ap );
      
      return false;
   }
}



inline bool 
Settings::saveToFile( const char* pszPath )
{
   FILE* pfile = fopen( pszPath, "w" );
   
   if( pfile == NULL )
   {
      fprintf( stderr, "Could not open %s for writing.\n", pszPath );
      return false;
   }

   if( !readAllProperties() )
   {
      return false;
   }

   for( int i = 0; i < DIGICLOPS_PROPERTY_TOTAL; i++ )
   {
      Property* pp = &m_arProperties[ i ];

      char pszPropertyString[ 512 ];

      if( getPropertyString( pp, pszPropertyString ) )
      {
         fprintf( pfile, "%s\n", pp->pszName );
         fprintf( pfile, "%s\n", pszPropertyString );
      }
   }

   fclose( pfile );

   return true;
}

inline bool 
Settings::loadFromFile( const char* pszPath )
{
   FILE* pfile = fopen( pszPath, "r" );
   
   if( pfile == NULL )
   {
      fprintf( stderr, "Could not open %s for reading.\n", pszPath );
      return false;
   }
   
   char pszLine[ 512 ];
   
   while( fgets( pszLine, sizeof( pszLine ), pfile ) != NULL )
   {
      size_t posTemp = strcspn( pszLine, "\n" );

      if( posTemp > 0 )
      {
         pszLine[ posTemp ] = '\0';
      }

      Property* pproperty = findPropertyFromName( pszLine );      
      
      if( pproperty != NULL )
      {
         if( fgets( pszLine, sizeof( pszLine ), pfile ) != NULL )
         {       
            if( scanPropertyString( pproperty, pszLine ) )
            {
               if( !writePropertyToCamera( pproperty ) )
               {
                  assert( false );
                  return false;
               }
            }               
         }
      }
   }
   
   
   fclose( pfile );
   
   return true;
}


inline bool 
Settings::readPropertyFromCamera( Property* property )
{
   DigiclopsError error = digiclopsGetCameraPropertyEx(
      m_context,
      property->property,
      &property->bOnePush,
      &property->bOnOff,
      &property->bAuto,
      &property->iValueA,
      &property->iValueB );
      
   return handleDigiclopsError( error, "digiclopsGetCameraPropertyEx()" );
}


inline bool 
Settings::writePropertyToCamera( const Property* property )
{
   DigiclopsError error = digiclopsSetCameraPropertyEx(
      m_context,
      property->property,
      property->bOnePush,
      property->bOnOff,
      property->bAuto,
      property->iValueA,
      property->iValueB );

   if( error == DIGICLOPS_NOT_IMPLEMENTED )
   {
      // Ignore this error.
      return true;
   }
   else
   {
      return handleDigiclopsError( error, "digiclopsSetCameraPropertyEx()" );
   }
}


inline bool 
Settings::readAllProperties()
{
   for( int i = 0; i < DIGICLOPS_PROPERTY_TOTAL; i++ )
   {
      readPropertyFromCamera( &m_arProperties[ i ] );
   }

   return true;
}


inline bool 
Settings::writeAllProperties()
{
   for( int i = 0; i < DIGICLOPS_PROPERTY_TOTAL; i++ )
   {
      writePropertyToCamera( &m_arProperties[ i ] );
   }
   
   return true;
}

inline bool 
Settings::getPropertyString( const Property* pproperty, char* pszString )
{
   sprintf( 
      pszString, 
      "ValueA=%d ValueB=%d OnePush=%d OnOff=%d Auto=%d",
      pproperty->iValueA,
      pproperty->iValueB,
      pproperty->bOnePush ? 1 : 0,
      pproperty->bOnOff ? 1 : 0,
      pproperty->bAuto ? 1 : 0 );

   return true;
}


inline bool 
Settings::scanPropertyString( Property* pproperty, const char* pszString )
{
   int iValueA;
   int iValueB;
   int iOnePush;
   int iOnOff;
   int iAuto;
   
   int iScan = sscanf(
      pszString,
      "ValueA=%d ValueB=%d OnePush=%d OnOff=%d Auto=%d",
      &iValueA,
      &iValueB,
      &iOnePush,
      &iOnOff,
      &iAuto );
   
   if( iScan == 5 )
   {
      pproperty->iValueA = iValueA;
      pproperty->iValueB = iValueB;
      pproperty->bOnePush = iOnePush != 0;
      pproperty->bOnOff = iOnOff != 0;
      pproperty->bAuto = iAuto != 0;
      
      return true;
   }
   else
   {
      return false;
   }
}


inline Settings::Property* 
Settings::findPropertyFromName( const char* pszPropertyName )
{
   for( int i = 0; i < DIGICLOPS_PROPERTY_TOTAL; i++ )
   {
      if( _stricmp( pszPropertyName, m_arProperties[ i ].pszName ) == 0 )
      {
         return &m_arProperties[ i ];
      }
   }

   return NULL;
}