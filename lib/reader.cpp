#include "rapidcsv.h"
#include <string_view>

void reader(std::string_view path) {
    rapidcsv::Document doc(path.data());
    auto schema = doc.GetColumnNames();
    
}