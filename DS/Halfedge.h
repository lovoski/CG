#ifndef HALFEDGE_H
#define HALFEDGE_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace CG {

struct Face;
struct Vertex;
struct Halfedge;

struct Vertex {
  int id;
  double x, y, z;
  int edge;  // one half edge take current vertex as origin
  bool border = false;
};

struct Halfedge {
  int id;
  int face;
  int origin, target;
  int twin, next, prev;
};

struct TriFace {
  int id;
  int v[3];  // indices for the triangle vertices
};

// Halfedge mesh data structure
class HalfedgeMesh {
 public:
  HalfedgeMesh() {}
  HalfedgeMesh(std::string filename) { LoadFromOff(filename); }
  ~HalfedgeMesh() {}
  // Initialize from .off triangle model file
  void LoadFromOff(std::string filename) {
    // Read the OFF file
    std::string line, tmp;
    std::ifstream offfile(filename);
    double a1, a2, a3;
    if (offfile.is_open()) {
      // Check first line is a OFF file
      while (std::getline(offfile, line)) {  // add check boundary vertices flag
        std::istringstream(line) >> tmp;
        if (tmp[0] != '#' && !isWhitespace(line)) {
          if (tmp[0] == 'O' && tmp[1] == 'F' &&
              tmp[2] == 'F')  // Check if the format is OFF
            break;
          else {
            std::cout << "The file is not an OFF file" << std::endl;
            exit(0);
          }
        }
      }
      // Read the number of vertices and faces
      while (std::getline(offfile, line)) {  // add check boundary vertices flag
        std::istringstream(line) >> tmp;
        if (tmp[0] != '#' && !isWhitespace(line)) {
          std::istringstream(line) >> this->numVertices >> this->numFaces;
          // Create space for internal storage
          vertices.resize(this->numVertices);
          faces.resize(this->numFaces);
          numHalfedges = 3 * this->numFaces;
          halfedges.resize(3 * this->numFaces);
          break;
        }
      }
      // Read vertices
      int index = 0;
      while (index < numVertices && std::getline(offfile, line)) {
        std::istringstream(line) >> tmp;
        if (tmp[0] != '#' && !isWhitespace(line)) {
          std::istringstream(line) >> a1 >> a2 >> a3;
          Vertex v;
          v.id = index;
          v.x = a1;
          v.y = a2;
          v.z = a3;
          vertices[index] = v;
          index++;
        }
      }
      // Read faces
      int verticesPerFace, t1, t2, t3;
      index = 0;
      while (index < numFaces && std::getline(offfile, line)) {
        std::istringstream(line) >> tmp;
        if (tmp[0] != '#' && !isWhitespace(line)) {
          std::istringstream(line) >> verticesPerFace >> t1 >> t2 >> t3;
          TriFace face;
          face.id = index;
          face.v[0] = t1;
          face.v[1] = t2;
          face.v[2] = t3;
          faces[index] = face;
          index++;
        }
      }
    } else
      std::cout << "Unable to open node file";
    offfile.close();

    ConstructHalfedges();
  }

  // void SaveToOff(std::string filename) {}

  const int GetNumHalfedges() { return numHalfedges; }
  const int GetNumVertices() { return numVertices; }
  const int GetNumFaces() { return numFaces; }

  // Query

  const int Next(int e) { return halfedges[e].next; }
  const int Prev(int e) { return halfedges[e].prev; }
  const int Twin(int e) { return halfedges[e].twin; }

  const int Face(int e) { return halfedges[e].face; }

  const int Origin(int e) { return halfedges[e].origin; }
  const int Target(int e) { return halfedges[e].target; }

  const int EdgeOfVertex(int v) { return vertices[v].edge; }
  const std::vector<int> NeighborsOfVertex(int v) {
    std::vector<int> ret;
    int eid = vertices[v].edge;
    int cur = eid;
    do {
      ret.push_back(halfedges[cur].target);
      cur = halfedges[cur].prev;
      cur = halfedges[cur].twin;
    } while (cur != eid);
    return ret;
  }

  // The border vertices can be recognized by the border property
  const bool IsBorderVertex(int v) { return vertices[v].border; }
  // The halfedges is border if the faceid is -1
  const bool IsBorderHalfedge(int e) { return halfedges[e].face == -1; }

  // Return the face struct
  const TriFace FaceId(int fid) { return faces[fid]; }
  // Return the vertex struct
  const Vertex VertexId(int vid) { return vertices[vid]; }
  // Return the halfedge struct
  const Halfedge HalfedgeId(int eid) { return halfedges[eid]; }

  // // TODO : Modification

  // // By removing the vertex, the halfedges and faces connected
  // // will wlso be removed
  // const bool RemoveVertex(int v) {return false;}
  // // By removing the face, only the three halfedges are removed
  // const bool RemoveFace(int f) {return false;}
  // // By removing the edge, the two faces adjacent to it will be removed
  // const bool RemoveEdge(int e) {return false;}

  // // Push one element to the vertices array, no connection will be made
  // // The index for created vertex will be returned
  // const int AddVertex(double x, double y, double z) {return -1;}
  // // Create face connection between three given vertices
  // // The index for created face will be returned
  // const int AddFace(int v1, int v2, int v3) {return -1;}
  // // Create only edge connection between two vertices
  // // The index for one of the created halfedge will be returned
  // const int AddEdge(int v1, int v2) {return -1;}

 private:
  int numHalfedges, numVertices, numFaces;
  int numBorderVertices;
  std::vector<Vertex> vertices;
  std::vector<TriFace> faces;
  std::vector<Halfedge> halfedges;

  // Returns false if the string contains any non-whitespace characters
  // Returns false if the string contains any non-ASCII characters
  bool isWhitespace(std::string s) {
    for (int index = 0; index < s.length(); index++)
      if (!std::isspace(s[index])) return false;
    return true;
  }

  // Convert each triangle face into three halfedges
  // Traversal the edges and create exteroir edges
  void ConstructHalfedges() {
    std::vector<std::vector<std::pair<int, int>>> map(numVertices);
    for (int i = 0; i < numFaces; ++i) {
      for (int j = 0; j < 3; ++j) {
        Halfedge he;
        he.id = i * 3 + j;
        int vOrigin = faces[i].v[j];
        int vTarget = faces[i].v[(j + 1) % 3];
        int minVertId = std::min(vOrigin, vTarget);
        int maxVertId = std::max(vOrigin, vTarget);
        he.twin = -1;
        if (map[minVertId].size() == 0) {
          // No twin has been stored
          map[minVertId].push_back(std::make_pair(maxVertId, i * 3 + j));
        } else {
          // Query the map for its twin with the same maxVertId
          for (auto stores : map[minVertId]) {
            if (stores.first == maxVertId) {
              // Twin found
              he.twin = stores.second;
              halfedges[stores.second].twin = i * 3 + j;
            }
          }
          if (he.twin == -1) {
            // Still found no twin
            map[minVertId].push_back(std::make_pair(maxVertId, i * 3 + j));
          }
        }
        he.origin = vOrigin;
        he.target = vTarget;
        he.prev = i * 3 + (j + 2) % 3;
        he.next = i * 3 + (j + 1) % 3;
        he.face = i;
        halfedges[i * 3 + j] = he;
        vertices[vOrigin].edge = i * 3 + j;
      }
    }
    // Locate vertices on border
    numBorderVertices = 0;
    std::vector<int> borderEdgeIds;
    for (int i = 0; i < numHalfedges; ++i) {
      if (halfedges[i].twin == -1) {
        numBorderVertices++;
        borderEdgeIds.push_back(i);
        vertices[halfedges[i].origin].border = true;
        vertices[halfedges[i].target].border = true;
      }
    }
    if (numBorderVertices > 0) {
      // Extend halfedge array to exterior edges
      halfedges.reserve(numHalfedges + numBorderVertices);
      for (int i = numHalfedges; i < numHalfedges + numBorderVertices; ++i) {
        Halfedge he;
        he.id = i;
        he.face = -1;
        he.twin = borderEdgeIds[i - numHalfedges];
        halfedges[he.twin].twin = i;
        he.origin = halfedges[he.twin].target;
        he.target = halfedges[he.twin].origin;
        halfedges.push_back(he);
      }
      // Build exterior edges prev and next
      for (int i = numHalfedges; i < numHalfedges + numBorderVertices; ++i) {
        int cur = halfedges[i].twin;
        while (halfedges[cur].face != -1)
          cur = halfedges[halfedges[cur].next].twin;
        halfedges[i].prev = cur;
        cur = halfedges[i].twin;
        while (halfedges[cur].face != -1)
          cur = halfedges[halfedges[cur].prev].twin;
        halfedges[i].next = cur;
      }
      numHalfedges = numHalfedges + numBorderVertices;
    }
  }
};

};  // namespace CG

#endif
