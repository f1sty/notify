CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c2x $(shell pkg-config --cflags dbus-1)
LDFLAGS=$(shell pkg-config --libs dbus-1)

TARGET=notify

all: $(TARGET)

$(TARGET): % : %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<
