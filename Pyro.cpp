#include <iostream>
#include <string_view>

#include "rapidcsv.h"

void runPyro(std::string_view csvFile) {
    rapidcsv::Document doc(csvFile.data());
    auto schema = doc.GetColumnNames();
}