#include "armlock.h"

#include <stdlib.h>
#include <sys/random.h>
#include <sys/apparmor.h>

#include <stdio.h>

VALUE rb_mArmlock;

struct rb_apparmor_magic_token {
  unsigned long magic_token;
};

static const rb_data_type_t armlock_type = {
  "armlock",
  {0, -1, 0},
  0, 0,
  0
};

static VALUE
armlock_alloc(VALUE klass){
  struct rb_apparmor_magic_token *token;
  return TypedData_Make_Struct(klass, struct rb_apparmor_magic_token, &armlock_type, token);
}

static VALUE
armlock_initialize(VALUE obj){
  struct rb_apparmor_magic_token *token;
  unsigned long newtok = (unsigned long)random();
  TypedData_Get_Struct(obj, struct rb_apparmor_magic_token, &armlock_type, token);
  token->magic_token = newtok;
  return obj;
}

static VALUE
armlock_change_hat(VALUE obj, VALUE profile_name){
  unsigned long magic_token;
  struct rb_apparmor_magic_token *token;
  Check_Type(profile_name, T_STRING);

  TypedData_Get_Struct(obj, struct rb_apparmor_magic_token, &armlock_type, token);

  if(aa_change_hat(RSTRING_PTR(profile_name), token->magic_token) < 0){
    perror("Warning: ");
    return Qfalse;
  } else {
    return Qtrue;
  }
}

static VALUE
armlock_recover_hat(VALUE obj){
  unsigned long magic_token;
  struct rb_apparmor_magic_token *token;
  TypedData_Get_Struct(obj, struct rb_apparmor_magic_token, &armlock_type, token);

  if(aa_change_hat(NULL, token->magic_token) < 0){
    perror("Warning: ");
    return Qfalse;
  } else {
    return Qtrue;
  }
}

void
Init_armlock(void)
{
  rb_mArmlock = rb_define_class("Armlock", rb_cObject);
  rb_define_alloc_func(rb_mArmlock, armlock_alloc);
  rb_define_method(rb_mArmlock, "initialize", armlock_initialize, 0);
  rb_define_method(rb_mArmlock, "change_hat", armlock_change_hat, 1);
  rb_define_method(rb_mArmlock, "recover_hat", armlock_recover_hat, 0);
}
