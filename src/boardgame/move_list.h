#ifndef SRC_BOARDGAME_MOVE_LIST
#define SRC_BOARDGAME_MOVE_LIST

#include <algorithm>
#include <vector>
#include <unordered_map>

namespace boardgame {

namespace detail
{

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

    // modify move_list_entries
    std::pair<int, int> commit(const T& constellation, int predecessor = invalid_id, const std::vector<int>& hint = {});
    void delete_move(const int move_id);

private:
    struct Move_List_Entry
    {
        T constellation;
        std::vector<int> hint;
        std::vector<int> branch_start_ids;
        int prev;
        std::vector<int> next;
    };
    using Entries = std::unordered_map<int, Move_List_Entry>;
    int next_id {0};
    Entries move_list_entries;
};

template <typename T>
std::pair<int, int> Move_List<T>::commit(const T& constellation, int predecessor, const std::vector<int>& hint)
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
                    return {i, invalid_id};
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
    return {entry_id, branch_start_id};
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

}

template <typename T, typename U>
class Move_List
{
public:
    Move_List(U move_list_ui)
        : ui(move_list_ui) {}
    bool set_current_move_id(const int move_id);
    bool set_current_move_and_branch_start_id(const int move_id);
    int get_current_move_id() const { return current_move_id; }
    int get_current_branch_start_id() const { return current_branch_start_id; }
    bool move_list_forward();
    bool move_list_back() { return set_current_move_id(move_list.prev(current_move_id)); }
    const T& constellation() const { return move_list.constellation(current_move_id); }
    const std::vector<int>& hint() const { return move_list.hint(current_move_id); }
    void commit(const T& constellation, const std::string& commitmsg, const std::vector<int>& hint = {});
    bool cut_off(const int move_id);

private:
    void delete_moves(const int move_id);

    detail::Move_List<T> move_list;
    U ui;
    int current_move_id {detail::Move_List<T>::invalid_id};
    int current_branch_start_id{detail::Move_List<T>::invalid_id};
};

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
bool Move_List<T, U>::set_current_move_and_branch_start_id(const int move_id)
{
    if (set_current_move_id(move_id)) {
        current_branch_start_id = move_list.branch_start_ids(move_id).front();
        return true;
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
void Move_List<T, U>::commit(const T& constellation, const std::string& commitmsg, const std::vector<int>& hint)
{
     auto [move_id, branch_start_id] = move_list.commit(constellation, current_move_id, hint);
     if (branch_start_id != detail::Move_List<T>::invalid_id) {
         if (current_move_id == detail::Move_List<T>::invalid_id) {
             initial_constellation(ui, move_id);
         } else {
             add_move(ui, move_id, branch_start_id, commitmsg, hint);
         }
     }
     set_current_move_and_branch_start_id(move_id);
}

template <typename T, typename U>
bool Move_List<T, U>::cut_off(const int move_id)
{
    if (move_list.has_entry(move_id)) {
        delete_moves(move_id);
        ui_cut_off(ui, move_id);
        if (!move_list.has_entry(current_move_id)) {
            return set_current_move_and_branch_start_id(detail::Move_List<T>::initial_id);
        }
    }
    return false;
}

// private

template <typename T, typename U>
void Move_List<T, U>::delete_moves(const int move_id)
{
    for (int id : move_list.next(move_id)) { // go to the end of each branch and start deleting there
        delete_moves(id);
    }
    move_list.delete_move(move_id);
    delete_move(ui, move_id);
}

}

#endif // SRC_BOARDGAME_MOVE_LIST
