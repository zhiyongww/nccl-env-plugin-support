/*************************************************************************
 * Copyright (c) 2022, NVIDIA CORPORATION. All rights reserved.
 *
 * See LICENSE.txt for license information
 ************************************************************************/

#ifndef ENV_H_
#define ENV_H_

#include "nccl.h"
#include "nccl_env.h"

ncclResult_t ncclEnvPlugininitEnv(uint8_t nccl_major, uint8_t nccl_minor, uint8_t nccl_patch, const char* suffix, const char* backend);

#endif
