#include <stdio.h>
#include <dbus-1.0/dbus/dbus.h>

int main(void)
{
  struct DBusError error = {0};
  struct DBusConnection *connection;
  if((connection = dbus_bus_get(DBUS_BUS_SESSION, &error)) == NULL) {
    fprintf(stderr, "[error] dbus_bus_get(): %s (%s)\n", error.message, error.name);
  }
  const char *name = dbus_bus_get_unique_name(connection);
  printf("[connected] got name: %s\n", name);
  return 0;
}
