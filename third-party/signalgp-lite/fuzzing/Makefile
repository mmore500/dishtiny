TARGET_NAMES += execute

TO_ROOT := $(shell git rev-parse --show-cdup)

CXX ?= g++

FLAGS = -std=c++17 -D_GLIBCXX_DEBUG -D_LIBCPP_DEBUG -g -pipe -pthread -Wall -Wno-unused-function -Wno-unused-private-field -I$(TO_ROOT)/include/ -I$(TO_ROOT)/third-party/

default: fuzz

fuzz-%: %.cpp
	$(CXX) $(FLAGS) $< -o $@.out
	# execute fuzz
	gdb ./$@.out --ex=run --ex=bt --ex='quit $$_exitcode'

# Test in debug mode without pointer tracker
fuzz: $(addprefix fuzz-, $(TARGET_NAMES))
	rm -rf fuzz*.out

# Test optimized version without debug features
opt: FLAGS := -std=c++17 -g -pipe -pthread -DNDEBUG -O3 -ffast-math -flto -march=native -Wno-unused-function -I$(TO_ROOT)/include/ -I$(TO_ROOT)/third-party/
opt: $(addprefix fuzz-, $(TARGET_NAMES))
	rm -rf fuzz*.out

# Test in debug mode with pointer tracking
fulldebug: FLAGS := -std=c++17 -pipe -pthread -g -Wall -Wno-unused-function -I$(TO_ROOT)/include/ -I$(TO_ROOT)/third-party/ -pedantic -D_GLIBCXX_DEBUG -D_LIBCPP_DEBUG -DEMP_TRACK_MEM -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -ftemplate-backtrace-limit=0 # -Wmisleading-indentation
fulldebug: $(addprefix fuzz-, $(TARGET_NAMES))
	rm -rf fuzz*.out

cranky: FLAGS := -std=c++17 -pipe -pthread -g -Wall -Wno-unused-function -I$(TO_ROOT)/include/ -I$(TO_ROOT)/third-party/ -pedantic -D_GLIBCXX_DEBUG -D_LIBCPP_DEBUG -DEMP_TRACK_MEM -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -Wconversion -Weffc++
cranky: $(addprefix fuzz-, $(TARGET_NAMES))
	rm -rf fuzz*.out

clean:
	rm -f *.out
	rm -f *.o
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.info
	rm -f *.gcov
	rm -f ./Coverage*
	rm -rf ./temp
