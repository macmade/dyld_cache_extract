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
 * @header      ImageInfo.cpp
 * @copyright   (c) 2016, Jean-David Gadina - www.xs-labs.com
 */

#include "ImageInfo.hpp"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

template<>
class XS::PIMPL::Object< DCE::ImageInfo >::IMPL
{
    public:
        
        IMPL( void );
        IMPL( const IMPL & o );
        ~IMPL( void );
        
        uint64_t    _address;
        uint64_t    _mTime;
        uint64_t    _inode;
        std::string _path;
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#define XS_PIMPL_CLASS DCE::ImageInfo
#include <XS/PIMPL/Object-IMPL.hpp>

namespace DCE
{
    ImageInfo::ImageInfo( void ): XS::PIMPL::Object< ImageInfo >()
    {}
    
    uint64_t ImageInfo::GetAddress( void ) const
    {
        return this->impl->_address;
    }
    
    uint64_t ImageInfo::GetModificationTime( void ) const
    {
        return this->impl->_mTime;
    }
    
    uint64_t ImageInfo::GetInode( void ) const
    {
        return this->impl->_inode;
    }
    
    std::string ImageInfo::GetPath( void ) const
    {
        return this->impl->_path;
    }
    
    std::string ImageInfo::GetModificationDate( void ) const
    {
        char        dbuf[ 256 ];
        time_t      t;
        struct tm * now;
        
        t   = static_cast< time_t >( this->impl->_mTime );
        now = localtime( &t );
        
        strftime( static_cast< char * >( dbuf ), sizeof( dbuf ), "%Y-%m-%d %H:%M:%S", now );
        
        return std::string( dbuf );
    }
    
    void ImageInfo::SetAddress( uint64_t value )
    {
        this->impl->_address = value;
    }
    
    void ImageInfo::SetModificationTime( uint64_t value )
    {
        this->impl->_mTime = value;
    }
    
    void ImageInfo::SetInode( uint64_t value )
    {
        this->impl->_inode = value;
    }
    
    void ImageInfo::SetPath( const std::string & value )
    {
        this->impl->_path = value;
    }
}

XS::PIMPL::Object< DCE::ImageInfo >::IMPL::IMPL( void ):
    _address( 0 ),
    _mTime( 0 ),
    _inode( 0 )
{}

XS::PIMPL::Object< DCE::ImageInfo >::IMPL::IMPL( const IMPL & o ):
    _address( o._address ),
    _mTime( o._mTime ),
    _inode( o._inode ),
    _path( o._path )
{}

XS::PIMPL::Object< DCE::ImageInfo >::IMPL::~IMPL( void )
{}
