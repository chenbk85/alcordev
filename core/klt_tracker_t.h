#include "alcor.extern/klt/klt.h"
#include "alcor/core/core.h"

namespace all { namespace core {

  class klt_tracker_t
  {
  public://ctor
    ///
    klt_tracker_t();
    ///
    ~klt_tracker_t();

  public://interface
    ///
    void init(const std::string& params="config/klt_conf.ini");
    ///
    void set_img(all::core::uint8_ptr imgpre);
    ///
    void track(all::core::uint8_ptr img);

  private://
    ///

  private://data
    ///
    all::core::uint8_ptr img_pre_;
    ///
    all::core::uint8_ptr img_post_;
    ///
    KLT_TrackingContext tc_;
    ///
    KLT_FeatureList fl_;
    ///
    //KLT_FeatureTable ft_;
    ///
    int nfeatures_;// = 150;
    //int nFrames;// = 10;
    ///
    int ncols_;
    ///
    int nrows_;
  };

}};