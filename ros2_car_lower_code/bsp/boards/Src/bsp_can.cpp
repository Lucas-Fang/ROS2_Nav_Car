#include "bsp_can.h"



void CAN_BUS::BSP::CAN_Start(CAN_HandleTypeDef *hcan)
{
    if(HAL_CAN_Start(hcan) != HAL_OK)
    {
        Error_Handler();
    }
    if(HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
    {
        Error_Handler();
    }
}


void CAN_BUS::BSP::Filter_Init(CAN_HandleTypeDef *hcan)
{
    // if(hcan->Instance == CAN1)
    // {
        CAN_FilterTypeDef CAN_Filter_st;

        CAN_Filter_st.FilterActivation = ENABLE;
        CAN_Filter_st.FilterMode = CAN_FILTERMODE_IDMASK;
        CAN_Filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
        CAN_Filter_st.FilterIdHigh = 0x0000;
        CAN_Filter_st.FilterIdLow = 0x0000;
        CAN_Filter_st.FilterMaskIdHigh = 0x0000;
        CAN_Filter_st.FilterMaskIdLow = 0x0000;
        CAN_Filter_st.FilterBank = 0;
        CAN_Filter_st.FilterFIFOAssignment = CAN_FILTER_FIFO0;

        CAN_Filter_st.SlaveStartFilterBank=14;

        if(HAL_CAN_ConfigFilter(hcan, &CAN_Filter_st)!=HAL_OK)       //滤波器初始化
        {
            Error_Handler();
        }

    // }
}