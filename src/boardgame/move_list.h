#ifndef SRC_BOARDGAME_MOVE_LIST
#define SRC_BOARDGAME_MOVE_LIST

#include <nlohmann/json.hpp>

#include <algorithm>
#include <vector>
#include <map>
#include <fstream>

namespace boardgame {

namespace detail
{

template <typename T>
struct Move_List_Entry
{
    T constellation;
    std::vector<int> hint;
    std::vector<int> branch_start_ids;
    int prev;
    std::vector<int> next;
};

template <typename T>
void to_json(nlohmann::json& j, const Move_List_Entry<T>& d)
{
    j = {{"co", d.constellation}};
    j.push_back({"hi", d.hint});
    j.push_back({"bs", d.branch_start_ids});
    j.push_back({"pr", d.prev});
    j.push_back({"ne", d.next});
}

template <typename T>
void from_json(const nlohmann::json& j, Move_List_Entry<T>& d)
{
    j.at("co").get_to(d.constellation);
    j.at("hi").get_to(d.hint);
    j.at("bs").get_to(d.branch_start_ids);
    j.at("pr").get_to(d.prev);
    j.at("ne").get_to(d.next);
}

template <typename T>
class Move_List
{
public:
    static constexpr int invalid_id = -1;
    static constexpr int initial_id = 0;
    bool has_entry(const int move_id) const { return move_list_entries.find(move_id) != move_list_entries.end(); }

    // getters for Move_List_Entry members
    const T& constellation(const int move_id) const { return move_list_entries.at(move_id).constellation; }
    const std::vector<int>& hint(const int move_id) const { return move_list_entries.at(move_id).hint; }
    const std::vector<int>& branch_start_ids(const int move_id) const { return move_list_entries.at(move_id).branch_start_ids; }
    int prev(const int move_id) const { return move_list_entries.at(move_id).prev; }
    const std::vector<int>& next(const int move_id) const { return move_list_entries.at(move_id).next; }
    nlohmann::json make_json();

    // modify move_list_entries
    std::pair<int, bool> commit(const T& constellation, int predecessor = invalid_id, const std::vector<int>& hint = {});
    void delete_move(const int move_id);
    bool process_json(nlohmann::json&& j);

private:
    using Entries = std::map<int, Move_List_Entry<T>>;
    int next_id {0};
    Entries move_list_entries;
};

template <typename T>
std::pair<int, bool> Move_List<T>::commit(const T& constellation, int predecessor, const std::vector<int>& hint)
{
    int entry_id = next_id;
    int branch_start_id = entry_id;
    if (move_list_entries.find(predecessor) != move_list_entries.end()) {
        auto& val = move_list_entries.at(predecessor);
        if (val.next.empty()) { // head of branch
            branch_start_id = val.branch_start_ids.at(0);
        } else {
            for (int i : val.next) { // if to be commited constellation matches a successor, return its entry id
                if (move_list_entries.at(i).constellation == constellation) {
                    return {i, false};
                }
            }
            for (int i = predecessor; i != invalid_id; i = move_list_entries.at(i).prev) {
                move_list_entries.at(i).branch_start_ids.push_back(branch_start_id); // all predecessors get the new branch_start_id
            }
        }
        val.next.push_back(entry_id);
    } else {
        predecessor = invalid_id;
    }
    move_list_entries[entry_id] = { constellation, hint, {branch_start_id}, predecessor, {} };
    next_id += 1;
    return {entry_id, true};
}

template <typename T>
void Move_List<T>::delete_move(const int move_id)
{
    auto curr = move_list_entries.find(move_id);
    if (curr != move_list_entries.end()) {

        // in case move_id is the start of a branch, remove the id from all predecessors' branch_start_ids vector
        for (int i = curr->second.prev; i != invalid_id; i = move_list_entries.at(i).prev) {
            auto b_it = std::find(move_list_entries.at(i).branch_start_ids.begin(), move_list_entries.at(i).branch_start_ids.end(), move_id);
            if (b_it != move_list_entries.at(i).branch_start_ids.end()) {
                move_list_entries.at(i).branch_start_ids.erase(b_it);
            } else {
                break; // move_id is not the start of a branch
            }
        }

        // the to be deleted move_id must not be an entry in the predecessor's next vector
        if (int prev_id = curr->second.prev; prev_id != invalid_id) {
            auto& pn = move_list_entries[prev_id].next;
            pn.erase(std::find(pn.begin(), pn.end(), move_id));
        }

        // finally delete
        move_list_entries.erase(curr);
    }
}

template <typename T>
bool Move_List<T>::process_json(nlohmann::json&& j)
{
    try {
        move_list_entries = j.get<std::map<int, Move_List_Entry<T>>>();
    } catch (const nlohmann::json::exception ex) {
        return false;
    }
    next_id = 0;
    if (!move_list_entries.empty()) {
        if (move_list_entries.find(initial_id) == move_list_entries.end()) {
            move_list_entries.clear();
            return false;
        }
        next_id = move_list_entries.crbegin()->first + 1;
    }
    return true;
}

template <typename T>
nlohmann::json Move_List<T>::make_json()
{
    return nlohmann::json(move_list_entries);
}

}

template <typename T>
std::vector<std::array<typename T::value_type, 2>> diff(const T& first, const T& second)
{
    std::vector<std::array<typename T::value_type, 2>> diffed;
    auto [f, s] = std::mismatch(first.begin(), first.end(), second.begin());
    while (s != second.end()) {
        diffed.push_back({*f, *s});
        f += 1;
        s += 1;
        std::tie(f, s) = std::mismatch(f, first.end(), s);
    }
    return diffed;
}

template <typename T, typename U>
class Move_List
{
public:
    Move_List(U move_list_ui, std::string (*commit_msg_provider)(std::vector<std::array<typename T::value_type, 2>>), const T& first_constellation, const std::vector<int>& hint);
    Move_List(U move_list_ui, std::string (*commit_msg_provider)(std::vector<std::array<typename T::value_type, 2>>), const std::string& import, bool is_path);
    bool set_current_move_and_branch_start_id(const int move_id);
    bool set_current_move_and_branch_start_id(const int move_id, const int branch_start_id);
    int get_current_move_id() const { return current_move_id; }
    int get_current_branch_start_id() const { return current_branch_start_id; }
    bool is_empty() const { return move_list.next(detail::Move_List<T>::initial_id).empty(); }
    bool move_list_forward();
    bool move_list_back() { return set_current_move_id(move_list.prev(current_move_id)); }
    const T& constellation() const { return move_list.constellation(current_move_id); }
    const std::vector<int>& hint() const { return move_list.hint(current_move_id); }
    bool commit(const T& new_constellation, const std::vector<int>& hint = {});
    bool cut_off(const int move_id);
    bool export_move_list(const std::string& path);
    std::string get_move_list_string();
    void set_modified();
    bool is_modified() { return modified; }

private:
    Move_List(U move_list_ui, std::string (*commit_msg_provider)(std::vector<std::array<typename T::value_type, 2>>));
    bool set_current_move_id(const int move_id);
    void delete_moves(const int move_id);
    void ui_replay(const int move_id);

    detail::Move_List<T> move_list {};
    U ui;
    int current_move_id {detail::Move_List<T>::invalid_id};
    int current_branch_start_id{detail::Move_List<T>::invalid_id};
    std::string (*msg)(std::vector<std::array<typename T::value_type, 2>>);
    bool modified { false };
};

template <typename T, typename U>
Move_List<T, U>::Move_List(U move_list_ui, std::string (*commit_msg_provider)(std::vector<std::array<typename T::value_type, 2>>), const T& first_constellation, const std::vector<int>& hint)
    : Move_List(move_list_ui, commit_msg_provider)
{
    move_list.commit(first_constellation, current_move_id, hint);
    set_current_move_and_branch_start_id(detail::Move_List<T>::initial_id);
}

template <typename T, typename U>
Move_List<T, U>::Move_List(U move_list_ui, std::string (*commit_msg_provider)(std::vector<std::array<typename T::value_type, 2>>), const std::string& import, bool is_path)
    : Move_List(move_list_ui, commit_msg_provider)
{
    nlohmann::json j;
    try {
        if (is_path) {
            std::ifstream in(import);
            in >> j;
        } else {
            j = nlohmann::json::parse(import);
        }
    } catch (const nlohmann::json::exception ex) {
        j.clear();
    }
    if (!j.empty()) {
        if (move_list.process_json(std::move(j))) {
            ui_replay(detail::Move_List<T>::initial_id);
            set_current_move_and_branch_start_id(detail::Move_List<T>::initial_id);
            return;
        }
    }
    current_move_id = detail::Move_List<T>::invalid_id;
}

template <typename T, typename U>
bool Move_List<T, U>::set_current_move_and_branch_start_id(const int move_id)
{
    if (set_current_move_id(move_id)) {
        current_branch_start_id = move_list.branch_start_ids(move_id).front();
        return true;
    }
    return false;
}

template <typename T, typename U>
bool Move_List<T, U>::set_current_move_and_branch_start_id(const int move_id, const int branch_start_id)
{
    if (set_current_move_and_branch_start_id(move_id)) {
        if (std::find(move_list.branch_start_ids(move_id).begin(), move_list.branch_start_ids(move_id).end(), branch_start_id) != move_list.branch_start_ids(move_id).end()) {
            current_branch_start_id = branch_start_id;
            return true;
        }
    }
    return false;
}

template <typename T, typename U>
bool Move_List<T, U>::move_list_forward()
{
    auto& next = move_list.next(current_move_id);
    for (int id : next) {
        auto& branch_start_ids = move_list.branch_start_ids(id);
        auto bsi_it = std::find(branch_start_ids.cbegin(), branch_start_ids.cend(), current_branch_start_id);
        if (bsi_it != branch_start_ids.cend() || next.size() == 1) {
            return set_current_move_id(id);
        }
    }
    if (!next.empty()) {
        return set_current_move_and_branch_start_id(next.front());
    }
    return false;
}

template <typename T, typename U>
bool Move_List<T, U>::commit(const T& new_constellation, const std::vector<int>& hint)
{
     auto [move_id, is_new_move] = move_list.commit(new_constellation, current_move_id, hint);
     if (is_new_move) {
         add_move(ui, move_id, msg(diff(constellation(), new_constellation)), hint);
         set_modified();
     }
     return set_current_move_and_branch_start_id(move_id);
}

template <typename T, typename U>
bool Move_List<T, U>::cut_off(const int move_id)
{
    if (move_list.has_entry(move_id) && move_id > detail::Move_List<T>::initial_id) {
        delete_moves(move_id);
        ui_cut_off(ui, move_id);
        if (!move_list.has_entry(current_move_id)) {
            return set_current_move_and_branch_start_id(detail::Move_List<T>::initial_id);
        }
        set_modified();
        return true;
    }
    return false;
}

template <typename T, typename U>
bool Move_List<T, U>::export_move_list(const std::string& path)
{
    std::ofstream out(path);
    out << get_move_list_string();
    return out.good();
}

template <typename T, typename U>
std::string Move_List<T, U>::get_move_list_string()
{
    return move_list.make_json().dump();
}

template <typename T, typename U>
void Move_List<T, U>::set_modified()
{
    if (!modified) {
        modified = true;
        need_confirm(ui, true);
    }
}

// private

template <typename T, typename U>
Move_List<T, U>::Move_List(U move_list_ui, std::string (*commit_msg_provider)(std::vector<std::array<typename T::value_type, 2>>))
    : ui(move_list_ui)
    , msg(commit_msg_provider)
{
    initial_constellation(ui, detail::Move_List<T>::initial_id);
}

template <typename T, typename U>
bool Move_List<T, U>::set_current_move_id(const int move_id)
{
    if (move_list.has_entry(move_id)) {
        current_move_id = move_id;
        current_move(ui, move_id);
        return true;
    }
    return false;
}

template <typename T, typename U>
void Move_List<T, U>::delete_moves(const int move_id)
{
    for (int id : move_list.next(move_id)) { // go to the end of each branch and start deleting there
        delete_moves(id);
    }
    move_list.delete_move(move_id);
    delete_move(ui, move_id);
}

template <typename T, typename U>
void Move_List<T, U>::ui_replay(const int move_id)
{
    for (auto successor : move_list.next(move_id)) {
        set_current_move_and_branch_start_id(move_id);
        add_move(ui, successor, msg(diff(move_list.constellation(move_id), move_list.constellation(successor))), move_list.hint(successor));
        ui_replay(successor);
    }
}

}

#endif // SRC_BOARDGAME_MOVE_LIST
