Backport of the prq49 viewer.
The compiled executable (`dishtiny*.js`, `dishtiny*.wasm`, `dishtiny.worker.js`) is unchanged from the `prq49` archive.
The page (`index.html`, `includes/`, `keys.toml`, `custom.css`, `make_html.py`) is regenerated from the latest `master` `web/` sources.
Features that depend on newer compiled code (e.g., new viewers) are not available.

`genomes/a=genome+ext=.json.gz` bundles the stint 100 wildtype genome (abundance criterion, series 16005, thread 0, endeavor 16) from the `prq49` S3 bucket.
Load it without S3 using the "stint 100 genome (bundled)" link in the Population Loader, or <https://mmore500.com/dishtiny/prq49-backport/?GENESIS=monoculture&autoinstall=genomes/a%3Dgenome%2Bext%3D.json.gz>.
