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
 * @file        SymTabCommand.cpp
 * @copyright   (c) 2016, Jean-David Gadina - www.xs-labs.com
 */

#include <DCE/MachO/Commands/SymTabCommand.hpp>
#include <DCE/MachO/Header.hpp>
#include <DCE/BinaryStream.hpp>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

template<>
class XS::PIMPL::Object< DCE::MachO::Commands::SymTabCommand >::IMPL
{
    public:
        
        IMPL( void );
        IMPL( const IMPL & o );
        ~IMPL( void );
        
        uint32_t _command;
        uint32_t _commandSize;
        uint32_t _symbolsOffset;
        uint32_t _symbolsCount;
        uint32_t _stringsOffset;
        uint32_t _stringsSize;
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#define XS_PIMPL_CLASS DCE::MachO::Commands::SymTabCommand
#include <XS/PIMPL/Object-IMPL.hpp>

namespace DCE
{
    namespace MachO
    {
        namespace Commands
        {
            bool SymTabCommand::Read( const Header & header, BinaryStream & stream )
            {
                ( void )header;
                
                if( stream.IsGood() == false || stream.IsEOF() )
                {
                    return false;
                }
                
                this->impl->_command        = stream.ReadUnsignedInteger();
                this->impl->_commandSize    = stream.ReadUnsignedInteger();
                this->impl->_symbolsOffset  = stream.ReadUnsignedInteger();
                this->impl->_symbolsCount   = stream.ReadUnsignedInteger();
                this->impl->_stringsOffset  = stream.ReadUnsignedInteger();
                this->impl->_stringsSize    = stream.ReadUnsignedInteger();
                
                stream.SeekG( this->impl->_commandSize - 24, std::ios_base::cur );
                
                if( stream.IsEOF() )
                {
                    return false;
                }
                
                return true;
            }
            
            uint32_t SymTabCommand::GetCommand( void ) const
            {
                return this->impl->_command;
            }
            
            uint32_t SymTabCommand::GetCommandSize( void ) const
            {
                return this->impl->_commandSize;
            }
            
            uint32_t SymTabCommand::GetSymbolsOffset( void ) const
            {
                return this->impl->_symbolsOffset;
            }
            
            uint32_t SymTabCommand::GetSymbolsCount( void ) const
            {
                return this->impl->_symbolsCount;
            }
            
            uint32_t SymTabCommand::GetStringsOffset( void ) const
            {
                return this->impl->_stringsOffset;
            }
            
            uint32_t SymTabCommand::GetStringsSize( void ) const
            {
                return this->impl->_stringsSize;
            }
        }
    }
}

XS::PIMPL::Object< DCE::MachO::Commands::SymTabCommand >::IMPL::IMPL( void ):
    _command( 0 ),
    _commandSize( 0 ),
    _symbolsOffset( 0 ),
    _symbolsCount( 0 ),
    _stringsOffset( 0 ),
    _stringsSize( 0 )
{}

XS::PIMPL::Object< DCE::MachO::Commands::SymTabCommand >::IMPL::IMPL( const IMPL & o ):
    _command( o._command ),
    _commandSize( o._commandSize ),
    _symbolsOffset( o._symbolsOffset ),
    _symbolsCount( o._symbolsCount ),
    _stringsOffset( o._stringsOffset ),
    _stringsSize( o._stringsSize )
{}

XS::PIMPL::Object< DCE::MachO::Commands::SymTabCommand >::IMPL::~IMPL( void )
{}
