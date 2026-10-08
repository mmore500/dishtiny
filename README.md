# dishtiny

[![version](https://img.shields.io/endpoint?url=https%3A%2F%2Fmmore500.github.io%2Fdishtiny%2Fmaster%2Fversion-badge.json)](https://github.com/mmore500/dishtiny/releases)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/2541bcefe83e4a0dbfca40ca7e6930ed)](https://www.codacy.com/gh/mmore500/dishtiny/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=mmore500/dishtiny&amp;utm_campaign=Badge_Grade)
[![continuous integration](https://github.com/mmore500/dishtiny/workflows/CI/badge.svg)](https://github.com/mmore500/dishtiny/actions?query=workflow%3ACI)
[![Documentation Status](https://readthedocs.org/projects/dishtiny/badge/?version=latest)](https://dishtiny.readthedocs.io/en/latest/?badge=latest)
[![documentation coverage](https://img.shields.io/endpoint?url=https%3A%2F%2Fmmore500.github.io%2Fdishtiny%2Fmaster%2Fdocumentation-coverage-badge.json)](https://dishtiny.readthedocs.io/en/latest/)
[![code coverage status](https://codecov.io/gh/mmore500/dishtiny/branch/master/graph/badge.svg)](https://codecov.io/gh/mmore500/dishtiny)
[![DockerHub link](https://img.shields.io/badge/DockerHub-Hosted-blue)](https://hub.docker.com/r/mmore500/dishtiny)
[![dotos](https://img.shields.io/endpoint?url=https%3A%2F%2Fmmore500.com%2Fdishtiny%2Fmaster%2Fdoto-badge.json)](https://github.com/mmore500/dishtiny/search?q=todo+OR+fixme&type=)
[![GitHub stars](https://img.shields.io/github/stars/mmore500/dishtiny.svg?style=flat-square&logo=github&label=Stars&logoColor=white)](https://github.com/mmore500/dishtiny)
[![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/mmore500/dishtiny/binder?filepath=binder%2Findex.ipynb)

Framework for digital multicellularity research.

Check out the live in-browser web app at <https://mmore500.com/dishtiny>.

-   Free software: MIT license
-   Documentation: <https://dishtiny.readthedocs.io>.
-   Microbenchmark results: <https://osf.io/3v9kp/>
-   Header-only C++17 Library


## Local Setup

For mac users, you will need
```bash
softwareupdate --install-rosetta --agree-to-license
xcode-select --install
```

Then, for all users,
```bash
git clone https://github.com/mmore500/dishtiny.git --single-branch --depth 1

./dishtiny/third-party/submodules.sh
./dishtiny/third-party/install_emsdk.sh

python3 -m venv dishtiny/.env
source dishtiny/.env/bin/activate
python3 -m pip install uv wheel "setuptools<82"
python3 -m uv pip install -r dishtiny/third-party/requirements.in --no-build-isolation

make -C dishtiny web-no-pthread
make -C dishtiny serve
```

and navigate to <http://localhost:8000/web>.

To build the native executable (requires MPI),
```
make -C dishtiny native
```

## Citing

If dishtiny contributes to a scientific publication, please cite it as

> Moreno, M. A., & Ofria, C. (2019). Toward open-ended fraternal transitions in individuality. Artificial life, 25(2), 117-133. <https://doi.org/10.1162/artl_a_00284>

```bibtex
@article{moreno2019toward,
    author = {Moreno, Matthew Andres and Ofria, Charles},
    title = "{Toward Open-Ended Fraternal Transitions in Individuality}",
    journal = {Artificial Life},
    volume = {25},
    number = {2},
    pages = {117-133},
    year = {2019},
    month = {05},
    issn = {1064-5462},
    doi = {10.1162/artl_a_00284},
    url = {https://doi.org/10.1162/artl\_a\_00284},
    eprint = {https://direct.mit.edu/artl/article-pdf/25/2/117/1896700/artl\_a\_00284.pdf},
}
```

Please also cite core dependencies [Empirical](https://github.com/devosoft/Empirical) and [signalgp-lite](https://github.com/mmore500/signalgp-lite).
And don't forget to leave a [star on GitHub](https://github.com/mmore500/dishtiny/stargazers)!
