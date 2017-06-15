/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

//BEGIN_INCLUDE(all)
#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>
#include "engine.h"
#include "../../../FishingFun/Win32Test/stdafx.h"
//extern int	g_iHeartTest;
void android_main(struct android_app* app)
{	
    // Make sure glue isn't stripped.
    app_dummy();

	NvEGLUtil* egl = NvEGLUtil::create();
    if (!egl) 
    {
        // if we have a basic EGL failure, we need to exit immediately; nothing else we can do
        nv_app_force_quit_no_cleanup(app);
        return;
    }
    Engine* engine = new Engine(*egl, app);
	long lastTime = egl->getSystemTime();
    // loop waiting for stuff to do.
	if(app->savedState)
	{
		//g_iHeartTest = *(int*)app->savedState;
	}
	while (nv_app_status_running(app))
    {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;
        // If not rendering, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident = ALooper_pollAll(((nv_app_status_focused(app) &&
										  engine->isGameplayMode()) ? 1 : 250),
        								NULL,
        								&events,
        								(void**)&source)) >= 0)
        {
			// If we timed out, then there are no pending messages
			if (ident == ALOOPER_POLL_TIMEOUT)
				break;
            // Process this event. 
            if (source != NULL)
                source->process(app, source);
            // Check if we are exiting.  If so, dump out
            if (!nv_app_status_running(app))
				break;
        }

		long currentTime = egl->getSystemTime();

		// clamp time - it must not go backwards, and we don't
		// want it to be more than a half second to avoid huge
		// delays causing issues.  Note that we do not clamp to above
		// zero because some tools will give us zero delta
		long deltaTime = currentTime - lastTime;
		if (deltaTime < 0)
			deltaTime = 0;
		else if (deltaTime > 500)
			deltaTime = 500;

		lastTime = currentTime;

		// Update the frame, which optionally updates time and animations
		// and renders
		while(glGetError())
		{
		
		}
		engine->renderFrame(true);
    }

	delete engine;
    delete egl;
}
