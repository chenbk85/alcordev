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

#include "alcor/core/wx_stream_dest_t.hpp"
#include "alcor/core/stream_client_t.hpp"
#include "alcor/core/core.h"

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

    void clear_selection()
    {
      selection_h = 1;
      selection_w = 1;
    };

////@begin wx_image_panel event handler declarations

    /// wxEVT_DESTROY event handler for ID__IMAGE_PANEL
    void OnDestroy( wxWindowDestroyEvent& event );

    /// wxEVT_PAINT event handler for ID__IMAGE_PANEL
    void OnPaint( wxPaintEvent& event );

    /// wxEVT_LEFT_UP event handler for ID__IMAGE_PANEL
    void OnLeftUp( wxMouseEvent& event );

    /// All mouse events event handler for ID__IMAGE_PANEL
    void OnMouse( wxMouseEvent& event );

////@end wx_image_panel event handler declarations

////@begin wx_image_panel member function declarations

    float Get_topleft_x() const { return beginx ; }

    float Get_topleft_y() const { return beginy ; }

    size_t Get_roi_height() const { return selection_h ; }

    size_t Get_roi_width() const { return selection_w ; }


    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end wx_image_panel member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin wx_image_panel member variables
public:
    float beginx;
    float beginy;
    size_t selection_h;
    size_t selection_w;
private:
    bool is_dragging;
////@end wx_image_panel member variables

    ///STREAMING OBJECTS!!
    wx_stream_dest_t            stream_dest;
    //raw pointer ... seems ... better
    all::core::stream_client_t* stream_ptr;  

    ///
    enum 
    {
      ID_TIMER_EVENT = 7117182
    };
    ///on_timer
	  void on_timer_(wxTimerEvent&);
    ///
    wxTimer* m_timer;

    ///Drawing Routine 
    void draw_image_panel(wxDC&);

    ///update_event callback
    void update_image(const core::jpeg_data_t&); 

    ///
    core::jpeg_data_t my_jpeg_data;
};

#endif
    // _WX_IMAGE_PANEL_H_
