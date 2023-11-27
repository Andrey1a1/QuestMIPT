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

struct Player {
    char name[20];
    POINT pos;
}player;

void player_Init(int x, int y, const char *name)
{
    player.pos.x = x;
    player.pos.y = y;
    strcpy(player.name, name);
}

void player_PutOnMap()
{
    map[player.pos.y][player.pos.x] = 1;
}

void player_Control()
{
    POINT old = player.pos;
    if (GetKeyState('W') < 0) player.pos.y--;
    if (GetKeyState('S') < 0) player.pos.y++;
    if (GetKeyState('A') < 0) player.pos.x--;
    if (GetKeyState('D') < 0) player.pos.x++;
    if (map[player.pos.y][player.pos.x] != ' ')
        player.pos = old;
}

void player_Save() {
    std::ofstream file(player.name, std::ios::out | std::ios::binary);
    file.write(reinterpret_cast<const char*>(&player), sizeof(player));
    file.close();
}

void player_Load(const char *name) {
    std::ifstream file(name, std::ios::in | std::ios::binary);
    if (!file.good())
        player_Init(5, 5, name);
    else
        file.read(reinterpret_cast<char*>(&player), sizeof(player));
    file.close();
}

int main() {
    player_Load("Andrey");
    loc_LoadFromFile("empty_map.txt");
    do
    {
        player_Control();
        loc_PutOnMap();
        player_PutOnMap();
        map_Show();
        Sleep(50);
    } while (GetKeyState(VK_ESCAPE) >= 0);
    player_Save();
    return 0;
}
