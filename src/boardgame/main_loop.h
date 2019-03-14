#ifndef SRC_BOARDGAME_MAIN_LOOP
#define SRC_BOARDGAME_MAIN_LOOP

#include <future>
#include <utility>
#include <chrono>

namespace boardgame
{

class Main_Loop
{
public:
    template <typename T>
    explicit Main_Loop(T main_loop) : main_loop_concept(std::make_shared<Main_Loop_Model<T>>(std::move(main_loop))) {}
    friend void engine_future(const Main_Loop& ml, std::future<bool>&& efu) { ml.main_loop_concept->ef(std::move(efu)); }
    friend void set_main_loop_engine_time(const Main_Loop& ml, std::chrono::seconds time_in_s) { ml.main_loop_concept->st(time_in_s); }
private:
    struct Main_Loop_Concept
    {
        virtual ~Main_Loop_Concept() = default;
        virtual void ef(std::future<bool>&& efu) const = 0;
        virtual void st(std::chrono::seconds t) const = 0;
    };
    template <typename T>
    struct Main_Loop_Model : Main_Loop_Concept
    {
        Main_Loop_Model(T main_loop) : Main_Loop_Concept(), ml(std::move(main_loop)) {}
        void ef(std::future<bool>&& efu) const override { engine_future(ml, std::move(efu)); }
        void st(std::chrono::seconds t) const override { set_main_loop_engine_time(ml, t); }
        T ml;
    };
    std::shared_ptr<const Main_Loop_Concept> main_loop_concept;
};

}

#endif // SRC_BOARDGAME_MAIN_LOOP
