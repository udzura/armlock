require "mkmf"

abort "missing libapparmor" unless have_library("apparmor")
abort "missing aa_change_hat" unless have_func("aa_change_hat")

create_makefile("armlock/armlock")
