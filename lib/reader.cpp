#include "reader.h"
#include "rapidcsv.h"
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <vector>

RelationalData RelationalData::reader(std::string_view path) {
    rapidcsv::Document doc(path.data());
    auto tmp = doc.GetColumnNames();
    std::vector<Attribute> schema;
    usize cnt = 0;
    for(auto attri : tmp) {
        schema.push_back({cnt++, attri});
    }
    std::vector<Column> columns;
    for(auto i = 0UL; i < schema.size(); i++) {
        auto column = doc.GetColumn<std::string>(i);
        std::vector<usize> data;
        std::unordered_map<std::string, usize> check;
        for(auto str : column) {
            if(check.count(str)) {
                data.push_back(check.at(str));
            } else {
                auto index = check.size(); 
                check[str] = index;
                data.push_back(index);
            }
        }
        columns.push_back({schema.at(i), data});
    }

    return {schema, columns};   
}