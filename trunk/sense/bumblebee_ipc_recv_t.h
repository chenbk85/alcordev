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
    void get_lock(){};
    //
    void release_lock(){};

    //
    int     height()    const;
    int     width()     const;
    int     channels()  const;
    float   focal()     const;
    ///
    //std::size_t memsize() const;

    //NUOVA INTERFACCIA
    	///Inherited
	  bool open(const std::string &);
	  ///
    all::core::uint8_sarr  get_color_buffer(core::left_img_t ,  bool shared = true);
	  ///
    all::core::uint8_sarr  get_color_buffer(core::right_img_t , bool shared = true);
	  ///
    all::core::single_sarr get_depth_buffer(bool shared = true);


	private:
		///
		boost::shared_ptr<sense::detail::bumblebee_ipc_recv_impl> impl;

	};
//-------------------------------------------------------------------------++
//-------------------------------------------------------------------------++
#endif //bumblebee_ipc_recv_t_H_INCLUDED