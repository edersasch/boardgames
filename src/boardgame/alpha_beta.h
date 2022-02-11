#ifndef SRC_BOARDGAME_ALPHA_BETA
#define SRC_BOARDGAME_ALPHA_BETA

// https://martin.ankerl.com/2019/04/01/hashmap-benchmarks-05-conclusion/
#include "robin_hood.h"

#include <chrono>
#include <vector>
#include <algorithm>
#include <atomic>

#include <iostream>

namespace boardgame {

template <typename Key, typename Game, typename Move_Data, typename Hash = robin_hood::hash<Key>>
class Alpha_Beta
{
public:
    /// -winning_score is used for lost game
    static constexpr std::int32_t winning_score         {2'000'000'000};
    static constexpr std::int32_t invalid_high_score    {winning_score + 1};
    static constexpr std::int32_t invalid_low_score     {-winning_score - 1};
    enum class No_Move_Policy {
        draw,
        lose
    };
    void start(const Key key, Move_Data md = {});
    std::vector<Key> get_next() const { return next; }
    std::int32_t get_score() const { return next_score; }
    std::int32_t get_depth() const { return current_depth; }
    bool is_running() const { return running; }
    void stop_running() { stop_request = true; }
    void set_target_depth(std::int32_t depth);
private:
    struct Key_Info
    {
        std::int32_t score {invalid_low_score};
        std::int32_t depth {0};
        std::vector<Key> successors;
    };
    void iterative_depth(const Key& key, Move_Data& md);
    std::int32_t engine(const Key& key, const std::int32_t depth, std::int32_t alpha, const std::int32_t beta, Move_Data& md);
    static constexpr std::int32_t target_depth_default {9999};
    std::int32_t target_depth {target_depth_default};
    std::int32_t current_depth {0};
    std::int32_t next_score {0};
    std::vector<Key> next;
    No_Move_Policy no_move_policy {No_Move_Policy::lose};
    std::atomic_bool running {false};
    std::atomic_bool stop_request {false};
    robin_hood::unordered_node_map<Key, Key_Info, Hash> transposition_table; // node map has stable references and pointers
};

template <typename Key, typename Game, typename Move_Data, typename Hash>
void Alpha_Beta<Key, Game, Move_Data, Hash>::start(const Key key, Move_Data md)
{
    if (running) {
        std::cerr << "alpha beta engine start, but running\n";
        return;
    }
    running = true;
    stop_request = false;
    next.clear();
    next_score = 0;
    iterative_depth(key, md);
    transposition_table.clear();
    stop_request = false;
    running = false;
}

template <typename Key, typename Game, typename Move_Data, typename Hash>
void Alpha_Beta<Key, Game, Move_Data, Hash>::set_target_depth(std::int32_t depth)
{
    target_depth = depth > 0 ? depth : target_depth_default;
}

// private

template <typename Key, typename Game, typename Move_Data, typename Hash>
void Alpha_Beta<Key, Game, Move_Data, Hash>::iterative_depth(const Key& key, Move_Data& md)
{
    auto start_time = std::chrono::steady_clock::now();
    for (current_depth = 1; current_depth <= target_depth; current_depth += 1) {
        engine(key, current_depth, invalid_low_score, invalid_high_score, md);
        auto end_time = std::chrono::steady_clock::now();
        std::chrono::milliseconds tdiff = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        std::cerr << (std::chrono::duration_cast<std::chrono::milliseconds>(end_time.time_since_epoch())).count() << "ms " << current_depth << " depth " << transposition_table.size() << " tt " << tdiff.count() << "ms\n";
        if (stop_request || next_score == winning_score || next_score == -winning_score) {
            break;
        }
    }
}

template <typename Key, typename Game, typename Move_Data, typename Hash>
std::int32_t Alpha_Beta<Key, Game, Move_Data, Hash>::engine(const Key& key, const std::int32_t depth, std::int32_t alpha, const std::int32_t beta, Move_Data& md)
{
    auto update_next = [this, &depth, &alpha](auto successors) {
        if (depth == current_depth) {
            auto currdepth = depth;
            next.clear();
            next_score = alpha;
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
    };
    Key_Info local_info;
    auto info = &local_info;
    auto info_it = transposition_table.find(key);
    if (info_it != transposition_table.end()) {
        info = &info_it->second;
    }
    if (depth == 0) {
        if (info->score < -winning_score) {
            info->score = Game::evaluate(key, winning_score);
        }
        return info->score;
    }
    if (info->depth >= depth && info->score > alpha) {
        alpha = info->score;
        update_next(&info->successors);
    }
    if (info->successors.empty()) {
        info->successors = Game::successor_constellations(key);
    }
    if (info->successors.empty()) {
        switch(no_move_policy) {
        case No_Move_Policy::lose:
            return -winning_score;
        default:
            return 0; // draw
        }
    }
    const auto successors_copy = info->successors; // successors might get reordered during recursion
    auto sccopy_it = successors_copy.begin();
    while(sccopy_it != successors_copy.end() && alpha < beta && !stop_request) {
        const auto& n = *sccopy_it;
        auto score = Game::make_move(md, key, n, winning_score, invalid_low_score);
        if (score == invalid_low_score) {
            score = -engine(n, depth - 1, -beta, -alpha, md);
        }
        Game::unmake_move(md, key, n);
        if (depth > info->depth || (depth == info->depth && score > info->score)) {
            info->depth = depth;
            info->score = score;
        }
        if (score > alpha) {
            alpha = score;
            if (info == &local_info) {
                info = &transposition_table[key];
                *info = local_info;
            }
            auto successors = &info->successors;
            auto it = std::find(successors->begin(), successors->end(), n);
            std::rotate(successors->begin(), it, it + 1);
            update_next(successors);
        }
        sccopy_it += 1;
    }
    return alpha;
}

}

#endif // SRC_BOARDGAME_ALPHA_BETA
