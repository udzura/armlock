#include "armlock.h"

VALUE rb_mArmlock;

void
Init_armlock(void)
{
  rb_mArmlock = rb_define_module("Armlock");
}
