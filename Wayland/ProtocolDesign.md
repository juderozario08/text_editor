## Messages
- Each message is an event (in the case of a server) and a request (in the case of a client)
- Each of these act on an object
- Message header is 2 words
	- affected object ID
	- 2 16-bit values; upper 16 bits is the size of the message and the other is the opcode of the request/event
- Recipient looks up the object ID's interface and the event/request defined by its opcode to determine the signature of the message

## Object ID
- When a message comes with *new_id* argument, sender allocates an *object ID*
- the ID is used in future messages as either the first word of the header or as an *object_id* argument
- ID of 0 represents a NULL object

## Interfaces, Requests and Events
- Protocol works by issuing *requests* and *events* that act on objects
- each object has an interface that defines what `requests and events` are possible
- example: wl_surface is an interface. Surface is a box of pixels that can be displayed on screen
- ### Requests
	- example request sent from the client is called *"damage"* which client uses to indicate part of surface changed and redraw needed
```
0000000A    Object ID (10) -> ID is 10
00180002    Message length (24) and request opcode (2) -> total length of the message is 24 and opcode is 2
# the following are just the arguments being passed in the message
00000000    X coordinate (int): 0
00000000    Y coordinate (int): 0
00000100    Width        (int): 256
00000100    Height       (int): 256
```
- ### Events (messages from server)
	- example event is *"enter"* which is sent when the surface is being displayed on a specific output
	- message references another object ID: wl_output object which the surface is being shown on.
	- After processing the message, then it dispatches the processing internally
```
0000000A    Object ID (10)
000C0000    Message length (12) and event opcode (0)
00000005    Output (object ID): 5
```
## Atomicity
- Most interfaces allows transactional updates using several requests to build up a new representation of its state,  and then commits them all at once.
- Example, several properties of `wl_surface` can be configured
	- attached pixel buffer
	- damaged area to be redrawn
	- region defined as opaque
	- region where input events are acceptable
	- transformations
	- buffer scale
- They are originally in pending state. After commit, these get merged into current state achieving perfect frame

## Resource Lifetimes
- Used to avoid situations where the server or client is sending events or requests that pertain to an invalid object
- Interfaces which define resources that have finite lifetimes will include requests and events through which the client/server can state their intention to no longer send requests or events for that object
	- once both sides agree, the object is destroyed asynchronously
- Message are guaranteed to arrive in order they were sent but only with respect to one sender as the Wayland protocol is asynchronous.

