#ifndef TREEGEN_H
#define TREEGEN_H

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <glm/glm.hpp>

void grassBlade(std::vector<glm::vec3> &vertices, glm::vec3 origin);
void outputTree(std::string filename = "");
void outputForest(std::string filename = "");
void outputVertices(std::vector<glm::vec3> vertices, std::string fileName = "", std::string color = "brown");
void outputGrass(std::string filename) ;
void fractalTree3d(std::vector<glm::vec3> &vertices, glm::vec3 origin, glm::vec3 direction,
                   float rad, int n, int max, float distancePerDiv, float baseAngle = 45.f,
                   std::vector<glm::vec3> base = {}, // Can't use a reference, because we need an empty default
                   int maxBranches = 4);

#endif // TREEGEN_H
