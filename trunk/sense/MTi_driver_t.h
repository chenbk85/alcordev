#ifndef MTi_driver_t_H_INCLUDED
#define MTi_driver_t_H_INCLUDED
///////////////////////////////////////////////////////////////////
#include <boost/shared_ptr.hpp>
///////////////////////////////////////////////////////////////////
#include "alcor/math/rpy_angle_t.h"
///////////////////////////////////////////////////////////////////
namespace all { namespace  sense{
  ///////////////////////////////////////////////////////////////////
namespace detail
{
  struct MTi_driver_impl;
} 
///////////////////////////////////////////////////////////////////ù
namespace tags
{
  struct heading_reset_t{};
  struct global_reset_t{};
  struct object_reset_t{};
  struct align_reset_t{};

  static heading_reset_t heading_reset = heading_reset_t(); 
  static global_reset_t global_reset = global_reset_t();
  static object_reset_t object_reset = object_reset_t();
  static align_reset_t align_reset   = align_reset_t();
}
///////////////////////////////////////////////////////////////////
  ///
  class MTi_driver_t
  {
  public:
    ///
    MTi_driver_t();
    ///
    ~MTi_driver_t();

    ///
    bool open(std::string configfile);

    //
    void reset(tags::heading_reset_t);
    //
    void reset(tags::global_reset_t);
    //
    void reset(tags::object_reset_t);
    //
    void reset(tags::align_reset_t);

	///
	bool req_data();

    ///
    all::math::rpy_angle_t  
        get_euler();

	///3 floats
    void  get_acc(float data_[]);

	///9 floats 
	// get GS
	//[a b c d e f g h i]
	//GS [a d g; b e h; c f i] //MTi(S) to global(G)
	//SG [a b c;d e f; g h i]  //global(G) to MTi(S)
	void get_dcm(float dcm_[]);


  private:
    boost::shared_ptr<detail::MTi_driver_impl> impl;

  };
///////////////////////////////////////////////////////////////////
  typedef boost::shared_ptr<MTi_driver_t> MTi_driver_ptr;
}}//namespace all::sense
///////////////////////////////////////////////////////////////////
#endif //MTi_driver_t_H_INCLUDED