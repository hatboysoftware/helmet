//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Topper Trading Workbench
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Workbench/I_Frame.hpp>

#include <wx/panel.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class TopFrame;

class Frame
:   public Helmet::Workbench::I_Frame
,   public wxPanel
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Frame implementation
    /// @{
public:
    virtual void setName(const std::string& _name) override;
    virtual const std::string getName() const override;
    virtual void addChildFrame(I_Frame& _frame) override;
    virtual void removeChildFrame(I_Frame& _frame) override;
    virtual void getChildFrames(I_FrameVisitor& _visitor) override;
    void show(bool _visible) override;
    void handleEvent(pEvent_type _pEvent) override;
    /// @}

    /// @name Frame implementation
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
protected:
    Frame(const std::string& _name,
          wxWindow& _parent);
    virtual ~Frame();
    /// @}

    /// @name Member Variables
    /// @{
protected:
    /// @}

};  // class Frame

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Hyperion
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
