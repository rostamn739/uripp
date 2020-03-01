#include <exception>

void crash_me() {
    throw std::exception();
}
