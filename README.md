# Horizon EDA

Horizon EDA is an **Electronic Design Automation** package supporting an integrated end-to-end workflow for printed circuit board design including parts management and schematic entry. 

See [the docs](https://docs.horizon-eda.org/en/latest/feature-overview.html) for an overview
of horizon's top features.

Wanna chat about the project? Join #horizon-eda on libera.chat

![collage](https://horizon-eda.org/screenshots/collage.png)

# Features for users so far
- Complete design flow from schematic entry to gerber export
- Sane library management
- Unified editor for everything from symbol to board
- Netlist-aware schematic editor
- KiCad's awesome interactive router
- Lag- and glitch-free rendering
- Rule-based DRC
- Hierarchical schematics
- Undo/redo
- Copy/paste for some objects
- Builds and runs on Linux and Windows

# Features for developers
- Written in modern C++, legacy-free codebase!
- Uses JSON as on-disk format
- Uses Gtkmm3 for GUI
- OpenGL 3 accelerated rendering
- Everything is referenced by UUIDs

# Getting Started
See the [the docs](https://docs.horizon-eda.org/en/latest/installation.html).
Debian packages are hosted by [packagecloud](https://packagecloud.io/).

# Included third-party software

| Directory in `3rd_party` | Project                   | Version                                  | URL                                                                     | License      |
|--------------------------|---------------------------|------------------------------------------|-------------------------------------------------------------------------|--------------|
| nlohmann                 | JSON for Modern C++       | 3.10.3                                   | https://github.com/nlohmann/json/                                       | MIT          |
| clipper                  | Clipper                   | 6.4.2                                    | http://www.angusj.com/delphi/clipper.php                                | Boost        |
| polypartition            | polypartition             | 7bdffb428b2b19ad1c43aa44c714dcc104177e84 | https://github.com/ivanfratric/polypartition/                           | MIT          |
| poly2tri                 | poly2tri                  | d949f3cd6f85b20728af0bdc454b090226068c73 | https://github.com/jhasse/poly2tri                                      | 3-Clause BSD |
| dxflib                   | dxflib                    | 3.17.0                                   | https://qcad.org/en/90-dxflib                                           | GPLv2        |
| alphanum                 | The Alphanum Algorithm    | 1.3                                      | http://www.davekoelle.com/alphanum.html                                 | MIT          |
| delaunator               | Delaunator C++            | 6f2879967bc96a9bcdbacf418e560e9f2e170ace | https://github.com/abellgithub/delaunator-cpp                           | MIT          |
| footag                   | footag                    | 99116328abe8f53e71831b446d35e93ee7128ef3 | https://github.com/endofexclusive/footag                                | GPLv3        |
| libzippp                 | libzip++                  | ba9aec43f7760c3384bbe36f9a5454912f61307a | https://github.com/leezu/libzippp                                       | ISC          |
| router                   | KiCad router              | 5.1.6                                    | https://gitlab.com/kicad/code/kicad/-/tree/5.1.6/pcbnew/router          | GPLv3        |
| sexpr                    | KiCad s-expression parser | 5.1.6                                    | https://gitlab.com/kicad/code/kicad/-/tree/5.1.6/utils/kicad2step/sexpr | GPLv3        |


- https://github.com/russdill/pybis
