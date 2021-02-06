# Usage

To compile this project
* natively in debugging mode (without compiler optimizations, with debugging symbols, and with runtime asserts), run `make debug` in the project's root directory.
* natively in optimized mode, run `make` in the project's root directory.
* natively in optimized mode with debugging symbols (for a more informative stack trace in the case of a crash) run `make production` in the project's root directory.
* into an interactive web app, run `make web` in the project's root directory.

Compiling natively will produce a `rundishtiny` executable.
You can run `./rundishtiny --help` to see available command-line options.
On startup, the executable will recursively log all files and folders it is situated with and will generate several output files.
To prevent excessive logging and filesystem pollution, make a temporary directory and run the executable in there.

Compiling for the web will generate JavaScript and HTML output files in the `web/` directory.
After running `make web`, run `make serve` and point your web browser to <http://127.0.0.1:8000/web> to access the generated web viewer.
You can pass most of the same arguments available on the command line to the web app using [query string syntax](https://developer.mozilla.org/en-US/docs/Web/API/URLSearchParams).
For example, you might navigate to <http://127.0.0.1:8000/web/?N_CELLS=100&POINT_MUTATION_RATE=0.0005&GENESIS=monoculture&autoinstall=https://osf.io/bjuf6/download> to set some configuration parameters and download a genome file to seed the population with.

As of February 2021, you'll need to use the Google Chrome desktop web browser to run the web viewer.
Other browsers may have compatibility, but you will have to manually enable support for JavaScript `SharedArrayBuffer`.
For more information, see [here](https://emscripten.org/docs/porting/pthreads.html) and [here](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/SharedArrayBuffer#security_requirements).

To use code from this project in your own project, add `-Ipath/to/dishtiny/include`
to your compiler flags, or add this project as a subrepo and use relative include paths.
