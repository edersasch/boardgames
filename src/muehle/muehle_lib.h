#if defined _MSC_VER
#if defined MUEHLE_LIB
#define MUEHLE_LIB_EXPORT __declspec(dllexport)
#else
#define MUEHLE_LIB_EXPORT __declspec(dllimport)
#endif
#else
#define MUEHLE_LIB_EXPORT
#endif