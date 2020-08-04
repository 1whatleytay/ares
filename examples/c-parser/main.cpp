#include <fmt/printf.h>

#include <fstream>

int main(int count, char **args) {
    if (count != 2) {
        fmt::print("Usage: c-parser /path/to/source.c");
        return 0;
    }

    std::ifstream stream(args[1], std::ios::ate);
    std::vector<char> data(stream.tellg());
    stream.seekg(0, std::ios::beg);
    stream.read(data.data(), data.size());
    stream.close();

    std::string source(data.begin(), data.end());
}
