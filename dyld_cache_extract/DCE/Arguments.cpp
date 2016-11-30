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
 * @file        Arguments.cpp
 * @copyright   (c) 2016, Jean-David Gadina - www.xs-labs.com
 */

#include <DCE/Arguments.hpp>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

template<>
class XS::PIMPL::Object< DCE::Arguments >::IMPL
{
    public:
        
        IMPL( void );
        IMPL( int argc, const char ** argv );
        IMPL( const IMPL & o );
        ~IMPL( void );
        
        int           _argc;
        const char ** _argv;
        bool          _showUsage;
        bool          _printInfo;
        bool          _extract;
        std::string   _cacheFile;
        std::string   _outDir;
        std::string   _library;
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#define XS_PIMPL_CLASS DCE::Arguments
#include <XS/PIMPL/Object-IMPL.hpp>

namespace DCE
{
    Arguments::Arguments( void ): XS::PIMPL::Object< Arguments >()
    {}
    
    Arguments::Arguments( int argc, const char ** argv ): XS::PIMPL::Object< Arguments >( argc, argv )
    {}
    
    bool Arguments::ShowUsage( void ) const
    {
        return this->impl->_showUsage;
    }
    
    bool Arguments::PrintInfo( void ) const
    {
        return this->impl->_printInfo;
    }
    
    bool Arguments::Extract( void ) const
    {
        return this->impl->_extract;
    }
    
    std::string Arguments::GetCacheFile( void ) const
    {
        return this->impl->_cacheFile;
    }
    
    std::string Arguments::GetOutputDirectory( void ) const
    {
        return this->impl->_outDir;
    }
    
    std::string Arguments::GetLibraryName( void ) const
    {
        return this->impl->_library;
    }
}

XS::PIMPL::Object< DCE::Arguments >::IMPL::IMPL( void ):
    _argc( 0 ),
    _argv( NULL ),
    _showUsage( true ),
    _printInfo( false ),
    _extract( false )
{}

XS::PIMPL::Object< DCE::Arguments >::IMPL::IMPL( int argc, const char ** argv ):
    _argc( argc ),
    _argv( argv ),
    _showUsage( false ),
    _printInfo( false ),
    _extract( false )
{
    int         i;
    std::string arg;
    
    for( i = 0; i < argc; i++ )
    {
        arg = ( argv[ i ] ) ? argv[ i ] : "";
        
        if( arg == "--help" )
        {
            this->_showUsage = true;
            
            break;
        }
        
        if( arg == "--info" )
        {
            if( i < argc - 1 )
            {
                this->_cacheFile = ( argv[ i + 1 ] ) ? argv[ i + 1 ] : "";
            }
            
            if( this->_cacheFile.length() )
            {
                this->_printInfo = true;
            }
            
            break;
        }
        
        if( arg == "--extract-all" )
        {
            if( i < argc - 2 )
            {
                this->_cacheFile = ( argv[ i + 1 ] ) ? argv[ i + 1 ] : "";
                this->_outDir    = ( argv[ i + 2 ] ) ? argv[ i + 2 ] : "";
            }
            
            if( this->_cacheFile.length() && this->_outDir.length() )
            {
                this->_extract = true;
            }
            
            break;
        }
        
        if( arg == "--extract" )
        {
            if( i < argc - 3 )
            {
                this->_cacheFile = ( argv[ i + 1 ] ) ? argv[ i + 1 ] : "";
                this->_outDir    = ( argv[ i + 2 ] ) ? argv[ i + 2 ] : "";
                this->_library   = ( argv[ i + 3 ] ) ? argv[ i + 3 ] : "";
            }
            
            if( this->_cacheFile.length() && this->_outDir.length() && this->_library.length() )
            {
                this->_extract = true;
            }
            
            break;
        }
    }
    
    if( this->_printInfo == false && this->_extract == false )
    {
        this->_showUsage = true;
    }
}

XS::PIMPL::Object< DCE::Arguments >::IMPL::IMPL( const IMPL & o ):
    _argc( o._argc ),
    _argv( o._argv ),
    _showUsage( o._showUsage ),
    _printInfo( o._printInfo ),
    _extract( o._extract ),
    _cacheFile( o._cacheFile ),
    _outDir( o._outDir ),
    _library( o._library )
{}

XS::PIMPL::Object< DCE::Arguments >::IMPL::~IMPL( void )
{}
