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
#ifndef INCLUDED_ACCESSIBILITY_INC_ACCESSIBILITY_EXTENDED_ACCESSIBLEGRIDCONTROLHEADERCELL_HXX
#define INCLUDED_ACCESSIBILITY_INC_ACCESSIBILITY_EXTENDED_ACCESSIBLEGRIDCONTROLHEADERCELL_HXX

#include "accessibility/extended/AccessibleGridControlTableCell.hxx"

namespace accessibility
{
    class AccessibleGridControlHeaderCell : public AccessibleGridControlCell, public css::accessibility::XAccessible
    {
        sal_Int32   m_nColumnRowId;
    public:
        AccessibleGridControlHeaderCell(sal_Int32 _nColumnRowId,
                        const css::uno::Reference< css::accessibility::XAccessible >& rxParent,
                        ::svt::table::IAccessibleTable& _rTable,
                        ::svt::table::AccessibleTableControlObjType  _eObjType);
        /** @return  The count of visible children. */
        virtual sal_Int32 SAL_CALL getAccessibleChildCount() throw ( css::uno::RuntimeException, std::exception ) SAL_OVERRIDE;

        /** @return  The XAccessible interface of the specified child. */
        virtual css::uno::Reference< css::accessibility::XAccessible > SAL_CALL
            getAccessibleChild( sal_Int32 nChildIndex ) throw ( css::lang::IndexOutOfBoundsException,css::uno::RuntimeException, std::exception ) SAL_OVERRIDE;

        /** @return  The index of this object among the parent's children. */
        virtual sal_Int32 SAL_CALL getAccessibleIndexInParent() throw ( css::uno::RuntimeException, std::exception ) SAL_OVERRIDE;

        /** Grabs the focus to the GridControl. */
        virtual void SAL_CALL grabFocus() throw ( css::uno::RuntimeException, std::exception ) SAL_OVERRIDE;

        // XInterface

        /** Queries for a new interface. */
        css::uno::Any SAL_CALL queryInterface( const css::uno::Type& rType )
            throw ( css::uno::RuntimeException, std::exception ) SAL_OVERRIDE;

        /** Aquires the object (calls acquire() on base class). */
        virtual void SAL_CALL acquire() throw () SAL_OVERRIDE;

        /** Releases the object (calls release() on base class). */
        virtual void SAL_CALL release() throw () SAL_OVERRIDE;
        // XAccessible

        /** @return  The XAccessibleContext interface of this object. */
        virtual css::uno::Reference< css::accessibility::XAccessibleContext > SAL_CALL
        getAccessibleContext()
            throw ( css::uno::RuntimeException, std::exception ) SAL_OVERRIDE;

        /** @return
                The name of this class.
        */
        virtual OUString SAL_CALL getImplementationName() throw ( css::uno::RuntimeException, std::exception ) SAL_OVERRIDE;

        /** Creates a new AccessibleStateSetHelper and fills it with states of the
            current object.
            @return
                A filled AccessibleStateSetHelper.
        */
        ::utl::AccessibleStateSetHelper* implCreateStateSetHelper() SAL_OVERRIDE;

    protected:
        virtual Rectangle implGetBoundingBox() SAL_OVERRIDE;

        virtual Rectangle implGetBoundingBoxOnScreen() SAL_OVERRIDE;
    };
}

#endif // INCLUDED_ACCESSIBILITY_INC_ACCESSIBILITY_EXTENDED_ACCESSIBLEGRIDCONTROLHEADERCELL_HXX

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
