#ifndef SRC_MUEHLE_MUEHLE_TYPES
#define SRC_MUEHLE_MUEHLE_TYPES

#include "boardgame/pieces_n_fields.h"
#include "robin_hood.h"

#include <bitset>
#include <vector>

namespace muehle
{

static constexpr boardgame::Piece_Range number_of_pieces_per_player  {9};
static constexpr boardgame::Piece_Range number_of_pieces           = number_of_pieces_per_player + number_of_pieces_per_player;
static constexpr boardgame::Piece_Number first_white_piece           {0};
static constexpr boardgame::Piece_Number first_black_piece         = first_white_piece + number_of_pieces_per_player;
static constexpr boardgame::Field_Range number_of_board_fields       {24};
static constexpr boardgame::Field_Range number_of_drawer_fields      {number_of_pieces_per_player.v};
static constexpr boardgame::Field_Range number_of_prison_fields      {number_of_drawer_fields.v - 2}; // game ends if only two pieces left, so two less than number_of_drawer_fields
static constexpr boardgame::Field_Range number_of_fields           = number_of_board_fields + number_of_drawer_fields + number_of_drawer_fields + number_of_prison_fields + number_of_prison_fields;
static constexpr boardgame::Field_Number first_board_field           {0};
static constexpr boardgame::Field_Number first_white_drawer_field  = first_board_field + number_of_board_fields;
static constexpr boardgame::Field_Number first_black_drawer_field  = first_white_drawer_field + number_of_drawer_fields;
static constexpr boardgame::Field_Number first_white_prison_field  = first_black_drawer_field + number_of_drawer_fields;
static constexpr boardgame::Field_Number first_black_prison_field  = first_white_prison_field + number_of_prison_fields;
static constexpr std::int32_t drawer_field = -1;
static constexpr std::int32_t prison_field = -2;
static constexpr auto white_id = "white";
static constexpr auto black_id = "black";

constexpr std::int32_t use_white_data_in_key = 54;
constexpr std::int32_t white_prison_in_key = 48;
constexpr std::int32_t black_prison_in_key = 51;
constexpr std::int32_t all_prisoners_in_key = 7;

constexpr std::int32_t free_field_factor = 16;

using Muehle_Constellation = std::array<boardgame::Field_Number, number_of_pieces.v>;
using Muehle_Key = std::bitset<55>; // 0-23 white board, 24-47 black board, 48-50 white BCD inmate count, 51-53 black BCD inmate count, 54 white's data if set

std::int32_t board_offset(const muehle::Muehle_Key& key);
Muehle_Key constellation_to_key(const Muehle_Constellation& constellation, const bool whites_turn);
std::int32_t game_phase(const Muehle_Key& key);
const std::vector<std::int32_t>& free_fields(const Muehle_Key& key);
const std::vector<std::int32_t>& fields_of_selectable_pieces(const Muehle_Key& key);
const std::vector<std::int32_t>& occupiable_fields(const Muehle_Key& key, const std::int32_t startfield_of_piece);
bool closed_muehle(const Muehle_Key& key, const std::int32_t field);
std::int64_t prisoner_count(const Muehle_Key& key);
std::pair<std::vector<std::int32_t>, Muehle_Key> occupy(Muehle_Key key, const std::int32_t from, const std::int32_t to);
bool can_slide(const Muehle_Key& key, const std::size_t field);
const std::vector<std::int32_t>& free_adjacent_fields(const Muehle_Key& key, const std::size_t field);
std::string diff_text(const boardgame::Field_Number_Diff& fndiff);
Muehle_Key switch_player(const Muehle_Key& key);
bool is_boring_move(const muehle::Muehle_Key& key, const muehle::Muehle_Key& successor);

struct Muehle_Key_Hash
{
    std::uint64_t operator()(const Muehle_Key& key) const { return key.to_ullong(); }
};

struct Muehle_Move_Data
{
    std::vector<std::int32_t> consecutive_boring_moves;
    robin_hood::unordered_node_map<Muehle_Key, std::vector<std::int32_t>, Muehle_Key_Hash> key_occurrence;
};

/// -engine_winning_score is used for lost game, 0 for draw
struct Engine_Helper
{
    static std::vector<Muehle_Key> successor_constellations(const Muehle_Key& key);
    static std::int32_t evaluate(const Muehle_Key& key, std::int32_t engine_winning_score);
    static std::int32_t make_move(Muehle_Move_Data& md, const Muehle_Key& key, const Muehle_Key& successor, std::int32_t engine_winning_score, std::int32_t engine_invalid_score);
    static void unmake_move(Muehle_Move_Data& md, const Muehle_Key& key, const Muehle_Key& successor);
};

}

#endif // SRC_MUEHLE_MUEHLE_TYPES
