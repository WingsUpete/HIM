#include <string>

#include "maze.h"

Maze::Maze(int num_of_row, int num_of_col, double _len) {
    this->row = num_of_row / 2;
    this->col = num_of_col / 2;

    this->len = _len;

    this->row = this->row <= 0 ? 11 : this->row;
    this->col = this->col <= 0 ? 11 : this->col;

    this->maze_map = new int *[2 * row + 4];
    for (int i = 0; i < 2 * row + 4; ++i) {
        this->maze_map[i] = new int[2 * col + 4];
    }
    this->make_maze();

    this->start = glm::vec2(1, 0);
    this->end = glm::vec2(get_row_num() - 2, get_col_num() - 1);

    while (true) {
        int i = rand() % (row * 2 + 1);
        int j = rand() % (col * 2 + 1);
        if (!isWall(i, j) && !isStartPoint(i, j) && !isEndPoint(i, j)) {
            this->thingOne.xPos = i;
            this->thingOne.yPos = j;
            thingOneCollected = false;
            this->thingOne.position = glm::vec3(i * len, -0.8, j * len);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, this->thingOne.position);
            model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
            this->thingOne.model = model;
            this->thingOne.model_res = glm::mat3(glm::transpose(glm::inverse(model)));
            this->thingOne.bonus = 10;
            break;
        }
    }

    while (true) {
        int i = rand() % (row * 2 + 1);
        int j = rand() % (col * 2 + 1);
        if (!isWall(i, j) && !isStartPoint(i, j) && !isEndPoint(i, j) &&
            (i != this->thingOne.xPos || j != this->thingOne.yPos)) {
            this->thingTwo.xPos = i;
            this->thingTwo.yPos = j;
            thingTwoCollected = false;
            this->thingTwo.position = glm::vec3(i * len, -0.8, j * len);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, this->thingTwo.position);
            model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
            this->thingTwo.model = model;
            this->thingTwo.model_res = glm::mat3(glm::transpose(glm::inverse(model)));
            this->thingTwo.bonus = 20;
            break;
        }
    }

    while (true) {
        int i = rand() % (row * 2 + 1);
        int j = rand() % (col * 2 + 1);
        if (!isWall(i, j) && !isStartPoint(i, j) && !isEndPoint(i, j) &&
            (i != this->thingOne.xPos || j != this->thingOne.yPos) &&
            (i != this->thingTwo.xPos || j != this->thingTwo.yPos)) {
            this->thingThree.xPos = i;
            this->thingThree.yPos = j;
            thingThreeCollected = false;
            this->thingThree.position = glm::vec3(i * len, -0.8, j * len);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, this->thingThree.position);
            model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
            this->thingThree.model = model;
            this->thingThree.model_res = glm::mat3(glm::transpose(glm::inverse(model)));
            this->thingThree.bonus = 50;
            break;
        }
    }
}

void Maze::make_maze() {
    // initialize all to wall
    for (int i = 0; i <= 2 * row + 2; ++i) {
        for (int j = 0; j <= 2 * col + 2; ++j) {
            this->maze_map[i][j] = 1;
        }
    }

    for (int x = 0; x <= 2 * row + 2; ++x) {
        this->maze_map[x][0] = 0;
        this->maze_map[x][2 * col + 2] = 0;
    }

    for (int x = 0; x <= 2 * col + 2; ++x) {
        this->maze_map[0][x] = 0;
        this->maze_map[2 * row + 2][x] = 0;
    }

    this->maze_map[2][1] = 0;
    this->maze_map[2 * row][2 * col + 1] = 0;

    srand((unsigned) time(nullptr));
    searchPath(rand() % row + 1, rand() % col + 1);
}

int Maze::searchPath(int x, int y) {
    static int dir[4][2] = {{0,  1},
                            {1,  0},
                            {0,  -1},
                            {-1, 0}};
    int zx = x * 2;
    int zy = y * 2;
    this->maze_map[zx][zy] = 0;
    int turn = rand() % 2 ? 1 : 3;
    for (int i = 0, next = rand() % 4; i < 4; ++i, next = (next + turn) % 4) {
        if (this->maze_map[zx + 2 * dir[next][0]][zy + 2 * dir[next][1]] == 1) {
            this->maze_map[zx + dir[next][0]][zy + dir[next][1]] = 0;
            searchPath(x + dir[next][0], y + dir[next][1]);
        }
    }
    return 0;
}

int **Maze::get_maze() const {
    int **res = new int *[2 * row + 1];
    for (int i = 0; i < 2 * row + 1; ++i) {
        res[i] = new int[2 * col + 1];
    }

    for (int x = 1; x <= row * 2 + 1; ++x) {
        for (int y = 1; y <= col * 2 + 1; ++y) {
            res[x - 1][y - 1] = this->maze_map[x][y];
        }
    }

    return res;
}

void Maze::print_maze() const {
    for (int x = 1; x <= row * 2 + 1; ++x) {
        for (int y = 1; y <= col * 2 + 1; ++y) {
            std::string value = this->maze_map[x][y] == 0 ? " " : "*";
            std::cout << value;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int Maze::get_col_num() const {
    return 2 * this->col + 1;
}

int Maze::get_row_num() const {
    return 2 * this->row + 1;
}

bool Maze::isWall(int i, int j) const {
    return maze_map[i + 1][j + 1] == 1;
}

bool Maze::isStartPoint(int i, int j) {
    return (i == (int)start.x && j == (int)start.y);
}
glm::vec3 Maze::getStartPoint() {
    return glm::vec3(start.x * len, 0, start.y * len);
}

bool Maze::isEndPoint(int i, int j) {
    return (i == (int)end.x && j == (int)end.y);
}
glm::vec3 Maze::getEndPoint() {
    return glm::vec3(end.x * len, 0, end.y * len);
}

Thing Maze::getThingOne() {
    return this->thingOne;
}

Thing Maze::getThingTwo() {
    return this->thingTwo;
}

Thing Maze::getThingThree() {
    return this->thingThree;
}