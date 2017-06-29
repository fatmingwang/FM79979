if you want to link .a file to the project make sure the file type is Mach-O object code

https://stackoverflow.com/questions/6124523/linking-a-static-library-to-an-ios-project-in-xcode-4
down vote
Xcode menu > View > Utilities > File Inspector

Select the static library file, and then set 'File Type' as 'Mach-O object code' in 'Identity and Type'.


!!================================!!
above is not working
I create a workplace and add 2 project into this workplace
and link Binary with libraries then choice workplace,it works.
make sure you have projects in the workplace not a project insode of a project!!
XCode sux,I try maby 30 times to make 2 project inside of the workplace
!!================================!!



IOS need a ViewController

and custom ViewController class


create a class with cocoTouch class and select the subClass type(GLKit?)

then ensure you hvae checked 

Is Initial View Controller

and in the project setup the Deployment info
the mai niterface set to the view you want