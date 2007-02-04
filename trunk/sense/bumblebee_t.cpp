#include "bumblebee_t.h"
//---------------------------------------------------------------------------
using namespace all;
using namespace all::sense;
//---------------------------------------------------------------------------
//IMPLEMENTATION
#include "bumblebee_impl.cpp"
//---------------------------------------------------------------------------
bumblebee_t::bumblebee_t()
	:pimpl(0)
	{
	}
//---------------------------------------------------------------------------
bool bumblebee_t::open(const std::string& in_file)
	{
	pimpl.reset(new bumblebee_impl(in_file));
	return true;
	}
//---------------------------------------------------------------------------
bool bumblebee_t::close()
	{
	return true;
	}
//---------------------------------------------------------------------------
size_t bumblebee_t::nrows() const
	{
	return pimpl->m_rows;
	}
//---------------------------------------------------------------------------
size_t bumblebee_t::ncols() const
	{
	return pimpl->m_cols;
	}
//---------------------------------------------------------------------------
void bumblebee_t::grab()
	{
	DigiclopsError		de;
	TriclopsError       te;

	pimpl->_grab();

	if (in_mode == e_mode::e_std)
	   {
		de = digiclopsExtractTriclopsInput( pimpl->digiclops, 
											RIGHT_IMAGE, 
											&pimpl->colorData );

		pimpl->handleDigiclopsError( "digiclopsExtractTriclopsInput()", de );		

 		te = triclopsRectifyColorImage( 
								pimpl->triclops, 
								TriCam_REFERENCE, 
								&pimpl->colorData, &pimpl->colorImage );

		pimpl->handleTriclopsError( "triclopsRectifyColorImage()", te );
	   }
   else
	   {
		//skipe   now
	   }

	}
////---------------------------------------------------------------------------
bool bumblebee_t::initialize(e_mode in_mode=e_std)
	{
	if(in_mode==e_std)
		{

		}
	return true;
	}
//---------------------------------------------------------------------------
bool  bumblebee_t::get_color_buffer(core::byte_t* user_buf, e_eye in_eye)
	{
	if(in_eye==e_right)
		{

		}
	else
		{

		}
	//bumblebee_impl& impl  = GETPIMPL(pimpl);

	//int planeinc = pimpl->colorImage.nrows * pimpl->colorImage.ncols;

	//core::spByteArray arr = core::byte_array_t::create(planeinc*3);

	//arr->fill(arr->begin(),	&pimpl->colorImage.red[0], planeinc);
	//arr->fill(boost::next(arr->begin(),planeinc), 
	//				&pimpl->colorImage.green[0],planeinc);
	//arr->fill(boost::next(arr->begin(),(planeinc*2)),
	//				&pimpl->colorImage.blue[0], planeinc);

	//return arr;
	return true;
	}
//---------------------------------------------------------------------------
bool bumblebee_t::get_depth_buffer()
	{
	//bumblebee_impl& impl =  GETPIMPL(pimpl);
	//int planeinc = pimpl->colorImage.nrows*pimpl->colorImage.ncols;

	//core::spFloatArray arr = core::float_array_t::create(planeinc*3);	
	//return arr;
	return true;
	}
////---------------------------------------------------------------------------