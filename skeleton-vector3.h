//
// Created by Joseph Montanez on 2018-12-02.
//

#ifndef SKELETON_VECTOR3_H
#define SKELETON_VECTOR3_H

extern zend_class_entry *skeleton_vector3_ce;

typedef struct _skeleton_vector3_object {
    Vector3 vector3;
    HashTable *prop_handler;
    zend_object std;
} skeleton_vector3_object;

static inline skeleton_vector3_object *skeleton_vector3_fetch_object(zend_object *obj) {
    return (skeleton_vector3_object *)((char *)obj - XtOffsetOf(skeleton_vector3_object, std));
}

#define Z_VECTOR3_OBJ_P(zv) skeleton_vector3_fetch_object(Z_OBJ_P(zv));

void skeleton_vector3_startup(INIT_FUNC_ARGS);

#endif //SKELETON_VECTOR3_H
