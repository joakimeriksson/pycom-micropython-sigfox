#include "py/nlr.h"
#include "py/obj.h"
#include "py/runtime.h"
#include "py/binary.h"
#include "portmodules.h"
#include <stdio.h>

STATIC mp_obj_t mymodule_hello(void) {
    printf("Hello world!\n");
    return mp_const_none;
}
STATIC mp_obj_t mymodule_hello2(mp_obj_t what) {
    printf("Hello %s!\n", mp_obj_str_get_str(what));
    return mp_const_none;
}

// this is the actual C-structure for our new object
typedef struct _mymodule_hello_obj_t {
    // base represents some basic information, like type
    mp_obj_base_t base;
    // a member created by us
    uint8_t hello_number;
    /* a callback function - in python */
    mp_obj_t callback;
} mymodule_hello_obj_t;

STATIC void mymodule_hello_print( const mp_print_t *print,
                                  mp_obj_t self_in,
                                  mp_print_kind_t kind ) {
    // get a ptr to the C-struct of the object
    mymodule_hello_obj_t *self = MP_OBJ_TO_PTR(self_in);
    // print the number
    printf("Hello(%u)\n", self->hello_number);
};

STATIC mp_obj_t mymodule_hello_increment(mp_obj_t self_in) {
    mymodule_hello_obj_t *self = MP_OBJ_TO_PTR(self_in);
    self->hello_number += 1;
    return mp_const_none;
};

STATIC mp_obj_t mymodule_callback(mp_obj_t self_in, mp_obj_t callback);

static MP_DEFINE_CONST_FUN_OBJ_1(mymodule_hello_increment_obj,
                                 mymodule_hello_increment);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mymodule_callback_obj,
                                 mymodule_callback);

// creating the table of global members
STATIC const mp_rom_map_elem_t mymodule_hello_locals_dict_table[] = {
  { MP_ROM_QSTR(MP_QSTR_inc), MP_ROM_PTR(&mymodule_hello_increment_obj) },
  { MP_ROM_QSTR(MP_QSTR_callback), MP_ROM_PTR(&mymodule_callback_obj) },
};

STATIC MP_DEFINE_CONST_DICT(mymodule_hello_locals_dict,
                            mymodule_hello_locals_dict_table);

const mp_obj_type_t mymodule_helloObj_type;
mp_obj_t mymodule_hello_make_new( const mp_obj_type_t *type,
                                  size_t n_args,
                                  size_t n_kw,
                                  const mp_obj_t *args ) {
    // this checks the number of arguments (min 1, max 1);
    // on error -> raise python exception
    mp_arg_check_num(n_args, n_kw, 1, 1, true);
    // create a new object of our C-struct type
    mymodule_hello_obj_t *self = m_new_obj(mymodule_hello_obj_t);
    // give it a type
    self->base.type = &mymodule_helloObj_type;
    // set the member number with the first argument of the constructor
    self->hello_number = mp_obj_get_int(args[0]);
    return MP_OBJ_FROM_PTR(self);
}

/// \method callback(fun)
/// Set the function to be called when the timer triggers.
/// `fun` is passed 1 argument, the timer object.
/// If `fun` is `None` then the callback will be disabled.
STATIC mp_obj_t mymodule_callback(mp_obj_t self_in, mp_obj_t callback) {
    mymodule_hello_obj_t *self = self_in;
    if (callback == mp_const_none) {
        self->callback = mp_const_none;
    } else if (mp_obj_is_callable(callback)) {
        self->callback = callback;
        printf("The callback was set - lets call it!\n");
        // Can be called like this?
        mp_call_function_1(callback, self);
    } else {
        mp_raise_ValueError("callback must be None or a callable object");
    }
    return mp_const_none;
}

// create the class-object itself
const mp_obj_type_t mymodule_helloObj_type = {
    // "inherit" the type "type"
    { &mp_type_type },
     // give it a name
    .name = MP_QSTR_helloObj,
     // give it a print-function
    .print = mymodule_hello_print,
     // give it a constructor
    .make_new = mymodule_hello_make_new,
     // and the global members
    .locals_dict = (mp_obj_dict_t*)&mymodule_hello_locals_dict,
};


STATIC MP_DEFINE_CONST_FUN_OBJ_0(mymodule_hello_obj, mymodule_hello);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mymodule_hello2_obj, mymodule_hello2);

STATIC const mp_map_elem_t mp_module_ulwm2m_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_mymodule) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_hello), (mp_obj_t)&mymodule_hello_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_hello2), (mp_obj_t)&mymodule_hello2_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_helloObj), (mp_obj_t)&mymodule_helloObj_type },
};

STATIC MP_DEFINE_CONST_DICT (
    mp_module_ulwm2m_globals,
    mp_module_ulwm2m_globals_table
);

const mp_obj_module_t mp_module_ulwm2m = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_ulwm2m_globals,
};
