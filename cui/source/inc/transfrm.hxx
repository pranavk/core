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
#ifndef INCLUDED_CUI_SOURCE_INC_TRANSFRM_HXX
#define INCLUDED_CUI_SOURCE_INC_TRANSFRM_HXX

#include <svx/dlgctrl.hxx>
#include <svx/dialcontrol.hxx>

#include <vcl/fixed.hxx>

#include <basegfx/range/b2drange.hxx>

// predefines
class SdrView;

/*************************************************************************
|*
|* Transform-Tab-Dialog
|*
\************************************************************************/

/** put this into the nAnchorTypes parameter of the SvxTransformTabDialog c'tor
    to disable the size controls */
const sal_uInt16 SVX_OBJ_NORESIZE = 0x0100;

/** put this into the nAnchorTypes parameter of the SvxTransformTabDialog c'tor
    to disable the protect controls */
const sal_uInt16 SVX_OBJ_NOPROTECT = 0x0200;

class SvxTransformTabDialog : public SfxTabDialog
{
    sal_uInt16         nPosSize;
    sal_uInt16         nSWPosSize;
    sal_uInt16         nRotation;
    sal_uInt16         nSlant;
private:
    const SdrView*      pView;

    sal_uInt16          nAnchorCtrls;
    Link<>              aValidateLink;

    virtual void        PageCreated( sal_uInt16 nId, SfxTabPage &rPage ) SAL_OVERRIDE;

public:

            SvxTransformTabDialog( vcl::Window* pParent, const SfxItemSet* pAttr,
                            const SdrView* pView,
                            sal_uInt16 nAnchorTypes = 0);

            //link for the Writer to validate positions
            void SetValidateFramePosLink( const Link<>& rLink );
};

/*************************************************************************
|*
|* position and size tab page
|*
\************************************************************************/

class SvxPositionSizeTabPage : public SvxTabPage
{
    using TabPage::ActivatePage;
    using TabPage::DeactivatePage;
    static const sal_uInt16 pPosSizeRanges[];

private:
    // position
    VclPtr<VclFrame>            m_pFlPosition;
    VclPtr<MetricField>         m_pMtrPosX;
    VclPtr<MetricField>         m_pMtrPosY;
    VclPtr<SvxRectCtl>          m_pCtlPos;

    // size
    VclPtr<VclFrame>            m_pFlSize;
    VclPtr<FixedText>           m_pFtWidth;
    VclPtr<MetricField>         m_pMtrWidth;
    VclPtr<FixedText>           m_pFtHeight;
    VclPtr<MetricField>         m_pMtrHeight;
    VclPtr<CheckBox>            m_pCbxScale;
    VclPtr<SvxRectCtl>          m_pCtlSize;

    // protect
    VclPtr<VclFrame>            m_pFlProtect;
    VclPtr<TriStateBox>         m_pTsbPosProtect;
    VclPtr<TriStateBox>         m_pTsbSizeProtect;

    // adjust
    VclPtr<VclFrame>            m_pFlAdjust;
    VclPtr<TriStateBox>         m_pTsbAutoGrowWidth;
    VclPtr<TriStateBox>         m_pTsbAutoGrowHeight;

private:
    const SfxItemSet&   mrOutAttrs;

    const SdrView*      mpView;

    // #i75273#
    basegfx::B2DRange   maRange;
    basegfx::B2DRange   maWorkRange;
    basegfx::B2DPoint   maAnchor;

    SfxMapUnit          mePoolUnit;
    FieldUnit           meDlgUnit;
    TriState            mnProtectSizeState;
    bool                mbPageDisabled;
    bool                mbProtectDisabled;
    bool                mbSizeDisabled;
    bool                mbAdjustDisabled;

    // from size
    // #i75273#
    double              mfOldWidth;
    double              mfOldHeight;
    RECT_POINT          meRP;


    DECL_LINK( ChangePosProtectHdl, void * );
    DECL_LINK( ChangeSizeProtectHdl, void * );

    void SetMinMaxPosition();
    void GetTopLeftPosition(double& rfX, double& rfY, const basegfx::B2DRange& rRange);

    DECL_LINK( ChangeWidthHdl, void * );
    DECL_LINK( ChangeHeightHdl, void * );
    DECL_LINK( ClickSizeProtectHdl, void * );
    DECL_LINK( ClickAutoHdl, void * );

public:
    SvxPositionSizeTabPage( vcl::Window* pParent, const SfxItemSet& rInAttrs  );
    virtual ~SvxPositionSizeTabPage();
    virtual void dispose() SAL_OVERRIDE;

    static VclPtr<SfxTabPage> Create( vcl::Window*, const SfxItemSet* );
    static const sal_uInt16* GetRanges() {  return pPosSizeRanges; }

    virtual bool FillItemSet( SfxItemSet* ) SAL_OVERRIDE;
    virtual void Reset( const SfxItemSet * ) SAL_OVERRIDE;

    virtual void ActivatePage( const SfxItemSet& rSet ) SAL_OVERRIDE;
    virtual sfxpg DeactivatePage( SfxItemSet* pSet ) SAL_OVERRIDE;

    virtual void PointChanged( vcl::Window* pWindow, RECT_POINT eRP ) SAL_OVERRIDE;

    void         Construct();
    void         SetView( const SdrView* pSdrView ) { mpView = pSdrView; }

    virtual void FillUserData() SAL_OVERRIDE;

    void        DisableResize();
    void        DisableProtect();

    void        UpdateControlStates();
};

/*************************************************************************
|*
|* rotation angle tab page
|*
\************************************************************************/
class SvxAngleTabPage : public SvxTabPage
{
    using TabPage::ActivatePage;
    using TabPage::DeactivatePage;
    static const sal_uInt16 pAngleRanges[];

private:
    VclPtr<VclFrame>            m_pFlPosition;
    VclPtr<MetricField>         m_pMtrPosX;
    VclPtr<MetricField>         m_pMtrPosY;
    VclPtr<SvxRectCtl>          m_pCtlRect;

    VclPtr<VclFrame>            m_pFlAngle;
    VclPtr<NumericField>        m_pNfAngle;
    VclPtr<svx::DialControl>    m_pCtlAngle;

    const SfxItemSet&   rOutAttrs;
    const SdrView*      pView;

    // #i75273#
    basegfx::B2DRange   maRange;
    basegfx::B2DPoint   maAnchor;

    SfxMapUnit          ePoolUnit;
    FieldUnit           eDlgUnit;

public:
         SvxAngleTabPage( vcl::Window* pParent, const SfxItemSet& rInAttrs  );
    virtual ~SvxAngleTabPage();
    virtual void dispose() SAL_OVERRIDE;

    static VclPtr<SfxTabPage> Create( vcl::Window*, const SfxItemSet* );
    static const sal_uInt16*  GetRanges() { return pAngleRanges; }

    virtual bool FillItemSet( SfxItemSet* ) SAL_OVERRIDE;
    virtual void Reset( const SfxItemSet * ) SAL_OVERRIDE;

    virtual void ActivatePage( const SfxItemSet& rSet ) SAL_OVERRIDE;
    virtual sfxpg DeactivatePage( SfxItemSet* pSet ) SAL_OVERRIDE;

    virtual void PointChanged( vcl::Window* pWindow, RECT_POINT eRP ) SAL_OVERRIDE;

    void         Construct();
    void         SetView( const SdrView* pSdrView ) { pView = pSdrView; }
};

/*************************************************************************
|*
|* slant/corner radius tab page
|*
\************************************************************************/
class SvxSlantTabPage : public SvxTabPage
{
    using TabPage::ActivatePage;
    using TabPage::DeactivatePage;
    static const sal_uInt16 pSlantRanges[];

private:
    VclPtr<VclFrame>            m_pFlRadius;
    VclPtr<MetricField>         m_pMtrRadius;
    VclPtr<VclFrame>            m_pFlAngle;
    VclPtr<MetricField>         m_pMtrAngle;

    const SfxItemSet&   rOutAttrs;

    const SdrView*      pView;

    // #i75273#
    basegfx::B2DRange   maRange;

    SfxMapUnit          ePoolUnit;
    FieldUnit           eDlgUnit;

public:
         SvxSlantTabPage( vcl::Window* pParent, const SfxItemSet& rInAttrs  );
    virtual ~SvxSlantTabPage();
    virtual void dispose() SAL_OVERRIDE;

    static VclPtr<SfxTabPage> Create( vcl::Window*, const SfxItemSet* );
    static const sal_uInt16* GetRanges() {  return pSlantRanges; }

    virtual bool FillItemSet( SfxItemSet* ) SAL_OVERRIDE;
    virtual void Reset( const SfxItemSet * ) SAL_OVERRIDE;

    virtual void ActivatePage( const SfxItemSet& rSet ) SAL_OVERRIDE;
    virtual sfxpg DeactivatePage( SfxItemSet* pSet ) SAL_OVERRIDE;

    virtual void PointChanged( vcl::Window* pWindow, RECT_POINT eRP ) SAL_OVERRIDE;

    void         Construct();
    void         SetView( const SdrView* pSdrView ) { pView = pSdrView; }
};



#endif // INCLUDED_CUI_SOURCE_INC_TRANSFRM_HXX

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
