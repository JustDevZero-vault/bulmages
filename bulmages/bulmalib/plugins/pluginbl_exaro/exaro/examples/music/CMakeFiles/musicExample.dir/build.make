# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/tborras/exaro/exaro/examples/music

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tborras/exaro/exaro/examples/music

# Include any dependencies generated for this target.
include CMakeFiles/musicExample.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/musicExample.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/musicExample.dir/flags.make

CMakeFiles/musicExample.dir/main.cpp.o: CMakeFiles/musicExample.dir/flags.make
CMakeFiles/musicExample.dir/main.cpp.o: main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/tborras/exaro/exaro/examples/music/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/musicExample.dir/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/musicExample.dir/main.cpp.o -c /home/tborras/exaro/exaro/examples/music/main.cpp

CMakeFiles/musicExample.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/musicExample.dir/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/tborras/exaro/exaro/examples/music/main.cpp > CMakeFiles/musicExample.dir/main.cpp.i

CMakeFiles/musicExample.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/musicExample.dir/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/tborras/exaro/exaro/examples/music/main.cpp -o CMakeFiles/musicExample.dir/main.cpp.s

CMakeFiles/musicExample.dir/main.cpp.o.requires:
.PHONY : CMakeFiles/musicExample.dir/main.cpp.o.requires

CMakeFiles/musicExample.dir/main.cpp.o.provides: CMakeFiles/musicExample.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/musicExample.dir/build.make CMakeFiles/musicExample.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/musicExample.dir/main.cpp.o.provides

CMakeFiles/musicExample.dir/main.cpp.o.provides.build: CMakeFiles/musicExample.dir/main.cpp.o
.PHONY : CMakeFiles/musicExample.dir/main.cpp.o.provides.build

# Object files for target musicExample
musicExample_OBJECTS = \
"CMakeFiles/musicExample.dir/main.cpp.o"

# External object files for target musicExample
musicExample_EXTERNAL_OBJECTS =

musicExample: CMakeFiles/musicExample.dir/main.cpp.o
musicExample: /usr/local/lib/libReport.so
musicExample: /usr/lib/libQtScript.so
musicExample: /usr/lib/libQtGui.so
musicExample: /usr/lib/libQtSql.so
musicExample: /usr/lib/libQtCore.so
musicExample: CMakeFiles/musicExample.dir/build.make
musicExample: CMakeFiles/musicExample.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable musicExample"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/musicExample.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/musicExample.dir/build: musicExample
.PHONY : CMakeFiles/musicExample.dir/build

CMakeFiles/musicExample.dir/requires: CMakeFiles/musicExample.dir/main.cpp.o.requires
.PHONY : CMakeFiles/musicExample.dir/requires

CMakeFiles/musicExample.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/musicExample.dir/cmake_clean.cmake
.PHONY : CMakeFiles/musicExample.dir/clean

CMakeFiles/musicExample.dir/depend:
	cd /home/tborras/exaro/exaro/examples/music && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tborras/exaro/exaro/examples/music /home/tborras/exaro/exaro/examples/music /home/tborras/exaro/exaro/examples/music /home/tborras/exaro/exaro/examples/music /home/tborras/exaro/exaro/examples/music/CMakeFiles/musicExample.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/musicExample.dir/depend
