/////////////////////////////////////////////////////////////////////////////
// Name:        wx_image_panel.h
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     18/03/2007 16:06:56
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_IMAGE_PANEL_H_
#define _WX_IMAGE_PANEL_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "wx_image_panel.h"
#endif

/*!
 * Includes
 */

////@begin includes
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wx_image_panel;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID__IMAGE_PANEL 10004
#define SYMBOL_WX_IMAGE_PANEL_STYLE wxNO_BORDER|wxTAB_TRAVERSAL
#define SYMBOL_WX_IMAGE_PANEL_IDNAME ID__IMAGE_PANEL
#define SYMBOL_WX_IMAGE_PANEL_SIZE wxSize(320, 240)
#define SYMBOL_WX_IMAGE_PANEL_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * wx_image_panel class declaration
 */

class wx_image_panel: public wxPanel
{    
    DECLARE_DYNAMIC_CLASS( wx_image_panel )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    wx_image_panel();
    wx_image_panel( wxWindow* parent, wxWindowID id = SYMBOL_WX_IMAGE_PANEL_IDNAME, const wxPoint& pos = SYMBOL_WX_IMAGE_PANEL_POSITION, const wxSize& size = SYMBOL_WX_IMAGE_PANEL_SIZE, long style = SYMBOL_WX_IMAGE_PANEL_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_WX_IMAGE_PANEL_IDNAME, const wxPoint& pos = SYMBOL_WX_IMAGE_PANEL_POSITION, const wxSize& size = SYMBOL_WX_IMAGE_PANEL_SIZE, long style = SYMBOL_WX_IMAGE_PANEL_STYLE );

    /// Destructor
    ~wx_image_panel();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin wx_image_panel event handler declarations

    /// wxEVT_PAINT event handler for ID__IMAGE_PANEL
    void OnPaint( wxPaintEvent& event );

////@end wx_image_panel event handler declarations

////@begin wx_image_panel member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end wx_image_panel member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin wx_image_panel member variables
////@end wx_image_panel member variables
};

#endif
    // _WX_IMAGE_PANEL_H_
