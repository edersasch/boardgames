#ifndef SRC_BOARDGAME_ALPHA_BETA
#define SRC_BOARDGAME_ALPHA_BETA

// https://martin.ankerl.com/2019/04/01/hashmap-benchmarks-05-conclusion/
#include "robin_hood.h"

#include <chrono>
#include <vector>
#include <algorithm>

#include <iostream>

namespace boardgame {

template <typename Key, typename Game, typename Hash = robin_hood::hash<Key>>
class Alpha_Beta
{
public:
    static constexpr int winning_score {2'000'000'000};
    enum class No_Move_Policy {
        draw,
        lose
    };
    void start(const Key key);
    std::vector<Key> get_next() const { return next; }
    int get_score() const { return next_score; }
    int get_depth() const { return current_depth; }
    bool is_running() const { return running; }
    void stop_running() { stop_request = true; }
    void set_target_depth(int depth);
private:
    struct Key_Info
    {
        int score {-winning_score - 1};
        int depth {0};
        std::vector<Key> successors;
    };
    void iterative_depth(const Key& key);
    int engine(const Key& key, const int depth, int alpha, const int beta);
    static constexpr int target_depth_default {9999};
    int target_depth {target_depth_default};
    int current_depth {0};
    int next_score {0};
    std::vector<Key> next;
    No_Move_Policy no_move_policy {No_Move_Policy::lose};
    bool running {false};
    bool stop_request {false};
    robin_hood::unordered_map<Key, Key_Info, Hash> transposition_table;
};

template <typename Key, typename Game, typename Hash>
void Alpha_Beta<Key, Game, Hash>::start(const Key key)
{
    if (running) {
        std::cerr << "alpha beta engine start, but running\n";
        return;
    }
    running = true;
    stop_request = false;
    iterative_depth(key);
    transposition_table.clear();
    stop_request = false;
    running = false;
}

template <typename Key, typename Game, typename Hash>
void Alpha_Beta<Key, Game, Hash>::set_target_depth(int depth)
{
    target_depth = depth > 0 ? depth : target_depth_default;
}

// private

template <typename Key, typename Game, typename Hash>
void Alpha_Beta<Key, Game, Hash>::iterative_depth(const Key& key)
{
    auto start_time = std::chrono::steady_clock::now();
    for (current_depth = 1; current_depth <= target_depth; current_depth += 1) {
        next_score = engine(key, current_depth, -winning_score - 1, winning_score + 1);
        auto end_time = std::chrono::steady_clock::now();
        std::chrono::milliseconds tdiff = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        std::cerr << current_depth << " depth " << transposition_table.size() << " tt " << tdiff.count() << "ms\n";
        if (stop_request || next_score == winning_score || next_score == -winning_score) {
            break;
        }
    }
}

template <typename Key, typename Game, typename Hash>
int Alpha_Beta<Key, Game, Hash>::engine(const Key& key, const int depth, int alpha, const int beta)
{
    auto& initial_info = transposition_table[key];
    if (depth == 0) {
        if (initial_info.score < -winning_score) {
            initial_info.score = Game::evaluate(key, winning_score);
        }
        return initial_info.score;
    }
    if (initial_info.depth >= depth && initial_info.score >= beta) {
        return initial_info.score;
    }
    if (initial_info.successors.empty()) {
        initial_info.successors = Game::successor_constellations(key);
    }
    if (initial_info.successors.empty()) {
        switch(no_move_policy) {
        case No_Move_Policy::lose:
            return -winning_score;
        default:
            return alpha;
        }
    }
    auto successors_copy = initial_info.successors; // recursive engine() calls alter the transpositon table, so use a copy
    for (const auto& n : successors_copy) {
        if (stop_request) {
            break;
        }
        auto score = -engine(n, depth - 1, -beta, -alpha);
        if (score > alpha) {
            alpha = score;
            auto& info = transposition_table[key];
            if (depth > info.depth || (depth == info.depth && alpha > info.score)) {
                info.depth = depth;
                info.score = alpha;
            }
            auto successors = &info.successors; // reorder the real successors
            auto it = std::find(successors->begin(), successors->end(), n);
            if (it != successors->begin()) {
                std::rotate(successors->begin(), it, it + 1);
                if (depth == current_depth && !stop_request) {
                    auto currdepth = depth;
                    next.clear();
                    while (!successors->empty() && currdepth) {
                        next.push_back(successors->front());
                        auto pos = transposition_table.find(successors->front());
                        if (pos == transposition_table.end()) {
                            break;
                        } else {
                            successors = &(pos->second.successors);
                        }
                        currdepth -= 1;
                    }
                }
            }
            if (alpha >= beta) {
                break;
            }
        }
    }
    return alpha;
}

}

#endif // SRC_BOARDGAME_ALPHA_BETA
