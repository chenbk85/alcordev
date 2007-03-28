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

void	splam_client::lock()
{
	pImpl_->lock();
}

void	splam_client::unlock()
{
	pImpl_->unlock();
}

void	splam_client::set_display(bool disp)
{
	if(disp)
	{
		if(!pImpl_->display_)
			pImpl_->init_display();
	}else{
		if(pImpl_->display_)
			pImpl_->exit_display();
	}
	pImpl_->display_ = disp;
}

bool	splam_client::get_display() const
{
	return pImpl_->display_;
}

uint8_sarr	splam_client::get_splam_image()
{
	return pImpl_->get_splam_image();
}

int		splam_client::get_row() const
{
	return pImpl_->splam_data_net_->data_->og_row_;
}

int		splam_client::get_col() const
{
	return pImpl_->splam_data_net_->data_->og_col_;
}


}//namespace splam
}//namespace all
//-----------------------------------------------------------------------------------------------
