# compile time type based schema declaration

Define xml/json or other type-based schema. Everything is defined at compile time.

The interesting features are 
- tagging occurrences/trees/arrays using strings and non-complete types
- possibility to set a restriction on occurrences separately from the definition itself via traits

# How to

## Basic usage with nodes paths describes 

Below is example of how to declare type which represents following xml structure

```xml
<MainTree>
    <CommonValue>10</CommonValue>
    <InnerTree>
        <Value>42</Value>
        <SomeTag>awesome tag</SomeTag>
    </<InnerTree>>
    <SomeArray>
        <Complex>
            <Imaginary>42.12</Imaginary>
        </Complex>
        <Complex>
            <Real>3</Real>
        </Complex>
        <Complex>
            <Real>12</Real>
            <Imaginary>42.12</Imaginary>
        </Complex>
    </SomeArray>
</MainTree>
```

for c++ code it will be looks like

```cpp
#include <schemapp/schemapp.hpp>

using namespace schemapp;
using namespace schemapp::literals;

// separated from main definition for readability
using common_value_node = entry<struct CommonValueNodeTag, "CommonValue"_fs>;

// thats too
using complex_node = tree<struct ComplexNodetag, "Complex"_fs,
                        entry<struct Re, "Real"_fs>,
                        entry<struct Im, "Imaginary"_fs>>;

using representation =
    tree<struct MainTreeTag, "MainTree"_fs,
        common_value_node
        tree<struct ComplexNodetag, "Complex"_fs,  // can be declared inside
                        entry<struct Re, "Real"_fs>,
                        entry<struct Im, "Imaginary"_fs>>,
        array<struct SomeArrayTag, "SomeArray"_fs, complex_node>>;
```

Tags can be either incomplete structures or complete structures, this can be used to more clearly show e.g. the structure of a config and its schema.

```cpp
struct Inner {
    std::string path;
}

struct Config {
    double value;
    Inner inner;
}

using config_schema = tree<Config, "Config"_fs, 
    entry<double, "Value"_fs>,
    tree<Inner, "Inner"_fs, 
        entry<std::string, "Path"_fs>>;
```

Within a single subtree, keys and tags must be unique to be able to pull internal nodes correctly

## Add constraints on nodes

You can set limits and constraints on values for a node.

```cpp

using node_with_constraints 
    = entry<struct SomeNodeTag, "Node">::constrained<min_c<0>, 
                                                     max_c<100>, 
                                                     default_value_c<50>, 
                                                     type_c<int>>;
```