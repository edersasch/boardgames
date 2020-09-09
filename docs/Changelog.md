# Changelog

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).
This project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).


## 0.1.1 "Hans" - unreleased

Game info can be viewed, engine is faster, but needs more RAM. The project is
now checked by [lgtm.com](https://lgtm.com/projects/g/edersasch/boardgames/).

The release is named after two persons: my uncle, who passed away in 2009 and
and my mother's friend, whose journey ended in 2006.


### Added

- game info: number of moves, time per player, engine forecast
- transposition table for engine using [martinus/robin-hood-hashing](https://github.com/martinus/robin-hood-hashing)
- english manual, chosen according to system language


### Changed

- put version info in toplevel CMakeLists.txt, show in manual
- clicking engine settings icons selects engine mode
- Muehle_State uses [boost-ext/sml](https://github.com/boost-ext/sml) state machine framework


### Fixed

- lgtm.com alerts for 7f1ad0e
- move list scrolling to highlighted button


## 0.1.0 "Klaus" - 2019-04-16

The first five big commits up to d1c4d8b served to find a good design and
naming. The other commits are also larger than they would have been when
developing software in a team, but with only one developer and no reviewer,
this granularity is ok.

The release is named after my father-in-law who passed away in 2018.


### Added

- framework with piece / field association, move list, engine and UI
- Mühle (German name for nine Men's Morris or Mill)
