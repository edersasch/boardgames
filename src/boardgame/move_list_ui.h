#ifndef SRC_BOARDGAME_MOVE_LIST_UI
#define SRC_BOARDGAME_MOVE_LIST_UI

#include <memory>
#include <vector>
#include <string>

class Move_List_Ui
{
public:
    template <typename T>
    explicit Move_List_Ui(T mui) : mC(std::make_shared<Move_List_Ui_Model<T>>(std::move(mui))) {}
    friend void initial_constellation(const Move_List_Ui& mui, const int constellation_id) { mui.mC->ic(constellation_id); }
    friend void current_move(const Move_List_Ui& mui, const int move_id) { mui.mC->cm(move_id); }
    friend void add_move(const Move_List_Ui& mui, const int move_id, int branch_id, const std::string& description, const std::vector<int>& hint) { mui.mC->am(move_id, branch_id, description, hint); }
    friend void delete_move(const Move_List_Ui& mui, const int move_id) { mui.mC->dm(move_id); }
    friend void ui_cut_off(const Move_List_Ui& mui, const int move_id) { mui.mC->co(move_id); }
    friend void cut_off(const Move_List_Ui& mui, const int move_id) { mui.mC->co(move_id); }
private:
    struct Move_List_Ui_Concept
    {
        virtual ~Move_List_Ui_Concept() = default;
        virtual void ic(const int ci) const = 0;
        virtual void cm(const int mi) const = 0;
        virtual void am(const int mi, const int bi, const std::string& d, const std::vector<int>& h) const = 0;
        virtual void dm(const int id) const = 0;
        virtual void co(const int id) const = 0;
    };
    template <typename T>
    struct Move_List_Ui_Model : Move_List_Ui_Concept
    {
        Move_List_Ui_Model(T mui) : Move_List_Ui_Concept(), mM(std::move(mui)) {}
        void ic(const int ci) const override { initial_constellation(mM, ci); }
        void cm(const int mi) const override { current_move(mM, mi); }
        void am(const int mi, const int bi, const std::string& d, const std::vector<int>& h) const override { add_move(mM, mi, bi, d, h); }
        void dm(const int id) const { delete_move(mM, id); }
        void co(const int id) const { cut_off(mM, id); }
        T mM;
    };
    std::shared_ptr<const Move_List_Ui_Concept> mC;
};

#endif // SRC_BOARDGAME_MOVE_LIST_UI
