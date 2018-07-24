#ifndef SRC_BOARDGAMES_MUEHLEUI
#define SRC_BOARDGAMES_MUEHLEUI

#include "boardgame/pieces_n_fields.h"

#include <memory>
#include <unordered_map>
#include <string>

class Muehle_Ui_Type
{
public:
    template <typename T>
    explicit Muehle_Ui_Type(T mui) : mC(std::make_shared<Muehle_Ui_Model<T>>(std::move(mui))) {}
    friend void draw(const Muehle_Ui_Type& mui) { mui.mC->dr(); }
    friend void win(const Muehle_Ui_Type& mui, const std::string& player) { mui.mC->wi(player); }
    friend void white_drawer_can_hide(const Muehle_Ui_Type& mui, const bool can_hide) { mui.mC->wdch(can_hide); }
    friend void black_drawer_can_hide(const Muehle_Ui_Type& mui, const bool can_hide) { mui.mC->bdch(can_hide); }
    friend void white_prison_can_hide(const Muehle_Ui_Type& mui, const bool can_hide) { mui.mC->wpch(can_hide); }
    friend void black_prison_can_hide(const Muehle_Ui_Type& mui, const bool can_hide) { mui.mC->bpch(can_hide); }
    friend void lock_pieces(const Muehle_Ui_Type& mui) { mui.mC->lp(); }
    friend void selectable(const Muehle_Ui_Type& mui, const boardgame::PieceNumber piece_id) { mui.mC->sel(piece_id); }
    friend void removable(const Muehle_Ui_Type& mui, const boardgame::PieceNumber piece_id) { mui.mC->rem(piece_id); }
    friend void lock_field(const Muehle_Ui_Type& mui, const boardgame::FieldNumber field_id) { mui.mC->lf(field_id); }
    friend void highlight(const Muehle_Ui_Type& mui, const boardgame::FieldNumber field_id) { mui.mC->hl(field_id); }
    friend void occupiable(const Muehle_Ui_Type& mui, const boardgame::FieldNumber field_id, const boardgame::PieceNumber piece_id) { mui.mC->occ(field_id, piece_id); }
    friend void set_field(const Muehle_Ui_Type& mui, const boardgame::PieceNumber piece_id, const boardgame::FieldNumber field_id) { mui.mC->sf(piece_id, field_id); }
private:
    struct Muehle_Ui_Concept
    {
        virtual ~Muehle_Ui_Concept() = default;
        virtual void dr() const = 0;
        virtual void wi(const std::string& p) const = 0;
        virtual void wdch(const bool ch) const = 0;
        virtual void bdch(const bool ch) const = 0;
        virtual void wpch(const bool ch) const = 0;
        virtual void bpch(const bool ch) const = 0;
        virtual void lp() const = 0;
        virtual void sel(const boardgame::PieceNumber pid) const = 0;
        virtual void rem(const boardgame::PieceNumber pid) const = 0;
        virtual void lf(const boardgame::FieldNumber fid) const = 0;
        virtual void hl(const boardgame::FieldNumber fid) const = 0;
        virtual void occ(const boardgame::FieldNumber fid, const boardgame::PieceNumber pid) const = 0;
        virtual void sf(const boardgame::PieceNumber pid, const boardgame::FieldNumber fid) const = 0;
    };
    template <typename T>
    struct Muehle_Ui_Model : Muehle_Ui_Concept
    {
        Muehle_Ui_Model(T mui) : Muehle_Ui_Concept(), mM(std::move(mui)) {}
        void dr() const override { draw(mM); }
        void wi(const std::string& p) const override { win(mM, p); }
        void wdch(const bool ch) const override { white_drawer_can_hide(mM, ch); }
        void bdch(const bool ch) const override { black_drawer_can_hide(mM, ch); }
        void wpch(const bool ch) const override { white_prison_can_hide(mM, ch); }
        void bpch(const bool ch) const override { black_prison_can_hide(mM, ch); }
        void lp() const override { lock_pieces(mM); }
        void sel(const boardgame::PieceNumber pid) const override { selectable(mM, pid); }
        void rem(const boardgame::PieceNumber pid) const override { removable(mM, pid); }
        void lf(const boardgame::FieldNumber fid) const override { lock_field(mM, fid); }
        void hl(const boardgame::FieldNumber fid) const override { highlight(mM, fid); }
        void occ(const boardgame::FieldNumber fid, const boardgame::PieceNumber pid) const override { occupiable(mM, fid, pid); }
        void sf(const boardgame::PieceNumber pid, const boardgame::FieldNumber fid) const override { set_field(mM, pid, fid); }
        T mM;
    };
    std::shared_ptr<const Muehle_Ui_Concept> mC;
};

#endif // SRC_BOARDGAMES_MUEHLEUI
