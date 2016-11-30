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
                case 0x01:              ret = std::make_shared< Commands::SegmentCommand          >(); break; /* LC_SEGMENT */
                case 0x02:              ret = std::make_shared< Commands::SymTabCommand           >(); break; /* LC_SYMTAB */
                case 0x03:              ret = std::make_shared< Commands::SymSegCommand           >(); break; /* LC_SYMSEG */
                case 0x04:              ret = std::make_shared< Commands::ThreadCommand           >(); break; /* LC_THREAD */
                case 0x05:              ret = std::make_shared< Commands::ThreadCommand           >(); break; /* LC_UNIXTHREAD */
                case 0x06:              ret = std::make_shared< Commands::FVMLibCommand           >(); break; /* LC_LOADFVMLIB */
                case 0x07:              ret = std::make_shared< Commands::FVMLibCommand           >(); break; /* LC_IDFVMLIB */
                case 0x08:              ret = std::make_shared< Commands::IdentCommand            >(); break; /* LC_IDENT */
                case 0x09:              ret = std::make_shared< Commands::FVMFileCommand          >(); break; /* LC_FVMFILE */
                case 0x0B:              ret = std::make_shared< Commands::DySymTabCommand         >(); break; /* LC_DYSYMTAB */
                case 0x0C:              ret = std::make_shared< Commands::DyLibCommand            >(); break; /* LC_LOAD_DYLIB */
                case 0x0D:              ret = std::make_shared< Commands::DyLibCommand            >(); break; /* LC_ID_DYLIB */
                case 0x0E:              ret = std::make_shared< Commands::DyLinkerCommand         >(); break; /* LC_LOAD_DYLINKER */
                case 0x0F:              ret = std::make_shared< Commands::DyLinkerCommand         >(); break; /* LC_ID_DYLINKER */
                case 0x10:              ret = std::make_shared< Commands::PreboundDyLibCommand    >(); break; /* LC_PREBOUND_DYLIB */
                case 0x11:              ret = std::make_shared< Commands::RoutinesCommand         >(); break; /* LC_ROUTINES */
                case 0x12:              ret = std::make_shared< Commands::SubFrameworkCommand     >(); break; /* LC_SUB_FRAMEWORK */
                case 0x13:              ret = std::make_shared< Commands::SubUmbrellaCommand      >(); break; /* LC_SUB_UMBRELLA */
                case 0x14:              ret = std::make_shared< Commands::SubClientCommand        >(); break; /* LC_SUB_CLIENT */
                case 0x15:              ret = std::make_shared< Commands::SubLibraryCommand       >(); break; /* LC_SUB_LIBRARY */
                case 0x16:              ret = std::make_shared< Commands::TwoLevelHintsCommand    >(); break; /* LC_TWOLEVEL_HINTS */
                case 0x17:              ret = std::make_shared< Commands::PrebindCKSumCommand     >(); break; /* LC_PREBIND_CKSUM */
                case 0x18 | 0x80000000: ret = std::make_shared< Commands::DyLibCommand            >(); break; /* LC_LOAD_WEAK_DYLIB */
                case 0x19:              ret = std::make_shared< Commands::SegmentCommand64        >(); break; /* LC_SEGMENT_64 */
                case 0x1A:              ret = std::make_shared< Commands::RoutinesCommand64       >(); break; /* LC_ROUTINES_64 */
                case 0x1B:              ret = std::make_shared< Commands::UUIDCommand             >(); break; /* LC_UUID */
                case 0x1C | 0x80000000: ret = std::make_shared< Commands::RPathCommand            >(); break; /* LC_RPATH */
                case 0x1D:              ret = std::make_shared< Commands::LinkEditDataCommand     >(); break; /* LC_CODE_SIGNATURE */
                case 0x1E:              ret = std::make_shared< Commands::LinkEditDataCommand     >(); break; /* LC_SEGMENT_SPLIT_INFO */
                case 0x1F | 0x80000000: ret = std::make_shared< Commands::DyLibCommand            >(); break; /* LC_REEXPORT_DYLIB */
                case 0x21:              ret = std::make_shared< Commands::EncryptionInfoCommand   >(); break; /* LC_ENCRYPTION_INFO */
                case 0x22:              ret = std::make_shared< Commands::DyldInfoCommand         >(); break; /* LC_DYLD_INFO */
                case 0x22 | 0x80000000: ret = std::make_shared< Commands::DyldInfoCommand         >(); break; /* LC_DYLD_INFO_ONLY */
                case 0x26:              ret = std::make_shared< Commands::LinkEditDataCommand     >(); break; /* LC_FUNCTION_STARTS */
                case 0x27:              ret = std::make_shared< Commands::DyLinkerCommand         >(); break; /* LC_DYLD_ENVIRONMENT */
                case 0x28 | 0x80000000: ret = std::make_shared< Commands::EntryPointCommand       >(); break; /* LC_MAIN */
                case 0x29:              ret = std::make_shared< Commands::LinkEditDataCommand     >(); break; /* LC_DATA_IN_CODE */
                case 0x2A:              ret = std::make_shared< Commands::SourceVersionCommand    >(); break; /* LC_SOURCE_VERSION */
                case 0x2B:              ret = std::make_shared< Commands::LinkEditDataCommand     >(); break; /* LC_DYLIB_CODE_SIGN_DRS */
                case 0x2C:              ret = std::make_shared< Commands::EncryptionInfoCommand64 >(); break; /* LC_ENCRYPTION_INFO_64 */
                case 0x2D:              ret = std::make_shared< Commands::LinkerOptionCommand     >(); break; /* LC_LINKER_OPTION */
                case 0x2E:              ret = std::make_shared< Commands::LinkEditDataCommand     >(); break; /* LC_LINKER_OPTIMIZATION_HINT */
                case 0x24:              ret = std::make_shared< Commands::VersionMinCommand       >(); break; /* LC_VERSION_MIN_MACOSX */
                case 0x25:              ret = std::make_shared< Commands::VersionMinCommand       >(); break; /* LC_VERSION_MIN_IPHONEOS */
                case 0x2F:              ret = std::make_shared< Commands::VersionMinCommand       >(); break; /* LC_VERSION_MIN_TVOS */
                case 0x30:              ret = std::make_shared< Commands::VersionMinCommand       >(); break; /* LC_VERSION_MIN_WATCHOS */
                default:                ret = std::make_shared< LoadCommand                       >(); break;
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
