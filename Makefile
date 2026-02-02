.DEFAULT_GOAL := all
CXX = g++

EXTRA_INC =
EXTRA_LIB =

ifneq ($(wildcard /opt/homebrew/opt/googletest/include/gtest/gtest.h),)
EXTRA_INC += -I /opt/homebrew/opt/googletest/include
EXTRA_LIB += -L /opt/homebrew/opt/googletest/lib
endif

ifneq ($(wildcard /opt/homebrew/opt/expat/include/expat.h),)
EXTRA_INC += -I /opt/homebrew/opt/expat/include
EXTRA_LIB += -L /opt/homebrew/opt/expat/lib
endif

CXXFLAGS = -std=c++20 -fprofile-arcs -ftest-coverage -I include $(EXTRA_INC)
LDFLAGS = $(EXTRA_LIB) -lgtest -lgtest_main -lpthread -fprofile-arcs -ftest-coverage

obj/%.o: src/%.cpp | obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

testobj/%.o: testsrc/%.cpp | testobj
	$(CXX) $(CXXFLAGS) -c $< -o $@

testbin/teststrutils: obj/StringUtils.o testobj/StringUtilsTest.o
	$(CXX) $^ $(LDFLAGS) -o $@

testbin/teststrdatasource: obj/StringDataSource.o testobj/StringDataSourceTest.o
	$(CXX) $^ $(LDFLAGS) -o $@

testbin/teststrdatasink: obj/StringDataSink.o testobj/StringDataSinkTest.o
	$(CXX) $^ $(LDFLAGS) -o $@

testbin/testdsv: obj/DSVReader.o obj/DSVWriter.o obj/StringDataSource.o obj/StringDataSink.o testobj/DSVTest.o
	$(CXX) $^ $(LDFLAGS) -o $@

testbin/testxml: obj/XMLReader.o obj/XMLWriter.o obj/StringDataSource.o obj/StringDataSink.o testobj/XMLTest.o
	$(CXX) $^ $(LDFLAGS) -lexpat -o $@

all: dirs testbin/teststrutils testbin/teststrdatasource testbin/teststrdatasink testbin/testdsv testbin/testxml
	./testbin/teststrutils
	./testbin/teststrdatasource
	./testbin/teststrdatasink
	./testbin/testdsv
	./testbin/testxml
	gcov obj/*.o || true
	lcov --capture --directory obj --output-file coverage.info --ignore-errors inconsistent,unsupported || true
	lcov --remove coverage.info '/Library/*' '/opt/homebrew/*' '*/googletest/*' '*/testsrc/*' \
		--output-file coverage.filtered.info --ignore-errors unused,inconsistent,unsupported || true
	test -f coverage.filtered.info && genhtml coverage.filtered.info --output-directory htmlcov \
		--ignore-errors inconsistent,corrupt,unsupported,category || true



obj testobj testbin bin lib htmlcov:
	mkdir -p $@

dirs:
	mkdir -p bin htmlcov lib obj testbin testobj

clean:
	rm -rf obj bin htmlcov lib testbin testobj htmlcov

.PHONY: all clean dirs
