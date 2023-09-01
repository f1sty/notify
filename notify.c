#include <dbus-1.0/dbus/dbus.h>
#include <stddef.h>
#include <stdio.h>

int main(void) {
  struct DBusError error;
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
  // destination=org.freedesktop.DBus serial=6 path=/org/freedesktop/DBus;
  // interface=org.freedesktop.DBus; member=GetNameOwner
  message = dbus_message_new_method_call(
      "org.freedesktop.Notifications", "/org/freedesktop/Notifications",
      "org.freedesktop.Notifications", "Notify");
  if (message == NULL) {
    fprintf(stderr, "[error] dbus_message_new(): out of memory\n");
  } else if (message == DBUS_MESSAGE_TYPE_INVALID) {
    fprintf(stderr, "[error] dbus_message_new(): invalid type\n");
  }

  const char *app_name = "notify";
  dbus_uint32_t replace_id = 0;
  const char *icon_name = "";
  const char *summary = "summary";
  const char *body = "body";
  dbus_int32_t timeout = -1;
  DBusMessageIter iter;
  DBusMessageIter array;
  DBusMessageIter dict;
  DBusMessageIter value;
  DBusMessageIter variant;
  DBusMessageIter data;

  /* Format: UINT32 org.freedesktop.Notifications.Notify
   *         (STRING app_name, UINT32 replaces_id, STRING app_icon, STRING
   * summary, STRING body, ARRAY actions, DICT hints, INT32 expire_timeout)
   */
  dbus_message_iter_init_append(message, &iter);
  dbus_message_iter_append_basic(&iter, DBUS_TYPE_STRING, &app_name);
  dbus_message_iter_append_basic(&iter, DBUS_TYPE_UINT32, &replace_id);
  dbus_message_iter_append_basic(&iter, DBUS_TYPE_STRING, &icon_name);
  dbus_message_iter_append_basic(&iter, DBUS_TYPE_STRING, &summary);
  dbus_message_iter_append_basic(&iter, DBUS_TYPE_STRING, &body);
  dbus_message_iter_open_container(&iter, DBUS_TYPE_ARRAY,
                                   DBUS_TYPE_STRING_AS_STRING, &array);
  dbus_message_iter_close_container(&iter, &array);
  dbus_message_iter_open_container(&iter, DBUS_TYPE_ARRAY, "{sv}", &array);
  dbus_message_iter_close_container(&iter, &array);
  dbus_message_iter_append_basic(&iter, DBUS_TYPE_INT32, &timeout);
  dbus_uint32_t serial;
  char *marshalled_data_p;
  int len_p = 0;
  if (dbus_message_marshal(message, &marshalled_data_p, &len_p) == FALSE) {
    fprintf(stderr, "[error] dbus_message_marshal(): out of memory\n");
  }

  fprintf(stderr, "[info] sending message: \n");
  for (int i = 0; i < len_p; i++) {
    putc(marshalled_data_p[i], stderr);
  }

  struct DBusMessage *response;
  if ((response = dbus_connection_send_with_reply_and_block(
           connection, message, -1, &error)) == NULL) {
    fprintf(stderr, "[error] dbus_connection_send(): couldn't send message\n");
  }

  serial = dbus_message_get_serial(response);
  fprintf(stderr, "\n[info] message sent, response serial=%u\n", serial);

  dbus_message_unref(message);
  return 0;
}
