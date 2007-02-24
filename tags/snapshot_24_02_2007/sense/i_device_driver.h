#ifndef i_device_driver_H_INCLUDED
#define i_device_driver_H_INCLUDED
//---------------------------------------------------------------------------
namespace all{ 
	namespace sense{
		class i_device_driver;
	}
	}
//---------------------------------------------------------------------------
#include <string>
///
class all::sense::i_device_driver
	{
	public:
		virtual ~i_device_driver(){};

	public:
		///
		bool open(const std::string &)  {return true;};
		///
		bool close() {return true;};
	};
//---------------------------------------------------------------------------
#endif //i_device_driver_H_INCLUDED