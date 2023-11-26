#include <SFML/Graphics.hpp>
#include <array>
#include <chrono>
#include <climits>
#include <map>
#include <float.h>
#include <cmath>


#include "Headers/GetAdjacentCells.hpp"
#include "Headers/Global.hpp"


// Guess what? I made a whole video explaining how the A star algorithm works
// along with Dijkstra and Breadth-first search. Here it is:
// https://youtu.be/W1qvb86YOK0

float calculate_h_score(const sf::Vector2<unsigned short> &i_cell_0,
                        const sf::Vector2<unsigned short> &i_cell_1) {
  float distance_x = abs(i_cell_1.x - i_cell_0.x);
  float distance_y = abs(i_cell_1.y - i_cell_0.y);

  // This is the minimal distance it takes to move from cell_0 to cell_1 if we
  // move in 8 directions and ignore every obstacle. I don't recommend using
  // other types of distance calculations because then Astar doesn't find the
  // shortest path.
  return std::max(distance_x, distance_y) +
         std::min(distance_x, distance_y) * (sqrt(2) - 1);
}

// SO MANY ARGUMENTS!!!!
void astar_reset(unsigned short &i_path_length,
                 std::map<gbl::Position<>, gbl::Position<>> &i_previous_cell,
                 std::vector<sf::Vector2<unsigned short>> &i_path_vector,
                 gbl::MAP::Map<float> &i_f_scores,
                 gbl::MAP::Map<float> &i_g_scores,
                 gbl::MAP::Map<float> &i_h_scores,
                 const sf::Vector2<unsigned short> &i_finish_position,
                 const sf::Vector2<unsigned short> &i_start_position,
                 gbl::MAP::Map<gbl::MAP::PathCell> &i_map) {
  i_path_length = 0;

  i_previous_cell.clear();

  i_path_vector.clear();
  i_path_vector.push_back(i_start_position);

  for (unsigned short a = 0; a < gbl::MAP::COLUMNS; a++) {
    for (unsigned short b = 0; b < gbl::MAP::ROWS; b++) {
      // Calculating the h score beforehand because we can.
      i_h_scores[a][b] = calculate_h_score(sf::Vector2<unsigned short>(a, b),
                                           i_finish_position);

      // We're not changing the map during the game so we can reset the map
      // here.
      if (gbl::MAP::PathCell::Wall != i_map[a][b]) {
        i_map[a][b] = gbl::MAP::PathCell::Empty;
      }

      if (a == i_start_position.x && b == i_start_position.y) {
        i_f_scores[a][b] = i_h_scores[a][b];
        i_g_scores[a][b] = 0;

      } else {
        i_f_scores[a][b] = FLT_MAX;
        i_g_scores[a][b] = FLT_MAX;
      }
    }
  }
}

// THIS ONE HAS EVEN MORE ARGUMENTS!!!!!!!!
void astar_search(unsigned short &i_path_length,
                  std::map<gbl::Position<>, gbl::Position<>> &i_previous_cell,
                  std::vector<sf::Vector2<unsigned short>> &i_path_vector,
                  gbl::MAP::Map<float> &i_f_scores,
                  gbl::MAP::Map<float> &i_g_scores,
                  const gbl::MAP::Map<float> &i_h_scores,
                  sf::Vector2<unsigned short> &i_next_cell,
                  const sf::Vector2<unsigned short> &i_finish_position,
                  const sf::Vector2<unsigned short> &i_start_position,
                  gbl::MAP::Map<gbl::MAP::PathCell> &i_map) {
  while (1) {
    if (1 == i_path_vector.empty()) {
      return;
    }

    std::vector<sf::Vector2<unsigned short>>::iterator min_f_cell_iterator =
        i_path_vector.begin();

    // Here we're finding the cell with the lowest f score.
    sf::Vector2<unsigned short> min_f_cell;

    for (std::vector<sf::Vector2<unsigned short>>::iterator a =
             1 + i_path_vector.begin();
         a != i_path_vector.end(); a++) {
      if (i_f_scores[a->x][a->y] <
          i_f_scores[min_f_cell_iterator->x][min_f_cell_iterator->y]) {
        min_f_cell_iterator = a;
      }
    }

    min_f_cell = *min_f_cell_iterator;

    if (FLT_MAX == i_f_scores[min_f_cell.x][min_f_cell.y]) {
      return;
    }

    i_path_vector.erase(min_f_cell_iterator);

    i_map[min_f_cell.x][min_f_cell.y] = gbl::MAP::PathCell::Visited;

    if (min_f_cell == i_finish_position) {
      gbl::Position<> path_cell(min_f_cell.x, min_f_cell.y);

      do {
        i_path_length++;

        // Steven only needs the position of the next cell he should go to.
        i_next_cell.x = path_cell.first;
        i_next_cell.y = path_cell.second;

        path_cell = i_previous_cell.at(path_cell);
      } while (i_start_position.x != path_cell.first ||
               i_start_position.y != path_cell.second);

      return;
    }

    for (const sf::Vector2<unsigned short> &adjacent_cell :
         get_adjacent_cells(min_f_cell, i_map)) {
      if (gbl::MAP::PathCell::Visited !=
          i_map[adjacent_cell.x][adjacent_cell.y]) {
        float g_score = i_g_scores[min_f_cell.x][min_f_cell.y];

        if (abs(adjacent_cell.x - min_f_cell.x) ==
            abs(adjacent_cell.y - min_f_cell.y)) {
          // If the adjacent cell is located diagonally, we add the square root
          // of 2.
          g_score += sqrt(2);
        } else {
          g_score++;
        }

        if (g_score < i_g_scores[adjacent_cell.x][adjacent_cell.y]) {
          i_previous_cell[gbl::Position<>(adjacent_cell.x, adjacent_cell.y)] =
              gbl::Position<>(min_f_cell.x, min_f_cell.y);

          i_f_scores[adjacent_cell.x][adjacent_cell.y] =
              g_score + i_h_scores[adjacent_cell.x][adjacent_cell.y];
          i_g_scores[adjacent_cell.x][adjacent_cell.y] = g_score;

          if (i_path_vector.end() == std::find(i_path_vector.begin(),
                                               i_path_vector.end(),
                                               adjacent_cell)) {
            i_path_vector.push_back(adjacent_cell);
          }
        }
      }
    }
  }
}
