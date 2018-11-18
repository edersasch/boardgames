#include "muehle.h"

#include <array>
#include <initializer_list>
#include <algorithm>

namespace
{

constexpr std::array<std::array<int, 2>, 24> vertical_muehle {{
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

constexpr std::array<std::initializer_list<int>, 24> adjacent_fields {{
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

std::vector<int> fields_of_removable_pieces;
std::vector<int> free_fields_store;
std::vector<int> fields_of_selectable_pieces_store;
std::vector<int> free_adjacent_fields_store;
std::vector<muehle::Muehle_Key> successors;

int prison_offset(const muehle::Muehle_Key& key)
{
    return key.test(54) ? 48 : 51;
}

/// @param field < number_of_board_fields
bool is_field_free(const muehle::Muehle_Key& key, const std::size_t field)
{
    return !(key[field] || key[field + muehle::number_of_board_fields.v]);
}

/// @param field < number_of_board_fields
int number_of_free_adjacent_fields(const muehle::Muehle_Key& key, const std::size_t field)
{
    auto check_free = [&key](const int f) {
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
muehle::Muehle_Key to_prison(muehle::Muehle_Key key, const int prisoner)
{
    key.reset(prisoner + muehle::board_offset(key));
    return increase_prisoner_count(key);
}

}

namespace muehle
{

int board_offset(const muehle::Muehle_Key& key)
{
    return key.test(54) ? muehle::first_board_field.v : muehle::number_of_board_fields.v;
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
    key[54] = true; // pretend white
    fill_key(first_white_piece.v);
    key[54] = false; // pretend black
    fill_key(first_black_piece.v);
    key[54] = whites_turn; // real value
    return key;
}

int game_phase(const Muehle_Key& key)
{
    auto captured = prisoner_count(key);
    if (captured == 6) {
        return 3;
    }
    auto active = (key & (Muehle_Key(0xffffff) << board_offset(key))).count();
    return captured + active == 9 ? 2 : 1;
}

/// @return < number_of_board_fields
const std::vector<int>& free_fields(const Muehle_Key& key)
{
    free_fields_store.clear();
    for (int f = 0U; f < number_of_board_fields.v; f += 1) {
        if (is_field_free(key, f)) {
            free_fields_store.push_back(f);
        }
    }
    return free_fields_store;
}

/// @return < number_of_board_fields
const std::vector<int>& fields_of_selectable_pieces(const Muehle_Key& key)
{
    fields_of_selectable_pieces_store.clear();
    int phase = game_phase(key);
    if (phase == 1) {
        fields_of_selectable_pieces_store.push_back(drawer_field);
    } else {
        auto offset = board_offset(key);
        for (auto i = 0U; i < number_of_board_fields.v; i += 1) {
            if (key.test(i + offset) &&
                         (phase == 3 || can_slide(key, i))) {
                fields_of_selectable_pieces_store.push_back(i);
            }
        }
    }
    return fields_of_selectable_pieces_store;
}

/// @param startfield_of_piece < number_of_board_fields
const std::vector<int>& occupiable_fields(const Muehle_Key& key, const int startfield_of_piece)
{
    return (game_phase(key) != 2) ?
                free_fields(key) :
                free_adjacent_fields(key, startfield_of_piece);
}

/// @param field < number_of_board_fields
bool closed_muehle(const Muehle_Key& key, const int field)
{
    auto offset = board_offset(key);
    auto muehle_on_fields = [key, offset] (int field_one, int field_two, int field_three) {
        return key[field_one + offset] && key[field_two + offset] && key[field_three + offset];
    };

    if (muehle_on_fields(field, vertical_muehle[static_cast<std::size_t>(field)][0], vertical_muehle[static_cast<std::size_t>(field)][1])) {
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

unsigned long prisoner_count(const Muehle_Key& key)
{
    return (key >> prison_offset(key)).to_ulong() & 7;
}

/// @return < number_of_board_fields
std::pair<std::vector<int>, Muehle_Key> occupy(Muehle_Key key, const int from, const int to)
{
    auto offset = board_offset(key);
    fields_of_removable_pieces.clear();
    if ((from == drawer_field || key.test(from + offset)) && is_field_free(key, to)) {
        if (from != drawer_field) {
            key.reset(from + offset);
        }
        key.set(to + offset);
        if (closed_muehle(key, to)) {
            key.flip(54);
            offset = board_offset(key);
            for (int i = 0; i < number_of_board_fields.v; i += 1) {
                if (key.test(i + offset) && !closed_muehle(key, i)) {
                    fields_of_removable_pieces.push_back(i);
                }
            }
            key.flip(54);
        }
    }
    return {fields_of_removable_pieces, key};
}

/// @param field < number_of_board_fields
bool can_slide(const Muehle_Key& key, const std::size_t field)
{
    auto check_free = [&key](const int f) {
        return is_field_free(key, f);
    };
    return std::find_if(adjacent_fields.at(field).begin(), adjacent_fields.at(field).end(), check_free) != adjacent_fields.at(field).end();
}

/// @param field < number_of_board_fields
/// @return < number_of_board_fields
const std::vector<int>& free_adjacent_fields(const Muehle_Key& key, const std::size_t field)
{
    auto check_free = [&key](const int f) {
        return is_field_free(key, f);
    };
    free_adjacent_fields_store.resize(adjacent_fields.at(field).size());
    free_adjacent_fields_store.erase(std::copy_if(adjacent_fields.at(field).begin(), adjacent_fields.at(field).end(), free_adjacent_fields_store.begin(), check_free), free_adjacent_fields_store.end());
    return free_adjacent_fields_store;
}

const std::vector<Muehle_Key> Engine_Helper::successor_constellations(const Muehle_Key& key)
{
    successors.clear();
    for (const auto& selectable : fields_of_selectable_pieces(key)) {
        for (const auto& occupiable : occupiable_fields(key, selectable)) {
            const auto& [removable, successor] = occupy(key, selectable, occupiable);
            if (removable.empty()) {
                successors.push_back(successor);
            } else {
                for (const auto prisoner : removable) {
                    successors.push_back(to_prison(switch_player(successor), prisoner).flip(54));
                }
            }
        }
    }
    return successors;
}

int Engine_Helper::evaluate(const Muehle_Key& key, int engine_winning_score)
{
    auto evaluate_free_fields = [](Muehle_Key ffkey){
        int free_fields_score = 0;
        auto offset = board_offset(ffkey);
        for (unsigned f = 0; f < number_of_board_fields.v; f += 1) {
            if (ffkey.test(f + offset)) {
                free_fields_score += number_of_free_adjacent_fields(ffkey, f);
            }
        }
        return free_fields_score;
    };
    auto prisoners = prisoner_count(key);
    auto other_prisoners = prisoner_count(switch_player(key));
    if (prisoners == 7) {
        return -engine_winning_score;
    }
    if (other_prisoners == 7) {
        return engine_winning_score;
    }
    return evaluate_free_fields(key) * 5 -
            evaluate_free_fields(switch_player(key)) * 5 +
            ((other_prisoners - prisoners) * 100);
}

Muehle_Key Engine_Helper::switch_player(const Muehle_Key& key)
{
    return Muehle_Key(key).flip(54);
}

}
