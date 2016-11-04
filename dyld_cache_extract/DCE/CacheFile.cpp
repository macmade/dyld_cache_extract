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
 * @header      CacheFile.cpp
 * @copyright   (c) 2016, Jean-David Gadina - www.xs-labs.com
 */

#include "CacheFile.hpp"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

template<>
class XS::PIMPL::Object< DCE::CacheFile >::IMPL
{
    public:
        
        IMPL( void );
        IMPL( const std::string & path );
        IMPL( const IMPL & o );
        ~IMPL( void );
        
        bool        _valid;
        std::string _path;
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#define XS_PIMPL_CLASS DCE::CacheFile
#include <XS/PIMPL/Object-IMPL.hpp>

namespace DCE
{
    CacheFile::CacheFile( void ): XS::PIMPL::Object< CacheFile >()
    {}
    
    CacheFile::CacheFile( const std::string & path ): XS::PIMPL::Object< CacheFile >( path )
    {}
    
    bool CacheFile::IsValid( void ) const
    {
        return this->impl->_valid;
    }
    
    std::string CacheFile::GetPath( void ) const
    {
        return this->impl->_path;
    }
    
    std::ostream & operator <<( std::ostream & os, const CacheFile & f )
    {
        ( void )f;
        
        return os;
    }
}

XS::PIMPL::Object< DCE::CacheFile >::IMPL::IMPL( void ):
    _valid( false )
{}

XS::PIMPL::Object< DCE::CacheFile >::IMPL::IMPL( const std::string & path ):
    _valid( false ),
    _path( path )
{
    
}

XS::PIMPL::Object< DCE::CacheFile >::IMPL::IMPL( const IMPL & o ):
    _valid( o._valid ),
    _path( o._path )
{}

XS::PIMPL::Object< DCE::CacheFile >::IMPL::~IMPL( void )
{}
