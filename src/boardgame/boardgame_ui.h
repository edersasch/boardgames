#ifndef SRC_BOARDGAME_BOARDGAME_UI
#define SRC_BOARDGAME_BOARDGAME_UI

#include "boardgame/pieces_n_fields.h"

#include <memory>
#include <string>

/** \brief Game and UI independent code
 */
namespace boardgame
{

/** \brief Interface for commands from the game to its user interface
 *
 * The called UI instance can use a narrow contract on all method parameters,
 * because the values are not user generated.
 *
 * No return value is necessary, because the UI shall execute the command
 * without errors.
 *
 * There are several categories of commands:
 *
 * * The game ended:
 *   * \ref draw
 *   * \ref win
 * * UI elements should be shown or hidden:
 *   * \ref drawer_can_hide
 *   * \ref prison_can_hide
 * * Piece control:
 *   * \ref lock_piece
 *   * \ref selectable
 *   * \ref removable
 * * Field control:
 *   * \ref lock_field
 *   * \ref highlight
 *   * \ref occupiable
 * * Visual feedback:
 *   * \ref setup_mode_active
 *   * \ref engine_active
 *   * \ref active_player
 *   * \ref need_confirm
 *
 * No inheritance required according to <https://sean-parent.stlab.cc/papers-and-presentations/#better-code-runtime-polymorphism>
 */

class Boardgame_Ui
{
public:
    template <typename T>
    explicit Boardgame_Ui(T mui) : bc(std::make_shared<Boardgame_Ui_Model<T>>(std::move(mui))) {}

    /** \brief The game ended and has no winner
     *
     * \param ui User interface instance
     */
    friend void draw(const Boardgame_Ui& ui) { ui.bc->dr(); }

    /** \brief The game ended and has a winner
     *
     * \param ui User interface instance
     * \param player_id Identification string of the winning player
     */
    friend void win(const Boardgame_Ui& ui, const std::string& player_id) { ui.bc->wi(player_id); }

    /** \brief The drawer needs not be shown
     *
     * \param ui User interface instance
     * \param player_id Identification string of the player owning the drawer
     * \param can_hide If `true`, the corresponding UI element may be hidden. If `false`, the UI shall show it.
     */
    friend void drawer_can_hide(const Boardgame_Ui& ui, const std::string& player_id, const bool can_hide) { ui.bc->dch(player_id, can_hide); }

    /** \brief The prison needs not be shown
     *
     * \param ui User interface instance
     * \param player_id Identification string of the player owning the prison
     * \param can_hide If `true`, the corresponding UI element may be hidden. If `false`, the UI shall show it.
     */
    friend void prison_can_hide(const Boardgame_Ui& ui, const std::string& player_id, const bool can_hide) { ui.bc->pch(player_id, can_hide); }

    /** \brief The piece must be visible, but the no interaction or manipulation is possible
     *
     * \param ui User interface instance
     * \param piece_id Identification number of the required piece
     */
    friend void lock_piece(const Boardgame_Ui& ui, const boardgame::Piece_Number piece_id) { ui.bc->lp(piece_id); }

    /** \brief The piece can be selected and moved by the player
     *
     * \param ui User interface instance
     * \param piece_id Identification number of the required piece
     */
    friend void selectable(const Boardgame_Ui& ui, const boardgame::Piece_Number piece_id) { ui.bc->sel(piece_id); }

    /** \brief The piece can be removed by the player
     *
     * \param ui User interface instance
     * \param piece_id Identification number of the required piece
     */
    friend void removable(const Boardgame_Ui& ui, const boardgame::Piece_Number piece_id) { ui.bc->rem(piece_id); }

    /** \brief The field must be visible, but no interaction or manipulation is possible
     *
     * \param ui User interface instance
     * \param field_id Identification number of the required field
     */
    friend void lock_field(const Boardgame_Ui& ui, const boardgame::Field_Number field_id) { ui.bc->lf(field_id); }

    /** \brief The field must differ visually from \ref lock_field "locked fields"
     *
     * \param ui User interface instance
     * \param field_id Identification number of the required field
     */
    friend void highlight(const Boardgame_Ui& ui, const boardgame::Field_Number field_id) { ui.bc->hl(field_id); }

    /** \brief The field must be able to receive a piece
     *
     * \param ui User interface instance
     * \param field_id Identification number of the required field
     * \param piece_id Identification number of the potentially placeable piece
     *
     * The piece_id can be used to show a transparent preview of the piece.
     */
    friend void occupiable(const Boardgame_Ui& ui, const boardgame::Field_Number field_id, const boardgame::Piece_Number piece_id) { ui.bc->occ(field_id, piece_id); }

    /** \brief The piece gets placed onto a field
     *
     * \param ui User interface instance
     * \param piece_id Identification number of the potentially placeable piece
     * \param field_id Identification number of the required field
     */
    friend void set_field(const Boardgame_Ui& ui, const boardgame::Piece_Number piece_id, const boardgame::Field_Number field_id) { ui.bc->sf(piece_id, field_id); }

    /** \brief Indicate setup mode
     *
     * \param ui User interface instance
     * \param is_active A UI element shall represent the state of this parameter, so the user is able to see whether setup mode is currently active or not
     */
    friend void setup_mode_active(const Boardgame_Ui& ui, const bool is_active) { ui.bc->sma(is_active); }

    /** \brief Indicate computer engine control
     *
     * \param ui User interface instance
     * \param player_id Identification string of the player
     * \param is_active A UI element shall represent the state of this parameter, so the user is able to see whether the engine is active or not
     */
    friend void engine_active(const Boardgame_Ui& ui, const std::string& player_id, const bool is_active) { ui.bc->ea(player_id, is_active); }

    /** \brief Indicate computer engine control
     *
     * \param ui User interface instance
     * \param player_id Identification string of the player
     *
     * This can be used to play an animation if the \ref engine_active "engine" for the same player is active.
     */
    friend void active_player(const Boardgame_Ui& ui, const std::string& player_id) { ui.bc->ap(player_id); }

    /** \brief Control confirmation
     *
     * \param ui User interface instance
     * \param is_needed If `false`, corresponding UI elements shall trigger their action with a single click.
     * If `true`, more than one click is required and also a double click shall not trigger the action.
     */
    friend void need_confirm(const Boardgame_Ui& ui, const bool is_needed) { ui.bc->nc(is_needed); }

private:
    struct Boardgame_Ui_Concept
    {
        virtual ~Boardgame_Ui_Concept() = default;
        virtual void dr() const = 0;
        virtual void wi(const std::string& p) const = 0;
        virtual void dch(const std::string& pi, const bool ch) const = 0;
        virtual void pch(const std::string& pi, const bool ch) const = 0;
        virtual void lp(const boardgame::Piece_Number pid) const = 0;
        virtual void sel(const boardgame::Piece_Number pid) const = 0;
        virtual void rem(const boardgame::Piece_Number pid) const = 0;
        virtual void lf(const boardgame::Field_Number fid) const = 0;
        virtual void hl(const boardgame::Field_Number fid) const = 0;
        virtual void occ(const boardgame::Field_Number fid, const boardgame::Piece_Number pid) const = 0;
        virtual void sf(const boardgame::Piece_Number pid, const boardgame::Field_Number fid) const = 0;
        virtual void sma(const bool ia) const = 0;
        virtual void ea(const std::string& pi, const bool ia) const = 0;
        virtual void ap(const std::string& pi) const = 0;
        virtual void nc(const bool in) const = 0;
    };
    template <typename T>
    struct Boardgame_Ui_Model : Boardgame_Ui_Concept
    {
        Boardgame_Ui_Model(T mui) : Boardgame_Ui_Concept(), bm(std::move(mui)) {}
        void dr() const override { draw(bm); }
        void wi(const std::string& p) const override { win(bm, p); }
        void dch(const std::string& pi, const bool ch) const override { drawer_can_hide(bm, pi, ch); }
        void pch(const std::string& pi, const bool ch) const override { prison_can_hide(bm, pi, ch); }
        void lp(const boardgame::Piece_Number pid) const override { lock_piece(bm, pid); }
        void sel(const boardgame::Piece_Number pid) const override { selectable(bm, pid); }
        void rem(const boardgame::Piece_Number pid) const override { removable(bm, pid); }
        void lf(const boardgame::Field_Number fid) const override { lock_field(bm, fid); }
        void hl(const boardgame::Field_Number fid) const override { highlight(bm, fid); }
        void occ(const boardgame::Field_Number fid, const boardgame::Piece_Number pid) const override { occupiable(bm, fid, pid); }
        void sf(const boardgame::Piece_Number pid, const boardgame::Field_Number fid) const override { set_field(bm, pid, fid); }
        void sma(const bool ia) const override { setup_mode_active(bm, ia); }
        void ea(const std::string& pi, const bool ia) const override { engine_active(bm, pi, ia); }
        void ap(const std::string& pi) const override { active_player(bm, pi); }
        void nc(const bool in) const override { need_confirm(bm, in); }
        T bm;
    };
    std::shared_ptr<const Boardgame_Ui_Concept> bc;
};

}

#endif // SRC_BOARDGAME_BOARDGAME_UI

/** \mainpage Boardgames
 *
 * namespace \ref boardgame contains code independent of game and UI
 *
 * namespace \ref muehle contains code for the game Muehle
 *
 *
 * # Glossary
 *
 * * **Drawer**: Area for pieces that are not on the game board from the beginning
 * * **Prison**: Destination for pieces that get removed from the game board
 */
