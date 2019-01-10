/******************************************************************************

  Copyright (C) 2015 Winner Micro electronics Co., Ltd.

 ******************************************************************************
  File Name     : alg.h
  Version       : Initial Draft
  Author        : Li Limin, lilm@winnermicro.com
  Created       : 2015/3/7
  Last Modified :
  Description   : Application layer gateway, (alg) only for apsta

  History       :
  1.Date        : 2015/3/7
    Author      : Li Limin, lilm@winnermicro.com
    Modification: Created file

******************************************************************************/
#ifndef __ALG_H__
#define __ALG_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


/* ============================== configure ===================== */
/* napt�����ϻ�ʱ��, ��λ�룬��ʱɾ���ü�¼ */
#define NAPT_TABLE_TIMEOUT           60

/* napt�˿ڳط�Χ��Ĭ�Ϸ�ΧΪ15000~19999 */
#define NAPT_LOCAL_PORT_RANGE_START  0x3A98
#define NAPT_LOCAL_PORT_RANGE_END    0x4E1F

/* napt icmp id range: 3000-65535 */
#define NAPT_ICMP_ID_RANGE_START     0xBB8
#define NAPT_ICMP_ID_RANGE_END       0xFFFF


/* napt table size */
#define NAPT_TABLE_LIMIT
#ifdef  NAPT_TABLE_LIMIT
#define NAPT_TABLE_SIZE_MAX          1000
#endif
/* ============================================================ */


#define NAPT_TMR_INTERVAL            ((NAPT_TABLE_TIMEOUT / 2) * 1000UL)

#define NAPT_TMR_TYPE_TCP            0x0
#define NAPT_TMR_TYPE_UDP            0x1
#define NAPT_TMR_TYPE_ICMP           0x2
#define NAPT_TMR_TYPE_GRE            0x3

extern bool alg_napt_port_is_used(u16 port);

extern void alg_napt_event_handle(u32 type);

extern int alg_napt_init(void);

extern int alg_input(const u8 *bssid, u8 *pkt_body, u32 pkt_len);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __ALG_H__ */
