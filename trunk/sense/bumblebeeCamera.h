#ifndef BumblebeeDeviceDriver_H_INCLUDED
#define BumblebeeDeviceDriver_H_INCLUDED
//---------------------------------------------------------------------------
namespace all{ 
	namespace sense {

	class BumblebeeDeviceDriver;
}
	}
//---------------------------------------------------------------------------
class all::sense::BumblebeeDeviceDriver
	{
public:
	/**
	 * Constructor.
	 */
	BumblebeeDeviceDriver();

protected:
	void *system_resources;
	};
//---------------------------------------------------------------------------
#endif //BumblebeeDeviceDriver_H_INCLUDED