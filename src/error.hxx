//
// Created by zxvnme on 09.08.2019.
//

#ifndef REPOSTRUCT_ERROR_HXX
#define REPOSTRUCT_ERROR_HXX

#include <iostream>
#include <iomanip>


enum class ERROR_CODE {
    ISNT_DIRECTORY = 1,
};

class ErrorHandler {
public:
    std::string errorCodeToString(ERROR_CODE code) {
        std::string_view error_codes_strings[] = {
                " ", // Filler
                "IS_NOT_DIRECTORY",
        };
        return error_codes_strings[static_cast<int>(code)].data();
    }

    int error(ERROR_CODE code) {
        std::cout << std::setw(25) << "An error has occured." << std::endl
                  << std::setw(15) << std::left << "Why:" << std::right << this->errorCodeToString(code) << std::endl;

        return static_cast<int>(code);
    }

};

inline ErrorHandler errorHandler;

#endif // REPOSTRUCT_ERROR_HXX
