C++ has no `text/template`. The lesson includes a small hand-written `Template` class that supports `{{.}}`, `{{.Field}}`, `{{if}}`/`{{else}}`/`{{end}}`, `{{range}}`/`{{end}}` and `-` whitespace trimming. Data is a `std::variant` of a string, a list of strings, or a map of fields (standing in for Go's struct/map).
___
#### To run the program, make sure you are in the program's folder, then use the Run Command below.
___
##### Run Command:

`$ zig run -lc++ -cflags -std=c++23 -- text-templates.cpp`

##### Results:

`Value: some text`
`Value: 5`
`Value: [Go Rust C++ C#]`
`Name: Jane Doe`
`Name: Mickey Mouse`
`yes `
`no `
`Range: Go Rust C++ C# `
