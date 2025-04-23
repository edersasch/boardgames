#if defined _MSC_VER
#if defined BOARDGAME_LIB
#define BOARDGAME_LIB_EXPORT __declspec(dllexport)
#else
#define BOARDGAME_LIB_EXPORT __declspec(dllimport)
#endif
#else
#define BOARDGAME_LIB_EXPORT
#endif