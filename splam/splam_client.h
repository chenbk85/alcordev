#ifndef	splam_client_H
#define	splam_client_H
//-----------------------------------------------------------------------------------------------
#include <boost/shared_ptr.hpp>
#include "splam_data.h"
//-----------------------------------------------------------------------------------------------
namespace all{
namespace splam{

//fwd
class splam_client_impl;
  
class splam_client
{
public:	
	explicit	splam_client(const char* name = "config/splam.ini");
				~splam_client();
				
public:	
	void		lock();
	void		unlock();
	void		get_splam_data(splam_data&);
	void		set_display(bool=true);
	bool		get_display() const;

private:
	boost::shared_ptr<splam_client_impl> pImpl_;
};

}//namespace splam
}//namespace all
//---------------------------------------------------------------------------
#endif