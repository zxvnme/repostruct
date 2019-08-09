#include <iostream>
#include <filesystem>
#include <vector>
#include <fstream>

#include "error.hxx"

namespace fs = std::filesystem;

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

    for (const auto &entry : fs::directory_iterator(input)) {
        if (entry.path().filename() == "output.txt")
            continue;

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

    for (const auto &entry : files)
        output_file << std::setw(40) << std::left << entry << std::right << "// Comment placeholder" << std::endl;

    return 0;
}