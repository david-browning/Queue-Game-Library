#pragma once

// Helpers
#include "include/Helpers/qgl_color_helpers.h"
#include "include/Helpers/qgl_window_helpers.h"

// Shaders
#include "include/Shaders/qgl_shader.h"
#include "include/Shaders/qgl_shader_lib.h"
#include "include/Shaders/qgl_sampler.h"
#include "include/Shaders/qgl_shader_metadata.h"
#include "include/Shaders/qgl_vertex_layout.h"
#include "include/Shaders/qgl_vert_types.h"

// Stagers
#include "include/Stagers/qgl_frame_stager.h"
#include "include/Stagers/qgl_ibindable_stager.h"
#include "include/Stagers/qgl_shader_stager.h"
#include "include/Stagers/qgl_vert_stager.h"

// Core Objects
#include "include/qgl_window.h"
#include "include/Helpers/qgl_graphics_device_helpers.h"
#include "include/GPU/Command-Lists/qgl_icmd_list.h"
#include "include/GPU/Command-Lists/qgl_graphics_cmd_list.h"

#include "include/GPU/qgl_ipso.h"
#include "include/GPU/qgl_graphics_pso.h"
#include "include/GPU/qgl_frame.h"

// Components
#include "include/Descriptors/qgL_text_format_descriptor.h"
#include "include/Components/Content/qgl_text_format.h"
#include "include/Components/Content/qgl_wtext.h"