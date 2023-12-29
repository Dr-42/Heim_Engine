#pragma once

#include <assimp/cimport.h>
#include <assimp/quaternion.h>
#include <assimp/vector3.h>
#include <assimp/matrix4x4.h>

#include "math/heim_mat.h"
#include "math/heim_vec.h"

HeimMat4 convert_matrix_to_glm(const struct aiMatrix4x4* matrix);
HeimVec3f get_glm_vec(const struct aiVector3D* vec);
HeimVec4f get_glm_quat(const struct aiQuaternion* quat);
