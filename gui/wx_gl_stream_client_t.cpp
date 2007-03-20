/////////////////////////////////////////////////////////////////////////////
// Name:        wx_gl_stream_client_t.cpp
// Purpose:     
// Author:      
// Modified by: 
// Created:     20/03/2007 19:55:47
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "wx_gl_stream_client_t.hpp"
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

#include "wx_gl_stream_client_t.hpp"
#include "trackball.h"

////@begin XPM images
////@end XPM images

/*!
 * wx_gl_stream_client_t type definition
 */

IMPLEMENT_DYNAMIC_CLASS( wx_gl_stream_client_t, wx_img_stream_panel_t )

/*!
 * wx_gl_stream_client_t event table definition
 */

BEGIN_EVENT_TABLE( wx_gl_stream_client_t, wx_img_stream_panel_t )

////@begin wx_gl_stream_client_t event table entries
    EVT_MOUSE_EVENTS( wx_gl_stream_client_t::on_mouse )

////@end wx_gl_stream_client_t event table entries

END_EVENT_TABLE()

/*!
 * wx_gl_stream_client_t constructors
 */

wx_gl_stream_client_t::wx_gl_stream_client_t(char* ini_file) : wx_img_stream_panel_t(ini_file)
{
    Init();
}

wx_gl_stream_client_t::wx_gl_stream_client_t( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, pos, size, style);
}

/*!
 * wx_gl_stream_client_t creator
 */

bool wx_gl_stream_client_t::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
////@begin wx_gl_stream_client_t creation
    wx_img_stream_panel_t::Create( parent, id, pos, size, style );

    CreateControls();
////@end wx_gl_stream_client_t creation
    return true;
}

/*!
 * wx_gl_stream_client_t destructor
 */

wx_gl_stream_client_t::~wx_gl_stream_client_t()
{
////@begin wx_gl_stream_client_t destruction
////@end wx_gl_stream_client_t destruction
}

/*!
 * Member initialisation 
 */

void wx_gl_stream_client_t::Init()
{
////@begin wx_gl_stream_client_t member initialisation
////@end wx_gl_stream_client_t member initialisation
	beginx = 0.0f;
    beginy = 0.0f;
    zoom   = 80.0f;
    fov    = 85.0f;
    xcam = 0.0f;
    ycam = 0.0f;
    zcam = -3.0f;
    trackball(quat, 0.0f, 0.0f, 0.0f, 0.0f);
}
/*!
 * Control creation for wx_gl_stream_client_t
 */

void wx_gl_stream_client_t::CreateControls()
{    
////@begin wx_gl_stream_client_t content construction
    wx_gl_stream_client_t* item_img_stream_panel_t1 = this;

////@end wx_gl_stream_client_t content construction
}

/*!
 * Should we show tooltips?
 */

bool wx_gl_stream_client_t::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap wx_gl_stream_client_t::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin wx_gl_stream_client_t bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end wx_gl_stream_client_t bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon wx_gl_stream_client_t::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin wx_gl_stream_client_t icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end wx_gl_stream_client_t icon retrieval
}
/*!
 * All mouse events event handler
 */

void wx_gl_stream_client_t::on_mouse( wxMouseEvent& event )
{
    if(event.GetEventType() == wxEVT_MOUSEWHEEL) {
		int mwheel = event.GetWheelRotation();
		if(mwheel>0) {
			zcam += 0.1;
      //(zcam > 10.0)? (zcam) : (10.0);
		}
		else if (mwheel < 0) {
			zcam -= 0.1;
		}
		all::core::net_packet_ptr_t packet (new all::core::net_packet_t());
  
		packet->double_to_buf(xcam);
		packet->double_to_buf(ycam);
		packet->double_to_buf(zcam);

		packet->double_to_buf(quat[0]);
		packet->double_to_buf(quat[1]);
		packet->double_to_buf(quat[2]);
		packet->double_to_buf(quat[3]);


		m_stream_client->send_source_cmd("boh", packet);
	}

	if (event.Dragging()) {
		if(event.LeftIsDown()) {
			wxSize sz(GetClientSize());

			/* drag in progress, simulate trackball */
			float spin_quat[4];
			trackball(spin_quat,
			(2.0*beginx - sz.x) / sz.x,
			(sz.y - 2.0*beginy) / sz.y,
			(2.0*event.GetX() - sz.x)    / sz.x,
			(sz.y - 2.0*event.GetY())    / sz.y);

			add_quats(spin_quat, quat, quat);
		}

		if(event.RightIsDown()) {
			wxSize sz(GetClientSize());
			/* orientation has changed, redraw mesh */
			//Refresh(false);
			if (beginx - event.GetX() > 0) {
				xcam-=0.1;
			}
			else if (beginx - event.GetX() < 0) {
				xcam+=0.1;
			}

      //if (beginy - event.GetY() > 0)
      //{
      //  ycam-=0.1;
      //}
      //else if (beginy - event.GetY() < 0)
      //{
      //  ycam+=0.1;
      //}
		}
		all::core::net_packet_ptr_t packet (new all::core::net_packet_t());
  
		packet->double_to_buf(xcam);
		packet->double_to_buf(ycam);
		packet->double_to_buf(zcam);

		packet->double_to_buf(quat[0]);
		packet->double_to_buf(quat[1]);
		packet->double_to_buf(quat[2]);
		packet->double_to_buf(quat[3]);


		m_stream_client->send_source_cmd("boh", packet);

	}

	beginx = event.GetX();
	beginy = event.GetY();
}


