//
//  GameViewController.h
//  MusicGame
//
//  Created by Cloud on 6/22/17.
//  Copyright © 2017 Cloud. All rights reserved.
//

#import <GLKit/GLKit.h>

@interface ViewController : GLKViewController
{
@private
    // GL context
    EAGLContext *glContext;

    /* OpenGL names for the renderbuffer and framebuffers used to render to this view */
    GLuint viewRenderbuffer, viewFramebuffer;

    /* OpenGL name for the depth buffer that is attached to viewFramebuffer, if it exists (0 if it does not exist) */
    GLuint depthRenderbuffer;

    //NSTimer *animationTimer;
    //NSTimeInterval animationInterval;
}

@end
