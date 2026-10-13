Backport of the prq49 viewer to the latest `master` web viewer.
The page (`index.html`, `includes/`, `keys.toml`, `custom.css`, `make_html.py`) is generated from `master` `web/` sources.
The compiled executable (`dishtiny*.js`, `dishtiny*.wasm`, `dishtiny.worker.js`) is built from `master` (`acbb8994`) with `dish2::Spec_prq49` and `DISH2_NLEV=2`, as in `configpacks/bucket=prq49+*/build_web.sh`, using emsdk 2.0.5 with the liblzma port patch from `third-party/install_emsdk.sh`.
`Spec_prq49` on `master` matches `Spec_default` on the `prq49` branch, apart from names and comments.
The previously archived prq49 executable was built from `master` `e7851dfb` with `dish2::Spec_default`, which is the same spec.

Features carried over from `master` include the newer viewers, PNG render saving, the "Download Saved" zip bundle, and the data output (`a=saved+what=dishtiny+ext=.jsonl.gz`).

`genomes/a=genome+ext=.json.gz` bundles the stint 100 wildtype genome (abundance criterion, series 16005, thread 0, endeavor 16) from the `prq49` S3 bucket.
Load it without S3 using the "stint 100 genome (bundled)" link in the Population Loader, or <https://mmore500.com/dishtiny/prq49-backport/?GENESIS=monoculture&autoinstall=genomes/a%3Dgenome%2Bext%3D.json.gz>.
