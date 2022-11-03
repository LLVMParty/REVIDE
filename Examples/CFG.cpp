// clang -c CFG.cpp.bc -o CFG.bc -emit-llvm -g -fno-discard-value-names

#include <cstdio>
#include <cstdlib>

extern "C" int fn_cfg(int n, int m) {
    if((n & 3) == 0) {
        n += 6;
        if(n < m) {
            m += (n + n);
        } else {
            n += (m + m);
        }
    }
    else if(n * 2 < m) {
        m += n;
    } else {
        m -= n;
    }
    return (m << 5) ^ n;
}

extern "C" int fn_switch(int arg) {
    switch(arg) {
        case 1:
            return arg % 33;
        case 2:
            return 312;
        case 3:
            return 123;
        case 4:
            return arg + arg;
        case 5:
            return arg * 42;
        case 6:
            return arg + 37;
        case 7:
            return 8471;
        case 8:
            return 93847 ^ arg;
        case 9:
            return 348743 & arg;
        case 10:
            return 1354;
        case 11:
            return 2387;
        default:
            return 1337;
    }
}

int main(int argc, char** argv) {
    if(argc < 2) {
        puts("Usage: test number");
        return 1;
    }

    auto n = atoi(argv[1]);
    auto x = fn_switch(n);
    return fn_cfg(n, x);

    return 0;
}