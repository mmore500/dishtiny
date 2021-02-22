# Data Overview

In order to write data files, you'll need to use the `--DATA_DUMP 1` command line flag.

## Data Files

Data file filenames look a little funny because they're named use a naming convention that allows metadata to be easily packed into and unpacked.
Usually, look for `a=` field to get a description of what's actually in a file.
You can extract key-value metadata from these filenames easily in Python using the [keyname](https://github.com/mmore500/keyname) package [hosted on PyPi](https://pypi.org/project/keyname/) and in C++ using [Empirical's header-only keyname utilities](https://github.com/devosoft/Empirical/blob/master/include/emp/tools/keyname_utils.hpp).

Each simulation thread dumps its own data files, which describe simulation state with its purview.

### Configuration Settings

* `a=asconfigured`...: a dump of runtime configuration settings, useful to keep a record of how a run was configured or to configure another run identically.

### Simulation State

* `a=birth_log`..., `a=death_log`..., `a=spawn_log`...: raw records of all cell birth, death, and spawn events that took place in the last `x` updates before the simulation ended.
* `a=cell_census`...: data describing each individual live cell's state, organized by cell index and update.
* `a=coalescence_result`...: if the population was inoculated (i.e., `-GENESIS "innoculate"`) with genome files, this file describes relative fitness of each strain.
* `a=kin_conflict_statistics`, `a=kin_conflict_by_replev_statistics`: summary of crowding conflict between cells with common versus different kin id.
* `a=demographic_phenotypic_phylogenetic_metrics`...: miscellaneous summary statistics of population state, organized by update.

### Serialized Genomes

* `a=genome`...: human-readable serialization of a single genome.
* `a=genome_statistics`...: various metrics describing the associated genome (each `a=genome` file has a corresponding `a=genome_statistics` file).
* `a=population`...: serialization of all genomes present in the population at simulation end.

### Visualizations

* `a=drawings`...: png visualizations of different aspects of simulation state.
* `a=montage`...: summary graphic combining several different visualizations.

### Data File Types

`*.bin` files contain binary-format serialization data generated using the [Cereal C++ Library](https://github.com/USCiLab/cereal).
These aren't portable or human-readable.

`*.csv` files are [comma-separated value tables](https://en.wikipedia.org/wiki/Comma-separated_values).
You should be able to open these with your preferred scripting language or spreadsheet program.

`*.gz` files are [gzipped](https://en.wikipedia.org/wiki/Gzip).
To work with these files, you'll need to use [`gunzip`](https://linux.die.net/man/1/gunzip) to uncompress them.

`*.json` files are formatted using [JavaScript Object Notation](https://en.wikipedia.org/wiki/JSON).
Your preferred scripting language should have [some tools](https://docs.python.org/3/library/json.html) to open these.

`*.meta` files contain metadata corresponding to each file.
You can ignore them or archive them alongside their corresponding data file for a more complete record of your experiments.

`*.tar` files are [tarballs](https://en.wikipedia.org/wiki/Tar_(computing)).
To wok with these files, you'll need to use [`tar`](https://linux.die.net/man/1/tar) to extract them.

## Data Repository

Our evolution experiments upload data to the [Open Science Framework](https://osf.io/) *as they run*.
This means that our data is available hot off the presses! ::hotsprings::
You can find peruse and download at <https://osf.io/9vkd7/>.
The [Center for Open Science](https://www.cos.io/) also provides a Python tool called [osfclient](https://github.com/osfclient/osfclient) that allows programatic file downloads.

If you have interesting ideas of hypotheses to test or further experiments to run with this data, [get in touch](mailto:m.more500@gmail.com) to collaborate --- or, if you prefer, take the data and run with it on your own!
(I'd still love to hear about what you're up to and what you find ::smile::.)

Inside the `repro/` folder, you'll find logs from our Slurm jobs --- not much of scientific interest to analyze here, but useful to verify how data was generated and troubleshoot technical issues.

At the most granular level, data is organized by `endeavor`.
This groups sets of evolutionary runs that are generated using the same software source and are part of a unified experimental design.
Probably, you only want to use data from one endeavor at a time.

We perform an evolutionary run as a chain of four hour Slurm jobs.
Except the first (which generates a random population), each job downloads and opens the preceding job's population, performs four hours of evolutionary simulation, then saves out and uploads data files and its population.
Lastly, each job submits the next job in the chain.
(This process is somewhat analogous to the daily flask transfer in the [E. coli long-term evolution experiment](https://en.wikipedia.org/wiki/E._coli_long-term_evolution_experiment)).

We concurrently run an array of entirely independent evolutionary chains.
Currently, we run 40 independent chains.
(These independent evolutionary chains are somewhat analogous to the twelve independent populations in the [E. coli long-term evolution experiment](https://en.wikipedia.org/wiki/E._coli_long-term_evolution_experiment))

Within each `endeavor`, the `evolve/` directory hosts data files generated by these runs.
These data files are organized and labeled according to `stint` --- the index within a successive daisy-chain of evolutionary jobs --- and `series` --- the index of the job's evolutionary chain among independent replicates.

The `genomes/` directory contains human-readable serialized genomes.
Evolutionary chain jobs upload genomes (one arbitrarily chosen, one chosen according to prevalence) here.
Genomes generated in follow-on analyses (for example, [nopping out](https://en.wikipedia.org/wiki/NOP_(code)) phenotypically-neutral sites or fitness-neutral sites) end up here, too.

The `strain-competition/` directory contains data from competition experiments between genomes from different `series`.
The `strain-fitness/` directory contains summary fitness statistics derived from the results of inter-strain competition experiments.

The `variant-competition/` directory contains data from competition experiments between wild-type and corresponding single instruction nop-out variants.
Wild-type versus wild-type competition results, used as controls, are also stored here.
