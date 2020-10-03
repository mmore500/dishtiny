TEST_NAMES := ChannelPack

TO_ROOT := $(shell git rev-parse --show-cdup)

EMP_DIR := $(TO_ROOT)/third-party/Empirical/source

CXX ?= g++

FLAGS = -std=c++17 -pthread -Wall -Wno-unused-function -Wno-unused-private-field -I$(TO_ROOT)/include/ -I$(TO_ROOT)/third-party/ -I$(EMP_DIR)

default: test

test-%: %.cpp ../third-party/Catch/single_include/catch2/catch.hpp
	sed "s/g++/${CXX}/g" "/usr/bin/h5c++" > "h5c++" && chmod +x "h5c++"
	./h5c++ $(FLAGS) $< -o $@.out
	# execute test
	./$@.out
	rm "h5c++"

cov-%: %.cpp ../coverage_include ../third-party/Catch/single_include/catch2/catch.hpp
	sed "s/g++/clang++/g" "/usr/bin/h5c++" > "h5c++" && chmod +x "h5c++"
	./h5c++ $(FLAGS) $< -o $@.out
	# execute test
	./$@.out
	llvm-profdata merge default.profraw -o default.profdata
	llvm-cov show ./$@.out -instr-profile=default.profdata > coverage_$@.txt
	python $(TO_ROOT)/third-party/force-cover/fix_coverage.py coverage_$@.txt
	rm "h5c++"

# Test in debug mode without pointer tracker
test: $(addprefix test-, $(TEST_NAMES))
	rm -rf test*.out

# Test optimized version without debug features
opt: FLAGS := -std=c++17 -pthread -DNDEBUG -O3 -Wno-unused-function -I$(TO_ROOT)/include/ -I$(TO_ROOT)/third-party/ -I$(EMP_DIR)
opt: $(addprefix test-, $(TEST_NAMES))
	rm -rf test*.out

# Test in debug mode with pointer tracking
fulldebug: FLAGS := -std=c++17 -pthread -g -Wall -Wno-unused-function -I$(TO_ROOT)/include/ -I$(TO_ROOT)/third-party/ -I$(EMP_DIR) -pedantic -DEMP_TRACK_MEM -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -ftemplate-backtrace-limit=0 # -Wmisleading-indentation
fulldebug: $(addprefix test-, $(TEST_NAMES))
	rm -rf test*.out

cranky: FLAGS := -std=c++17 -pthread -g -Wall -Wno-unused-function -I$(TO_ROOT)/include/ -I$(TO_ROOT)/third-party/ -I$(EMP_DIR) -pedantic -DEMP_TRACK_MEM -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -Wconversion -Weffc++
cranky: $(addprefix test-, $(TEST_NAMES))
	rm -rf test*.out

../coverage_include:
	./convert_for_tests.sh

../third-party/Catch/single_include/catch2/catch.hpp:
	git submodule init
	git submodule update

coverage: FLAGS := -std=c++17 -pthread -g -Wall -Wno-unused-function -I$(TO_ROOT)/coverage_include/ -I$(TO_ROOT)/third-party/ -DEMP_TRACK_MEM -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -ftemplate-backtrace-limit=0 -fprofile-instr-generate -fcoverage-mapping -fno-inline -fno-elide-constructors -O0
coverage: $(addprefix cov-, $(TEST_NAMES))

clean:
	rm -f *.out
	rm -f *.o
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.info
	rm -f *.gcov
	rm -f ./Coverage*
	rm -rf ./temp
