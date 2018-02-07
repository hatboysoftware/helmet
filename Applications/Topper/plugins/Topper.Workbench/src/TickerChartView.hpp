//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Topper Trading Workbench
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//  @author Tony Richards - <trichards@indiezen.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include "View.hpp"
#include "NotebookPage.hpp"

#include <wx/chartpanel.h>
#include <wx/xy/xyplot.h>
#include <wx/axis/numberaxis.h>
#include <wx/axis/dateaxis.h>

#include <vector>
#include <utility>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class Workbench;

class TickerChartView
:   public View
,   public NotebookPage
{
    /// @name Type
    /// @{
public:
    /// @}

    /// @name I_Observer implementation
    /// @{
public:
    pConnection_type getConnection() const override;
    void setConnection(pConnection_type _pConnection) override;
    /// @}

    /// @name I_NotebookPage interface
    /// @{
public:
    void activate() override;
    void deactivate() override;
    /// @}

    /// @name I_View implementation
    /// @{
public:
    const Helmet::Workbench::I_Model* getModel() const override;
    /// @}

    /// @name View implementation
    /// @{
public:
    void modelUpdate(Helmet::Core::Event::I_Event& _event) override;
    /// @}

    /// @name TickerChartView implementation
    /// @{
public:
    /// @}

    /// @name Internal Structures
    /// @{
private:
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
    class TickerDataset
    :   public XYDataset
    ,   public DateTimeDataset
    {
        /// @name Types
        /// @{
    public:
        /// @}

        /// @name Dataset implementation
        /// @{
    public:
        size_t GetSerieCount() override;
        size_t GetCount(size_t serie) override;
        wxString GetSerieName(size_t serie) override;
        /// @}

        /// @name XYDataset implementation
        /// @{
    public:
        double GetX(size_t index, size_t serie) override;
        double GetY(size_t index, size_t serie) override;
        /// @}

        /// @name DateTimeDataset implementation
        /// @{
    public:
        time_t GetDate(size_t index) override;
        size_t GetCount() override;
        /// @}

        /// @name TickerDataset implementation
        /// @{
    public:
        /// @}

        /// @name 'Structors
        /// @{
    public:
                 TickerDataset();
        virtual ~TickerDataset();
        //// @}

        /// @name Member Variables
        /// @{
    private:
        std::vector<std::pair<time_t,double>>   m_tickerData;
        /// @}

    };  // class TickerDataset
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
    /// @}

    /// @name 'Structors
    /// @{
public:
    TickerChartView(Workbench& _workbench,
                    Notebook& _parent,
                    const std::string& _name);
    virtual ~TickerChartView();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Workbench&          m_workbench;
    wxSizer*            m_pSizer;
    TickerDataset*      m_pDataset;
    XYPlot*             m_pPlot;
    NumberAxis*         m_pLeftAxis;
    DateAxis*           m_pBottomAxis;
    Chart*              m_pChart;
    wxChartPanel*       m_pChartPanel;
    pConnection_type    m_pConnection;
    /// @}

};  // class TickerChartView

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
