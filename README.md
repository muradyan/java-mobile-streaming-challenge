#Mobile Streaming Engineer Challenge

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
