/* If defined, the following flags inhibit definition of the indicated items.*/
#define NOGDICAPMASKS     // CC_*, LC_*, PC_*, CP_*, TC_*, RC_
#define NOVIRTUALKEYCODES // VK_*
#define NOWINMESSAGES     // WM_*, EM_*, LB_*, CB_*
#define NOWINSTYLES       // WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
#define NOSYSMETRICS      // SM_*
#define NOMENUS           // MF_*
#define NOICONS           // IDI_*
#define NOKEYSTATES       // MK_*
#define NOSYSCOMMANDS     // SC_*
#define NORASTEROPS       // Binary and Tertiary raster ops
#define NOSHOWWINDOW      // SW_*
#define OEMRESOURCE       // OEM Resource values
#define NOATOM            // Atom Manager routines
#define NOCLIPBOARD       // Clipboard routines
#define NOCOLOR           // Screen colors
#define NOCTLMGR          // Control and Dialog routines
#define NODRAWTEXT        // DrawText() and DT_*
#define NOGDI             // All GDI defines and routines
#define NOKERNEL          // All KERNEL defines and routines
#define NOUSER            // All USER defines and routines
/*#define NONLS             // All NLS defines and routines*/
#define NOMB              // MB_* and MessageBox()
#define NOMEMMGR          // GMEM_*, LMEM_*, GHND, LHND, associated routines
#define NOMETAFILE        // typedef METAFILEPICT
#define NOMINMAX          // Macros min(a,b) and max(a,b)
#define NOMSG             // typedef MSG and associated routines
#define NOOPENFILE        // OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#define NOSCROLL          // SB_* and scrolling routines
#define NOSERVICE         // All Service Controller routines, SERVICE_ equates, etc.
#define NOSOUND           // Sound driver routines
#define NOTEXTMETRIC      // typedef TEXTMETRIC and associated routines
#define NOWH              // SetWindowsHook and WH_*
#define NOWINOFFSETS      // GWL_*, GCL_*, associated routines
#define NOCOMM            // COMM driver routines
#define NOKANJI           // Kanji support stuff.
#define NOHELP            // Help engine interface.
#define NOPROFILER        // Profiler interface.
#define NODEFERWINDOWPOS  // DeferWindowPos routines
#define NOMCX             // Modem Configuration Extensions

/* Type required before windows.h inclusion  */
typedef struct tagMSG *LPMSG;

#include "php.h"

#undef LOG_INFO
#undef LOG_WARNING
#undef LOG_DEBUG

#include "skeleton.h"
#include "skeleton-vector3.h"
#include "skeleton-ray.h"
#include "php_skeleton.h"


//------------------------------------------------------------------------------------------------------
//-- raylib Ray PHP Custom Object
//------------------------------------------------------------------------------------------------------
zend_object_handlers skeleton_ray_object_handlers;

static HashTable skeleton_ray_prop_handlers;

typedef skeleton_vector3_object * (*skeleton_ray_read_vector3_object_t)(skeleton_ray_object *obj);

typedef int (*skeleton_ray_write_vector3_object_t)(skeleton_ray_object *obj, zval *value);

typedef struct _skeletonray_prop_handler {
    skeleton_ray_read_vector3_object_t read_vector3_func;
    skeleton_ray_write_vector3_object_t write_vector3_func;
} skeleton_ray_prop_handler;
/* }}} */

static void skeleton_ray_register_prop_handler(HashTable *prop_handler, char *name, skeleton_ray_read_vector3_object_t read_vector3_func, skeleton_ray_write_vector3_object_t write_vector3_func) /* {{{ */
{
    skeleton_ray_prop_handler hnd;

    hnd.read_vector3_func = read_vector3_func;
    hnd.write_vector3_func = write_vector3_func;
    zend_hash_str_add_mem(prop_handler, name, strlen(name), &hnd, sizeof(skeleton_ray_prop_handler));

    /* Register for reflection */
    zend_declare_property_null(skeleton_ray_ce, name, strlen(name), ZEND_ACC_PUBLIC);
}
/* }}} */

static zval *skeleton_ray_property_reader(skeleton_ray_object *obj, skeleton_ray_prop_handler *hnd, zval *rv) /* {{{ */
{
    if (obj != NULL && hnd->read_vector3_func) {
        skeleton_vector3_object *ret;
        ret = hnd->read_vector3_func(obj);
        ZVAL_OBJ(rv, &ret->std);
    }

    return rv;
}
/* }}} */

static zval *skeleton_ray_get_property_ptr_ptr(zend_object *object, zend_string *name, int type, void **cache_slot) /* {{{ */
{
    skeleton_ray_object *obj;
    zval *retval = NULL;
    skeleton_ray_prop_handler *hnd = NULL;

    obj = skeleton_ray_fetch_object(object);

    if (obj->prop_handler != NULL) {
        hnd = zend_hash_find_ptr(obj->prop_handler, name);
    }

    if (hnd == NULL) {
        retval = zend_std_get_property_ptr_ptr(object, name, type, cache_slot);
    }

    return retval;
}
/* }}} */

static zval *skeleton_ray_read_property(zend_object *object, zend_string *name, int type, void **cache_slot, zval *rv) /* {{{ */
{
    skeleton_ray_object *obj;
    zval *retval = NULL;
    skeleton_ray_prop_handler *hnd = NULL;

    obj = skeleton_ray_fetch_object(object);

    if (obj->prop_handler != NULL) {
        hnd = zend_hash_find_ptr(obj->prop_handler, name);
    }

    if (hnd) {
        retval = skeleton_ray_property_reader(obj, hnd, rv);
    } else {
        retval = zend_std_read_property(object, name, type, cache_slot, rv);
    }

    return retval;
}
/* }}} */

/* {{{ skeleton_ray_write_property(zend_object *object, zval *name, zval *value[, const zend_literal *key])
   Generic object property writer */
zval *skeleton_ray_write_property(zend_object *object, zend_string *name, zval *value, void **cache_slot)
{
    skeleton_ray_object *obj;
    skeleton_ray_prop_handler *hnd;

    obj = skeleton_ray_fetch_object(object);

    if (obj->prop_handler != NULL) {
        hnd = zend_hash_find_ptr(obj->prop_handler, name);
    }

    if (hnd) {
        hnd->write_vector3_func(obj, value);
    } else {
        value = zend_std_write_property(object, name, value, cache_slot);
    }

    return value;
}
/* }}} */

static int skeleton_ray_has_property(zend_object *object, zend_string *name, int has_set_exists, void **cache_slot) /* {{{ */
{
    skeleton_ray_object *obj;
    skeleton_ray_prop_handler *hnd = NULL;
    int ret = 0;

    if ((hnd = zend_hash_find_ptr(obj->prop_handler, name)) != NULL) {
        switch (has_set_exists) {
            case ZEND_PROPERTY_EXISTS:
                ret = 1;
                break;
            case ZEND_PROPERTY_NOT_EMPTY: {
                zval rv;
                zval *value = skeleton_ray_read_property(object, name, BP_VAR_IS, cache_slot, &rv);
                if (value != &EG(uninitialized_zval)) {
                    convert_to_boolean(value);
                    ret = Z_TYPE_P(value) == IS_TRUE ? 1 : 0;
                }
                break;
            }
            case ZEND_PROPERTY_ISSET: {
                zval rv;
                zval *value = skeleton_ray_read_property(object, name, BP_VAR_IS, cache_slot, &rv);
                if (value != &EG(uninitialized_zval)) {
                    ret = Z_TYPE_P(value) != IS_NULL? 1 : 0;
                    zval_ptr_dtor(value);
                }
                break;
            }
            EMPTY_SWITCH_DEFAULT_CASE();
        }
    } else {
        ret = zend_std_has_property(object, name, has_set_exists, cache_slot);
    }

    return ret;
}
/* }}} */

static HashTable *skeleton_ray_get_gc(zend_object *object, zval **gc_data, int *gc_data_count) /* {{{ */
{
    *gc_data = NULL;
    *gc_data_count = 0;
    return zend_std_get_properties(object);
}
/* }}} */

static HashTable *skeleton_ray_get_properties(zend_object *object)/* {{{ */
{
    skeleton_ray_object *obj;
    HashTable *props;
    skeleton_ray_prop_handler *hnd;
    zend_string *key;

    obj = skeleton_ray_fetch_object(object);
    props = zend_std_get_properties(object);

    if (obj->prop_handler == NULL) {
        return NULL;
    }

    ZEND_HASH_FOREACH_STR_KEY_PTR(obj->prop_handler, key, hnd) {
        zval *ret, val;
        ret = skeleton_ray_property_reader(obj, hnd, &val);
        if (ret == NULL) {
            ret = &EG(uninitialized_zval);
        }
        zend_hash_update(props, key, ret);
    } ZEND_HASH_FOREACH_END();

    return props;
}
/* }}} */

void skeleton_ray_free_storage(zend_object *object)
{
    skeleton_ray_object *intern = skeleton_ray_fetch_object(object);

    zend_object_std_dtor(&intern->std);
}

zend_object * skeleton_ray_new(zend_class_entry *ce)
{
    skeleton_ray_object *intern;
    intern = (skeleton_ray_object*) ecalloc(1, sizeof(skeleton_ray_object) + zend_object_properties_size(ce));
    intern->prop_handler = &skeleton_ray_prop_handlers;

    zend_object_std_init(&intern->std, ce);
    object_properties_init(&intern->std, ce);

    intern->std.handlers = &skeleton_ray_object_handlers;

    return &intern->std;
}

static zend_object *skeleton_ray_clone(zend_object *object)
{
    skeleton_ray_object *old_object;
    zend_object *new_object;

    old_object = skeleton_ray_fetch_object(object);
    new_object = skeleton_ray_new(old_object->std.ce);

    skeleton_ray_object *new_ray = skeleton_ray_fetch_object(new_object);
    new_ray->ray = old_object->ray;

    return new_object;
}

// PHP property handling

static skeleton_vector3_object * skeleton_ray_direction(skeleton_ray_object *obj) /* {{{ */
{
    return obj->direction;
}

static skeleton_vector3_object * skeleton_ray_position(skeleton_ray_object *obj) /* {{{ */
{
    return obj->position;
}
/* }}} */


static int skeleton_ray_write_direction(skeleton_ray_object *ray_object, zval *newval) /* {{{ */
{
    int ret = SUCCESS;

    if (Z_TYPE_P(newval) == IS_NULL) {
        // Cannot set this to null...
        return ret;
    }

    skeleton_vector3_object *phpDirection = Z_VECTOR3_OBJ_P(newval);
    ray_object->direction = phpDirection;

    return ret;
}
/* }}} */

static int skeleton_ray_write_position(skeleton_ray_object *ray_object, zval *newval) /* {{{ */
{
    int ret = SUCCESS;

    if (Z_TYPE_P(newval) == IS_NULL) {
        // Cannot set this to null...
        return ret;
    }

    skeleton_vector3_object *phpPosition = Z_VECTOR3_OBJ_P(newval);
    ray_object->position = phpPosition;

    return ret;
}
/* }}} */

// PHP object handling

ZEND_BEGIN_ARG_INFO_EX(arginfo_ray__construct, 0, 0, 2)
                ZEND_ARG_INFO(0, position)
                ZEND_ARG_INFO(0, direction)
ZEND_END_ARG_INFO()
PHP_METHOD(Ray, __construct)
{
    zval *position;
    zval *direction;

    ZEND_PARSE_PARAMETERS_START(2, 2)
            Z_PARAM_ZVAL(position)
            Z_PARAM_ZVAL(direction)
    ZEND_PARSE_PARAMETERS_END();

    skeleton_vector3_object *phpPosition = Z_VECTOR3_OBJ_P(position);
    skeleton_vector3_object *phpDirection = Z_VECTOR3_OBJ_P(direction);

    skeleton_ray_object *intern = Z_RAY_OBJ_P(ZEND_THIS);

    intern->position = phpPosition;
    intern->direction = phpDirection;
    intern->ray = (Ray) {
            .position = phpPosition->vector3,
            .direction = phpDirection->vector3,
    };

}

ZEND_BEGIN_ARG_INFO_EX(arginfo_ray_getPosition, 0, 0, 0)
ZEND_END_ARG_INFO()
PHP_METHOD(Ray, getPosition)
{
    skeleton_ray_object *intern = Z_RAY_OBJ_P(ZEND_THIS);

    RETURN_OBJ(&intern->position->std);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_ray_setPosition, 0, 0, 1)
                ZEND_ARG_INFO(0, position)
ZEND_END_ARG_INFO()
PHP_METHOD(Ray, setPosition)
{
    zval *position;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_ZVAL(position)
    ZEND_PARSE_PARAMETERS_END();

    skeleton_vector3_object *phpPosition = Z_VECTOR3_OBJ_P(position);

    skeleton_ray_object *intern = Z_RAY_OBJ_P(ZEND_THIS);

    intern->ray.position = phpPosition->vector3;
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_ray_getDirection, 0, 0, 0)
ZEND_END_ARG_INFO()
PHP_METHOD(Ray, getDirection)
{
    skeleton_ray_object *intern = Z_RAY_OBJ_P(ZEND_THIS);

    RETURN_OBJ(&intern->direction->std);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_ray_setDirection, 0, 0, 1)
                ZEND_ARG_INFO(0, direction)
ZEND_END_ARG_INFO()
PHP_METHOD(Ray, setDirection)
{
    zval *direction;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_ZVAL(direction)
    ZEND_PARSE_PARAMETERS_END();

    skeleton_vector3_object *phpDirection = Z_VECTOR3_OBJ_P(direction);

    skeleton_ray_object *intern = Z_RAY_OBJ_P(ZEND_THIS);
    intern->direction = phpDirection;

    intern->ray.direction = phpDirection->vector3;
}


const zend_function_entry skeleton_ray_methods[] = {
        PHP_ME(Ray, __construct, arginfo_ray__construct, ZEND_ACC_PUBLIC)
        PHP_ME(Ray, getPosition, arginfo_ray_getPosition, ZEND_ACC_PUBLIC)
        PHP_ME(Ray, setPosition, arginfo_ray_setPosition, ZEND_ACC_PUBLIC)
        PHP_ME(Ray, getDirection, arginfo_ray_getDirection, ZEND_ACC_PUBLIC)
        PHP_ME(Ray, setDirection, arginfo_ray_setDirection, ZEND_ACC_PUBLIC)
        PHP_FE_END
};

static void skeleton_ray_free_prop_handler(zval *el) /* {{{ */ {
    pefree(Z_PTR_P(el), 1);
} /* }}} */

// Extension class startup

void skeleton_ray_startup(INIT_FUNC_ARGS)
{
    zend_class_entry ce;

    memcpy(&skeleton_ray_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    skeleton_ray_object_handlers.offset    = XtOffsetOf(skeleton_ray_object, std);
    skeleton_ray_object_handlers.free_obj  = &skeleton_ray_free_storage;
    skeleton_ray_object_handlers.clone_obj = skeleton_ray_clone;

    // Props
    skeleton_ray_object_handlers.get_property_ptr_ptr = skeleton_ray_get_property_ptr_ptr;
    skeleton_ray_object_handlers.get_gc               = skeleton_ray_get_gc;
    skeleton_ray_object_handlers.get_properties       = skeleton_ray_get_properties;
    skeleton_ray_object_handlers.read_property	    = skeleton_ray_read_property;
    skeleton_ray_object_handlers.write_property       = skeleton_ray_write_property;
    skeleton_ray_object_handlers.has_property	        = skeleton_ray_has_property;

    // Init
    INIT_NS_CLASS_ENTRY(ce, "skeleton", "Ray", skeleton_ray_methods);
    skeleton_ray_ce = zend_register_internal_class(&ce);
    skeleton_ray_ce->create_object = skeleton_ray_new;

    // Props
    zend_hash_init(&skeleton_ray_prop_handlers, 0, NULL, skeleton_ray_free_prop_handler, 1);
    skeleton_ray_register_prop_handler(&skeleton_ray_prop_handlers, "position", skeleton_ray_position, skeleton_ray_write_position);
    skeleton_ray_register_prop_handler(&skeleton_ray_prop_handlers, "direction", skeleton_ray_direction, skeleton_ray_write_direction);
}