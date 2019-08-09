#include <iostream>
#include <filesystem>
#include <vector>
#include <fstream>

#include "error.hxx"
#include "../toml/toml.hpp"

namespace fs = std::filesystem;

bool passBlacklistedEntries(const fs::directory_entry &entry, const std::vector<std::string_view> &blacklist) {
    for (const auto &blacklisted : blacklist) {
        if (entry.path().filename().string() == blacklisted.data())
            return false;
    }
    return true;
}

int main(int argc, char *argv[]) {
    if (argc < 2)
        return errorHandler.error(EXIT_CODE::TOO_FEW_ARGUMENTS);

    if (!fs::is_directory(argv[1]))
        return errorHandler.error(EXIT_CODE::ISNT_DIRECTORY);

    std::string input = argv[1];
    std::vector<std::string> files;
    std::ofstream output_file(input + "repostruct.txt");
    const std::string fname(input + "rsconfig.toml");

    toml::value config;

    try {
        config = toml::parse(fname);
    } catch (std::runtime_error ex) {
        return errorHandler.error(EXIT_CODE::NO_CONFIG_FILE_FOUND);
    }

    const auto blacklist = toml::find<std::vector<std::string_view>>(config, "blacklist");

    for (const auto &entry : fs::directory_iterator(input)) {
        if (entry.path().filename().string() == "repostruct.txt")
            continue;

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

    std::cout << "Successfully generated structure." << std::endl;

    return static_cast<int>(EXIT_CODE::SUCCESSFUL);
}