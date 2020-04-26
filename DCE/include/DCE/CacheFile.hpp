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
 * @header      CacheFile.hpp
 * @copyright   (c) 2016, Jean-David Gadina - www.xs-labs.com
 */

#ifndef DCE_CACHE_FILE_H
#define DCE_CACHE_FILE_H

#include <string>
#include <iostream>
#include <cstdint>
#include <vector>
#include <functional>
#include <DCE/ImageInfo.hpp>
#include <DCE/MappingInfo.hpp>
#include <DCE/MachO/File.hpp>
#include <XS/PIMPL/Object.hpp>

namespace DCE
{
    class CacheFile: public XS::PIMPL::Object< CacheFile >
    {
        public:
            
            typedef enum
            {
                ExtractDuplicateHandlingStop        = 0,
                ExtractDuplicateHandlingSkip        = 1,
                ExtractDuplicateHandlingOverwrite   = 2
            }
            ExtractDuplicateHandling;
            
            using XS::PIMPL::Object< CacheFile >::impl;
            
            CacheFile( void );
            CacheFile( const std::string & path );
            
            bool                            Exists( void )              const;
            bool                            IsValid( void )             const;
            std::string                     GetPath( void )             const;
            std::string                     GetVersion( void )          const;
            std::string                     GetArchitecture( void )     const;
            uint32_t                        GetMappingOffset( void )    const;
            uint32_t                        GetMappingCount( void )     const;
            uint32_t                        GetImagesOffset( void )     const;
            uint32_t                        GetImagesCount( void )      const;
            uint64_t                        GetDYLDBaseAddress( void )  const;
            std::vector< DCE::ImageInfo >   GetImageInfos( void )       const;
            std::vector< DCE::MappingInfo > GetMappingInfos( void )     const;
            std::vector< DCE::MachO::File > GetMachOFiles( void )       const;
            
            bool ExtractImage( const std::string & imagePath, const std::string & outDirPath, std::function< ExtractDuplicateHandling( const std::string &, const std::string & ) > duplicateHandler ) const;
            bool ExtractAll( const std::string & outDirPath, std::function< ExtractDuplicateHandling( const std::string &, const std::string & ) > duplicateHandler ) const;
            
            friend std::ostream & operator <<( std::ostream & os, const CacheFile & e );
    };
}

#endif /* DCE_CACHE_FILE_H */
