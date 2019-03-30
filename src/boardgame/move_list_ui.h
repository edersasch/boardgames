#ifndef SRC_BOARDGAME_MOVE_LIST_UI
#define SRC_BOARDGAME_MOVE_LIST_UI

#include <memory>
#include <vector>
#include <string>

namespace boardgame
{

class Move_List_Ui
{
public:
    template <typename T>
    explicit Move_List_Ui(T move_list_ui) : move_list_ui_concept(std::make_shared<Move_List_Ui_Model<T>>(std::move(move_list_ui))) {}
    friend void initial_constellation(const Move_List_Ui& mui, const int constellation_id) { mui.move_list_ui_concept->ic(constellation_id); }
    friend void current_move(const Move_List_Ui& mui, const int move_id) { mui.move_list_ui_concept->cm(move_id); }
    friend void add_move(const Move_List_Ui& mui, const int move_id, const std::string& description, const std::vector<int>& hint) { mui.move_list_ui_concept->am(move_id, description, hint); }
    friend void delete_move(const Move_List_Ui& mui, const int move_id) { mui.move_list_ui_concept->dm(move_id); }
    friend void ui_cut_off(const Move_List_Ui& mui, const int move_id) { mui.move_list_ui_concept->co(move_id); }
    friend void cut_off(const Move_List_Ui& mui, const int move_id) { mui.move_list_ui_concept->co(move_id); }
    friend void need_confirm(const Move_List_Ui& mui, const bool is_needed) { mui.move_list_ui_concept->nc(is_needed); }
private:
    struct Move_List_Ui_Concept
    {
        virtual ~Move_List_Ui_Concept() = default;
        virtual void ic(const int ci) const = 0;
        virtual void cm(const int mi) const = 0;
        virtual void am(const int mi, const std::string& d, const std::vector<int>& h) const = 0;
        virtual void dm(const int id) const = 0;
        virtual void co(const int id) const = 0;
        virtual void nc(const bool in) const = 0;
    };
    template <typename T>
    struct Move_List_Ui_Model : Move_List_Ui_Concept
    {
        Move_List_Ui_Model(T move_list_ui) : Move_List_Ui_Concept(), ui(std::move(move_list_ui)) {}
        void ic(const int ci) const override { initial_constellation(ui, ci); }
        void cm(const int mi) const override { current_move(ui, mi); }
        void am(const int mi, const std::string& d, const std::vector<int>& h) const override { add_move(ui, mi, d, h); }
        void dm(const int id) const override { delete_move(ui, id); }
        void co(const int id) const override { cut_off(ui, id); }
        void nc(const bool in) const override { need_confirm(ui, in); }
        T ui;
    };
    std::shared_ptr<const Move_List_Ui_Concept> move_list_ui_concept;
};

}

#endif // SRC_BOARDGAME_MOVE_LIST_UI
