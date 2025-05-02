#include <glm/glm.hpp>

#include <iostream>
#include <vector>

#include "../src/spatial_hashtable.hpp"

signed main(int argc, char *argv[]) {
  spatial_hash_table_t spatial_hash_table(1.0f);

  std::vector<glm::vec3> points = {
    {0.0f, 0.0f, 0.0f}, {0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f},
    {2.0f, 2.0f, 2.0f}, {3.0f, 3.0f, 3.0f}, {4.0f, 4.0f, 4.0f},
    {5.0f, 5.0f, 5.0f}, {6.0f, 6.0f, 6.0f}, {7.0f, 7.0f, 7.0f},
    {8.0f, 8.0f, 8.0f}, {9.0f, 9.0f, 9.0f}, {10.f, 10.f, 10.f}
  };

  for(int i = 0; i < points.size(); ++i) {
    spatial_hash_table.insert(i, points[i]);
  }

  for(int i=0; i<(int) points.size(); ++i) {
    auto p = points[i];
    auto neighbors = spatial_hash_table.find_neighbors(p);
    std::cout << "Neighbors of #" << i << " (" << p.x << ", " << p.y << ", " << p.z << "):\n";
    for(auto n: neighbors) {
      auto n_pos = points[n];
      std::cout << n << " " << "(" << n_pos.x << ", " << n_pos.y << ", " << n_pos.z << ")\n";
    }
  }

  return 0;
}
