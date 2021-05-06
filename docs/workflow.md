# Experiment Workflow Overview

This page overviews the scripts built to run DISHTINY experiments on [SLURM workload manager](https://slurm.schedmd.com/) on [MSU iCER](https://icer.msu.edu/)'s HPCC cluster.
By their nature, these scripts are somewhat coupled to the particulars of SLURM and MSU's HPCC.
They should be entirely compatible with other HPCC users, though, and mostly compatible with other SLURM clusters.

You probably want to [check when this page was last updated](https://github.com/mmore500/dishtiny/commits/master/docs/workflow.md).

```{note} ::hammer_and_wrench:: ::construction_worker_woman::
If anything is unclear or out of date, please open [an issue](https://github.com/mmore500/dishtiny/issues/new) or [a pull request](https://github.com/mmore500/dishtiny/compare) so we can fix it!
Also, if you have project-specific questions or any thing else you'd like to chat about you can always feel free to [get in touch directly](mailto:m.more500@gmail.com).
I'd love to hear from you!
```

::bangbang:: ::bangbang::
In order to prevent the docs from getting out of date, many example command snippets below are annotated with `[run without params to get a help message]` instead of actual arguments.
For example, something like `./script/examplescript.sh [run without params to get a help message]`.
In order to see what actual arguments you need to provide, just run the script without providing any arguments (like `./script/examplescript.sh`) and it will print a message explaining what arguments it needs.

## Experimental Evolution

The core of DISHTINY's workflow is the evolutionary chain, a sequence of three hour simulations that evolve a particular population.
(Three hours allows work to comfortably fit into our compute cluster's four hour priority job window, taking into account some setup and teardown time.)

```
         --
        |
         |           submits            submits            submits
series  <   stint 0 ---------> stint 1 ---------> stint 2 ---------> ...
         |   (3hrs)             (3hrs)             (3hrs)
        |
         --
```

Each link in the chain is referred to as a "stint."
Stint 0 randomly generates a population and evolves it for three hours.
At termination, it saves the genomes in the population and submits a follow-up job to run the next stint.
Subsequent stints load the preceding stint's population and evolve for three hours before dumping the population and submitting a follow-up job.
This chain will continue on self-submitting until canceled.

Each independent chain of stints is referred to as a "series."
A series is identified by its random number generator seed value, an positive integer.
For example, series 10 or series 101.
We can run multiple series in parallel, perhaps with different conditions between series, in order to gain statistical significance for later analyses.

To get one or several series started, run
```bash
./slurm/evolve/evolvekickoff.sh [run without params to get a help message]
```

Code used within a series is strictly version-pinned, meaning that the repository can be freely updated without impacting existing jobs.

## Follow-up Experiments Launch Every Ten Stints

Every ten stints (including stint 0), the stint evolve job will launch a bevvy of follow-up jobs in addition to submitting its successor job.
These jobs do things like
* grow the most abundant genotype in monoculture without mutations,
  * (useful to assess a genotype's phenotypic characteristics in an isolated, controlled environment)
* perform phenotype-equivalent nopouts, and
  * (nopping instructions that don't affect the phenotype reduces the number of op instructions to analyze in further experiments and gives us a sense how many instructions actually affect phenotype),
  * launch competitions to assess the number of distinct environmental states genomes are interacting with to increase their fitness and how many distinct messages genomes are passing to increase their fitness.
  * (allows us to quantify aspects of an organism's complexity)

## Series are Grouped into Endeavors

Chunks of 1,000 series ID's constitute an "endeavor."
For example, series 11000-11999 all fall into endeavor 11.
Series 10009 falls into endeavor 10 and series 12000 falls into endeavor 12.
Later scripts will collate data from series within the same endeavor.
(If you want data from your series to remain separate, choose series IDs that fall into different endeavors.)

Endeavors 0-9 are special and reserved for debugging.
Series within endeavor 0 run extremely shortened stints (1 minute).
Series within endeavors 1-9 also run somewhat shortened stints (10 minutes).

## Data and Artifacts Live in S3 Buckets

In order for to continue experiments between different SLURM jobs, we need to transmit information between the jobs.
Some information (like software version pin, current stint number, series ID, etc.) is passed directly from through [Jinja templating](https://jinja.palletsprojects.com/en/2.11.x/) of the job scripts.
That is, a parent job specifies these fields to instantiate a child job's runscript template.
Other information (like the set of genomes constituting a population, individual genomes used for follow-up experiments) is saved to file by the parent job and then read from file by the child job.
We also need to save certain statistics (such as the amount of resource sharing, the results of a fitness competition) to file in order to interpret experimental outcomes later.

Artifacts and data from experiments are uploaded to Amazon's S3 ("Simple Storage Service") cloud.
The term artifact refers to serialized simulation objects, like a genome or a population.
The term data refers to statistics describing simulation state.
Buckets are the coarsest container for data storage available through S3.
They organize the Amazon S3 namespace at the highest level and allow access, retention, and other policies to be mapped to the set of objects they contain.
You can think of a bucket kind of like a root-level directory.

S3 buckets also provide the coarsest level of organization for DISHTINY data.
Generally speaking, data corresponding to different projects resides in different buckets.
Buckets are currently named after the unique identifier (i.e., "dnh2v" or "prq49") of corresponding [Open Science Framework](https://osf.io) projects, although there's no programmatic reason they must be.

The S3 bucket you'll be working in must be specified when launching DISHTINY scripts.
At runtime, You'll also need access AWS credentials with permissions to the bucket you're using .
DISHTINY scripts assume that these credentials will be available as environment variables.

The easiest way to provide credentials is by throwing a file called `.secrets.sh` into your home directory.
The scripts will check for this file and source it if it's available.
Here's what I've got in my `~/.secrets.sh` file.
```bash
# open science framework, a nifty free data depository
# https://osf.io/
export OSF_USERNAME=my_secret
export OSF_PASSWORD=my_secret
# optional, used to send push notifications in addition to failmail
# https://pushover.net/
export PUSHOVER_APP_TOKEN=my_secret
export PUSHOVER_USER_TOKEN=my_secret
# used to ssh into host from inside container
# i.e., your cluster login credentials
export HOST_USERNAME=my_secret
export HOST_PASSWORD=my_secret
# used for S3, a high-performance data depository
# https://aws.amazon.com/
export AWS_ACCESS_KEY_ID=my_secret
export AWS_SECRET_ACCESS_KEY=my_secret
export AWS_DEFAULT_REGION=my_secret
# optional, used to prevent DockerHub throttling
# https://hub.docker.com
export SINGULARITY_DOCKER_USERNAME=mmore500
export SINGULARITY_DOCKER_PASSWORD=Docker2\!rekcod
# optional, used for twitterbot
# https://twitter.com/TheySeeMeEvolvn
export TWITTER_API_KEY=my_secret
export TWITTER_API_SECRET_KEY=my_secret
export TWITTER_BEARER_TOKEN=my_secret
export TWITTER_ACCESS_TOKEN=my_secret
export TWITTER_ACCESS_TOKEN_SECRET=my_secret
```

Storing data and artifacts in S3 buckets has several benefits:
* files are protected from our cluster's data-clearing policies and occasional filesystem downtime,
* files are already in a publicly-accessible medium suitable for long-term storage,
* compute jobs are more reproducible because they rely on cloud resources instead of local resources,
* "hot-off-the-press" data can be trivially used or contributed to by collaborators or made publicly available,
* no file count limits.

The major downside, of course, is that S3 costs money to use.
Although we're storing many, many files they're mostly pretty small.
I pay less than $50 a month at my current usage level, running 40 evolutionary series in parallel around the clock  .

If you're interested in working with DISHTINY but you are concerned about the cost of S3 service, get in touch and I'll set you up.

## Jobs Run Inside a Container

Containerization allow DISHTINY's dependencies to be bundled up and trivially deployed to heterogeneous endpoints like Google Cloud shell, your desktop machine, or the HPCC.
DISHTINY's Dockerfile defines a recipe for building a container hosting all of DISHTINY's dependencies.
Built containers are [hosted on DockerHub](https://hub.docker.com/r/mmore500/dishtiny).
[Singularity](https://sylabs.io/singularity/) allows us to run these containers on the HPCC.

You can take the container for a whirl with an interactive shell session with this command,
```bash
singularity shell docker://mmore500/dishtiny:latest
```
Here, `docker://` specifies that we should get the image from DockerHub and `latest` is a tag specifying the particular build of the container we should use.
This might take a few minutes to download and set up the first time you run it.
Caching should enable future runs with the same container image to boot up almost instantaneously, though.

Almost all of DISHTINY's runtime scripts start off by booting up a containerized environment and then jumping in there to do the rest of their work.
Some resources are so tightly coupled to the host environment, though, that they can't really be used from inside the container --- like `sbatch`, `squeue`, `mpiexec` or even `singularity` to boot up a nested script call.
The scripts get around this by `ssh`'ing out of the container back into the host to perform such business.

You don't really need to worry about this, except you'll need to shove your HPCC username and password into your `~/.secrets.sh` file.
You'll also have to specify a tag to pull the DISHTINY container at when you launch any scripts.
If you're not sure which to use, you can always specify the tag `latest`.
(This tag isn't strictly ideal because it gets regularly reassigned to the latest available container image.
Ideally you'll use a tag associated with a specific container image that won't get reassigned, something like `sha-8e9493f`.)

```{warning}
::bangbang:: ::bangbang::
DockerHub imposes rate limits on queries to their container repository.
You'll want to register a paid account ($7 a month circa 2021) if you plan to run thousands of container sessions.
You can shove your DockerHub credentials inside of `~/.secrets.sh`.
```

## Configpacks

Configpacks enable configuration settings to differ between series.
Each configpack contains several archives of configuration files, one of which is downloaded and unpacked when the DISHTINY executable starts up.
(The particular archive chosen from within a configpack corresponds 1:1 with the particular SLURM script running.
For example, the stint evolution SLURM script versus the monoculture SLURM script, etc.)

Configpacks also specify how to build the DISHTINY executable (allowing for compile-time configuration).
Ideally, a configpack might also specify which follow-up jobs to run or what process count to run simulations with, but this hasn't been implemented.

Configpacks are stored in an unarchived (i.e., folders n' files) format inside of the `configpacks/` directory.
This repository's GitHub Actions integration zips up configpacks corresponding to every commit and pushes them over to <https://github.com/mmore500/dishtiny-assets> where they can be curled from a programatically-generable url.

The rationale behind fetching configuration assets from a url is to enable straightforward integration with the DISHTINY web viewer.

## Slurm Stoker

Our cluster limits the number of jobs that can be have on the scheduler queue at once.
Unfortunately, this limit is *far* lower than the number of follow-up jobs we need to run, especially if we're running several independent series at once.

To get around this restriction, jobs are bundled using the `script/slurm_stoker.slurm.sh.jinja` template and then [slowly shoveled onto the scheduler](https://en.wikipedia.org/wiki/Fireman_(steam_engine)) as space becomes available.

Here's how that works.

We start off with several job scripts we want to submit.
```
job scripts in pwd:
  a.slurm.sh
  b.slurm.sh
  c.slurm.sh
  ...
```

We instantiate the `slurm_stoker.slurm.sh.jinja` template to create the runscript `slurm_stoker.slurm.sh`.
This instantiated runscript has, inside of it as text data, a zipped archive of all the job scripts that were in the working directory.
```
slurm_stoker.slurm.sh, zipped inside of which are:
  a.slurm.sh
  b.slurm.sh
  c.slurm.sh
  ...
```
(This runscript is essentially a [self-extracting archive](https://en.wikipedia.org/wiki/Self-extracting_archive)).


We submit the instantiated runscript.
When it runs, it unzips its child runscripts and submits as many of them as there are currently space for on the queue.
```
job scripts in pwd:
  a.slurm.sh --> submitted
  b.slurm.sh --> submitted
  c.slurm.sh
  ...
```

Unsubmitted runscripts are zipped inside of another instantiation of the `slurm_stoker.slurm.sh.jinja` template.
```
slurm_stoker.slurm.sh, zipped inside of which are:
  c.slurm.sh
  ...
```

That instantiation is then submitted.
This cycle repeats until all child runscripts have been submitted to the scheduler.

These `slurm_stoker` jobs contain a count of remaining scripts in their SLURM job names so you can `squeue -u "${USER}" --format "%j"` to get a sense for the amount of work remaining to be done.

## Series Competition

Fitness competitions fill a toroidal grid with equal counts of two genomes.
Then, we can assess the fitness of these genomes relative to one another by tracking their abundance in the toroidal grid over time.
We can use this assay to compare fitness of strains from different series, allowing us to assess how genome attributes that may differ between series (such as phenotype complexity, fitness complexity, etc.) correlate with fitness.
In order to gain statistical power, rather than a one-off competion between just two series we want to perform a systematic tournament of competitions between pairs of series.

Although most follow-up jobs are launched automatically by the evolve stint job, not all can be.
Competitions between series can only be scheduled once all series have reached a particular stint.
Because individual series don't have an easy way of assessing whether they're the last series to reach a particular stint without race conditions, you have to launch this job manually if you want this data.

```bash
./slurm/competition/straincompetitionkickoff.sh [run without params to get a help message]
```

## Noncritical Nopout

Critical sites are instructions in a genetic program that appreciably harm fitness when individually nopped out.
Noncritical sites are the exact opposite --- sites that, taken alone, don't appreciably harm fitness when individually nopped out.

Noncritical nopout refers to a genome morph where all noncritical sites have been nopped out.

For similar reasons as before, you'll have to launch jobs manually to work with these noncritical nopout morphs.

The first step is to generate these morphs using `variant-competitions/` and `control-competitions/` data and then upload them to your S3 bucket.
This part's not a SLURM job --- you can run this from your desktop machine.

```bash
source ~/.secrets.sh
./scriptroutines/collate_control_competition_series.sh [run without params to get a help message]
./scriptroutines/collate_variant_competition_series.sh [run without params to get a help message]
./script/make_noncritical_nopouts.py [bucket] [endeavor] [stint]
```

Then, you can use
```bash
./slurm/competition/noncritical-nopout-competitionkickoff.sh [run without params to get a help message]
```
to assess this morph's fitness relative to wildtype.

Phenotype-equivalent nopout refers to a genome morph where all sites that can be removed without altering a genome's phenotype have been removed.
In some cases, we're interested in understanding how noncritical sites that *do* affect phenotype affect fitness.
We can study this by interpolating between these morphs by starting at the phenotype-equivalent nopout morph and nopping out sites until we reach the noncritical nopout morph.
We then measure the fitness of these interpolated genomes.

You can also use
```bash
./slurm/competition/noncritical-phenotypeequivalent-nopinterpolation-competitionkickoff.sh [run without params to get a help message]
```
to generate these interpolated genomes and launch competitions that assess their fitness relative to the phenotype-equivalent nopout morph.

## Collating A Stint Across Series

Once all data for a particular stint has been generated, we need to stitch it together.
Joining many small data files into one big data file makes it much easier to analyze, particularly when we want to make comparisons across stints or across series.

The collation scripts are designed so that data can be repeatedly collated on an ongoing, as-available basis.
You might also choose to collage data all at once, after all data has been generated.
(This would yield an equivalent result.)

We can collate all available individual datasets across series as follows,
```bash
source ~/.secrets.sh
./scriptroutines/collate_one_stint_across_series.sh [run without params to get a help message]
```

Then, we can postprocess and stitch together individual datasets into a master dataset for a particular stint,
```bash
source ~/.secrets.sh
./script/tabulate_and_stitch_stint.py [run without params to get a help message]
```

## Collating Across Stints

Finally, we can collate our data across available stints,
```bash
source ~/.secrets.sh
./scriptroutines/collate_tabulated_and_stitched_stints.sh  [run without params to get a help message]
```

## Data Analysis

Existing data analysis uses Python scripting inside of Jupyter notebooks.
Existing notebooks are stored inside of the `binder/` directory.
You can boot them up in your web browser without any local install by visiting the project's [BinderHub page](https://mybinder.org/v2/gh/mmore500/dishtiny/binder?filepath=binder).

You can host your Jupyter notebooks locally, too, if you prefer.
If you `source ~/.secrets.sh` before launching `jupyter notebook` locally, [pandas](https://pandas.pydata.org/) will automagically be able to open load files directly from S3 URI's.

## Handling Failed Jobs

Unfortunately, jobs occasionally crash and burn for reasons outside our control.

If they crash, all DISHTINY jobs will attempt to re-queue themselves for a second try.
At this point, they will dispatch a [Pushover notification](https://pushover.net/) warning of the failure with a link to the log of the failed job, uploaded via [transfer.sh](https://transfer.sh).

If they crash again, jobs won't requeue themselves again.
You'll get an email from the SLURM job manager in addition to a Pushover notification.

It's a good idea to manually check the logs of jobs SLURM emails you about before taking action.
SLURM has occasionally been emailing about "failed jobs" that actually ran to completion just fine.
SLURM will also notify by email about NODE FAILs, even though the DISHTINY jobs are configured to be eligible for automatic requeue under these circumstances.

If you want to find a job log, they're all saved into `/mnt/scratch/${USER}/slurmlogs/`.
You can pull one up from its `SLURM_JOB_ID` via
```bash
less /mnt/scratch/${USER}/slurmlogs/*<slurm job id>*
```

```{warning} ::bangbang:: ::bangbang::
You'll need to manually create the `/mnt/scratch/${USER}/slurmlogs/` folder before starting to run jobs.
Otherwise, SLURM will quietly refuse to run any DISHTINY jobs for you.
```

If you want to look at a job's run script, they're all saved into `/mnt/scratch/${USER}/slurmscripts/`.
You can pull one up from its `SLURM_JOB_ID` via
```bash
less /mnt/scratch/${USER}/slurmscripts/*<slurm job id>*
```

If you want to resubmit a particular job, you can just
```bash
sbatch /mnt/scratch/${USER}/slurmscripts/*<slurm job id>*
```

```{warning} ::bangbang:: ::bangbang::
The `slurmlogs/` and `slurmscripts/` might start to fill up your file count quota.
You should Pushover notifications if your file count quota comes close to running out.
Run `./script/purge_logs.sh` to clear out old logs and runscripts.
```

If you're in need of log files from an older job, job log files are compressed and uploaded to your AWS S3 bucket inside of the `repro/` prefix.

## Twitterbot Integration

DISHTINY includes scripts for a (very) experimental twitterbot that tweets stats and visualizations from experiments.
It's currently hooked up to the account [@TheySeeMeEvolvn](https://twitter.com/theyseemeevolvn).

All DISHTINY runscripts include hooks that launch corresponding python scripts inside of `twitterbot/`.
Unlike all other aspects of the runscripts these hooks aren't version-pinned, meaning that they'll always launch the latest twitterbot scripts from `master`.

If the twitterbot fails or crashes, it will do so silently and won't bring down the rest of the job.
So, unless you're actually trying to tweet, you can safely ignore the twitterbot integration.
