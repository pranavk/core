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

#ifndef INCLUDED_DBACCESS_SOURCE_CORE_INC_VETO_HXX
#define INCLUDED_DBACCESS_SOURCE_CORE_INC_VETO_HXX

#include <com/sun/star/util/XVeto.hpp>

#include <cppuhelper/implbase.hxx>

namespace dbaccess
{

    // Veto
    typedef ::cppu::WeakImplHelper<   ::com::sun::star::util::XVeto
                                  >   Veto_Base;
    /** implements ::com::sun::star::util::XVeto
    */
    class Veto : public Veto_Base
    {
    private:
        const OUString               m_sReason;
        const ::com::sun::star::uno::Any    m_aDetails;

    public:
        Veto( const OUString& _rReason, const ::com::sun::star::uno::Any& _rDetails );

        virtual OUString SAL_CALL getReason() throw (::com::sun::star::uno::RuntimeException, std::exception) SAL_OVERRIDE;
        virtual ::com::sun::star::uno::Any SAL_CALL getDetails() throw (::com::sun::star::uno::RuntimeException, std::exception) SAL_OVERRIDE;

    protected:
        virtual ~Veto();

    private:
        Veto( const Veto& ) SAL_DELETED_FUNCTION;
        Veto& operator=( const Veto& ) SAL_DELETED_FUNCTION;
    };

} // namespace dbaccess

#endif // INCLUDED_DBACCESS_SOURCE_CORE_INC_VETO_HXX

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
