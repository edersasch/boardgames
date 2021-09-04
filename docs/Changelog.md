# Changelog

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).
This project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).


## 0.1.1 "Hans" - unreleased

Game info can be viewed, engine is faster, but needs a lot of RAM.
Muehle game is draw if 50 consecutive moves without capturing a piece are
played or the same constellation occurs three times.
The project is now checked by [lgtm.com](https://lgtm.com/projects/g/edersasch/boardgames/).
The software can be built for windows.

The release is named after two persons: my uncle, who passed away in 2009 and
and my mother's friend, whose journey ended in 2006.


### Added

- game info: number of moves, time per player, engine forecast
- transposition table for engine using [martinus/robin-hood-hashing](https://github.com/martinus/robin-hood-hashing)
- english manual, chosen according to system language
- windows visual studio build support
- draw muehle game after 50 consecutive boring moves or three times constellation repetition
- tools/muehle_installer.sh for linux


### Changed

- put version info in toplevel CMakeLists.txt, show in manual
- clicking engine settings icons selects engine mode
- Muehle_State uses [boost-ext/sml](https://github.com/boost-ext/sml) state machine framework
- requires cmake 3.11 for FetchContent, no need for DownloadProject any more
- evaluation uses only the difference in number of destination fields
- field is an svg file, also used as application icon
- appimage build is based on ubuntu bionic


### Fixed

- lgtm.com alerts for 7f1ad0e
- move list scrolling to highlighted button
- clang tidy findings


## 0.1.0 "Klaus" - 2019-04-16

The first five big commits up to d1c4d8b served to find a good design and
naming. The other commits are also larger than they would have been when
developing software in a team, but with only one developer and no reviewer,
this granularity is ok.

The release is named after my father-in-law who passed away in 2018.


### Added

- framework with piece / field association, move list, engine and UI
- Mühle (German name for nine Men's Morris or Mill)
