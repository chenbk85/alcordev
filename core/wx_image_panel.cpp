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

////@end wx_image_panel event table entries

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
  //link drawing callback
  stream_dest.set_draw_callback(boost::bind(&wx_image_panel::draw_image_panel, this, _1));

  //create streaming client endpoint
  stream_ptr  = 
    new all::core::stream_client_t(stream_dest, "config/trm_stream_client.ini");
  //run!
  stream_ptr->run_async();

  //wxMessageBox(_T("stream_ptr created and running"));

////@begin wx_image_panel member initialisation
////@end wx_image_panel member initialisation
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
////@begin wxEVT_PAINT event handler for ID__IMAGE_PANEL in wx_image_panel.
    // Before editing this code, remove the block markers.
    wxPaintDC dc(this);
////@end wxEVT_PAINT event handler for ID__IMAGE_PANEL in wx_image_panel. 
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
void wx_image_panel::draw_image_panel(const core::jpeg_data_t& todraw)
{
  wxClientDC dc_client(this);

  //wxPaintDC dc_client(this);

  wxImage cam_image(todraw.width, todraw.height, reinterpret_cast <unsigned char*> (todraw.data.get()), true);

  wxBitmap cam_bmp(cam_image, 24);

  PrepareDC(dc_client);
  dc_client.DrawBitmap(cam_bmp, 0, 0, false);
}
