/////////////////////////////////////////////////////////////////////////////
// Name:        wx_img_stream_panel_t.hpp
// Purpose:     
// Author:      
// Modified by: 
// Created:     20/03/2007 18:19:51
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_IMG_STREAM_PANEL_T_H_
#define _WX_IMG_STREAM_PANEL_T_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "wx_img_stream_panel_t.hpp"
#endif

/*!
 * Includes
 */

////@begin includes
////@end includes
#include <alcor/core/stream_client_t.hpp>
#include "wx_stream_dest_t.hpp"

/*!
 * Forward declarations
 */

////@begin forward declarations
class wx_img_stream_panel_t;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * wx_img_stream_panel_t class declaration
 */

class wx_img_stream_panel_t: public wxPanel
{    
    DECLARE_DYNAMIC_CLASS( wx_img_stream_panel_t )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    wx_img_stream_panel_t(char* ini_file = "config/stream_client.ini");
    wx_img_stream_panel_t( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER);

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER);

    /// Destructor
    ~wx_img_stream_panel_t();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin wx_img_stream_panel_t event handler declarations
////@end wx_img_stream_panel_t event handler declarations

////@begin wx_img_stream_panel_t member function declarations
    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end wx_img_stream_panel_t member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin wx_img_stream_panel_t member variables
////@end wx_img_stream_panel_t member variables

	char* m_stream_ini;
	all::core::stream_client_ptr m_stream_client;
	wx_stream_dest_ptr m_stream_dest;

};

#endif
    // _WX_IMG_STREAM_PANEL_T_H_
