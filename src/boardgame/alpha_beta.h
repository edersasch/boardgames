#ifndef SRC_BOARDGAME_ALPHA_BETA
#define SRC_BOARDGAME_ALPHA_BETA

#include <iostream>

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
    T get_next() { return next; }
    bool is_running() { return running; }
    void stop_running() { running = false; }
    void discard();
    void set_target_depth(int depth);
private:
    void iterative_depth(const T key);
    int engine(const T key, const int depth, int alpha, const int beta);
    static constexpr int target_depth_default {9999};
    int target_depth {target_depth_default};
    int current_depth {0};
    T possible_next;
    T next;
    No_Move_Policy no_move_policy {No_Move_Policy::lose};
    bool running {false};
    bool valid_move {true};

    int debug_cut {0};
};

template <typename T, typename U>
bool Alpha_Beta<T, U>::start(const T key)
{
    if (running) {
        std::cerr << "alpha beta engine start, but running\n";
    }
    std::cerr << "4  1  87      0         0      3  0         0         0\n";
    std::cerr << key << " before\n";
    running = true;
    iterative_depth(U::switch_player(key));
    running = false;
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
    for (current_depth = 1; current_depth <= target_depth; current_depth += 1) {
        debug_cut = 0;
        auto score = engine(key, current_depth, -winning_score, winning_score);
        if (running) {
            next = possible_next;
        }
        if (!running || score == winning_score || score == -winning_score) {
            break;
        }
    }
    std::cerr << current_depth - (running ? 1 : 0) << " depth " << debug_cut << " cut\n";
}

template <typename T, typename U>
int Alpha_Beta<T, U>::engine(const T key, const int depth, int alpha, const int beta)
{
    if (depth == 0) {
        return U::evaluate(key, winning_score);
    }
    auto successors = U::successor_constellations(key);
    if (successors.empty()) {
        switch(no_move_policy) {
        case No_Move_Policy::lose:
            return -winning_score;
        default:
            return alpha;
        }
    }
    for (const auto& n : successors) {
        if (!running) {
            break;
        }
        auto score = -engine(U::switch_player(n), depth - 1, -beta, -alpha);
        if (score > alpha) {
            alpha = score;
            if (alpha >= beta && beta < winning_score) {
                debug_cut += 1;
                break;
            }
            if (depth == current_depth && running) {
                std::cerr << n << " next " << current_depth << " depth " << score << " score\n";
                possible_next = n;
            }
        }
    }
    return alpha;
}

}

#endif // SRC_BOARDGAME_ALPHA_BETA
