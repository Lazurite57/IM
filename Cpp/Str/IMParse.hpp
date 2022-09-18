#pragma once
#ifndef _IMPARSE_HPP_
#define _IMPARSE_HPP_

#include "../IMC.h"

void split(const std::string& s,
    std::vector<std::string>& sv,
                   const char delim) {
    sv.clear();
    std::istringstream iss(s);
    std::string temp;

    while (std::getline(iss, temp, delim)) {
        sv.emplace_back(std::move(temp));
    }
    return;
}

#endif