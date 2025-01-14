#include <stdint.h>
#include <stdio.h>
#include <wayland-client-core.h>
#include <wayland-client-protocol.h>

/** wl_surface listener **/
void wl_surface_enter(void *data, struct wl_surface *surface,
                      struct wl_output *output) {}
void wl_surface_leave(void *data, struct wl_surface *surface,
                      struct wl_output *output) {}

void wl_surface_buffer_scale(void *data, struct wl_surface *surface,
                             int32_t factor) {}

void wl_surface_buffer_transform(void *data, struct wl_surface *surface,
                                 uint32_t transform) {}

static const struct wl_surface_listener surface_listener = {
    .enter = wl_surface_enter,
    .leave = wl_surface_leave,
    .preferred_buffer_scale = wl_surface_buffer_scale,
    .preferred_buffer_transform = wl_surface_buffer_transform};

/** wl_registry listener **/
void registry_handle_global(void *data, struct wl_registry *registry,
                            uint32_t name, const char *interface,
                            uint32_t version) {
  printf("interface: '%s', version: %d, name: %d\n", interface, version, name);
}

void registry_handle_global_remove(void *data, struct wl_registry *registry,
                                   uint32_t name) {}

static const struct wl_registry_listener registry_listener = {
    .global = registry_handle_global,
    .global_remove = registry_handle_global_remove};

struct wl_surface *surface;
struct wl_display *display;
struct wl_registry *registry;

int main() {
  display = wl_display_connect(NULL);
  if (!display) {
    fprintf(stderr, "Could not connect to display\n");
    return -1;
  }
  registry = wl_display_get_registry(display);

  wl_registry_add_listener(registry, &registry_listener, NULL);
  wl_surface_add_listener(surface, &surface_listener, NULL);
  wl_display_roundtrip(display);

  // Event handler for the client if needed
  // while (wl_display_dispatch(display) != -1) {
  // }

  wl_display_disconnect(display);
  return 0;
}
