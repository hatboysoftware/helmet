//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Topper Trading Workbench
//
// Copyright (C) 2018 Hat Boy Software, Inc.
//
//  @author Matthew Alan Gray - <mgray@hatboysoftware.com>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "TickerChartView.hpp"
#include "TickerDataModel.hpp"
#include "Workbench.hpp"
#include "Notebook.hpp"

#include <Helmet/Core/Event/I_Connection.hpp>

#include <wx/xy/xylinerenderer.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Topper {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TickerChartView::TickerChartView(Workbench& _workbench,
                                 Notebook& _parent,
                                 const std::string& _name)
:   View(_name)
,   NotebookPage(_name, _parent)
,   m_workbench(_workbench)
,   m_pSizer(new wxBoxSizer(wxHORIZONTAL))
,   m_pDataset(new TickerDataset())
,   m_pPlot(new XYPlot())
,   m_pLeftAxis(new NumberAxis(AXIS_LEFT))
,   m_pBottomAxis(new DateAxis(AXIS_BOTTOM))
,   m_pChart(new Chart(m_pPlot, _name))
,   m_pChartPanel(new wxChartPanel(&_parent,
                                   wxID_ANY,
                                   nullptr,
                                   wxDefaultPosition,
                                   wxDefaultSize))
,   m_pConnection(nullptr)
{
    m_pDataset->SetRenderer(new XYLineRenderer());
    m_pPlot->AddDataset(m_pDataset);
    m_pBottomAxis->SetVerticalLabelText(true);
    m_pBottomAxis->SetDateFormat(wxT("%d-%m"));
    m_pPlot->AddAxis(m_pLeftAxis);
    m_pPlot->AddAxis(m_pBottomAxis);
    m_pPlot->LinkDataVerticalAxis(0, 0);
    m_pPlot->LinkDataHorizontalAxis(0, 0);
    m_pChartPanel->SetChart(m_pChart);

    m_pSizer->Add(m_pChartPanel, 1, wxEXPAND);
    _parent.SetSizer(m_pSizer);
    _parent.Layout();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TickerChartView::~TickerChartView() = default;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Helmet::Core::Event::I_Observer::pConnection_type
TickerChartView::getConnection() const
{
    return View::getConnection();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TickerChartView::setConnection(pConnection_type _pConnection)
{
    auto& source = _pConnection->getSource();
    auto pModel = dynamic_cast<const TickerDataModel*>(&source);
    if (pModel != nullptr)
    {
        View::setConnection(_pConnection);
        return;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TickerChartView::activate()
{
    NotebookPage::activate();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TickerChartView::deactivate()
{
    NotebookPage::deactivate();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Helmet::Workbench::I_Model*
TickerChartView::getModel() const
{
    return View::getModel();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TickerChartView::modelUpdate(Helmet::Core::Event::I_Event& _event)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TickerChartView::TickerDataset::TickerDataset()
:   m_tickerData()
{
    m_tickerData.push_back(std::make_pair(wxDateTime(17, wxDateTime::Month::Mar, 2006).GetTicks(), 64.09));
    m_tickerData.push_back(std::make_pair(wxDateTime(20, wxDateTime::Month::Mar, 2006).GetTicks(), 63.34));
    m_tickerData.push_back(std::make_pair(wxDateTime(21, wxDateTime::Month::Mar, 2006).GetTicks(), 61.41));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TickerChartView::TickerDataset::~TickerDataset() = default;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
size_t
TickerChartView::TickerDataset::GetSerieCount()
{
    return 1;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
size_t
TickerChartView::TickerDataset::GetCount(size_t serie)
{
    return m_tickerData.size();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
wxString
TickerChartView::TickerDataset::GetSerieName(size_t serie)
{
    return wxT("BTC/USD");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
double
TickerChartView::TickerDataset::GetX(size_t index, size_t serie)
{
    return index;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
double
TickerChartView::TickerDataset::GetY(size_t index, size_t serie)
{
    return m_tickerData[index].second;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
time_t
TickerChartView::TickerDataset::GetDate(size_t index)
{
    return m_tickerData[index].first;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
size_t
TickerChartView::TickerDataset::GetCount()
{
    return m_tickerData.size();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Topper
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
