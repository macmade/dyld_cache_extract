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
 * @header      SegmentCommand64.hpp
 * @copyright   (c) 2016, Jean-David Gadina - www.xs-labs.com
 */

#ifndef DCE_MACH_O_COMMANDS_SEGMENT_COMMAND_64_H
#define DCE_MACH_O_COMMANDS_SEGMENT_COMMAND_64_H

#include <cstdint>
#include <string>
#include <XS/PIMPL/Object.hpp>
#include <DCE/MachO/LoadCommand.hpp>

namespace DCE
{
    namespace MachO
    {
        namespace Commands
        {
            class SegmentCommand64: public LoadCommand, public XS::PIMPL::Object< SegmentCommand64 >
            {
                public:
                    
                    using XS::PIMPL::Object< SegmentCommand64 >::impl;
                    
                    bool Read( const Header & header, BinaryStream & stream ) override;
                    
                    uint32_t GetCommand( void )     const override;
                    uint32_t GetCommandSize( void ) const override;
                    
                    std::string GetName( void )          const;
                    uint64_t    GetVMAddress( void )     const;
                    uint64_t    GetVMSize( void )        const;
                    uint64_t    GetFileOffset( void )    const;
                    uint64_t    GetFileSize( void )      const;
                    uint32_t    GetMaxProt( void )       const;
                    uint32_t    GetInitProt( void )      const;
                    uint32_t    GetSectionsCount( void ) const;
                    uint32_t    GetFlags( void )         const;
            };
        }
    }
}

#endif /* DCE_MACH_O_COMMANDS_SEGMENT_COMMAND_64_H */
