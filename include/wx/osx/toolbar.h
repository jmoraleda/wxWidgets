/////////////////////////////////////////////////////////////////////////////
// Name:        wx/osx/toolbar.h
// Purpose:     wxToolBar class
// Author:      Stefan Csomor
// Created:     1998-01-01
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_TOOLBAR_H_
#define _WX_TOOLBAR_H_

#if wxUSE_TOOLBAR

#include "wx/tbarbase.h"
#include "wx/dynarray.h"

class WXDLLIMPEXP_CORE wxToolBar: public wxToolBarBase
{
    wxDECLARE_DYNAMIC_CLASS(wxToolBar);
public:
  /*
   * Public interface
   */

   wxToolBar() { Init(); }

  wxToolBar(wxWindow *parent, wxWindowID id,
                   const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                   long style = wxTB_DEFAULT_STYLE,
                   const wxString& name = wxASCII_STR(wxToolBarNameStr))
  {
    Init();
    Create(parent, id, pos, size, style, name);
  }
  virtual ~wxToolBar();

  bool Create(wxWindow *parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
            long style = wxTB_DEFAULT_STYLE,
            const wxString& name = wxASCII_STR(wxToolBarNameStr));

    virtual void SetWindowStyleFlag(long style) override;

    virtual bool Destroy() override;

    // override/implement base class virtuals
    virtual wxToolBarToolBase *FindToolForPosition(wxCoord x, wxCoord y) const override;

#ifndef __WXOSX_IPHONE__
    virtual bool Show(bool show = true) override;
    virtual bool IsShown() const override;
#endif
    virtual bool Realize() override;

    virtual wxSize GetToolSize() const override;

    virtual void SetRows(int nRows) override;


    virtual void SetToolNormalBitmap(int id, const wxBitmapBundle& bitmap) override;
    virtual void SetToolDisabledBitmap(int id, const wxBitmapBundle& bitmap) override;

#ifndef __WXOSX_IPHONE__
    // Add all the buttons

    virtual wxString MacGetToolTipString( wxPoint &where ) override;
    void OnPaint(wxPaintEvent& event) ;
    void OnMouse(wxMouseEvent& event) ;
    virtual void MacSuperChangedPosition() override;
#endif

#if wxOSX_USE_NATIVE_TOOLBAR
    // make all tools selectable
    void OSXSetSelectableTools(bool set);
    void OSXSelectTool(int toolId);

    bool MacInstallNativeToolbar(bool usesNative);
    void MacUninstallNativeToolbar();
    bool MacWantsNativeToolbar();
    bool MacTopLevelHasNativeToolbar(bool *ownToolbarInstalled) const;
#endif

    virtual wxToolBarToolBase *CreateTool(int id,
                                          const wxString& label,
                                          const wxBitmapBundle& bmpNormal,
                                          const wxBitmapBundle& bmpDisabled = wxNullBitmap,
                                          wxItemKind kind = wxITEM_NORMAL,
                                          wxObject *clientData = nullptr,
                                          const wxString& shortHelp = wxEmptyString,
                                          const wxString& longHelp = wxEmptyString) override;
    virtual wxToolBarToolBase *CreateTool(wxControl *control,
                                          const wxString& label) override;

protected:
    // common part of all ctors
    void Init();

    void DoLayout();

    void DoSetSize(int x, int y, int width, int height, int sizeFlags) override;

#ifndef __WXOSX_IPHONE__
    virtual void DoGetSize(int *width, int *height) const override;
    virtual wxSize DoGetBestSize() const override;
#endif
#ifdef __WXOSX_COCOA__
    virtual void DoGetPosition(int*x, int *y) const override;
#endif
    virtual bool DoInsertTool(size_t pos, wxToolBarToolBase *tool) override;
    virtual bool DoDeleteTool(size_t pos, wxToolBarToolBase *tool) override;

    virtual void DoEnableTool(wxToolBarToolBase *tool, bool enable) override;
    virtual void DoToggleTool(wxToolBarToolBase *tool, bool toggle) override;
    virtual void DoSetToggle(wxToolBarToolBase *tool, bool toggle) override;

    virtual void DoSetToolBitmapSize(const wxSize& size) override;

    wxDECLARE_EVENT_TABLE();
#if wxOSX_USE_NATIVE_TOOLBAR
    bool m_macUsesNativeToolbar ;
    void* m_macToolbar ;
#endif
#ifdef __WXOSX_IPHONE__
    WX_UIView m_macToolbar;
#endif

private:
#if wxOSX_USE_NATIVE_TOOLBAR
    wxString FormatToolId(const wxToolBarToolBase *tool) const;
#endif
};

#ifdef __WXOSX_COCOA__

class WXDLLIMPEXP_CORE wxToolBarTool : public wxToolBarToolBase
{
public:
    wxToolBarTool(
        wxToolBar *tbar,
        int id,
        const wxString& label,
        const wxBitmapBundle& bmpNormal,
        const wxBitmapBundle& bmpDisabled,
        wxItemKind kind,
        wxObject *clientData,
        const wxString& shortHelp,
        const wxString& longHelp );

    wxToolBarTool(wxToolBar *tbar, wxControl *control, const wxString& label);

    virtual ~wxToolBarTool();

    WXWidget GetControlHandle()
    {
        return (WXWidget) m_controlHandle;
    }

    void SetControlHandle( WXWidget handle )
    {
        m_controlHandle = handle;
    }

    void SetPosition( const wxPoint& position );

    void ClearControl();

    wxSize GetSize() const;

    wxPoint GetPosition() const
    {
        return wxPoint( m_x, m_y );
    }

    bool Enable( bool enable ) override;

    void UpdateImages();

    void UpdateToggleImage( bool toggle );

    void UpdateLabel();

    void Action()
    {
        wxToolBar *tbar = (wxToolBar*) GetToolBar();
        if (CanBeToggled())
        {
            bool    shouldToggle;

            shouldToggle = !IsToggled();
            tbar->ToggleTool( GetId(), shouldToggle );
        }

        tbar->OnLeftClick( GetId(), IsToggled() );
    }

#if wxOSX_USE_NATIVE_TOOLBAR
    void SetToolbarItemRef( WX_NSToolbarItem ref );

    WX_NSToolbarItem GetToolbarItemRef() const
    {
        return m_toolbarItem;
    }

    void SetIndex( CFIndex idx )
    {
        m_index = idx;
    }

    CFIndex GetIndex() const
    {
        return m_index;
    }

    virtual void SetLabel(const wxString& label) override;

    virtual bool SetShortHelp(const wxString& help) override;
#endif // wxOSX_USE_NATIVE_TOOLBAR

private:
    void Init();

    WXWidget m_controlHandle;
    wxCoord     m_x;
    wxCoord     m_y;
    wxBitmap    m_alternateBitmap;

#if wxOSX_USE_NATIVE_TOOLBAR
    WX_NSToolbarItem m_toolbarItem;
    // position in its toolbar, -1 means not inserted
    CFIndex m_index;
#endif
};

#endif // __WXOSX_COCOA__

#ifdef __WXOSX_IPHONE__

class WXDLLIMPEXP_CORE wxToolBarTool : public wxToolBarToolBase
{
public:
    wxToolBarTool(
                  wxToolBar *tbar,
                  int id,
                  const wxString& label,
                  const wxBitmapBundle& bmpNormal,
                  const wxBitmapBundle& bmpDisabled,
                  wxItemKind kind,
                  wxObject *clientData,
                  const wxString& shortHelp,
                  const wxString& longHelp );

    wxToolBarTool(wxToolBar *tbar, wxControl *control, const wxString& label);

    virtual ~wxToolBarTool();

    void Action()
    {
        wxToolBar *tbar = (wxToolBar*) GetToolBar();
        if (CanBeToggled())
        {
            bool    shouldToggle;

            shouldToggle = !IsToggled();
            tbar->ToggleTool( GetId(), shouldToggle );
        }

        tbar->OnLeftClick( GetId(), IsToggled() );
    }

    void MakeStretchable() override;

    WX_UIBarButtonItem GetUIBarButtonItem() const {return m_toolbarItem;}
private:

    void Init()
    {
        m_toolbarItem = nullptr;
        m_index = -1;
    }

    void RemoveAssociation();

    WX_UIBarButtonItem m_toolbarItem;
    // position in its toolbar, -1 means not inserted
    CFIndex m_index;
};

#endif // __WXOSX_IPHONE__

#endif // wxUSE_TOOLBAR

#endif
    // _WX_TOOLBAR_H_
