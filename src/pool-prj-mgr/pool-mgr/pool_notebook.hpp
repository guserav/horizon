#pragma once
#include <gtkmm.h>
#include <memory>
#include "util/uuid.hpp"
#include "pool/part.hpp"
#include "pool/unit.hpp"
#include "pool/entity.hpp"
#include "pool/symbol.hpp"
#include "pool/package.hpp"
#include "pool/padstack.hpp"

#include "pool/pool.hpp"
#include "pool/pool_parametric.hpp"
#include "util/editor_process.hpp"
#include <zmq.hpp>
#include "util/win32_undef.hpp"
#include "util/paned_state_store.hpp"

namespace horizon {
class PoolNotebook : public Gtk::Notebook {
    friend class PoolRemoteBox;
    friend class PoolGitBox;

public:
    PoolNotebook(const std::string &bp, class PoolProjectManagerAppWindow &aw);
    void populate();
    bool get_close_prohibited() const;
    void prepare_close();
    void pool_update(const std::vector<std::string> &filenames = {});
    bool get_needs_save() const;
    void save();
    void go_to(ObjectType type, const UUID &uu);
    const UUID &get_pool_uuid() const;
    ~PoolNotebook();
    PoolProjectManagerAppWindow &appwin;

    typedef sigc::signal<void> type_signal_saved;
    type_signal_saved signal_saved()
    {
        return s_signal_saved;
    }

private:
    const std::string base_path;
    Pool pool;
    PoolParametric pool_parametric;
    std::map<ObjectType, class PoolBrowser *> browsers;
    std::map<std::string, class PoolBrowserParametric *> browsers_parametric;
    class PartWizard *part_wizard = nullptr;
    class KiCadSymbolImportWizard *kicad_symbol_import_wizard = nullptr;
    class DuplicateWindow *duplicate_window = nullptr;
    class ImportKiCadPackageWindow *import_kicad_package_window = nullptr;
    bool closing = false;

    void reload();
    std::function<void()> pool_update_done_cb = nullptr;

    void show_duplicate_window(ObjectType ty, const UUID &uu);

    void construct_units();
    void handle_create_unit();
    void handle_edit_unit(const UUID &uu);
    void handle_create_symbol_for_unit(const UUID &uu);
    void handle_create_entity_for_unit(const UUID &uu);
    void handle_duplicate_unit(const UUID &uu);

    void construct_symbols();
    void handle_edit_symbol(const UUID &uu);
    void handle_create_symbol();
    void handle_duplicate_symbol(const UUID &uu);

    void construct_entities();
    void handle_edit_entity(const UUID &uu);
    void handle_create_entity();
    void handle_duplicate_entity(const UUID &uu);

    void construct_padstacks();
    void handle_edit_padstack(const UUID &uu);
    void handle_create_padstack();
    void handle_duplicate_padstack(const UUID &uu);

    void construct_packages();
    void handle_edit_package(const UUID &uu);
    void handle_create_package();
    void handle_create_padstack_for_package(const UUID &uu);
    void handle_duplicate_package(const UUID &uu);
    void handle_import_kicad_package();

    void handle_part_wizard();
    void handle_kicad_symbol_import_wizard();

    void construct_parts();
    void handle_edit_part(const UUID &uu);
    void handle_create_part();
    void handle_create_part_from_part(const UUID &uu);
    void handle_duplicate_part(const UUID &uu);

    void construct_frames();
    void handle_edit_frame(const UUID &uu);
    void handle_create_frame();
    void handle_duplicate_frame(const UUID &uu);

    void construct_decals();
    void handle_edit_decal(const UUID &uu);
    void handle_create_decal();
    void handle_duplicate_decal(const UUID &uu);

    void construct_parametric();

    Gtk::Button *add_action_button(const std::string &label, Gtk::Box *bbox, sigc::slot0<void>);
    Gtk::Button *add_action_button(const std::string &label, Gtk::Box *bbox, class PoolBrowser *br,
                                   sigc::slot1<void, UUID>);
    Gtk::Button *add_merge_button(Gtk::Box *bbox, class PoolBrowser *br, std::function<void(UUID)> cb = nullptr);
    void add_preview_stack_switcher(Gtk::Box *bbox, Gtk::Stack *stack);

    void handle_delete(ObjectType ty, const UUID &uu);
    void handle_move_rename(ObjectType ty, const UUID &uu);
    void handle_copy_path(ObjectType ty, const UUID &uu);
    void add_context_menu(class PoolBrowser *br);

    Pool::ItemPoolInfo get_pool_uuids(ObjectType ty, const UUID &uu);
    void handle_edit_item(ObjectType ty, const UUID &uu);
    void handle_duplicate_item(ObjectType ty, const UUID &uu);

    void install_search_once(Gtk::Widget *page, PoolBrowser *browser);
    bool widget_is_visible(Gtk::Widget *widget);

    void create_paned_state_store(Gtk::Paned *paned, const std::string &prefix);
    std::vector<std::unique_ptr<PanedStateStore>> paned_state_stores;

    std::string remote_repo;
    class PoolRemoteBox *remote_box = nullptr;
    class PoolSettingsBox *settings_box = nullptr;
    class PoolGitBox *git_box = nullptr;
    class PoolCacheBox *cache_box = nullptr;

    UUID pool_uuid;

    void pool_updated();
    bool pool_busy = false;

    type_signal_saved s_signal_saved;
};
} // namespace horizon
