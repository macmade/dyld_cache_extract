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
 * @header      Header.cpp
 * @copyright   (c) 2016, Jean-David Gadina - www.xs-labs.com
 */

#include "Header.hpp"
#include "BinaryStream.hpp"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

template<>
class XS::PIMPL::Object< DCE::MachO::Header >::IMPL
{
    public:
        
        IMPL( void );
        IMPL( const IMPL & o );
        ~IMPL( void );
        
        uint32_t _magic;
        uint32_t _cpuType;
        uint32_t _cpuSubType;
        uint32_t _fileType;
        uint32_t _commandsCount;
        uint32_t _commandsSize;
        uint32_t _flags;
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#define XS_PIMPL_CLASS DCE::MachO::Header
#include <XS/PIMPL/Object-IMPL.hpp>

namespace DCE
{
    namespace MachO
    {
        bool Header::Read( BinaryStream & stream )
        {
            if( stream.IsGood() == false || stream.IsEOF() )
            {
                return false;
            }
            
            this->impl->_magic = stream.ReadUnsignedInteger();
            
            if( this->Is32Bits() == false && this->Is64Bits() == false )
            {
                return false;
            }
            
            this->impl->_cpuType        = stream.ReadUnsignedInteger();
            this->impl->_cpuSubType     = stream.ReadUnsignedInteger();
            this->impl->_fileType       = stream.ReadUnsignedInteger();
            this->impl->_commandsCount  = stream.ReadUnsignedInteger();
            this->impl->_commandsSize   = stream.ReadUnsignedInteger();
            this->impl->_flags          = stream.ReadUnsignedInteger();
            
            if( this->Is64Bits() )
            {
                stream.ReadUnsignedInteger(); /* Reserved */
            }
            
            if( stream.IsEOF() )
            {
                return false;
            }
            
            return true;
        }
        
        bool Header::Is32Bits( void ) const
        {
            return this->impl->_magic == 0xFEEDFACE;
        }
        
        bool Header::Is64Bits( void ) const
        {
            return this->impl->_magic == 0xFEEDFACF;
        }
        
        uint32_t Header::GetMagic( void ) const
        {
            return this->impl->_magic;
        }
        
        uint32_t Header::GetCPUType( void ) const
        {
            return this->impl->_cpuType;
        }
        
        uint32_t Header::GetCPUSubType( void ) const
        {
            return this->impl->_cpuSubType;
        }
        
        uint32_t Header::GetFileType( void ) const
        {
            return this->impl->_fileType;
        }
        
        uint32_t Header::GetCommandsCount( void ) const
        {
            return this->impl->_commandsCount;
        }
        
        uint32_t Header::GetCommandsSize( void ) const
        {
            return this->impl->_commandsSize;
        }
        
        uint32_t Header::GetFlags( void ) const
        {
            return this->impl->_flags;
        }
    }
}

XS::PIMPL::Object< DCE::MachO::Header >::IMPL::IMPL( void ):
    _magic( 0 ),
    _cpuType( 0 ),
    _cpuSubType( 0 ),
    _fileType( 0 ),
    _commandsCount( 0 ),
    _commandsSize( 0 ),
    _flags( 0 )
{}

XS::PIMPL::Object< DCE::MachO::Header >::IMPL::IMPL( const IMPL & o ):
    _magic( o._magic ),
    _cpuType( o._cpuType ),
    _cpuSubType( o._cpuSubType ),
    _fileType( o._fileType ),
    _commandsCount( o._commandsCount ),
    _commandsSize( o._commandsSize ),
    _flags( o._flags )
{}

XS::PIMPL::Object< DCE::MachO::Header >::IMPL::~IMPL( void )
{}
