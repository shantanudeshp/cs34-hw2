# Project 2 - DSV and XML

Student: Shantanu (921200468)

## Overview

Implementation of CDSVReader, CDSVWriter, CXMLReader, and CXMLWriter classes for parsing and generating delimiter-separated value and XML files. Includes GoogleTest test suites and code coverage reporting.

## Build and Run

```
make        # compiles, runs all tests, generates coverage
make clean  # removes all build artifacts
```

## Code References

* [C++ Reference Documentation](http://www.cplusplus.com/reference/) - reference for C++ built-in functions and classes
* [Expat XML Library Guide](https://www.xml.com/pub/1999/09/expat/index.html) - libexpat function descriptions for XML parsing
* [Delimiter-Separated Values](https://en.wikipedia.org/wiki/Delimiter-separated_values) - DSV format overview referenced in project spec
* [RFC 4180 - CSV Format](https://tools.ietf.org/html/rfc4180) - standard conventions for quoting and escaping in CSV files
* ECS34 guides provided on Canvas

## Generative AI Use

- AI-assisted tab completion (IDE autocomplete) - explicitly permitted per course policy
- Consulted generative AI assistant for implementation guidance and debugging

---

### 1. GoogleTest Makefile Structure

**Prompt:** "How do I set up a Makefile for a C++ project using GoogleTest with code coverage?"

**Original AI Response:**
"You need a few key components. First, set your compiler flags to include `-std=c++20` for modern C++ and `-fprofile-arcs -ftest-coverage` for coverage instrumentation. Add `-I` flags for your include directories. For linking, you need `-lgtest -lgtest_main -lpthread` plus the same coverage flags. Use pattern rules like `obj/%.o: src/%.cpp` to avoid writing a separate compile command for every file. Do the same for test objects with `testobj/%.o: testsrc/%.cpp`. Each test executable links together the relevant implementation object files with the test object file. Your default target should build all test executables and run them in sequence."

**Changes Made:** Structured the Makefile with separate pattern rules for src and testsrc. Set up five test executable targets, each linking only the object files it needs. The `all` target builds everything then runs each test binary in the order specified by the assignment.

**Rationale:** AI provided the general structure; I tailored the specific targets and link dependencies to match the project's five test executables and their required object files.

---

### 2. Architecture Mismatch on macOS

**Prompt:** "I'm getting linker errors saying it's ignoring libraries because of wrong architecture on my Mac. What's going on?"

**Original AI Response:**
"This is an architecture mismatch. Modern Macs use ARM64 (Apple Silicon) processors, but some package managers install libraries compiled for x86_64 (Intel). The linker can't use x86_64 libraries on an ARM64 system. Check which libraries you're linking against with `file` to see their architecture. You need to find ARM64 versions of the same libraries. On macOS, Homebrew installs native ARM64 packages by default when running on Apple Silicon, so installing via Homebrew is usually the fix. Conda environments can be trickier since some may default to x86_64."

**Changes Made:** Checked the conda gtest libraries and confirmed they were x86_64. Installed googletest via Homebrew which provided native ARM64 builds. Used `file` to verify both gtest and expat libraries were the correct architecture before updating the Makefile paths.

**Rationale:** AI explained why the mismatch was happening; I switched to Homebrew since it provided the correct architecture without needing to configure anything special.

---

### 3. Conditional Include Paths for Portability

**Prompt:** "How do I make my Makefile work on both my local machine and a different Linux server without hardcoding paths?"

**Original AI Response:**
"GNU Make has a `wildcard` function you can use in conditionals. The pattern is: use `ifneq ($(wildcard /some/path/file.h),)` to check if a specific file exists. If it does, set your extra include and library paths. If it doesn't (like on a server where libraries are at standard system paths), leave them empty and the compiler will find them in the default search paths. This way the same Makefile works in both environments without any changes. Put the conditional near the top of the Makefile before you define CXXFLAGS and LDFLAGS."

**Changes Made:** Used the wildcard check on the Homebrew gtest header path. When it exists (local machine), added both the gtest and expat include and library paths. When it doesn't exist (CSIF), the variables stay empty and the system default paths are used.

**Rationale:** AI suggested the wildcard conditional approach; I applied it to check for the Homebrew gtest header specifically since that's the indicator of whether we're on the local machine or CSIF.

---

### 4. Directory Creation in Make

**Prompt:** "My Makefile fails because the output directory doesn't exist when trying to write object files. How do I handle this?"

**Original AI Response:**
"The cleanest approach is to create a phony target that runs `mkdir -p` for all your build directories. Make it a dependency of your default target so directories are always created before any compilation happens. `mkdir -p` is safe to call even if the directory already exists — it just does nothing. Mark the target as `.PHONY` so Make always runs it regardless of whether a file with that name exists. An alternative is order-only prerequisites on each pattern rule, but a single `dirs` target is simpler when you have multiple directories to create."

**Changes Made:** Created a `dirs` target that runs `mkdir -p bin htmlconv lib obj testbin testobj`. Made it the first dependency of the `all` target so directories exist before any object files are compiled. Added `dirs` to the `.PHONY` declaration.

**Rationale:** AI suggested both approaches; I went with the single `dirs` target since it keeps directory creation in one place and the expected usage is always through `make` which runs `all`.

---

### 5. Linking Test Helper Classes

**Prompt:** "I'm getting undefined symbol errors for CStringDataSource and CStringDataSink when linking my DSV test executable. The DSV classes themselves link fine. Why?"

**Original AI Response:**
"The linker needs every object file that contains symbols referenced by your code. Your DSV test file uses CStringDataSource and CStringDataSink to create test fixtures — it constructs them and calls their methods. Even though those classes aren't part of the DSV implementation itself, the test code references them, so their compiled object files need to be included in the link command. You need to add `obj/StringDataSource.o` and `obj/StringDataSink.o` to the link line for your DSV test executable. The same applies to any test that uses helper classes not already in its dependency list."

**Changes Made:** Added `obj/StringDataSource.o` and `obj/StringDataSink.o` to both the `testdsv` and `testxml` link targets in the Makefile. The DSV and XML tests both use these as test fixtures to feed data to readers and collect output from writers.

**Rationale:** AI explained that the linker resolves all referenced symbols regardless of which class "owns" them; I added both helper object files since the tests need them to construct data sources and sinks.

---

### 6. Coverage Instrumentation Flags

**Prompt:** "What compiler and linker flags do I need for gcov code coverage to work correctly?"

**Original AI Response:**
"You need `-fprofile-arcs -ftest-coverage` on both the compile and link steps — this is a common mistake. The compile flags tell the compiler to instrument the code and generate `.gcno` files (the coverage graph). The link flags tell the linker to include the coverage runtime library that actually records execution data into `.gcda` files at runtime. If you only add them to compile, the program won't have the runtime to record coverage. After running your tests, use `gcov` on the object files to process the data, then `lcov --capture` to aggregate it, and `genhtml` to produce an HTML report."

**Changes Made:** Added `-fprofile-arcs -ftest-coverage` to both CXXFLAGS and LDFLAGS. Added the coverage pipeline (`gcov`, `lcov`, `genhtml`) at the end of the `all` target recipe, each guarded with `|| true` so the build doesn't fail if lcov or genhtml aren't installed locally.

**Rationale:** AI clarified that both compile and link need the flags; I added the `|| true` guards since lcov and genhtml aren't installed on the local machine but will be on CSIF where grading happens.

---

### 7. CSV Field Quoting Rules

**Prompt:** "In CSV format, when exactly does a field need to be wrapped in quotes?"

**Original AI Response:**
"A field needs to be quoted in three cases: when it contains the delimiter character (usually a comma), when it contains a double-quote character, and when it contains a newline character. Fields without any of these special characters can be written as-is. The quoting acts as an escape mechanism — without it, a comma inside a field would be ambiguous with the field delimiter. Some CSV implementations support a 'quote all' mode where every field is quoted regardless of its content, which can be useful for consistency."

**Changes Made:** Implemented the three special character checks using `string::find()`. Added the quoteall flag as the first condition in the boolean check, since if quoteall is set we skip the character checks entirely.

**Rationale:** AI covered the three core cases; I added quoteall as a constructor parameter since the assignment spec requires it, and checked it first as a short-circuit optimization.

---

### 8. Escaping Quotes Inside Quoted Fields

**Prompt:** "How do you represent a literal double-quote character inside a quoted CSV field?"

**Original AI Response:**
"The standard convention from RFC 4180 is to double the quote character. If a field contains a literal quote, each one becomes two quotes in the output. So the text `a"b` inside a quoted field becomes `"a""b"`. When parsing, inside a quoted field, if you encounter a quote character you need to peek at the next character. If it's also a quote, that's an escaped literal — consume both and add a single quote to your field buffer. If the next character is anything else, the quote you saw was the closing delimiter of the field."

**Changes Made:** In the writer, handled it by checking each character — if it's a quote, write an extra quote before writing the character itself, so quotes naturally get doubled. In the reader, used Peek() after seeing a quote inside a quoted field to check for the doubled-quote escape pattern.

**Rationale:** AI explained the RFC 4180 convention; the writer approach of writing an extra quote before the character was simpler than building the escaped string first and then writing it.

---

### 9. CSV Parser State Machine Design

**Prompt:** "How should I structure a CSV parser that reads one character at a time from a data source stream?"

**Original AI Response:**
"A character-by-character CSV parser works best as a state machine. The main loop should Peek() at the next character before deciding what to do, so you can make decisions without consuming characters prematurely. The key states are: normal mode (reading an unquoted field), quoted mode (inside a quoted field), and row-complete. In normal mode: newline ends the row, quote starts a quoted field, delimiter separates fields, anything else appends to the current field. In quoted mode: doubled quotes are escapes, a single quote ends the quoted field. Using Peek() before Get() is important so you can branch on the character without losing it."

**Changes Made:** Used a nested loop for quoted field parsing instead of tracking an explicit state variable. When a quote is encountered in normal mode, entered a separate inner loop that handles everything until the closing quote. This keeps the main loop focused on normal-mode logic.

**Rationale:** AI suggested a state machine; I simplified to nested loops since the quoted-field case is self-contained and doesn't need to interleave with normal parsing, making the code easier to follow.

---

### 10. Empty Rows vs Empty Fields

**Prompt:** "In CSV, what's the difference between an empty row and a row with empty fields? How do I tell them apart while parsing?"

**Original AI Response:**
"An empty row is a bare newline with no content at all — it represents zero fields. A row with empty fields has delimiters but nothing between them. For example, a bare `\n` is an empty row (zero-element vector), while `,\n` is a row with two empty string fields. The tricky part is distinguishing these while parsing character by character. Track whether you've seen any 'content' — either a delimiter or any non-newline character. If you hit a newline without having seen any content and your current field buffer is empty, return an empty vector. If you've seen a delimiter, you have at least two fields even if they're all empty strings."

**Changes Made:** Added a `seenContent` boolean that gets set to true on any delimiter or non-newline character. On newline, if `!seenContent` and the field buffer is empty, return an empty vector. Otherwise push the current field and return normally.

**Rationale:** AI explained the distinction clearly; the single boolean flag was the simplest way to track whether we've seen meaningful content without adding extra state to the parser.

---

### 11. EOF Without Trailing Newline

**Prompt:** "What should a CSV reader do if the input ends without a trailing newline character?"

**Original AI Response:**
"A robust CSV reader should handle missing trailing newlines gracefully. If you've been accumulating characters into a field and the data source hits EOF before you see a newline, treat it the same as if you'd seen a newline — push the current field into the row and return it as a valid row. Set your end flag at that point too. This handles files saved without a final newline, which is very common. The key is checking for EOF inside your main parsing loop, not just at the start of ReadRow."

**Changes Made:** Added an EOF check at the top of the main while loop. When the source is at EOF mid-row, push the current field buffer, set DEnd to true, and return true. This catches EOF regardless of where in the parsing it occurs.

**Rationale:** AI suggested checking for EOF inside the loop; placing it at the top of the loop ensures it's checked after every character is consumed, naturally handling the no-trailing-newline case without special logic.

---

### 12. Quote Character as Delimiter Fallback

**Prompt:** "What should happen if someone passes a double-quote as the delimiter character in a CSV reader or writer?"

**Original AI Response:**
"Using a double-quote as the delimiter would create an ambiguous grammar — the parser wouldn't be able to tell the difference between a field delimiter and the start or end of a quoted field. The safest approach is to detect this in the constructor and fall back to a sensible default. Comma is the universal standard CSV delimiter, so falling back to comma is the most reasonable choice. A simple conditional in the constructor handles it: if the delimiter is a quote, use comma instead. This way the class still produces valid, parseable output even with this edge case input."

**Changes Made:** Added a ternary check in both CDSVWriter and CDSVReader constructors: `(delimiter == '"') ? ',' : delimiter`. Kept it as a single expression since it's a straightforward substitution.

**Rationale:** AI suggested the fallback approach and comma as the default; I applied it to both reader and writer constructors so they handle the edge case consistently.

---

### 13. End() Flag Timing

**Prompt:** "In my CSV reader, when should I set the end flag? I want End() to return true as soon as the last row has been read, not on the next call after that."

**Original AI Response:**
"There are two places you need to set the end flag. First, at the start of ReadRow — if the source is already at EOF before you read anything, set end and return false. Second, and this one is easy to miss: after consuming a newline character, check if the source is now at EOF. If it is, set end before returning. Without this second check, after reading the last row (which ends with a newline that puts the source at EOF), End() would still return false until the next ReadRow call tries and fails. The post-newline check makes End() reflect the actual state immediately."

**Changes Made:** Added both checks. The first is at the very top of ReadRow before the main loop. The second is right after consuming the newline character, before the empty-row check or field push. This ensures End() is accurate immediately after the last row is read.

**Rationale:** AI identified the subtle timing issue; without the post-newline check, three of my tests were failing because End() returned false after the last valid row was successfully read.

---

### 14. Round-Trip Testing for CSV

**Prompt:** "How should I write tests that verify writing CSV data and reading it back produces the original data?"

**Original AI Response:**
"Round-trip tests are one of the best ways to verify format correctness. The pattern is straightforward: create a StringDataSink, write rows to it using your CDSVWriter, then wrap the sink's output string in a StringDataSource and read it back with CDSVReader. Compare each row you read against the original rows you wrote. For good coverage, you want at least one basic round-trip with simple fields, and another that exercises special characters — fields containing the delimiter, fields containing quotes, and fields containing newlines. The round-trip implicitly tests that the writer's quoting and the reader's unquoting work together correctly."

**Changes Made:** Wrote two round-trip tests. The first writes two rows of different lengths and verifies both read back correctly and End() is true after. The second focuses on a single row with all three special character types (delimiter, quote, newline) in separate fields.

**Rationale:** AI suggested the general pattern; I split into two tests to keep each one focused — one for multi-row correctness, one specifically for special character preservation.

---

### 15. shared_ptr with DataSource and DataSink

**Prompt:** "Why does the CDSVReader constructor take a shared_ptr to CDataSource instead of a raw pointer or a reference? How should I store it?"

**Original AI Response:**
"shared_ptr is used here for shared ownership semantics. The caller creates the DataSource and passes it to the reader, but both the caller and the reader need to keep it alive. A shared_ptr ensures the DataSource isn't destroyed while the reader is still using it, even if the caller's original variable goes out of scope. A raw pointer would risk a dangling pointer, and a reference can't be stored easily in a class member. Store the shared_ptr inside your pimpl struct (SImplementation) — this keeps the implementation details private and the shared ownership working correctly through the pimpl's lifetime."

**Changes Made:** Stored the shared_ptr directly in the SImplementation struct as a member variable. The constructor copies the shared_ptr into the pimpl, which keeps the reference count alive for the lifetime of the reader object.

**Rationale:** AI explained the ownership reasoning; I stored it in the pimpl struct since that's where all the other implementation state lives, keeping everything consistent with the pattern used across the project.

---

### 16. Handling CompelteELement conversion in XMLREader

**Prompt:**
How should XMLReader detect and represent a complete XML element?

**Original AI Response:**
When the end element callback is triggered, you can check whether the most recent queued entity is a StartElement with the same name. If so, you should convert that entity into a CompleteElement rather than emitting a separate EndElement. This preserves the logical structure of the XML and matches the expected entity stream. The safest approach is to modify the existing element in place rather than removing and reinserting it.

**Changes Made:**
Updated the last queued SXMLEntity directly by changing its type from StartElement to CompleteElement instead of popping and pushing a new entity.

**Rationale**
This avoids unnecessary copying and keeps the queue order intact while correctly representing complete XML elements.

---

### 17. Using a character buffer for XML character data

**Prompt:**
Why do I need to buffer character data in XMLReader instead of pushing it immediately in the character data handler?

**Original AI Response:**
XML parsers like Expat may invoke the character data handler multiple times for what logically appears as one text node. Buffering the character data ensures that consecutive chunks are merged into a single CharData entity. You should flush this buffer when starting or ending an element.

**Changes Made:**
Added a string buffer to accumulate character data and a helper function to flush it into the entity queue at appropriate boundaries.

**Rationale**
Buffering ensures correct grouping of character data and prevents fragmented CharData entities from appearing in the output.

---

### 18. Skipping character data conditionally in ReadEntity

**Prompt:**
How should the skipcdata flag in ReadEntity be handled?

**Original AI Response:**
Parse and queue character data internally, but when returning entities to the caller, it should skip over CharData entities if skipcdata is true. This allows one parsing implementation to support both behaviors.

**Changes Made:**
Implemented a loop in ReadEntity that discards CharData entities when skipcdata is enabled and continues until a non-CharData entity is found.

**Rationale**
This keeps parsing logic centralized while giving callers control over whether text nodes are relevant to their use case.

---

### 19. Using the pimpl pattern in XMLReader and XMLWriter

**Prompt:**
Why does XMLReader use a private SImplementation struct instead of putting everything directly in the class?

**Original AI Response:**
The pimpl pattern hides implementation details from the header, reduces compile-time dependencies, and keeps the public interface stable. It also allows you to store complex state (like parser handles, queues, and buffers) without exposing them.

**Changes Made:**
All internal state (parser, queue, buffers, and DataSource pointer) was stored inside SImplementation, with the public class holding only a unique pointer to it.

**Rationale**
This follows the design pattern used throughout the project and keeps the header minimal and stable.

---

### 20. Running correct test executable

**Prompt:**
./runTEsts doesn't exist so how do I run the XML tests?

**Original AI Response:**
List the contents of the test binary directory and run the appropriate executable directly, such as ./testbin/testxml.

**Changes Made:**
Ran XML tests using ./testbin/testxml.

**Rationale**
The project organizes test executables by component, so running the correct binary ensures the intended tests are executed.

---

### 21. XML CompleteElement Handling

**Prompt:**
How should I handle XML elements that have no character data between their start and end tags?

**Original AI Response:**
When parsing XML, if an element’s start tag is immediately followed by its end tag with no character data in between, it can be represented as a single complete element instead of separate start and end entities. To do this, check whether the most recent queued entity is a start element with the same name when the end tag callback fires. If so, you can convert that start element into a complete element rather than pushing a separate end element. This keeps the entity stream simpler and matches common XML parsing expectations.

**Changes Made:**
Modified the XML end-element handler to check the most recent queued entity and update its type to CompleteElement when appropriate.

**Rationale**
This approach simplifies the XML entity stream and matches the expected output format used by the provided tests.

---

### 22. Character Data Buffering in XML Parsing

**Prompt:**
Why do I need to buffer character data instead of pushing it immediately in the XML character data callback?

**Original AI Response:**
XML parsers like Expat may invoke the character data callback multiple times for what appears to be a single block of text. If you push character data immediately on each callback, you may end up with fragmented text nodes. The correct approach is to buffer character data into a string and flush it only when you encounter a structural boundary, such as a new start element or an end element. This ensures character data is grouped correctly.

**Changes Made:**
Added a character buffer string and flushed it to the entity queue only when entering or exiting an element.

**Rationale**
Buffering prevents fragmented character data entities and ensures text content is grouped correctly, which aligns with XMLReader test expectations

---

### 23. SKipping Character Data Logic

**Prompt:**
How should I implement the option to skip character data when reading XML entities?

**Original AI Response:**
The parser itself should always parse character data normally. Skipping character data should be handled at the point where entities are returned to the caller, not during parsing. This keeps the parsing logic simple and correct. When reading from the entity queue, you can simply ignore entities of type CharData if the skip flag is enabled and continue to the next entity.

**Changes Made:**
Implemented character-data skipping inside the ReadEntity function instead of modifying the parser callbacks.

**Rationale**
Separating parsing from filtering keeps the parser correct and avoids breaking XML structure while still supporting optional character-data skipping.

---

### 24. Cleaning up "Make" output

**Prompt:**
How do I make the "make" command have the output of only whether the tests passed or fail?

**Original AI Response:**
The AI explained that the extra output was coming from additional commands in the Makefile, such as coverage generation (gcov, lcov, and genhtml). It suggested separating test execution from coverage steps, using a dedicated test target that only runs the test binaries, and filtering test output using Google Test flags or shell commands so that only pass/fail results are displayed.

**Changes Made:**
Added the lines below to Makefile

// Tests to only make output show only test results and clean things up
test: dirs testbin/teststrutils testbin/teststrdatasource testbin/teststrdatasink testbin/testdsv testbin/testxml
	@./testbin/teststrutils --gtest_brief=1 2>&1 | egrep '\[  (PASSED|FAILED)  \]'
	@./testbin/teststrdatasource --gtest_brief=1 2>&1 | egrep '\[  (PASSED|FAILED)  \]'
	@./testbin/teststrdatasink --gtest_brief=1 2>&1 | egrep '\[  (PASSED|FAILED)  \]'
	@./testbin/testdsv --gtest_brief=1 2>&1 | egrep '\[  (PASSED|FAILED)  \]'
	@./testbin/testxml --gtest_brief=1 2>&1 | egrep '\[  (PASSED|FAILED)  \]'

all: test


**Rationale**
These changes were made to reduce unnecessary console output and make test results easier to read. By limiting the output to pass/fail information, it becomes clearer whether the project is functioning correctly without being distracted by coverage data or build logs.

---