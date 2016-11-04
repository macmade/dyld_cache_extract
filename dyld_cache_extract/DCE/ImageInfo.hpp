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
 * @header      ImageInfo.hpp
 * @copyright   (c) 2016, Jean-David Gadina - www.xs-labs.com
 */

#ifndef DCE_IMAGE_INFO_H
#define DCE_IMAGE_INFO_H

#include <cstdint>
#include <string>
#include <XS/PIMPL/Object.hpp>

namespace DCE
{
    class ImageInfo: public XS::PIMPL::Object< ImageInfo >
    {
        public:
            
            using XS::PIMPL::Object< ImageInfo >::impl;
            
            ImageInfo( void );
            
            uint64_t    GetAddress( void )          const;
            uint64_t    GetModificationTime( void ) const;
            uint64_t    GetInode( void )            const;
            std::string GetPath( void )             const;
            
            void SetAddress( uint64_t value );
            void SetModificationTime( uint64_t value );
            void SetInode( uint64_t value );
            void SetPath( const std::string & value );
    };
}

#endif /* DCE_IMAGE_INFO_H */
