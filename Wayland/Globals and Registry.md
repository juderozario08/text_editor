### Purpose
- know what interface the object ID represents when decoding messages
- negotiate available objects, creation of new objects, assigning IDs
- these are solved when binding an object ID in wayland by agreeing on an interface and stashing a mapping of object IDs to interfaces in our local state
- `globals` provide information about
    - provides information and functionality
    - used to broker additional obojects to fulfill various purposes such as creating application window
    - `globals` have their own object IDs and interfaces
- `wl_display::get_registry` request can be used to bind an object ID to the `wl_registry` interface, which is the next one found in `Wayland.xml`
- After creating registry object, server emits `global` event for each global on the server which can be `bound` to the globals

## Binding to Globals
- `Binding` is the process of taking an object and assingning it an ID.
- Once client binds to registry, the server emits global event several times to advertise which interfaces it supports.
- Each global is assigned a unique `name`, as an unsigned integer
- `Interface` string maps to the name of the interface found in the protocol
- `Version` number is defined here
- to bind of these interfaces, we use the bind request
