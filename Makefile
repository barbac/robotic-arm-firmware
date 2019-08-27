# Points to the root of Google Test, relative to where this file is.
GTEST_DIR := googletest
SRC_DIR := src
TESTS_DIR := tests
BUILD_DIR := build
CXX := clang++-3.9

# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include -DNO_ARDUINO

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread -std=c++11

# All tests produced by this Makefile.  Remember to add new tests you
# created to the list.
TESTS = ${BUILD_DIR}/arm_test

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h


all: $(TESTS)

clean:
	rm -f ${BUILD_DIR}/*

test: all
	./build/arm_test

# Builds gtest.a and gtest_main.a.

# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
${BUILD_DIR}/gtest-all.o: $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc -o $@

${BUILD_DIR}/gtest_main.o: $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc -o $@

${BUILD_DIR}/gtest.a: ${BUILD_DIR}/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

${BUILD_DIR}/gtest_main.a: ${BUILD_DIR}/gtest-all.o ${BUILD_DIR}/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

# Builds a sample test.  A test should link with either gtest.a or
# gtest_main.a, depending on whether it defines its own main()
# function.

${BUILD_DIR}/arm.o: $(SRC_DIR)/arm.cpp $(SRC_DIR)/arm.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC_DIR)/arm.cpp -o $@

${BUILD_DIR}/arm_test.o: $(TESTS_DIR)/arm_test.cpp \
                     $(SRC_DIR)/arm.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I${SRC_DIR} -c $(TESTS_DIR)/arm_test.cpp -o $@

${BUILD_DIR}/arm_test: ${BUILD_DIR}/arm.o ${BUILD_DIR}/arm_test.o ${BUILD_DIR}/gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@
