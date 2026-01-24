#include "mesh.h"

vec3_t mesh_vertices[N_MESH_VERTICES] = {
	{ -1, -1, -1 },
	{  1, -1, -1 },
	{  1,  1, -1 },
	{ -1,  1, -1 },
	{ -1, -1,  1 },
	{  1, -1,  1 },
	{  1,  1,  1 },
	{ -1,  1,  1 }
};


face_t mesh_faces[N_MESH_FACES] = {
	{ 0, 1, 2 }, { 0, 2, 3 }, // Back face
	{ 4, 5, 6 }, { 4, 6, 7 }, // Front face
	{ 0, 1, 5 }, { 0, 5, 4 }, // Bottom face
	{ 2, 3, 7 }, { 2, 7, 6 }, // Top face
	{ 1, 2, 6 }, { 1, 6, 5 }, // Right face
	{ 0, 3, 7 }, { 0, 7, 4 }  // Left face
};