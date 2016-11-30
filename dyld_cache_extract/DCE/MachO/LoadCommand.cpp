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
 * @file        LoadCommand.cpp
 * @copyright   (c) 2016, Jean-David Gadina - www.xs-labs.com
 */

#include <DCE/MachO/LoadCommand.hpp>
#include <DCE/MachO/Header.hpp>
#include <DCE/BinaryStream.hpp>

/*
#include <DCE/MachO/Commands/DyldInfoCommand.hpp>
#include <DCE/MachO/Commands/DyLibCommand.hpp>
#include <DCE/MachO/Commands/DyLinkerCommand.hpp>
#include <DCE/MachO/Commands/DySymTabCommand.hpp>
#include <DCE/MachO/Commands/EncryptionInfoCommand.hpp>
#include <DCE/MachO/Commands/EncryptionInfoCommand64.hpp>
#include <DCE/MachO/Commands/EntryPointCommand.hpp>
#include <DCE/MachO/Commands/FVMFileCommand.hpp>
#include <DCE/MachO/Commands/FVMLibCommand.hpp>
#include <DCE/MachO/Commands/IdentCommand.hpp>
#include <DCE/MachO/Commands/LinkEditDataCommand.hpp>
#include <DCE/MachO/Commands/LinkerOptionCommand.hpp>
#include <DCE/MachO/Commands/PrebindCKSumCommand.hpp>
#include <DCE/MachO/Commands/PreboundDyLibCommand.hpp>
#include <DCE/MachO/Commands/RoutinesCommand.hpp>
#include <DCE/MachO/Commands/RoutinesCommand64.hpp>
#include <DCE/MachO/Commands/RPathCommand.hpp>
#include <DCE/MachO/Commands/SegmentCommand.hpp>
#include <DCE/MachO/Commands/SegmentCommand64.hpp>
#include <DCE/MachO/Commands/SourceVersionCommand.hpp>
#include <DCE/MachO/Commands/SubClientCommand.hpp>
#include <DCE/MachO/Commands/SubFrameworkCommand.hpp>
#include <DCE/MachO/Commands/SubLibraryCommand.hpp>
#include <DCE/MachO/Commands/SubUmbrellaCommand.hpp>
#include <DCE/MachO/Commands/SymSegCommand.hpp>
#include <DCE/MachO/Commands/SymTabCommand.hpp>
#include <DCE/MachO/Commands/ThreadCommand.hpp>
#include <DCE/MachO/Commands/TwoLevelHintsCommand.hpp>
#include <DCE/MachO/Commands/UUIDCommand.hpp>
#include <DCE/MachO/Commands/VersionMinCommand.hpp>
*/

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

template<>
class XS::PIMPL::Object< DCE::MachO::LoadCommand >::IMPL
{
    public:
        
        IMPL( void );
        IMPL( const IMPL & o );
        ~IMPL( void );
        
        uint32_t _command;
        uint32_t _commandSize;
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#define XS_PIMPL_CLASS DCE::MachO::LoadCommand
#include <XS/PIMPL/Object-IMPL.hpp>

namespace DCE
{
    namespace MachO
    {
        std::shared_ptr< LoadCommand > LoadCommand::FromStream( const Header & header, BinaryStream & stream )
        {
            std::streampos                 pos;
            uint32_t                       command;
            std::shared_ptr< LoadCommand > ret;
            
            if( stream.IsGood() == false || stream.IsEOF() )
            {
                return nullptr;
            }
            
            pos     = stream.TellG();
            command = stream.ReadUnsignedInteger();
            
            stream.SeekG( pos );
            
            switch( command )
            {
                default: ret = std::make_shared< LoadCommand >(); break;
            }
            
            if( ret == nullptr )
            {
                return {};
            }
            
            if( ret->Read( header, stream ) )
            {
                return ret;
            }
            
            return {};
        }
        
        bool LoadCommand::Read( const Header & header, BinaryStream & stream )
        {
            std::streampos pos;
            
            ( void )header;
            
            if( stream.IsGood() == false || stream.IsEOF() )
            {
                return false;
            }
            
            pos                      = stream.TellG();
            this->impl->_command     = stream.ReadUnsignedInteger();
            this->impl->_commandSize = stream.ReadUnsignedInteger();
            
            if( stream.IsEOF() )
            {
                return false;
            }
            
            stream.SeekG( pos + static_cast< std::streampos >( this->impl->_commandSize ) );
            
            if( stream.IsEOF() )
            {
                return false;
            }
            
            return true;
        }
        
        uint32_t LoadCommand::GetCommand( void ) const
        {
            return this->impl->_command;
        }
        
        uint32_t LoadCommand::GetCommandSize( void ) const
        {
            return this->impl->_commandSize;
        }
    }
}

XS::PIMPL::Object< DCE::MachO::LoadCommand >::IMPL::IMPL( void ):
    _command( 0 ),
    _commandSize( 0 )
{}

XS::PIMPL::Object< DCE::MachO::LoadCommand >::IMPL::IMPL( const IMPL & o ):
    _command( o._command ),
    _commandSize( o._commandSize )
{}

XS::PIMPL::Object< DCE::MachO::LoadCommand >::IMPL::~IMPL( void )
{}
