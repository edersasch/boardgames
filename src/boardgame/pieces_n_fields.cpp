#include "pieces_n_fields.h"

namespace boardgame
{

void from_json(const nlohmann::json& j, boardgame::Field_Number& d)
{
    d.v = j;
}

void to_json(nlohmann::json& j, const boardgame::Field_Number& d)
{
    j = d.v;
}

}
