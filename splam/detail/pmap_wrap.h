#ifndef PmapWrap_H
#define PmapWrap_H
//-----------------------------------------------------------------------------------------------
#include <boost/shared_ptr.hpp>
#include "alcor/splam/splam_data.h"
#include "alcor/splam/scan_data.h"
#include "alcor/math/pose2d.h"
#include "alcor.extern/pmap/lodo.h"
#include "alcor.extern/pmap/omap.h"
#include "alcor.extern/pmap/pmap.h"
#include "alcor.extern/pmap/slap.h"
//-----------------------------------------------------------------------------------------------
using namespace std;
using namespace all::math;
//-----------------------------------------------------------------------------------------------
namespace all{
namespace splam{

class pmap_wrap{
public:
				pmap_wrap(const char* name);
				~pmap_wrap();
				void	fill_splam_data(boost::shared_ptr<splam_data> data);
	void		process(scan_data_ptr scan); /// add a scan
	void		save_map_as_file(const char* filename); ///< save the PGN grayscale OccupancyGrid file 
	int			get_laser_scan_number() const {return laser_scan_number_;}
	size_t		get_map_cells() const {return omap_->grid_sx*omap_->grid_sy;} ///< get the number of map cells, RAW*COL
	size2d		get_map_size() const {return size2d(omap_->grid_sy,omap_->grid_sx);} ///< get a pair of map dimension (RAW,COL)
	pose2d		get_current_position() const; ///< fast "current position" method
	size2d		get_current_coord() const; ///< get tho cells coord (raw,col) of current robot position
	bool		is_first_scan() const {return (lodo_->scan_count == 0);}
	//pose2d	GetLastLodoPose(){return current_lodoPose;}

private:
	omap_t*		omap_;
	pmap_t*		pmap_;
	lodo_t*		lodo_;
	int			laser_scan_number_;	///< number of laser scan value in a single scan
	double*		laser_scan_data_;	///< array of single laser scan
	pose2d		offset_laser_pose_;	///< laser displacement with respect to local robot coord center
	//pose2_t corrpose;
};

}//namespace splam
}//namespace all
//-----------------------------------------------------------------------------------------------
#endif
