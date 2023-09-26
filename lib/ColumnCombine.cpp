#include "ColumnCombine.h"
#include "RelationalData.h"
#include "Trie.h"
#include <string>

std::string ColumnCombine::toString() {
    std::string res = "[";
    for(auto index = indices.find_first();
        index != BitSet::npos;
        index = indices.find_next(index + 1)) {
        auto str = schema.columns.at(index)->name;
        res += str;
        res += ", ";
    }

    return res + "]";
}