/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file incorporates work covered by the following license notice:
 *
 *   Licensed to the Apache Software Foundation (ASF) under one or more
 *   contributor license agreements. See the NOTICE file distributed
 *   with this work for additional information regarding copyright
 *   ownership. The ASF licenses this file to you under the Apache
 *   License, Version 2.0 (the "License"); you may not use this file
 *   except in compliance with the License. You may obtain a copy of
 *   the License at http://www.apache.org/licenses/LICENSE-2.0 .
 */

#include <sfx2/sidebar/ResourceDefinitions.hrc>
#include <sfx2/sidebar/Theme.hxx>
#include <sfx2/sidebar/ControlFactory.hxx>
#include <com/sun/star/chart2/LegendPosition.hpp>
#include <com/sun/star/chart/ChartLegendExpansion.hpp>

#include "ChartElementsPanel.hxx"
#include "ChartController.hxx"
#include <sfx2/bindings.hxx>
#include <sfx2/dispatch.hxx>
#include <sfx2/imagemgr.hxx>
#include <vcl/fixed.hxx>
#include <vcl/lstbox.hxx>
#include <vcl/field.hxx>
#include <vcl/toolbox.hxx>
#include <svl/intitem.hxx>
#include <svl/stritem.hxx>
#include <comphelper/processfactory.hxx>

#include "LegendHelper.hxx"
#include "TitleHelper.hxx"
#include "ChartModelHelper.hxx"
#include "AxisHelper.hxx"
#include "DiagramHelper.hxx"

#include "ChartModel.hxx"


using namespace css;
using namespace css::uno;
using ::sfx2::sidebar::Theme;

namespace chart { namespace sidebar {

namespace {

enum class GridType
{
    VERT_MAJOR,
    VERT_MINOR,
    HOR_MAJOR,
    HOR_MINOR
};

enum class AxisType
{
    X_MAIN,
    Y_MAIN,
    Z_MAIN,
    X_SECOND,
    Y_SECOND
};

ChartModel* getChartModel(css::uno::Reference<css::frame::XModel> xModel)
{
    ChartModel* pModel = dynamic_cast<ChartModel*>(xModel.get());

    return pModel;
}

bool isLegendVisible(css::uno::Reference<css::frame::XModel> xModel)
{
    ChartModel* pModel = getChartModel(xModel);
    if (!pModel)
        return false;

    Reference< beans::XPropertySet > xLegendProp( LegendHelper::getLegend(*pModel), uno::UNO_QUERY );
    if( xLegendProp.is())
    {
        try
        {
            bool bShow = false;
            if( xLegendProp->getPropertyValue( "Show") >>= bShow )
            {
                return bShow;
            }
        }
        catch(const uno::Exception &)
        {
        }
    }

    return false;
}

void setLegendVisible(css::uno::Reference<css::frame::XModel> xModel, bool bVisible)
{
    ChartModel* pModel = getChartModel(xModel);
    if (!pModel)
        return;

    if (bVisible)
        LegendHelper::showLegend(*pModel, comphelper::getProcessComponentContext());
    else
        LegendHelper::hideLegend(*pModel);
}

bool isTitleVisisble(css::uno::Reference<css::frame::XModel> xModel, TitleHelper::eTitleType eTitle)
{
    return TitleHelper::getTitle(eTitle, xModel).is();
}

void setTitleVisible(css::uno::Reference<css::frame::XModel> xModel, TitleHelper::eTitleType eTitle, bool bVisible)
{
    if (bVisible)
    {
        TitleHelper::createTitle(eTitle, "Title", xModel, comphelper::getProcessComponentContext());
    }
    else
    {
        TitleHelper::removeTitle(eTitle, xModel);
    }
}

bool isGridVisible(css::uno::Reference<css::frame::XModel> xModel, GridType eType)
{
    Reference< chart2::XDiagram > xDiagram(ChartModelHelper::findDiagram(xModel));
    if(xDiagram.is())
    {
        sal_Int32 nDimensionIndex = 0;
        if (eType == GridType::HOR_MAJOR || eType == GridType::HOR_MINOR)
            nDimensionIndex = 1;
        sal_Int32 nCooSysIndex = 0;

        bool bMajor = (eType == GridType::HOR_MAJOR || eType == GridType::VERT_MAJOR);

        bool bHasGrid = AxisHelper::isGridShown(nDimensionIndex, nCooSysIndex, bMajor, xDiagram);
        return bHasGrid;
    }
    return false;
}

void setGridVisible(css::uno::Reference<css::frame::XModel> xModel, GridType eType, bool bVisible)
{
    Reference< chart2::XDiagram > xDiagram(ChartModelHelper::findDiagram(xModel));
    if(xDiagram.is())
    {
        sal_Int32 nDimensionIndex = 0;
        if (eType == GridType::HOR_MAJOR || eType == GridType::HOR_MINOR)
            nDimensionIndex = 1;
        sal_Int32 nCooSysIndex = 0;

        bool bMajor = (eType == GridType::HOR_MAJOR || eType == GridType::VERT_MAJOR);

        if (bVisible)
            AxisHelper::showGrid(nDimensionIndex, nCooSysIndex, bMajor,
                    xDiagram, comphelper::getProcessComponentContext());
        else
            AxisHelper::hideGrid(nDimensionIndex, nCooSysIndex, bMajor, xDiagram);
    }
}

bool isAxisVisible(css::uno::Reference<css::frame::XModel> xModel, AxisType eType)
{
    Reference< chart2::XDiagram > xDiagram(ChartModelHelper::findDiagram(xModel));
    if(xDiagram.is())
    {
        sal_Int32 nDimensionIndex = 0;
        if (eType == AxisType::Y_MAIN || eType == AxisType::Y_SECOND)
            nDimensionIndex = 1;
        else if (eType == AxisType::Z_MAIN)
            nDimensionIndex = 2;

        bool bMajor = !(eType == AxisType::X_SECOND || eType == AxisType::Y_SECOND);

        bool bHasAxis = AxisHelper::isAxisShown(nDimensionIndex, bMajor, xDiagram);
        return bHasAxis;
    }
    return false;
}

void setAxisVisible(css::uno::Reference<css::frame::XModel> xModel, AxisType eType, bool bVisible)
{
    Reference< chart2::XDiagram > xDiagram(ChartModelHelper::findDiagram(xModel));
    if(xDiagram.is())
    {
        sal_Int32 nDimensionIndex = 0;
        if (eType == AxisType::Y_MAIN || eType == AxisType::Y_SECOND)
            nDimensionIndex = 1;
        else if (eType == AxisType::Z_MAIN)
            nDimensionIndex = 2;

        bool bMajor = !(eType == AxisType::X_SECOND || eType == AxisType::Y_SECOND);

        if (bVisible)
            AxisHelper::showAxis(nDimensionIndex, bMajor, xDiagram, comphelper::getProcessComponentContext());
        else
            AxisHelper::hideAxis(nDimensionIndex, bMajor, xDiagram);
    }
}

sal_Int32 getLegendPos(css::uno::Reference<css::frame::XModel> xModel)
{
    ChartModel* pModel = getChartModel(xModel);
    if (!pModel)
        return 4;

    Reference< beans::XPropertySet > xLegendProp( LegendHelper::getLegend(*pModel), uno::UNO_QUERY );
    if (!xLegendProp.is())
        return 4;

    chart2::LegendPosition eLegendPos = chart2::LegendPosition_CUSTOM;
    xLegendProp->getPropertyValue("AnchorPosition") >>= eLegendPos;
    switch(eLegendPos)
    {
        case chart2::LegendPosition_LINE_START:
            return 1;
        case chart2::LegendPosition_LINE_END:
            return 2;
        case chart2::LegendPosition_PAGE_START:
            return 3;
        case chart2::LegendPosition_PAGE_END:
            return 0;
        default:
            return 4;
    }
}

void setLegendPos(css::uno::Reference<css::frame::XModel> xModel, sal_Int32 nPos)
{
    ChartModel* pModel = getChartModel(xModel);
    if (!pModel)
        return;

    Reference< beans::XPropertySet > xLegendProp( LegendHelper::getLegend(*pModel), uno::UNO_QUERY );
    if (!xLegendProp.is())
        return;

    chart2::LegendPosition eLegendPos = chart2::LegendPosition_CUSTOM;
    css::chart::ChartLegendExpansion eExpansion = css::chart::ChartLegendExpansion_HIGH;
    switch(nPos)
    {
        case 3:
            eLegendPos = chart2::LegendPosition_PAGE_START;
            eExpansion = css::chart::ChartLegendExpansion_WIDE;
            break;
        case 1:
            eLegendPos = chart2::LegendPosition_LINE_START;
            break;
        case 2:
            eLegendPos = chart2::LegendPosition_LINE_END;
            break;
        case 0:
            eLegendPos = chart2::LegendPosition_PAGE_END;
            eExpansion = css::chart::ChartLegendExpansion_WIDE;
            break;
        case 4:
            eLegendPos = chart2::LegendPosition_CUSTOM;
            break;
        default:
            assert(false);
    }

    xLegendProp->setPropertyValue("AnchorPosition", css::uno::makeAny(eLegendPos));
    xLegendProp->setPropertyValue("Expansion", css::uno::makeAny(eExpansion));

    if (eLegendPos != chart2::LegendPosition_CUSTOM)
    {
        xLegendProp->setPropertyValue("RelativePosition", uno::Any());
    }
}

}

ChartElementsPanel::ChartElementsPanel(
    vcl::Window* pParent,
    const css::uno::Reference<css::frame::XFrame>& rxFrame,
    ChartController* pController)
  : PanelLayout(pParent, "ChartElementsPanel", "modules/schart/ui/sidebarelements.ui", rxFrame),
    mxFrame(rxFrame),
    maContext(),
    mxModel(pController->getModel()),
    mxListener(new ChartSidebarModifyListener(this))
{
    get(mpCBTitle,  "checkbutton_title");
    get(mpCBSubtitle,  "checkbutton_subtitle");
    get(mpCBXAxis,  "checkbutton_x_axis");
    get(mpCBXAxisTitle,  "checkbutton_x_axis_title");
    get(mpCBYAxis,  "checkbutton_y_axis");
    get(mpCBYAxisTitle,  "checkbutton_y_axis_title");
    get(mpCBZAxis,  "checkbutton_z_axis");
    get(mpCBZAxisTitle,  "checkbutton_z_axis_title");
    get(mpCB2ndXAxis,  "checkbutton_2nd_x_axis");
    get(mpCB2ndXAxisTitle,  "checkbutton_2nd_x_axis_title");
    get(mpCB2ndYAxis,  "checkbutton_2nd_y_axis");
    get(mpCB2ndYAxisTitle,  "checkbutton_2nd_y_axis_title");
    get(mpCBLegend,  "checkbutton_legend");
    get(mpCBGridVerticalMajor,  "checkbutton_gridline_vertical_major");
    get(mpCBGridHorizontalMajor,  "checkbutton_gridline_horizontal_major");
    get(mpCBGridVerticalMinor,  "checkbutton_gridline_vertical_minor");
    get(mpCBGridHorizontalMinor,  "checkbutton_gridline_horizontal_minor");

    get(mpLBLegendPosition, "comboboxtext_legend");

    Initialize();
}

ChartElementsPanel::~ChartElementsPanel()
{
    disposeOnce();
}

void ChartElementsPanel::dispose()
{

    css::uno::Reference<css::util::XModifyBroadcaster> xBroadcaster(mxModel, css::uno::UNO_QUERY_THROW);
    xBroadcaster->removeModifyListener(mxListener);
    mpCBTitle.clear();
    mpCBSubtitle.clear();
    mpCBXAxis.clear();
    mpCBXAxisTitle.clear();
    mpCBYAxis.clear();
    mpCBYAxisTitle.clear();
    mpCBZAxis.clear();
    mpCBZAxisTitle.clear();
    mpCB2ndXAxis.clear();
    mpCB2ndXAxisTitle.clear();
    mpCB2ndYAxis.clear();
    mpCB2ndYAxisTitle.clear();
    mpCBLegend.clear();
    mpCBGridVerticalMajor.clear();
    mpCBGridHorizontalMajor.clear();
    mpCBGridVerticalMinor.clear();
    mpCBGridHorizontalMinor.clear();

    PanelLayout::dispose();
}

void ChartElementsPanel::Initialize()
{
    css::uno::Reference<css::util::XModifyBroadcaster> xBroadcaster(mxModel, css::uno::UNO_QUERY_THROW);
    xBroadcaster->addModifyListener(mxListener);
    updateData();

    Link<> aLink = LINK(this, ChartElementsPanel, CheckBoxHdl);
    mpCBTitle->SetClickHdl(aLink);
    mpCBSubtitle->SetClickHdl(aLink);
    mpCBXAxis->SetClickHdl(aLink);
    mpCBXAxisTitle->SetClickHdl(aLink);
    mpCBYAxis->SetClickHdl(aLink);
    mpCBYAxisTitle->SetClickHdl(aLink);
    mpCBZAxis->SetClickHdl(aLink);
    mpCBZAxisTitle->SetClickHdl(aLink);
    mpCB2ndXAxis->SetClickHdl(aLink);
    mpCB2ndXAxisTitle->SetClickHdl(aLink);
    mpCB2ndYAxis->SetClickHdl(aLink);
    mpCB2ndYAxisTitle->SetClickHdl(aLink);
    mpCBLegend->SetClickHdl(aLink);
    mpCBGridVerticalMajor->SetClickHdl(aLink);
    mpCBGridHorizontalMajor->SetClickHdl(aLink);
    mpCBGridVerticalMinor->SetClickHdl(aLink);
    mpCBGridHorizontalMinor->SetClickHdl(aLink);

    mpLBLegendPosition->SetSelectHdl(LINK(this, ChartElementsPanel, LegendPosHdl));
}

void ChartElementsPanel::updateData()
{
    Reference< chart2::XDiagram > xDiagram(ChartModelHelper::findDiagram(mxModel));
    sal_Int32 nDimension = DiagramHelper::getDimension(xDiagram);
    SolarMutexGuard aGuard;

    mpCBLegend->Check(isLegendVisible(mxModel));
    mpCBTitle->Check(isTitleVisisble(mxModel, TitleHelper::MAIN_TITLE));
    mpCBSubtitle->Check(isTitleVisisble(mxModel, TitleHelper::SUB_TITLE));
    mpCBXAxisTitle->Check(isTitleVisisble(mxModel, TitleHelper::X_AXIS_TITLE));
    mpCBYAxisTitle->Check(isTitleVisisble(mxModel, TitleHelper::Y_AXIS_TITLE));
    mpCBZAxisTitle->Check(isTitleVisisble(mxModel, TitleHelper::Z_AXIS_TITLE));
    mpCB2ndXAxisTitle->Check(isTitleVisisble(mxModel, TitleHelper::SECONDARY_X_AXIS_TITLE));
    mpCB2ndYAxisTitle->Check(isTitleVisisble(mxModel, TitleHelper::SECONDARY_Y_AXIS_TITLE));
    mpCBGridVerticalMajor->Check(isGridVisible(mxModel, GridType::VERT_MAJOR));
    mpCBGridHorizontalMajor->Check(isGridVisible(mxModel, GridType::HOR_MAJOR));
    mpCBGridVerticalMinor->Check(isGridVisible(mxModel, GridType::VERT_MINOR));
    mpCBGridHorizontalMinor->Check(isGridVisible(mxModel, GridType::HOR_MINOR));
    mpCBXAxis->Check(isAxisVisible(mxModel, AxisType::X_MAIN));
    mpCBYAxis->Check(isAxisVisible(mxModel, AxisType::Y_MAIN));
    mpCBZAxis->Check(isAxisVisible(mxModel, AxisType::Z_MAIN));
    mpCB2ndXAxis->Check(isAxisVisible(mxModel, AxisType::X_SECOND));
    mpCB2ndYAxis->Check(isAxisVisible(mxModel, AxisType::Y_SECOND));

    if (nDimension != 3)
    {
        mpCBZAxis->Disable();
        mpCBZAxisTitle->Disable();
    }

    mpLBLegendPosition->SelectEntryPos(getLegendPos(mxModel));
}

VclPtr<vcl::Window> ChartElementsPanel::Create (
    vcl::Window* pParent,
    const css::uno::Reference<css::frame::XFrame>& rxFrame,
    ChartController* pController)
{
    if (pParent == NULL)
        throw lang::IllegalArgumentException("no parent Window given to ChartElementsPanel::Create", NULL, 0);
    if ( ! rxFrame.is())
        throw lang::IllegalArgumentException("no XFrame given to ChartElementsPanel::Create", NULL, 1);
    return  VclPtr<ChartElementsPanel>::Create(
                        pParent, rxFrame, pController);
}

void ChartElementsPanel::DataChanged(
    const DataChangedEvent& )
{
    updateData();
}

void ChartElementsPanel::HandleContextChange(
    const ::sfx2::sidebar::EnumContext& rContext)
{
    if(maContext == rContext)
    {
        // Nothing to do.
        return;
    }

    maContext = rContext;
    updateData();
}

void ChartElementsPanel::NotifyItemUpdate(
    sal_uInt16 /*nSID*/,
    SfxItemState /*eState*/,
    const SfxPoolItem* /*pState*/,
    const bool )
{
}

void ChartElementsPanel::modelInvalid()
{

}

IMPL_LINK(ChartElementsPanel, CheckBoxHdl, CheckBox*, pCheckBox)
{
    bool bChecked = pCheckBox->IsChecked();
    if (pCheckBox == mpCBTitle.get())
        setTitleVisible(mxModel, TitleHelper::MAIN_TITLE, bChecked);
    else if (pCheckBox == mpCBSubtitle.get())
        setTitleVisible(mxModel, TitleHelper::SUB_TITLE, bChecked);
    else if (pCheckBox == mpCBXAxis.get())
        setAxisVisible(mxModel, AxisType::X_MAIN, bChecked);
    else if (pCheckBox == mpCBXAxisTitle.get())
        setTitleVisible(mxModel, TitleHelper::X_AXIS_TITLE, bChecked);
    else if (pCheckBox == mpCBYAxis.get())
        setAxisVisible(mxModel, AxisType::Y_MAIN, bChecked);
    else if (pCheckBox == mpCBYAxisTitle.get())
        setTitleVisible(mxModel, TitleHelper::Y_AXIS_TITLE, bChecked);
    else if (pCheckBox == mpCBZAxis.get())
        setAxisVisible(mxModel, AxisType::Z_MAIN, bChecked);
    else if (pCheckBox == mpCBZAxisTitle.get())
        setTitleVisible(mxModel, TitleHelper::Z_AXIS_TITLE, bChecked);
    else if (pCheckBox == mpCB2ndXAxis.get())
        setAxisVisible(mxModel, AxisType::X_SECOND, bChecked);
    else if (pCheckBox == mpCB2ndXAxisTitle.get())
        setTitleVisible(mxModel, TitleHelper::SECONDARY_X_AXIS_TITLE, bChecked);
    else if (pCheckBox == mpCB2ndYAxis.get())
        setAxisVisible(mxModel, AxisType::Y_SECOND, bChecked);
    else if (pCheckBox == mpCB2ndYAxisTitle.get())
        setTitleVisible(mxModel, TitleHelper::SECONDARY_Y_AXIS_TITLE, bChecked);
    else if (pCheckBox == mpCBLegend.get())
        setLegendVisible(mxModel, bChecked);
    else if (pCheckBox == mpCBGridVerticalMajor.get())
        setGridVisible(mxModel, GridType::VERT_MAJOR, bChecked);
    else if (pCheckBox == mpCBGridHorizontalMajor.get())
        setGridVisible(mxModel, GridType::HOR_MAJOR, bChecked);
    else if (pCheckBox == mpCBGridVerticalMinor.get())
        setGridVisible(mxModel, GridType::VERT_MINOR, bChecked);
    else if (pCheckBox == mpCBGridHorizontalMinor.get())
        setGridVisible(mxModel, GridType::HOR_MINOR, bChecked);

    return 0;
}

IMPL_LINK_NOARG(ChartElementsPanel, LegendPosHdl)
{
    sal_Int32 nPos = mpLBLegendPosition->GetSelectEntryPos();
    setLegendPos(mxModel, nPos);
    return 0;
}

}} // end of namespace ::chart::sidebar

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
