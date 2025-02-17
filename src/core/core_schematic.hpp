#pragma once
#include "blocks/blocks_schematic.hpp"
#include "core.hpp"
#include "document/idocument_schematic.hpp"
#include "document/idocument_block_symbol.hpp"
#include "schematic/iinstancce_mapping_provider.hpp"
#include <memory>
#include <optional>

namespace horizon {
class CoreSchematic : public Core,
                      public virtual IDocumentSchematic,
                      public virtual IDocumentBlockSymbol,
                      public IInstanceMappingProvider {
public:
    CoreSchematic(const std::string &blocks_filename, const std::string &pictures_dir, IPool &pool,
                  IPool &pool_caching);
    bool has_object_type(ObjectType ty) const override;

    Schematic *get_current_schematic() override;
    const Schematic *get_current_schematic() const;
    Schematic *get_top_schematic() override;
    const Schematic *get_top_schematic() const;
    Sheet *get_sheet() override;
    const Sheet *get_sheet() const;

    BlockSymbol &get_block_symbol() override;

    Junction *get_junction(const UUID &uu) override;
    Junction *insert_junction(const UUID &uu) override;
    void delete_junction(const UUID &uu) override;

    class Block *get_top_block() override;
    class Block *get_current_block() override;
    class LayerProvider &get_layer_provider() override;

    bool set_property(ObjectType type, const UUID &uu, ObjectProperty::ID property,
                      const class PropertyValue &value) override;
    bool get_property(ObjectType type, const UUID &uu, ObjectProperty::ID property,
                      class PropertyValue &value) override;
    bool get_property_meta(ObjectType type, const UUID &uu, ObjectProperty::ID property,
                           class PropertyMeta &meta) override;

    std::string get_display_name(ObjectType type, const UUID &uu) override;
    std::string get_display_name(ObjectType type, const UUID &uu, const UUID &sheet) override;

    class Rules *get_rules() override;

    BOMExportSettings *get_bom_export_settings()
    {
        return &bom_export_settings;
    }

    PDFExportSettings *get_pdf_export_settings()
    {
        return &pdf_export_settings;
    }

    void add_sheet();
    void delete_sheet(const UUID &uu);

    void set_sheet(const UUID &uu);
    const Sheet &get_canvas_data();
    const BlockSymbol &get_canvas_data_block_symbol();
    std::pair<Coordi, Coordi> get_bbox() override;

    void set_block(const UUID &uu);
    const UUID &get_current_block_uuid() const;
    const UUID &get_top_block_uuid() const;

    void set_block_symbol_mode();
    bool get_block_symbol_mode() const override;

    const BlocksSchematic &get_blocks() const;
    BlocksSchematic &get_blocks() override;
    bool current_block_is_top() const override;

    void set_instance_path(const UUIDVec &path);
    const UUIDVec &get_instance_path() const override;
    bool in_hierarchy() const override;

    const BlockInstanceMapping *get_block_instance_mapping() const override;
    BlockInstanceMapping *get_block_instance_mapping();

    unsigned int get_sheet_index(const class UUID &sheet) const override;
    unsigned int get_sheet_total() const override;
    unsigned int get_sheet_index_for_path(const class UUID &sheet, const UUIDVec &path) const;

    Schematic &get_schematic_for_instance_path(const UUIDVec &path) override;

    const std::string &get_filename() const override;

    ObjectType get_object_type() const override
    {
        return ObjectType::SCHEMATIC;
    }

    const FileVersion &get_version() const override
    {
        return get_top_schematic()->version;
    }

    void reload_pool() override;

private:
    std::map<UUID, Line> *get_line_map() override;
    std::map<UUID, Arc> *get_arc_map() override;
    std::map<UUID, Text> *get_text_map() override;
    std::map<UUID, Picture> *get_picture_map() override;

    std::optional<BlocksSchematic> blocks;

    SchematicRules rules;

    BOMExportSettings bom_export_settings;
    PDFExportSettings pdf_export_settings;

    UUID sheet_uuid;
    UUID block_uuid;
    UUIDVec instance_path;

    std::string m_blocks_filename;
    std::string m_pictures_dir;

    class HistoryItem : public Core::HistoryItem {
    public:
        HistoryItem(const BlocksSchematic &bl, const std::string &cm) : Core::HistoryItem(cm), blocks(bl)
        {
        }
        BlocksSchematic blocks;
    };
    void rebuild_internal(bool from_undo, const std::string &comment) override;
    void history_push(const std::string &comment) override;
    void history_load(unsigned int i) override;
    void save(const std::string &suffix) override;
    void delete_autosave() override;
    void fix_current_block();
};
} // namespace horizon
