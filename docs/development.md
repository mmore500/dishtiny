# Development Overview

You probably want to [check when this page was last updated](https://github.com/mmore500/dishtiny/commits/master/docs/development.md).

::hammer_and_wrench:: ::construction_worker_woman::
If anything is unclear or out of date, please open [an issue](https://github.com/mmore500/dishtiny/issues/new) or [a pull request](https://github.com/mmore500/dishtiny/compare) so we can fix it!
Also, if you have project-specific questions or any thing else you'd like to chat about you can always feel free to [get in touch directly](mailto:m.more500@gmail.com).
I'd love to hear from you!

## Cells & Cardinals

The goal of the DISHTINY project is to power scalable digital evolution simulations of [fraternal transitions in individuality](https://en.wikipedia.org/wiki/The_Major_Transitions_in_Evolution) (a.k.a., multicellularity and eusociality).

At the most coarse-grained level, a DISHTINY simulation is just an undirected graph (in the [discrete mathematics sense of the word](https://en.wikipedia.org/wiki/Graph_theory)).
We refer to individual nodes within this graph as "cells."
A single digital organism can occupy each cell or a cell can be unoccupied.

In a graph, each node has a collection of edges that connect it to other nodes.
Likewise, in the DISHTINY simulation, each cell is adjacent to several neighbor cells.

Each cell contains an array of "cardinal" hardware units, one per neighbor cell.
(Cardinal in the sense of [cardinal directions](https://en.wikipedia.org/wiki/Cardinal_direction).)
Each cardinal manages a cell's interactions with a single neighbor cell using a single SignalGP virtual CPU.
All cardinals within a cell independently execute the same genetic program.
Cardinals units within the same cell can interact with each other via message passing.

The following schematic summarizes
1. how a single SignalGP CPU functions, executing instruction modules in response to stimuli from the environment and from other SignalGP CPUs, and
2. how SignalGP CPUs are arranged into cells in the DISHTINY model.

![hardware layout schematics](docs/assets/hardware.png)

By default, cells are arranged as a [two-dimensional toroidal grid](https://en.wikipedia.org/wiki/Torus_interconnect), basically a chess board with wraparound edges.
However, this structure can be easily reconfigured!
A DISHTINY population is just a collection of digital organisms occupying available cells.
The following visualizations show
1. cell kin groups grown on the toroidal grid (individual squares correspond to cells), and
2. a snapshot of open spawn requests on a cardinal-by-cardinal basis (each cardinal corresponds to a small triangle wedge, there are four cardinals per cell).

![population visualizations](docs/assets/population.png)

The `dish2::Cardinal` class manages the state of a single cardinal.
Likewise, the `dish2::Cell` class (which contains a vector of `dish2::Cardinal`) manages the state of a single cell.

The following schematic summarizes the class structure that organizes simulation state under the hood.
Click for a [larger, zoomable view](docs/assets/classdiagram.pdf).

[![class diagram](docs/assets/classdiagram.png)](docs/assets/classdiagram.pdf)

This diagram was generated using [Lucidchart](https://www.lucidchart.com/).
The source document can be accessed [here](https://lucid.app/lucidchart/invitations/accept/inv_04d6a5d1-75fe-4335-923d-9591ac93a707) and [here](https://lucid.app/documents/view/52847e06-54f0-428e-8181-87ab3caa353b).
Vector-format versions are available in `docs/assets`.

## Simulation Update Loop

DISHTINY performs simulation in discrete steps called updates.
During an update, each cell has an opportunity to perform tasks like executing some SignalGP code, collect resource, share resource, reproducing, etc.

These tasks are organized as discrete "services."
Each service specifies,
1. how often it should run,
2. if it should run on dead cells, live cells, or both,
3. what it should do to a cell when it runs.

Not every service runs every update!
For example, at current default settings we only process reproduction requests every 16 updates.

So, all we do during an update is loop over the cells and, within each cell, test and potentially execute each service.
We specify which services should run and in what order within the `dish2::Spec` object.
Most simulation customizations will probably involve copying and modifying a service or writing an entirely new service.

## Repository Layout

A [whistle-stop tour](https://en.wikipedia.org/wiki/Whistle_stop_train_tour) of the repository!
::steam_locomotive::

### `binder/`

```{include} ../include/dish2/binder/README.md
```

### `configpacks/`

```{include} ../include/dish2/configpacks/README.md
```

### `dishpylib/`

```{include} ../include/dish2/dishpylib/README.md
```

### `include/`

```{include} ../include/README.md
```

### `include/dish2/algorithm/`

```{include} ../include/dish2/algorithm/README.md
```

### `include/dish2/cell/`

```{include} ../include/dish2/cell/README.md
```

### `include/dish2/config/`

```{include} ../include/dish2/config/README.md
```

### `include/dish2/configbyroot/`

```{include} ../include/dish2/configbyroot/README.md
```

### `include/dish2/debug/`

```{include} ../include/dish2/debug/README.md
```

### `include/dish2/enum/`

```{include} ../include/dish2/enum/README.md
```

### `include/dish2/events/`

```{include} ../include/dish2/events/README.md
```

### `include/dish2/genome/`

```{include} ../include/dish2/genome/README.md
```

### `include/dish2/introspection/`

```{include} ../include/dish2/introspection/README.md
```

### `include/dish2/load/`

```{include} ../include/dish2/load/README.md
```

### `include/dish2/operations/`

```{include} ../include/dish2/operations/README.md
```

### `include/dish2/peripheral/`

```{include} ../include/dish2/peripheral/README.md
```

### `include/dish2/prefab/`

```{include} ../include/dish2/prefab/README.md
```

### `include/dish2/push/`

```{include} ../include/dish2/push/README.md
```

### `include/dish2/py/`

```{include} ../include/dish2/py/README.md
```

### `include/dish2/quorum/`

```{include} ../include/dish2/quorum/README.md
```

### `include/dish2/record/`

```{include} ../include/dish2/record/README.md
```

### `include/dish2/run/`

```{include} ../include/dish2/run/README.md
```

### `include/dish2/runninglog/`

```{include} ../include/dish2/runninglog/README.md
```

### `include/dish2/services/`

```{include} ../include/dish2/services/README.md
```

### `include/dish2/spec/`

```{include} ../include/dish2/spec/README.md
```

### `include/dish2/utility/`

```{include} ../include/dish2/utility/README.md
```

### `include/dish2/viz/`

```{include} ../include/dish2/viz/README.md
```

### `include/dish2/web/`

```{include} ../include/dish2/web/README.md
```

### `include/dish2/world/`

```{include} ../include/dish2/world/README.md
```

### `postprocessing/`

```{include} ../postprocessing/README.md
```

### `source/`

```{include} ../source/README.md
```

### `script/`

```{include} ../script/README.md
```

### `slurm/`

```{include} ../slurm/README.md
```

### `web/`

```{include} ../web/README.md
```

## Dependencies

Notably, this project is built on top of the [conduit](https://github.com/mmore500/conduit), [Empirical](https://github.com/devosoft/Empirical), and [signalgp-lite](https://github.com/mmore500/signalgp-lite) C++ libraries.
We use the [Emscripten compiler](https://github.com/emscripten-core/emscripten) to build the project's interactive web viewer.
For a more complete understanding of how this software works, click over to those projects and peruse their `README.md` for a sense of what they do.
