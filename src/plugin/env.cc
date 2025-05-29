#include "param.h"
#include "env.h"
#include "checks.h"
#include "plugin.h"

extern ncclEnv_t* getNcclEnv_v1(void* envPluginLib);

static pthread_mutex_t envLock = PTHREAD_MUTEX_INITIALIZER;
static int envPluginRefCount;
static void* envPluginLib;
static ncclEnv_t* ncclEnv;

enum {
  envPluginLoadFailed = -1,
  envPluginLoadReady = 0,
  envPluginLoadSuccess = 1,
};

static int envPluginStatus = envPluginLoadReady;

static ncclResult_t ncclEnvPluginLoad(void) {
  if (envPluginLoadFailed == envPluginStatus) {
    return ncclSuccess;
  }

  pthread_mutex_lock(&envLock);
  if (envPluginLoadSuccess == envPluginStatus) {
    ++envPluginRefCount;
    goto exit;
  }

  envPluginLib = ncclOpenEnvPluginLib(getenv("NCCL_ENV_PLUGIN"));
  if (envPluginLib == nullptr) {
    goto fail;
  }

  ncclEnv = getNcclEnv_v1(envPluginLib);
  if (ncclEnv == NULL) {
    goto fail;
  }

  ++envPluginRefCount;
  envPluginStatus = envPluginLoadSuccess;

exit:
  pthread_mutex_unlock(&envLock);
  return ncclSuccess;
fail:
  if (envPluginLib) NCCLCHECK(ncclClosePluginLib(envPluginLib));
  envPluginStatus = envPluginLoadFailed;
  goto exit;
}

static ncclResult_t ncclEnvPluginUnload(void) {
  pthread_mutex_lock(&envLock);
  if (0 == (--envPluginRefCount)) {
    INFO(NCCL_ENV, "ENV/Plugin: Closing env plugin %s", ncclEnv->name);
    NCCLCHECK(ncclClosePluginLib(envPluginLib));
    envPluginLib = nullptr;
    ncclEnv = nullptr;
    envPluginStatus = envPluginLoadReady;
  }
  pthread_mutex_unlock(&envLock);
  return ncclSuccess;
}


ncclResult_t ncclEnvPlugininitEnv(uint8_t nccl_major, uint8_t nccl_minor, uint8_t nccl_patch, const char* suffix, const char* backend) {
  ncclEnvPluginLoad();
  if (__builtin_expect(ncclEnv != NULL, 0)) {
    int err = ncclEnv->initEnv(nccl_major, nccl_minor, nccl_patch, suffix, backend);
    if (err) {
      WARN("Env init failed with error (%d). Continue without env.", err);
      ncclEnv = NULL;
    }
  }
  return ncclSuccess;
}
