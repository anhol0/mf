#ifndef CORE_HPP
#define CORE_HPP

#include <cstdlib>
#include <gtkmm.h>
#include <memory>

class File {

}; 

class Application : public Gtk::Window {
    public:
        Application() {
            set_title("mf");
            set_default_size(800, 600); 
            toolbar->set_margin(4);

            // Grid setup
            setup_toolbar();
            setup_grid_content(current_dir);
            setup_factory_signals();
            update_grid(grid);
            setup_grid_signals();
            auto scroll = Gtk::make_managed<Gtk::ScrolledWindow>();
            scroll->set_child(grid);
            scroll->set_vexpand(true);
            // Adding elements to the root node
            root->append(*toolbar);
            root->append(*scroll);
            set_child(*root);
        }
    protected:
        std::string current_dir = std::getenv("HOME");
        // Root nodes of the window
        Gtk::Box *root = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
        Gtk::Box *toolbar = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 4);

        // Toolbar members
        Gtk::Button* btn_back;
        Gtk::Button* btn_forward;
        Gtk::Button* btn_home;
        Gtk::Entry* path_bar;

        // Grid set up
        Gtk::GridView grid;
        std::shared_ptr<Gio::ListStore<Gio::File>> store;
        std::shared_ptr<Gtk::SingleSelection> selection;
        std::shared_ptr<Gtk::SignalListItemFactory> factory = Gtk::SignalListItemFactory::create();
        Gtk::SearchBar bar;
        
        void enum_dir(std::shared_ptr<Gio::ListStore<Gio::File>> store, std::string &dir); 
        void navigate_to(std::string &path); 
        void setup_grid_content(std::string &path);
        void setup_toolbar();
        void setup_factory_signals();
        void setup_grid_signals();
        void update_grid(Gtk::GridView &grid);
};

#endif
