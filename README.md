[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/edersasch/boardgames.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/edersasch/boardgames/context:cpp)


# Boardgames

* C++ 17 framework with piece / field association, move list, engine and UI
* Mühle (German name for nine Men's Morris or Mill) [Manual](docs/manuals/muehle_en.md) [Anleitung](docs/manuals/muehle_de.md)
* Available under [MIT license](LICENSE)


## Framework UI Independent

* Interface `boardgame_ui` for commands from the game to its user interface
* Interface `move_list_ui` for commands from the provided `move_list` implementation to its user interface
* Interface `main_loop` to delegate tasks to the utilized main loop implementation
* Engine implementation `alpha_beta` (depends on martinus/robin-hood-hashing, see "Build Requirements" below)
* Move list implementation `move_list`, supporting branches and json import / export (depends on nlohmann/json, see "Build Requirements" below)
* Piece and field functions `pieces_n_fields` that help manage associations; defines strong types to prevent mixing up field and piece ids


## Framework UI

depends on Qt, see "Build Requirements" below

* `Confirm_Button` widget
* `Field` and `Piece`, `Piece_Column_Box`, `Piece_Row_Box`
* `multi_move_list`, `move_list` with `Move_Button`, `Move_List_Control`, `Move_List_Entry`, `Move_List_Root`


## Mühle UI Independent

* `muehle` with constants, free functions and engine methods
* `muehle_state` class (depends on boost-ext/sml, see "Build Requirements" below)


## Mühle UI

depends on Qt, see "Build Requirements" below

* `Muehle_Board`
* `muehle` entry point and setup


# Build Requirements

* C++ 17 toolchain (e.g. [gcc](https://gcc.gnu.org/) >= 7.3.0)
* [CMake](https://cmake.org) >= 3.14.0
* [Qt](https://qt.io) >= 5.15.0 with Qml
* [nlohmann/json](https://github.com/nlohmann/json/) >= 3.4.0, downloaded automatically if option `BOARDGAMES_USE_EXTERNAL_JSON` is `OFF` (license: [MIT](https://github.com/nlohmann/json/blob/v3.4.0/LICENSE.MIT))
* [martinus/robin-hood-hashing](https://github.com/martinus/robin-hood-hashing) >= 3.11.5, downloaded automatically if option `BOARDGAMES_USE_EXTERNAL_ROBIN_HOOD_HASH` is `OFF` (license: [MIT](https://github.com/martinus/robin-hood-hashing/blob/3.8.0/LICENSE))
* [boost-ext/sml](https://github.com/boost-ext/sml) >= 1.1.3, downloaded automatically if option `BOARDGAMES_USE_EXTERNAL_SML` is `OFF` (license: [Boost 1.0](http://www.boost.org/LICENSE_1_0.txt))


# Optional Tools

* [GTest](https://github.com/google/googletest) >= 1.12.1, downloaded automatically if option `BOARDGAMES_BUILD_TESTS` is `ON`
* [Umlet](https://www.umlet.com) UML Editor (`*.uxf` files in `/docs`)
* [Doxygen](http://www.doxygen.nl) documentation tool, enables `apidoc` build target
* [Pandoc](https://pandoc.org) enables `manuals` build target
* GCov (part of [gcc](https://gcc.gnu.org/)) / [LCov](http://ltp.sourceforge.net/coverage/lcov.php) for coverage report if option `BOARDGAMES_COVERAGE_REPORT` is `ON`


# Directory Structure

The top level structure follows [the pitchfork layout](https://api.csswg.org/bikeshed/?force=1&url=https://raw.githubusercontent.com/vector-of-bool/pitchfork/develop/data/spec.bs).


## `data/`

Contains graphics files and a valgrind memcheck suppression file [memcheck.supp](data/memcheck.supp)


## `docs/`

Manuals, [Changelog.md](docs/Changelog.md) and UML diagrams


## `src/`

Framework parts are located in the directories `boardgame/` and `boardgame_qml/`.
"Mühle" resides in `muehle` and `muehle_qml`.


## `tests/`

Unit tests are
[placed separately](https://api.csswg.org/bikeshed/?force=1&url=https://raw.githubusercontent.com/vector-of-bool/pitchfork/develop/data/spec.bs#src.tests.separate)
from their sources, because measuring test coverage becomes incorrect if the
test resides in the same directory as the source files. Test code coverage is
of course 100%, so the overall coverage would look better than it is. Naming
unit test files is done as described in the pitchfork paragraph
[Merged Test Placement](https://api.csswg.org/bikeshed/?force=1&url=https://raw.githubusercontent.com/vector-of-bool/pitchfork/develop/data/spec.bs#src.tests.merged).

The directories whose names match a directory name from `src/` contain unit
tests.


# Included Resources

CMake file `AddGoogleTest.cmake` adapted from
<https://github.com/CLIUtils/cmake/blob/4e52e4d0bc2e9fd27171926d0b5d9f396dd8637c/AddGoogleTest.cmake>
is licensed under a
[BSD 3-Clause License](https://github.com/CLIUtils/cmake/blob/master/LICENSE) ([local copy](docs/LICENSE_CLIUtils_cmake)):
use URL instead of GIT

CMake file `CodeCoverage.cmake` adapted from
<https://github.com/bilke/cmake-modules/blob/d98828f54f6974717798e63195cfbf08fe2daad0/CodeCoverage.cmake>
is licensed under a BSD 3-Clause License included in the file itself: changed
`LCOV_FILTER_CMD` parameter from `--remove` to `--extract`, so
`setup_target_for_coverage_lcov()` parameter `EXCLUDE` is turned on its head
and will only include what is listed.

CMake file `DeployQt.cmake` adapted from <https://github.com/nitroshare/nitroshare-desktop/blob/f4feebef29d9d3985d1699ab36f0fac59d3df7da/cmake/DeployQt.cmake>
is licensed under MIT License included in the file itself: added QML dirs

Icons adapted from [tiddlywiki](https://tiddlywiki.com) 5.1.17 are licensed
under a [BSD 3-Clause License](https://tiddlywiki.com/#License) ([local copy](docs/LICENSE_tiddlywiki)): added
`version="1.1" xmlns="http://www.w3.org/2000/svg"` to svg tag

* back.svg ([$:/core/images/left-arrow](https://tiddlywiki.com/#%24%3A%2Fcore%2Fimages%2Fleft-arrow))
* cancel.svg ([$:/core/images/cancel-button](https://tiddlywiki.com/#%24%3A%2Fcore%2Fimages%2Fcancel-button))
* delete.svg ([$:/core/images/delete-button](https://tiddlywiki.com/#%24%3A%2Fcore%2Fimages%2Fdelete-button))
* depth.svg ([$:/core/images/line-width](https://tiddlywiki.com/#%24%3A%2Fcore%2Fimages%2Fline-width))
* export.svg ([$:/core/images/unfold-button](https://tiddlywiki.com/#%24%3A%2Fcore%2Fimages%2Funfold-button))
* forward.svg ([$:/core/images/right-arrow](https://tiddlywiki.com/#%24%3A%2Fcore%2Fimages%2Fright-arrow))
* info.svg ([$:/core/images/info-button](https://tiddlywiki.com/#%24%3A%2Fcore%2Fimages%2Finfo-button))
* help.svg ([$:/core/images/help](https://tiddlywiki.com/#%24%3A%2Fcore%2Fimages%2Fhelp))
* hide.svg ([$:/core/images/preview-closed](https://tiddlywiki.com/#%24%3A%2Fcore%2Fimages%2Fpreview-closed))
* home.svg ([$:/core/images/home-button](https://tiddlywiki.com/#%24%3A%2Fcore%2Fimages%2Fhome-button))
* import.svg ([$:/core/images/fold-button](https://tiddlywiki.com/#%24%3A%2Fcore%2Fimages%2Ffold-button))
* list.svg ([$:/core/images/list-bullet](https://tiddlywiki.com/#%24%3A%2Fcore%2Fimages%2Flist-bullet))
* refresh.svg ([$:/core/images/refresh-button](https://tiddlywiki.com/#%24%3A%2Fcore%2Fimages%2Frefresh-button))
* setup.svg ([$:/core/images/copy-clipboard](https://tiddlywiki.com/#%24%3A%2Fcore%2Fimages%2Fcopy-clipboard))
* settings.svg ([$:/core/images/options-button](https://tiddlywiki.com/#%24%3A%2Fcore%2Fimages%2Foptions-button))
* show.svg ([$:/core/images/preview-open](https://tiddlywiki.com/#%24%3A%2Fcore%2Fimages%2Fpreview-open))
* time.svg ([$:/core/images/timestamp-on](https://tiddlywiki.com/#%24%3A%2Fcore%2Fimages%2Ftimestamp-on))
* warning.svg ([$:/core/images/warning](https://tiddlywiki.com/#%24%3A%2Fcore%2Fimages%2Fwarning))

Icon taken from [openclipart](https://openclipart.com) is licensed under
[Creative Commons Zero 1.0 Public Domain License](https://openclipart.org/share):

* engine.svg ([Wireframe-Head.svg](https://openclipart.org/detail/275796/wireframe-head)): changed stroke-width to 19.0665, set width and height to 22pt

pandoc.css taken from <https://gist.github.com/killercup/5917178> is most likely licensed under
[Creative Commons Zero 1.0 Public Domain License](https://gist.github.com/killercup/5917178#gistcomment-1924106)
