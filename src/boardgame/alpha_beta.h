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
    bool start(const Key key);
    std::vector<Key> get_next() const { return next; }
    int get_score() const { return next_score; }
    int get_depth() const { return current_depth; }
    bool is_running() const { return running; }
    void stop_running() { running = false; }
    void discard();
    void set_target_depth(int depth);
private:
    struct Key_Info
    {
        int score {-winning_score - 1};
        std::vector<Key> successors;
    };
    void iterative_depth(const Key& key);
    int engine(const Key& key, const int depth, int alpha, const int beta);
    std::vector<Key> successor_constellations(const Key& key);
    int evaluate(const Key& key);
    static constexpr int target_depth_default {9999};
    int target_depth {target_depth_default};
    int current_depth {0};
    int next_score {0};
    std::vector<Key> next;
    No_Move_Policy no_move_policy {No_Move_Policy::lose};
    bool running {false};
    bool valid_move {true};
    robin_hood::unordered_map<Key, Key_Info, Hash> transposition_table;
};

template <typename Key, typename Game, typename Hash>
bool Alpha_Beta<Key, Game, Hash>::start(const Key key)
{
    if (running) {
        std::cerr << "alpha beta engine start, but running\n";
        return false;
    }
    running = true;
    iterative_depth(key);
    running = false;
    transposition_table.clear();
    bool is_valid = valid_move;
    valid_move = true;
    return is_valid;
}

template <typename Key, typename Game, typename Hash>
void Alpha_Beta<Key, Game, Hash>::discard()
{
    if (running) {
        valid_move = false;
        running = false;
    }
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
        if (!running || next_score == winning_score || next_score == -winning_score) {
            break;
        }
    }
    auto end_time = std::chrono::steady_clock::now();
    std::chrono::milliseconds tdiff = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cerr << current_depth << " depth " << transposition_table.size() << " tt " << tdiff.count() << "ms\n";
}

template <typename Key, typename Game, typename Hash>
int Alpha_Beta<Key, Game, Hash>::engine(const Key& key, const int depth, int alpha, const int beta)
{
    if (depth == 0) {
        return evaluate(key);
    }
    auto successors_copy = successor_constellations(key); // recursive engine() calls alter the transpositon table, so use a copy
    if (successors_copy.empty()) {
        switch(no_move_policy) {
        case No_Move_Policy::lose:
            return -winning_score;
        default:
            return alpha;
        }
    }
    for (const auto& n : successors_copy) {
        if (!running) {
            break;
        }
        auto score = -engine(n, depth - 1, -beta, -alpha);
        if (score > alpha) {
            alpha = score;
            if (alpha >= beta) {
                break;
            }
            auto successors = &(transposition_table[key].successors); // reorder the real successors
            auto it = std::find(successors->begin(), successors->end(), n);
            std::rotate(successors->begin(), it, it + 1);
            if (depth == current_depth && running) {
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
    }
    return alpha;
}

template <typename Key, typename Game, typename Hash>
std::vector<Key> Alpha_Beta<Key, Game, Hash>::successor_constellations(const Key& key)
{
    auto& info = transposition_table[key];
    if (info.successors.empty()) {
        info.successors = Game::successor_constellations(key);
    }
    return info.successors;
}

template <typename Key, typename Game, typename Hash>
int Alpha_Beta<Key, Game, Hash>::evaluate(const Key& key)
{
    auto& info = transposition_table[key];
    if (info.score < -winning_score) {
        info.score = Game::evaluate(key, winning_score);
    }
    return info.score;
}

}

#endif // SRC_BOARDGAME_ALPHA_BETA
