#include "alcor/core/klt_tracker_t.h"
#include "alcor/core/config_parser_t.hpp"
//-------------------------------------------------------------------
namespace all { namespace core {
//-------------------------------------------------------------------
  klt_tracker_t::klt_tracker_t():count_(0)
  {
    tc_ = KLTCreateTrackingContext();
  }
//-------------------------------------------------------------------
  klt_tracker_t::~klt_tracker_t()
  {
    KLTFreeFeatureList(fl_);
    KLTFreeTrackingContext(tc_);
    delete [] img_pre_; 
    //delete [] img_post_;
  }
//-------------------------------------------------------------------
  void klt_tracker_t::init(std::string& params)
  {
    //
    core::config_parser_t config;
    //
    if(config.load(core::ini, params))
    {
    printf("KLT::OPENED config file\n");
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
    tc_->lighting_insensitive   =
      config.get<KLT_BOOL>("kltconf.lighting_insensitive",0);
    //
    tc_->writeInternalImages    = 
      config.get<KLT_BOOL>("kltconf.writeInternalImages", 0);
    //
    tc_->affineConsistencyCheck = 
      config.get<int>("kltconf.affineConsistencyCheck", 2); 
    //
    KLTSetVerbosity(config.get<int>("kltconf.verbosity", 0));
    }
    else
    {
    printf("KLT::cannot find config file %s\n",params);
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
    //
    KLTSetVerbosity(0);
    }

    //
    printf("KLT initialized: %d %d\n", nrows_, ncols_);
    KLTPrintTrackingContext(tc_);
    //
    npixels_ = static_cast<size_t>(nrows_*ncols_);
    //allocate images, the raw way
    img_pre_ = new core::uint8_t[npixels_];
    //img_post_= new core::uint8_t[npixels_];

  }
//-------------------------------------------------------------------
  void klt_tracker_t::set_first_image(core::uint8_ptr imgpre)
  {
    memcpy(img_pre_, imgpre, npixels_);
    KLTSelectGoodFeatures(tc_, img_pre_, ncols_, nrows_, fl_);
    count_++;
  }
//-------------------------------------------------------------------
  void klt_tracker_t::track(core::uint8_ptr img_cur)
  {
    KLTTrackFeatures(tc_, img_pre_, img_cur, ncols_, nrows_, fl_);
    KLTReplaceLostFeatures(tc_,  img_cur, ncols_, nrows_, fl_);
    memcpy(img_pre_, img_cur, npixels_);
    count_++;
  }
  //-------------------------------------------------------------------
  void klt_tracker_t::track(core::uint8_sarr img_cur)
  {
    KLTTrackFeatures(tc_, img_pre_, img_cur.get(), ncols_, nrows_, fl_);
    memcpy(img_pre_, img_cur.get(), npixels_);
    count_++;
  }
//-------------------------------------------------------------------
  void klt_tracker_t::log_status() const
  {
    char* ppmname;
    sprintf(ppmname, "tracking_img_%.4d.ppm",count_);
    KLTWriteFeatureListToPPM(fl_, img_pre_, ncols_, nrows_, ppmname);
  }
//-------------------------------------------------------------------
  void klt_tracker_t::set_verbosity(bool verbosity)
  {
    if(verbosity)
      KLTSetVerbosity(1);
    else
      KLTSetVerbosity(0);
  }
//-------------------------------------------------------------------
}};