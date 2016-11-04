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

#include <ios>
#include <iomanip>
#include "CacheFile.hpp"
#include "BinaryStream.hpp"

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
        
        void Parse( void );
        bool ParseHeader( DCE::BinaryStream & s );
        bool ParseImages( DCE::BinaryStream & s );
        
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
        std::vector< DCE::ImageInfo >   _images;
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
    
    std::vector< DCE::ImageInfo > CacheFile::GetImages( void ) const
    {
        return this->impl->_images;
    }
    
    std::ostream & operator <<( std::ostream & os, const CacheFile & f )
    {
        if( f.Exists() == false || f.IsValid() == false )
        {
            return os;
        }
        
        os << "File:                "   << std::dec << std::uppercase << f.GetPath()            << std::endl;
        os << "Version:             "   << std::dec << std::uppercase << f.GetVersion()         << std::endl;
        os << "Architecture:        "   << std::dec << std::uppercase << f.GetArchitecture()    << std::endl;
        os << "Mapping Offset:      0x" << std::hex << std::uppercase << f.GetMappingOffset()   << std::endl;
        os << "Mapping Count:       "   << std::dec << std::uppercase << f.GetMappingCount()    << std::endl;
        os << "Images Offset:       0x" << std::hex << std::uppercase << f.GetImagesOffset()    << std::endl;
        os << "Images Count:        "   << std::dec << std::uppercase << f.GetImagesCount()     << std::endl;
        os << "DYLD Base Address:   0x" << std::hex << std::uppercase << f.GetDYLDBaseAddress() << std::endl;
        
        os << std::endl;
        os << "Images:";
        os << std::endl;
        os << std::endl;
        
        for( const auto & i: f.GetImages() )
        {
            os << "    "
               << i.GetModificationDate()
               << " - 0x"
               << std::hex
               << std::uppercase
               << std::setfill( '0' )
               << std::setw( 16 )
               << i.GetAddress()
               << std::endl;
               
            os << "    "
               << i.GetPath()
               << std::endl
               << std::endl;
        }
        
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
    _images( o._images )
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
    
    if( this->ParseImages( s ) == false )
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

bool XS::PIMPL::Object< DCE::CacheFile >::IMPL::ParseImages( DCE::BinaryStream & s )
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
    
    this->_images = infos;
    
    return true;
}
