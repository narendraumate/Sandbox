//
//  MeshGen.cpp
//
//
//  Created by Narendra Umate on 8/21/16.
//
//

#include "MeshGen.h"

#include "par_shapes.h"

void shapeToMesh(par_shapes_mesh* shape, Mesh* mesh) {
	if (shape->npoints > 0) {
		bool fillNormals = (shape->normals != nullptr);
		bool fillTexcoords = (shape->tcoords != nullptr);
		mesh->vertices.resize(shape->npoints);
		for (unsigned int i = 0; i < shape->npoints; ++i) {
			mesh->vertices[i].position.x = shape->points[3 * i];
			mesh->vertices[i].position.y = shape->points[3 * i + 1];
			mesh->vertices[i].position.z = shape->points[3 * i + 2];
			if (fillNormals) {
				mesh->vertices[i].normal.x = shape->normals[3 * i];
				mesh->vertices[i].normal.y = shape->normals[3 * i + 1];
				mesh->vertices[i].normal.z = shape->normals[3 * i + 2];
			}
			if (fillTexcoords) {
				mesh->vertices[i].texcoord.x = shape->tcoords[2 * i];
				mesh->vertices[i].texcoord.y = shape->tcoords[2 * i + 1];
			} else {
				// Randomize Texture Coordinates for Tangents and Bitangnents
				mesh->vertices[i].texcoord.x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
				mesh->vertices[i].texcoord.y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			}
		}
	}

	if (shape->ntriangles > 0) {
		unsigned int nindices = shape->ntriangles * 3;
		mesh->primitives[0].triangles.indices.resize(nindices);
		for (unsigned int i = 0; i < nindices; ++i) {
			mesh->primitives[0].triangles.indices[i] = shape->triangles[i];
		}
	}
}

void createIcosahedron(Mesh* mesh) {
	par_shapes_mesh* shape = par_shapes_create_icosahedron();
	par_shapes_unweld(shape, true);
	par_shapes_compute_normals(shape);
	shapeToMesh(shape, mesh);
	par_shapes_free_mesh(shape);
}

void createDodecahedron(Mesh* mesh) {
	par_shapes_mesh* shape = par_shapes_create_dodecahedron();
	par_shapes_unweld(shape, true);
	par_shapes_compute_normals(shape);
	shapeToMesh(shape, mesh);
	par_shapes_free_mesh(shape);
}

void createOctahedron(Mesh* mesh) {
	par_shapes_mesh* shape = par_shapes_create_octahedron();
	par_shapes_unweld(shape, true);
	par_shapes_compute_normals(shape);
	shapeToMesh(shape, mesh);
	par_shapes_free_mesh(shape);
}

void createTetrahedron(Mesh* mesh) {
	par_shapes_mesh* shape = par_shapes_create_tetrahedron();
	par_shapes_unweld(shape, true);
	par_shapes_compute_normals(shape);
	shapeToMesh(shape, mesh);
	par_shapes_free_mesh(shape);
}

void createCube(Mesh* mesh) {
	par_shapes_mesh* shape = par_shapes_create_cube();
	par_shapes_unweld(shape, true);
	par_shapes_compute_normals(shape);
	shapeToMesh(shape, mesh);
	par_shapes_free_mesh(shape);
}
