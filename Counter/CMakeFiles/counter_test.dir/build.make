# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pedromoreira/Desktop/cadeiras/Cripto/HomomorphicVoteTally/Counter

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pedromoreira/Desktop/cadeiras/Cripto/HomomorphicVoteTally/Counter

# Include any dependencies generated for this target.
include CMakeFiles/counter_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/counter_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/counter_test.dir/flags.make

CMakeFiles/counter_test.dir/counter_test.cpp.o: CMakeFiles/counter_test.dir/flags.make
CMakeFiles/counter_test.dir/counter_test.cpp.o: counter_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pedromoreira/Desktop/cadeiras/Cripto/HomomorphicVoteTally/Counter/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/counter_test.dir/counter_test.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/counter_test.dir/counter_test.cpp.o -c /home/pedromoreira/Desktop/cadeiras/Cripto/HomomorphicVoteTally/Counter/counter_test.cpp

CMakeFiles/counter_test.dir/counter_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/counter_test.dir/counter_test.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pedromoreira/Desktop/cadeiras/Cripto/HomomorphicVoteTally/Counter/counter_test.cpp > CMakeFiles/counter_test.dir/counter_test.cpp.i

CMakeFiles/counter_test.dir/counter_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/counter_test.dir/counter_test.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pedromoreira/Desktop/cadeiras/Cripto/HomomorphicVoteTally/Counter/counter_test.cpp -o CMakeFiles/counter_test.dir/counter_test.cpp.s

# Object files for target counter_test
counter_test_OBJECTS = \
"CMakeFiles/counter_test.dir/counter_test.cpp.o"

# External object files for target counter_test
counter_test_EXTERNAL_OBJECTS =

counter_test: CMakeFiles/counter_test.dir/counter_test.cpp.o
counter_test: CMakeFiles/counter_test.dir/build.make
counter_test: libsss.a
counter_test: /usr/local/lib/libseal-3.4.a
counter_test: librandombytes.a
counter_test: libhazmat.a
counter_test: libtweet.a
counter_test: /usr/lib/libz.so
counter_test: CMakeFiles/counter_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pedromoreira/Desktop/cadeiras/Cripto/HomomorphicVoteTally/Counter/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable counter_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/counter_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/counter_test.dir/build: counter_test

.PHONY : CMakeFiles/counter_test.dir/build

CMakeFiles/counter_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/counter_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/counter_test.dir/clean

CMakeFiles/counter_test.dir/depend:
	cd /home/pedromoreira/Desktop/cadeiras/Cripto/HomomorphicVoteTally/Counter && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pedromoreira/Desktop/cadeiras/Cripto/HomomorphicVoteTally/Counter /home/pedromoreira/Desktop/cadeiras/Cripto/HomomorphicVoteTally/Counter /home/pedromoreira/Desktop/cadeiras/Cripto/HomomorphicVoteTally/Counter /home/pedromoreira/Desktop/cadeiras/Cripto/HomomorphicVoteTally/Counter /home/pedromoreira/Desktop/cadeiras/Cripto/HomomorphicVoteTally/Counter/CMakeFiles/counter_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/counter_test.dir/depend

