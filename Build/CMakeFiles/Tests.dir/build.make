# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/nestoroprysk/Documents/GitHub/JsonParser2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/nestoroprysk/Documents/GitHub/JsonParser2/Build

# Include any dependencies generated for this target.
include CMakeFiles/Tests.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Tests.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Tests.dir/flags.make

CMakeFiles/Tests.dir/Tests/BasicTests.cpp.o: CMakeFiles/Tests.dir/flags.make
CMakeFiles/Tests.dir/Tests/BasicTests.cpp.o: ../Tests/BasicTests.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/nestoroprysk/Documents/GitHub/JsonParser2/Build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Tests.dir/Tests/BasicTests.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Tests.dir/Tests/BasicTests.cpp.o -c /Users/nestoroprysk/Documents/GitHub/JsonParser2/Tests/BasicTests.cpp

CMakeFiles/Tests.dir/Tests/BasicTests.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Tests.dir/Tests/BasicTests.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/nestoroprysk/Documents/GitHub/JsonParser2/Tests/BasicTests.cpp > CMakeFiles/Tests.dir/Tests/BasicTests.cpp.i

CMakeFiles/Tests.dir/Tests/BasicTests.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Tests.dir/Tests/BasicTests.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/nestoroprysk/Documents/GitHub/JsonParser2/Tests/BasicTests.cpp -o CMakeFiles/Tests.dir/Tests/BasicTests.cpp.s

# Object files for target Tests
Tests_OBJECTS = \
"CMakeFiles/Tests.dir/Tests/BasicTests.cpp.o"

# External object files for target Tests
Tests_EXTERNAL_OBJECTS =

Tests: CMakeFiles/Tests.dir/Tests/BasicTests.cpp.o
Tests: CMakeFiles/Tests.dir/build.make
Tests: libJsonParser.a
Tests: /usr/local/lib/libgtest.a
Tests: /usr/local/lib/libgtest_main.a
Tests: CMakeFiles/Tests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/nestoroprysk/Documents/GitHub/JsonParser2/Build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Tests"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Tests.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Tests.dir/build: Tests

.PHONY : CMakeFiles/Tests.dir/build

CMakeFiles/Tests.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Tests.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Tests.dir/clean

CMakeFiles/Tests.dir/depend:
	cd /Users/nestoroprysk/Documents/GitHub/JsonParser2/Build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/nestoroprysk/Documents/GitHub/JsonParser2 /Users/nestoroprysk/Documents/GitHub/JsonParser2 /Users/nestoroprysk/Documents/GitHub/JsonParser2/Build /Users/nestoroprysk/Documents/GitHub/JsonParser2/Build /Users/nestoroprysk/Documents/GitHub/JsonParser2/Build/CMakeFiles/Tests.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Tests.dir/depend

