#include "gtkmm/application.h"
#include <cstdlib>
#include <gtkmm.h>
#include "core.hpp"

int main(int argc, char **argv) {
    auto app = Gtk::Application::create("org.gtkmm.mf");
    return app->make_window_and_run<Application>(argc, argv);
}

