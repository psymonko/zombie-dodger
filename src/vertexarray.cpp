#include "vertexarray.h"
#include <GL/glew.h>

VertexArray::VertexArray(const float* verts, unsigned int numVerts,
			 const unsigned int* indices, unsigned int numIndices)
  :mNumVerts(numVerts)
  ,mNumIndices(numIndices)
{
  // create a vertex array to be rendered
  // can hold multiple vbos, and also usually an index buffer
  glGenVertexArrays(1, &mVertexArray);
  glBindVertexArray(mVertexArray);

  // create vertex buffer that holds the raw vertex data (position, colors,
  // normals, tex coordinates, etc)
  glGenBuffers(1, &mVertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
  // each vertex has 5 attributes (3 position and 2 tex coord) allocate memory
  glBufferData(GL_ARRAY_BUFFER, numVerts * 5 * sizeof(float), verts, GL_STATIC_DRAW);

  // create index buffer to specify what vertices the triangles use
  // so you can reuse vertices and save memory
  glGenBuffers(1, &mIndexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

  // now specify the vertex attributes in the VBO
  // (how are the vertices stored and what info they contain)
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5,
			reinterpret_cast<void*>(sizeof(float) * 3));
}

VertexArray::~VertexArray() {
  glDeleteBuffers(1, &mVertexBuffer);
  glDeleteBuffers(1, &mIndexBuffer);
  glDeleteVertexArrays(1, &mVertexArray);
}

void VertexArray::SetActive() {
  glBindVertexArray(mVertexArray);
}
