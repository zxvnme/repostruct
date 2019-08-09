#include <iostream>
#include <filesystem>
#include <vector>
#include <fstream>

#include "error.hxx"
#include "../toml/toml.hpp"

namespace fs = std::filesystem;

bool passBlacklistedEntries(const fs::directory_entry& entry, const std::vector<std::string_view>& blacklist) {
    for (const auto &blacklisted : blacklist) {
        if (entry.path().filename().string() == blacklisted.data())
            return false;
    }
    return true;
}

int main() {
    std::string output_path;
    std::string input;
    std::vector<std::string> files;
    std::cout << "Output save directory: ";
    std::cin >> output_path;

    if (!fs::is_directory(output_path))
        return errorHandler.error(ERROR_CODE::ISNT_DIRECTORY);

    std::ofstream output_file(output_path + "output.txt");

    std::cout << "Directory from where structure will be generated: ";
    std::cin >> input;

    if (!fs::is_directory(input))
        return errorHandler.error(ERROR_CODE::ISNT_DIRECTORY);

    const auto config = toml::parse(input + "rsconfig.toml");
    const auto blacklist = toml::find<std::vector<std::string_view>>(config, "blacklist");

    for (const auto &entry : fs::directory_iterator(input)) {
        if (passBlacklistedEntries(entry, blacklist)) {
            if (entry.is_directory()) {
                files.emplace_back("+-- " + entry.path().filename().string().append("/"));
                for (const auto &sub_entry : fs::directory_iterator(entry.path())) {
                    if (sub_entry.is_directory()) {
                        files.emplace_back("   +---- " + sub_entry.path().filename().string().append("/"));
                        continue;
                    }
                    files.emplace_back("   +---- " + sub_entry.path().filename().string());
                }
                continue;
            }
            files.emplace_back("+-- " + entry.path().filename().string());
        }
    }

    for (const auto &entry : files)
        output_file << std::setw(40) << std::left << entry << std::right << "// Comment placeholder" << std::endl;

    return 0;
}