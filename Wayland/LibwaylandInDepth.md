## wayland-util primitives
- defines number of structs, utility functions and macros (client / server) that establish a handful of primitives
    - structures for un/marshalling
    - wl_list (linked list implementation)
    - wl_array (array implementation)
    - utilities for conversion between Wayland scalars and C types
    - debug logging

## wayland-scanner
- used to generate C headers and glue code from wayland protocol XML files
- used in the build process to pre-generate headers and glue code for the core protocol
- headers become `wayland-client-protocol.h` and `wayland-server-protocol.h` usually doesnt include the word *protocol*.
- Usage: wayland-scanner -h
```sh
# Generating client header
wayland-scanner client-header < protocol.xml > protocol.h
# generate server header
wayland-scanner server-header < protocol.xml > protocol.h
# generate the glue code
wayland-scanner private-code < protocol.xml > protocol.c
```

## Proxies & Resources
- `object` is known to both client and server that has some state changes to which are negotiated over the wire
- (client side) `libwayland` refers to these as `wl_proxy` interface
- it provides functions which are inderectly used by the client to marshall requests into the wire format
- (server side) `objects` are abstracted through `wl_resource`
- server has to track which object belongs to which client
- each `wl_resource` is owned by 1 client
- interface is similar to `wl_proxy`

## Interfaces and Listeners
- Running an XML file through `wayland-scanner` it generates *interfaces and listeners* and glue code between them
- Client listens to `events` and sends `requests` and server listens for `requests` and sends `events`
```c
struct wl_surface_listener {
    /** surface enters an output **/
    void (*enter) (void *data, struct wl_surface *wl_surface, struct wl_output *output);
    /** surface leaves an output **/
    void (*leave) (void *data, struct wl_surface *wl_surface, struct wl_output *output);
}

/** This is how to setup the code for the surface enter/leave **/
static void wl_surface_enter(void *data,
        struct wl_surface *wl_surface, struct wl_output *output) {}
static void wl_surface_leave(void *data,
        struct wl_surface *wl_surface, struct wl_output *output) {}
static const struct wl_surface_listener surface_listener = {
    .enter = wl_surface_enter,
    .leave = wl_surface_leave,
};
struct wl_surface *surface;
wl_surface_add_listener(surface, &surface_listener, NULL);
```
- server side for this is identical but just opposite. Produces glue code for events and listeners for requests
- when `libwayland` receives a message, it looks up the `object ID` and the `interface` to decode the message then looks at the listeners and invokes the functions accordingly
