#include <iostream>
#include <fstream>
#include <cstring>

#define width 80
#define height 25

struct Location {
    char mp[height][width + 1];
};

void loc_LoadFromFile(Location &loc, const char *fileName) {
    memset(&loc.mp, ' ', sizeof(loc));
    for (int i = 0; i < height; i++)
        loc.mp[i][width] = '\0';

    std::ifstream f(fileName);
    char c[80];
    int line = 0;
    while (f.getline(c, width)) {
        int cnt = strlen(c);
        if (c[cnt - 1] == '\n')
            cnt--;
        strncpy(loc.mp[line], c, cnt);
        line++;
    }
    f.close();

    loc.mp[height - 1][width - 1] = '\0';
}

int main() {
    Location loc;
    loc_LoadFromFile(loc, "empty_map.txt");
    for (int i = 0; i < height; i++)
        std::cout << loc.mp[i] << std::endl;
    return 0;
}
