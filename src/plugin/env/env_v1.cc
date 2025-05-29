#include "comm.h"
#include "nccl_env.h"

static ncclEnv_v1_t* ncclEnv_v1;

ncclEnv_t* getNcclEnv_v1(void* lib) {
  ncclEnv_v1 = (ncclEnv_v1_t*)dlsym(lib, "ncclEnv_v1");
  if (ncclEnv_v1) {
    INFO(NCCL_INIT|NCCL_ENV, "ENV/Plugin: Loaded env plugin %s (v1)", ncclEnv_v1->name);
    return ncclEnv_v1;
  }
  INFO(NCCL_INIT|NCCL_ENV, "ENV/Plugin: Failed to find ncclEnv_v1 ncclEnvPlugin_v1 symbol.");
  return nullptr;
}
