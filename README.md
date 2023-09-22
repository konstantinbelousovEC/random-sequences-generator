# random-sequences-generator

lib of generation sequences (stl containers) of scalar or user-provided types

now it supports:

sequenced containers:

- std::vector
- std::deque
- std::array
- std::list
- std::forward_list

associative containers:

- std::map
- std::unordered_map
- std::set
- std::unordered_set

types:

- all integral and float-point types
- std::string (via user-defined possible char collection)
- user-provided types with explcitly defined value generators by user

examples of using you can see in main.cpp file

## TODO

- opportunity to generate strings according to regular expressions
- more built-in generators for frequently used structures
