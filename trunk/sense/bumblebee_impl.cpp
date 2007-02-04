//#include "triclops.h"
//---------------------------------------------------------------------------
#include "digiclops.h"
#include "pnmutils.h"
//---------------------------------------------------------------------------
//bumblebee_parameters_t inc.
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <string>
//---------------------------------------------------------------------------
///Structure for defining the open() parameters of the camera.
class bumblebee_parameters_t
{
public:
	typedef enum _fr{
		e012 = 12,
		e025 = 25,
		e050 = 50,
		e100 = 100
		}e_fr;

	///Constructor. Add here the parameters for the open().
	bumblebee_parameters_t():
		digiclops_conf("digiclops.ini"),
		triclops_conf("triclops.ini")
	{
		// parameters initialization
		unit_number = 5160167;		
		//rows = 480;
		//cols = 640;
		framerate = e050;
		
	}

	// Parameters
	unsigned long _unit_number;

	e_fr	_framerate;

	std::string _digiclops_conf;
	std::string _triclops_conf;

    void load(const std::string &filename);
    void save(const std::string &filename);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void bumblebee_parameters_t::load(const std::string& in_file)
	{
   // Create empty property tree object
    using boost::property_tree::ptree;
    ptree pt;
    // Load XML file and put its contents in property tree. 
    // No namespace qualification is needed, because of Koenig 
    // lookup on the second argument. If reading fails, exception
    // is thrown.
    read_xml(in_file, pt);

    _unit_number = pt.get<unsigned long>("bumblebee.unit_number");

	_framerate = pt.get<e_fr>("bumblebee.framerate", e050);

	_digiclops_conf = pt.get<std::string>("bumblebee.digiclops");
	_triclops_conf = pt.get<std::string>("bumblebee.triclops");

	}
//---------------------------------------------------------------------------
void bumblebee_parameters_t::save(const std::string& filename)
	{

	}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
///
class bumblebee_impl
	{
public:
	///
	bumblebee_impl(const std::string&);
	///
	bumblebee_parameters_t params;
	
    float   m_focal;
    float   m_centerRow;
    float   m_centerCol;
    float   m_baseline;
    int     m_serialNum;
	size_t	m_rows;
	size_t	m_cols;

	///
	void _init_context();
	/// 
	void _digiclops_private_init();

	void _grab();

	///Depth map ...
	void _triclops_process();
	///
	void _destroy_context();

public:
	///Error Handling
    void handleDigiclopsError(char*, DigiclopsError);
	///Error Handling
    void handleTriclopsError(char*, TriclopsError);

public:
	///
	TriclopsInput       colorData;
	///
	TriclopsColorImage  colorImage;
	///
	TriclopsColorImage  leftColorImage ;
	///
	TriclopsColorImage  rightColorImage ;

	///
	TriclopsInput       stereoData;
	///
	TriclopsImage16     depthImage16;

	///Triclops Context
	TriclopsContext     triclops;
	///Digiclops Context
	DigiclopsContext    digiclops;

//Settings*			m_settings;
	};
//---------------------------------------------------------------------------
bumblebee_impl::bumblebee_impl(const std::string& in_file)
	{
	params.load(in_file);
	}
//---------------------------------------------------------------------------
inline void bumblebee_impl::_init_context()
	{
	DigiclopsError      de;
	//printf("Creating Context\n");

   // open the Digiclops
   de = digiclopsCreateContext( &digiclops );
   handleDigiclopsError( "digiclopsCreateContext()", de );

   de = digiclopsInitializeFromSerialNumber(digiclops, params._unit_number);
   handleDigiclopsError( "digiclopsInitialize()", de );

   // get the camera module configuration
   de = digiclopsGetTriclopsContextFromCamera( digiclops, &triclops );
   handleDigiclopsError( "digiclopsGetTriclopsContextFromCamera()", de );
	}
//---------------------------------------------------------------------------
inline void bumblebee_impl::_destroy_context()
	{
	printf("Stopping the camera\n");
	DigiclopsError      de;
	TriclopsError       te; 

	//if(bRunning)
	//	{
	//	stop_grabbing();
	//	}

	de = digiclopsStop(digiclops); 
	handleDigiclopsError( "digiclopsStop()", de );
	   
	de = digiclopsDestroyContext(digiclops );
	handleDigiclopsError( "digiclopsDestroyContext()", de );

  	te = triclopsDestroyContext(triclops);
	handleTriclopsError("triclopsDestroyContext",te);
   
    printf("CAMERA STOPPED\n");
    //bRunning = false;
	}
//---------------------------------------------------------------------------
inline void bumblebee_impl::_grab()
	{
	// grab the image set
	DigiclopsError      de;
	de = digiclopsGrabImage(digiclops );
	handleDigiclopsError( "digiclopsGrabImage()", de );
	}
//---------------------------------------------------------------------------
inline void bumblebee_impl::_triclops_process()
	{

	TriclopsError       te;
	DigiclopsError		de;

	de = digiclopsExtractTriclopsInput( digiclops, STEREO_IMAGE, &stereoData );
		handleDigiclopsError( "digiclopsExtractTriclopsInput()", de );

	// preprocessing the images
	te = triclopsPreprocess( triclops, &stereoData );
		handleTriclopsError( "triclopsPreprocess()", te );

	// stereo processing
	te = triclopsStereo( triclops ) ;
		handleTriclopsError( "triclopsStereo()", te );
   
	// retrieve the interpolated depth image from the context
	te = triclopsGetImage16( 
      triclops, TriImg16_DISPARITY, TriCam_REFERENCE, &depthImage16 );
		handleTriclopsError( "triclopsGetImage16()", te );
	}
//---------------------------------------------------------------------------