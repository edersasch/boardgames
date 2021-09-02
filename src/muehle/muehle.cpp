#include "muehle.h"

#include <array>
#include <initializer_list>
#include <algorithm>

namespace
{

constexpr std::array<std::array<std::int8_t, 2>, muehle::number_of_board_fields.v> vertical_muehle {{
    {9, 21},
    {4, 7},
    {14, 23},
    {10, 18},
    {1, 7},
    {13, 20},
    {11, 15},
    {1, 4},
    {12, 17},
    {0, 21},
    {3, 18},
    {6, 15},
    {8, 17},
    {5, 20},
    {2, 23},
    {6, 11},
    {19, 22},
    {8, 12},
    {3, 10},
    {16, 22},
    {5,13},
    {0, 9},
    {16, 19},
    {2, 14}
}};

std::array<std::initializer_list<std::int8_t>, muehle::number_of_board_fields.v> adjacent_fields {{
    {1, 9},
    {0, 2, 4},
    {1, 14},
    {4, 10},
    {1, 3, 5, 7},
    {4, 13},
    {7, 11},
    {4, 6, 8},
    {7, 12},
    {0, 10, 21},
    {3, 9, 11, 18},
    {6, 10, 15},
    {8, 13, 17},
    {5, 12, 14, 20},
    {2, 13, 23},
    {11, 16},
    {15, 17, 19},
    {12, 16},
    {10, 19},
    {16, 18, 20, 22},
    {13, 19},
    {9, 22},
    {19, 21, 23},
    {14, 22}
}};

constexpr std::array<const char[3], 24> coordinate_names {
    "A7",
    "D7",
    "G7",
    "B6",
    "D6",
    "F6",
    "C5",
    "D5",
    "E5",
    "A4",
    "B4",
    "C4",
    "E4",
    "F4",
    "G4",
    "C3",
    "D3",
    "E3",
    "B2",
    "D2",
    "F2",
    "A1",
    "D1",
    "G1"
};

std::vector<std::int32_t> fields_of_removable_pieces;
std::vector<std::int32_t> pieces_in_muehle;
std::vector<std::int32_t> free_fields_store;
std::vector<std::int32_t> fields_of_selectable_pieces_store;
std::vector<std::int32_t> free_adjacent_fields_store;
std::vector<muehle::Muehle_Key> successors;

std::size_t prison_offset(const muehle::Muehle_Key& key)
{
    return key.test(muehle::use_white_data_in_key) ? muehle::white_prison_in_key : muehle::black_prison_in_key;
}

/// @param field < number_of_board_fields
bool is_field_free(const muehle::Muehle_Key& key, const std::size_t field)
{
    return !(key[field] || key[field + muehle::number_of_board_fields.v]);
}

/// @param field < number_of_board_fields
std::int32_t number_of_free_adjacent_fields(const muehle::Muehle_Key& key, const std::size_t field)
{
    auto check_free = [&key](const std::int32_t f) {
        return is_field_free(key, f);
    };
    return std::count_if(adjacent_fields.at(field).begin(), adjacent_fields.at(field).end(), check_free);
}

muehle::Muehle_Key increase_prisoner_count(const muehle::Muehle_Key& key)
{
    auto offset = prison_offset(key);
    return (muehle::Muehle_Key(key).reset(offset).reset(offset + 1).reset(offset + 2)) | muehle::Muehle_Key((muehle::prisoner_count(key) + 1) << offset);
}

/// @param prisoner < number_of_board_fields
muehle::Muehle_Key to_prison(muehle::Muehle_Key key, const std::int32_t prisoner)
{
    key.reset(prisoner + muehle::board_offset(key));
    return increase_prisoner_count(key);
}

}

namespace muehle
{

std::int32_t board_offset(const muehle::Muehle_Key& key)
{
    return key.test(use_white_data_in_key) ? muehle::first_board_field.v : muehle::number_of_board_fields.v;
}

Muehle_Key constellation_to_key(const Muehle_Constellation& constellation, const bool whites_turn)
{
    Muehle_Key key;
    auto fill_key = [&constellation, &key](std::size_t start) {
        for (auto i = start; i < start + number_of_pieces_per_player.v; i += 1) {
            auto field = constellation.at(i).v;
            if (field < first_board_field.v + number_of_board_fields.v) {
                key.set(static_cast<std::size_t>(field) + board_offset(key));
            } else {
                if (field >= first_white_prison_field.v) {
                    key = increase_prisoner_count(key);
                }
            }
        }
    };
    key[use_white_data_in_key] = true; // pretend white
    fill_key(first_white_piece.v);
    key[use_white_data_in_key] = false; // pretend black
    fill_key(first_black_piece.v);
    key[use_white_data_in_key] = whites_turn; // real value
    return key;
}

std::int32_t game_phase(const Muehle_Key& key)
{
    static constexpr std::int64_t all_active = 0xffffff;
    static constexpr std::int32_t six_prisoners = 6;
    auto captured = prisoner_count(key);
    if (captured == six_prisoners) {
        return 3;
    }
    std::int64_t active = (key & (Muehle_Key(all_active) << board_offset(key))).count();
    return captured + active == number_of_pieces_per_player.v ? 2 : 1;
}

/// @return < number_of_board_fields
const std::vector<std::int32_t>& free_fields(const Muehle_Key& key)
{
    free_fields_store.clear();
    for (std::int32_t f = 0U; f < number_of_board_fields.v; f += 1) {
        if (is_field_free(key, f)) {
            free_fields_store.push_back(f);
        }
    }
    return free_fields_store;
}

/// @return < number_of_board_fields
const std::vector<std::int32_t>& fields_of_selectable_pieces(const Muehle_Key& key)
{
    fields_of_selectable_pieces_store.clear();
    std::int32_t phase = game_phase(key);
    if (phase == 1) {
        fields_of_selectable_pieces_store.push_back(drawer_field);
    } else {
        auto offset = board_offset(key);
        for (auto i = 0; i < number_of_board_fields.v; i += 1) {
            if (key.test(i + offset) &&
                         (phase == 3 || can_slide(key, i))) {
                fields_of_selectable_pieces_store.push_back(i);
            }
        }
    }
    return fields_of_selectable_pieces_store;
}

/// @param startfield_of_piece < number_of_board_fields
const std::vector<std::int32_t>& occupiable_fields(const Muehle_Key& key, const std::int32_t startfield_of_piece)
{
    return (game_phase(key) != 2) ?
                free_fields(key) :
                free_adjacent_fields(key, startfield_of_piece);
}

/// @param field < number_of_board_fields
bool closed_muehle(const Muehle_Key& key, const std::int32_t field)
{
    auto offset = board_offset(key);
    auto muehle_on_fields = [key, offset] (std::int32_t field_one, std::int32_t field_two, std::int32_t field_three) {
        return key[field_one + offset] && key[field_two + offset] && key[field_three + offset];
    };

    if (muehle_on_fields(field, vertical_muehle.at(static_cast<std::size_t>(field)).at(0), vertical_muehle.at(static_cast<std::size_t>(field)).at(1))) {
        return true;
    }

    if (field % 3 == 0) { // horizontal
        return muehle_on_fields(field, field + 1, field + 2);
    }
    if (field % 3 == 1) {
        return muehle_on_fields(field, field - 1, field + 1);
    }
    return muehle_on_fields(field, field - 1, field - 2);
}

std::int64_t prisoner_count(const Muehle_Key& key)
{
    return static_cast<std::int64_t>((key >> prison_offset(key)).to_ullong() & all_prisoners_in_key);
}

/// @return < number_of_board_fields
std::pair<std::vector<std::int32_t>, Muehle_Key> occupy(Muehle_Key key, const std::int32_t from, const std::int32_t to)
{
    auto offset = board_offset(key);
    fields_of_removable_pieces.clear();
    pieces_in_muehle.clear();
    if ((from == drawer_field || key.test(from + offset)) && is_field_free(key, to)) {
        if (from != drawer_field) {
            key.reset(from + offset);
        }
        key.set(to + offset);
        if (closed_muehle(key, to)) {
            key.flip(use_white_data_in_key);
            offset = board_offset(key);
            for (std::int32_t i = 0; i < number_of_board_fields.v; i += 1) {
                if (key.test(i + offset)) {
                    if (!closed_muehle(key, i)) {
                        fields_of_removable_pieces.push_back(i);
                    } else {
                        pieces_in_muehle.push_back(i);
                    }
                }
            }
        } else {
            key.flip(use_white_data_in_key);
        }
    }
    return {fields_of_removable_pieces.empty() ? pieces_in_muehle : fields_of_removable_pieces, key};
}

/// @param field < number_of_board_fields
bool can_slide(const Muehle_Key& key, const std::size_t field)
{
    auto check_free = [&key](const std::int32_t f) {
        return is_field_free(key, f);
    };
    return std::find_if(adjacent_fields.at(field).begin(), adjacent_fields.at(field).end(), check_free) != adjacent_fields.at(field).end();
}

/// @param field < number_of_board_fields
/// @return < number_of_board_fields
const std::vector<std::int32_t>& free_adjacent_fields(const Muehle_Key& key, const std::size_t field)
{
    auto check_free = [&key](const std::int32_t f) {
        return is_field_free(key, f);
    };
    free_adjacent_fields_store.resize(adjacent_fields.at(field).size());
    free_adjacent_fields_store.erase(std::copy_if(adjacent_fields.at(field).begin(), adjacent_fields.at(field).end(), free_adjacent_fields_store.begin(), check_free), free_adjacent_fields_store.end());
    return free_adjacent_fields_store;
}

std::string diff_text(const boardgame::Field_Number_Diff& fndiff)
{
    std::string commitmsg = "--"; // used in case the player was not able to move -> cannot happen, means game over in muehle
    if (!fndiff.empty()) { // example "F2-F4 xB2" means: moved from field F2 to field F4, removed piece from field B2
        std::string from;
        std::string to;
        std::string removed;
        for (const auto& d : fndiff) {
            if (d.at(1).v >= muehle::first_board_field.v && d.at(1).v < muehle::first_board_field.v + muehle::number_of_board_fields.v) {
                to = coordinate_names.at(d.at(1).v);
                if (d.at(0).v >= muehle::first_board_field.v && d.at(0).v < muehle::first_board_field.v + muehle::number_of_board_fields.v) {
                    from = std::string(coordinate_names.at(d.at(0).v)) + "-";
                }
            } else {
                if (d.at(1).v >= muehle::first_white_prison_field.v) {
                    removed = " x" + std::string(coordinate_names.at(d.at(0).v));
                }
            }
        }
        commitmsg = from + to + removed;
    }
    return commitmsg;
}

Muehle_Key switch_player(const Muehle_Key& key)
{
    return Muehle_Key(key).flip(use_white_data_in_key);
}

bool is_boring_move(const muehle::Muehle_Key& key, const muehle::Muehle_Key& successor)
{
    if (muehle::game_phase(key) > 1) {
        auto key_prisoner_count = muehle::prisoner_count(muehle::switch_player(key));
        auto successor_prisoner_count = muehle::prisoner_count(successor);
        return key_prisoner_count == successor_prisoner_count;
    }
    return false;
}

std::vector<Muehle_Key> Engine_Helper::successor_constellations(const Muehle_Key& key)
{
    successors.clear();
    for (const auto& selectable : fields_of_selectable_pieces(key)) {
        for (const auto& occupiable : occupiable_fields(key, selectable)) {
            const auto& [removable, successor] = occupy(key, selectable, occupiable);
            if (removable.empty()) {
                successors.push_back(successor);
            } else {
                for (const auto prisoner : removable) {
                    successors.push_back(to_prison(successor, prisoner));
                }
            }
        }
    }
    return successors;
}

std::int32_t Engine_Helper::evaluate(const Muehle_Key& key, std::int32_t engine_winning_score)
{
    auto evaluate_free_fields = [](const Muehle_Key& ffkey){
        std::int32_t free_fields_score = 0;
        auto offset = board_offset(ffkey);
        for (auto f = 0; f < number_of_board_fields.v; f += 1) {
            if (ffkey.test(f + offset)) {
                free_fields_score += number_of_free_adjacent_fields(ffkey, f);
            }
        }
        return free_fields_score;
    };
    if (prisoner_count(key) == all_prisoners_in_key) {
        return -engine_winning_score;
    }
    if (prisoner_count(switch_player(key)) == all_prisoners_in_key) {
        return engine_winning_score;
    }
    return static_cast<std::int32_t>((evaluate_free_fields(key) - evaluate_free_fields(switch_player(key))) * free_field_factor);
}

std::int32_t Engine_Helper::make_move(Muehle_Move_Data &md, const Muehle_Key &key, const Muehle_Key &successor, std::int32_t engine_winning_score, std::int32_t engine_invalid_score)
{
    (void)engine_winning_score; // not needed, checks lead to draw
    if (is_boring_move(key, successor)) {
        md.number_of_consecutive_boring_moves_stack.push_back(md.number_of_consecutive_boring_moves_stack.back() + 1);
    } else {
        md.number_of_consecutive_boring_moves_stack.push_back(0);
    }
    return md.number_of_consecutive_boring_moves_stack.back() < 50 ? engine_invalid_score : 0;
}

void Engine_Helper::unmake_move(Muehle_Move_Data &md, const Muehle_Key &key, const Muehle_Key &successor)
{
    (void)key; // not needed, just set to previous value
    (void)successor; // not needed, just set to previous value
    md.number_of_consecutive_boring_moves_stack.pop_back();
}

}
