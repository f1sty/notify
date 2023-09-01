#include <dbus-1.0/dbus/dbus.h>
#include <stddef.h>
#include <stdio.h>

int main(void) {
  struct DBusError error = {0};
  struct DBusConnection *connection;
  if ((connection = dbus_bus_get(DBUS_BUS_SESSION, &error)) == NULL) {
    fprintf(stderr, "[error] dbus_bus_get(): %s (%s)\n", error.message,
            error.name);
  }

  const char *name = dbus_bus_get_unique_name(connection);
  printf("[connected] got name: %s\n", name);

  struct DBusMessage *message;
  // Types include DBUS_MESSAGE_TYPE_METHOD_CALL, DBUS_MESSAGE_TYPE_SIGNAL, and
  // so forth.
  /* message = dbus_message_new(DBUS_MESSAGE_TYPE_SIGNAL); */
  message = dbus_message_new_method_call(
      "org.freedesktop.Notifications", "/org/freedesktop/Notifications",
      "org.freedesktop.Notifications", "Notify");
  if (message == NULL) {
    fprintf(stderr, "[error] dbus_message_new(): out of memory\n");
  } else if (message == DBUS_MESSAGE_TYPE_INVALID) {
    fprintf(stderr, "[error] dbus_message_new(): invalid type\n");
  }

  dbus_uint32_t serial;
  char *marshalled_data_p;
  int len_p = 0;
  if (dbus_message_marshal(message, &marshalled_data_p, &len_p) == FALSE) {
    fprintf(stderr, "[error] dbus_message_marshal(): out of memory\n");
  }

  fprintf(stderr, "[info] sending message=");
  for (int i = 0; i < len_p; i++) {
    putc(marshalled_data_p[i], stderr);
  }

  if (dbus_connection_send(connection, message, &serial) == FALSE) {
    fprintf(stderr, "[error] dbus_connection_send(): couldn't send message\n");
  }

  fprintf(stderr, "\n[info] message sent, serial=%u\n", serial);

  dbus_message_unref(message);
  // destination=org.freedesktop.DBus serial=6 path=/org/freedesktop/DBus;
  // interface=org.freedesktop.DBus; member=GetNameOwner
  return 0;
}
