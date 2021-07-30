# Glossary

## Actuator

An output that a cell's genetic program can control to interact with the environment.
Also referred to as an output register.

## Apex Hereditary Group

The highest level of hereditary grouping defined within a simulation.
If one hereditary group level is defined, this is the L0 group.
If two hierarchical group levels are defined, this is the L1 group.

See [Hereditary Group](#hereditary-group) and [L0, L1, etc. Hereditary Groups](#l0-l1-etc-hereditary-group).

## Artifact

Serialized simulation objects, like a genome or a population.

## Cardinal

The virtual CPU and hardware a cell uses to manage environmental and agent-agent interactions with respect to a particular neighboring cell.
Each cell has one cardinal per neighboring cell, four in total on the two-dimensional toroidal grid.
The term cardinal is used in the sense of "cardinal directions."

## Cell

The smallest reproducing element of the simulation.
Occupies a single tile on the two-dimensional toroidal grid.
Contains a genome, which is run in parallel on each virtual CPU within the cell.

## Conduit

A one-way communication channel used in implementation to wire together simulation elements.
May span between threads or processes.
Designed to enable best-effort communication between hardware elements executing threads and/or processes.
Can transmit discrete packets (e.g., messages) or continuous-valued quantities representing a conserved substance (i.e., resource).

See the [Conduit C++ Library](https://github.com/mmore500/conduit) for more detail.

## Configuration

Refers to settings that may be adjusted run-time (as opposed to compile-time).
Can be controlled by command-line flag, URL query parameters, or configuration file (`.cfg`).

## Configpack

A bundled set of configuration (run-time settings) and specs (compile-time settings).
Corresponds to an experimental treatment.
Deployed as a downloadable zip archive.

## Control Competition

A fitness competition between a strain and an identical copy of itself, used as a statistical control.

## Critical Site

An instruction in a genome's program critical to fitness.
Specifically, consider a variant is created where that site is disabled.
If the site is critical, the variant would perform significantly worse in fitness competition against its wild-type counterpart.

## Doubling Time Assay

A test used to estimate fitness  where a single strain is seeded into a contiguous fourth of the toroidal grid and the population growth rate is monitored until it fills half of the toroidal grid or a timeout is reached.

## Event

A condition that can trigger execution of a genome module in a particular cardinal.
The simulation monitors whether the condition is fulfilled.
When the condition is fulfilled, a corresponding tag is dispatched to the cardinal's virtual CPU.
If the event tag matches with a tag identifying a genome module, execution of that genome module is triggered.

## Event Library

The set of supported events that are dispatched by the simulation.

## Extrospective State

The readable state of a neighboring cell's cardinal.
This state is available for a cell to read from its neighbor on a best-effort basis.
This state is also used by the simulation to dispatch certain events.

## Fitness

A genome's reproductive success.

## Fitness Competition

A test used to compare the relative fitness of two strains.
The two-dimensional toroidal grid is seeded with equal copy counts of two strains, intermixed.
The simulation is run with mutation disabled for a fixed amount of time.
Then, relative abundances of the two strains is measured.
Replicate measurements allow for statistical detection of fitness differences between the strains.
A strain that is significantly more abundant than its competitor is determined to be more fit.

## Fitness Complexity

The number of critical sites in a genome.

## Fitness Tournament

A set of fitness competitions among strains from different series, used to estimate fitness of the strains relative to one another.
Competitions are conducted so that each strain participates in fitness competitions with a unique, arbitrarily chosen set of twenty other strains.
Fitness of a focal strain can be estimated in terms of mean fitness in its competitions or the number of competitions won.

## Fitness-Noncritical Nopout Morph

A morph in which all instructions that are not critical sites are nopped out.

## Genome

Consists of a genetic program and an array of tags to identify events.
Also contains bookkeeping information about elapsed mutations and elapsed updates.

## Hereditary Group

A heritable label that cells can use to mark and recognize kin.
As cells reproduce, they can choose to keep offspring cells in the parent's hereditary group or expel offspring to found a new hereditary group.
During their lifetime, cells can condition their behavior on whether neighbor cells are members of the same hereditary group.

The spatial layout of hereditary groups emerges via individual cell-level reproductive decisions.
Depending on run time and compile time settings, the size and shape of these hereditary groups may confer consequences (positive and/or negative) on constituent cells via simulation processes such as resource collection or resource decay.

Depending on compile time settings, hereditary groups may be hierarchically nested.
See [L0, L1, etc. Hereditary Groups](#l0-l1-etc-hereditary-group).

May be referred to in legacy code and earlier articles as same-channel signaling networks or signaling channel groups.

## Inter Messages

Messages that are transmitted between cells.
These messages contain a tag used to induce computation in the target virtual CPU and may also contain additional data.

## Intra Messages

Messages that are transmitted between cardinals within a cell.
These messages contain a tag used to induce computation in the target virtual CPU and may also contain additional data.

## Introspective State

State describing a cardinal's environmental conditions that can be read, but not written, by the cardinal's virtual CPU.

## L0, L1, etc. Hereditary Group

Hierarchical levels of nested hereditary grouping.
L0 referes to the lowest hierarchical level of hereditary grouping.
L0 groups are composed of cells.

If two hierarchical levels of hereditary grouping are defined, the L1 group is the next-highest grouping.
L1 groups are composed of L0 groups.

This pattern may continue with the addition of L2, L3, etc. groupings.

See [Hereditary Group](#hereditary-group).

## Module

A sequence of virtual CPU instructions in a program that is identified by a tag so that it may be triggered by an event or a message.

## Morph

A genome derived from a source strain by a set of many modifications.
For example, we might construct a morph in which *all* messaging instructions were nopped out.
As opposed to a variant, in which a small number of explicitly-named changes are applied to a genome.

## Mutation Event

An event during cellular reproduction in which mutations are applied to the genome inherited by the daughter cell.
Mutations applied include insertions, deletions, and point mutations.

## Nop Instruction

A virtual CPU instruction that, when executed, performs no action.
(Except possibly advancing the random number generator engine, depending on configuration.)

## Nopout

Replacing an instruction in a program with a Nop instruction.

This is the conceptual equivalent of a ["knockout"](https://en.wikipedia.org/wiki/Gene_knockout) in biological experiments.
Such manipulations can be used to assess functionality of the targeted gene by comparing organisms with and without the original gene.

We replace the original instruction with a nop instead of deleting it to avoid introducing unintended side effects with execution timing.
This Nop instruction is configured so that it advances the random number generator engine identically to the original instruction, again reducing the possibility of unintended side effects.


## Op Instruction

An instruction that is not a nop instruction.

## Operation

The action that an instruction in a genetic program causes the virtual CPU to take.
For example, a Read State operation or a Send Message operation.

## Peripheral

A struct passed to operation functions in addition to hardware state.
Operation functions can read from or write to this struct.
Each virtual CPU has its own peripheral instance, which is passed to all operation that the virtual CPU executes.
All interactions between a virtual CPU and other agents/the environment are mediated through the virtual CPU's peripheral.
Equivalent of a "trait" in vanilla SignalGP.

## Phenotype

The set of outwardly-observable traits of a biological organism.
Essentially, simulation state outside of the virtual CPU hardware.

## Phenotype Complexity

The number of op instructions in the strain's phenotype-equivalent nopout morph.

## Phenotype Differentiation

A test used to determine if two strains are phenotypically equivalent.
The strains are run in independent simulations with identical initial random number generator state.
These simulations are advanced step-by-step for a fixed number of updates.
If resource concentrations remain identical between the simulations throughout the computed updates, the genomes are consider phenotypically equivalent.
Otherwise, they are considered phenotypically differentiated.

## Phenotype-Equivalent Nopout Morph

A genome morph with all instructions that can be nopped out while maintaining phenotypic equivalence are nopped out.
This genome is generated using a "jenga" procedure in which instructions that do not cause phenotypic divergence are sequentially nopped out until no more instructions can be nopped out without causing phenotypic divergence.
This guarantees that the phenotype-equivalent nopout morph is exactly phenotypically equivalent to its source strain.

(This "jenga" procedure contrasts with an older "battleship" procedure in which all instructions were independently tested for phenotypic divergence under nopout.
Under this procedure, the resulting genome was not guaranteed to be phenotypically identical to the source strain due to epistatic effects.)

## Readable State

All cardinal state that the virtual CPU can read, including writable state and introspective state.
Readable state is hosted within the cardinal's peripheral.

## SignalGP / SignalGP-Lite

The virtual hardware framework that powers cardinal's virtual CPUs.
This hardware framework allows genetic programs to harness the event-driven computing paradigm.
This project uses a variant of "vanilla" SignalGP called [SignalGP-Lite](https://github.com/mmore500/signalgp-lite).

## Self-send Perturbation

An experimental condition in which a subset of cells are induced to deliver messages (either intra or inter) back to the sending cardinal rather than to the original target.
Assess the impact of message passing on fitness.
Used instead of simply deleting messages in order to minimize detection of contingent effects.

## Sensor

An input that a cell's genetic program can read to observe environmental state.
Also referred to as an input register.

## Service

A struct with member functions that determine whether a task should be performed on a particular cell and how to perform that task to update simulation state for that single cell.
A spec type specifies an ordered list of services to run at each update.

## Spec

A type specifying compile-time configuration options.
Most simulation methods and objects are templated on this type.
Which spec to compile with can be specified by passing `-CXXFLAGS="-DDISH2_SPEC=dish2::Spec_myspec"` to the `make` command.

## State Exchange Perturbation

An experimental condition in which a subset of cells are manipulated so that their virtual CPU's perceive inputs meant for another cell or have output overwritten by those created by another cell.
Tests whether the spatial distribution of certain inputs or outputs affects fitness.
Used instead of simply zeroing out inputs or outputs in order to minimize detection of contingent effects.

You can think of this something like us "[Freaky Friday](https://en.wikipedia.org/wiki/Freaky_Friday_(2003_film))"-ing different cells.

## State Rotate Perturbation

An experimental condition in which a subset of cells are manipulated so that their virtual CPU's perceive inputs meant for another cardinal within the cell or have output overwritten by those created by another cardinal within the cell.
Tests whether the directionality of certain inputs or outputs affects fitness.
Used instead of simply zeroing out inputs or outputs in order to minimize detection of contingent effects.

You can think of this something like us "[Freaky Friday](https://en.wikipedia.org/wiki/Freaky_Friday_(2003_film))"-ing cardinals within a cell.

## Tag

A bitstring used to match messages and events with program modules.

## Threadlocal Service

A struct with member functions that determine whether a task should be performed on a thread's population of cells and how to perform that task to update simulation state on a thread's population of cells.
A spec type specifies an ordered list of threadlocal services to run at each update.

## Variant

A genome derived from a source strain by a single modification.
For example, nopping out a particular instruction.
As opposed to a morph, where a larger set of changes are applied to a genome.

## Wildtype Morph

A genome harvested from an evolutionary experiment exactly as evolved, with no modifications.

## Writable State

All cardinal state that the virtual CPU can write, including actuator state and nop state.
Writable state is hosted within the cardinal's peripheral.
