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
 * @header      DCECacheFile.h
 * @copyright   (c) 2016, Jean-David Gadina - www.xs-labs.com
 */

#ifdef __cplusplus
#import <Foundation/Foundation.h>
#else
@import Foundation;
#endif

#import <DCE/Objective-C/DCEImageInfo.h>
#import <DCE/Objective-C/DCEMappingInfo.h> 
#import <DCE/Objective-C/DCEMachOFile.h> 

NS_ASSUME_NONNULL_BEGIN

@interface DCECacheFile: NSObject

@property( atomic, readonly ) BOOL                          exists;
@property( atomic, readonly ) BOOL                          isValid;
@property( atomic, readonly ) NSString                    * path;
@property( atomic, readonly ) NSString                    * version;
@property( atomic, readonly ) NSString                    * architecture;
@property( atomic, readonly ) uint32_t                      mappingOffset;
@property( atomic, readonly ) uint32_t                      mappingCount;
@property( atomic, readonly ) uint32_t                      imagesOffset;
@property( atomic, readonly ) uint32_t                      imagesCount;
@property( atomic, readonly ) uint64_t                      dyldBaseAddress;
@property( atomic, readonly ) NSArray< DCEImageInfo   * > * imageInfos;
@property( atomic, readonly ) NSArray< DCEMappingInfo * > * mappingInfos;
@property( atomic, readonly ) NSArray< DCEMachOFile   * > * machOFiles;

- ( instancetype )initWithURL:  ( NSURL    * )url;
- ( instancetype )initWithPath: ( NSString * )path;

@end

NS_ASSUME_NONNULL_END
