/*!
** Copyright 2021 NXP
** @file main.c
** @brief
**         Main module.
**         This module contains user's application code.
*/
/*!
**  @addtogroup main_module main module documentation
**  @{
*/
/* MODULE main */


/* Including necessary configuration files. */
#include "Mcal.h"
#include "Clock_Ip.h"
#include "FlexCAN_Ip.h"
#include "IntCtrl_Ip.h"
#include "check_example.h"
#include "Siul2_Port_Ip_Cfg.h"
#include "Siul2_Port_Ip.h"


#define FLEXCAN_INST 0U
#define MSG_ID 20u
#define RX_MB_IDX 1U
#define TX_MB_IDX 0U
volatile int exit_code = 0;
/* User includes */
uint8 dummyData[8] = {1,2,3,4,5,6,7};
/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
extern void CAN0_ORED_0_7_MB_IRQHandler(void);

int main(void)
{
    /* Write your code here */
	Siul2_Port_Ip_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    Clock_Ip_Init(&Mcu_aClockConfigPB[0]);

    IntCtrl_Ip_EnableIrq(CAN0_ORED_0_7_MB_IRQn);
    IntCtrl_Ip_InstallHandler(CAN0_ORED_0_7_MB_IRQn, CAN0_ORED_0_7_MB_IRQHandler, NULL_PTR);

    Flexcan_Ip_DataInfoType rx_info = {
            .msg_id_type = FLEXCAN_MSG_ID_STD,
            .data_length = 8u,
            .is_polling = TRUE,
            .is_remote = FALSE
    };
    Flexcan_Ip_MsgBuffType rxData;
    FlexCAN_Ip_Init(FLEXCAN_INST, &FlexCAN_State0, &FlexCAN_Config0);
    FlexCAN_Ip_SetStartMode(FLEXCAN_INST);
    FlexCAN_Ip_ConfigRxMb(FLEXCAN_INST, RX_MB_IDX, &rx_info, MSG_ID);
    rx_info.is_polling = FALSE;
    FlexCAN_Ip_Send(FLEXCAN_INST, TX_MB_IDX, &rx_info, MSG_ID, (uint8 *)&dummyData);
    FlexCAN_Ip_Receive(FLEXCAN_INST, RX_MB_IDX, &rxData, TRUE);
    while(1)
    {
    	while(FlexCAN_Ip_GetTransferStatus(FLEXCAN_INST, RX_MB_IDX) != FLEXCAN_STATUS_SUCCESS)
    	    { FlexCAN_Ip_MainFunctionRead(FLEXCAN_INST, RX_MB_IDX);
    	    FlexCAN_Ip_Send(FLEXCAN_INST, TX_MB_IDX, &rx_info, MSG_ID, (uint8 *)&dummyData);}
    	dummyData[0]++;
    	FlexCAN_Ip_Send(FLEXCAN_INST, TX_MB_IDX, &rx_info, MSG_ID, (uint8 *)&dummyData);
    	FlexCAN_Ip_Receive(FLEXCAN_INST, RX_MB_IDX, &rxData, TRUE);
    }

    FlexCAN_Ip_SetStopMode(FLEXCAN_INST);
    FlexCAN_Ip_Deinit(FLEXCAN_INST);
    Exit_Example(TRUE);
}

/* END main */
/*!
** @}
*/
