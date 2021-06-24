/*
 * File:   main.c
 * Author: olipi
 *
 * Created on 12 marca 2019, 18:00
 */


#include "xc.h"
#include "allcode_api.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"

struct Cell {
    bool north;
    bool south;
    bool west;
    bool east;
    bool visited;
};

struct Cell maze[5][5];
int orientation = 0;
// 0 - upwards
// 1 - left
// 2 - right
// 3 - down
int x = 2;
int y = 5;

void init_struct() {
    int i, j;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            maze[i][j].north = false;
            maze[i][j].east = false;
            maze[i][j].west = false;
            maze[i][j].south = false;
            maze[i][j].visited = false;
        }
    }
}

void stop_in_shade() {
    if (FA_ReadLight() < 100) {
        while (1) {
            FA_SetMotors(0, 0);
            if (FA_ReadLight() > 100) {
                break;
            }
        }
    }
}

bool near_wall(int sensor) {
    if (FA_ReadIR(sensor) > 400) {
        return true;
    } else {
        return false;
    }
}

void set_cell(int orientation, int a, int b) {
    switch (orientation) {
        case 0: //when looking upwards
            if (near_wall(IR_FRONT)) {
                maze[a][b].north = true;
            }
            if (near_wall(IR_LEFT)) {
                maze[a][b].west = true;
            }
            if (near_wall(IR_RIGHT)) {
                maze[a][b].east = true;
            }
            if (near_wall(IR_REAR)) {
                maze[a][b].south = true;
            }
            maze[a][b].visited = true;
            break;
        case 1: //when looking left
            if (near_wall(IR_FRONT)) {
                maze[a][b].west = true;
            }
            if (near_wall(IR_LEFT)) {
                maze[a][b].south = true;
            }
            if (near_wall(IR_RIGHT)) {
                maze[a][b].north = true;
            }
            if (near_wall(IR_REAR)) {
                maze[a][b].east = true;
            }
            maze[a][b].visited = true;
            break;
        case 2: //when looking right
            if (near_wall(IR_FRONT)) {
                maze[a][b].east = true;
            }
            if (near_wall(IR_LEFT)) {
                maze[a][b].north = true;
            }
            if (near_wall(IR_RIGHT)) {
                maze[a][b].south = true;
            }
            if (near_wall(IR_REAR)) {
                maze[a][b].west = true;
            }
            maze[a][b].visited = true;
            break;
        case 3: //when looking down
            if (near_wall(IR_FRONT)) {
                maze[a][b].south = true;
            }
            if (near_wall(IR_LEFT)) {
                maze[a][b].east = true;
            }
            if (near_wall(IR_RIGHT)) {
                maze[a][b].west = true;
            }
            if (near_wall(IR_REAR)) {
                maze[a][b].north = true;
            }
            maze[a][b].visited = true;
            break;
    }
}

void explore() {
    set_cell(orientation, x, y);
    switch (orientation) {
        case 0:
            if (near_wall(IR_LEFT) && !near_wall(IR_FRONT)) {
                FA_Forwards(200);
                y--;
            } else if (!near_wall(IR_LEFT) && !near_wall(IR_FRONT) && maze[x - 1][y].visited == false) {
                FA_Left(90);
                FA_Forwards(200);
                x--;
                orientation = 1;
            } else if (!near_wall(IR_LEFT) && !near_wall(IR_FRONT) && maze[x - 1][y].visited == true && maze[x][y - 1].visited == false) {
                FA_Forwards(200);
                y--;
            } else if (!near_wall(IR_LEFT) && !near_wall(IR_FRONT) && !near_wall(IR_RIGHT) && maze[x - 1][y].visited == true && maze[x][y - 1].visited == true && maze[x + 1][y].visited == false) {
                FA_Right(90);
                FA_Forwards(200);
                x++;
                orientation = 2;
            } else if (near_wall(IR_LEFT) && near_wall(IR_FRONT) && near_wall(IR_RIGHT)) {
                FA_Right(180);
                FA_Forwards(200);
                y++;
                orientation = 3;
            }
        case 1:
            if (near_wall(IR_LEFT) && !near_wall(IR_FRONT)) {
                FA_Forwards(200);
                x--;
            } else if (!near_wall(IR_LEFT) && !near_wall(IR_FRONT) && maze[x][y + 1].visited == false) {
                FA_Left(90);
                FA_Forwards(200);
                y++;
                orientation = 3;
            } else if (!near_wall(IR_LEFT) && !near_wall(IR_FRONT) && maze[x][y - 1].visited == true && maze[x - 1][y].visited == false) {
                FA_Forwards(200);
                x--;
            } else if (!near_wall(IR_LEFT) && !near_wall(IR_FRONT) && !near_wall(IR_RIGHT) && maze[x][y + 1].visited == true && maze[x - 1][y].visited == true && maze[x][y - 1].visited == false) {
                FA_Right(90);
                FA_Forwards(200);
                y--;
                orientation = 0;
            } else if (near_wall(IR_LEFT) && near_wall(IR_FRONT) && near_wall(IR_RIGHT)) {
                FA_Right(180);
                FA_Forwards(200);
                x++;
                orientation = 2;
            }
        case 2:
            if (near_wall(IR_LEFT) && !near_wall(IR_FRONT)) {
                FA_Forwards(200);
                x++;
            } else if (!near_wall(IR_LEFT) && !near_wall(IR_FRONT) && maze[x][y - 1].visited == false) {
                FA_Left(90);
                FA_Forwards(200);
                y--;
                orientation = 0;
            } else if (!near_wall(IR_LEFT) && !near_wall(IR_FRONT) && maze[x][y - 1].visited == true && maze[x + 1][y].visited == false) {
                FA_Forwards(200);
                x++;
            } else if (!near_wall(IR_LEFT) && !near_wall(IR_FRONT) && !near_wall(IR_RIGHT) && maze[x][y - 1].visited == true && maze[x + 1][y].visited == true && maze[x][y + 1].visited == false) {
                FA_Right(90);
                FA_Forwards(200);
                y++;
                orientation = 3;
            } else if (near_wall(IR_LEFT) && near_wall(IR_FRONT) && near_wall(IR_RIGHT)) {
                FA_Right(180);
                FA_Forwards(200);
                x--;
                orientation = 1;
            }
        case 3:
            if (near_wall(IR_LEFT) && !near_wall(IR_FRONT)) {
                FA_Forwards(200);
                y++;
            } else if (!near_wall(IR_LEFT) && !near_wall(IR_FRONT) && maze[x + 1][y].visited == false) {
                FA_Left(90);
                FA_Forwards(200);
                x++;
                orientation = 2;
            } else if (!near_wall(IR_LEFT) && !near_wall(IR_FRONT) && maze[x + 1][y].visited == true && maze[x][y + 1].visited == false) {
                FA_Forwards(200);
                y++;
            } else if (!near_wall(IR_LEFT) && !near_wall(IR_FRONT) && !near_wall(IR_RIGHT) && maze[x + 1][y].visited == true && maze[x][y + 1].visited == true && maze[x - 1][y].visited == false) {
                FA_Right(90);
                FA_Forwards(200);
                x--;
                orientation = 1;
            } else if (near_wall(IR_LEFT) && near_wall(IR_FRONT) && near_wall(IR_RIGHT)) {
                FA_Right(180);
                FA_Forwards(200);
                y--;
                orientation = 0;
            }
    }
}

void drive_straight() {
    static int lm = 0;
    static int rm = 0;
    FA_SetMotors(lm, rm);
    if (FA_ReadIR(2) > 100) {
        lm = 0;
        rm = 0;
    }
    if (FA_ReadIR(2) < 1700 && FA_ReadIR(3) < 1700 && FA_ReadIR(1) < 1700) {
        lm = 20;
        rm = 20;
    }
    if (FA_ReadIR(1) > 1200) {
        lm = 25;
        rm = -25;
    }
    if (FA_ReadIR(0) > 1200) {
        lm = 30;
        rm = -30;
    }
    if (FA_ReadIR(3) > 1200) {
        lm = -25;
        rm = 25;
    }
    if (FA_ReadIR(4) > 1200) {
        lm = -30;
        rm = 30;
    }
}

void detect_line() {
    if (FA_ReadLine(0) < 50 || FA_ReadLine(1) < 50) {
        while (FA_ReadLine(0) < 50 || FA_ReadLine(1) < 50) {
            //when line is crossed
        }
        FA_DelayMillis(250);
        set_cell(orientation, x, y);
    }
}

int main(void) {
    FA_RobotInit();
    init_struct();
    while (1) {
        if (near_wall(IR_FRONT)) {
            FA_Forwards(100);
        }
    }
    return 0;
}
