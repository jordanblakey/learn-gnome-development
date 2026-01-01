import os
import sys
import gi

# Force Cairo to bypass GPU driver glitches
os.environ["GSK_RENDERER"] = "cairo"

gi.require_version('Gtk', '4.0')
from gi.repository import Gtk, Gdk, Gio, GLib

class AppWindow(Gtk.ApplicationWindow):
    def __init__(self, app):
        super().__init__(application=app)
        self.set_title("invisicalc")
        self.set_default_size(520, 400)
        
        # UI Setup
        main_box = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=0)
        self.set_child(main_box)

        self.display = Gtk.Entry()
        self.display.set_can_focus(False) 
        self.display.set_text("READY")
        main_box.append(self.display)

        # KEY CONTROLLER FIX: Use Capture Phase
        key_controller = Gtk.EventControllerKey()
        key_controller.set_propagation_phase(Gtk.PropagationPhase.CAPTURE)
        key_controller.connect("key-pressed", self.on_key_pressed)
        key_controller.connect("key-released", self.on_key_released)
        self.add_controller(key_controller)
        self.connect("close-request", self.on_close)

    def on_close(self, *args):
        self.get_application().quit()

    def on_key_pressed(self, controller, keyval, keycode, state):
        print(f"Pressed: {keycode}")
        self.display.set_text("1")
        return True

    def on_key_released(self, controller, keyval, keycode, state):
        print(f"Released: {keycode}")
        self.display.set_text("2")
        return True

class App(Gtk.Application):
    def __init__(self):
        # Setting this to None is the 'Reset' button for this behavior
        super().__init__(application_id=None,
                         flags=Gio.ApplicationFlags.FLAGS_NONE)

    def do_activate(self):
        win = self.props.active_window
        if not win:
            win = AppWindow(self)
        win.present() # This tells the OS to give the app focus

if __name__ == "__main__":
    # KILL PREVIOUS GHOSTS BEFORE RUNNING
    # In Linux terminal: pkill -f your_script_name.py
    app = App()
    app.run(sys.argv)