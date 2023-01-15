﻿#pragma once

// Helpers
#include "include/Helpers/qgl_color_helpers.h"
#include "include/Helpers/qgl_window_helpers.h"

// Memory
#include "include/GPU/Memory/qgl_igpu_allocator.h"
#include "include/GPU/Memory/qgl_slot_allocator.h"
#include "include/GPU/Memory/qgl_committed_allocator.h"
#include "include/GPU/Memory/qgl_tex2d_allocator.h"

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
#include "include/GPU/Command-Lists/qgl_cmd_executor.h"

#include "include/GPU/qgl_ipso.h"
#include "include/GPU/qgl_graphics_pso.h"
#include "include/GPU/qgl_frame.h"

// Synchronization
#include "include/GPU/Synchronization/qgl_fence.h"
#include "include/GPU/Synchronization/qgl_sync_object.h"

// Components
#include "include/Descriptors/qgL_text_format_descriptor.h"
#include "include/Components/Content/qgl_wtext.h"