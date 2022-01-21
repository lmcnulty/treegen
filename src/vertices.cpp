#include "vertices.h"

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

std::vector<glm::vec3> rotateVertices(std::vector<glm::vec3> &vertices,
                                      float degrees,
                                      glm::vec3 axis,
                                      glm::vec3 offset = glm::vec3(0, 0, 0)) {
    std::vector<glm::vec3> result = std::vector<glm::vec3>(vertices.size());
    for (int i = 0; i < vertices.size(); i++) {
        result.push_back(
              glm::rotate(vertices.at(i) - offset, glm::radians(degrees), axis)
            + offset
        );
    }
    return result;
}

glm::vec3 rotateVertex(glm::vec3 vertex, float degrees, glm::vec3 axis, glm::vec3 offset) {
    return glm::rotate(vertex - offset, glm::radians(degrees), axis) + offset;
}

/**
 * @param v1 - A pointer to the vector to be added to
 * @param v2 - A vector to be added to the vector referenced by v1
 */
void appendVertices(std::vector<glm::vec3> &v1, std::vector<glm::vec3> &v2) {
    v1.insert(v1.end(), v2.begin(), v2.end());
}
