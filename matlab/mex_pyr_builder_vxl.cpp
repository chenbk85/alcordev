//MATLAB Includes
#include "mex.h"
//VXL Includes
#include <vcl_iostream.h>

#include <vimt/vimt_image_pyramid.h>
#include <vimt/vimt_image_2d_of.h>
#include <vimt/vimt_gaussian_pyramid_builder_2d.h>
#include <vimt/vimt_dog_pyramid_builder_2d.h>

#include <vil/vil_image_view.h>
#include <vil/vil_convert.h>
#include <vil/algo/vil_sobel_3x3.h>
#include <vil/vil_convert.h>
#include <vil/vil_save.h>
#include <vil\algo\vil_histogram_equalise.h>
#include <vil/vil_transpose.h>
#include <vxl_config.h> // for vxl_byte
//#include <vcl_iostream.h>

//Linker
#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "vsl.lib")
#pragma comment (lib, "vcl.lib")
#pragma comment (lib, "vil.lib")
#pragma comment (lib, "vil_algo.lib")
#pragma comment (lib, "vil_io.lib")
#pragma comment (lib, "png.lib")
#pragma comment (lib, "tiff.lib")
#pragma comment (lib, "jpeg.lib")
#pragma comment (lib, "vnl.lib")
//#pragma comment (lib, "vsl.lib")
#pragma comment (lib, "vgl.lib")
#pragma comment (lib, "vimt.lib")
//#pragma comment (lib, "vimt_algo.lib")
//#pragma comment (lib, "vul.lib")
#pragma comment (lib, "z.lib")
//#pragma comment (lib, "netlib.lib")
//#pragma comment (lib, "v3p_netlib.lib")
//#pragma comment (lib, "vimt_algo.lib")
//Local Defines
#define MX_INPUT_IMAGE prhs[0]

void mexFunction( int nlhs, mxArray* plhs[], int nrhs, const mxArray
*prhs[] )
{

  if(nrhs > 0)
  {
   if ( mxIsUint8(MX_INPUT_IMAGE) )
    {
    vxl_byte* input_image_ptr = static_cast<vxl_byte*>(mxGetData(MX_INPUT_IMAGE));

    size_t n = mxGetN(MX_INPUT_IMAGE);
    size_t m = mxGetM(MX_INPUT_IMAGE);
    //const mwSize* im_size = mxGetDimensions(MX_INPUT_IMAGE);

    printf("Size: M: %d N:%d\n", m, n);


    //
    vil_image_view<vxl_byte> base_image_view(input_image_ptr 
                                             ,n,m //
                                             ,1,1
                                             ,1
                                             ,m*n);  

    vil_image_view<vxl_byte> base_image_view2(input_image_ptr 
                                             ,m,n //
                                             ,1,1 
                                             ,1
                                             ,m*n);

    //I tried this one and it succeded ... but if passed as
    //argument to algorithms it fails.
    //This seems to me a logical way to build a 
    //vil_image_view from a column major raw memory array
    // M rows and N columns.

    vil_image_view<vxl_byte> cmajor_image(input_image_ptr 
                                             ,n,m //
                                             ,m,1 
                                             ,1
                                             ,m*n);

    base_image_view;
    base_image_view2;
    printf("Vil images Created\n");

    printf("save base_image_view\n");
    vil_save(base_image_view , "base_image_view_1.pnm");
    vil_save(base_image_view2 , "base_image_view_2.pnm");


    printf ("Transpose_1\n");
    vil_image_view<vxl_byte> transposed_view  = 
            vil_transpose(base_image_view);

    printf ("Transpose_2\n");
    vil_image_view<vxl_byte> transposed_view2  = 
            vil_transpose(base_image_view2);

    printf("Saving Transposed\n");    
    vil_save(transposed_view,  "transposed_view_1.pnm");
    vil_save(transposed_view2, "transposed_view_2.ppm");        


    printf("vil_histogram_equalise 1\n");
    vil_histogram_equalise(transposed_view);
    
    printf("vil_histogram_equalise 2\n");
    vil_histogram_equalise(transposed_view2);

    printf("save base_image_view hist\n");
    vil_save(transposed_view , "vil_histogram_equalise_1.ppm");
    printf("save base_image_view hist\n");
    vil_save(transposed_view2 , "vil_histogram_equalise_2.ppm");

    //     
    printf("Vimt Pyramid\n");
    vimt_image_2d_of<vxl_byte>  base_image_b(transposed_view);

    vimt_image_pyramid image_pyr;
    vimt_gaussian_pyramid_builder_2d<vxl_byte> builder;

    printf("Build Pyramid\n");
    builder.build(image_pyr, base_image_b);
    
    printf("Levels: %d\n", image_pyr.n_levels());

    //save level 2
    vimt_image_2d_of<float>   level2;
    vil_image_view<vxl_byte>  level_2image;
    level2 = static_cast<vimt_image_2d_of<float>& > (image_pyr(2)) ;
    vil_convert_stretch_range(level2.image(),level_2image);

    vil_save(level_2image,"level2.ppm");

    printf("Flattening\n");
    vimt_image_2d_of<vxl_byte>  gauss_pyr;
    vimt_image_pyramid_flatten(gauss_pyr, image_pyr);


    ////
    printf("Saving\n");
    vil_save(gauss_pyr.image(),"pyramid.ppm");


    //DOG

    vimt_image_2d_of<float> image_f, flat_dog, flat_smooth;
    vil_convert_cast(transposed_view, image_f.image());

    vimt_image_pyramid dog_pyramid,smooth_pyramid;
    vimt_dog_pyramid_builder_2d<float> pyr_builder;
    pyr_builder.build_dog(dog_pyramid,smooth_pyramid,image_f);

    vimt_image_pyramid_flatten(flat_dog,dog_pyramid);
    vimt_image_pyramid_flatten(flat_smooth,smooth_pyramid);

    vil_image_view<vxl_byte> out_dog;
    vil_convert_stretch_range(flat_dog.image(),out_dog);
    vil_save(out_dog, "dog_pyramid.png");
    vcl_cout<<"DoG saved "<< vcl_endl;

    vil_image_view<vxl_byte> out_smooth;
    vil_convert_stretch_range(flat_smooth.image(),out_smooth);
    vil_save(out_smooth,"smooth_pyramid.png");
    vcl_cout<< "Smooth pyramid saved" <<vcl_endl;

    }
     else
     {
       printf("Image is not Uint8\n");
     }

  }

}