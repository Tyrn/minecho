/*
 ******************************************************************************
 * @file           : circus.h
 * @brief          : A circular buffer servicing the VCP
 ******************************************************************************
 */
#ifndef _CIRCUS_H
#define _CIRCUS_H

int8_t c_circus_cdc_init_fs();
int8_t c_circus_cdc_receive_fs(uint8_t *Buf, uint32_t *Len);

int l_circus_vcp_send_available(void);
int l_circus_vcp_send(uint8_t *buf, uint16_t len);

int l_circus_vcp_recv_available(void);
int l_circus_vcp_recv(uint8_t *buf, uint16_t len);

void v_circus_vcp_service();

//#define NO_MIN
#ifndef NO_MIN
  #include "min.h"
  extern struct min_context min_ctx;
#endif

#endif /* _CIRCUS_H */
