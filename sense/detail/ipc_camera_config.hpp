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