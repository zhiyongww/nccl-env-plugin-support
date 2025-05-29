#ifndef ENV_V1_H_
#define ENV_V1_H_

#include <string>
#include <cstdint>

typedef struct {
  const char* name;
  ncclResult_t (*initEnv)(uint8_t nccl_major, uint8_t nccl_minor, uint8_t nccl_patch, const char* suffix, const char* backend);
} ncclEnv_v1_t;

#endif
