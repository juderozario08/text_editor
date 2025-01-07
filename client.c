#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wayland-client.h>

#define DISPLAY_COULD_NOT_CONNECT_NUMBER -1

void reg_glob(void *data, struct wl_registry *reg, uint32_t name,
              const char *interface, uint32_t version) {
  if (!strcmp(interface, wl_compositor_interface.name)) {
  }
}

void reg_glob_remove(void *data, struct wl_registry *reg, uint32_t name) {}

struct wl_registry_listener registry_listener = {
    .global = reg_glob, .global_remove = reg_glob_remove};

int main() {
  struct wl_display *display = wl_display_connect(0);
  if (!display) {
    fprintf(stderr, "Failed to connect to display!\n");
    exit(DISPLAY_COULD_NOT_CONNECT_NUMBER);
  }
  struct wl_registry *registry = wl_display_get_registry(display);
  wl_registry_add_listener(registry, &registry_listener, 0);
  wl_display_roundtrip(display);

  wl_display_disconnect(display);
  return 0;
}
