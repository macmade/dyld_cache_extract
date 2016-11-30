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
 * @file        FileWindowController.m
 * @copyright   (c) 2016, Jean-David Gadina - www.xs-labs.com
 */

#import "FileWindowController.h"
#import "ImageItem.h"
#import <DCE/Objective-C/DCECacheFile.h>

NS_ASSUME_NONNULL_BEGIN

@interface FileWindowController()

@property( atomic, readwrite, strong )          NSURL                  * url;
@property( atomic, readwrite, strong )          DCECacheFile           * file;
@property( atomic, readwrite, strong )          NSArray< ImageItem * > * items;
@property( atomic, readwrite, strong ) IBOutlet NSArrayController      * arrayController;

@end

NS_ASSUME_NONNULL_END

@implementation FileWindowController

- ( instancetype )init
{
    return [ self initWithWindowNibName: NSStringFromClass( [ self class ] ) ];
}

- ( instancetype )initWithURL: ( NSURL * )url
{
    if( ( self = [ self init ] ) )
    {
        self.url   = url;
        self.file  = [ [ DCECacheFile alloc ] initWithURL: url ];
        self.items = @[];
    }
    
    return self;
}

- ( void )windowDidLoad
{
    ImageItem    * item;
    DCEImageInfo * info;
    
    [ super windowDidLoad ];
    
    self.window.title = self.url.path.lastPathComponent;
    
    if( self.file.exists == NO || self.file.isValid == NO )
    {
        dispatch_after
        (
            dispatch_time( DISPATCH_TIME_NOW, ( int64_t )( 200 * NSEC_PER_MSEC ) ),
            dispatch_get_main_queue(),
            ^( void )
            {
                [ self close ];
            }
        );
        
        return;
    }
    
    for( info in self.file.imageInfos )
    {
        item = [ [ ImageItem alloc ] initWithImageInfo: info ];
        
        if( item )
        {
            [ self.arrayController addObject: item ];
        }
    }
    
    if( self.items.count )
    {
        self.window.title = [ NSString stringWithFormat: @"%@ - %llu files", self.url.path.lastPathComponent, ( unsigned long long )( self.items.count ) ];
    }
    
    self.arrayController.sortDescriptors = @[ [ NSSortDescriptor sortDescriptorWithKey: @"title" ascending: YES selector: @selector( localizedCaseInsensitiveCompare: ) ] ];
}

@end
