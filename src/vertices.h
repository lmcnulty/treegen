#ifndef VERTICES_H
#define VERTICES_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

std::vector<glm::vec3> rotateVertices(std::vector<glm::vec3> &vertices,
                                      float degrees, glm::vec3 axis,
                                      glm::vec3 offset);

glm::vec3 rotateVertex(glm::vec3 vertex, float degrees, glm::vec3 axis, glm::vec3 offset);

/**
 * @param v1 - A pointer to the vector to be added to
 * @param v2 - A vector to be added to the vector referenced by v1
 */
void appendVertices(std::vector<glm::vec3> &v1, std::vector<glm::vec3> &v2);

#endif // VERTICES_H
