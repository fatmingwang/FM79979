//
//  GameViewController.m
//  MusicGame
//
//  Created by Cloud on 6/22/17.
//  Copyright © 2017 Cloud. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import "ViewController.h"

#include "stdafx.h"
#include "MusicGameApp.h"
cMusicGameApp*g_pMusicGameApp = nullptr;

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    glContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    if (!glContext) {
        NSLog(@"Failed to create ES context");
    }
    

    
    GLKView *view = (GLKView *)self.view;
    view.context = glContext;
    view.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
    //view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    view.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
     [EAGLContext setCurrentContext:glContext];


}

- (void)viewDidUnload
{
    [super viewDidUnload];
    

    
    if ([EAGLContext currentContext] == glContext) {
        [EAGLContext setCurrentContext:nil];
    }
    glContext = nil;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Release any cached data, images, etc. that aren't in use.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
        return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
    } else {
        return YES;
    }
}



- (void)update
{
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    if( !g_pMusicGameApp )
    {
        GLKView *view = (GLKView *)self.view;
        
        
        //CGSize l_Size = [UIScreen mainScreen].bounds.size;
        g_pMusicGameApp = new cMusicGameApp(Vector2(2208,1242),
                                            Vector2(view.drawableWidth,view.drawableHeight));
        g_pMusicGameApp->Init();
        //cGameApp::SetAcceptRationWithGameresolution(view.drawableWidth,view.drawableHeight,1600,1024);
       
    }
    if(g_pMusicGameApp)
        g_pMusicGameApp->Run();
    [ glContext presentRenderbuffer:GL_RENDERBUFFER];
}


- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    CGPoint location = [[touches anyObject] locationInView:[self view]];
    //UITouch* touch = [[event touchesForView:self] anyObject];
   //CGPoint location = [touch locationInView:self];
    GLKView *view = (GLKView *)self.view;
//    view.drawableWidth;
  //  view.drawableHeight;
    // Translate coordinates
    int locX = static_cast<int>(location.y)*view.contentScaleFactor;
    int locY = static_cast<int>(location.x)*view.contentScaleFactor;//view.drawableWidth-static_cast<int>(location.x);
    //NSLog(@"touchesBegan: (%d, %d)", locX, locY);
    
    g_pMusicGameApp->MouseDown(locY, locX);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    //UITouch* touch = [[event touchesForView:self] anyObject];
    //CGPoint location = [touch locationInView:self];
    CGPoint location = [[touches anyObject] locationInView:[self view]];
    GLKView *view = (GLKView *)self.view;
    int locX = static_cast<int>(location.y)*view.contentScaleFactor;
    int locY = static_cast<int>(location.x)*view.contentScaleFactor;//view.drawableWidth-static_cast<int>(location.x);
    //NSLog(@"touchesMoved: (%d, %d)", locX, locY);
    
    g_pMusicGameApp->MouseMove(locY, locX);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    //UITouch* touch = [[event touchesForView:self] anyObject];
    //CGPoint location = [touch locationInView:self];
    NSUInteger l_iNumTouches = [touches count];
    if( l_iNumTouches >= 3 )
    {
        cGameApp::m_sbDebugFunctionWorking = !cGameApp::m_sbDebugFunctionWorking;
    }
    CGPoint location = [[touches anyObject] locationInView:[self view]];
    GLKView *view = (GLKView *)self.view;
    int locX = static_cast<int>(location.y)*view.contentScaleFactor;
    int locY = static_cast<int>(location.x)*view.contentScaleFactor;//view.drawableWidth-static_cast<int>(location.x);
    //NSLog(@"touchesEnded: (%d, %d)", locX, locY);
    
    g_pMusicGameApp->MouseUp(locY, locX);
}


@end
