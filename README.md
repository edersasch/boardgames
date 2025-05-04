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


## Build Requirements

* C++ 17 toolchain (e.g. [gcc](https://gcc.gnu.org/) >= 7.3.0)
* [CMake](https://cmake.org) >= 3.21.0
* [Qt](https://qt.io) >= 6.4.2 with Qml
* [nlohmann/json](https://github.com/nlohmann/json/) >= 3.4.0 (license: [MIT](https://github.com/nlohmann/json/blob/v3.4.0/LICENSE.MIT))
* [martinus/robin-hood-hashing](https://github.com/martinus/robin-hood-hashing) >= 3.11.5 (license: [MIT](https://github.com/martinus/robin-hood-hashing/blob/3.8.0/LICENSE))
* [boost-ext/sml](https://github.com/boost-ext/sml) >= 1.1.3 (license: [Boost 1.0](http://www.boost.org/LICENSE_1_0.txt))


## Optional Tools

* [GTest](https://github.com/google/googletest) >= 1.16.0, downloaded automatically if option `BOARDGAMES_BUILD_TESTS` is `ON`
* [Umlet](https://www.umlet.com) UML Editor (`*.uxf` files in `/docs`)
* [Doxygen](http://www.doxygen.nl) documentation tool, enables `apidoc` build target
* [Pandoc](https://pandoc.org) enables `manuals` build target
* GCov (part of [gcc](https://gcc.gnu.org/)) / [LCov](http://ltp.sourceforge.net/coverage/lcov.php) for coverage report if option `BOARDGAMES_COVERAGE_REPORT` is `ON`


## Directory Structure

The top level structure follows [the pitchfork layout](https://api.csswg.org/bikeshed/?force=1&url=https://raw.githubusercontent.com/vector-of-bool/pitchfork/develop/data/spec.bs).


### `data/`

Contains graphics files and a valgrind memcheck suppression file [memcheck.supp](data/memcheck.supp)


### `docs/`

Manuals, [Changelog](docs/Changelog.md), and UML diagrams


### `src/`

Framework parts are located in the directories `boardgame/` and `boardgame_qml/`.
"Mühle" resides in `muehle` and `muehle_qml`.


### `tests/`

Unit tests are
[placed separately](https://api.csswg.org/bikeshed/?force=1&url=https://raw.githubusercontent.com/vector-of-bool/pitchfork/develop/data/spec.bs#src.tests.separate)
from their sources, because measuring test coverage becomes incorrect if the
test resides in the same directory as the source files. Test code coverage is
of course 100%, so the overall coverage would look better than it is. Naming
unit test files is done as described in the pitchfork paragraph
[Merged Test Placement](https://api.csswg.org/bikeshed/?force=1&url=https://raw.githubusercontent.com/vector-of-bool/pitchfork/develop/data/spec.bs#src.tests.merged).

The directories whose names match a directory name from `src/` contain unit
tests.


## Included Resources

[`CPM.cmake`](https://github.com/cpm-cmake/CPM.cmake/releases/tag/v0.40.8)
(MIT License in the file)

[`AddGoogleTest.cmake`](https://github.com/CLIUtils/cmake/blob/4e52e4d0bc2e9fd27171926d0b5d9f396dd8637c/AddGoogleTest.cmake)
([BSD 3-Clause License](https://github.com/CLIUtils/cmake/blob/master/LICENSE),
[local copy](docs/LICENSE_CLIUtils_cmake)):
modified to use CPM, remove support for older cmake versions

[`CodeCoverage.cmake`](https://github.com/bilke/cmake-modules/blob/d98828f54f6974717798e63195cfbf08fe2daad0/CodeCoverage.cmake)
(BSD 3-Clause License in the file):
modified `LCOV_FILTER_CMD` parameter from `--remove` to `--extract`, so
`setup_target_for_coverage_lcov()` parameter `EXCLUDE` is turned on its head
and will only include what is listed.

Icons from [tiddlywiki](https://tiddlywiki.com) 5.1.17
([BSD 3-Clause License](https://tiddlywiki.com/#License), [local copy](docs/LICENSE_tiddlywiki)):
modified to include `version="1.1" xmlns="http://www.w3.org/2000/svg"` in svg tag

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

Icon from [openclipart](https://openclipart.com)
([Creative Commons Zero 1.0 License](https://openclipart.org/share)):

* engine.svg ([Wireframe-Head.svg](https://openclipart.org/detail/275796/wireframe-head)): changed stroke-width to 19.0665, set width and height to 22pt

[pandoc.css](https://gist.github.com/killercup/5917178)
([Creative Commons Zero 1.0](https://gist.github.com/killercup/5917178#gistcomment-1924106))


## Development Setup with QtCreator

My personal preferences,
tested with [v16.0.1](https://github.com/qt-creator/qt-creator/releases/tag/v16.0.1)

Tip: Toggle the menu bar with [Ctrl+Alt+M](https://doc.qt.io/qtcreator/creator-how-to-show-and-hide-main-menu.html)

Build Path `~/opt/build/%{ActiveProject:Name}-%{Asciify:%{Kit:FileSystemName}-%{BuildConfig:Name}}`

`~` is the [home path](https://doc.qt.io/qt-6/qdir.html#homePath).

`opt` is used only locally, not backed up / synced

Disable user defined C++ coding style, because the
root directory has a `.clang-format` file.

Enable `prefer clang-tidy file` in
`Analyzer -> Clang Tools`, because the
root directory has a `.clang-tidy` file.

enable ccache: `-DCMAKE_C_COMPILER_LAUNCHER=ccache -DCMAKE_CXX_COMPILER_LAUNCHER=ccache`

[CPM source cache](https://github.com/cpm-cmake/CPM.cmake?tab=readme-ov-file#cpm_source_cache):
`-DCPM_SOURCE_CACHE=~/opt/build/%{ActiveProject:Name}-CPM-Source-Cache`

Do not synchronize CMakeLists.txt.user files to other machines,
if using Visual Studio also skip the .vs folder.


## Development Setup with Visual Studio

My personal preferences,
tested with v17.13.6


Cmake must have the Qt dlls in its path, because google test cmake functions
run the test executable to scan for tests.

example CMakeSettings.json

```
{
  "environments": [
    {
      "PATH": "${env.USERPROFILE}\\opt\\install\\qt\\6.9.0\\msvc2022_64\\bin;${env.PATH}"
    }
  ],
  "configurations": [
    {
      "name": "x64-Debug",
      "generator": "Ninja",
      "configurationType": "Debug",
      "inheritEnvironments": [ "msvc_x64_x64" ],
      "buildRoot": "C:\\Users\\eders\\opt\\build\\boardgames-vs-${name}",
      "installRoot": "C:\\Users\\eders\\opt\\build\\boardgames-vs-${name}\\install",
      "buildCommandArgs": "",
      "ctestCommandArgs": "",
      "variables": [
        {
          "name": "CMAKE_PREFIX_PATH",
          "value": "C:/Users/eders/opt/install/qt/6.9.0/msvc2022_64",
          "type": "PATH"
        },
        {
          "name": "CPM_SOURCE_CACHE",
          "value": "C:/Users/eders/opt/build/cpm-source-cache",
          "type": "PATH"
        }
      ]
    }
  ]
}
```

To run the executables within Visual Studio the launch.vs.json
must set the path to the Qt dlls.

example launch.vs.json

```
{
  "version": "0.2.1",
  "defaults": {},
  "environments": [
    {
      "PATH": "${env.USERPROFILE}\\opt\\install\\qt\\6.9.0\\msvc2022_64\\bin;${env.PATH}"
    }
  ]
}
```
