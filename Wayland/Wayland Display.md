## The Wayland Display
- `wl_display` exists on every Wayland connection implicitly and this is how it manages ownership of object between client and server
- registry is used to allocate other objects
- rest of the interface is for housekeeping unless we are building a libwayland replacement
```xml
<interface name="wl_display" version="1">
  <request name="sync">
    <arg name="callback" type="new_id" interface="wl_callback"
       summary="callback object for the sync request"/>
  </request>

  <request name="get_registry">
    <arg name="registry" type="new_id" interface="wl_registry"
      summary="global registry object"/>
  </request>

  <event name="error">
    <arg name="object_id" type="object" summary="object where the error occurred"/>
    <arg name="code" type="uint" summary="error code"/>
    <arg name="message" type="string" summary="error description"/>
  </event>

  <enum name="error">
    <entry name="invalid_object" value="0" />
    <entry name="invalid_method" value="1" />
    <entry name="no_memory" value="2" />
    <entry name="implementation" value="3" />
  </enum>

  <event name="delete_id">
    <arg name="id" type="uint" summary="deleted object ID"/>
  </event>
</interface>
```
## Creating a Display
### Client
- `wl_display` makes managing connection states easy
```c
#include <stdio.h>
#include <wayland-client.h>

int main(int argc, char *argv[])
{
    struct wl_display *display = wl_display_connect(NULL);
    if (!display) {
        fprintf(stderr, "Failed to connect to Wayland display.\n");
        return 1;
    }
    fprintf(stderr, "Connection established!\n");

    wl_display_disconnect(display);
    return 0;
}
```
- `wl_display_connect` is most common way to establish a connection
```c
struct wl_display *wl_display_connect(const char *name);
```
- `name` is the name of the **Wayland Display** usually: *wayland-0*
- `NULL` is usually recommended
- Corresponds to the name of a UNIX socket `$XDG_RUNTIME_DIR`
- When NULL is provided, `libwayland` will
    - if $WAYLAND_DISPLAY is set, attempt to connect to $XDG_RUNTIME_DIR/$WAYLAND_DISPLAY
    - else attempt to connect to $XDG_RUNTIME_DIR/wayland-0
    - else fail
- this lets user set the $WAYLAND_DISPLAY variable and create a custom connection
```c
struct wl_display *wl_display_connect_to_fd(int fd); // Creating a wayland display from a file descriptor
struct wl_display *wl_display_get_fd(struct wl_display *display); // Get the file descriptor of the current display
```
### Server
- Creation of the display and binding to a socket are separate to give time to configure the display before any clients are able to connect to it.
```c
#include <stdio.h>
#include <wayland-server.h>

int main(int argc, char *argv[])
{
    struct wl_display *display = wl_display_create();
    if (!display) {
        fprintf(stderr, "Unable to create Wayland display.\n");
        return 1;
    }

    const char *socket = wl_display_add_socket_auto(display);
    if (!socket) {
        fprintf(stderr, "Unable to add socket to Wayland display.\n");
        return 1;
    }

    fprintf(stderr, "Running Wayland display on %s\n", socket);
    wl_display_run(display);

    wl_display_destroy(display);
    return 0;
}
```
- using `wl_display_add_socket_auto` will allow `libwayland` to decide the name for the display automatically (default *wayland-0* or *wayland-$n*)
- options for configuring the display
```c
int wl_display_add_socket(struct wl_display *display, const char *name);
int wl_display_add_socket_fd(struct wl_display *display, int sock_fd);
```
- after adding sockets, calling `wl_display_run` will run libwayland's internal event loop and block until `wl_display_terminate` is called

## Incorporating Event Loop
### Server
- `wl_display` has a corresponding `wl_event_loop` which can be obtained with `wl_display_get_event_loop`.
- for Wayland compositors, this is likely the only event loop
- add file descriptor using `wl_event_loop_add_fd`
- add timers with `wl_event_loop_add_timer`
- handles signals with `wl_event_loop_add_signal`
- server can now monitor all events the compositor has to respond to
- all events can be processed by calling `wl_display_run` which will process and block until the display terminates (`wl_display_terminate`)
- `wl_display` uses event loop to process clients internally and can choose to monitor the Wayland event loop by making a custom one.
- `wl_event_loop_get_fd` to obtain *poll-able* file descriptor, then call `wl_event_loop_dispatch` to process events when activity occurs on that file descriptor
- `wl_display_flush_clients` also needed to write data to clients

### Client
- does not have its own event loop
- for wayland events, the following loop should be used
```c
while (wl_display_dispatch(display) != -1) {
    // Left blank
}
```
- we can also build our own event loop and obtain the Wayland display's file descriptor with `wl_display_get_fd`
- Unpon `POLLIN` events, call `wl_display_dispatch` to process incoming events
- to flush outgoing requests, call `wl_display_flush`
