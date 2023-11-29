#include <iostream>
#include <fstream>
#include <cstring>
#include <windows.h>

#define width 80
#define height 25

struct Location {
    char map[height][width + 1];
    POINT sz;
}loc;

void loc_LoadFromFile(const char *fileName) {
    memset(&loc.map, ' ', sizeof(loc));
    for (int i = 0; i < height; i++)
        loc.map[i][width] = '\0';

    std::ifstream file(fileName);
    if (file.is_open()) {
        std::string line;
        loc.sz.x = 0;
        loc.sz.y = 0;

        while (std::getline(file, line) && loc.sz.y < height) {
            int cnt = line.size();
            if (cnt > width) cnt = width;
            std::strncpy(loc.map[loc.sz.y], line.c_str(), cnt);
            loc.map[loc.sz.y][cnt] = '\0';
            loc.sz.y++;
            if (cnt > loc.sz.x) loc.sz.x = cnt;
        }
        file.close();
    }
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
    POINT locPos;
}player;

void player_LoadLocation() {
    std::string fileName = "map_" + std::to_string(player.locPos.x) + "_" + std::to_string(player.locPos.y) + ".txt";
    loc_LoadFromFile(fileName.c_str());
}

void player_Init(int xLoc, int yLoc, int x, int y, const char *name)
{
    player.pos.x = x;
    player.pos.y = y;
    strcpy(player.name, name);
    player.locPos.x = xLoc;
    player.locPos.y = yLoc;
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
    
    if (player.pos.x > loc.sz.x - 2) {
        player.locPos.x++;
        player_LoadLocation();
        player.pos.x = 1;
    }
    if (player.pos.x < 1) {
        player.locPos.x--;
        player_LoadLocation();
        player.pos.x = loc.sz.x - 2;
    }

    if (player.pos.y > loc.sz.y - 2) {
        player.locPos.y++;
        player_LoadLocation();
        player.pos.y = 1;
    }
    if (player.pos.y < 1) {
        player.locPos.y--;
        player_LoadLocation();
        player.pos.y = loc.sz.y - 2;
    }
}

void player_Save() {
    std::ofstream file(player.name, std::ios::out | std::ios::binary);
    file.write(reinterpret_cast<const char*>(&player), sizeof(player));
    file.close();
}

void player_Load(const char *name) {
    std::ifstream file(name, std::ios::in | std::ios::binary);
    if (!file.good())
        player_Init(0, 0, 5, 5, name);
    else
        file.read(reinterpret_cast<char*>(&player), sizeof(player));
    file.close();
}

int main() {
    player_Load("Andrey");
    player_LoadLocation();
    do
    {
        player_Control();
        loc_PutOnMap();
        player_PutOnMap();
        map_Show();
        Sleep(30);
    } while (GetKeyState(VK_ESCAPE) >= 0);
    player_Save();
    return 0;
}
