//
//  MeshPly.h
//
//
//  Created by Narendra Umate on 8/6/16.
//
//

#pragma once

#include "Mesh.h"

struct Mesh;
struct Material;
struct Texture;

bool readPly(const std::string& filepath, std::vector<Mesh>& meshes, std::vector<Material>& materials);
