#ifndef SRC_BOARDGAME_ALPHA_BETA
#define SRC_BOARDGAME_ALPHA_BETA

#include <chrono>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

namespace boardgame {

template <typename T, typename U>
class Alpha_Beta
{
public:
    static constexpr int winning_score {2'000'000'000};
    enum class No_Move_Policy {
        draw,
        lose
    };
    bool start(const T key);
    std::vector<T> get_next() { return next; }
    int get_score() { return next_score; }
    int get_depth() { return current_depth; }
    bool is_running() { return running; }
    void stop_running() { running = false; }
    void discard();
    void set_target_depth(int depth);
private:
    struct Key_Info
    {
        int score {-winning_score - 1};
        std::vector<T> successors;
    };
    void iterative_depth(const T key);
    int engine(const T key, const int depth, int alpha, const int beta);
    Key_Info& successor_constellations(const T key);
    int evaluate(const T key);
    static constexpr int target_depth_default {9999};
    int target_depth {target_depth_default};
    int current_depth {0};
    int next_score {0};
    std::vector<T> to_check;
    std::vector<T> next;
    No_Move_Policy no_move_policy {No_Move_Policy::lose};
    bool running {false};
    bool valid_move {true};
    std::unordered_map<T, Key_Info> transposition_table;
};

template <typename T, typename U>
bool Alpha_Beta<T, U>::start(const T key)
{
    if (running) {
        std::cerr << "alpha beta engine start, but running\n";
    }
    //std::cerr << "4  1  87      0         0      3  0         0         0\n" << key << " before\n";
    running = true;
    iterative_depth(key);
    running = false;
    transposition_table.clear();
    bool is_valid = valid_move;
    valid_move = true;
    return is_valid;
}

template <typename T, typename U>
void Alpha_Beta<T, U>::discard()
{
    if (running) {
        valid_move = false;
        running = false;
    }
}

template <typename T, typename U>
void Alpha_Beta<T, U>::set_target_depth(int depth)
{
    target_depth = depth > 0 ? depth : target_depth_default;
}

// private

template <typename T, typename U>
void Alpha_Beta<T, U>::iterative_depth(const T key)
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

template <typename T, typename U>
int Alpha_Beta<T, U>::engine(const T key, const int depth, int alpha, const int beta)
{
    if (depth == 0) {
        return evaluate(key);
    }
    auto& info = successor_constellations(key);
    if (info.successors.empty()) {
        switch(no_move_policy) {
        case No_Move_Policy::lose:
            return -winning_score;
        default:
            return alpha;
        }
    }
    for (const auto& n : info.successors) {
        if (!running) {
            break;
        }
        auto score = -engine(n, depth - 1, -beta, -alpha);
        if (score > alpha) {
            alpha = score;
            if (alpha >= beta) {
                break;
            }
            auto tmp = n;
            info.successors.erase(std::find(info.successors.begin(), info.successors.end(), n));
            info.successors.insert(info.successors.begin(), tmp);
            if (depth == current_depth && running) {
                auto curr = key;
                auto currdepth = depth;
                next.clear();
                while (!transposition_table[curr].successors.empty() && currdepth) {
                    next.push_back(transposition_table[curr].successors.front());
                    if (transposition_table.find(next.back()) == transposition_table.end()) {
                        break;
                    } else {
                        curr = next.back();
                    }
                    currdepth -= 1;
                }
                //std::cerr << info.successors.front() << " next " << current_depth << " depth " << score << " score\n";
            }
        }
    }
    return alpha;
}

template <typename T, typename U>
typename Alpha_Beta<T, U>::Key_Info& Alpha_Beta<T, U>::successor_constellations(const T key)
{
    auto pos = transposition_table.find(key);
    if (pos == transposition_table.end()) {
        auto& info = transposition_table[key];
        info.successors = U::successor_constellations(key);
        return info;
    }
    if (pos->second.successors.empty()) {
        pos->second.successors = U::successor_constellations(key);
    }
    return pos->second;
}

template <typename T, typename U>
int Alpha_Beta<T, U>::evaluate(const T key)
{
    auto pos = transposition_table.find(key);
    if (pos == transposition_table.end()) {
        auto& info = transposition_table[key];
        info.score = U::evaluate(key, winning_score);
        return info.score;
    }
    if (pos->second.score < -winning_score) {
        pos->second.score = U::evaluate(key, winning_score);
    }
    return pos->second.score;
}

}

#endif // SRC_BOARDGAME_ALPHA_BETA
