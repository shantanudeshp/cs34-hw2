# CDSVWriter

Writes rows of string fields to a delimiter-separated value (DSV) format via a `CDataSink`.

## Constructor

```cpp
CDSVWriter(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall = false);
```

- `sink` — destination to write output characters to
- `delimiter` — character used to separate fields (if `"` is passed, falls back to `,`)
- `quoteall` — if true, every field is quoted regardless of content

## Methods

### WriteRow

```cpp
bool WriteRow(const std::vector<std::string> &row);
```

Writes a single row of fields followed by a newline. Returns true on success.

**Quoting rules:**
- A field is quoted if it contains the delimiter, a `"`, or a `\n`
- If `quoteall` is true, every field is quoted
- Fields that don't need quoting are written as-is
- Inside a quoted field, literal `"` characters are escaped by doubling them (`""`)

**Empty row:** passing an empty vector writes just a newline (`\n`), which represents a valid row with zero fields.

## Examples

```cpp
// basic csv output
auto sink = std::make_shared<CStringDataSink>();
CDSVWriter writer(sink, ',');
writer.WriteRow({"name", "age", "city"});
// sink->String() == "name,age,city\n"

// field containing the delimiter gets quoted
writer.WriteRow({"hello, world", "foo"});
// output: "hello, world",foo\n

// field containing a quote gets quoted and escaped
writer.WriteRow({"she said \"hi\"", "ok"});
// output: "she said ""hi""",ok\n

// quoteall mode
auto sink2 = std::make_shared<CStringDataSink>();
CDSVWriter writer2(sink2, ',', true);
writer2.WriteRow({"a", "b"});
// sink2->String() == "\"a\",\"b\"\n"

// tab-delimited
auto sink3 = std::make_shared<CStringDataSink>();
CDSVWriter writer3(sink3, '\t');
writer3.WriteRow({"col1", "col2", "col3"});
// sink3->String() == "col1\tcol2\tcol3\n"
```
