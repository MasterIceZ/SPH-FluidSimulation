#ifndef __SPATIAL_HASHTABLE_HPP__
#define __SPATIAL_HASHTABLE_HPP__

#include <glm/glm.hpp>

#include <unordered_map>
#include <vector>

class spatial_hash_table_t {
private:
  float cell_size;
  std::unordered_map<size_t, std::vector<int>> grid;
 public:
  spatial_hash_table_t(float cell_size): 
    cell_size(cell_size) {}
  
  size_t hasher(const glm::vec3 &p) const {
    constexpr size_t HP_1 = 73856093;
    constexpr size_t HP_2 = 19349663;
    constexpr size_t HP_3 = 83492791;
    return (
      static_cast<size_t> (p.x) * HP_1 ^
      static_cast<size_t> (p.y) * HP_2 ^
      static_cast<size_t> (p.z) * HP_3
    );
  }

  glm::ivec3 get_coordinates(const glm::vec3 &pos) const {
    return glm::ivec3(
      static_cast<int> (pos.x / cell_size),
      static_cast<int> (pos.y / cell_size),
      static_cast<int> (pos.z / cell_size)
    );
  }

  void insert(int particle_id, const glm::vec3 &p) {
    glm::ivec3 cell = get_coordinates(p);
    size_t key = hasher(cell);
    grid[key].push_back(particle_id);
  }

  std::vector<int> find_neighbors(const glm::vec3 &p) const {
    glm::ivec3 cell = get_coordinates(p);
    std::vector<int> neighbors;

    for(int dz=-1; dz<=1; ++dz) {
      for(int dy=-1; dy<=1; ++dy) {
        for(int dx=-1; dx<=1; ++dx) {
          glm::ivec3 neighbor_cell = cell + glm::ivec3(dx, dy, dz);
          size_t neightbor_key = hasher(neighbor_cell);
          auto it = grid.find(neightbor_key);
          if(it != grid.end()) {
            neighbors.insert(
              neighbors.end(), 
              it->second.begin(),        
              it->second.end()
            );
          }
        }
      }
    }
    return neighbors;
  }

  void clear() {
    grid.clear();
  }

  void reserve(size_t capacity) {
    grid.reserve(capacity);
  }
};

#endif