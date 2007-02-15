//-----------------------------------------------------------------------++
namespace all { namespace core {
//-----------------------------------------------------------------------++
//-----------------------------------------------------------------------++
template <typename T>
inline ipc_serializable_t<T>::ipc_serializable_t(core::detail::open_t, const std::string& _memtag)
	:data_(0)
	,mutex_tag_("__mutex")
	,memory_tag_("__memory")
	{
	open_ipc_channel_write_(_memtag);
	}
//-----------------------------------------------------------------------++
template <typename T>
inline ipc_serializable_t<T>::ipc_serializable_t(core::detail::read_t, const std::string& _memtag)
	:data_(0)
	,mutex_tag_("__mutex_tag")
	,memory_tag_("__memory_tag")
	{
	open_ipc_channel_read_(_memtag);
	}
//-----------------------------------------------------------------------++
template <typename T>
inline ipc_serializable_t<T>::~ipc_serializable_t()
	{
	}
//-----------------------------------------------------------------------++
template <typename T>
inline bool ipc_serializable_t<T>::open_ipc_channel_write_(const std::string& in_name)
	{
	try
		{
		std::string memory_name = in_name + memory_tag_;
		std::string mutex_name = in_name + mutex_tag_;

		memory_.reset(
			new shared_memory_object
			(open_or_create                  
			,memory_name.c_str()              //name
			,read_write//read-write mode
			)
			);

		//Set the size of the shared memory segment
		memory_->truncate(sizeof(value_type));

		std::cout << "Memory Size is: " << sizeof(value_type)
			<< std::endl;

		//Map the whole shared memory in this process
		region_.reset(
			new mapped_region
			( *memory_.get()
			,read_write 
			)
			);

		//Get the address of the mapped region
		void * obj_addr  = region_->get_address();

		//Construct the shared structure in memory
		data_.reset( new (obj_addr) value_type );

		///
		//mutex_.reset( new named_mutex(open_or_create, mutex_name.c_str()) );

		}//try block

	catch(interprocess_exception &ex)
		{
		std::cout << "Unexpected exception: " << ex.what() << std::endl;
		return false;
		}//catch block

	return true;
	}
//-----------------------------------------------------------------------++
//-----------------------------------------------------------------------++
template <typename T>
inline bool ipc_serializable_t<T>::open_ipc_channel_read_(const std::string& in_name)
{
	try 
		{	
		std::string memory_name = in_name + memory_tag_;
		std::string mutex_name = in_name + mutex_tag_;
	////////////////////////////////////////////////////
	//	//Open already created shared memory object.
		memory_.reset( new shared_memory_object 
			(open_only
			,memory_name.c_str()           
			,read_only)
			);

	////Map the whole shared memory in this process
		region_.reset( new mapped_region
			(*memory_.get()                        
			,read_only)
		);

		///
		//mutex_.reset( new named_mutex(open_only, mutex_name.c_str()) );

		//Get the address of the mapped region
		//std::cout << "Get the address of the mapped region" << std::endl;
		void *   mem_ptr = region_->get_address();

		// //Construct the shared structure in memory
		data_.reset(static_cast<value_type*> (mem_ptr));

		} //try_block

	catch(interprocess_exception &ex)
		{
		std::cout << "Unexpected exception: " << ex.what() << std::endl;
		data_.reset(0);
		return false;
		}//catch block	

	return true;
}
//-----------------------------------------------------------------------++
//-----------------------------------------------------------------------++
}}//namespace
//-----------------------------------------------------------------------++