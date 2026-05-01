#include "core.hpp"
#include "giomm/appinfo.h"
#include "gtkmm/entry.h"
#include "gtkmm/object.h"
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>
#include <ostream>

void Application::setup_grid_content(std::string &path) { 
    store = Gio::ListStore<Gio::File>::create();

    // Creating the list of the files 
    enum_dir(store, path); 
    
    factory = Gtk::SignalListItemFactory::create();
}

void Application::setup_toolbar() {
    btn_back = Gtk::make_managed<Gtk::Button>();
    btn_back->set_icon_name("go-previous-symbolic");

    btn_forward = Gtk::make_managed<Gtk::Button>();
    btn_forward->set_icon_name("go-next-symbolic");

    btn_home = Gtk::make_managed<Gtk::Button>();
    btn_home->set_icon_name("go-home-symbolic");

    path_bar = Gtk::make_managed<Gtk::Entry>();
    path_bar->set_hexpand(true);
    path_bar->set_text(getenv("HOME"));

    toolbar->append(*btn_back);
    toolbar->append(*btn_forward);
    toolbar->append(*btn_home);
    toolbar->append(*path_bar);
}

void Application::setup_factory_signals() {
    factory->signal_setup().connect([](const Glib::RefPtr<Gtk::ListItem> &item) {
        // Creating layout of properties of the element
        auto box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
        auto image  = Gtk::make_managed<Gtk::Image>();
        auto label = Gtk::make_managed<Gtk::Label>();
        
        // Adding everything to the 
        box->append(*image);
        box->append(*label);
        item->set_child(*box);
    });

    factory->signal_bind().connect([](const Glib::RefPtr<Gtk::ListItem> &item) {
        auto file = std::dynamic_pointer_cast<Gio::File>(item->get_item()); 
        if(!file) return;
        auto box = dynamic_cast<Gtk::Box*>(item->get_child());
        if(!box) return;
        auto label = dynamic_cast<Gtk::Label*>(box->get_last_child());
        if(!label) return;
        label->set_text(file->get_basename());
        auto image = dynamic_cast<Gtk::Image*>(box->get_first_child());
        if(image) {
            auto f_info = file->query_info("standard::icon", Gio::FileQueryInfoFlags::NONE);
            if(f_info) {
                image->set(f_info->get_icon());
            }
        }
    });
}

void Application::update_grid(Gtk::GridView &grid) {
    selection = Gtk::SingleSelection::create(store);
    grid.set_min_columns(5);
    grid = Gtk::GridView(selection, factory);
}

void Application::setup_grid_signals() {
    grid.signal_activate().connect([&](guint position) {
        (void)position;
        auto file = std::dynamic_pointer_cast<Gio::File>(selection->get_selected_item());
        if(!file) return;
        auto path = file->get_path();
        std::cout << path << std::endl;
        if(std::filesystem::is_directory(path)) {
            navigate_to(path);
        }
        else {
            Gio::AppInfo::launch_default_for_uri(file->get_uri());
        }
    }); 
}

// Misc functions 
void Application::enum_dir(std::shared_ptr<Gio::ListStore<Gio::File>> store, std::string &dir) {
    auto current_dir_f = Gio::File::create_for_path(dir);
    if(current_dir_f->has_parent()) {
        auto parent = current_dir_f->get_parent();
        store->append(parent);
    }
    for(const auto& file: std::filesystem::directory_iterator(dir)) {
        store->append(Gio::File::create_for_path(file.path()));                    
    }
}
void Application::navigate_to(std::string &path) {
    current_dir = path;
    store->remove_all();
    enum_dir(store, path);
}
