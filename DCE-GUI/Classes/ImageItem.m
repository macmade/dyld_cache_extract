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
 * @file        ImageItem.m
 * @copyright   (c) 2016, Jean-David Gadina - www.xs-labs.com
 */

#import "ImageItem.h"

NS_ASSUME_NONNULL_BEGIN

@interface ImageItem()

@property( atomic, readwrite, strong, nullable ) NSString     * title;
@property( atomic, readwrite, strong, nullable ) NSString     * subtitle;
@property( atomic, readwrite, strong, nullable ) NSImage      * icon;
@property( atomic, readwrite, strong           ) DCEImageInfo * info;

@end

NS_ASSUME_NONNULL_END

@implementation ImageItem

- ( instancetype )initWithImageInfo: ( DCEImageInfo * )info
{
    if( ( self = [ self init ] ) )
    {
        self.info     = info;
        self.title    = info.path.lastPathComponent;
        self.subtitle = [ info.path stringByDeletingLastPathComponent ];
        
        if( [ info.path containsString: [ NSString stringWithFormat: @"/%@.framework", info.path.lastPathComponent ] ] )
        {
            self.icon = [ NSImage imageNamed: @"Framework" ];
        }
        else if( [ info.path containsString: [ NSString stringWithFormat: @"/%@.bundle", info.path.lastPathComponent ] ] )
        {
            self.icon = [ NSImage imageNamed: @"Bundle" ];
        }
        else if( [ info.path.pathExtension isEqualToString: @"dylib" ] )
        {
            self.icon = [ NSImage imageNamed: @"Library" ];
        }
        else
        {
            NSString * path = info.path;

            if( path != nil )
            {
                self.icon = [ [ NSWorkspace sharedWorkspace ] iconForFile: path ];
            }
            else
            {
                self.icon = [ NSImage imageNamed: @"Library" ];
            }
        }
    }
    
    return self;
}

@end
