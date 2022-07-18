/**
  ******************************************************************************
  * @file    bsp_can.c
  * @author  gxf
  * @version V1.0.0
  * @date    2021.11.16
  * @brief   小远机器人大巡逻车CAN的底层文件
 ==============================================================================
  */
 
#include "bsp_can.h"


/**
 * [bsp_can1_init description]
 * @author Gxf
 * @dateTime 2021-11-16
 * @brief    [description] CAN1 初始化
 * @note     [description]
 * @param    baudrate      [description] CAN的速率 1000、500、250、125、100、50、20
 */
void bsp_can1_init(u16 baudrate)
{

  GPIO_InitTypeDef GPIO_InitStructure; 
  /* GPIOB */     
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);                                                                         
  /* CAN1  */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); 
  /* Configure CAN pin: RX PB8 */    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;     
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  
  /* Configure CAN pin: TX PB9 */  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE); 


  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
  /* CAN register init */
  CAN_DeInit(CAN1);
  CAN_StructInit(&CAN_InitStructure);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM=DISABLE;//时间戳是否使能
  CAN_InitStructure.CAN_ABOM=DISABLE;//自动离线管理
  CAN_InitStructure.CAN_AWUM=DISABLE;//自动唤醒模式
  CAN_InitStructure.CAN_NART=DISABLE;//错误恢复的处理方式：0报文自动重传，1报文只传一次
  CAN_InitStructure.CAN_RFLM=DISABLE;//接收报文FIFO溢出的处理模式：0溢出时新报文会覆盖原有报文，1溢出时，新报文丢弃
  CAN_InitStructure.CAN_TXFP=DISABLE;//发送报文FIFO的优先级:0优先级取决于报文标示符，1优先级取决于发送请求顺序
  CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;//
  CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;//同步补偿宽度1个时间单元
  CAN_InitStructure.CAN_BS1=CAN_BS1_2tq;//时间段1占用2个时间单元
  CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;//时间段2占用3个时间单元
  if(baudrate==1000)
    /*36/(1+2+3)/6=1Mbps*/
  CAN_InitStructure.CAN_Prescaler=6;
  else  if(baudrate==500)
  CAN_InitStructure.CAN_Prescaler=12;
  else  if(baudrate==250)
  CAN_InitStructure.CAN_Prescaler=24;
  else  if(baudrate==125)
  CAN_InitStructure.CAN_Prescaler=48;
  else  if(baudrate==100)
  CAN_InitStructure.CAN_Prescaler=60;
  else  if(baudrate==50)
  CAN_InitStructure.CAN_Prescaler=120;
  else  if(baudrate==20)
  CAN_InitStructure.CAN_Prescaler=300;  
  CAN_Init(CAN1,&CAN_InitStructure);

  /* CAN filter init */
  CAN_FilterInitStructure.CAN_FilterNumber=0;//指定初始化过滤器的编号
  /*FM1R过滤器组0的工作模式
  0：过滤器组x的2个32位寄存器工作在标示符屏蔽位模式
  1：过滤器组x的2个32位寄存器工作在标示符列表模式*/
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;//标示符屏蔽位模式
  /*FS1R过滤器组0的位宽
  0：过滤器位宽为2个16位，1：过滤器位宽为单个32位*/
  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//
  CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;//所有位都不关心
  CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;//所有位都不关心
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;//
  /*报文在通过了某过滤器的过滤后，将被存放到关联的FIFO中*/
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=0;//过滤器被关联到FIFO0
  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;//
  CAN_FilterInit(&CAN_FilterInitStructure);//

CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0消息挂号中断允许.
}



void USB_LP_CAN1_RX0_IRQHandler(void)
{
  CanRxMsg RxMessage;
  CAN_Receive(CAN1, 0, &RxMessage);

}

/**
 * [can1_send_msg description]
 * @author Gxf
 * @dateTime 2021-11-16
 * @brief    [description] CAN发送函数
 * @note     [description]
 * @param    msg           [description] 待发送的数据地址
 * @param    len           [description] 待发送的数据长度
 * @return                 [description]
 */
u8 can1_send_msg(u8* msg,u8 len)
{ 
  u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId=0x141;             // 标准标识符为0
  TxMessage.ExtId=0x000;             // 设置扩展标示符（29位）
  TxMessage.IDE=0;                   // 标准帧
  TxMessage.RTR=0;                   // 消息类型为数据帧，一帧8位
  TxMessage.DLC=len;                 // 发送两帧信息
  for(i=0;i<8;i++)
  TxMessage.Data[i]=msg[i];          // 第一帧信息          
  mbox= CAN_Transmit(CAN1, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++; //等待发送结束
  if(i>=0XFFF)
    return 1;
  return 0;   

}