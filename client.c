#include <stdio.h>
#include <wayland-client-core.h>
#include <wayland-client-protocol.h>

void wl_surface_enter(void *data, struct wl_surface *wl_surface,
                      struct wl_output *output) {}
void wl_surface_leave(void *data, struct wl_surface *wl_surface,
                      struct wl_output *output) {}

void wl_surface_buffer_scale(void *data, struct wl_surface *wl_surface,
                             int32_t factor) {}

void wl_surface_buffer_transform(void *data, struct wl_surface *wl_surface,
                                 uint32_t transform) {}

struct wl_surface *surface;
struct wl_display *display;
struct wl_registry *registry;

int main() {
  display = wl_display_connect(NULL);
  if (!display) {
    fprintf(stderr, "Could not connect to display\n");
    return -1;
  }
  int fd = wl_display_get_fd(display);
  registry = wl_display_get_registry(display);
  fprintf(stderr, "Connected to display. fd: %d\n", fd);
  while (wl_display_dispatch(display) != -1) {
  }
  wl_display_disconnect(display);
  static const struct wl_surface_listener surface_listener = {
      .enter = wl_surface_enter,
      .leave = wl_surface_leave,
      .preferred_buffer_scale = wl_surface_buffer_scale,
      .preferred_buffer_transform = wl_surface_buffer_transform};
  return 0;
}
