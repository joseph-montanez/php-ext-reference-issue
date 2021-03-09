//
// Created by joseph on 3/8/21.
//

#ifndef SKELETON_SKELETON_H
#define SKELETON_SKELETON_H

// Vector3 type
typedef struct Vector3 {
    float x;
    float y;
    float z;
} Vector3;

// Ray type (useful for raycast)
typedef struct Ray {
    Vector3 position;       // Ray position (origin)
    Vector3 direction;      // Ray direction
} Ray;

#endif //SKELETON_SKELETON_H
