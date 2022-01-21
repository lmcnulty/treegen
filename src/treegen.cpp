#include "treegen.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "vertices.h"

float zeroToOne(bool normalDist = true) {
    if (!normalDist) {
        return (rand() / static_cast<float>(RAND_MAX));
    }
    return (   (rand() / static_cast<float>(RAND_MAX))
             + (rand() / static_cast<float>(RAND_MAX)) / 2.f);
}

void outputTree(std::string filename) {
    //qDebug() << ">>" << filename.c_str();
    std::vector<glm::vec3> vertices = {};
    fractalTree3d(vertices, glm::vec3(0, 0, 0), glm::vec3(0, 5.f, 0), .2f, 1, 7, 1.5f, 45.f, {}, 6);
    outputVertices(vertices, filename);
}

void outputGrass(std::string filename) {
    std::vector<glm::vec3> vertices = {};
    fractalTree3d(vertices, glm::vec3(0, 0, 0), glm::vec3(0, 5.f, 0), .2f, 1, 7, 1.5f, 45.f, {}, 6);
    for (int i = 0; i < 10000; i++) {
        glm::vec3 pos = glm::vec3((zeroToOne(false) - .5f) * 10.f, 0, (zeroToOne(false) - .5f) * 10.f);
        std::vector<glm::vec3> blade = {};
        grassBlade(blade, pos);
        appendVertices(vertices, blade);
    }

    outputVertices(vertices, filename, "grass");
}

void outputForest(std::string filename) {
    std::vector<glm::vec3> vertices = {};
    float distance = 15.f;
    int rows = 3;
    float emptyRadius = 30.f;
    for (int row = 0; row < rows; row++) {
        int cols = (2 * (emptyRadius + distance * (row + 1)) * M_PI) / distance;
        for (int col = 0; col < cols; col++) {
            //qDebug() << "tree:" << row << col;
            float distOffset = zeroToOne() * distance - distance / 2.f;
            float rotOffset = zeroToOne() - 1;
            glm::vec3 location = rotateVertex(
                glm::vec3(emptyRadius + distance * (row + 1) + distOffset, 0, 0),
                (rotOffset + col) * 360.f / cols,
                glm::vec3(0, 1.f, 0),
                glm::vec3(0, 0, 0)
            );

            fractalTree3d(
                vertices,
                location,
                glm::vec3(0, zeroToOne() * 10.f, 0),
                .25f + .15f * (zeroToOne() - .5f),
                1,
                5,
                1.5f * row,
                45.f + (zeroToOne() - 1.f) * 15.f, {}, 4
            );
        }
    }
    outputVertices(vertices, filename);
}

void outputVertices(std::vector<glm::vec3> vertices, std::string fileName, std::string color) {
    if (fileName.size() == 0) {
        std::cout << "mtllib tree.mtl\nusemtl brown\n";
        for (int i = 0; i < vertices.size(); i++) {
            glm::vec3 vertex = vertices.at(i);
            std::cout << "v " << vertex.x << " " << vertex.y << " " << vertex.z << "\n";
            if ((i + 1) % 3 == 0) {
                std::cout << "f " << (i - 1) << " " << i << " " << (i + 1) << "\n";
            }
        }
    }  else {
        //qDebug() << "Writing to" << fileName.c_str();
        std::ofstream file;
        file.open(fileName);
        file << "mtllib tree.mtl\nusemtl " << color << "\n";
        for (int i = 0; i < vertices.size(); i++) {
            glm::vec3 vertex = vertices.at(i);
            file << "v " << vertex.x << " " << vertex.y << " " << vertex.z << "\n";
            if ((i + 1) % 3 == 0) {
                file << "f " << (i - 1) << " " << i << " " << (i + 1) << "\n";
            }
        }
        file.close();
    }
}

void connect(std::vector<glm::vec3> &vertices,
             std::vector<glm::vec3> &bottomCircle,
             std::vector<glm::vec3> &topCircle) {

    int edges = bottomCircle.size();

    for (int i = 0; i < edges; i++) {
        vertices.push_back(topCircle.at(i));
        vertices.push_back(bottomCircle.at(i));
        vertices.push_back(topCircle.at((i + 1) % (edges)));

        vertices.push_back(topCircle.at( (i + 1) % (edges) ));
        vertices.push_back(bottomCircle.at(i));
        vertices.push_back(bottomCircle.at((i + 1) % (edges) ));
    }
}

void grassBlade(std::vector<glm::vec3> &vertices, glm::vec3 origin) {
    float rad = .005f;
    float height = zeroToOne() * .1f;
    std::vector<glm::vec3> base = {
        origin + glm::vec3(-rad, 0, rad),
        origin + glm::vec3(-rad, 0, -rad),
        origin + glm::vec3(rad, 0, -rad),
        origin + glm::vec3(rad, 0, rad)
    };
    std::vector<glm::vec3> top = {
        origin + glm::vec3(-rad, height, rad),
        origin + glm::vec3(-rad, height, -rad),
        origin + glm::vec3(rad, height, -rad),
        origin + glm::vec3(rad, height, rad)
    };
    glm::vec3 pointPoint = origin + glm::vec3(zeroToOne() * height / 2.f, 2 * height, zeroToOne() * height / 2.f);
    std::vector<glm::vec3> point = { pointPoint, pointPoint, pointPoint, pointPoint };
    connect(vertices, base, top);
    connect(vertices, top, point);
}

void fractalTree3d(std::vector<glm::vec3> &vertices, glm::vec3 origin,
                   glm::vec3 direction, float rad, int n, int max,
                   float distancePerDiv, float baseAngle,
                   std::vector<glm::vec3> base, int maxBranches)
{
    if (n > max) return;

    // Draw the branch
    float distance  = glm::length(direction);
    glm::vec3 a     = origin;
    glm::vec3 b     = origin + direction;
    glm::vec3 ab    = glm::normalize(direction);
    glm::vec3 perp  = glm::cross(ab, glm::vec3(0, 0, 1));
    glm::vec3 perp2 = glm::cross(perp, ab);
    glm::vec3 radX  = rad * perp; //glm::vec3(rad, 0, 0);

    std::vector<std::vector<glm::vec3>> circles;

    float length = glm::length(b - a);
    float lengthCount = 0;
    int divs = std::max(2, static_cast<int>(length / distancePerDiv));
    int edges = 5;

    // Split the branch into fragments to draw edges between
    for (int div = 0; div < divs + 1; div++) {
        if (div == 0 && base.size() == edges) {
            circles.push_back(base);
            continue;
        }
        glm::vec3 center = a + div * (length / divs) * ab;
        lengthCount += div * (length / divs);

        std::vector<glm::vec3> circle = std::vector<glm::vec3>(edges);

        for (int i = 0; i < edges; i++) {
            float distance;
            float degrees;
            distance = rad * (2.f - (div / divs) ) * zeroToOne();

            if (div > divs - 1) {
                if (n == max) {
                    distance = 0;
                } else if (n == 1) {
                    distance = rad;
                }
            }
            degrees = i * (360.f / edges);
            circle.at(i) = (rotateVertex(center + (glm::normalize(radX) * distance), degrees, glm::normalize(a - b), center));
        }
        circles.push_back(circle);
    }
    for (int div = 0; div < divs; div++) {
        connect(vertices, circles.at(div), circles.at(div + 1));
    }

    // Recurse
    int branches = 2 + (rand() % (maxBranches - 2));
    if (maxBranches == 1) branches = 1;
    for (int branch = 0; branch < branches; branch++) {
        glm::vec3 childDirection = glm::normalize(direction);
        childDirection = glm::rotate(childDirection, glm::radians(baseAngle / n), perp);
        childDirection = glm::rotate(childDirection, glm::radians(branch * 360.f / branches), ab);
        float childDistance = (.5f + .75f*zeroToOne() * distance) / glm::sqrt(n);
        fractalTree3d(vertices, b, childDistance * childDirection, rad / n, n + 1, max, distancePerDiv, baseAngle, circles.back());
    }
}

