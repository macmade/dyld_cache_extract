/*******************************************************************************
 * The MIT License (MIT)
 * 
 * Copyright (c) 2016 Jean-David Gadina - www-xs-labs.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

/*!
 * @file        TwoLevelHintsCommand.cpp
 * @copyright   (c) 2016, Jean-David Gadina - www.xs-labs.com
 */

#include <DCE/MachO/Commands/TwoLevelHintsCommand.hpp>
#include <DCE/MachO/Header.hpp>
#include <DCE/BinaryStream.hpp>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

template<>
class XS::PIMPL::Object< DCE::MachO::Commands::TwoLevelHintsCommand >::IMPL
{
    public:
        
        IMPL( void );
        IMPL( const IMPL & o );
        ~IMPL( void );
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#define XS_PIMPL_CLASS DCE::MachO::Commands::TwoLevelHintsCommand
#include <XS/PIMPL/Object-IMPL.hpp>

namespace DCE
{
    namespace MachO
    {}
}

XS::PIMPL::Object< DCE::MachO::Commands::TwoLevelHintsCommand >::IMPL::IMPL( void )
{}

XS::PIMPL::Object< DCE::MachO::Commands::TwoLevelHintsCommand >::IMPL::IMPL( const IMPL & o )
{
    ( void )o;
}

XS::PIMPL::Object< DCE::MachO::Commands::TwoLevelHintsCommand >::IMPL::~IMPL( void )
{}
