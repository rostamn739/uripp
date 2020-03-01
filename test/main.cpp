#include <iostream>

void crash_me();

int main(int argc, char** argv) {
    using std::cout;
    using std::endl;
    cout << "we want to crash" << endl;
    crash_me();
    cout << "unreachable" << endl;
    (void) argc;
    (void) argv;
}
