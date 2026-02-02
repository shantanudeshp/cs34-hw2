# CDSVReader

Reads rows of string fields from a delimiter-separated value (DSV) format via a `CDataSource`.

## Constructor

```cpp
CDSVReader(std::shared_ptr<CDataSource> src, char delimiter);
```

- `src` — data source to read input characters from
- `delimiter` — character used to separate fields (if `"` is passed, falls back to `,`)

## Methods

### End

```cpp
bool End() const;
```

Returns true if all rows have been read from the source. Becomes true immediately after the last row is read — no extra call to `ReadRow` is needed to detect it.

### ReadRow

```cpp
bool ReadRow(std::vector<std::string> &row);
```

Reads the next row from the source into `row`. Returns true if a row was successfully read, false if there is no more data.

**Parsing rules:**
- Fields are separated by the delimiter character
- A field wrapped in `"` is a quoted field — the delimiter and newlines inside it are treated as literal characters
- Inside a quoted field, `""` represents a single literal `"` character
- A bare newline (no fields before it) is an empty row — `row` will be an empty vector
- A delimiter followed by a newline (e.g. `,\n`) produces empty string fields, not an empty row
- If the input ends without a trailing newline, the last row is still returned

## Examples

```cpp
// reading simple fields
auto src = std::make_shared<CStringDataSource>("a,b,c\n");
CDSVReader reader(src, ',');
std::vector<std::string> row;
reader.ReadRow(row);  // row == {"a", "b", "c"}

// quoted field with delimiter inside
auto src2 = std::make_shared<CStringDataSource>("\"hello, world\",foo\n");
CDSVReader reader2(src2, ',');
reader2.ReadRow(row);  // row == {"hello, world", "foo"}

// escaped quote inside quoted field
auto src3 = std::make_shared<CStringDataSource>("\"she said \"\"hi\"\"\",ok\n");
CDSVReader reader3(src3, ',');
reader3.ReadRow(row);  // row == {"she said \"hi\"", "ok"}

// empty row vs empty fields
auto src4 = std::make_shared<CStringDataSource>("\n,\n");
CDSVReader reader4(src4, ',');
reader4.ReadRow(row);  // row == {} (empty row, zero fields)
reader4.ReadRow(row);  // row == {"", ""} (two empty fields)

// reading multiple rows until End()
auto src5 = std::make_shared<CStringDataSource>("x,y\n1,2\n");
CDSVReader reader5(src5, ',');
while(!reader5.End()){
    reader5.ReadRow(row);
    // first iteration: row == {"x", "y"}
    // second iteration: row == {"1", "2"}
}
```
