//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Helmet Software Framework
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Helmet/Core/Utility/I_Visitor.hpp>

#include <Helmet/Workbench/I_Frame.hpp>

#include <boost/smart_ptr/shared_ptr.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Helmet {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_NotebookPage;

class HELMET_WORKBENCH_DLL_LINK I_Notebook
:   public I_Frame
{
    /// @name Types
    /// @{
public:
    typedef boost::shared_ptr<I_NotebookPage>   pNotebookPage_type;
    /// @}

    /// @name I_Notebook interface
    /// @{
public:
    virtual Helmet::Workbench::I_NotebookPage& addPage(I_NotebookPage& _page) = 0;
    virtual void showPage(int _index) = 0;
    virtual void showPage(I_NotebookPage& _page) = 0;
    virtual void removePage(int _index) = 0;
    virtual void removePage(I_NotebookPage& _page) = 0;
    virtual void getPages(Core::Utility::I_Visitor<I_NotebookPage>& _visitor) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Notebook();
    virtual ~I_Notebook();
    /// @}

};  // interface I_Notebook

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Helmet
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
