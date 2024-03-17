#include "Halfedge.h"
#include "ds_config.h"

void PrintEdgeInfo(CG::HalfedgeMesh &mesh, int eid) {
  int prev = mesh.Prev(eid);
  int next = mesh.Next(eid);

  int origin = mesh.Origin(eid);
  int target = mesh.Target(eid);

  int twin = mesh.Twin(eid);

  int face = mesh.Face(eid);

  printf("eid=%d : prev=%d, next=%d, twin=%d, origin=%d, target=%d, face=%d\n",
         eid, prev, next, twin, origin, target, face);
}

void PrintFaceInfo(CG::HalfedgeMesh &mesh, int fid) {
  int v1 = mesh.FaceId(fid).v[0];
  int v2 = mesh.FaceId(fid).v[1];
  int v3 = mesh.FaceId(fid).v[2];

  printf("fid=%d : v1=%d, v2=%d, v3=%d\n", fid, v1, v2, v3);
}

void PrintVertexInfo(CG::HalfedgeMesh &mesh, int vid) {
  double x = mesh.VertexId(vid).x;
  double y = mesh.VertexId(vid).y;
  double z = mesh.VertexId(vid).z;

  bool border = mesh.VertexId(vid).border;

  int edge = mesh.VertexId(vid).edge;

  printf("vid=%d : x=%lf, y=%lf, z=%lf, border=%d, edge=%d\n", vid, x, y, z,
         border, edge);
}

int main() {
  CG::HalfedgeMesh mesh(RESOURCE_DIR "/cloth.off");

  int vn = mesh.GetNumVertices();
  int vf = mesh.GetNumFaces();
  int ve = mesh.GetNumHalfedges();

  PrintVertexInfo(mesh, 0);

  int e1 = mesh.EdgeOfVertex(0);
  PrintEdgeInfo(mesh, e1);

  PrintEdgeInfo(mesh, mesh.Next(e1));

  PrintEdgeInfo(mesh, mesh.Next(mesh.Next(e1)));

  PrintVertexInfo(mesh, 1525);

  PrintEdgeInfo(mesh, mesh.EdgeOfVertex(1525));

  auto neigh = mesh.NeighborsOfVertex(0);

  CG::HalfedgeMesh mesh1(RESOURCE_DIR "/sphere.off");

  neigh = mesh1.NeighborsOfVertex(0);

  return 0;
}