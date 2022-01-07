#include <ncurses.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <random>

#define SPEED_LIMITER 10000

struct yx{
    int y, x;

    friend bool operator==(const yx& lhs, const yx& rhs){
        return (lhs.x == rhs.x && lhs.y == rhs.y);
    }
};

