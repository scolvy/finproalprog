#ifndef NETWORK_UTILS_HPP
#define NETWORK_UTILS_HPP

#include <string>
#include <vector>
#include <sstream>

class NetworkUtils {
public:
    static std::vector<std::string> Split(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(str);
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }
};

#endif