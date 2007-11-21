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
    void init(std::string& params);
    ///
    void set_first_image(core::uint8_ptr imgpre);
    ///
    void track(core::uint8_ptr img);
    ///
    void track(core::uint8_sarr img);
    ///
    KLT_FeatureList status() const {return fl_;};
    ///
    void log_status() const;
    ///
    void set_verbosity(bool);

  private://klt stuff
    ///
    KLT_TrackingContext tc_;
    ///
    KLT_FeatureList fl_;

  private://data
    ///
    all::core::uint8_ptr img_pre_;
    ///
    all::core::uint8_ptr img_post_;
    ///
    int nfeatures_;
    ///
    int ncols_;
    ///
    int nrows_;
    ///
    size_t npixels_;
    ///
    int count_;
  };

}};