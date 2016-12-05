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
 * @file        MainWindowController.m
 * @copyright   (c) 2016, Jean-David Gadina - www.xs-labs.com
 */

#import "MainWindowController.h"
#import "ApplicationDelegate.h"
#import "FileItem.h"
#import "Preferences.h"

NS_ASSUME_NONNULL_BEGIN

@interface MainWindowController()

@property( atomic, readwrite, assign )          BOOL                    hasSelection;
@property( atomic, readwrite, strong )          NSArray< FileItem * > * items;
@property( atomic, readwrite, strong ) IBOutlet NSArrayController     * arrayController;

- ( IBAction )openSelectedFile: ( nullable id )sender;
- ( void )loadFiles;

@end

NS_ASSUME_NONNULL_END

@implementation MainWindowController

- ( instancetype )init
{
    return [ self initWithWindowNibName: NSStringFromClass( [ self class ] ) ];
}

- ( instancetype )initWithWindowNibName: ( NSString * )name
{
    if( ( self = [ super initWithWindowNibName: name ] ) )
    {
        self.items = @[];
    }
    
    return self;
}

- ( void )dealloc
{
    [ self.arrayController removeObserver: self forKeyPath: @"selectionIndexes" ];
    [ [ NSNotificationCenter defaultCenter ] removeObserver: self ];
}

- ( void )windowDidLoad
{
    [ super windowDidLoad ];
    [ self.arrayController addObserver: self forKeyPath: @"selectionIndexes" options: NSKeyValueObservingOptionNew context: NULL ];
    [ self loadFiles ];
    [ [ NSNotificationCenter defaultCenter ] addObserver: self selector: @selector( loadFiles ) name: PreferencesNotificationDefaultsChanged object: nil ];
}

- ( void )observeValueForKeyPath: ( NSString * )keyPath ofObject: ( id )object change: ( NSDictionary * )change context: ( void * )context
{
    if( object == self.arrayController && [ keyPath isEqualToString: @"selectionIndexes" ] )
    {
        self.hasSelection = self.arrayController.selectedObjects.count > 0;
    }
    else
    {
        [ super observeValueForKeyPath: keyPath ofObject: object change: change context: context ];
    }
}

- ( IBAction )openSelectedFile: ( nullable id )sender
{
    FileItem * item;
    NSURL    * url;
    
    ( void )sender;
    
    item = self.arrayController.selectedObjects.firstObject;
    
    if( item == nil )
    {
        return;
    }
    
    url = [ NSURL fileURLWithPath: item.path ];
    
    [ ( ApplicationDelegate * )( NSApp.delegate ) openURL: url ];
}

- ( void )loadFiles
{
    NSMutableArray * paths;
    NSString       * path;
    BOOL             isDir;
    FileItem       * item;
    
    [ self.arrayController removeObjects: self.items ];
    
    paths = [ [ Preferences sharedInstance ].recentFiles mutableCopy ];
    
    for( path in [ [ NSFileManager defaultManager ] contentsOfDirectoryAtPath: @"/var/db/dyld/" error: NULL ] )
    {
        path = [ @"/var/db/dyld/" stringByAppendingPathComponent: path ];
        
        if( path.pathExtension.length )
        {
            continue;
        }
        
        if( [ [ NSFileManager defaultManager ] fileExistsAtPath: path isDirectory: &isDir ] && isDir )
        {
            continue;
        }
        
        [ paths addObject: path ];
    }
    
    for( path in paths )
    {
        if( [ [ NSFileManager defaultManager ] fileExistsAtPath: path isDirectory: &isDir ] == NO || isDir )
        {
            continue;
        }
        
        item          = [ FileItem new ];
        item.path     = path;
        item.title    = path.lastPathComponent;
        item.subtitle = [ path stringByDeletingLastPathComponent ];
        item.icon     = [ NSImage imageNamed: @"Library" ];
        
        [ self.arrayController addObject: item ];
    }
    
    self.arrayController.sortDescriptors = @[ [ NSSortDescriptor sortDescriptorWithKey: @"title" ascending: YES selector: @selector( localizedCaseInsensitiveCompare: ) ] ];
}

@end
