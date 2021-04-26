# Force-cover

[![Build Status](https://travis-ci.org/emilydolson/force-cover.svg?branch=master)](https://travis-ci.org/emilydolson/force-cover)

Getting accurate test coverage information about C++ code containing templates is challenging; uninstantiated templates don't make it into the compiled binary, so compilers don't instrument them for coverage tracking (i.e. if you never use a template the compiler thinks it isn't runnable code and doesn't count it as lines that should be covered). Since templates with no test coverage are likely to never get instantiated this results in overly accurate test coverage metrics.

Force-cover is a set of tools for dealing with this problem. It consists of two parts: 
* a C++ program (built with Clang Libtooling) that reads your C++ code, finds the templates, and sticks comments before and after them to indicate that they should be covered.
* a python program that looks at the final test coverage output, finds the macros, and adjusts the file as necessary to indicate that uncovered template code should be counted as uncovered code.

# Requirements:
- Python (any version)
- clang (version 5+)
- libclang-dev (version 5+ - must be same version as clang)

Theoretically force-cover should work on any operating system, but it's currently only been tested on Ubuntu and Linux Mint.

# Installation

You can install the requirements on Ubuntu-flavored Linux with:
```
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
sudo apt-add-repository "deb http://apt.llvm.org/xenial/ llvm-toolchain-xenial-6.0 main"
sudo apt-get update
sudo apt-get install -y clang-6.0 libclang-6.0-dev
```
(more information [here](https://blog.kowalczyk.info/article/k/how-to-install-latest-clang-6.0-on-ubuntu-16.04-xenial-wsl.html))

You can build force-cover by cloning this repo and running Make inside it:
```
git clone https://github.com/emilydolson/force-cover.git
cd force-cover
make
```

This will create the force_cover executable. No additional work is needed to set up the Python script.

### Troubleshooting

If you have multiple versions of clang or llvm on your computer, the Make command may fail. You may be able to fix this by changing the default version as described at the bottom of [this page](https://blog.kowalczyk.info/article/k/how-to-install-latest-clang-6.0-on-ubuntu-16.04-xenial-wsl.html). Alternatively, you can modify the Makefile to include absolute paths to the installation location. Uncomment out the `CLANG_INCLUDES :=` line and `LLVM_SRC_PATH := ` line and set LLVM_SRC_PATH equal to the path to your llvm installation location (e.g. `/usr/lib/llvm-6.0`). You may also need to put `$(LLVM_SRCC_PATH)/bin/` before `llvm-config` in the `LLVM_CXXFLAGS` and `LLVM_LDFLAGS` lines if you are getting errors about llvm-config not being found.

# Using force-cover

The workflow for using force-cover is as follows:
* Run all of your C++ code through the force_cover C++ program to insert comments.
* Compile your program using appropriate flags for your compiler to indicate that you want to measure test coverage on this program
* Run your program
* Run your coverage program 
* Run the python script on the coverage program's output

In theory, this should be possible with a variety of compilers and code coverage programs. Thus far, I have only tested it with LLVM Source Based coverage. If you have tested it and found that it worked with a different toolchain, let me know so I can add it to this documentation!

### Step 1: Run force_cover on your code

The syntax for running the force_cover C++ program is:

```
./force_cover [C++ code file to be evaluated] -- [any flags you would pass to the compiler when compiling this program]
```

For instance, to run it on the example you could use:

```
./force_cover examples/example.cc -- --language c++ -std=c++11
```

By default, it prints the modified version of the code to stdout. In order to compile programs using the modified code, you'll need to pipe this new code to a file. For instance:

```
./force_cover examples/example.cc -- --language c++ -std=c++11 > examples/example_with_template_coverage_info.cc
```

For larger code-bases, one option is to make a copy of your code, rewrite all of the files in the copy, and use those files to compile your tests. This can be achived with a few lines of bash code. For instance, let's say you're writing a header-only library and all of the headers live in a directory called `source`. You could run the following code:

```
cp -r source coverage_source
for filename in `find ../coverage_source -name "*.h"`
do
    ./force_cover $filename -- -I../coverage_source --language c++ -std=c++14 | xargs -0 echo > $filename.temp
    mv $filename.temp $filename
done
```

Then when you go to compile your tests for coverage, instead of including `source` you would include `coverage_source` (i.e. replace `-Isource` with `-Icoverage_source`).

If you are running tests on a continuous integration platform you may choose to skip the step of copying the code to a different directory. Just be aware that **this is dangerous because it will overwrite your code**.

### Step 2: Compile your program

In order to get coverage information, you need to compile your program with coverage instrumentation turned on. This can be achieved by passing a few flags to the compiler. In LLVM, there are a number of different systems of coverage instrumention. The one I have had by far the most luck with is Source Based coverage, which can be enabled with the `-fprofile-instr-generate` and  `-fcoverage-mapping` flags. The other version, which mirrors GCC's gcov system, sometimes optimizes unused class methods out of the binary, preventing them from getting appropriately flagged as not covered.

Some other useful flags to prevent the compiler from making optimizations that hide uncovered code are: `-O0 -fno-inline -fno-elide-constructors`.

So your compilation step will probably look something like:

```
clang++ -fprofile-instr-generate -fcoverage-mapping -O0 -fno-inline -fno-elide-constructors examples/example_with_template_coverage_info.cc -o example
```

Note that Source Based coverage is only available in clang. Theoeretically, the tools in this repo should work on code instrumented in other ways but, as mentioned before, it hasn't been tested on them.

### Step 3: Run your program

The most straightforward step! Run your program so that the coverage instrumentation can record which lines were executed.

For instance:
```
./example
```

### Step 4: Extract coverage information

Now that you've run your program, coverage data exists but it's probably not in an easy-to-interpret form. You'll have to run a program to extract it. For LLVM Source Based coverage, that will look like:

```
llvm-profdata merge default.profraw -o default.profdata
llvm-cov show ./example -instr-profile=default.proddata > coverage.txt
```

This processes the raw coverage data and then compares that information ot the executable to generate a report indicating the number of time each line was executed. Specifically, the format should look like this:

```
[line_number] |     [times_line_executed]|  [code from source file]
```

Whatever compiler and tools you used, you need to end up with data in this format for step 5 to work. Fortunately, it seems to be a relatively common format (Note: if anyone knows the actual name of this format, send me a PR! I wrote this tool because I needed it and thought others might too, not because I'm some kind of code coverage expert).

### Step 5: Run fix_coverage.py

For the final step, run fix_coverage.py on your output file from the previous step. **Note that this will overwrite your output file**.

```
python fix_coverage.py coverage.txt
```

This script will go through and find all of the regions that are erroneously being excluded from coverage analysis and modify the coverage file to indicate that they should be covered but are not.

### Step 6: Profit!

Ta-da! You have code coverage data that includes uninstantiated templates! You can look at the file directly, or pass it along to a service like [codecov](https://codecov.io) that will give you a more user-friendly way to examine your coverage (codecov's documentation on using llvm-cov isn't super clear, but it will accept files in this format with names matching the pattern `coverage*.txt`). 

# Caveats

Code coverage is a flawed metric. Just because a line of code is executed doesn't mean it's being rigorously tested. This is especially true for templates, since different instantiations of the same template could be wildly different from each other. That's the whole reason uninstantiated templates don't get included in the binary in the first place: template definitions only have a meaning with an appropriate set of arguments. Force-cover can increase the accuracy of your code coverage and alert you to uninstantiated templates, but it can't gauruntee that your tests are actually good.

# Bugs? Contributions?

Open an issue or send me a PR! I'm not an expert on this stuff, so I'm sure there are myriad ways force-cover could be better. I welcome all contributions. The code is pretty succinct, so hopefully it's not too overwhelming to wade into.

In particular I would love to recieve:
- Additional rules for `validate_line` in `fix_coverage.py`. Its goal is to detect lines that should not be marked as potentially coverable (e.g. lines containing only comments). I wrote some very basic rules, but I'm sure there are a bunch of edge cases it's missing. 
- Improvements to the AST matching rules in `force_cover.cpp`. I'm sure there are edge cases that they're currently missing. Also in general they're a little overzealous at this point (in mostly harmless ways).
- There is probably a smoother way to do all of this (e.g. one that doesn't require both a pre-processing step and a post-processing step). Potential options (some of which I tried and gave up on):
    - Automatically add code that instantiates templates. Problem: you need to know what types to instantiate them with.
    - Detect uninstantiated templates and replace them with an equivalent number of lines of non-templated code. Problem: detecting uninstantiated templates is non-trivial.
    - Ditch the preprocessing script and let Python find templates in the coverage output. Problem: probably requires parsing C++ in Python.
