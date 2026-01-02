# learn-gnome-development

<!-- Bookmark: https://docs.gtk.org/gtk4/getting_started.html#a-trivial-application -->
Bookmark: https://docs.gtk.org/gtk4/getting_started.html#opening-files


## Getting Started
https://docs.gtk.org/gtk4/index.html
https://docs.gtk.org/gobject/concepts.html#conventions
https://docs.gtk.org/gtk4/compiling.html
https://docs.gtk.org/gio/struct.Resource.html
https://docs.gtk.org/gio/class.Application.html

## Tools

```sh
# THE Gnome IDE
flatpak install flathub org.gnome.Builder
flatpak run org.gnome.Builder

# GNOME Workbench: Best for Learning & Prototyping
# This is codepen for GTK. Uses Blueprint or XML, PyGObject, CSS, gjs
flatpak install flathub re.sonny.Workbench
flatpak run re.sonny.Workbench
```

## Main object heirarchy
https://docs.gtk.org/gtk4/class.Widget.html
  https://docs.gtk.org/gtk4/class.Window.html
    https://docs.gtk.org/gtk4/class.ApplicationWindow.html

## Interfaces
https://docs.gtk.org/gtk4/iface.Accessible.html
https://docs.gtk.org/gtk4/iface.Buildable.html
https://docs.gtk.org/gtk4/iface.ConstraintTarget.html
https://docs.gtk.org/gtk4/iface.Native.html
https://docs.gtk.org/gtk4/iface.Root.html
https://docs.gtk.org/gtk4/iface.ShortcutManager.html

## Components
https://docs.gtk.org/gtk4/class.Widget.html
  https://docs.gtk.org/gtk4/class.Application.html
  https://docs.gtk.org/gtk4/class.Box.html
  https://docs.gtk.org/gtk4/class.Grid.html
  https://docs.gtk.org/gtk4/class.Revealer.html
  https://docs.gtk.org/gtk4/class.Stack.html
  https://docs.gtk.org/gtk4/class.Overlay.html
  https://docs.gtk.org/gtk4/class.Paned.html
  https://docs.gtk.org/gtk4/class.Expander.html
  https://docs.gtk.org/gtk4/class.Fixed.html

`.ui` files allow declarative creation of gtk interfaces
See individual component docs for details.

```txt
  <object class="GtkStack" id="stack">
    <child>
      <object class="GtkStackPage">
        <property name="name">page1</property>
        <property name="title">In the beginning…</property>
        <property name="child">
          <object class="GtkLabel">
            <property name="label">It was dark</property>
          </object>
        </property>
      </object>
    </child>
```

## Application structure

An application consists of a number of files:

- **The binary**: This gets installed in /usr/bin.

- **A desktop file**: The desktop file provides important information about the application to the desktop shell, such as its name, icon, D-Bus name, commandline to launch it, etc. It is installed in /usr/share/applications.

- **An icon**: The icon gets installed in /usr/share/icons/hicolor/48x48/apps, where it will be found regardless of the current theme.

- **A settings schema**: If the application uses GSettings, it will install its schema in /usr/share/glib-2.0/schemas, so that tools like dconf-editor can find it.

- **Other resources**: Other files, such as GtkBuilder ui files, are best loaded from resources stored in the application binary itself. This eliminates the need for most of the files that would traditionally be installed in an application-specific location in /usr/share.
