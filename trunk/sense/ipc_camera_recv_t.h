#ifndef ipc_camera_recv_t_H_INCLUDED
#define ipc_camera_recv_t_H_INCLUDED
//-------------------------------------------------------------------------++
#pragma comment(lib, "ipc_camera_recv_t.lib")
//-------------------------------------------------------------------------++
#include "alcor/core/core.h"
//-------------------------------------------------------------------------++
#include <string>
#include <memory>
#include <vector>
//-------------------------------------------------------------------------++
using std::vector;
//-------------------------------------------------------------------------++
namespace all { namespace sense {
	//-------------------------++
	///Forward declared .....
	//class ipc_camera_recv_impl;
	class ipc_camera_recv_t;
	//-------------------------++
	}}
//-------------------------------------------------------------------------++
///
class ipc_camera_recv_impl;
///
class all::sense::ipc_camera_recv_t
	{
	public:
		///c'tor
		ipc_camera_recv_t();

		///
		~ipc_camera_recv_t();

		///
		bool open(const std::string& in_name="xmlconf/grabber.xml");

		///
		bool get_image_buffer(all::core::uint8_ptr);
		///
    bool get_image_buffer2(all::core::uint8_ptr );
		///
    bool get_image_buffer(vector<core::uint8_t>&);
		///
		void use_internal_buffer();

		///
    core::uint8_ptr get_internal_buffer() ;

		///
		std::size_t get_width() const;

		///
		std::size_t get_height() const;

		///
		std::size_t get_nchannels() const;

		///
		std::size_t get_size() const;

	private:
		///
		std::auto_ptr<ipc_camera_recv_impl> impl_;
        ///TODO: experimental .. ok  funziona
        std::vector<core::uint8_t> v_image_buffer_;
	};
//-------------------------------------------------------------------------++
	//}}
//-------------------------------------------------------------------------++
#endif //ipc_camera_recv_t_H_INCLUDED