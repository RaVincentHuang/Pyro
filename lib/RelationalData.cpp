#include "RelationalData.h"
#include "PLICache.h"
#include "ColumnData.h"
#include "rapidcsv.h"
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

using std::shared_ptr;

RelationalData RelationalData::reader(std::string_view path) {
    rapidcsv::Document doc(path.data());
    auto names = doc.GetColumnNames();
    
    Schema schema;
    for(auto attr : names) {
        schema.columns.push_back(std::make_shared<Column>(schema, attr, static_cast<size_t>(schema.columns.size())));
    }

    std::vector<ColumnData> columns;

    for(size_t i = 0; i < schema.columns.size(); i++) {
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
        auto pli = std::make_shared<PLI>(data);
        columns.push_back({schema.columns.at(i), std::vector<ValueId>(pli->getProbTable()), pli});
    }


    return {schema, columns};
}

std::shared_ptr<Vertical> Schema::getVertical(const boost::dynamic_bitset<size_t>& bitset) const {

}