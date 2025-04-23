#ifndef SRC_MUEHLE_MUEHLE_FSM
#define SRC_MUEHLE_MUEHLE_FSM

#include "muehle_lib.h"
#include "muehle_fsm_action_handler.h"
#include <boost/sml.hpp>

namespace sml = boost::sml;

namespace muehle
{

static constexpr auto piece_selection                       = sml::state<class Piece_Selection>;
static constexpr auto destination_selection                 = sml::state<class Destination_Selection>;
static constexpr auto prisoner_selection                    = sml::state<class Prisoner_Selection>;
static constexpr auto engine_running                        = sml::state<class Engine_Running>;

struct Trigger_Make_Move {};
static constexpr auto trigger_make_move                     = sml::event<Trigger_Make_Move>;
struct Trigger_Start_Engine {};
static constexpr auto trigger_start_engine                  = sml::event<Trigger_Start_Engine>;
struct Trigger_Select_Piece
{
    boardgame::Piece_Number pn;
};
static constexpr auto trigger_select_piece                  = sml::event<Trigger_Select_Piece>;
struct Trigger_Occupy
{
    boardgame::Field_Number fn;
};
static constexpr auto trigger_occupy                        = sml::event<Trigger_Occupy>;
struct Trigger_Remove
{
    boardgame::Piece_Number pn;
};
static constexpr auto trigger_remove                        = sml::event<Trigger_Remove>;
struct Trigger_No_Removable_Piece {};
static constexpr auto trigger_no_removable_piece            = sml::event<Trigger_No_Removable_Piece>;
struct Trigger_Engine_Started {};
static constexpr auto trigger_engine_started                = sml::event<Trigger_Engine_Started>;
struct Trigger_Engine_Stopped {};
static constexpr auto trigger_engine_stopped                = sml::event<Trigger_Engine_Stopped>;

struct Set_Selectable_Game_Pieces
{
    void operator()(Muehle_FSM_Action_Handler* handler) const { handler->set_selectable_game_pieces(); }
};
static constexpr Set_Selectable_Game_Pieces set_selectable_game_pieces;
struct Select_Game_Piece
{
    template <typename EventT>
    void operator()(Muehle_FSM_Action_Handler* handler, const EventT& e) const { handler->select_game_piece(e.pn); }
};
static constexpr Select_Game_Piece select_game_piece;
struct Occupy_Game
{
    template <typename EventT>
    void operator()(Muehle_FSM_Action_Handler* handler, const EventT& e) const { handler->occupy_game(e.fn); }
};
static constexpr Occupy_Game occupy_game;
struct Remove_Piece_And_Finish_Move
{
    template <typename EventT>
    void operator()(Muehle_FSM_Action_Handler* handler, const EventT& e) const { handler->remove_piece_and_finish_move(e.pn); }
};
static constexpr Remove_Piece_And_Finish_Move remove_piece_and_finish_move;
struct Finish_Move
{
    void operator()(Muehle_FSM_Action_Handler* handler) const { handler->finish_move(); }
};
static constexpr Finish_Move finish_move;
struct Finish_Engine_Move
{
    void operator()(Muehle_FSM_Action_Handler* handler) const { handler->finish_engine_move(); }
};
static constexpr Finish_Engine_Move finish_engine_move;
struct Restore_Last_Constellation
{
    void operator()(Muehle_FSM_Action_Handler* handler) const { handler->restore_last_constellation(); }
};
static constexpr Restore_Last_Constellation restore_last_constellation;

struct Make_Move
{
    auto operator()() const noexcept
    {
        using namespace sml;
        return make_transition_table(
                 *piece_selection                       + sml::on_entry<_>                                              / set_selectable_game_pieces
                , piece_selection                       + trigger_select_piece                                          / select_game_piece                         = destination_selection
                , piece_selection                       + trigger_engine_started                                                                                    = engine_running
                , piece_selection                       + trigger_start_engine                                          / restore_last_constellation                = piece_selection
                , destination_selection                 + trigger_select_piece                                          / select_game_piece
                , destination_selection                 + trigger_occupy                                                / occupy_game                               = prisoner_selection
                , destination_selection                 + trigger_start_engine                                          / restore_last_constellation                = piece_selection
                , prisoner_selection                    + trigger_remove                                                / remove_piece_and_finish_move              = piece_selection
                , prisoner_selection                    + trigger_no_removable_piece                                    / finish_move                               = piece_selection
                , prisoner_selection                    + trigger_start_engine                                          / restore_last_constellation                = piece_selection
                , engine_running                        + trigger_engine_stopped                                        / finish_engine_move                        = piece_selection
                );
    }
};

static constexpr auto muehle_start                          = sml::state<class Muehle_Start>;
static constexpr auto move_list_import                      = sml::state<class Move_List_Import>;
static constexpr auto move_list_import_engine_stopping      = sml::state<class Move_List_Import_Engine_Stopping>;
static constexpr auto set_move_and_branch_engine_stopping   = sml::state<class Set_Move_And_Branch_Engine_Stopping>;
static constexpr auto new_game_engine_stopping              = sml::state<class New_Game_Engine_Stopping>;
static constexpr auto setup_mode_engine_stopping            = sml::state<class Setup_Mode_Engine_Stopping>;
static constexpr auto setup_mode                            = sml::state<class Setup_Mode>;
static constexpr auto make_move                             = sml::state<Make_Move>;

struct Trigger_Setup_Mode
{
    bool is_engine_running;
};
static constexpr auto trigger_setup_mode                    = sml::event<Trigger_Setup_Mode>;
struct Trigger_Import_Move_List
{
    bool is_engine_running;
};
static constexpr auto trigger_import_move_list              = sml::event<Trigger_Import_Move_List>;
struct Trigger_Start_New_Game
{
    bool is_engine_running;
};
static constexpr auto trigger_start_new_game                = sml::event<Trigger_Start_New_Game>;
struct Trigger_Set_Move_And_Branch
{
    bool is_engine_running;
};
static constexpr auto trigger_set_move_and_branch           = sml::event<Trigger_Set_Move_And_Branch>;

struct Is_Engine_Running
{
    template <typename EventT>
    bool operator()(const EventT& e) const
    {
        return e.is_engine_running;
    }
};
static constexpr Is_Engine_Running is_engine_running;

struct Select_Setup_Piece
{
    template <typename EventT>
    void operator()(Muehle_FSM_Action_Handler* handler, const EventT& e) const { handler->select_setup_piece(e.pn); }
};
static constexpr Select_Setup_Piece select_setup_piece;
struct Occupy_Setup
{
    template <typename EventT>
    void operator()(Muehle_FSM_Action_Handler* handler, const EventT& e) const { handler->occupy_setup(e.fn); }
};
static constexpr Occupy_Setup occupy_setup;
struct Start_New_Game
{
    void operator()(Muehle_FSM_Action_Handler* handler) const { handler->start_new_game(); }
};
static constexpr Start_New_Game start_new_game;
struct Import_Move_List
{
    void operator()(Muehle_FSM_Action_Handler* handler) const { handler->import_move_list(); }
};
static constexpr Import_Move_List import_move_list;
struct Set_Move_And_Branch
{
    void operator()(Muehle_FSM_Action_Handler* handler) const { handler->set_move_and_branch(); }
};
static constexpr Set_Move_And_Branch set_move_and_branch;
struct Stop_Engine
{
    void operator()(Muehle_FSM_Action_Handler* handler) const { handler->stop_engine(); }
};
static constexpr Stop_Engine stop_engine;
struct Enter_Setup_Mode
{
    void operator()(Muehle_FSM_Action_Handler* handler) const { handler->enter_setup_mode(); }
};
static constexpr Enter_Setup_Mode enter_setup_mode;
struct Exit_Setup_Mode
{
    void operator()(Muehle_FSM_Action_Handler* handler) const { handler->exit_setup_mode(); }
};
static constexpr Exit_Setup_Mode exit_setup_mode;

struct Transition_Table
{
    auto operator()() const noexcept
    {
        using namespace sml;
        return make_transition_table(
                 *muehle_start                          + trigger_import_move_list                                                                                  = move_list_import
                , muehle_start                          + trigger_start_new_game                                        / start_new_game                            = make_move
                , move_list_import                      + sml::on_entry<_>                                              / import_move_list
                , move_list_import                      + trigger_start_new_game                                        / start_new_game                            = make_move
                , move_list_import                      + trigger_set_move_and_branch                                   / set_move_and_branch                       = make_move
                , make_move                             + trigger_setup_mode                  [ is_engine_running ]     / stop_engine                               = setup_mode_engine_stopping
                , make_move                             + trigger_setup_mode                  [ !is_engine_running ]                                                = setup_mode
                , make_move                             + trigger_start_new_game              [ is_engine_running ]     / stop_engine                               = new_game_engine_stopping
                , make_move                             + trigger_start_new_game              [ !is_engine_running ]    / start_new_game                            = make_move
                , make_move                             + trigger_import_move_list            [ is_engine_running ]     / stop_engine                               = move_list_import_engine_stopping
                , make_move                             + trigger_import_move_list            [ !is_engine_running ]                                                = move_list_import
                , make_move                             + trigger_set_move_and_branch         [ is_engine_running ]     / stop_engine                               = set_move_and_branch_engine_stopping
                , make_move                             + trigger_set_move_and_branch         [ !is_engine_running ]    / set_move_and_branch                       = make_move
                , setup_mode                            + sml::on_entry<_>                                              / enter_setup_mode
                , setup_mode                            + trigger_select_piece                                          / select_setup_piece
                , setup_mode                            + trigger_occupy                                                / occupy_setup
                , setup_mode                            + trigger_make_move                                                                                         = make_move
                , setup_mode                            + trigger_start_new_game                                        / start_new_game                            = make_move
                , setup_mode                            + trigger_import_move_list                                                                                  = move_list_import
                , setup_mode                            + sml::on_exit<_>                                               / exit_setup_mode
                , new_game_engine_stopping              + trigger_engine_stopped                                        / start_new_game                            = make_move
                , move_list_import_engine_stopping      + trigger_engine_stopped                                                                                    = move_list_import
                , set_move_and_branch_engine_stopping   + trigger_engine_stopped                                        / set_move_and_branch                       = make_move
                , setup_mode_engine_stopping            + trigger_engine_stopped                                                                                    = setup_mode
                );
    }
};

class MUEHLE_LIB_EXPORT Fsm
{
public:
    Fsm(muehle::Muehle_FSM_Action_Handler* action_handler) : fsm{action_handler} {}
    auto& get_fsm() { return fsm; }
private:
    sml::sm<muehle::Transition_Table> fsm;
};

}

#endif // SRC_MUEHLE_MUEHLE_FSM
