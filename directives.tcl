############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2018 Xilinx, Inc. All Rights Reserved.
############################################################
set_directive_array_partition -type block -factor 3 -dim 1 "baseline" local_image
set_directive_array_partition -type complete -dim 0 "baseline" local_kernel
set_directive_array_partition -type block -factor 4 -dim 2 "baseline" local_image
set_directive_array_partition -type block -factor 5 -dim 1 "baseline" local_output
set_directive_pipeline "compute1/LW"
set_directive_dependence -variable local_image -type intra -dependent false "baseline/LH"
set_directive_pipeline -II 1 -enable_flush -rewind "baseline/IK0"
