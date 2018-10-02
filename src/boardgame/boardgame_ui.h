#ifndef SRC_BOARDGAME_BOARDGAME_UI
#define SRC_BOARDGAME_BOARDGAME_UI

#include "boardgame/pieces_n_fields.h"

#include <memory>
#include <unordered_map>
#include <string>

namespace boardgame
{

class Boardgame_Ui
{
public:
    template <typename T>
    explicit Boardgame_Ui(T mui) : bc(std::make_shared<Boardgame_Ui_Model<T>>(std::move(mui))) {}
    friend void draw(const Boardgame_Ui& ui) { ui.bc->dr(); }
    friend void win(const Boardgame_Ui& ui, const std::string& player) { ui.bc->wi(player); }
    friend void white_drawer_can_hide(const Boardgame_Ui& ui, const bool can_hide) { ui.bc->wdch(can_hide); }
    friend void black_drawer_can_hide(const Boardgame_Ui& ui, const bool can_hide) { ui.bc->bdch(can_hide); }
    friend void white_prison_can_hide(const Boardgame_Ui& ui, const bool can_hide) { ui.bc->wpch(can_hide); }
    friend void black_prison_can_hide(const Boardgame_Ui& ui, const bool can_hide) { ui.bc->bpch(can_hide); }
    friend void lock_piece(const Boardgame_Ui& ui, const boardgame::Piece_Number piece_id) { ui.bc->lp(piece_id); }
    friend void selectable(const Boardgame_Ui& ui, const boardgame::Piece_Number piece_id) { ui.bc->sel(piece_id); }
    friend void removable(const Boardgame_Ui& ui, const boardgame::Piece_Number piece_id) { ui.bc->rem(piece_id); }
    friend void lock_field(const Boardgame_Ui& ui, const boardgame::Field_Number field_id) { ui.bc->lf(field_id); }
    friend void highlight(const Boardgame_Ui& ui, const boardgame::Field_Number field_id) { ui.bc->hl(field_id); }
    friend void occupiable(const Boardgame_Ui& ui, const boardgame::Field_Number field_id, const boardgame::Piece_Number piece_id) { ui.bc->occ(field_id, piece_id); }
    friend void set_field(const Boardgame_Ui& ui, const boardgame::Piece_Number piece_id, const boardgame::Field_Number field_id) { ui.bc->sf(piece_id, field_id); }
    friend void setup_mode_active(const Boardgame_Ui& ui, const bool is_active) { ui.bc->sma(is_active); }
    friend void white_engine_active(const Boardgame_Ui& ui, const bool is_active) { ui.bc->wea(is_active); }
    friend void black_engine_active(const Boardgame_Ui& ui, const bool is_active) { ui.bc->bea(is_active); }
    friend void active_player(const Boardgame_Ui& ui, const std::string& player_id) { ui.bc->ap(player_id); }
private:
    struct Boardgame_Ui_Concept
    {
        virtual ~Boardgame_Ui_Concept() = default;
        virtual void dr() const = 0;
        virtual void wi(const std::string& p) const = 0;
        virtual void wdch(const bool ch) const = 0;
        virtual void bdch(const bool ch) const = 0;
        virtual void wpch(const bool ch) const = 0;
        virtual void bpch(const bool ch) const = 0;
        virtual void lp(const boardgame::Piece_Number pid) const = 0;
        virtual void sel(const boardgame::Piece_Number pid) const = 0;
        virtual void rem(const boardgame::Piece_Number pid) const = 0;
        virtual void lf(const boardgame::Field_Number fid) const = 0;
        virtual void hl(const boardgame::Field_Number fid) const = 0;
        virtual void occ(const boardgame::Field_Number fid, const boardgame::Piece_Number pid) const = 0;
        virtual void sf(const boardgame::Piece_Number pid, const boardgame::Field_Number fid) const = 0;
        virtual void sma(const bool ia) const = 0;
        virtual void wea(const bool ia) const = 0;
        virtual void bea(const bool ia) const = 0;
        virtual void ap(const std::string& pi) const = 0;
    };
    template <typename T>
    struct Boardgame_Ui_Model : Boardgame_Ui_Concept
    {
        Boardgame_Ui_Model(T mui) : Boardgame_Ui_Concept(), bm(std::move(mui)) {}
        void dr() const override { draw(bm); }
        void wi(const std::string& p) const override { win(bm, p); }
        void wdch(const bool ch) const override { white_drawer_can_hide(bm, ch); }
        void bdch(const bool ch) const override { black_drawer_can_hide(bm, ch); }
        void wpch(const bool ch) const override { white_prison_can_hide(bm, ch); }
        void bpch(const bool ch) const override { black_prison_can_hide(bm, ch); }
        void lp(const boardgame::Piece_Number pid) const override { lock_piece(bm, pid); }
        void sel(const boardgame::Piece_Number pid) const override { selectable(bm, pid); }
        void rem(const boardgame::Piece_Number pid) const override { removable(bm, pid); }
        void lf(const boardgame::Field_Number fid) const override { lock_field(bm, fid); }
        void hl(const boardgame::Field_Number fid) const override { highlight(bm, fid); }
        void occ(const boardgame::Field_Number fid, const boardgame::Piece_Number pid) const override { occupiable(bm, fid, pid); }
        void sf(const boardgame::Piece_Number pid, const boardgame::Field_Number fid) const override { set_field(bm, pid, fid); }
        void sma(const bool ia) const override { setup_mode_active(bm, ia); }
        void wea(const bool ia) const override { white_engine_active(bm, ia); }
        void bea(const bool ia) const override { black_engine_active(bm, ia); }
        void ap(const std::string& pi) const override { active_player(bm, pi); }
        T bm;
    };
    std::shared_ptr<const Boardgame_Ui_Concept> bc;
};

}

#endif // SRC_BOARDGAME_BOARDGAME_UI
