#pragma once

#include <stdint.h>
#include <stdlib.h>

void log_commit(uint16_t log_, void * ptr, size_t len);

#define CAT(a, b) (a ## b)
#define STR(str) #str

typedef enum {
    ERR,
    WRN,
    INF,
    DBG,
    VRB
} log_lvl_t;

typedef enum {
    NUL,
    U08,
    U16,
    U32,
    U64,
    I08,
    I16,
    I32,
    I64,
    F32,
    D64
} log_typ_t;

struct gnu_build_id {
    uint32_t namesz;
    uint32_t descsz;
    uint32_t type;
    char name[4];
    uint8_t desc[20];
};

#define LOG_BUILD_ID() extern const gnu_build_id __log_build_id; \
    __LOG_VAR(VRB, "BUILD_ID", U08, (void *)__log_build_id.desc, 20)

#define LOGM_ERR(msg_) __LOG_MSG(ERR, msg_)
#define LOGM_WRN(msg_) __LOG_MSG(WRN, msg_)
#define LOGM_INF(msg_) __LOG_MSG(INF, msg_)
#define LOGM_DBG(msg_) __LOG_MSG(DBG, msg_)
#define LOGM_VRB(msg_) __LOG_MSG(VRB, msg_)

#define LOGV_ERR(msg_, typ_, ptr_, len_) __LOG_VAR(ERR, msg_, typ_, ptr_, len_)
#define LOGV_WRN(msg_, typ_, ptr_, len_) __LOG_VAR(WRN, msg_, typ_, ptr_, len_)
#define LOGV_INF(msg_, typ_, ptr_, len_) __LOG_VAR(INF, msg_, typ_, ptr_, len_)
#define LOGV_DBG(msg_, typ_, ptr_, len_) __LOG_VAR(DBG, msg_, typ_, ptr_, len_)
#define LOGV_VRB(msg_, typ_, ptr_, len_) __LOG_VAR(VRB, msg_, typ_, ptr_, len_)
 
#define __LOG_MSG(lvl_, msg_)                                                                      \
    __LOG_DEF(__COUNTER__, lvl_, __FILE__, __LINE__, msg_, NUL, 0, 0)

#define __LOG_VAR(lvl_, msg_, typ_, ptr_, len_)                                                    \
    __LOG_DEF(__COUNTER__, lvl_, __FILE__, __LINE__, msg_, typ_, ptr_, len_)

#define __LOG_DEF(log_, lvl_, fil_, lin_, msg_, typ_, ptr_, len_)                                  \
    const static __attribute__((used, aligned(1), section(".logs"))) char __LOG_IDN(log_)[] =      \
    __LOG_FMT(log_, lvl_, fil_, lin_, msg_, typ_, ptr_, len_);                                     \
    log_commit(log_, ptr_, len_)

#define __LOG_FMT(log_, lvl_, fil_, lin_, msg_, typ_, ptr_, len_)                                  \
    "{"                                                                                            \
        "\"log\":" STR(log_) ","                                                                   \
        "\"lvl\":\"" STR(lvl_) "\","                                                               \
        "\"fil\":\"" fil_ "\","                                                                    \
        "\"lin\":" STR(lin_) ","                                                                   \
        "\"msg\":\"" msg_ "\","                                                                    \
        "\"typ\":\"" STR(typ_) "\","                                                               \
        "\"len\":" STR(len_)                                                                       \
    "}\r\n"                                                                                        \

#define __LOG_IDN(n) CAT(log_str_, n)