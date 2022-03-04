#pragma once
#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <string>
#include <stdio.h>
#include<windows.h>
#include<map>
#include "thread_pool.hpp"


typedef unsigned char byte;

typedef struct {
	char processNmae[40];
	char uuid[40];
}UPB;

typedef struct {
	char host[20];
	int port;
	int length;
	int thread;
	UPB upbdata[];
}PBINFO;

typedef struct uuid {
	uint32_t  time_low;
	uint16_t  time_mid;
	uint16_t  time_hi_and_version;
	uint8_t   clock_seq_hi_and_reserved;
	uint8_t   clock_seq_low;
	byte      node[6];
} uuid_t, * uuid_p_t;

#define UUID_SIZE  16

typedef uint16_t p_context_id_t;

typedef struct p_syntax_id {
	uuid_t   if_uuid;
	uint32_t if_version; /* Major/Minor version. */
} p_syntax_id_t;

/* Elements in a presentation context list. */
typedef struct _p_cont_elem {
	p_context_id_t p_cont_id;
	uint8_t       n_transfer_syn;
	uint8_t       reserved;
	p_syntax_id_t abstract_syntax;
	p_syntax_id_t* transfer_syntaxes;
} p_cont_elem_t;

/* Presentation context list. */
typedef struct p_cont_list {
	uint8_t        n_context_elem;
	uint8_t        reserved;
	uint16_t       reserved2;
	p_cont_elem_t* p_cont_elem;
} p_cont_list_t;

/* Common Authentication Verifier. */
typedef struct auth_verifier_co {
	/* Restore 4-byte alignment */
	/* uint8_t auth_pad[]; */
	uint8_t auth_type;
	uint8_t auth_level;
	uint8_t auth_pad_length;
	uint8_t auth_reserved;
	uint8_t auth_context_id;
	uint8_t* auth_value; /* Size is auth_length. */
} auth_verifier_co_t;

/* BIND PDU header */
typedef struct rpcconn_bind_hdr {
	uint8_t  rpc_vers;       /* RPC version. */
	uint8_t  rpc_vers_minor; /* Minor version. */
	uint8_t  ptype;          /* Bind PDU. */
	uint8_t  pfc_flags;      /* PFC Flags. */
	uint8_t  packed_drep[4]; /* NDR data representation format label. */
	uint16_t frag_length;    /* Total length of fragment. */
	uint16_t auth_length;    /* Length of auth_value. */
	uint32_t call_id;        /* Call identifier. */
	uint16_t max_xmit_frag;  /* Max transmit fragment size. */
	uint16_t max_recv_frag;  /* Max receive fragment size. */
	uint32_t assoc_group_id; /* Incarnation of client-server assoc group. */
	p_cont_list_t p_context_elem; /* Presentation context list. */
	auth_verifier_co_t auth_verifier; /* if auth_length != 0. */
} rpcconn_bind_hdr_t;

typedef struct port_any {
	uint16_t length;
	char* port_spec;
} port_any_t;

/* Result of a presentation context negotiation. */
typedef uint16_t p_cont_def_result_t;
/* Reason for rejection of a context element. */
typedef uint16_t p_provider_reason_t;

typedef struct p_result {
	p_cont_def_result_t result;
	p_provider_reason_t reason;
	p_syntax_id_t       transfer_syntax;
} p_result_t;

typedef struct p_result_list {
	uint8_t  n_results;
	uint8_t  reserved;
	uint16_t reserved2;
	p_result_t* p_results;
} p_result_list_t;

/* BIND_ACK PDU header */
typedef struct rpcconn_bind_ack_hdr {
	uint8_t  rpc_vers;
	uint8_t  rpc_vers_minor;
	uint8_t  ptype;
	uint8_t  pfc_flags;
	byte     packed_drep[4];
	uint16_t frag_length;
	uint16_t auth_length;
	uint32_t call_id;
	uint16_t max_xmit_frag;
	uint16_t max_recv_frag;
	uint32_t assoc_group_id; /* Returned assoc_group_id. */
	port_any_t sec_addr;     /* Optional secondary address. */
	/* Restore 8-octet alignment. */
	/* uint8_t  pad2[]; */
	p_result_list_t p_result_list;    /* Variable size. */
	auth_verifier_co_t auth_verifier; /* if auth_length != 0. */
} rpcconn_bind_ack_hdr_t;

/* Presentation context reject */
typedef uint16_t p_reject_reason_t;

typedef struct version {
	uint8_t  major;
	uint8_t  minor;
} version_t;

typedef version_t p_rt_version_t;

typedef struct p_rt_versions_supported {
	uint8_t  n_protocols;
	p_rt_version_t* p_protocols;
} p_rt_versions_supported_t;

/* BIND NAK PDU header */
typedef struct rpcconn_bind_nak_hdr {
	uint8_t  rpc_vers;
	uint8_t  rpc_vers_minor;
	uint8_t  ptype;
	uint8_t  pfc_flags;
	byte     packed_drep[4];
	uint16_t frag_length;
	uint16_t auth_length;
	uint32_t call_id;
	p_reject_reason_t provider_reject_reason;
	p_rt_versions_supported_t* versions; /* if reject reason is 4. */
} rpcconn_bind_nak_hdr_t;

/* Reasons for rejection of an association in the bind nak PDU. */
#define REASON_NOT_SPECIFIED           0
#define TEMPORARY_CONGESTION           1
#define LOCAL_LIMIT_EXCEEDED           2
#define CALLED_PADDR_UNKNOWN           3 /* Not used. */
#define PROTOCOL_VERSION_NOT_SUPPORTED 4
#define DEFAULT_CONTEXT_NOT_SUPPORTED  5 /* Not used. */
#define USER_DATA_NOT_READABLE         6 /* Not used. */ 
#define NO_PSAP_AVAILABLE              7 /* Not used. */

/* REQUEST PDU header */
typedef struct rpcconn_request_hdr {
	uint8_t  rpc_vers;        /* RPC version. */
	uint8_t  rpc_vers_minor;  /* Minor version. */
	uint8_t  ptype;           /* Request PDU. */
	uint8_t  pfc_flags;       /* PFC Flags. */
	uint8_t  packed_drep[4];  /* NDR data representation format label. */
	uint16_t frag_length;     /* Total length of fragment. */
	uint16_t auth_length;     /* Length of auth_value. */
	uint32_t call_id;         /* Call identifier. */
	uint32_t alloc_hint;      /* Allocation hint. */
	p_context_id_t p_cont_id; /* Presentation context. */
	uint16_t opnum;           /* Operation # within the interface! */
	uuid_t   object; /* Only present if the PFC_OBJECT_UUID field is non zero. */
	/* Stub data, 8-octet aligned. */
	auth_verifier_co_t auth_verifier; /* if auth_length != 0 */
} rpcconn_request_hdr_t;

/* RESPONSE PDU header */
typedef struct rpcconn_response {
	uint8_t  rpc_vers;
	uint8_t  rpc_vers_minor;
	uint8_t  ptype;
	uint8_t  pfc_flags;
	uint8_t  packed_drep[4];
	uint16_t frag_length;
	uint16_t auth_length;
	uint32_t call_id;
	uint32_t alloc_hint;
	p_context_id_t p_cont_id;
	uint8_t  cancel_count;
	uint8_t  reserved;
	/* Stub data here, 8-octet aligned. */
	auth_verifier_co_t auth_verifier; /* if auth_length != 0. */
} rpcconn_response_hdr_t;

/* FAULT PDU header */
typedef struct rpcconn_fault_hdr {
	uint8_t  rpc_vers;
	uint8_t  rpc_vers_minor;
	uint8_t  ptype;
	uint8_t  pfc_flags;
	byte     packed_drep[4];
	uint16_t frag_length;
	uint16_t auth_length;
	uint32_t call_id;
	uint32_t alloc_hint;
	p_context_id_t p_cont_id; /* Presentation context. */
	uint8_t  cancel_count;    /* Received cancel count. */
	uint8_t  reserved;
	uint32_t status;          /* Run-time fault code or zero. */
	uint8_t  reserved2[4];
	/* Stub data here, 8-octet aligned. */
	auth_verifier_co_t auth_verifier; /* if auth_length != 0. */
} rpcconn_fault_hdr_t;

/* SHUTDOWN PDU header */
typedef struct rpcconn_shutdown_hdr {
	uint8_t   rpc_vers;
	uint8_t   rpc_vers_minor;
	uint8_t   ptype;
	uint8_t   pfc_flags;
	byte      packed_drep[4];
	uint16_t  frag_length;
	uint16_t  auth_length;
	uint32_t  call_id;
} rpcconn_shutdown_hdr_t;

#define DEFAULT_EPMAP_PORT 135

/* PDU types */
#define RPC_PTYPE_REQUEST        0x00 /* CO/CL */
#define RPC_PTYPE_PING           0x01 /* CL    */
#define RPC_PTYPE_RESPONSE       0x02 /* CO/CL */
#define RPC_PTYPE_FAULT          0x03 /* CO/CL */
#define RPC_PTYPE_WORKING        0x04 /* CL    */ 
#define RPC_PTYPE_NOCALL         0x05 /* CL    */
#define RPC_PTYPE_REJECT         0x06 /* CL    */
#define RPC_PTYPE_ACK            0x07 /* CL    */
#define RPC_PTYPE_CL_CANCEL      0x08 /* CL    */
#define RPC_PTYPE_FACK           0x09 /* CL    */
#define RPC_PTYPE_CANCEL_ACK     0x0a /* CL    */
#define RPC_PTYPE_BIND           0x0b /* CO    */
#define RPC_PTYPE_BIND_ACK       0x0c /* CO    */
#define RPC_PTYPE_BIND_NAK       0x0d /* CO    */
#define RPC_PTYPE_ALTER_CTX      0x0e /* CO    */
#define RPC_PTYPE_ALTER_CTX_RESP 0x0f /* CO    */
#define RPC_PTYPE_SHUTDOWN       0x11 /* CO    */
#define RPC_PTYPE_CO_CANCEL      0x12 /* CO    */
#define RPC_PTYPE_ORPHANED       0x13 /* CO    */

/* PFC FLAGS */
#define PFC_FIRST_FRAG      0x01 /* First fragment. */
#define PFC_LAST_FRAG       0x02 /* Last fragment. */
#define PFC_PENDING_CANCEL  0x04 /* Cancel was pending at sender. */
#define PFC_RESERVED_1      0x08
#define PFC_CONC_MPX        0x10 /* Supports concurrent multiplexing.*/ 
#define PFC_DID_NOT_EXECUTE 0x20 /* Fault packet. */
#define PFC_MAYBE           0x40 /* "Maybe" call semantics requested. */
#define PFC_OBJECT_UUID     0x80 /* A non-nil object UUID is present. */


/* DCE/RPC Endpoint Mapper Protocol. */
/* Reference: pubs.opengroup.org/onlinepubs/009629399/apdxo.htm */

#define EPT_INSERT       0
#define EPT_DELETE       1
#define EPT_LOOKUP       2   /* Lookup entries in an endpoint map. */  
#define EPT_MAP          4
#define EPT_LOOKUP_FREE  5

typedef struct ept_lookup_handle {
	uint32_t attributes;
	uuid_t   uuid;
} ept_lookup_handle_t;

/**********************************
typedef struct ndr_context_handle {
   uint32_t context_handle_attributes;
   uuid_t   context_handle_uuid;
} ndr_context_handle_t;
**********************************/

/* Inquiry types. */
#define RPC_C_EP_ALL_ELTS             0  /* Return all elements from the endpoint map. */
#define RPC_C_EP_MATCH_BY_IF          1
#define RPC_C_EP_MATCH_BY_OBJ         2
#define RPC_C_EP_MATCH_BY_BOTH        3

#define RPC_C_VERS_ALL                1
#define RPC_C_VERS_COMPATIBLE         2
#define RPC_C_VERS_EXACT              3
#define RPC_C_VERS_MAJOR_ONLY         4
#define RPC_C_VERS_UPTO               5
#define RPC_C_ERROR_STRING_LEN      256

#define RPC_C_MGMT_INQ_IF_IDS         0
#define RPC_C_MGMT_INQ_PRINC_NAME     1
#define RPC_C_MGMT_INQ_STATS          2
#define RPC_C_MGMT_IS_SERVER_LISTEN   3
#define RPC_C_MGMT_STOP_SERVER_LISTEN 4

typedef struct ept_lookup {
	uint32_t inquiry_type;
	uint32_t object_referent_id;
	uuid_t   object_uuid;
	uint32_t interface_referent_id;
	uuid_t   interface_uuid;
	uint16_t version_major;
	uint16_t version_minor;
	uint32_t vers_option;
	uuid_t   handle;
	uint32_t max_entries;
} ept_lookup_t;

#define EPT_MAX_ANNOTATION_SIZE 64

typedef struct ept_entry {
	uuid_t object;
	int tower;
	char annotation[EPT_MAX_ANNOTATION_SIZE];
} ept_entry_t, * ept_entry_p_t;


/* Internal "opaque" objects. */
typedef struct buffer {
	void* data;    /* SND and RCV buffers. */
	size_t   bufsize;  /* Allocated bytes. */
	size_t   offset;
	size_t   length;   /* Length of the marshalled packet. */
	int      eof;
	uint32_t index;
} buffer_t;

/* Internal state. */
typedef struct epmap {
	SOCKET sockfd;
	char* server;
	uint16_t port;
	struct sockaddr_in sin;
	buffer_t buffer[2];
	uint32_t call_id;
	uint32_t assoc_group;       /* This is usually ignored. */
	ept_lookup_handle_t handle;
	int state;

	p_reject_reason_t reason;   /* Rejection reason code in the bind_nak PDU. */
	uint32_t status;            /* Run-time fault code or zero (fault PDU). */
	int      wsacode;           /* wsa code */
} epmap_t;

/* Error and status codes. $fixme */

#define EPMAP_EOK       0x000 /* Operation completed successfully. */
#define EPMAP_ENOMEM    0x200 /* A call to malloc() failed. */
#define EPMAP_EINVAL    0x201 /* An invalid argument was passed to a library function. */
#define EPMAP_EBADPTR   0x202 /* An invalid pointer was detected. */
#define EPMAP_EWSAINIT  0x203 /* WSAStartup() initialization failed. */
#define EPMAP_EDNSFAIL  0x204 /* Could not resolve host name. */
#define EPMAP_ESOCKET   0x205 /* Could not create socket or connect to server. */
#define EPMAP_ESEND     0x206 /* A call to send() failed. */
#define EPMAP_ERECV     0x207 /* A call to recv() failed. */

#define EPMAP_EACK      0x300 /* BIND-ACK PDU */
#define EPMAP_ENAK      0x301 /* BIND-NAK PDU */
#define EPMAP_EFAULT    0x302 /* Received a FAULT PDU. */
#define EPMAP_EPROTO    0x303 /* Generic protocol error. */
#define EPMAP_ENODATA   0x304 /* Status returned by response $fixme */

#define EPMAP_EDEBUG    0x400

#define EPMAPAPI extern
int resolve(PBINFO* pbinfo);
