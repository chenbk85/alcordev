#ifndef ipc_serializable_i_H_INC
#define ipc_serializable_i_H_INC
//-----------------------------------------------------------------------++
//boost.interprocess
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
//-----------------------------------------------------------------------++
using namespace boost::interprocess;
//-----------------------------------------------------------------------++
#include <boost/call_traits.hpp>
//-----------------------------------------------------------------------++
#include <memory>
//-----------------------------------------------------------------------++
#include "detail/memory_tags.hpp"
//-----------------------------------------------------------------------++
///
#define ENTER_CRITICAL(_OBJ_REF_)\
{\
boost::interprocess::scoped_lock<boost::interprocess::named_mutex> \
lock( *(_OBJ_REF_.mutex_.get()));
//-----------------------------------------------------------------------++
///
#define LEAVE_CRITICAL	}
//-----------------------------------------------------------------------++
//-----------------------------------------------------------------------++
namespace all { namespace core {
//-----------------------------------------------------------------------++
///T must be a default constructibile class ...
template <typename T>
class ipc_serializable
	{
public:
	///
    //ipc_serializable();
	///
    ipc_serializable(core::detail::open_t, const std::string&);
	///
    ipc_serializable(core::detail::read_t, const std::string&);

	///
	~ipc_serializable();

    ///
    typedef ipc_serializable<T>   self_type;
	///
	typedef typename boost::call_traits<T>::value_type  value_type;
	///
	typedef typename boost::call_traits<T>::const_reference  const_reference;
	typedef typename boost::call_traits<T>::reference        reference;
	
private:
	///
	bool open_ipc_channel_write_(const std::string&);
	///
	bool open_ipc_channel_read_(const std::string&);

public:
	///
	value_type*	get_ptr(){return data_.get();};

	///
	const_reference get_const() const {return *data_.get();};
	///
	reference get_ref()   {return *data_.get();};
    ///
	std::auto_ptr<named_mutex>	mutex_;	 

private:		

    ///
    std::auto_ptr<value_type>               data_;
	///
	std::auto_ptr<shared_memory_object>		memory_;
	///
	std::auto_ptr<mapped_region>			region_;
	///
	std::string	                            mutex_tag_;
	///
	std::string	                            memory_tag_;

	};
//-------------------------------------------------------------------------++
	}}//all::core
//-------------------------------------------------------------------------++
#include "ipc_serializable_inline.hpp"
//-------------------------------------------------------------------------++
#endif //ipc_serializable_i_H_INC