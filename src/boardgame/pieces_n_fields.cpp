#include "pieces_n_fields.hpp"

namespace boardgame
{

void from_json(const nlohmann::json& json, boardgame::Field_Number& fieldNumber)
{
    fieldNumber.v = json;
}

void to_json(nlohmann::json& json, const boardgame::Field_Number& fieldNumber)
{
    json = fieldNumber.v;
}

}
