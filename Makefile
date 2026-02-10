.DEFAULT_GOAL := all
CXX = g++

# conditional paths for local (homebrew) vs CSIF
ifneq ($(wildcard /opt/homebrew/include/gtest/gtest.h),)
EXTRA_INC = -I /opt/homebrew/include -I /opt/homebrew/opt/expat/include
EXTRA_LIB = -L /opt/homebrew/lib -L /opt/homebrew/opt/expat/lib
endif

CXXFLAGS = -std=c++20 -fprofile-arcs -ftest-coverage -I include $(EXTRA_INC)
LDFLAGS = -lgtest -lgtest_main -lpthread -fprofile-arcs -ftest-coverage $(EXTRA_LIB)

.PHONY: all test coverage clean dirs

# Tests to only make output show only test results and clean things up
test: dirs testbin/teststrutils testbin/teststrdatasource testbin/teststrdatasink testbin/testdsv testbin/testxml
	@./testbin/teststrutils --gtest_brief=1 2>&1 | egrep '\[  (PASSED|FAILED)  \]'
	@./testbin/teststrdatasource --gtest_brief=1 2>&1 | egrep '\[  (PASSED|FAILED)  \]'
	@./testbin/teststrdatasink --gtest_brief=1 2>&1 | egrep '\[  (PASSED|FAILED)  \]'
	@./testbin/testdsv --gtest_brief=1 2>&1 | egrep '\[  (PASSED|FAILED)  \]'
	@./testbin/testxml --gtest_brief=1 2>&1 | egrep '\[  (PASSED|FAILED)  \]'

all: test

coverage: test
	@gcov obj/*.o >/dev/null 2>&1 || true
	@lcov --capture --directory obj --output-file coverage.info --ignore-errors inconsistent,unsupported >/dev/null 2>&1 || true
	@lcov --remove coverage.info '/Library/*' '/opt/homebrew/*' '*/googletest/*' '*/testsrc/*' \
		--output-file coverage.filtered.info --ignore-errors unused,inconsistent,unsupported >/dev/null 2>&1 || true
	@test -f coverage.filtered.info && genhtml coverage.filtered.info --output-directory htmlcov \
		--ignore-errors inconsistent,corrupt,unsupported,category >/dev/null 2>&1 || true

obj/%.o: src/%.cpp | obj
	@$(CXX) $(CXXFLAGS) -c $< -o $@

testobj/%.o: testsrc/%.cpp | testobj
	@$(CXX) $(CXXFLAGS) -c $< -o $@

testbin/teststrutils: obj/StringUtils.o testobj/StringUtilsTest.o
	@$(CXX) $^ $(LDFLAGS) -o $@

testbin/teststrdatasource: obj/StringDataSource.o testobj/StringDataSourceTest.o
	@$(CXX) $^ $(LDFLAGS) -o $@

testbin/teststrdatasink: obj/StringDataSink.o testobj/StringDataSinkTest.o
	@$(CXX) $^ $(LDFLAGS) -o $@

testbin/testdsv: obj/DSVReader.o obj/DSVWriter.o obj/StringDataSource.o obj/StringDataSink.o testobj/DSVTest.o
	@$(CXX) $^ $(LDFLAGS) -o $@

testbin/testxml: obj/XMLReader.o obj/XMLWriter.o obj/StringDataSource.o obj/StringDataSink.o testobj/XMLTest.o
	@$(CXX) $^ $(LDFLAGS) -lexpat -o $@

obj testobj testbin bin lib htmlcov:
	@mkdir -p $@

dirs:
	@mkdir -p bin htmlcov lib obj testbin testobj

clean:
	@rm -rf bin htmlcov lib obj testbin testobj *.gcda *.gcno *.info *.gcov
