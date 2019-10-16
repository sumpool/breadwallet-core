/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: FreezeService.proto */

#ifndef PROTOBUF_C_FreezeService_2eproto__INCLUDED
#define PROTOBUF_C_FreezeService_2eproto__INCLUDED

#include "protobuf-c.h"

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1003000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003002 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif

#include "TransactionResponse.pb-c.h"
#include "Transaction.pb-c.h"



/* --- enums --- */


/* --- messages --- */

/* --- per-message closures --- */


/* --- services --- */

typedef struct _Proto__FreezeService_Service Proto__FreezeService_Service;
struct _Proto__FreezeService_Service
{
  ProtobufCService base;
  void (*freeze)(Proto__FreezeService_Service *service,
                 const Proto__Transaction *input,
                 Proto__TransactionResponse_Closure closure,
                 void *closure_data);
};
typedef void (*Proto__FreezeService_ServiceDestroy)(Proto__FreezeService_Service *);
void proto__freeze_service__init (Proto__FreezeService_Service *service,
                                  Proto__FreezeService_ServiceDestroy destroy);
#define PROTO__FREEZE_SERVICE__BASE_INIT \
    { &proto__freeze_service__descriptor, protobuf_c_service_invoke_internal, NULL }
#define PROTO__FREEZE_SERVICE__INIT(function_prefix__) \
    { PROTO__FREEZE_SERVICE__BASE_INIT,\
      function_prefix__ ## freeze  }
void proto__freeze_service__freeze(ProtobufCService *service,
                                   const Proto__Transaction *input,
                                   Proto__TransactionResponse_Closure closure,
                                   void *closure_data);

/* --- descriptors --- */

extern const ProtobufCServiceDescriptor proto__freeze_service__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_FreezeService_2eproto__INCLUDED */