#include "RelationalData.h"
#include "rapidcsv.h"
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

RelationalData RelationalData::reader(std::string_view path) {
    rapidcsv::Document doc(path.data());
    auto names = doc.GetColumnNames();
    
    std::vector<Attribute> schema;
    for(auto attr : names) {
        schema.push_back({attr, static_cast<ValueId>(schema.size())});
    }

    std::vector<Column> columns;

    for(ValueId i = 0; i < schema.size(); i++) {
        auto value = doc.GetColumn<std::string>(i);
        std::unordered_map<std::string, ValueId> check;
        std::vector<ValueId> data;
        for(auto str : value) {
            if(check.count(str)) {
                data.push_back(check.at(str));
            } else {
                auto len = check.size();
                check[str] = len;
                data.push_back(len);
            }
        }
        columns.push_back({schema.at(i), data});
    }


    return {schema, columns};
}
