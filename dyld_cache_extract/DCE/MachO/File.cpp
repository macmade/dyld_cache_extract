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
 * @file        File.cpp
 * @copyright   (c) 2016, Jean-David Gadina - www.xs-labs.com
 */

#include <DCE/MachO/File.hpp>
#include <DCE/BinaryStream.hpp>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

template<>
class XS::PIMPL::Object< DCE::MachO::File >::IMPL
{
    public:
        
        IMPL( void );
        IMPL( const IMPL & o );
        ~IMPL( void );
        
        DCE::MachO::Header                                        _header;
        std::vector< std::shared_ptr< DCE::MachO::LoadCommand > > _loadCommands;
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#define XS_PIMPL_CLASS DCE::MachO::File
#include <XS/PIMPL/Object-IMPL.hpp>

namespace DCE
{
    namespace MachO
    {
        bool File::Read( BinaryStream & stream )
        {
            Header                                        header;
            std::vector< std::shared_ptr< LoadCommand > > commands;
            uint32_t                                      i;
            
            if( stream.IsGood() == false || stream.IsEOF() )
            {
                return false;
            }
            
            if( header.Read( stream ) == false )
            {
                return false;
            }
            
            for( i = 0; i < header.GetCommandsCount(); i++ )
            {
                {
                    std::shared_ptr< LoadCommand > command;
                    
                    command = LoadCommand::FromStream( header, stream );
                    
                    if( command == nullptr )
                    {
                        return false;
                    }
                    
                    commands.push_back( command );
                }
            }
            
            this->impl->_header       = header;
            this->impl->_loadCommands = commands;
            
            return true;
        }
        
        Header File::GetHeader( void ) const
        {
            return this->impl->_header;
        }
        
        std::vector< std::shared_ptr< LoadCommand > > File::GetLoadCommands( void ) const
        {
            return this->impl->_loadCommands;
        }
    }
}

XS::PIMPL::Object< DCE::MachO::File >::IMPL::IMPL( void )
{}

XS::PIMPL::Object< DCE::MachO::File >::IMPL::IMPL( const IMPL & o ):
    _header( o._header ),
    _loadCommands( o._loadCommands )
{}

XS::PIMPL::Object< DCE::MachO::File >::IMPL::~IMPL( void )
{}
