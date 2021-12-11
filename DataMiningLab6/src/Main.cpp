#include "Main/Main.hpp"

int main(int argc, char* argv[]) {
    return std::make_unique<Main>(argc, argv)->LaunchMain();
}
