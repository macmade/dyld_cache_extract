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
 * @file        ApplicationDelegate.m
 * @copyright   (c) 2016, Jean-David Gadina - www.xs-labs.com
 */

#import "ApplicationDelegate.h"
#import "AboutWindowController.h"
#import "MainWindowController.h"
#import "FileWindowController.h"

@import GitHubUpdates;

NS_ASSUME_NONNULL_BEGIN

@interface ApplicationDelegate()

@property( atomic, readwrite, strong )          AboutWindowController                    * aboutWindowController;
@property( atomic, readwrite, strong )          MainWindowController                     * mainWindowController;
@property( atomic, readwrite, strong )          NSMutableArray< FileWindowController * > * windowControllers;
@property( atomic, readwrite, strong ) IBOutlet GitHubUpdater                            * updater;

- ( void )windowWillClose: ( NSNotification * )notification;
- ( IBAction )openDocument: ( nullable id )sender;
- ( IBAction )showAboutWindow: ( nullable id )sender;

@end

NS_ASSUME_NONNULL_END

@implementation ApplicationDelegate

- ( void )applicationDidFinishLaunching: ( NSNotification * )notification
{
    ( void )notification;
    
    self.windowControllers    = [ NSMutableArray new ];
    self.mainWindowController = [ MainWindowController new ];
    
    [ self.mainWindowController.window center ];
    [ self.mainWindowController.window makeKeyAndOrderFront: nil ];
    [ self.updater checkForUpdatesInBackground ];
}

- ( void )windowWillClose: ( NSNotification * )notification
{
    NSWindow * window;
    
    window = notification.object;
    
    if( window == nil )
    {
        return;
    }
    
    [ [ NSNotificationCenter defaultCenter ] removeObserver: self name: NSWindowWillCloseNotification object: window ];
    [ self.windowControllers removeObject: window.windowController ];
}

- ( IBAction )openDocument: ( nullable id )sender
{
    NSOpenPanel * panel;
    
    ( void )sender;
    
    panel = [ NSOpenPanel openPanel ];
    
    panel.canChooseFiles                  = YES;
    panel.canChooseDirectories            = NO;
    panel.canCreateDirectories            = NO;
    panel.canSelectHiddenExtension        = YES;
    panel.showsHiddenFiles                = YES;
    panel.treatsFilePackagesAsDirectories = YES;
    panel.allowsMultipleSelection         = NO;
    
    if( [ panel runModal ] != NSModalResponseOK || panel.URL == nil )
    {
        return;
    }
    
    [ self openURL: panel.URL ];
}

- ( void )openURL: ( NSURL * )url
{
    FileWindowController * controller;
    NSPoint                origin;
    
    if( url == nil )
    {
        return;
    }
    
    controller = [ [ FileWindowController alloc ] initWithURL: url ];
    
    if( self.windowControllers.count == 0 )
    {
        [ controller.window center ];
    }
    else
    {
        origin = [ controller.window cascadeTopLeftFromPoint: self.windowControllers.lastObject.window.frame.origin ];
        
        [ controller.window setFrameOrigin: origin ];
    }
    
    [ [ NSNotificationCenter defaultCenter ] addObserver: self selector: @selector( windowWillClose: ) name: NSWindowWillCloseNotification object: controller.window ];
    [ controller.window makeKeyAndOrderFront: nil ];
    [ self.windowControllers addObject: controller ];
}

- ( IBAction )showAboutWindow: ( nullable id )sender
{
    if( self.aboutWindowController == nil )
    {
        self.aboutWindowController = [ AboutWindowController new ];
        
        [ self.aboutWindowController.window makeKeyAndOrderFront: sender ];
        [ self.aboutWindowController.window center ];
    }
    else
    {
        [ self.aboutWindowController.window makeKeyAndOrderFront: sender ];
    }
}

@end
