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
 * @file        main.cpp
 * @copyright   (c) 2016, Jean-David Gadina - www.xs-labs.com
 */

#include "Arguments.hpp"
#include <DCE/CacheFile.hpp>

static void PrintUsage( void );

int main( int argc, const char * argv[] )
{
    Arguments      args( argc, argv );
    DCE::CacheFile file;
    
    if( args.ShowUsage() )
    {
        PrintUsage();
        
        return 0;
    }
    
    file = DCE::CacheFile( args.GetCacheFile() );
    
    if( file.Exists() == false )
    {
        std::cerr << "Error - cannot open DYLD cache file: " << file.GetPath() << std::endl;
        
        return -1;
    }
    
    if( file.IsValid() == false )
    {
        std::cerr << "Error - invalid DYLD cache file: " << file.GetPath() << std::endl;
        
        return -1;
    }
    
    if( args.PrintInfo() )
    {
        std::cout << file << std::endl;
    }
    else
    {
        
    }
    
    return 0;
}

void PrintUsage( void )
{
    std::cout << "dyld_cache_extract - Extractor for DYLD shared cache"
              << std::endl
              << std::endl
              << "Available options:"
              << std::endl
              << std::endl
              << "    --help                            Shows this help dialog."
              << std::endl
              << "    --info [PATH]                     Displays informations about a dyld_shared_cache file."
              << std::endl
              << "    --extract-all [PATH] [OUT_DIR]    Extracts all libraries from a dyld_shared_cache file."
              << std::endl
              << "    --extract [LIB] [PATH] [OUT_DIR]  Extracts a specific library from a dyld_shared_cache file."
              << std::endl
              << std::endl;
}
