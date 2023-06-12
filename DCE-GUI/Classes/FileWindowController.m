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
#import "ObjectPair.h"
#import "Preferences.h"
#import <DCE/Objective-C/DCECacheFile.h>

NS_ASSUME_NONNULL_BEGIN

@interface FileWindowController() < NSTableViewDelegate, NSTableViewDataSource >

@property( atomic, readwrite, assign )          BOOL                      hasSelection;
@property( atomic, readwrite, assign )          BOOL                      extracting;
@property( atomic, readwrite, strong )          NSURL                   * url;
@property( atomic, readwrite, strong )          DCECacheFile            * file;
@property( atomic, readwrite, strong )          NSArray< ImageItem * >  * items;
@property( atomic, readwrite, strong ) IBOutlet NSArrayController       * itemsController;
@property( atomic, readwrite, strong )          NSArray< ObjectPair * > * infos;
@property( atomic, readwrite, strong ) IBOutlet NSArrayController       * infosController;
@property( atomic, readwrite, strong ) IBOutlet NSPopover               * infoPopover;

- ( IBAction )exportSelection: ( nullable id )sender;
- ( IBAction )showInfo: ( id )sender;
- ( void )extract: ( NSArray< ImageItem * > * )items to: ( NSURL * )destination;
- ( void )showAlertOnMainThread: ( NSAlert * )alert useModalSession: ( BOOL )modalSession completion: ( void ( ^ __nullable )( NSModalResponse returnCode ) )completion;

@end

NS_ASSUME_NONNULL_END

@implementation FileWindowController

- ( instancetype )init
{
    return [ self initWithWindowNibName: NSStringFromClass( [ self class ] ) ];
}

- ( nullable instancetype )initWithURL: ( NSURL * )url
{
    if( ( self = [ self init ] ) )
    {
        DCECacheFile * file = [ [ DCECacheFile alloc ] initWithURL: url ];

        if( file == nil )
        {
            return nil;
        }

        self.url   = url;
        self.file  = file;
        self.items = @[];
        self.infos = @[];
    }
    
    return self;
}

- ( void )dealloc
{
    [ self.itemsController removeObserver: self forKeyPath: @"selectionIndexes" ];
}

- ( void )windowDidLoad
{
    ImageItem    * item;
    DCEImageInfo * info;
    
    [ super windowDidLoad ];
    
    [ self.itemsController addObserver: self forKeyPath: @"selectionIndexes" options: NSKeyValueObservingOptionNew context: NULL ];

    NSString * name = self.url.path.lastPathComponent;

    if( name != nil )
    {
        self.window.title = name;
    }

    if( self.file.exists == NO || self.file.isValid == NO )
    {
        dispatch_after
        (
            dispatch_time( DISPATCH_TIME_NOW, ( int64_t )( 200 * NSEC_PER_MSEC ) ),
            dispatch_get_main_queue(),
            ^( void )
            {
                NSString * title;
                NSString * message;
                NSAlert  * alert;
                
                if( self.file.exists == NO )
                {
                    title   = NSLocalizedString( @"Missing File", nil );
                    message = [ NSString stringWithFormat: @"The file \"%@\" does not exist.", self.file.path.lastPathComponent ];
                }
                else
                {
                    title   = NSLocalizedString( @"Invalid File", nil );
                    message = [ NSString stringWithFormat: @"The file \"%@\" is not a valid DYLD shared cache file.", self.file.path.lastPathComponent ];
                }
                
                alert = [ NSAlert new ];
                
                alert.messageText     = title;
                alert.informativeText = message;

                NSWindow * window = self.window;

                if( window == nil )
                {
                    [ self close ];

                    return;
                }
                
                [ alert addButtonWithTitle: NSLocalizedString( @"Close", nil ) ];
                [ alert beginSheetModalForWindow: window completionHandler: ^( NSModalResponse response )
                    {
                        ( void )response;
                        
                        [ self close ];
                    }
                ];
            }
        );
        
        return;
    }

    {
        NSString * path = self.file.path;

        if( path != nil )
        {
            [ [ Preferences sharedInstance ] addRecentFile: path ];
        }
    }

    for( info in self.file.imageInfos )
    {
        item = [ [ ImageItem alloc ] initWithImageInfo: info ];
        
        if( item )
        {
            [ self.itemsController addObject: item ];
        }
    }
    
    if( self.items.count )
    {
        self.window.title = [ NSString stringWithFormat: @"%@ - %llu files", self.url.path.lastPathComponent, ( unsigned long long )( self.items.count ) ];
    }

    NSString * path         = self.file.path;
    NSString * version      = self.file.version;
    NSString * architecture = self.file.architecture;

    if( path == nil || version == nil || architecture == nil )
    {
        [ self close ];

        return;
    }
    
    {
        [ self.infosController addObject: [ [ ObjectPair alloc ] initWithFirst: @"File:"                second: path.lastPathComponent ] ];
        [ self.infosController addObject: [ [ ObjectPair alloc ] initWithFirst: @"Path:"                second: [ path stringByDeletingLastPathComponent ] ] ];
        [ self.infosController addObject: [ [ ObjectPair alloc ] initWithFirst: @"Version:"             second: version ] ];
        [ self.infosController addObject: [ [ ObjectPair alloc ] initWithFirst: @"Architecture:"        second: architecture ] ];
        [ self.infosController addObject: [ [ ObjectPair alloc ] initWithFirst: @"Mapping Offset:"      second: [ NSString stringWithFormat: @"0x%X", self.file.mappingOffset ] ] ];
        [ self.infosController addObject: [ [ ObjectPair alloc ] initWithFirst: @"Mapping Count:"       second: [ NSString stringWithFormat: @"%u", self.file.mappingCount ] ] ];
        [ self.infosController addObject: [ [ ObjectPair alloc ] initWithFirst: @"Image Offset:"        second: [ NSString stringWithFormat: @"0x%X", self.file.imagesOffset ] ] ];
        [ self.infosController addObject: [ [ ObjectPair alloc ] initWithFirst: @"Image Count:"         second: [ NSString stringWithFormat: @"%u", self.file.imagesCount ] ] ];
        [ self.infosController addObject: [ [ ObjectPair alloc ] initWithFirst: @"Dyld Base Address:"   second: [ NSString stringWithFormat: @"0x%llX", self.file.dyldBaseAddress ] ] ];
    }
    
    self.itemsController.sortDescriptors = @[ [ NSSortDescriptor sortDescriptorWithKey: @"title" ascending: YES selector: @selector( localizedCaseInsensitiveCompare: ) ] ];
}

- ( void )observeValueForKeyPath: ( NSString * )keyPath ofObject: ( id )object change: ( NSDictionary * )change context: ( void * )context
{
    if( object == self.itemsController && [ keyPath isEqualToString: @"selectionIndexes" ] )
    {
        self.hasSelection = self.itemsController.selectedObjects.count > 0;
    }
    else
    {
        [ super observeValueForKeyPath: keyPath ofObject: object change: change context: context ];
    }
}

- ( IBAction )exportSelection: ( nullable id )sender
{
    NSOpenPanel * panel;
    NSWindow    * window = self.window;

    if( window == nil )
    {
        NSBeep();

        return;
    }
    
    ( void )sender;
    
    panel = [ NSOpenPanel openPanel ];
    
    panel.canChooseFiles                  = NO;
    panel.canChooseDirectories            = YES;
    panel.canCreateDirectories            = YES;
    panel.canSelectHiddenExtension        = YES;
    panel.showsHiddenFiles                = NO;
    panel.treatsFilePackagesAsDirectories = NO;
    panel.allowsMultipleSelection         = NO;
    
    [ panel beginSheetModalForWindow: window completionHandler: ^( NSInteger res )
        {
            NSURL * url = panel.URL;

            if( res != NSModalResponseOK || url == nil )
            {
                return;
            }
            
            self.extracting = YES;
            
            dispatch_async
            (
                dispatch_get_global_queue( DISPATCH_QUEUE_PRIORITY_HIGH, 0 ),
                ^( void )
                {
                    [ self extract: self.itemsController.selectedObjects to: url ];
                    
                    dispatch_sync
                    (
                        dispatch_get_main_queue(),
                        ^( void )
                        {
                            self.extracting = NO;
                        }
                    );
                }
            );
        }
    ];
}

- ( IBAction )showInfo: ( id )sender
{
    NSView * view;
    
    if( self.infoPopover.isShown )
    {
        [ self.infoPopover performClose: sender ];
        
        return;
    }
    
    if( [ sender isKindOfClass: [ NSView class ] ] )
    {
        view = ( NSView * )sender;
    }
    else
    {
        return;
    }
    
    [ self.infoPopover showRelativeToRect: NSZeroRect ofView: view preferredEdge: NSMinYEdge ];
}

- ( void )extract: ( NSArray< ImageItem * > * )items to: ( NSURL * )destination
{
    ImageItem  * item;
    BOOL         extracted;
    __block BOOL stop;
    
    ( void )destination;
    
    stop = NO;
    
    for( item in items )
    {
        NSString * source = item.info.path;
        NSString * target = destination.path;

        if( source == nil || destination == nil )
        {
            continue;
        }

        extracted = [ self.file extractImage:     source
                                toDirectory:      target
                                duplicateHandler: ^ DCECacheFileExtractDuplicateHandling ( NSString * path, NSString * outDir )
            {
                        NSAlert                            * alert;
                __block DCECacheFileExtractDuplicateHandling handling;
                
                alert = [ NSAlert new ];
                
                alert.messageText     = NSLocalizedString( @"Duplicate file", nil );
                alert.informativeText = [ NSString stringWithFormat: @"A file named %@ already exists in %@. What would you like to do?", path.lastPathComponent, outDir.lastPathComponent ];
                
                [ alert addButtonWithTitle: NSLocalizedString( @"Overwrite", nil ) ];
                [ alert addButtonWithTitle: NSLocalizedString( @"Skip",      nil ) ];
                [ alert addButtonWithTitle: NSLocalizedString( @"Stop",      nil ) ];
                
                [ self showAlertOnMainThread: alert useModalSession: YES completion: ^( NSModalResponse res )
                    {
                        if( res == NSAlertFirstButtonReturn )
                        {
                            handling = DCECacheFileExtractDuplicateHandlingOverwrite;
                        }
                        else if( res == NSAlertSecondButtonReturn )
                        {
                            handling = DCECacheFileExtractDuplicateHandlingSkip;
                        }
                        else
                        {
                            handling = DCECacheFileExtractDuplicateHandlingStop;
                            stop     = YES;
                        }
                        
                        [ NSApp stopModal ];
                    }
                ];
                
                return handling;
            }
        ];
        
        if( stop )
        {
            return;
        }
        
        if( extracted == false )
        {
            {
                NSAlert * alert;
                
                alert = [ NSAlert new ];
                
                alert.messageText     = NSLocalizedString( @"Error", nil );
                alert.informativeText = NSLocalizedString( @"Cannot extract image files. An unknown error occured.", nil );
                
                [ alert addButtonWithTitle: NSLocalizedString( @"OK", nil ) ];
                [ self showAlertOnMainThread: alert useModalSession: NO completion: NULL ];
            }
            
            return;
        }
    }
}

- ( void )showAlertOnMainThread: ( NSAlert * )alert useModalSession: ( BOOL )modalSession completion: ( void ( ^ __nullable )( NSModalResponse returnCode ) )completion
{
    NSWindow * window = self.window;

    if( window == nil )
    {
        NSBeep();

        return;
    }

    if( [ NSThread isMainThread ] )
    {
        [ alert beginSheetModalForWindow: window completionHandler: completion ];
        
        if( modalSession )
        {
            [ NSApp runModalForWindow: window ];
        }
    }
    else
    {
        dispatch_sync
        (
            dispatch_get_main_queue(),
            ^( void )
            {
                [ alert beginSheetModalForWindow: window completionHandler: completion ];
                
                if( modalSession )
                {
                    [ NSApp runModalForWindow: window ];
                }
            }
        );
    }
}

#pragma mark - NSTableViewDataSource

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-implementations"
- ( BOOL )tableView: ( NSTableView * )tableView writeRowsWithIndexes: ( NSIndexSet * )rowIndexes toPasteboard: ( NSPasteboard * )pasteboard
#pragma clang diagnostic pop
{
    NSString                     * ext;
    ImageItem                    * item;
    NSArray< ImageItem * >       * items;
    NSMutableArray< NSString * > * extensions;
    NSMutableArray< NSString * > * paths;
    
    [ tableView setDraggingSourceOperationMask: NSDragOperationCopy forLocal: NO ];
    
    items      = [ self.itemsController.arrangedObjects objectsAtIndexes: rowIndexes ];
    extensions = [ NSMutableArray new ];
    paths      = [ NSMutableArray new ];
    
    for( item in items )
    {
        NSString * path = item.info.path;

        if( path == nil )
        {
            continue;
        }

        ext = path.pathExtension;
        
        if( ext == nil )
        {
            ext = @"";
        }
        
        [ extensions addObject: ext ];
        [ paths addObject: path ];
    }
    
    if( extensions.count )
    {
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wdeprecated-declarations"
        [ pasteboard setPropertyList: extensions forType: NSFilesPromisePboardType ];
        [ pasteboard setString: [ paths componentsJoinedByString: @"\n" ] forType: NSStringPboardType ];
        #pragma clang diagnostic pop

        return YES;
    }
    
    return NO;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-implementations"
- ( NSArray< NSString * > * )tableView: ( NSTableView * )tableView namesOfPromisedFilesDroppedAtDestination: ( NSURL * )dropDestination forDraggedRowsWithIndexes: ( NSIndexSet * )indexSet
#pragma clang diagnostic pop
{
    NSArray< ImageItem * > * items;
    
    ( void )tableView;
    
    items           = [ self.itemsController.arrangedObjects objectsAtIndexes: indexSet ];
    self.extracting = YES;
    
    dispatch_async
    (
        dispatch_get_global_queue( DISPATCH_QUEUE_PRIORITY_HIGH, 0 ),
        ^( void )
        {
            [ self extract: items to: dropDestination ];
            
            dispatch_sync
            (
                dispatch_get_main_queue(),
                ^( void )
                {
                    self.extracting = NO;
                }
            );
        }
    );
    
    return @[];
}

@end
