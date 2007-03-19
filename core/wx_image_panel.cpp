/////////////////////////////////////////////////////////////////////////////
// Name:        wx_image_panel.cpp
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     18/03/2007 16:06:56
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "wx_image_panel.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes
#include <wx/dcbuffer.h>
#include "wx_image_panel.h"

////@begin XPM images
////@end XPM images

/*!
 * wx_image_panel type definition
 */

IMPLEMENT_DYNAMIC_CLASS( wx_image_panel, wxPanel )

/*!
 * wx_image_panel event table definition
 */

BEGIN_EVENT_TABLE( wx_image_panel, wxPanel )

////@begin wx_image_panel event table entries
    EVT_WINDOW_DESTROY( wx_image_panel::OnDestroy )
    EVT_PAINT( wx_image_panel::OnPaint )
    EVT_LEFT_UP( wx_image_panel::OnLeftUp )
    EVT_MOUSE_EVENTS( wx_image_panel::OnMouse )

////@end wx_image_panel event table entries
    EVT_TIMER(wx_image_panel::ID_TIMER_EVENT, wx_image_panel::on_timer_)
END_EVENT_TABLE()

/*!
 * wx_image_panel constructors
 */

wx_image_panel::wx_image_panel()
{
    Init();
}

wx_image_panel::wx_image_panel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, pos, size, style);
}

/*!
 * wx_image_panel creator
 */

bool wx_image_panel::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
////@begin wx_image_panel creation
    wxPanel::Create( parent, id, pos, size, style );

    CreateControls();
////@end wx_image_panel creation
    return true;
}

/*!
 * wx_image_panel destructor
 */

wx_image_panel::~wx_image_panel()
{
////@begin wx_image_panel destruction
////@end wx_image_panel destruction
}

/*!
 * Member initialisation 
 */

void wx_image_panel::Init()
{
  stream_dest.reset(new wx_stream_dest_t);

  //link drawing callback
  stream_dest->set_update_callback(boost::bind(&wx_image_panel::update_image, this, _1));

  //create streaming client endpoint
  stream_ptr  = 
    new all::core::stream_client_t(stream_dest, "config/trm_stream_client.ini");
  //run!
  stream_ptr->run_async();

  //wxMessageBox(_T("stream_ptr created and running"));

////@begin wx_image_panel member initialisation
    beginx = 0;
    beginy = 0;
    is_dragging = false;
    selection_h = 1;
    selection_w = 1;
    dragging_enabled = false;
////@end wx_image_panel member initialisation
    m_timer = new wxTimer(this, ID_TIMER_EVENT);
    m_timer->Start(75);
}

/*!
 * Control creation for wx_image_panel
 */

void wx_image_panel::CreateControls()
{    
////@begin wx_image_panel content construction
    // Connect events and objects
    Connect(ID__IMAGE_PANEL, wxEVT_DESTROY, wxWindowDestroyEventHandler(wx_image_panel::OnDestroy), NULL, this);
////@end wx_image_panel content construction
}

/*!
 * Should we show tooltips?
 */

bool wx_image_panel::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap wx_image_panel::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin wx_image_panel bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end wx_image_panel bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon wx_image_panel::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin wx_image_panel icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end wx_image_panel icon retrieval
}

/*!
 * wxEVT_PAINT event handler for ID__IMAGE_PANEL
 */

void wx_image_panel::OnPaint( wxPaintEvent& event )
{
  wxBufferedPaintDC dc(this);

  draw_image_panel(dc);

  if(dragging_enabled)
  {
    if(is_dragging || selection_h>1)
    {
      draw_model_selection(dc);
    }
  }
}

/*!
 * wxEVT_DESTROY event handler for ID__IMAGE_PANEL
 */

void wx_image_panel::OnDestroy( wxWindowDestroyEvent& event )
{
  stream_ptr->stop();
  //delete stream_ptr;
}

/*!
 * Drawing Routine (called from stream_dest)
 */
void wx_image_panel::draw_image_panel(wxDC& dc)
{     

  //PrepareDC(dc);

  if(my_jpeg_data.size > 0)
  {
    wxImage cam_image(my_jpeg_data.width
                    , my_jpeg_data.height, 
                    reinterpret_cast <unsigned char*> (my_jpeg_data.data.get())
                    , true);
    wxBitmap cam_bmp(cam_image, 24);

    //wxMemoryDC temp_dc;
    //temp_dc.SelectObject(cam_bmp);

    dc.DrawBitmap(cam_bmp, 0, 0, false);
  }

}

/*!
 * Draw Selection.
 */
void wx_image_panel::draw_model_selection(wxDC& dc)
{
  dc.SetPen(*wxGREEN_PEN);
  dc.SetBrush( *wxTRANSPARENT_BRUSH );
  dc.DrawRectangle(beginx, beginy,selection_w, selection_h);
}

/*!
 * Updating Routine (called from stream_dest)
 */
void wx_image_panel::update_image(const core::jpeg_data_t& newimage)
{
  if(newimage.size != my_jpeg_data.size)
  {
    my_jpeg_data.data.reset(new core::uint8_t[newimage.size]);
    my_jpeg_data.size = newimage.size;
    my_jpeg_data.height = newimage.height;
    my_jpeg_data.width = newimage.width;
    my_jpeg_data.depth = newimage.depth;
    //my_jpeg_data.data = newimage.data;    
    my_jpeg_data.size = newimage.size;
  }
  my_jpeg_data.data = newimage.data;
}


/*!
 * All mouse events event handler for ID__IMAGE_PANEL
 */

void wx_image_panel::OnMouse( wxMouseEvent& event )
{
  if (event.Dragging())
  {
    if(event.LeftIsDown())
    {
      if(is_dragging)
      {
        selection_h = event.GetY() - beginy;
        selection_w = event.GetX() - beginx;
      }
      else
      {  
        beginx = event.GetX();
        beginy = event.GetY();
        is_dragging=true;
        selection_h = 1;
        selection_w = 1;
      }

    }
  }

}


/*!
 * wxEVT_LEFT_UP event handler for ID__IMAGE_PANEL
 */

void wx_image_panel::OnLeftUp( wxMouseEvent& event )
{
  is_dragging=false;

}

/*!
 * EVT_TIMER event handler for timer
 */
void wx_image_panel::on_timer_(wxTimerEvent&)
{
  Refresh(false);
}
