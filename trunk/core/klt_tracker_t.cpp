#include "alcor/core/klt_tracker_t.h"
#include "alcor/core/config_parser_t.hpp"
//-------------------------------------------------------------------
namespace all { namespace core {
//-------------------------------------------------------------------
  klt_tracker_t::klt_tracker_t()
  {
    tc_ = KLTCreateTrackingContext();
  }
//-------------------------------------------------------------------
  klt_tracker_t::~klt_tracker_t()
  {
    KLTFreeFeatureList(fl_);
    KLTFreeTrackingContext(tc_);
    delete [] img_pre_; 
    delete [] img_post_;
  }
//-------------------------------------------------------------------
  void klt_tracker_t::init(const std::string& params)
  {
    //
    core::config_parser_t config;
    //
    if(config.load(core::ini, params))
    {
    //
    nfeatures_ = config.get<int>("kltconf.nFeatures", 150);
    //
    nrows_ = config.get<int>("kltconf.nrows", 240);
    //
    ncols_ = config.get<int>("kltconf.ncols", 320);
    //
    fl_ = KLTCreateFeatureList(nfeatures_);    
    //
    tc_->mindist = config.get<int>("kltconf.mindist", 20);
    //
    tc_->window_width  = config.get<int>("kltconf.window_width", 9);
    //
    tc_->window_height = config.get<int>("kltconf.window_height", 9);
    //
    tc_->sequentialMode         = 
      config.get<KLT_BOOL>("kltconf.sequentialMode", 1);
    //
    tc_->writeInternalImages    = 
      config.get<KLT_BOOL>("kltconf.writeInternalImages", 0);
    //
    tc_->affineConsistencyCheck = 
      config.get<int>("kltconf.affineConsistencyCheck", 2); 
    }
    else
    {
    //
    nfeatures_ = 150;
    //
    nrows_ = 240;
    //
    ncols_ = 320;
    //
    fl_ = KLTCreateFeatureList(nfeatures_);    
    //
    tc_->mindist = 20;
    //
    tc_->window_width  = 9;
    //
    tc_->window_height = 9;
    //
    tc_->sequentialMode         = 1;
    //
    tc_->writeInternalImages    = 0;
    //
    tc_->affineConsistencyCheck = 2;
    }

    //allocate images, the raw way
    img_pre_ = new core::uint8_t[nrows_*ncols_];
    img_post_= new core::uint8_t[nrows_*ncols_];

  }
//-------------------------------------------------------------------
  void klt_tracker_t::set_img(core::uint8_ptr imgpre)
  {
    memcpy(img_pre_, imgpre, ncols_*nrows_);
    KLTSelectGoodFeatures(tc_, img_pre_, ncols_, nrows_, fl_);
  }
//-------------------------------------------------------------------
  void klt_tracker_t::track(all::core::uint8_ptr img_cur)
  {
    KLTTrackFeatures(tc_, img_pre_, img_cur, ncols_, nrows_, fl_);
    memcpy(img_pre_, img_cur, ncols_*nrows_);
  }
//-------------------------------------------------------------------
}};