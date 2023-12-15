#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include <fstream>

using namespace std;
struct point {
    unsigned is_live : 1;
};
//на самом деле ширина
#define __WORLD_HEIGHT__ 80

//на самом деле высота
#define __WORLD_WIDTH__ 47

// Игровое поле размером 10x10 клеток
point world[__WORLD_WIDTH__][__WORLD_HEIGHT__];

/*
 * Инициализация первого поколения игры псевдослучайными значениями
 */
void init_world(point world[][__WORLD_HEIGHT__])
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10000);

    unsigned int i, j;

    for (i = 0; i < __WORLD_WIDTH__; i++) {
        for (j = 0; j < __WORLD_HEIGHT__; j++) {
            unsigned int num = dis(gen);
            if (num % 2 == 0) {
                world[i][j].is_live = 1;
            }
            else {
                world[i][j].is_live = 0;
            }
        }
    }
}
void init_shape_boat(point world[][__WORLD_HEIGHT__]) {
    world[__WORLD_WIDTH__ / 2 - 1][__WORLD_HEIGHT__ / 2 - 1].is_live = 1;
    world[__WORLD_WIDTH__ / 2 ][__WORLD_HEIGHT__ / 2 - 1].is_live = 1;
    world[__WORLD_WIDTH__ / 2 - 1][__WORLD_HEIGHT__ / 2 ].is_live = 1;
    world[__WORLD_WIDTH__ / 2 + 1][__WORLD_HEIGHT__ / 2 ].is_live = 1;
    world[__WORLD_WIDTH__ / 2][__WORLD_HEIGHT__ / 2 +1].is_live = 1;
}
void init_shape_blinker(point world[][__WORLD_HEIGHT__]) {
    world[__WORLD_WIDTH__ / 2 - 1][__WORLD_HEIGHT__ / 2 - 1].is_live = 1;
    world[__WORLD_WIDTH__ / 2][__WORLD_HEIGHT__ / 2 - 1].is_live = 1;
    world[__WORLD_WIDTH__ / 2 + 1][__WORLD_HEIGHT__ / 2 - 1].is_live = 1;
}
void init_shape_longboat(point world[][__WORLD_HEIGHT__]) {
    world[__WORLD_WIDTH__ / 2 +1 ][__WORLD_HEIGHT__ / 2 + 1].is_live = 1;
    world[__WORLD_WIDTH__ / 2][__WORLD_HEIGHT__ / 2 + 1].is_live = 1;
    world[__WORLD_WIDTH__ / 2 + 1][__WORLD_HEIGHT__ / 2].is_live = 1;
    world[__WORLD_WIDTH__ / 2 - 1][__WORLD_HEIGHT__ / 2].is_live = 1;
    world[__WORLD_WIDTH__ / 2][__WORLD_HEIGHT__ / 2 - 1].is_live = 1;
    world[__WORLD_WIDTH__ / 2 - 2][__WORLD_HEIGHT__ / 2-1].is_live = 1;
    world[__WORLD_WIDTH__ / 2-1][__WORLD_HEIGHT__ / 2 - 2].is_live = 1;
}
void init_shape_glider(point world[][__WORLD_HEIGHT__]) {
    world[__WORLD_WIDTH__ / 2 + 1][__WORLD_HEIGHT__ / 2 + 1].is_live = 1;
    world[__WORLD_WIDTH__ / 2][__WORLD_HEIGHT__ / 2 + 1].is_live = 1;
    world[__WORLD_WIDTH__ / 2 + 1][__WORLD_HEIGHT__ / 2].is_live = 1;
    world[__WORLD_WIDTH__ / 2 + 1][__WORLD_HEIGHT__ / 2-1].is_live = 1;
    world[__WORLD_WIDTH__ / 2-1][__WORLD_HEIGHT__ / 2 ].is_live = 1;
}
void init_shape_lightweight(point world[][__WORLD_HEIGHT__]) {
    world[__WORLD_WIDTH__ / 2 + 1][__WORLD_HEIGHT__ / 2 - 1].is_live = 1;
    world[__WORLD_WIDTH__ / 2][__WORLD_HEIGHT__ / 2 - 1].is_live = 1;
    world[__WORLD_WIDTH__ / 2-1][__WORLD_HEIGHT__ / 2 - 1].is_live = 1;
    world[__WORLD_WIDTH__ / 2-2][__WORLD_HEIGHT__ / 2 - 1].is_live = 1;
    world[__WORLD_WIDTH__ / 2 + 1][__WORLD_HEIGHT__ / 2].is_live = 1;
    world[__WORLD_WIDTH__ / 2 -3][__WORLD_HEIGHT__ / 2].is_live = 1;
    world[__WORLD_WIDTH__ / 2 + 1][__WORLD_HEIGHT__ / 2+1].is_live = 1;
    world[__WORLD_WIDTH__ / 2][__WORLD_HEIGHT__ / 2 + 2].is_live = 1;
    world[__WORLD_WIDTH__ / 2-3][__WORLD_HEIGHT__ / 2 + 2].is_live = 1;
}
void init_from_file(point world[][__WORLD_HEIGHT__]) {
    fstream f;
    f.open("Initiation.txt", ios::in);
    if (!f.is_open()) { cout << "File Initiation.txt not found.\n"; }
    else {
        if (f.eof()) { cout << "File was empty.\n"; }
        else {
            char tmp;
            int i = 0, j = 0;
            while (!f.eof()) {
                f >>noskipws>> tmp;
                if (tmp == '.') { j++; }
                else if (tmp == 'X') { world[i][j].is_live = 1; j++; }
                else if (tmp == '\n') { j = 0; i++; }
            }
            
            
        }
        
    }
}


/*
 * Количество живых клеток на игровом поле
*/
unsigned int get_live_count(point world[][__WORLD_HEIGHT__])
{
    unsigned int count = 0;
    unsigned i, j;
    for (i = 0; i < __WORLD_WIDTH__; i++) {
        for (j = 0; j < __WORLD_HEIGHT__; j++) {
            if (world[i][j].is_live == 1) {
                count++;
            }
        }
    }
    return count;
}
void read_point_neighbors(signed int nb[][2], signed int x, signed int y)
{
    signed int i, j;
    unsigned int k = 0;

    for (i = x - 1; i <= x + 1; i++) {
        for (j = y - 1; j <= y + 1; j++) {
            if (i == x && j == y) {
                continue;
            }
            if (i < 0) nb[k][0] = __WORLD_WIDTH__ - 1;
            else if (i == __WORLD_WIDTH__) nb[k][0] = 0;
            else nb[k][0] = i;
            if (j < 0) nb[k][1] = __WORLD_HEIGHT__ - 1;
            else if (j == __WORLD_HEIGHT__) nb[k][1] = 0;
            else nb[k][1] = j;
            k++;
        }
    }
}
/*
 * Количество живых соседей у клетки с координатами x, y
 */
unsigned int count_live_neighbors(point world[][__WORLD_HEIGHT__], signed int x, signed int y)
{
    unsigned int count = 0;
    unsigned int i;
    signed int nb[8][2];
    signed int _x, _y;

    read_point_neighbors(nb, x, y);

    for (i = 0; i < 8; i++) {
        _x = nb[i][0];
        _y = nb[i][1];

        if (_x < 0 || _y < 0) {
            if (_x < 0)_x = __WORLD_WIDTH__ - 1;
            if (_y<0) _y = __WORLD_HEIGHT__ - 1;
        }
        if (_x >= __WORLD_WIDTH__ || _y >= __WORLD_HEIGHT__) {
            if (_x >= __WORLD_WIDTH__)_x = 0;
            if(_y>=__WORLD_HEIGHT__) _y = 0;
        }

        if (world[_x][_y].is_live == 1) {
            count++;
        }
    }

    return count;
}
/*
 * Сгенерировать следующее поколение игрового мира
 */
void next_generation(point world[][__WORLD_HEIGHT__], point prev_world[][__WORLD_HEIGHT__], unsigned int newlife, unsigned int lowdeathbar, unsigned int highdeathbar)
{
    unsigned int i, j;
    unsigned int live_nb;
    point p;

    for (i = 0; i < __WORLD_WIDTH__; i++) {
        for (j = 0; j < __WORLD_HEIGHT__; j++) {
            p = prev_world[i][j];
            live_nb = count_live_neighbors(prev_world, i, j);

            if (p.is_live == 0) {
                if (live_nb == newlife) {
                    world[i][j].is_live = 1;
                }
            }
            else {
                if (live_nb < lowdeathbar || live_nb > highdeathbar) {
                    world[i][j].is_live = 0;
                }
            }
        }
    }
}
/*
 * Копирование игрового мира. Используется для сохранения предыдущего поколения
*/
void copy_world(point src[][__WORLD_HEIGHT__], point dest[][__WORLD_HEIGHT__])
{
    unsigned int i, j;
    for (i = 0; i < __WORLD_WIDTH__; i++) {
        for (j = 0; j < __WORLD_HEIGHT__; j++) {
            dest[i][j] = src[i][j];
        }
    }
}


/*
 * Сравнение игровых миров текущего и предыдущего поколения
 */
int cmp_world(point w1[][__WORLD_HEIGHT__], point w2[][__WORLD_HEIGHT__])
{
    unsigned int i, j;
    for (i = 0; i < __WORLD_WIDTH__; i++) {
        for (j = 0; j < __WORLD_HEIGHT__; j++) {
            if (w1[i][j].is_live != w2[i][j].is_live) {
                return -1;
            }
        }
    }
    return 0;
}
/*
 * Вывести на экран игровое поле
*/
void print_world(point world[][__WORLD_HEIGHT__])
{

    unsigned int i, j;
    for (i = 0; i < __WORLD_HEIGHT__*2;i++) {
        cout << '_';
    }
    cout << endl;
    for (i = 0; i < __WORLD_WIDTH__; i++) {
        for (j = 0; j < __WORLD_HEIGHT__; j++) {
            if (world[i][j].is_live == 1) {
                cout << '*';
            }
            else {
                cout << ' ';
            }
            cout << ' ';
        }
        cout << '|'<<endl;
    }
    for (i = 0; i < __WORLD_HEIGHT__ * 2; i++) {
        cout << '_';
    }
}
int main()
{
    unsigned int newlife = 3;
    unsigned int lowdeathbar = 2;
    unsigned int highdeathbar = 3;
    unsigned int worldgeneration= 0;
    unsigned int outputspeed = 500;
    char c;
    bool flagexit = 0;
    cout << "Hello!\n";
    while (1) {
        cout << "Please choose launch options:\n1) Change rules;\n2) Specific shape to generate;\n3) Change output speed;\n4) Launch (If no options chosen then default launch);\n";
        cin >> c;
        switch (c) {
        case '1':
            cout << "Choose the amount of neighbors to create new life(1-7): ";
            cin >> newlife;
            if (newlife < 1 || newlife>8) { cout << "Wrong input, will use default.\n"; newlife = 3; }
            cout << "Please enter the low death bar, that will kill a square(0-"<<newlife-1<<"): ";
            cin >> lowdeathbar;
            if (lowdeathbar < 0 || lowdeathbar>newlife-1) { cout << "Wrong input, will use "<<newlife-1<<"."; lowdeathbar=newlife-1; }
            else lowdeathbar++;
            cout << "Please enter the high death bar, that will kill a square(" << newlife+1 << "-8): ";
            cin >> highdeathbar;
            if (highdeathbar < newlife + 1 || highdeathbar>8) { cout << "Wrong input, will use "<<newlife+1<<"."; highdeathbar = newlife + 1; }
            else highdeathbar--;
            break;
        case '2':
            cout << "Choose shape to generate:\n1) Random generation\n2) Boat\n3) Longboat\n4) Glider\n5) Light Starship\n6) Blinker\n0) Generate from file\n";
            cin >> c;
            switch (c) {
            case'1':
                cout << "Random generation chosen.\n";
                worldgeneration = 0;
                break;
            case'2':
                cout << "Boat generation chosen.\n";
                worldgeneration = 1;
                break;
            case'3':
                cout << "Longboat generation chosen.\n";
                worldgeneration = 2;
                break;
            case'4':
                cout << "Glider generation chosen.\n";
                worldgeneration = 3;
                break;
            case'5':
                cout << "Light starship generation chosen.\n";
                worldgeneration = 4;
                break;
            case'6':
                cout << "Blinker generation chosen.\n";
                worldgeneration = 5;
                break;
            case'0':
                cout << "Please make sure to set correct screen size, output speed and make sure that file Initiation.txt exists.\n";
                worldgeneration = 9;
                break;
            default:
                cout << "Wrong input, random generation chosen.\n";
                worldgeneration = 0;
                break;
            }
            break;
        case '3':
            cout << "Please enter output speed (100-2000): ";
            cin >> outputspeed;
            if (outputspeed == 0 || outputspeed > 2000) { cout << "Wrong input, will set speed to 500\n"; outputspeed = 500; }
            break;
        case '4':
            flagexit = 1;
            break;
        default:
            cout << "Wrong input. Will launch with default options.\n";
            break;

        }
        if (flagexit == 1)break;
    }


    point world[__WORLD_WIDTH__][__WORLD_HEIGHT__];
    point prev_world[__WORLD_WIDTH__][__WORLD_HEIGHT__];

    if(worldgeneration==0)init_world(world);
    else if(worldgeneration==1)init_shape_boat(world);
    else if(worldgeneration==2)init_shape_longboat(world);
    else if(worldgeneration==3)init_shape_glider(world);
    else if (worldgeneration == 4)init_shape_lightweight(world);
    else if (worldgeneration == 5)init_shape_blinker(world);
    else if (worldgeneration == 9)init_from_file(world);


    unsigned int live_points = -1;
    bool is_optimal = false;

    do {
        print_world(world);
        copy_world(world, prev_world);
        next_generation(world, prev_world, newlife, lowdeathbar, highdeathbar);

        is_optimal = cmp_world(world, prev_world) == 0;
        live_points = get_live_count(world);

        if (is_optimal) {
            cout << "Optimal configuration detected" << endl;
        }

        if (live_points == 0) {
            cout << "All points died" << endl;
        }
        this_thread::sleep_for(std::chrono::milliseconds(outputspeed));
        system("CLS");
    } while (live_points != 0 && !is_optimal);

    return 0;
}