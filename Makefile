# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_SOURCE_DIR = "/mnt/d/Рабочий стол/projects/warships"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/mnt/d/Рабочий стол/projects/warships"

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start "/mnt/d/Рабочий стол/projects/warships/CMakeFiles" "/mnt/d/Рабочий стол/projects/warships/CMakeFiles/progress.marks"
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start "/mnt/d/Рабочий стол/projects/warships/CMakeFiles" 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named warships

# Build rule for target.
warships: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 warships
.PHONY : warships

# fast build rule for target.
warships/fast:
	$(MAKE) -f CMakeFiles/warships.dir/build.make CMakeFiles/warships.dir/build
.PHONY : warships/fast

src/field.o: src/field.cpp.o

.PHONY : src/field.o

# target to build an object file
src/field.cpp.o:
	$(MAKE) -f CMakeFiles/warships.dir/build.make CMakeFiles/warships.dir/src/field.cpp.o
.PHONY : src/field.cpp.o

src/field.i: src/field.cpp.i

.PHONY : src/field.i

# target to preprocess a source file
src/field.cpp.i:
	$(MAKE) -f CMakeFiles/warships.dir/build.make CMakeFiles/warships.dir/src/field.cpp.i
.PHONY : src/field.cpp.i

src/field.s: src/field.cpp.s

.PHONY : src/field.s

# target to generate assembly for a file
src/field.cpp.s:
	$(MAKE) -f CMakeFiles/warships.dir/build.make CMakeFiles/warships.dir/src/field.cpp.s
.PHONY : src/field.cpp.s

src/game.o: src/game.cpp.o

.PHONY : src/game.o

# target to build an object file
src/game.cpp.o:
	$(MAKE) -f CMakeFiles/warships.dir/build.make CMakeFiles/warships.dir/src/game.cpp.o
.PHONY : src/game.cpp.o

src/game.i: src/game.cpp.i

.PHONY : src/game.i

# target to preprocess a source file
src/game.cpp.i:
	$(MAKE) -f CMakeFiles/warships.dir/build.make CMakeFiles/warships.dir/src/game.cpp.i
.PHONY : src/game.cpp.i

src/game.s: src/game.cpp.s

.PHONY : src/game.s

# target to generate assembly for a file
src/game.cpp.s:
	$(MAKE) -f CMakeFiles/warships.dir/build.make CMakeFiles/warships.dir/src/game.cpp.s
.PHONY : src/game.cpp.s

src/gamerules.o: src/gamerules.cpp.o

.PHONY : src/gamerules.o

# target to build an object file
src/gamerules.cpp.o:
	$(MAKE) -f CMakeFiles/warships.dir/build.make CMakeFiles/warships.dir/src/gamerules.cpp.o
.PHONY : src/gamerules.cpp.o

src/gamerules.i: src/gamerules.cpp.i

.PHONY : src/gamerules.i

# target to preprocess a source file
src/gamerules.cpp.i:
	$(MAKE) -f CMakeFiles/warships.dir/build.make CMakeFiles/warships.dir/src/gamerules.cpp.i
.PHONY : src/gamerules.cpp.i

src/gamerules.s: src/gamerules.cpp.s

.PHONY : src/gamerules.s

# target to generate assembly for a file
src/gamerules.cpp.s:
	$(MAKE) -f CMakeFiles/warships.dir/build.make CMakeFiles/warships.dir/src/gamerules.cpp.s
.PHONY : src/gamerules.cpp.s

src/main.o: src/main.cpp.o

.PHONY : src/main.o

# target to build an object file
src/main.cpp.o:
	$(MAKE) -f CMakeFiles/warships.dir/build.make CMakeFiles/warships.dir/src/main.cpp.o
.PHONY : src/main.cpp.o

src/main.i: src/main.cpp.i

.PHONY : src/main.i

# target to preprocess a source file
src/main.cpp.i:
	$(MAKE) -f CMakeFiles/warships.dir/build.make CMakeFiles/warships.dir/src/main.cpp.i
.PHONY : src/main.cpp.i

src/main.s: src/main.cpp.s

.PHONY : src/main.s

# target to generate assembly for a file
src/main.cpp.s:
	$(MAKE) -f CMakeFiles/warships.dir/build.make CMakeFiles/warships.dir/src/main.cpp.s
.PHONY : src/main.cpp.s

src/user.o: src/user.cpp.o

.PHONY : src/user.o

# target to build an object file
src/user.cpp.o:
	$(MAKE) -f CMakeFiles/warships.dir/build.make CMakeFiles/warships.dir/src/user.cpp.o
.PHONY : src/user.cpp.o

src/user.i: src/user.cpp.i

.PHONY : src/user.i

# target to preprocess a source file
src/user.cpp.i:
	$(MAKE) -f CMakeFiles/warships.dir/build.make CMakeFiles/warships.dir/src/user.cpp.i
.PHONY : src/user.cpp.i

src/user.s: src/user.cpp.s

.PHONY : src/user.s

# target to generate assembly for a file
src/user.cpp.s:
	$(MAKE) -f CMakeFiles/warships.dir/build.make CMakeFiles/warships.dir/src/user.cpp.s
.PHONY : src/user.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... warships"
	@echo "... src/field.o"
	@echo "... src/field.i"
	@echo "... src/field.s"
	@echo "... src/game.o"
	@echo "... src/game.i"
	@echo "... src/game.s"
	@echo "... src/gamerules.o"
	@echo "... src/gamerules.i"
	@echo "... src/gamerules.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
	@echo "... src/user.o"
	@echo "... src/user.i"
	@echo "... src/user.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system
