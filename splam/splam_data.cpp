#include "splam_data.h"
#include "detail/value_iteration.h"
//-----------------------------------------------------------------------------------------------
using namespace all::util;
//-----------------------------------------------------------------------------------------------
namespace all{
namespace splam{

bool	splam_data::metric_goal_finding(pose2d& goal, pose2d& goal_temp)
{
	static value_iteration vi(og_row_, og_col_, static_cast<int>(og_resolution_*100.0));


}


}//namespace splam
}//namespace all
