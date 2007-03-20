/////////////////////////////////////////////////////////////////////////////
// Name:        wx_img_stream_panel_t.cpp
// Purpose:     
// Author:      
// Modified by: 
// Created:     20/03/2007 18:19:51
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "wx_img_stream_panel_t.hpp"
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

#include "wx_img_stream_panel_t.hpp"

////@begin XPM images

////@end XPM images

/*!
 * wx_img_stream_panel_t type definition
 */

IMPLEMENT_DYNAMIC_CLASS( wx_img_stream_panel_t, wxPanel )

/*!
 * wx_img_stream_panel_t event table definition
 */

BEGIN_EVENT_TABLE( wx_img_stream_panel_t, wxPanel )

////@begin wx_img_stream_panel_t event table entries
////@end wx_img_stream_panel_t event table entries

END_EVENT_TABLE()

/*!
 * wx_img_stream_panel_t constructors
 */

wx_img_stream_panel_t::wx_img_stream_panel_t(char* ini_file)
{
	m_stream_ini = ini_file;

    Init();
}

wx_img_stream_panel_t::wx_img_stream_panel_t( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, pos, size, style);
}

/*!
 * wx_img_stream_panel_t creator
 */

bool wx_img_stream_panel_t::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
////@begin wx_img_stream_panel_t creation
    wxPanel::Create( parent, id, pos, size, style );

    CreateControls();
////@end wx_img_stream_panel_t creation
    return true;
}

/*!
 * wx_img_stream_panel_t destructor
 */

wx_img_stream_panel_t::~wx_img_stream_panel_t()
{
////@begin wx_img_stream_panel_t destruction
////@end wx_img_stream_panel_t destruction
}

/*!
 * Member initialisation 
 */

void wx_img_stream_panel_t::Init()
{
////@begin wx_img_stream_panel_t member initialisation
////@end wx_img_stream_panel_t member initialisation
}
/*!
 * Control creation for wx_img_stream_panel_t
 */

void wx_img_stream_panel_t::CreateControls()
{    
	m_stream_dest.reset(new wx_stream_dest_t(this));

	m_stream_client.reset (new all::core::stream_client_t(m_stream_dest, m_stream_ini));
	m_stream_client->run_async();
}

/*!
 * Should we show tooltips?
 */

bool wx_img_stream_panel_t::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap wx_img_stream_panel_t::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin wx_img_stream_panel_t bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end wx_img_stream_panel_t bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon wx_img_stream_panel_t::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin wx_img_stream_panel_t icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end wx_img_stream_panel_t icon retrieval
}
