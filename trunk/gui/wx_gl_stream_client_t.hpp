/////////////////////////////////////////////////////////////////////////////
// Name:        wx_gl_stream_client_t.hpp
// Purpose:     
// Author:      
// Modified by: 
// Created:     20/03/2007 19:55:47
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GL_STREAM_CLIENT_T_H_
#define _WX_GL_STREAM_CLIENT_T_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "wx_gl_stream_client_t.hpp"
#endif

/*!
 * Includes
 */

////@begin includes
////@end includes
#include "wx_img_stream_panel_t.hpp"

/*!
 * Forward declarations
 */

////@begin forward declarations
class wx_gl_stream_client_t;
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
 * wx_gl_stream_client_t class declaration
 */

class wx_gl_stream_client_t: public wx_img_stream_panel_t
{    
    DECLARE_DYNAMIC_CLASS( wx_gl_stream_client_t )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    wx_gl_stream_client_t(char* ini_file = "config/stream_server.ini");
    wx_gl_stream_client_t( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER );

    /// Destructor
    ~wx_gl_stream_client_t();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin wx_gl_stream_client_t event handler declarations

    /// All mouse events event handler for ID_UNIDENTIFIED
    void on_mouse( wxMouseEvent& event );

////@end wx_gl_stream_client_t event handler declarations

////@begin wx_gl_stream_client_t member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end wx_gl_stream_client_t member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin wx_gl_stream_client_t member variables
////@end wx_gl_stream_client_t member variables
    
    float beginx, beginy;       // position of mouse
    float quat[4];              // orientation of object
    float zoom;
    float fov;
    float xcam;
    float ycam;
    float zcam;
};

#endif
    // _WX_GL_STREAM_CLIENT_T_H_
