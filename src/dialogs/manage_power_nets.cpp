#include "manage_power_nets.hpp"
#include "util/gtk_util.hpp"
#include "util/util.hpp"
#include "block/block.hpp"
#include "blocks/iblock_provider.hpp"

namespace horizon {

class PowerNetEditor : public Gtk::Box {
public:
    PowerNetEditor(Net &n, IBlockProvider &bl) : Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 4), net(n), blocks(bl)
    {
        auto entry = Gtk::manage(new Gtk::Entry());
        entry->set_text(net.name);
        entry->signal_changed().connect([this, entry] { net.name = entry->get_text(); });
        pack_start(*entry, true, true, 0);

        auto style_la = Gtk::manage(new Gtk::Label("Style"));
        style_la->set_margin_start(4);
        style_la->get_style_context()->add_class("dim-label");
        pack_start(*style_la, false, false, 0);

        auto combo = Gtk::manage(new Gtk::ComboBoxText);

        static const std::map<Net::PowerSymbolStyle, std::string> lut = {
                {Net::PowerSymbolStyle::GND, "Signal Ground"},
                {Net::PowerSymbolStyle::EARTH, "Earth Ground"},
                {Net::PowerSymbolStyle::DOT, "Dot"},
                {Net::PowerSymbolStyle::ANTENNA, "Antenna"},
        };
        bind_widget(combo, lut, net.power_symbol_style);

        pack_start(*combo, false, false, 0);

        auto name_visible_cb = Gtk::manage(new Gtk::CheckButton("Show net name"));
        bind_widget(name_visible_cb, net.power_symbol_name_visible);
        pack_start(*name_visible_cb, false, false, 0);

        auto delete_button = Gtk::manage(new Gtk::Button());
        delete_button->set_margin_start(4);
        delete_button->set_image_from_icon_name("list-remove-symbolic", Gtk::ICON_SIZE_BUTTON);
        auto all_blocks = blocks.get_blocks();
        delete_button->set_sensitive(std::all_of(all_blocks.begin(), all_blocks.end(),
                                                 [this](auto &x) { return x.second->can_delete_power_net(net.uuid); }));
        pack_start(*delete_button, false, false, 0);
        delete_button->signal_clicked().connect([this] {
            blocks.get_top_block().nets.erase(net.uuid);
            delete this->get_parent();
        });

        set_margin_start(8);
        set_margin_end(8);
        set_margin_top(4);
        set_margin_bottom(4);
        show_all();
    }

private:
    Net &net;
    IBlockProvider &blocks;
};

ManagePowerNetsDialog::ManagePowerNetsDialog(Gtk::Window *parent, IBlockProvider &bl)
    : Gtk::Dialog("Manage power nets", *parent,
                  Gtk::DialogFlags::DIALOG_MODAL | Gtk::DialogFlags::DIALOG_USE_HEADER_BAR),
      blocks(bl)
{
    add_button("Cancel", Gtk::ResponseType::RESPONSE_CANCEL);
    add_button("OK", Gtk::ResponseType::RESPONSE_OK);
    set_default_response(Gtk::ResponseType::RESPONSE_OK);
    set_default_size(400, 300);


    auto box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
    auto add_button = Gtk::manage(new Gtk::Button("Add Power net"));
    add_button->signal_clicked().connect(sigc::mem_fun(*this, &ManagePowerNetsDialog::handle_add_power_net));
    add_button->set_halign(Gtk::ALIGN_START);
    add_button->set_margin_bottom(8);
    add_button->set_margin_top(8);
    add_button->set_margin_start(8);
    add_button->set_margin_end(8);

    box->pack_start(*add_button, false, false, 0);

    {
        auto sep = Gtk::manage(new Gtk::Separator(Gtk::ORIENTATION_HORIZONTAL));
        box->pack_start(*sep, false, false, 0);
    }


    auto sc = Gtk::manage(new Gtk::ScrolledWindow());
    sc->set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
    listbox = Gtk::manage(new Gtk::ListBox());
    listbox->set_selection_mode(Gtk::SELECTION_NONE);
    listbox->set_header_func(sigc::ptr_fun(header_func_separator));
    sc->add(*listbox);
    box->pack_start(*sc, true, true, 0);

    std::vector<Net *> nets_sorted;

    for (auto &it : blocks.get_top_block().nets) {
        if (it.second.is_power) {
            nets_sorted.push_back(&it.second);
        }
    }
    std::sort(nets_sorted.begin(), nets_sorted.end(),
              [](const auto a, const auto b) { return strcmp_natural(a->name, b->name) < 0; });

    for (auto net : nets_sorted) {
        auto ne = Gtk::manage(new PowerNetEditor(*net, blocks));
        listbox->add(*ne);
    }

    get_content_area()->pack_start(*box, true, true, 0);
    get_content_area()->set_border_width(0);
    show_all();
}

void ManagePowerNetsDialog::handle_add_power_net()
{
    auto net = blocks.get_top_block().insert_net();
    net->name = "GND";
    net->is_power = true;

    auto ne = Gtk::manage(new PowerNetEditor(*net, blocks));
    listbox->add(*ne);
}
} // namespace horizon
