#include "splam_client.h"
#include "detail/splam_client_impl.cpp"
//-----------------------------------------------------------------------------------------------
namespace all{
namespace splam{

splam_client::splam_client(const char* name)
{
	pImpl_.reset(new splam_client_impl(name));
}

splam_client::~splam_client(){}

void	splam_client::get_splam_data(splam_data& data)
{
	pImpl_->get_splam_data(data);
}

}//namespace splam
}//namespace all
//-----------------------------------------------------------------------------------------------
