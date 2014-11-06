
# Problem Statement

The challenge is based around implementing some simple logic in the native
layer of an Android application and sending notifications to the application
layer. There are three types of events that can be generated :

- User Generated Events : A message sent from a specific user
- System Generated Events : An notification representing a system event
- Logging Events : An event representing a piece of information to log

Your task is to implement the engine which generates one of these events
(randomly chosen) and send it to the application to display on a user
interface. The interval at which the events are sent can be up to you. All
events should have the identifer of the event type and a message associated
with it.

The application should stop sending events when put into the background and
then restart sending events when the application comes back into the forground.
While the application is not complicated, it should take into consideration
things like performance & resource consumption.

A skeleton project exists which already has a sample native method returning a
message that is being displayed on screen implemented.  Using this skeleton
project implement a very simple engine that creates an event and sends it back
to the UI to display the contents of the event through the listener callback
provided.

The focus here is not on UI design, or knowledge of Android, the focus instead
should be on production quality code but feel free to get creative. Consider
this application is going to be released to millions of people, not a side
hack, so focus on the things that show us what good, quality code means to you.
You are free to modify the gradle build script and structure in ways you see
fit to achieve the aims of this project however a README file with the
submission detailing your changes and reasons for it will be beneficial. 

Feel free to write the native layer of your code in C or C++.

# Environment setup

- In order to build the project the following environment variable must be set
  NDK_TOOLCHAIN_VERSION=4.8 

- Developed under: Ubuntu 14.04 LTS, kernel: 3.13.0-24-generic

- Used GCC version: 4.8.2

- Android NDK: r9d (64-bit)

- Some C++11/STL functionality is used to implement JNI module therefore
  streamer/build.gradle has been updated accordingly

- local.properties - contains the paths to the NDK and ADT; update according to
  yours if necessary

- to build the project call './gradlew build' it will download gradle if
  necessary and call.  call './gradlew tasks' to see other useful
  tasks/commands.

- launch_tomcat.sh script is used to monitor the output (stdout, stderr,
  stdlog) of our application.  For more sophisticated usage other filtering
  should be applied on tomcat.

- Unit tests are available in the unit-tests directory.  Just call 'make run'
  which will build and run the C++ based unit tests.

# Implementation details

From the high level the source code is divided into 2 pieces - Java part
(managed side) and the JNI (C++) part.  The task is to trigger events on the
JNI side and send notifications to the Java side (presentation layer) of the
application.  

The JNI engine which triggers the events is started by the presentation layer
(Java) - UI thread, upon application activation and is suspended when
application is put to the background or is closed.

The engine starting function in the JNI code is called by the UI thread so, in
order to keep the UI thread responsive we create another thread inside JNI
which is generating events and calls Java method, which in its turn must update
the UI so we redirect the textView updating part to the UI thread.

Another solution would be to setup a Timer (or Handler) instead.  From JNI the
messages would be pushed to a  queue and the timer would read and display on
the UI.  But this would bring the overhead caused by the timer, synchronization
when reading/writing form the queue.

Depending on the frequency, size of the message the importance how fast they
should be displayed to the user, multiple messages could be grouped/packaged to
reduce the number of calls from/to JNI.


