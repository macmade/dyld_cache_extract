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
 * @file        CacheFile.cpp
 * @copyright   (c) 2016, Jean-David Gadina - www.xs-labs.com
 */

#include <ios>
#include <iomanip>
#include <memory>
#include <iostream>
#include <fstream>
#include <string.h>
#include <DCE/CacheFile.hpp>
#include <DCE/BinaryStream.hpp>
#include <DCE/MachO/Commands/SegmentCommand.hpp>
#include <DCE/MachO/Commands/SegmentCommand64.hpp>

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
        
        void           Parse( void );
        bool           ParseHeader( DCE::BinaryStream & s );
        bool           ParseMappingInfos( DCE::BinaryStream & s );
        bool           ParseImageInfos( DCE::BinaryStream & s );
        bool           ParseMachOFiles( DCE::BinaryStream & s );
        std::streamoff GetFileOffsetForImageInfo( const DCE::ImageInfo & info ) const;
        bool           SaveMachOFile( const DCE::MachO::File & file, const DCE::ImageInfo & info, const std::string & path ) const;
        
        bool                            _exists;
        bool                            _valid;
        std::string                     _path;
        std::string                     _version;
        std::string                     _arch;
        uint32_t                        _mappingOffset;
        uint32_t                        _mappingCount;
        uint32_t                        _imagesOffset;
        uint32_t                        _imagesCount;
        uint64_t                        _dyldBaseAddress;
        std::vector< DCE::ImageInfo >   _imageInfos;
        std::vector< DCE::MappingInfo > _mappingInfos;
        std::vector< DCE::MachO::File > _machOFiles;
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
    
    bool CacheFile::Exists( void ) const
    {
        return this->impl->_exists;
    }
    
    bool CacheFile::IsValid( void ) const
    {
        return this->impl->_valid;
    }
    
    std::string CacheFile::GetPath( void ) const
    {
        return this->impl->_path;
    }
    
    std::string CacheFile::GetVersion( void ) const
    {
        return this->impl->_version;
    }
    
    std::string CacheFile::GetArchitecture( void ) const
    {
        return this->impl->_arch;
    }
    
    uint32_t CacheFile::GetMappingOffset( void ) const
    {
        return this->impl->_mappingOffset;
    }
    
    uint32_t CacheFile::GetMappingCount( void ) const
    {
        return this->impl->_mappingCount;
    }
    
    uint32_t CacheFile::GetImagesOffset( void ) const
    {
        return this->impl->_imagesOffset;
    }
    
    uint32_t CacheFile::GetImagesCount( void ) const
    {
        return this->impl->_imagesCount;
    }
    
    uint64_t CacheFile::GetDYLDBaseAddress( void ) const
    {
        return this->impl->_dyldBaseAddress;
    }
    
    std::vector< DCE::ImageInfo > CacheFile::GetImageInfos( void ) const
    {
        return this->impl->_imageInfos;
    }
    
    std::vector< DCE::MappingInfo > CacheFile::GetMappingInfos( void ) const
    {
        return this->impl->_mappingInfos;
    }
    
    std::vector< DCE::MachO::File > CacheFile::GetMachOFiles( void ) const
    {
        return this->impl->_machOFiles;
    }
    
    bool CacheFile::ExtractImage( const std::string & imagePath, const std::string & outDirPath, std::function< ExtractDuplicateHandling( const std::string &, const std::string & ) > duplicateHandler ) const
    {
        ImageInfo                info;
        MachO::File              macho;
        bool                     found;
        std::string              path;
        std::string              name;
        ExtractDuplicateHandling duplicateHandling;
        char                     c;
        size_t                   pos;
        size_t                   x;
        
        if( duplicateHandler == nullptr )
        {
            return false;
        }
        
        if( imagePath.length() == 0 || outDirPath.length() == 0 )
        {
            return false;
        }
        
        found = false;
        x     = 0;
        
        for( const auto & i: this->GetImageInfos() )
        {
            if( i.GetPath() == imagePath )
            {
                info  = i;
                
                if( x < this->impl->_machOFiles.size() )
                {
                    macho = this->impl->_machOFiles[ x ];
                }
                else
                {
                    return false;
                }
                
                found = true;
                
                break;
            }
            
            x++;
        }
        
        if( found == false )
        {
            return false;
        }
        
        path = outDirPath;
        
        #ifdef _WIN32
        c = '\\';
        #else
        c = '/';
        #endif
        
        if( path.back() != c )
        {
            path += c;
        }
        
        pos = imagePath.find_last_of( c );
        
        if( pos != std::string::npos )
        {
            name = imagePath.substr( pos + 1 );
        }
        else
        {
            name = imagePath;
        }
        
        if( name.length() == 0 )
        {
            return false;
        }
        
        path += name;
        
        {
            std::ifstream fs( path );
            
            if( fs.good() )
            {
                duplicateHandling = duplicateHandler( imagePath, outDirPath );
                
                if( duplicateHandling == ExtractDuplicateHandlingStop || duplicateHandling == ExtractDuplicateHandlingSkip )
                {
                    return true;
                }
            }
        }
        
        return this->impl->SaveMachOFile( macho, info, path );
    }
    
    bool CacheFile::ExtractAll( const std::string & outDirPath, std::function< ExtractDuplicateHandling( const std::string &, const std::string & ) > duplicateHandler ) const
    {
        if( duplicateHandler == nullptr )
        {
            return false;
        }
        
        for( const auto & i: this->GetImageInfos() )
        {
            if( this->ExtractImage( i.GetPath(), outDirPath, duplicateHandler ) == false )
            {
                return false;
            }
        }
        
        return true;
    }
    
    std::ostream & operator <<( std::ostream & os, const CacheFile & f )
    {
        if( f.Exists() == false || f.IsValid() == false )
        {
            return os;
        }
        
        os << "{"                                                                                   << std::endl;
        os << "    File:                "   << std::dec << std::uppercase << f.GetPath()            << std::endl;
        os << "    Version:             "   << std::dec << std::uppercase << f.GetVersion()         << std::endl;
        os << "    Architecture:        "   << std::dec << std::uppercase << f.GetArchitecture()    << std::endl;
        os << "    Mapping Offset:      0x" << std::hex << std::uppercase << f.GetMappingOffset()   << std::endl;
        os << "    Mapping Count:       "   << std::dec << std::uppercase << f.GetMappingCount()    << std::endl;
        os << "    Images Offset:       0x" << std::hex << std::uppercase << f.GetImagesOffset()    << std::endl;
        os << "    Images Count:        "   << std::dec << std::uppercase << f.GetImagesCount()     << std::endl;
        os << "    DYLD Base Address:   0x" << std::hex << std::uppercase << f.GetDYLDBaseAddress() << std::endl;
        
        os                   << std::endl;
        os << "    Mapping:" << std::endl;
        os << "    {"        << std::endl;
        
        for( const auto & m: f.GetMappingInfos() )
        {
            os << "        {"
               << std::endl
               << std::hex
               << std::uppercase
               << std::setfill( '0' )
               << std::setw( 16 )
               << "            Address:     0x"
               << m.GetAddress()
               << std::endl
               << std::dec
               << std::setw( 0 )
               << "            Size:        "
               << m.GetSize()
               << std::endl
               << std::hex
               << std::uppercase
               << std::setfill( '0' )
               << std::setw( 16 )
               << "            File Offset: 0x"
               << m.GetFileOffset()
               << std::endl
               << std::dec
               << std::setw( 0 )
               << "            Max Prot:    "
               << m.GetMaxProt()
               << std::endl
               << "            Init Prot:   "
               << m.GetInitProt()
               << std::endl
               << "        }"
               << std::endl;
        }
        
        os << "    }"       << std::endl;
        os                  << std::endl;
        os << "    Images:" << std::endl;
        os << "    {"       << std::endl;
        
        for( const auto & i: f.GetImageInfos() )
        {
            os << "        {"
               << std::endl
               << "            Path:          "
               << i.GetPath()
               << std::endl
               << std::hex
               << std::uppercase
               << std::setfill( '0' )
               << std::setw( 16 )
               << "            Address:       0x"
               << i.GetAddress()
               << std::endl
               << "            Last modified: "
               << i.GetModificationDate()
               << std::endl
               << std::dec
               << std::setw( 0 )
               << "            Inode:         " 
               << i.GetInode()
               << std::endl
               << "        }"
               << std::endl;
        }
        
        os << "    }" << std::endl;
        os << "}"     << std::endl;
        
        return os;
    }
}

XS::PIMPL::Object< DCE::CacheFile >::IMPL::IMPL( void ):
    _exists( false ),
    _valid( false ),
    _mappingOffset( 0 ),
    _mappingCount( 0 ),
    _imagesOffset( 0 ),
    _imagesCount( 0 ),
    _dyldBaseAddress( 0 )
{}

XS::PIMPL::Object< DCE::CacheFile >::IMPL::IMPL( const std::string & path ):
    _exists( false ),
    _valid( false ),
    _path( path ),
    _mappingOffset( 0 ),
    _mappingCount( 0 ),
    _imagesOffset( 0 ),
    _imagesCount( 0 ),
    _dyldBaseAddress( 0 )
{
    this->Parse();
}

XS::PIMPL::Object< DCE::CacheFile >::IMPL::IMPL( const IMPL & o ):
    _exists( o._exists ),
    _valid( o._valid ),
    _path( o._path ),
    _version( o._version ),
    _arch( o._arch ),
    _mappingOffset( o._mappingOffset ),
    _mappingCount( o._mappingCount ),
    _imagesOffset( o._imagesOffset ),
    _imagesCount( o._imagesCount ),
    _dyldBaseAddress( o._dyldBaseAddress ),
    _imageInfos( o._imageInfos ),
    _mappingInfos( o._mappingInfos ),
    _machOFiles( o._machOFiles )
{}

XS::PIMPL::Object< DCE::CacheFile >::IMPL::~IMPL( void )
{}

void XS::PIMPL::Object< DCE::CacheFile >::IMPL::Parse( void )
{
    DCE::BinaryStream s( this->_path );
    
    if( s.IsGood() == false )
    {
        return;
    }
    
    this->_exists = true;
    
    if( this->ParseHeader( s ) == false )
    {
        return;
    }
    
    if( this->ParseMappingInfos( s ) == false )
    {
        return;
    }
    
    if( this->ParseImageInfos( s ) == false )
    {
        return;
    }
    
    if( this->ParseMachOFiles( s ) == false )
    {
        return;
    }
    
    this->_valid = true;
}

bool XS::PIMPL::Object< DCE::CacheFile >::IMPL::ParseHeader( DCE::BinaryStream & s )
{
    char        header[ 16 ];
    std::string magic;
    std::string version;
    std::string arch;
    
    if( s.IsGood() == false || s.IsEOF() )
    {
        return false;
    }
    
    memset( header, 0, sizeof( header ) );
    s.Read( header, sizeof( header ) );
    
    magic = header;
    
    if( magic.find( " " ) == std::string::npos )
    {
        return false;
    }
    
    version = magic.substr( 0, magic.find( " " ) );
    arch    = magic.substr( magic.find_last_of( " " ) + 1 );
    
    if( version.find( "dyld_" ) == std::string::npos )
    {
        return false;
    }
    
    if( arch.length() == 0 )
    {
        return false;
    }
    
    this->_version = version;
    this->_arch    = arch;
    
    if( s.IsEOF() )
    {
        return false;
    }
    
    this->_mappingOffset = s.ReadUnsignedInteger();
    
    if( s.IsEOF() )
    {
        return false;
    }
    
    this->_mappingCount = s.ReadUnsignedInteger();
    
    if( s.IsEOF() )
    {
        return false;
    }
    
    this->_imagesOffset = s.ReadUnsignedInteger();
    
    if( s.IsEOF() )
    {
        return false;
    }
    
    this->_imagesCount = s.ReadUnsignedInteger();
    
    if( s.IsEOF() )
    {
        return false;
    }
    
    this->_dyldBaseAddress = s.ReadUnsignedLong();
    
    return true;
}

bool XS::PIMPL::Object< DCE::CacheFile >::IMPL::ParseMappingInfos( DCE::BinaryStream & s )
{
    uint32_t                        i;
    std::vector< DCE::MappingInfo > infos;
    
    if( s.IsGood() == false || s.IsEOF() )
    {
        return false;
    }
    
    s.SeekG( this->_mappingOffset, std::ios::beg );
    
    if( s.IsEOF() )
    {
        return false;
    }
    
    for( i = 0; i < this->_mappingCount; i++ )
    {
        {
            DCE::MappingInfo info;
            
            info.SetAddress( s.ReadUnsignedLong() );
            info.SetSize( s.ReadUnsignedLong() );
            info.SetFileOffset( s.ReadUnsignedLong() );
            info.SetMaxProt( s.ReadUnsignedInteger() );
            info.SetInitProt( s.ReadUnsignedInteger() );
            
            if( s.IsEOF() )
            {
                return false;
            }
            
            infos.push_back( info );
        }
    }
    
    this->_mappingInfos = infos;
    
    return true;
}

bool XS::PIMPL::Object< DCE::CacheFile >::IMPL::ParseImageInfos( DCE::BinaryStream & s )
{
    uint32_t                      i;
    std::vector< DCE::ImageInfo > infos;
    std::vector< uint32_t >       offsets;
    
    if( s.IsGood() == false || s.IsEOF() )
    {
        return false;
    }
    
    if( this->_imagesOffset == 0 || this->_imagesCount == 0 )
    {
        return false;
    }
    
    s.SeekG( this->_imagesOffset, std::ios::beg );
    
    if( s.IsEOF() )
    {
        return false;
    }
    
    for( i = 0; i < this->_imagesCount; i++ )
    {
        {
            DCE::ImageInfo info;
            uint32_t       offset;
            
            info.SetAddress( s.ReadUnsignedLong() );
            info.SetModificationTime( s.ReadUnsignedLong() );
            info.SetInode( s.ReadUnsignedLong() );
            
            offset = s.ReadUnsignedInteger();
            
            s.ReadUnsignedInteger(); /* Padding */
            
            if( s.IsEOF() || offset == 0 )
            {
                return false;
            }
            
            infos.push_back( info );
            offsets.push_back( offset );
        }
    }
    
    for( i = 0; i < this->_imagesCount; i++ )
    {
        s.SeekG( offsets[ i ], std::ios::beg );
        
        if( s.IsEOF() )
        {
            return false;
        }
        
        infos[ i ].SetPath( s.ReadNULLTerminatedString() );
    }
    
    this->_imageInfos = infos;
    
    return true;
}

bool XS::PIMPL::Object< DCE::CacheFile >::IMPL::ParseMachOFiles( DCE::BinaryStream & s )
{
    std::streamoff offset;
    
    if( s.IsGood() == false || s.IsEOF() )
    {
        return false;
    }
    
    if( this->_mappingInfos.size() == 0 )
    {
        return false;
    }
    
    for( const auto & i: this->_imageInfos )
    {
        offset = this->GetFileOffsetForImageInfo( i );
        
        if( offset < 0 )
        {
            return false;
        }
        
        {
            DCE::MachO::File file;
            
            s.SeekG( static_cast< std::streamoff >( offset ), std::ios::beg );
            
            if( file.Read( s ) == false )
            {
                return false;
            }
            
            this->_machOFiles.push_back( file );
        }
    }
    
    return this->_machOFiles.size() == this->_imageInfos.size();
}

std::streamoff XS::PIMPL::Object< DCE::CacheFile >::IMPL::GetFileOffsetForImageInfo( const DCE::ImageInfo & info ) const
{
    uint64_t offset;
    
    for( const auto & m: this->_mappingInfos )
    {
        if( m.GetAddress() <= info.GetAddress() && info.GetAddress() < m.GetAddress() + m.GetSize() )
        {
            offset = info.GetAddress() - ( m.GetAddress() + m.GetFileOffset() );
            
            return static_cast< std::streamoff >( offset );
        }
    }
    
    return -1;
}

bool XS::PIMPL::Object< DCE::CacheFile >::IMPL::SaveMachOFile( const DCE::MachO::File & file, const DCE::ImageInfo & info, const std::string & path ) const
{
    size_t                        size;
    size_t                        pos;
    bool                          found;
    std::streamoff                offset;
    DCE::BinaryStream             stream;
    std::ofstream                 out;
    std::vector< std::streamoff > offsets;
    
    pos    = 0;
    found  = false;
    offset = this->GetFileOffsetForImageInfo( info );
    
    if( offset < 0 )
    {
        return -1;
    }
    
    stream = DCE::BinaryStream( this->_path );
    
    if( stream.IsGood() == false )
    {
        return false;
    }
    
    for( const auto & i: this->_imageInfos )
    {
        {
            std::streamoff o;
            
            o = this->GetFileOffsetForImageInfo( i );
            
            if( o >= 0 )
            {
                offsets.push_back( o );
            }
        }
    }
    
    std::sort( offsets.begin(), offsets.end() );
    
    {
        size_t i;
        
        i = 0;
        
        for( const auto & o: offsets )
        {
            if( o == offset )
            {
                found = true;
                pos   = i;
            }
            
            i++;
        }
    }
    
    if( found == false )
    {
        return false;
    }
    
    if( static_cast< size_t >( pos ) == offsets.size() - 1 )
    {
        stream.SeekG( std::ios_base::end );
        
        if( stream.TellG() < offset )
        {
            return false;
        }
        
        size = static_cast< size_t >( stream.TellG() - offsets[ pos ] );
    }
    else
    {
        size = static_cast< size_t >( offsets[ pos + 1 ] - offsets[ pos ] );
    }
    
    if( size == 0 )
    {
        return false;
    }
    
    out = std::ofstream( path, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc );
    
    if( out.good() == false )
    {
        return false;
    }
    
    {
        char * buf;
        
        buf = new char[ size ];
        
        if( buf == nullptr )
        {
            std::remove( path.c_str() );
            
            return false;
        }
        
        stream.SeekG( offset );
        stream.Read( buf, static_cast< std::streamsize >( size ) );
        out.write( buf, static_cast< std::streamsize >( size ) );
        
        delete[] buf;
    }
    
    {
        DCE::MachO::Commands::SegmentCommand   * le;
        DCE::MachO::Commands::SegmentCommand64 * le64;
        uint64_t                                  leSize;
        uint64_t                                  leOffset;
        uint64_t                                  nullSize;
        char                                    * buf;
        
        le   = nullptr;
        le64 = nullptr;
        
        for( const auto & sp: file.GetLoadCommands() )
        {
            {
                DCE::MachO::Commands::SegmentCommand   * seg;
                DCE::MachO::Commands::SegmentCommand64 * seg64;
                
                seg   = dynamic_cast< DCE::MachO::Commands::SegmentCommand   * >( sp.get() );
                seg64 = dynamic_cast< DCE::MachO::Commands::SegmentCommand64 * >( sp.get() );
                
                if( seg != nullptr && seg->GetName() == "__LINKEDIT" )
                {
                    le = seg;
                    
                    break;
                }
                
                if( seg64 != nullptr && seg64->GetName() == "__LINKEDIT" )
                {
                    le64 = seg64;
                    
                    break;
                }
            }
        }
        
        if( le != nullptr )
        {
            leSize   = le->GetFileSize();
            leOffset = le->GetFileOffset();
        }
        else if( le64 != nullptr )
        {
            leSize   = le64->GetFileSize();
            leOffset = le64->GetFileOffset();
        }
        else
        {
            return false;
        }
        
        if( leSize == 0 || leOffset < static_cast< uint64_t >( offset ) + size )
        {
            return false;
        }
        
        nullSize = leOffset - size;
        
        if( nullSize > 0 )
        {
            buf = new char[ nullSize ];
            
            if( buf == nullptr )
            {
                return false;
            }
            
            memset( buf, 0, nullSize );
            out.write( buf, static_cast< std::streamsize >( nullSize ) );
            
            delete[] buf;
        }
        
        buf = new char[ leSize ];
        
        if( buf == nullptr )
        {
            return false;
        }
        
        stream.SeekG( static_cast< std::streamsize >( leOffset ) );
        stream.Read( buf, static_cast< std::streamsize >( leSize ) );
        out.write( buf, static_cast< std::streamsize >( leSize ) );
        
        delete[] buf;
    }
    
    return true;
}
