Configuration assets.
Each folder represents an individual "configpack."
Each configpack contains scripts to build the main executable, both in native and web modes.
Subfolders represent configuration asset packs for specific SLURM jobscripts.
Files within these subfolders are zipped into an archive and deployed via GitHub Actions so that they can be curled from a programatically-generable URL.
