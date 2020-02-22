// Luc Vermeer 11041633 assignment 3
// made with sudo g++ -Wall -Wextra -o grid grid.cpp
// Execute with ./grid
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
using namespace std;


struct grid {
  size_t height;
  size_t width;
  int array[200][200];
  size_t x_pos;
  size_t y_pos;
  size_t x_span;
  size_t y_span;
  char live_cell;
  char dead_cell;
  float live_probability;
};

struct grid *grid_init () {
  struct grid *g = (struct grid *)malloc(sizeof(struct grid));

  g->x_pos = 0;
  g->y_pos = 0;

  g->x_span = 50;
  g->y_span = 50;

  // g->live_cell = malloc(sizeof(char));
  // g->dead_cell = malloc(sizeof(char));

  g->live_cell = '#';
  g->dead_cell = '.';

  g->live_probability = 0.5;

  g->height = sizeof(g->array) / sizeof(g->array[0]);
  g->width = sizeof(g->array[0]) / sizeof(int);

  for (size_t i = 0; i < g->height; i++) {
    for (size_t j = 0; j < g->width; j++) {
      g->array[i][j] = 0;
    }
  }
  return g;
}

void print_grid(struct grid *g) {
    if (g->height < (g->y_pos + g->y_span)) {
      cout << "Viewport out of bounds\n";
      return;
    }
    if (g->width < (g->x_pos + g->x_span)) {
      cout << "Viewport out of bounds\n";
      return;
    }

    for (size_t x = g->x_pos; x < (g->x_pos + g->x_span); x++) {
      cout << "__";
    }
    cout << "\n";
    for (size_t y = g->y_pos; y < (g->y_pos + g->y_span); y++) {
      for (size_t x = g->x_pos; x < (g->x_pos + g->x_span); x++) {
        int value = g->array[y][x];
        if (!value) {
          cout << g->dead_cell;
        } else {
          cout << g->live_cell;
        }
        cout << " ";
      }
      cout << "\n";
    }
  }

int n_neighbours(struct grid *g, const size_t x, const size_t y) {
    int n = 0;
    if (g->width > x) {
      n += g->array[x+1][y];
      if (g->height > y) {
        n += g->array[x+1][y+1];
      }
      if (y > 0) {
        n += g->array[x+1][y-1];
      }
    }
    if (x > 0) {
      n += g->array[x-1][y];
      if (g->height > y) {
        n += g->array[x-1][y+1];
      }
      if (y > 0) {
        n += g->array[x-1][y-1];
      }
    }
    if (g->height > y) {
      n += g->array[x][y+1];
    }
    if (y > 0) {
      n += g->array[x][y-1];
    }
    return n;
}

int move(struct grid *g) {
// Any live cell with fewer than two live neighbours dies, as if by underpopulation.
// Any live cell with two or three live neighbours lives on to the next generation.
// Any live cell with more than three live neighbours dies, as if by overpopulation.
// Any dead cell with exactly three live neighbours becomes a live cell,
// as if by reproduction.
  int new_array[200][200];
  // memcpy(new_array, g->array, sizeof(g->array));
  int sum = 0;
  for (size_t y = 0; y < g->height; y++) {
    for (size_t x = 0; x < g->width; x++) {
      int value = g->array[y][x];
      sum += value;
      int neighbours = n_neighbours(g, y, x);
      if (neighbours < 2) {
        new_array[y][x] = 0;
      } else if (neighbours > 1 && neighbours < 4 && value == 1) {
        new_array[y][x] = 1;
      } else if (neighbours > 3) {
        new_array[y][x] = 0;
      } else if (neighbours == 3 && value == 0){
        new_array[y][x] = 1;
      }
    }
  }
  memcpy(g->array, new_array, sizeof(new_array));
  return sum;
}

struct grid *go(struct grid *g) {
  // Takes 100 steps or until population is dead.
  for (int i = 0; i < 10000; i++) {
    print_grid(g);
    if(!move(g)) {
      break;
    }
    usleep(100000);
  }
  return g;
}

struct grid *clean (struct grid *g) {
  for (size_t i = 0; i < g->height; i++) {
    for (size_t j = 0; j < g->width; j++) {
      g->array[i][j] = 0;
    }
  }
  return g;
}

int PRNG() {
  // Source:
  // https://stackoverflow.com/questions/15500621/c-c-algorithm-to-produce-same
  // -pseudo-random-number-sequences-from-same-seed-on
    // our initial starting seed is 5323
    unsigned long nSeed = 12345;
    // Take the current seed and generate a new value from it
    // Due to our use of large constants and overflow, it would be
    // very hard for someone to predict what the next number is
    // going to be from the previous one.
    nSeed = (8253729 * nSeed + 2396403);

    // Take the seed and return a value between 0 and 32767
    return nSeed % 2;
}

struct grid *randomize (struct grid *g) {
  for (size_t i = 0; i < g->height; i++) {
    for (size_t j = 0; j < g->width; j++) {
      g->array[i][j] = rand() % 2;
    }
  }
  return g;
}

void insert_file(struct grid *g, const string file) {
  ifstream fin;
  fin.open(file.c_str());
  char current;

  int x = g->x_pos;
  int y = g->y_pos;

  while(fin.get(current)) {
    if (current == '\n') {
      x = g->x_pos;
      y++;
    } else {
      if (current == '.' || current == ' '){
        g->array[y][x] = 0;
      } else {
        g->array[y][x] = 1;
      }
      x++;
    }
  }
  fin.close();
}

int main() {
  struct grid *g = grid_init();
  print_grid(g);
  while(1) {
    string user_input;
    char dead_cell_input;
    char live_cell_input;
    cout << "MENU: stop, clean, randomize, one, go, parameters, file \n";
    cin >> user_input;
    if (user_input == "stop") {
      exit(0);
    }
    if (user_input == "clean") {
       g = clean(g);
       print_grid(g);
     }
    if (user_input == "randomize") {
       g = randomize(g);
       print_grid(g);
    }
    if (user_input == "one") {
      move(g);
      print_grid(g);
    }
    if (user_input == "go") {
      g = go(g);
    }
    if (user_input == "parameters") {
      cout << "Horizontal viewport size (0-200)\n";
      cin >> g->x_span;
      cout << "Vertical viewport size (0-200)\n";
      cin >> g->y_span;
      cout << "X position (0-200)\n";
      cin >> g->x_pos;
      cout << "Y position (0-200)\n";
      cin >> g->y_pos;
      cout << "Character of live cell\n";
      cin >> live_cell_input;
      cout << "Character of dead cell\n";
      cin >> dead_cell_input;
      cout << "Probability of a cell being alive when randomizing grid (0.0-1.0)";
      cin >> g->live_probability;
      // strcpy(g->live_cell, live_cell_input);
      // strcpy(g->dead_cell, dead_cell_input);
      g->live_cell = live_cell_input;
      g->dead_cell = dead_cell_input;
      print_grid(g);
    }
    if (user_input == "file") {
      string file;
      cout << "Enter a file name\n";
      cin >> file;
      insert_file(g, file);
      print_grid(g);
    }
    cin.clear();
    cin.ignore(10000,'\n');
  }
  return 0;
}
