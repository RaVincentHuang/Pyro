#ifndef READER_H
#define READER_H

#include <string>
#include <string_view>
#include <vector>

using usize = unsigned long;

struct Attribute {
    usize index;
    std::string_view name;
};

struct Column {
    Attribute& attribute;
    std::vector<usize> data;
};

class RelationalData {
public:
    std::vector<Attribute> schema;
    std::vector<Column> columns;

    static RelationalData reader(std::string_view path);
};

#endif