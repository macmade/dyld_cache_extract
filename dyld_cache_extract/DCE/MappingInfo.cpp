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
 * @header      MappingInfo.cpp
 * @copyright   (c) 2016, Jean-David Gadina - www.xs-labs.com
 */

#include "MappingInfo.hpp"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

template<>
class XS::PIMPL::Object< DCE::MappingInfo >::IMPL
{
    public:
        
        IMPL( void );
        IMPL( const IMPL & o );
        ~IMPL( void );
        
        uint64_t    _address;
        uint64_t    _size;
        uint64_t    _fileOffset;
        uint32_t    _maxProt;
        uint32_t    _initProt;
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#define XS_PIMPL_CLASS DCE::MappingInfo
#include <XS/PIMPL/Object-IMPL.hpp>

namespace DCE
{
    MappingInfo::MappingInfo( void ): XS::PIMPL::Object< MappingInfo >()
    {}
    
    uint64_t MappingInfo::GetAddress( void ) const
    {
        return this->impl->_address;
    }
    
    uint64_t MappingInfo::GetSize( void ) const
    {
        return this->impl->_size;
    }
    
    uint64_t MappingInfo::GetFileOffset( void ) const
    {
        return this->impl->_fileOffset;
    }
    
    uint32_t MappingInfo::GetMaxProt( void ) const
    {
        return this->impl->_maxProt;
    }
    
    uint32_t MappingInfo::GetInitProt( void ) const
    {
        return this->impl->_initProt;
    }
    
    void MappingInfo::SetAddress( uint64_t value )
    {
        this->impl->_address = value;
    }
    
    void MappingInfo::SetSize( uint64_t value )
    {
        this->impl->_size = value;
    }
    
    void MappingInfo::SetFileOffset( uint64_t value )
    {
        this->impl->_fileOffset = value;
    }
    
    void MappingInfo::SetMaxProt( uint32_t value )
    {
        this->impl->_maxProt = value;
    }
    
    void MappingInfo::SetInitProt( uint32_t value )
    {
        this->impl->_initProt = value;
    }
}

XS::PIMPL::Object< DCE::MappingInfo >::IMPL::IMPL( void ):
    _address( 0 ),
    _size( 0 ),
    _fileOffset( 0 ),
    _maxProt( 0 ),
    _initProt( 0 )
{}

XS::PIMPL::Object< DCE::MappingInfo >::IMPL::IMPL( const IMPL & o ):
    _address( o._address ),
    _size( o._size ),
    _fileOffset( o._fileOffset ),
    _maxProt( o._maxProt ),
    _initProt( o._initProt )
{}

XS::PIMPL::Object< DCE::MappingInfo >::IMPL::~IMPL( void )
{}
