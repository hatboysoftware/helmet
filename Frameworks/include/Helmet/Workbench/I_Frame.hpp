//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Workbench/Configuration.hpp>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class wxWindow;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Workbench {
class I_Event;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class HELMET_WORKBENCH_DLL_LINK I_Frame
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef boost::shared_ptr<I_Event>  pEvent_type;
    /// @}

    /// @name Internal structures
    /// @{
public:
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    struct HELMET_WORKBENCH_DLL_LINK I_FrameVisitor
    {
        /// @name I_PageVisitor interface
        /// @{
    public:
        virtual void begin() = 0;
        virtual void visit(I_Frame& _frame) = 0;
        virtual void end() = 0;
        /// @}

        /// @name 'Structors
        /// @{
    protected:
                 I_FrameVisitor();
        virtual ~I_FrameVisitor();
        /// @}

    };  // interface I_FrameVisitor
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    /// @}

    /// @name I_Frame interface
    /// @{
public:
    virtual void setName(const std::string& _name) = 0;
    virtual const std::string getName() const = 0;
    virtual void addChildFrame(I_Frame& _frame) = 0;
    virtual void removeChildFrame(I_Frame& _frame) = 0;
    virtual void getChildFrames(I_FrameVisitor& _visitor) = 0;
    virtual void show(bool _visible) = 0;
    virtual void handleEvent(pEvent_type _pEvent) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Frame();
    virtual ~I_Frame();
    /// @}

};  // interface I_Frame

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
