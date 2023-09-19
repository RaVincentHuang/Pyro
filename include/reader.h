#ifndef READER_H
#define READER_H

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>


struct Attribute {
    size_t index;
    std::string_view name;
};

struct Column {
    Attribute& attribute;
    std::vector<size_t> data;
};

class RelationalData {
public:
    std::vector<Attribute> schema;
    std::vector<Column> columns;

    static RelationalData reader(std::string_view path);
};

#endif