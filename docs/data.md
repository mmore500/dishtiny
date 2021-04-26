# Data Overview

This page overviews data output from DISHTINY experiments across two contexts,
1. generated on your own machine using the DISHTINY executable, and
2. generated on a compute cluster and uploaded to S3 using DISHTINY scripts.

You probably want to [check when this page was last updated](https://github.com/mmore500/dishtiny/commits/master/docs/workflow.md).

::hammer_and_wrench:: ::construction_worker_woman::
If anything is unclear or out of date, please open [an issue](https://github.com/mmore500/dishtiny/issues/new) or [a pull request](https://github.com/mmore500/dishtiny/compare) so we can fix it!
Also, if you have project-specific questions or any thing else you'd like to chat about you can always feel free to [get in touch directly](mailto:m.more500@gmail.com).
I'd love to hear from you!

## Data Files

This section covers running the dishtiny executable yourself to generate your own data.

In order to write data files, you'll need to use the `--DATA_DUMP 1` command line flag.

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

`*.xz` files are [xzipped](https://en.wikipedia.org/wiki/XZ_Utils).
To work with these files, you'll need to use [`xz --decompress`](https://linux.die.net/man/1/xz) to uncompress them.

`*.json` files are formatted using [JavaScript Object Notation](https://en.wikipedia.org/wiki/JSON).
Your preferred scripting language should have [some tools](https://docs.python.org/3/library/json.html) to open these.

`*.meta` files contain metadata corresponding to each file.
You can ignore them or archive them alongside their corresponding data file for a more complete record of your experiments.

`*.tar` files are [tarballs](https://en.wikipedia.org/wiki/Tar_(computing)).
To wok with these files, you'll need to use [`tar`](https://linux.die.net/man/1/tar) to extract them.

## Data Repository

This section covers using data generated from existing dishtiny experiments.

If you have interesting ideas of hypotheses to test or further experiments to run with this data, [get in touch](mailto:m.more500@gmail.com) to collaborate --- or, if you prefer, take the data and run with it on your own!
(I'd still love to hear about what you're up to and what you find ::smile::.)

### Hassle-free Pre-wrangled Data via Web GUI ::cowboy::

We host a pre-configured Jupyter notebook for each of our consolidated datasets on <https://mybinder.org>.
You can jump into them [here](https://mybinder.org/v2/gh/mmore500/dishtiny/binder?filepath=binder%2Findex.ipynb).

Right now, the binder environment is stocked with a few Python data analysis and visualization tools.
Binder also has other scripting languages like R and Julia on tap, though!
If you don't see your preferred tools or packages in the binder, make a pull request, open an issue, or [send me an email](mailto:m.more500@gmail.com) and we'll make sure they're there.

### Spelunking in the OSF Data Repository

Our evolution experiments upload all data to the [Open Science Framework](https://osf.io/) *as they run*.
This means that our data is available hot off the presses! ::hotsprings::
You can find peruse and download at <https://osf.io/9vkd7/>.
The [Center for Open Science](https://www.cos.io/) also provides a Python tool called [osfclient](https://github.com/osfclient/osfclient) that allows programatic file downloads.

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
(These independent evolutionary chains are somewhat analogous to the twelve independent populations in the [E. coli long-term evolution experiment](https://en.wikipedia.org/wiki/E._coli_long-term_evolution_experiment).)

Here's an overview of what's within each `endeavor` folder.

#### `control-competitions/` directory

Contains results from wild-type versus wild-type competitions, which are used as controls to determine if other competitions have a clear winner.

#### `evolve/` directory

Contains data files directly generated by the evolutionary chains.
These data files are organized and labeled according to `stint` --- the index within a successive daisy-chain of evolutionary jobs --- and `series` --- the index of the job's evolutionary chain among independent replicates.

#### `genomes/` directory

Contains human-readable serialized genomes.
Evolutionary chain jobs upload genomes (one arbitrarily chosen, one chosen according to prevalence) here.
Genomes generated in follow-on analyses (for example, [nopping out](https://en.wikipedia.org/wiki/NOP_(code)) phenotypically-neutral sites or fitness-neutral sites) end up here, too.

#### `monocultures/` directory

Contains data from monoculture experiments, where a single genome extracted from an evolutionary chain is grown in isolation with mutation disabled in order to study its phenotypic characteristics.

#### `mutant-competitions/` directory

Contains data from competition experiments between a wild-type strain and a variant of that strain after the mutation operator has been applied.
Used to characterize the effects of a single mutation event on fitness for particular strains.

#### `mutant-phenotype-differentiation/` directory

Contains data from side-by-side comparison experiments to determine if any phenotypic difference exists between a wild-type strain and a variant of that strain after the mutation operator has been applied.

#### `mutating-competitions/` directory

Contains data from competition experiments between a wild-type strain with ongoing mutations disabled and the same wild-type strain with ongoing mutations enabled.
Used to characterize the effects of ongoing mutation on fitness for particular strains.

#### `noncritical-nopout-competitions/` directory

Contains data from competition experiments between a wild-type strain and a variant of the strain with nopout of sites that do not detectably decrease fitness when individually nopped out.
Used to assess potential functionality of noncritical sites.

#### `noncritical-phenotypeneutral-nopinterpolation-competitions` directory

A noncritical nopout variant refers to a genome with nopout of all sites that do not detectably decrease fitness when individually nopped out.
A phenotype neutral variant refers to a gnome with nopout of as many sites as possible without affecting phenotype (kind of "whittled down").

In order to understand the role of noncritical sites that do affect the phenotype, we need to study intermediate genomes between these two poles.
Because the noncritical nopout variant should have strictly *more* sites nopped out, we can generate intermediate genomes by nopping out sites that the phenotype neutral nopout variant contains but the noncritical nopout variant does not.

This directory contains competition experiments between a wild-type strain and variants that interpolate between the noncritical nopout variant and the phenotype neutral nopout variant.

#### `perturbation-...-state-...-competitions` directory

Contains data from competition experiments between a wild-type strain and variants of that strain where elements of virtual CPU input or output are disabled.
Used to assess how many independent inputs and outputs a strain is relying on.

#### `phenotype-neutral-nopout-competitions` directory

Contains data from competition experiments between a wild-type strain and its phenotype-neutral nopout variant.
A sanity check that phenotype neutral nopout variants should indeed also be fitness neutral.

#### `phenotype-neutral-nopout-phenotype-differentiation-competitions` directory

Contains data from side-by-side comparison experiments to determine if any phenotypic difference exists between a wild-type strain and its phenotype-neutral nopout variant.
A sanity check that phenotype neutral nopout variants should indeed be phenotypicaly neutral.

#### `predecessor-battles` directory

Contains data from comparison experiments between a population and its ancestor population from 10 stints ago.
Used to assess how fitness changes over evolutionary time.

#### `selfsend-...-competitions` directory

Contains data from competition experiments between a wild-type strain and variants of that strain where messages with certain tags are disabled by returning them to the sender.
Used to assess how many distinct messages a strain is relying on.

#### `stage=...+what=tabulated_and_stitched` directory

Contains summary data that has been collated, tabulated, and stitched from across series for a single stint.

#### `stage=...+what=collated` directory

Contains summary data that has been collated across all series and all stints.

#### `strain-competition/` directory

Contains data from competition experiments between genomes from different `series`.

#### `variant-competition/` directory

Contains data from competition experiments between wild-type and corresponding single instruction nop-out variants.

#### `wildtype-doubling-time/` directory

Contains measurements of how fast a wild-type strain grows from filling a quarter of a grid to filling half of the grid.
Used, in addition to competitions, as another measure of strain fitness.
