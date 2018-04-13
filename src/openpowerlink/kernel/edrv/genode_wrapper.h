#include <oplk/basictypes.h>

#include <stdbool.h>

#include "edrv-struct.h"

#ifdef __cplusplus
extern "C" {
#endif

  	void  get_Mac_Address(UINT8 addr[6]);

  	int init_Session(tEdrvInstance *init);

  	void sendTXBuffer(tEdrvInstance *init, unsigned char* buffer, size_t size);

	void _tx_ack(bool block);

#ifdef __cplusplus
} //end extern "C"
#endif