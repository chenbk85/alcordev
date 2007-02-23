#ifndef bumblebee_driver_t_H_INCLUDED
#define bumblebee_driver_t_H_INCLUDED
//---------------------------------------------------------------------------
#include "alcor/sense/i_device_driver.h"
//---------------------------------------------------------------------------
#include "alcor/core/core.h"
//---------------------------------------------------------------------------
#include <boost/shared_ptr.hpp>
//---------------------------------------------------------------------------
#pragma comment (lib, "bumblebee_driver_t.lib")
//---------------------------------------------------------------------------
namespace all{ 
	namespace sense {
	///A Point Grey Bumblebee digital camera.
	class bumblebee_driver_t;
	class bumblebee_parameters_t;
  typedef boost::shared_ptr<all::sense::bumblebee_driver_t> bumblebee_sptr;
  namespace detail{
  class bumblebee_driver_impl;
  }
}
	}
//---------------------------------------------------------------------------
///Structure for defining the open() parameters of the camera.
class all::sense::bumblebee_parameters_t
{
public:
	///Constructor. Add here the parameters for the open().
  bumblebee_parameters_t():
      _unit_number(0)
	{
		// parameters initialization default?
		_unit_number = 0;		
	}

	///Parameters
	unsigned int  _unit_number;

  ///
  std::string   _digiclopsini;
  /// 
  std::string   _triclopsini;
  ///
  std::string   _name;    
  ///
  bool load(const std::string &);

};

typedef boost::shared_ptr<all::sense::bumblebee_driver_t> bumblebee_driver_ptr;
//---------------------------------------------------------------------------
class all::sense::bumblebee_driver_t : public i_device_driver
	{
public:
	///Constructor.
	bumblebee_driver_t();

  ///
  //boost::mutex mutex;

	///Inherited
	bool open(const std::string &);

	///Inherited
	bool close();

	///Grabs Stereo Color Image.
	bool grab();

  ///
  long serialnum () const;
	///
	size_t nrows() const;
	///
	size_t ncols() const;
  ///
  float focal() const;
  ///
  float center_row() const;
  ///
  float center_col() const;
  ///
  std::string name() const;

	///
  all::core::uint8_sarr  get_color_buffer(core::left_img_t ,  bool shared = true);
	///
  all::core::uint8_sarr  get_color_buffer(core::right_img_t , bool shared = true);
	///
  all::core::single_sarr get_depth_buffer(bool shared = true);

protected:
	///PIMPL Idiom
  boost::shared_ptr<detail::bumblebee_driver_impl> impl;
  bumblebee_parameters_t params;
	};


//---------------------------------------------------------------------------
#endif //bumblebee_t_H_INCLUDED