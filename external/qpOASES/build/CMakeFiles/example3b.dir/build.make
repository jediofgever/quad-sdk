# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nflowers/catkin_ws/src/spirit-software/external/qpOASES

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nflowers/catkin_ws/src/spirit-software/external/qpOASES/build

# Include any dependencies generated for this target.
include CMakeFiles/example3b.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/example3b.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/example3b.dir/flags.make

CMakeFiles/example3b.dir/examples/example3b.cpp.o: CMakeFiles/example3b.dir/flags.make
CMakeFiles/example3b.dir/examples/example3b.cpp.o: ../examples/example3b.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nflowers/catkin_ws/src/spirit-software/external/qpOASES/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/example3b.dir/examples/example3b.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/example3b.dir/examples/example3b.cpp.o -c /home/nflowers/catkin_ws/src/spirit-software/external/qpOASES/examples/example3b.cpp

CMakeFiles/example3b.dir/examples/example3b.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/example3b.dir/examples/example3b.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nflowers/catkin_ws/src/spirit-software/external/qpOASES/examples/example3b.cpp > CMakeFiles/example3b.dir/examples/example3b.cpp.i

CMakeFiles/example3b.dir/examples/example3b.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/example3b.dir/examples/example3b.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nflowers/catkin_ws/src/spirit-software/external/qpOASES/examples/example3b.cpp -o CMakeFiles/example3b.dir/examples/example3b.cpp.s

# Object files for target example3b
example3b_OBJECTS = \
"CMakeFiles/example3b.dir/examples/example3b.cpp.o"

# External object files for target example3b
example3b_EXTERNAL_OBJECTS =

bin/example3b: CMakeFiles/example3b.dir/examples/example3b.cpp.o
bin/example3b: CMakeFiles/example3b.dir/build.make
bin/example3b: libs/libqpOASES.so.3.2
bin/example3b: CMakeFiles/example3b.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nflowers/catkin_ws/src/spirit-software/external/qpOASES/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bin/example3b"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/example3b.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/example3b.dir/build: bin/example3b

.PHONY : CMakeFiles/example3b.dir/build

CMakeFiles/example3b.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/example3b.dir/cmake_clean.cmake
.PHONY : CMakeFiles/example3b.dir/clean

CMakeFiles/example3b.dir/depend:
	cd /home/nflowers/catkin_ws/src/spirit-software/external/qpOASES/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nflowers/catkin_ws/src/spirit-software/external/qpOASES /home/nflowers/catkin_ws/src/spirit-software/external/qpOASES /home/nflowers/catkin_ws/src/spirit-software/external/qpOASES/build /home/nflowers/catkin_ws/src/spirit-software/external/qpOASES/build /home/nflowers/catkin_ws/src/spirit-software/external/qpOASES/build/CMakeFiles/example3b.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/example3b.dir/depend

