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
 * @file        DCECacheFile.mm
 * @copyright   (c) 2016, Jean-David Gadina - www.xs-labs.com
 */

#import <DCE/Objective-C/DCECacheFile.h>
#import <DCE/CacheFile.hpp>

NS_ASSUME_NONNULL_BEGIN

@interface DCECacheFile()
{
    DCE::CacheFile _file;
}

@end

NS_ASSUME_NONNULL_END

@implementation DCECacheFile

- ( nullable instancetype )initWithURL: ( NSURL * )url
{
    NSString * path = url.path;

    if( path == nil )
    {
        return nil;
    }

    return [ self initWithPath: path ];
}

- ( instancetype )initWithPath: ( NSString * )path
{
    if( ( self = [ self init ] ) )
    {
        _file = DCE::CacheFile( path.UTF8String );
    }
    
    return self;
}

- ( BOOL )exists
{
    @synchronized( self )
    {
        return _file.Exists();
    }
}

- ( BOOL )isValid
{
    @synchronized( self )
    {
        return _file.IsValid();
    }
}

- ( NSString * )path
{
    @synchronized( self )
    {
        return [ NSString stringWithUTF8String: _file.GetPath().c_str() ];
    }
}

- ( NSString * )version
{
    @synchronized( self )
    {
        return [ NSString stringWithUTF8String: _file.GetVersion().c_str() ];
    }
}

- ( NSString * )architecture
{
    @synchronized( self )
    {
        return [ NSString stringWithUTF8String: _file.GetArchitecture().c_str() ];
    }
}

- ( uint32_t )mappingOffset
{
    @synchronized( self )
    {
        return _file.GetMappingOffset();
    }
}

- ( uint32_t )mappingCount
{
    @synchronized( self )
    {
        return _file.GetMappingCount();
    }
}

- ( uint32_t )imagesOffset
{
    @synchronized( self )
    {
        return _file.GetImagesOffset();
    }
}

- ( uint32_t )imagesCount
{
    @synchronized( self )
    {
        return _file.GetImagesCount();
    }
}

- ( uint64_t )dyldBaseAddress
{
    @synchronized( self )
    {
        return _file.GetDYLDBaseAddress();
    }
}

- ( NSArray< DCEImageInfo * > * )imageInfos
{
    NSMutableArray< DCEImageInfo * > * infos;
    DCEImageInfo                     * info;
    
    @synchronized( self )
    {
        infos = [ NSMutableArray new ];
        
        for( const auto & i: _file.GetImageInfos() )
        {
            info = [ [ DCEImageInfo alloc ] initWithCPPObject: i ];
            
            if( info )
            {
                [ infos addObject: info ];
            }
        }
        
        return [ NSArray arrayWithArray: infos ];
    }
}

- ( NSArray< DCEMappingInfo * > * )mappingInfos
{
    NSMutableArray< DCEMappingInfo * > * infos;
    DCEMappingInfo                     * info;
    
    @synchronized( self )
    {
        infos = [ NSMutableArray new ];
        
        for( const auto & i: _file.GetMappingInfos() )
        {
            info = [ [ DCEMappingInfo alloc ] initWithCPPObject: i ];
            
            if( info )
            {
                [ infos addObject: info ];
            }
        }
        
        return [ NSArray arrayWithArray: infos ];
    }
}

- ( NSArray< DCEMachOFile * > * )machOFiles
{
    NSMutableArray< DCEMachOFile * > * files;
    DCEMachOFile                     * file;
    
    @synchronized( self )
    {
        files = [ NSMutableArray new ];
        
        for( const auto & f: _file.GetMachOFiles() )
        {
            file = [ [ DCEMachOFile alloc ] initWithCPPObject: f ];
            
            if( file )
            {
                [ files addObject: file ];
            }
        }
        
        return [ NSArray arrayWithArray: files ];
    }
}

- ( BOOL )extractImage: ( NSString * )path toDirectory: ( NSString * )outDir duplicateHandler: ( DCECacheFileExtractDuplicateHandler )handler
{
    if( handler == nullptr )
    {
        return NO;
    }
    
    return _file.ExtractImage
    (
        path.UTF8String,
        outDir.UTF8String,
        [ = ]( const std::string & s1, const std::string & s2 )
        {
            DCECacheFileExtractDuplicateHandling res;
            NSString                           * ns1 = [ NSString stringWithUTF8String: s1.c_str() ];
            NSString                           * ns2 = [ NSString stringWithUTF8String: s2.c_str() ];

            if( ns1 == nil || ns2 == nil )
            {
                return DCE::CacheFile::ExtractDuplicateHandlingStop;
            }

            res = handler( ns1, ns2 );
        
            if( res == DCECacheFileExtractDuplicateHandlingSkip )
            {
                return DCE::CacheFile::ExtractDuplicateHandlingSkip;
            }
            else if( res == DCECacheFileExtractDuplicateHandlingOverwrite )
            {
                return DCE::CacheFile::ExtractDuplicateHandlingOverwrite;
            }
            
            return DCE::CacheFile::ExtractDuplicateHandlingStop;
        }
    );
}

- ( BOOL )extractAll: ( NSString * )outDir duplicateHandler: ( DCECacheFileExtractDuplicateHandler )handler
{
    if( handler == nullptr )
    {
        return NO;
    }
    
    return _file.ExtractAll
    (
        outDir.UTF8String,
        [ = ]( const std::string & s1, const std::string & s2 )
        {
            DCECacheFileExtractDuplicateHandling res;
            NSString                           * ns1 = [ NSString stringWithUTF8String: s1.c_str() ];
            NSString                           * ns2 = [ NSString stringWithUTF8String: s2.c_str() ];

            if( ns1 == nil || ns2 == nil )
            {
                return DCE::CacheFile::ExtractDuplicateHandlingStop;
            }

            res = handler( ns1, ns2 );

            if( res == DCECacheFileExtractDuplicateHandlingSkip )
            {
                return DCE::CacheFile::ExtractDuplicateHandlingSkip;
            }
            else if( res == DCECacheFileExtractDuplicateHandlingOverwrite )
            {
                return DCE::CacheFile::ExtractDuplicateHandlingOverwrite;
            }
            
            return DCE::CacheFile::ExtractDuplicateHandlingStop;
        }
    );
}

@end
