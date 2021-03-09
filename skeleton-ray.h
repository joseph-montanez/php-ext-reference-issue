//
// Created by Joseph Montanez on 2018-12-02.
//

#ifndef SKELETON_RAY_H
#define SKELETON_RAY_H

extern zend_class_entry *skeleton_ray_ce;

typedef struct _skeleton_ray_object {
    Ray ray;
    HashTable *prop_handler;
    skeleton_vector3_object *position;
    skeleton_vector3_object *direction;
    zend_object std;
} skeleton_ray_object;

static inline skeleton_ray_object *skeleton_ray_fetch_object(zend_object *obj) {
    return (skeleton_ray_object *)((char *)obj - XtOffsetOf(skeleton_ray_object, std));
}

#define Z_RAY_OBJ_P(zv) skeleton_ray_fetch_object(Z_OBJ_P(zv));

void skeleton_ray_startup(INIT_FUNC_ARGS);

#endif //SKELETON_RAY_H
