#include "opencv_data_source_t.h"

namespace all{ namespace sense {

  opencv_data_source_t::opencv_data_source_t() :	m_jpeg_quality (100)
{

}

void opencv_data_source_t::init_()
{
  m_cam.reset(new sense::opencv_grabber_t);

  if(m_cam->open(core::open_camera))
  {
    image.reset( new core::uint8_t[m_cam->width()*m_cam->height()*m_cam->channels()] );

    m_encoder.reset(new core::jpeg_encoder_t);
  
    switch(m_cam->channels())
    {
    case 3:
      m_encoder->reset
        (all::core::rgb_tag, all::core::planar_tag, m_cam->height(), m_cam->width());
      break;

    case 1:
      m_encoder->reset(all::core::gray_tag, m_cam->height(), m_cam->width());
      break;    

    default:
      m_encoder.reset();
      break;
    }
  }

}

void opencv_data_source_t::set_quality(unsigned int quality) 
{
	m_jpeg_quality = quality;
}

int opencv_data_source_t::get_data(all::core::uint8_ptr* data) 
{
  //printf("get_data\n");
  size_t buffer_size = 0;

  //Se non lo faccio qui .. opencv rifiuta di funzionare.
  //Deve stare nel thread principale.
  if(!m_cam || !m_encoder) init_();

	//        
  //printf("get_color_buffer\n");
  if (m_cam->get_color_buffer(image))
  {
    /*get_color_buffer*/
    if(m_encoder)
    {  
      all::core::jpeg_data_t jpeg_data;
      //
        //printf("encode\n");
      jpeg_data = m_encoder->encode(image, m_jpeg_quality);

      buffer_size = jpeg_data.size + sizeof(jpeg_data.crc);

      all::core::uint8_ptr return_data = new all::core::uint8_t[buffer_size];

      all::core::uint8_ptr write_ptr = return_data;

      memcpy(write_ptr, &(jpeg_data.crc), sizeof(jpeg_data.crc));
      //
      write_ptr+=sizeof(jpeg_data.crc);

      memcpy(write_ptr, jpeg_data.data.get(), jpeg_data.size);

      *data = return_data;
    }
  }
  else
  {
    printf("Cannot grab\n");
  }

  return buffer_size;
}


}}