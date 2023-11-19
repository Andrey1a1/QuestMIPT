#include <iostream>
#include <fstream>
#include <cstring>
#include <windows.h>

#define width 80
#define height 25

struct Location {
    char map[height][width + 1];
}loc;

void loc_LoadFromFile(const char *fileName) {
    memset(&loc.map, ' ', sizeof(loc));
    for (int i = 0; i < height; i++)
        loc.map[i][width] = '\0';

    std::ifstream file(fileName);
    std::string line;
    int lineNum = 0;
    while (std::getline(file, line) && lineNum < height) {
        strncpy(loc.map[lineNum], line.substr(0, width).c_str(), width);
        lineNum++;
    }
    file.close();

    loc.map[height - 1][width - 1] = '\0';
}


void SetCurPos()
{
    COORD coord;
    coord.X = 0;
    coord.Y = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

char map[height][width + 1];

void map_Show()
{
    SetCurPos();
    for (int i = 0; i < height; i++)
        std::cout << map[i]<<std::endl;
}

void loc_PutOnMap()
{
    memcpy(map, loc.map, sizeof(map));
}

struct Player
{
    POINT pos;
} player;

void player_Init(int x, int y)
{
    player.pos.x = x;
    player.pos.y = y;
}

void player_PutOnMap()
{
    map[player.pos.y][player.pos.x] = 1;
}

void player_Control()
{
    if (GetKeyState('W') < 0) player.pos.y--;
    if (GetKeyState('S') < 0) player.pos.y++;
    if (GetKeyState('A') < 0) player.pos.x--;
    if (GetKeyState('D') < 0) player.pos.x++;
}

int main() {
    player_Init(5, 5);
    loc_LoadFromFile("empty_map.txt");
    do
    {
        player_Control();
        loc_PutOnMap();
        player_PutOnMap();
        map_Show();
        Sleep(50);
    } while (GetKeyState(VK_ESCAPE) >= 0);
    return 0;
}
