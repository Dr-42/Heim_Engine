#include "math/heim_vector.h"

HeimVec2f heim_vec2f_add(HeimVec2f a, HeimVec2f b){
    return (HeimVec2f){a.x + b.x, a.y + b.y};
}
