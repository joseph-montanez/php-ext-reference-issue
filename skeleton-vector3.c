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

#include "raylib.h"
#include "skeleton-vector3.h"

//------------------------------------------------------------------------------------------------------
//-- raylib Vector3 PHP Custom Object
//------------------------------------------------------------------------------------------------------
zend_object_handlers skeleton_vector3_object_handlers;

static HashTable skeleton_vector3_prop_handlers;

typedef double (*skeleton_vector3_read_float_t)(skeleton_vector3_object *obj);

typedef int (*skeleton_vector3_write_float_t)(skeleton_vector3_object *obj, zval *value);

typedef struct _skeletonvector3_prop_handler {
    skeleton_vector3_read_float_t read_float_func;
    skeleton_vector3_write_float_t write_float_func;
} skeleton_vector3_prop_handler;
/* }}} */

static void skeleton_vector3_register_prop_handler(HashTable *prop_handler, char *name, skeleton_vector3_read_float_t read_float_func, skeleton_vector3_write_float_t write_float_func) /* {{{ */
{
    skeleton_vector3_prop_handler hnd;

    hnd.read_float_func = read_float_func;
    hnd.write_float_func = write_float_func;
    zend_hash_str_add_mem(prop_handler, name, strlen(name), &hnd, sizeof(skeleton_vector3_prop_handler));

    /* Register for reflection */
    zend_declare_property_null(skeleton_vector3_ce, name, strlen(name), ZEND_ACC_PUBLIC);
}
/* }}} */

static zval *skeleton_vector3_property_reader(skeleton_vector3_object *obj, skeleton_vector3_prop_handler *hnd, zval *rv) /* {{{ */
{
    double ret = 0;

    if (obj != NULL && hnd->read_float_func) {
//        php_error_docref(NULL, E_WARNING, "Internal raylib vector3 found");
        ret = hnd->read_float_func(obj);
    } else {
//        php_error_docref(NULL, E_WARNING, "Internal raylib vectro2 error returned");
    }

    ZVAL_DOUBLE(rv, (double) ret);

    return rv;
}
/* }}} */

static zval *skeleton_vector3_get_property_ptr_ptr(zend_object *object, zend_string *name, int type, void **cache_slot) /* {{{ */
{
    skeleton_vector3_object *obj;
    zval *retval = NULL;
    skeleton_vector3_prop_handler *hnd = NULL;

    obj = skeleton_vector3_fetch_object(object);

    if (obj->prop_handler != NULL) {
        hnd = zend_hash_find_ptr(obj->prop_handler, name);
    }

    if (hnd == NULL) {
        retval = zend_std_get_property_ptr_ptr(object, name, type, cache_slot);
    }

    return retval;
}
/* }}} */

static zval *skeleton_vector3_read_property(zend_object *object, zend_string *name, int type, void **cache_slot, zval *rv) /* {{{ */
{
    skeleton_vector3_object *obj;
    zval *retval = NULL;
    skeleton_vector3_prop_handler *hnd = NULL;

    obj = skeleton_vector3_fetch_object(object);

    if (obj->prop_handler != NULL) {
        hnd = zend_hash_find_ptr(obj->prop_handler, name);
    }

    if (hnd) {
        retval = skeleton_vector3_property_reader(obj, hnd, rv);
    } else {
        retval = zend_std_read_property(object, name, type, cache_slot, rv);
    }

    return retval;
}
/* }}} */

/* {{{ skeleton_vector3_write_property(zend_object *object, zend_string *member, zval *value, void **cache_slot)
   Generic object property writer */
zval *skeleton_vector3_write_property(zend_object *object, zend_string *member, zval *value, void **cache_slot)
{
    skeleton_vector3_object *obj;
    skeleton_vector3_prop_handler *hnd;

    obj = skeleton_vector3_fetch_object(object);

    if (obj->prop_handler != NULL) {
        hnd = zend_hash_find_ptr(obj->prop_handler, member);
    }

    if (hnd) {
        hnd->write_float_func(obj, value);
    } else {
        value = zend_std_write_property(object, member, value, cache_slot);
    }

    return value;
}
/* }}} */

static int skeleton_vector3_has_property(zend_object *object, zend_string *name, int has_set_exists, void **cache_slot) /* {{{ */
{
    skeleton_vector3_object *obj;
    skeleton_vector3_prop_handler *hnd = NULL;
    int ret = 0;

    if ((hnd = zend_hash_find_ptr(obj->prop_handler, name)) != NULL) {
        switch (has_set_exists) {
            case ZEND_PROPERTY_EXISTS:
                ret = 1;
                break;
            case ZEND_PROPERTY_NOT_EMPTY: {
                zval rv;
                zval *value = skeleton_vector3_read_property(object, name, BP_VAR_IS, cache_slot, &rv);
                if (value != &EG(uninitialized_zval)) {
                    convert_to_boolean(value);
                    ret = Z_TYPE_P(value) == IS_TRUE ? 1 : 0;
                }
                break;
            }
            case ZEND_PROPERTY_ISSET: {
                zval rv;
                zval *value = skeleton_vector3_read_property(object, name, BP_VAR_IS, cache_slot, &rv);
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

static HashTable *skeleton_vector3_get_gc(zend_object *object, zval **gc_data, int *gc_data_count) /* {{{ */
{
    *gc_data = NULL;
    *gc_data_count = 0;
    return zend_std_get_properties(object);
}
/* }}} */

static HashTable *skeleton_vector3_get_properties(zend_object *object)/* {{{ */
{
    skeleton_vector3_object *obj;
    HashTable *props;
    skeleton_vector3_prop_handler *hnd;
    zend_string *key;

    obj = skeleton_vector3_fetch_object(object);
    props = zend_std_get_properties(object);

    if (obj->prop_handler == NULL) {
        return NULL;
    }

    ZEND_HASH_FOREACH_STR_KEY_PTR(obj->prop_handler, key, hnd) {
        zval *ret, val;
        ret = skeleton_vector3_property_reader(obj, hnd, &val);
        if (ret == NULL) {
            ret = &EG(uninitialized_zval);
        }
        zend_hash_update(props, key, ret);
    } ZEND_HASH_FOREACH_END();

    return props;
}
/* }}} */

void skeleton_vector3_free_storage(zend_object *object)
{
    skeleton_vector3_object *intern = skeleton_vector3_fetch_object(object);

    zend_object_std_dtor(&intern->std);
}

zend_object * skeleton_vector3_new_ex(zend_class_entry *ce, zend_object *orig)
{
    skeleton_vector3_object *intern;

    intern = zend_object_alloc(sizeof(skeleton_vector3_object), ce);

    intern->prop_handler = &skeleton_vector3_prop_handlers;

    if (orig) {
        skeleton_vector3_object *other = skeleton_vector3_fetch_object(orig);
        intern->vector3 = (Vector3) {
                .x = other->vector3.x,
                .y = other->vector3.y,
                .z = other->vector3.z
        };
    }

    zend_object_std_init(&intern->std, ce);
    object_properties_init(&intern->std, ce);
    intern->std.handlers = &skeleton_vector3_object_handlers;

    return &intern->std;
}


/* {{{  */
static zend_object *skeleton_vector3_new(zend_class_entry *class_type)
{
    return skeleton_vector3_new_ex(class_type, NULL);
}
/* }}} */

static zend_object *skeleton_vector3_clone(zend_object *old_object)
{
    zend_object *new_object;

    new_object = skeleton_vector3_new_ex(old_object->ce, old_object);

    zend_objects_clone_members(new_object, old_object);

    return new_object;
}

// PHP property handling

static double skeleton_vector3_x(skeleton_vector3_object *obj) /* {{{ */
{
    return (double) obj->vector3.x;
}
/* }}} */

static double skeleton_vector3_y(skeleton_vector3_object *obj) /* {{{ */
{
    return (double) obj->vector3.y;
}
/* }}} */

static double skeleton_vector3_z(skeleton_vector3_object *obj) /* {{{ */
{
    return (double) obj->vector3.z;
}
/* }}} */

static int skeleton_vector3_write_x(skeleton_vector3_object *vector3_object, zval *newval) /* {{{ */
{
    int ret = SUCCESS;

    if (Z_TYPE_P(newval) == IS_NULL) {
        vector3_object->vector3.x = 0;
        return ret;
    }

    vector3_object->vector3.x = (float) zval_get_double(newval);

    return ret;
}
/* }}} */

static int skeleton_vector3_write_y(skeleton_vector3_object *vector3_object, zval *newval) /* {{{ */
{
    int ret = SUCCESS;

    if (Z_TYPE_P(newval) == IS_NULL) {
        vector3_object->vector3.y = 0;
        return ret;
    }

    vector3_object->vector3.y = (float) zval_get_double(newval);

    return ret;
}
/* }}} */

static int skeleton_vector3_write_z(skeleton_vector3_object *vector3_object, zval *newval) /* {{{ */
{
    int ret = SUCCESS;

    if (Z_TYPE_P(newval) == IS_NULL) {
        vector3_object->vector3.z = 0;
        return ret;
    }

    vector3_object->vector3.z = (float) zval_get_double(newval);

    return ret;
}
/* }}} */

// PHP object handling
ZEND_BEGIN_ARG_INFO_EX(arginfo_vector3__construct, 0, 0, 3)
                ZEND_ARG_INFO(0, x)
                ZEND_ARG_INFO(0, y)
                ZEND_ARG_INFO(0, z)
ZEND_END_ARG_INFO()
PHP_METHOD(Vector3, __construct)
{
    double x;
    double y;
    double z;

    ZEND_PARSE_PARAMETERS_START(3, 3)
        Z_PARAM_DOUBLE(x)
        Z_PARAM_DOUBLE(y)
        Z_PARAM_DOUBLE(z)
    ZEND_PARSE_PARAMETERS_END();

    skeleton_vector3_object *intern = Z_VECTOR3_OBJ_P(ZEND_THIS);

    intern->vector3 = (Vector3) {
        .x = (float) x,
        .y = (float) y,
        .z = (float) z
    };

}

ZEND_BEGIN_ARG_INFO_EX(arginfo_vector3_getX, 0, 0, 0)
ZEND_END_ARG_INFO()
PHP_METHOD(Vector3, getX)
{
    skeleton_vector3_object *intern = Z_VECTOR3_OBJ_P(ZEND_THIS);
    RETURN_DOUBLE(intern->vector3.x);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_vector3_setX, 0, 0, 1)
                ZEND_ARG_INFO(0, x)
ZEND_END_ARG_INFO()
PHP_METHOD(Vector3, setX)
{
    double x;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_DOUBLE(x)
    ZEND_PARSE_PARAMETERS_END();

    skeleton_vector3_object *intern = Z_VECTOR3_OBJ_P(ZEND_THIS);

    intern->vector3.x = (float) x;
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_vector3_getY, 0, 0, 0)
ZEND_END_ARG_INFO()
PHP_METHOD(Vector3, getY)
{
    skeleton_vector3_object *intern = Z_VECTOR3_OBJ_P(ZEND_THIS);
    RETURN_DOUBLE(intern->vector3.y);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_vector3_setY, 0, 0, 1)
                ZEND_ARG_INFO(0, y)
ZEND_END_ARG_INFO()
PHP_METHOD(Vector3, setY)
{
    double y;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_DOUBLE(y)
    ZEND_PARSE_PARAMETERS_END();

    skeleton_vector3_object *intern = Z_VECTOR3_OBJ_P(ZEND_THIS);

    intern->vector3.y = (float) y;
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_vector3_getZ, 0, 0, 0)
ZEND_END_ARG_INFO()
PHP_METHOD(Vector3, getZ)
{
    skeleton_vector3_object *intern = Z_VECTOR3_OBJ_P(ZEND_THIS);
    RETURN_DOUBLE(intern->vector3.z);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_vector3_setZ, 0, 0, 1)
                ZEND_ARG_INFO(0, z)
ZEND_END_ARG_INFO()
PHP_METHOD(Vector3, setZ)
{
    double z;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_DOUBLE(z)
    ZEND_PARSE_PARAMETERS_END();

    skeleton_vector3_object *intern = Z_VECTOR3_OBJ_P(ZEND_THIS);

    intern->vector3.z = (float) z;
}

const zend_function_entry skeleton_vector3_methods[] = {
        PHP_ME(Vector3, __construct, arginfo_vector3__construct, ZEND_ACC_PUBLIC)
        PHP_ME(Vector3, getX, arginfo_vector3_getX, ZEND_ACC_PUBLIC)
        PHP_ME(Vector3, setX, arginfo_vector3_setX, ZEND_ACC_PUBLIC)
        PHP_ME(Vector3, getY, arginfo_vector3_getY, ZEND_ACC_PUBLIC)
        PHP_ME(Vector3, setY, arginfo_vector3_setY, ZEND_ACC_PUBLIC)
        PHP_ME(Vector3, getZ, arginfo_vector3_getZ, ZEND_ACC_PUBLIC)
        PHP_ME(Vector3, setZ, arginfo_vector3_setZ, ZEND_ACC_PUBLIC)
        PHP_FE_END
};

static void skeleton_vector3_free_prop_handler(zval *el) /* {{{ */ {
    pefree(Z_PTR_P(el), 1);
} /* }}} */

// Extension class startup

void skeleton_vector3_startup(INIT_FUNC_ARGS)
{
    zend_class_entry ce;

    memcpy(&skeleton_vector3_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    skeleton_vector3_object_handlers.offset = XtOffsetOf(skeleton_vector3_object, std);
    skeleton_vector3_object_handlers.free_obj = &skeleton_vector3_free_storage;
    skeleton_vector3_object_handlers.clone_obj = skeleton_vector3_clone;

    // Props
    skeleton_vector3_object_handlers.get_property_ptr_ptr = skeleton_vector3_get_property_ptr_ptr;
    skeleton_vector3_object_handlers.get_gc               = skeleton_vector3_get_gc;
    skeleton_vector3_object_handlers.get_properties       = skeleton_vector3_get_properties;
    skeleton_vector3_object_handlers.read_property	    = skeleton_vector3_read_property;
    skeleton_vector3_object_handlers.write_property       = skeleton_vector3_write_property;
    skeleton_vector3_object_handlers.has_property	        = skeleton_vector3_has_property;

    // Init
    INIT_NS_CLASS_ENTRY(ce, "skeleton", "Vector3", skeleton_vector3_methods);
    skeleton_vector3_ce = zend_register_internal_class(&ce);
    skeleton_vector3_ce->create_object = skeleton_vector3_new;

    // Props
    zend_hash_init(&skeleton_vector3_prop_handlers, 0, NULL, skeleton_vector3_free_prop_handler, 1);
    skeleton_vector3_register_prop_handler(&skeleton_vector3_prop_handlers, "x", skeleton_vector3_x, skeleton_vector3_write_x);
    skeleton_vector3_register_prop_handler(&skeleton_vector3_prop_handlers, "y", skeleton_vector3_y, skeleton_vector3_write_y);
    skeleton_vector3_register_prop_handler(&skeleton_vector3_prop_handlers, "z", skeleton_vector3_z, skeleton_vector3_write_z);
}