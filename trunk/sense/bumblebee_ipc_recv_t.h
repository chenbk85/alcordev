#ifndef bumblebee_ipc_recv_t_H_INCLUDED
#define bumblebee_ipc_recv_t_H_INCLUDED
//-------------------------------------------------------------------------++
#pragma comment(lib, "ipc_bumblebee_recv_t.lib")
//-------------------------------------------------------------------------++
#include "../core/core.h"
//-------------------------------------------------------------------------++
#include <boost/shared_ptr.hpp>
//-------------------------------------------------------------------------++
#include <string>
#include <memory>
#include <vector>
//-------------------------------------------------------------------------++
namespace all { namespace sense {
	//-------------------------++
	class bumblebee_ipc_recv_t;
	namespace detail
		{
		///Forward declared .....
		struct bumblebee_ipc_recv_impl;
		}
	//-------------------------++
	}}
//-------------------------------------------------------------------------++
///
class all::sense::bumblebee_ipc_recv_t
	{
	public:
		///c'tor
		bumblebee_ipc_recv_t();
		///
		~bumblebee_ipc_recv_t();

    //
    int     height()    const;
    int     width()     const;
    int     channels()  const;
    float   focal()     const;
    ///
    std::size_t memsize() const;

		/////
		//bool open_rgb(const std::string& in_name="ipc_bumblebee_rgb");
  //  ///
		//bool open_xyz(const std::string& in_name="ipc_bumblebee_xyz");

    //NUOVA INTERFACCIA
    	///Inherited
	  bool open(const std::string &);
	  ///
    all::core::uint8_sarr  get_color_buffer(core::left_img_t ,  bool shared = true);
	  ///
    all::core::uint8_sarr  get_color_buffer(core::right_img_t , bool shared = true);
	  ///
    all::core::single_sarr get_depth_buffer(bool shared = true);

		/////deprecata
		//void use_internal_buffers();

		/////deprecata
  //  bool get_color(core::uint8_ptr rgb_user_buffer);
		/////deprecata
  //  bool get_color(std::vector<core::uint8_t>& rgb_user_buffer);
		/////deprecata
  //  boost::shared_array<core::uint8_t> 
  //    get_internal_color(sense::right_img_t);

  //  		///deprecata
  //  bool get_depth(core::single_ptr);
  // 		///deprecata
  //  boost::shared_array<core::single_t> 
  //      get_internal_depth();

	private:
		///
		boost::shared_ptr<sense::detail::bumblebee_ipc_recv_impl> impl_;
    ///
    void allocate_();
		/////
  //  all::core::uint8_sarr   left_image_sptr;		
		///////
  //  all::core::uint8_sarr   right_image_sptr;
		/////
  //  all::core::single_sarr  depth_image_sptr;



	};
//-------------------------------------------------------------------------++
	//}}
//-------------------------------------------------------------------------++
#endif //bumblebee_ipc_recv_t_H_INCLUDED