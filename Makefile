# Usage: make NUM=99
CC = gcc
CFLAGS = `pkg-config --cflags gtk4`
LIBS = `pkg-config --libs gtk4`

all:
	@echo "Run examples like this: make run-example NUM=0"

# This target cleans the environment before running
# g_unsetenv inside your C code is better, but this works for testing
run-example:
	@if [ -z "$(NUM)" ]; then echo "Error: NUM is missing"; exit 1; fi
	$(CC) $(CFLAGS) -o examples/example-$(NUM) examples/example-$(NUM).c $(LIBS)
	./examples/example-$(NUM)

setup: setup-c setup-python setup-builder setup-workbench

setup-c:
	sudo apt install \
		libgtk-4-dev \
		build-essential \
		libgtk2.0-dev

setup-python:
	sudo apt update
	sudo apt install \
		libgirepository-2.0-dev \
		python3-gi \
		python3-gi-cairo \
		gir1.2-gtk-4.0

setup-builder:
	flatpak install flathub org.gnome.Builder \
	flatpak run org.gnome.Builder

setup-workbench:
	flatpak install flathub re.sonny.Workbench
	flatpak run re.sonny.Workbench
