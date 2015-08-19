_CrtIsValidHeapPointer fails on globally created object within a static llibrary 

http://social.msdn.microsoft.com/forums/zh-TW/vcgeneral/thread/fa0e9340-619a-4b07-a86b-894358d415f6/

The reason why you get this error is that a winforms application has a managed entry point. The initialization of the native global objects is done by the CRT (C RunTime) startup routine. Since in this case there is no CRT startup routine the MyBoard global object fails to initialize correctly.

This issue looks very similar to the issue reported here http://lab.msdn.microsoft.com/ProductFeedback/viewFeedback.aspx?feedbackid=f4674f22-1382-492a-a2d6-1539dc50bf82.

If possible, always include the call-stack from the point you get the assert. This will greatly help the investigation. In this case, you can see that the call stack originates from the managed startup code.

I am assuming that you are compiling your app as /clr and your static library is compiled native. The following is a workaround solution to make the debug assertion fail message go away. You can see this workaround at http://lab.msdn.microsoft.com/ProductFeedback/ViewWorkaround.aspx?FeedbackID=FDBK44086#1

Workaround Steps:
In the project properties:

1. Set Linker\Advanced\Entry Point to "" (empty string)
2. Set Linker\System\Subsystem to Not Set

Step 1: Makes sure that the CRT startup code is invoked. This is because, if no entry point is specified, the linker will automatically use mainCRTStartup, which is defined in the CRT libraries. mainCRTStartup will make sure that the global object is initialized correctly.

Step 2: Makes sure that the linker will look for the symbol ¡§main¡¨. The linker looks for ¡§main¡¨ because mainCRTStartup calls main() in its body. The default option for a Winforms application is Subsystem:Windows and this makes the linker look for WinMain().

Thanks

Sarita Bafna

Visual C++ team
=============================================================
add mm to extension file
==========================================================
