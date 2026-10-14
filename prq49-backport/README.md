Backport of the latest `master` web viewer onto the `prq49` branch simulation code.

The page (`index.html`, `includes/`, `keys.toml`, `custom.css`, `make_html.py`) is generated from `master` `web/` sources.

The compiled executable (`dishtiny*.js`, `dishtiny*.wasm`, `dishtiny.worker.js`) is built from the `prq49` branch (`978abb3e`) with the viewer code taken from `master` (`acbb8994`).
`source.patch` is the full source diff against `prq49`; apply it with `git apply` on a `prq49` checkout to reproduce the build.
The patch:

* replaces `include/dish2/web/`, `include/dish2/viz/`, `source/web.cpp`, and `web/` with their `master` versions;
* adds four `master`-only helper headers the viewer needs (`utility/print_js_stacktrace.hpp`, `introspection/get_live_cardinal_kin_group_ids.hpp`, `introspection/get_live_cardinal_root_ids.hpp`, `cell/cardinal_iterators/RepLevRequestWrapper.hpp`);
* calls `prq49`'s non-template `dish2::setup` from `source/web.cpp`;
* ports `master` commit `1f5f3699` ("Disable RootIDChangeoverRecorder for web"), also applied to `prq49`'s `multiculture_population.hpp`, since the recorder aborts when seeding a population in the browser (it only logs root ID changeovers and does not affect simulation dynamics);
* uses `master`'s `Makefile` for its web build flags (`-idirafter third-party/mpi-abi-stubs/`, growable memory for the no-pthread build).

All other simulation code is unchanged from `prq49`.
The build uses `dish2::Spec_prq49` and `DISH2_NLEV=2`, as in `configpacks/bucket=prq49+*/build_web.sh`, with emsdk 2.0.5 plus the liblzma port patch from `third-party/install_emsdk.sh`.
`Spec_prq49` matches `prq49`'s `Spec_default` apart from names and comments.

Features carried over from `master` include the newer viewers, PNG render saving, the "Download Saved" zip bundle, and the data output (`a=saved+what=dishtiny+ext=.jsonl.gz`).

`genomes/a=genome+ext=.json.gz` bundles the stint 100 wildtype genome (abundance criterion, series 16005, thread 0, endeavor 16) from the `prq49` S3 bucket.
Load it without S3 using the "stint 100 genome (bundled)" link in the Population Loader, or <https://mmore500.com/dishtiny/prq49-backport/?GENESIS=monoculture&autoinstall=genomes/a%3Dgenome%2Bext%3D.json.gz>.
