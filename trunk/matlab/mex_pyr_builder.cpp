//MATLAB Includes
#include "mex.h"
//#include "matrix.h"
//
#pragma comment (lib , "vigraimpex.dll.lib")

//
#include <iostream>
#include <cstdio>
#include "vigra/stdimage.hxx"
#include "vigra/convolution.hxx"
#include "vigra/stdconvolution.hxx"
#include "vigra/resizeimage.hxx"
#include "vigra/impex.hxx"
#include "vigra/basicimageview.hxx"
#include "vigra/fixedpoint.hxx"
#include "vigra/copyimage.hxx"
//
using namespace vigra; 

// Gaussian reduction to next pyramid level
template <class Image>
void reduceToNextLevel(Image & in, Image & out)
{    
    // image size at current level
    int width = in.width();
    int height = in.height();
    
    // image size at next smaller level
    int newwidth = (width + 1) / 2;
    int newheight = (height + 1) / 2;
    
    // resize result image to appropriate size
    out.resize(newwidth, newheight);
    
    // define a Gaussian kernel (size 5x1)
    vigra::Kernel1D<double> filter;
    filter.initExplicitly(-2, 2) = 0.05, 0.25, 0.4, 0.25, 0.05;
    
    vigra::BasicImage<typename Image::value_type> tmpimage1(width, height);
    vigra::BasicImage<typename Image::value_type> tmpimage2(width, height);
    
    // smooth (band limit) input image
    separableConvolveX(srcImageRange(in),
                       destImage(tmpimage1), kernel1d(filter));
    separableConvolveY(srcImageRange(tmpimage1),
                       destImage(tmpimage2), kernel1d(filter));
                       
    // downsample smoothed image
    resizeImageNoInterpolation(srcImageRange(tmpimage2), destImageRange(out));
    
}

//------------------------------------------------------------------------
template <typename Image>
mxArray* create_array_from_vigra_image(const Image& src , mxClassID mx_tag)
{
  //       
  mwSize dims[] = {src.width(),src.height()};
  //mwSize ndim = 2;
  mxArray* mx_arr = mxCreateNumericArray(2, dims, mx_tag, mxREAL);
  Image::pointer mx_arr_cptr = static_cast<Image::pointer>
                    (mxGetData(mx_arr));
  memcpy(
        mx_arr_cptr, //dest
        src.data(),    //src
        (src.end() - src.begin())*sizeof(Image::value_type)
        );

  return mx_arr;
}
//------------------------------------------------------------------------

//Local Defines
#define MX_INPUT_IMAGE  prhs[0]
#define MX_INPUT_LEVELS prhs[1]

#define MX_OUTPUT_PYR    plhs[0]
#define MX_OUTPUT_GRAD   plhs[1]
//
static const int k_levels = 5;
//
void mexFunction( int nlhs, mxArray* plhs[], int nrhs, const mxArray
*prhs[] )
{

  if(nrhs > 0)
  {
   if ( mxIsUint8(MX_INPUT_IMAGE) )
    {
    //int nlevels = 5;

    //if (nrhs>1)
    //  nlevels = static_cast<int>(mxGetScalar(MX_INPUT_LEVELS) );

    vigra::UInt8* input_image_ptr = static_cast<vigra::UInt8*>
          (mxGetData(MX_INPUT_IMAGE));

    size_t n = mxGetN(MX_INPUT_IMAGE);
    size_t m = mxGetM(MX_INPUT_IMAGE);

    try {
       vigra::BasicImageView<vigra::UInt8> input_image(input_image_ptr,m,n);       
        if(1)//TODO:check if grayscale
        {
          //GET INPUT IMAGE
          //vigra::FImage base_image(m,n);
          vigra::FImage levels[k_levels];
          //copyImage(srcImageRange(input_image), destImage(base_image) );
          levels[0].resize(input_image.width(), input_image.height());
          copyImage(srcImageRange(input_image), destImage(levels[0]) );

          // implement sobel filter in x-direction
          Kernel1D<double> kx, ky;
          kx.initSymmetricGradient();
          ky.initBinomial(1);

          vigra::FImage sobel_image(m,n);
           vigra::convolveImage(srcImageRange(levels[0]), 
             destImage(sobel_image), kx, ky);

          //exportImage( srcImageRange(sobel_image ), vigra::ImageExportInfo("sobel.ppm") );
          //// define horizontal Sobel filter
          //vigra::Kernel2D<float> sobel;
          //vigra::FImage sobel_image(m,n);
          //sobel.initExplicitly(Diff2D(-1,-1), Diff2D(1,1)) =  // upper left and lower right
          //                     0.125, 0.0, -0.125,
          //                     0.25,  0.0, -0.25,
          //                     0.125, 0.0, -0.125;

          //vigra::convolveImage(
          //  srcImageRange(base_image), 
          //  destImage(sobel_image ), 
          //  kernel2d(sobel));

          //exportImage( srcImageRange(sobel_image), 
          //          vigra::ImageExportInfo("sobel.ppm") );

          //sobel.normalize();
          //sobel.setBorderTreatment(BORDER_TREATMENT_AVOID);


          //vigra::FImage  gradx( base_image.width(), base_image.height() );
          //vigra::FImage  grady( base_image.width(), base_image.height() );


         // // calculate gradient vector at scale = 3.0
         // vigra::gaussianGradient(srcImageRange(base_image),
         //                          destImage(gradx), destImage(grady), 3.0);


         // vigra::FImage hxx(base_image.width(), base_image.height());
         // vigra::FImage hxy(base_image.width(), base_image.height());
         // vigra::FImage hyy(base_image.width(), base_image.height());

         // // calculate Hessian of Gaussian at scale = 3.0
         // vigra::hessianMatrixOfGaussian(srcImageRange(base_image),
         //     destImage(hxx), destImage(hxy), destImage(hyy), 3.0);

         // vigra::FImage l_o_g(base_image.width(), base_image.height());
         // // calculate Laplacian of Gaussian at scale = 3.0
         // vigra::laplacianOfGaussian(srcImageRange(base_image), 
         //                 destImage(l_o_g), 3.0);


          
          for(int i=1; i < k_levels; ++i)
          {
              // reduce gray image 3 times
              reduceToNextLevel(levels[i-1], levels[i]);

          }
            
        if(nlhs > 0)
        {  
        //
        const char *field_names[] = {"l0", "l1","l2","l3", "l4"};
        mwSize ndim = 2;
        const mwSize dims[] = {1, 1};
        int fields = 5;

        //creates an empty struct
        MX_OUTPUT_PYR = mxCreateStructArray(
                    ndim,
                    dims, 
                    fields,
                    field_names);
        //
        int l0_field = mxGetFieldNumber(MX_OUTPUT_PYR,"l0");
        int l1_field = mxGetFieldNumber(MX_OUTPUT_PYR,"l1");
        int l2_field = mxGetFieldNumber(MX_OUTPUT_PYR,"l2");
        int l3_field = mxGetFieldNumber(MX_OUTPUT_PYR,"l3");
        int l4_field = mxGetFieldNumber(MX_OUTPUT_PYR,"l4");

        //
        mxClassID mx_tag = mxSINGLE_CLASS;

        //
        mxArray* mx_l0 = 
          create_array_from_vigra_image (levels[0], mx_tag);
        mxArray* mx_l1 = 
          create_array_from_vigra_image (levels[1], mx_tag);
        mxArray* mx_l2 = 
          create_array_from_vigra_image (levels[2], mx_tag);
        mxArray* mx_l3 = 
          create_array_from_vigra_image (levels[3], mx_tag);
        mxArray* mx_l4 = 
          create_array_from_vigra_image (levels[4], mx_tag);
 

        //
        mxSetFieldByNumber(MX_OUTPUT_PYR, 0, l0_field, mx_l0);
        //
        mxSetFieldByNumber(MX_OUTPUT_PYR, 0, l1_field, mx_l1);
        //
        mxSetFieldByNumber(MX_OUTPUT_PYR, 0, l2_field, mx_l2);
        //
        mxSetFieldByNumber(MX_OUTPUT_PYR, 0, l3_field, mx_l3);
        //
        mxSetFieldByNumber(MX_OUTPUT_PYR, 0, l4_field, mx_l4);

       // if(nlhs > 1)
       // {
       // vigra::FImage  grad( base_image.width(), base_image.height() );
       //// calculate gradient magnitude at scale = 3.0
       // vigra::gaussianGradientMagnitude(srcImageRange(base_image), 
       //                                             destImage(grad), 3.0);
       // //
       // MX_OUTPUT_GRAD = mxCreateNumericArray(ndim, 
       //                                         level0_dims, 
       //                                         mxSINGLE_CLASS, 
       //                                         mxREAL);
       // vigra::FImage::pointer  mx_grad_ptr = 
       //         static_cast<vigra::FImage::pointer>(mxGetData(MX_OUTPUT_GRAD));

       // memcpy(mx_grad_ptr, grad.data(), (grad.end() - grad.begin())*4);
       // }

        }


        //exportImage( srcImageRange(levels[0]), vigra::ImageExportInfo("level0.ppm") );
        //exportImage( srcImageRange(levels[1]), vigra::ImageExportInfo("level1.ppm") );
        //exportImage( srcImageRange(levels[2]), vigra::ImageExportInfo("level2.ppm") );
        //exportImage( srcImageRange(levels[3]), vigra::ImageExportInfo("level3.ppm") );
        //exportImage( srcImageRange(levels[4]), vigra::ImageExportInfo("level4.ppm") );

        //exportImage( srcImageRange(gradx), vigra::ImageExportInfo("gradx.ppm") );
        //exportImage( srcImageRange(grady), vigra::ImageExportInfo("grady.ppm") );
        //exportImage( srcImageRange(grad), vigra::ImageExportInfo("grad.ppm") );

        //exportImage( srcImageRange(hxx), vigra::ImageExportInfo("hxx.ppm") );
        //exportImage( srcImageRange(hxy), vigra::ImageExportInfo("hxy.ppm") );
        //exportImage( srcImageRange(hyy), vigra::ImageExportInfo("hyy.ppm") );

        //exportImage( srcImageRange(l_o_g), vigra::ImageExportInfo("laplacian_of_gaussian.ppm") );
        //printf("Done\n");
        } 

    }
    catch (vigra::StdException & e)
    {
        std::cout << e.what() << std::endl;
        return;
    }

    }
   else
   {
     printf("Image is not Uint8\n");
   }

  }

}