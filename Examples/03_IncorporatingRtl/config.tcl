create_project -part xcu250-figd2104-2L-e -force [lindex $argv 1]/Targets/[lindex $argv 2]_temp [lindex $argv 2]
add_files [lindex $argv 3]
ipx::package_project -import_files -root_dir [lindex $argv 1]/Targets/[lindex $argv 2]_temp/ip_repo
set_property sdx_kernel true [ipx::current_core]
set_property sdx_kernel_type rtl [ipx::current_core]
package_xo  -force -xo_path [lindex $argv 1]/Targets/[lindex $argv 2].xo -kernel_name [lindex $argv 2] -ip_directory [lindex $argv 1]/Targets/[lindex $argv 2]_temp/ip_repo -ctrl_protocol ap_ctrl_none

