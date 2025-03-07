/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      app_common_flags.h
   * @brief     This file is used to config app functions.
   * @author    jane
   * @date      2017-06-06
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */
#ifndef _APP_COMMON_FLAGS_H_
#define _APP_COMMON_FLAGS_H_

/** @brief  Config local address type: 0-pulic address, 1-static random address, 2-random resolvable private address */
#define F_BT_LE_USE_RANDOM_ADDR      1

/** @brief  Config device name characteristic and appearance characteristic property: 0-Not writeable, 1-writeable, save to flash*/
#define F_BT_GAPS_CHAR_WRITEABLE            0

#endif
