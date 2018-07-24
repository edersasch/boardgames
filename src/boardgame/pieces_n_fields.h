#ifndef SRC_BOARDGAME_PIECES_N_FIELDS
#define SRC_BOARDGAME_PIECES_N_FIELDS

#include <algorithm>
#include <vector>
#include <array>
#include <tuple>
#include <type_traits>
#include <unordered_map>

namespace boardgame {

namespace detail {

template <typename T>
struct ElementNumber { int v {-1}; }; // default constructed means unset
template <typename T>
constexpr bool operator==(const ElementNumber<T>& lhs, const ElementNumber<T>& rhs) { return lhs.v == rhs.v; }
template <typename T>
constexpr bool operator!=(const ElementNumber<T>& lhs, const ElementNumber<T>& rhs) { return lhs.v != rhs.v; }

template <typename T>
struct ElementRange { int v; };
template <typename T>
constexpr bool operator==(const ElementRange<T>& lhs, const ElementRange<T>& rhs) { return lhs.v == rhs.v; }

template <typename T>
constexpr ElementNumber<T> operator+(ElementNumber<T> lhs, const ElementRange<T> rhs)
{
    lhs.v += rhs.v;
    return lhs;
}

template <typename T>
constexpr ElementRange<T> operator+(ElementRange<T> lhs, const ElementRange<T> rhs)
{
    lhs.v += rhs.v;
    return lhs;
}

template <typename T, typename U>
struct Elementgroup { const T b, e, o; const U tag; };

template <typename T, typename U>
constexpr auto make_elementgroup(const T& elements, const ElementNumber<U> start, const ElementRange<U> range)
{
    auto o = std::cbegin(elements); // origin
    auto b = o + start.v;           // begin of group
    auto e = b + range.v;           // end of group
    return Elementgroup<decltype(b), U>{b, e, o, {}};
}

template <typename T>
constexpr bool is_set(const T& elem)
{
    return elem != T{};
}

template <typename T>
constexpr bool is_unset(const T& elem)
{
    return elem == T{};
}

template <typename T>
constexpr auto first_set_element(const T& elemgr)
{
    auto found = std::find_if_not(elemgr.b, elemgr.e, &is_unset<decltype(*elemgr.b)>);
    return found == elemgr.e ? decltype(*elemgr.b){} : *found;
}

template <typename T>
constexpr auto first_unset_element(const T& elemgr)
{
    const auto found = std::find(elemgr.b, elemgr.e, decltype(*elemgr.b){});
    return found == elemgr.e ? ElementNumber<decltype(elemgr.tag)>{} : ElementNumber<decltype(elemgr.tag)>{static_cast<int>(found - elemgr.o)}; // narrowing is ok as long as you don't have more than 2 million elements
}

template <typename T>
constexpr auto number_of_set_elements(const T& elemgr)
{
    return std::count_if(elemgr.b, elemgr.e, &is_set<decltype(*elemgr.b)>);
}

template <typename T, typename U>
constexpr auto is_in_group(const T& elem, const U& elemgr)
{
    int first = elemgr.b - elemgr.o;
    int last = elemgr.e - elemgr.o;
    return elem.v >= first && elem.v < last;
}

template <typename T>
auto all_unset_elements(const T& elemgr)
{
    std::vector<ElementNumber<decltype(elemgr.tag)>> found;
    for (auto b = elemgr.b; b != elemgr.e; b += 1) {
        if (is_unset(*b)) {
            found.push_back(ElementNumber<decltype(elemgr.tag)>{static_cast<int>(b - elemgr.o)}); // narrowing is ok as long as you don't have more than 2 million elements
        }
    }
    return found;
}

template <typename T, typename U>
auto filter_elements_in_group(const T& elemgr1, const U& elemgr2)
{
    std::vector<std::decay_t<decltype(*elemgr1.b)>> found;
    int first = elemgr2.b - elemgr2.o;
    int last = elemgr2.e - elemgr2.o;
    std::copy_if(elemgr1.b, elemgr1.e, std::back_inserter(found), [first, last](auto elem) { return elem.v >= first && elem.v < last; });
    return found; // contains values of elemgr1 in the range of elemgr2
}

}

struct PieceTag {};
using PieceNumber = detail::ElementNumber<const PieceTag>;
static constexpr PieceNumber noPiece; // default constructed means unset
using PieceRange = detail::ElementRange<const PieceTag>;
template <typename T>
using Piecegroup = detail::Elementgroup<T, const PieceTag>;

template <typename T>
constexpr auto make_piecegroup(const T& pieces, const PieceNumber start, const PieceRange range)
{
    return make_elementgroup(pieces, start, range);
}

struct FieldTag {};
using FieldNumber = detail::ElementNumber<const FieldTag>;
static constexpr FieldNumber noField; // default constructed means unset
using FieldRange = detail::ElementRange<const FieldTag>;
template <typename T>
using Fieldgroup = detail::Elementgroup<T, const FieldTag>;

template <typename T>
constexpr auto make_fieldgroup(const T& fields, const FieldNumber start, const FieldRange range)
{
    return make_elementgroup(fields, start, range);
}

template <typename T>
constexpr auto first_piece(const T& fieldgroup)
{
    return first_set_element(fieldgroup);
}

template <typename T>
constexpr bool is_fieldgroup_empty(const T& fieldgroup)
{
    return first_piece(fieldgroup) == noPiece;
}

template <typename T>
constexpr FieldNumber first_empty_field(const T& fieldgroup)
{
    return first_unset_element(fieldgroup);
}

template <typename T>
constexpr int number_of_pieces_in_fieldgroup(const T& fieldgroup)
{
    return number_of_set_elements(fieldgroup);
}

template <typename T>
auto all_free_fields(const T& fieldgroup)
{
    return all_unset_elements(fieldgroup);
}

template <typename T, typename U>
auto positions_of_pieces_in_fieldgroup(const T& piecegroup, const U& fieldgroup)
{
    return filter_elements_in_group(piecegroup, fieldgroup);
}

using Diff = std::vector<std::pair<boardgame::PieceNumber, std::array<FieldNumber, 2>>>;

template <typename T>
Diff diff(const T& first, const T& second)
{
    Diff diffed;
    auto [f, s] = std::mismatch(first.begin(), first.end(), second.begin());
    while (s != second.end()) {
        diffed.push_back({PieceNumber{static_cast<int>(s - second.begin())}, {{*f, *s}}}); // narrowing ok if less than 2 million pieces
        f += 1;
        s += 1;
        std::tie(f, s) = std::mismatch(f, first.end(), s);
    }
    return diffed;
}

template <typename T>
Diff diff (const T& move_list, const int id)
{
    int prev = move_list.prev();
    if (prev == T::invalid_id) {
        return {};
    }
    auto target = constellation(move_list.constellation(id));
    auto before = constellation(move_list.constellation(prev));
    return diff(before, target);
}

}

#endif // SRC_BOARDGAME_PIECES_N_FIELDS
