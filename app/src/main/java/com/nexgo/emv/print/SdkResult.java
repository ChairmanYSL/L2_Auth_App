package com.nexgo.emv.print;

public class SdkResult {
	public final static int Success = 0;
	public final static int Fail = -1;
	public final static int Param_In_Invalid = -2;
	public final static int TimeOut = -3;
	/* 设备未登入 */
	public final static int Device_Not_Ready = -4;
	
	public final static int LOGIN_SUCCESS = 0;
	public final static int LOGIN_FAIL = 1;
	public final static int LOGIN_SUCCESS_NOT_EMV_FILE = 2;
	
	//---- Printer Error -----
	public final static int Printer_Base_Error = -1000;
	/** 打印失败*/
	public final static int Printer_Print_Fail = Printer_Base_Error -1;
	/** 设置字符串缓冲失败*/
	public final static int Printer_AddPrnStr_Fail = Printer_Base_Error -2;
	/** 设置图片缓冲失败*/
	public final static int Printer_AddImg_Fail = Printer_Base_Error -3;
	/**打印机忙*/
	public final static int Printer_Busy 			= Printer_Base_Error - 4;	
	/**打印机缺纸*/
	public final static int Printer_PaperLack 		= Printer_Base_Error - 5;	
	/**打印数据包格式错*/
	public final static int Printer_Wrong_Package 	= Printer_Base_Error - 6;	
	/**打印机故障*/
	public final static int Printer_Fault 			= Printer_Base_Error - 7;	
	/**打印机过热*/
	public final static int Printre_TooHot 			= Printer_Base_Error - 8;	
	/**打印未完成*/
	public final static int Printer_UnFinished 		= Printer_Base_Error - 9;	
	/**打印机未装字库*/
	public final static int Printer_NoFontLib 		= Printer_Base_Error - 10;	
	/**数据包过长*/
	public final static int Printer_OutOfMemory 	= Printer_Base_Error - 11;
	/**其他异常错误*/
	public final static int Printer_Other_Error = Printer_Base_Error-999;
	
	//---- Scanner Error -----
	public final static int Scanner_Base_Error = -2000;
	/**用户按键退出*/
	public final static int Scanner_Customer_Exit = Scanner_Base_Error-1;
	/**其他异常错误*/
	public final static int Scanner_Other_Error = Scanner_Base_Error-999;
	
	//---- Modem Error -----
	public final static int Modem_Base_Error = -3000;
	/**拨号失败*/
	public final static int Modem_Dial_Fail = Modem_Base_Error-1;
	/**Modem没初始化*/
	public final static int Modem_Not_Init = Modem_Base_Error-2;
	/**发送失败*/
	public final static int Modem_Send_Fail = Modem_Base_Error-3;
	/**接收失败*/
	public final static int Modem_Recv_Fail = Modem_Base_Error-4;
	/**发送缓冲区满*/
	public final static int Modem_Send_Buf_OverFlow = Modem_Base_Error - 5;           
	/**旁置电话占用*/
	public final static int Modem_Telephone_Occupied = Modem_Base_Error - 6;      
	/** 电话线未接好或并线电话占用，线电压不为0，但过低*/
	public final static int Modem_Not_Properly_Connected = Modem_Base_Error - 7;  
	/** 电话线未接，线电压为0*/
	public final static int Modem_Unconnected = Modem_Base_Error - 8;            
	/** 旁置电话、并线电话均空闲（仅用于发号转人工接听方式）*/
	public final static int Modem_Bypass = Modem_Base_Error - 9;                 
	/**线路载波丢失（同步建链失败）*/
	public final static int Modem_Carrier_Wave_Lost = Modem_Base_Error - 10;       
	/**拨号无应答*/
	public final static int Modem_No_Response = Modem_Base_Error - 11;             
	/**已开始发号（仅用于发号转人工接听方式）*/
	public final static int Modem_Have_Start_Dialing = Modem_Base_Error - 12;      
	/**正在拨号*/
	public final static int Modem_Dialing = Modem_Base_Error - 13;                  
	/**正在挂机和空闲*/
	public final static int Modem_Hangup_And_Free = Modem_Base_Error - 14;       
	/**接听数据请求被拒绝（接收缓冲区为空）*/
	public final static int Modem_Data_Request_Declined = Modem_Base_Error - 15;  
	/**被叫线路忙.*/
	public final static int Modem_Line_Busy = Modem_Base_Error - 16;              
	/**（主CPU）已无可用的通讯口（两个动态分配端口正全被其它通讯口使用）*/
	public final static int Modem_No_Port = Modem_Base_Error - 17;                 
	/**CANCEL键按下（modem将终止拨号操作）*/
	public final static int Modem_Cancel_Key_Pressed = Modem_Base_Error - 18;	  
	/**无效的数据长度（len=0 或 len>2048）不会发送数据*/ 
	public final static int Modem_Invalid_Data_Length = Modem_Base_Error - 19;    
	/**其他异常错误*/
    public final static int Modem_Other_Error = Modem_Base_Error - 999;           

	//---- SerialPort Error -----
	public final static int SerialPort_Base_Error = -4000;
	/**串口连接失败*/
	public final static int SerialPort_Connect_Fail = SerialPort_Base_Error - 1;
	/**串口数据发送失败*/
	public final static int SerialPort_Send_Fail = SerialPort_Base_Error - 2;
	/**Fd错误*/
	public final static int SerialPort_Fd_Error = SerialPort_Base_Error - 3;
	/**串口未打开*/
	public final static int SerialPort_Port_Not_Open = SerialPort_Base_Error - 4;
	/**串口断链失败*/
	public final static int SerialPort_DisConnect_Fail = SerialPort_Base_Error - 5;
	/**发送缓冲区未空（尚余数据待发送）*/
	public final static int SerialPort_Sending_Buf_IsNot_Null = SerialPort_Base_Error - 6;           
	/** 非法通道号*/
	public final static int SerialPort_Invalid_Channel = SerialPort_Base_Error - 7;           
	/**通道未打开且未与任何物理端口连通*/
	public final static int SerialPort_Channel_Isnot_Open = SerialPort_Base_Error - 8;               
	/**发送缓冲区错误（持续500ms为满状态）*/
	public final static int SerialPort_Sending_Buffer_Error = SerialPort_Base_Error - 9;             
	/** 无可用的物理端口*/
	public final static int SerialPort_No_Available_Ports = SerialPort_Base_Error - 10;               
	/**设备未完成枚举和配置过程（USB DEV专用*/
	public final static int SerialPort_Conf_Process_Error = SerialPort_Base_Error - 11;              
	/**设备已断电和与主机失去连接（USB DEV专用*/
	public final static int SerialPort_Device_Lost_Power = SerialPort_Base_Error - 12;               
	 /**设备从主机拨除后又插上（USB DEV专用）*/ 
	public final static int SerialPort_Unplug_Error = SerialPort_Base_Error - 13;                   
	/**设备已关闭（USBDEV专用）*/
	public final static int SerialPort_Device_Is_Off = SerialPort_Base_Error - 14;                   
	/**数据接收超时*/
	public final static int SerialPort_Timeout_Receiving_Data = SerialPort_Base_Error - 15;          
	/*** 通道正在被系统占用*/
	public final static int SerialPort_Channle_Is_Occupied = SerialPort_Base_Error - 16;             
	/**无效的通讯参数，通讯参数不符合字符串规则或数据超出正常范围。*/
	public final static int SerialPort_Invalid_Communication_Parameter = SerialPort_Base_Error - 17; 
	 /**USB转串口设备挂载不成功（仅FIDI USB转串口使用的返回值）*/
	public final static int SerialPort_Usb_Mounted_Unsuccessful = SerialPort_Base_Error - 18;       
	 /**usb转串口设备出错（仅FTDI USB转口串口使用的返回值*/
	public final static int SerialPort_Reset_Usb_Error = SerialPort_Base_Error - 19;                
	/**设备USB转串口芯片通讯阻塞（仅FIDI USB转接串口使用的返回值）*/
	public final static int SerialPort_Devices_Error = SerialPort_Base_Error - 20;                  
	/**其他异常错误*/
	public final static int SerialPort_Other_Error = SerialPort_Base_Error-999;
	
	//---- MagCardReader Error -----
	public final static int MagCardReader_Base_Error = -5000;
	/** 无刷卡*/
	public final static int MagCardReader_No_Swiped = MagCardReader_Base_Error -1;
	/** 其他异常错误*/
	public final static int MagCardReader_Other_Error = MagCardReader_Base_Error -999;
	
	//---- IccCardReader Error -----
	public final static int IccCardReader_Base_Error = -6000;
	/**其他异常错误*/
	public final static int IccCardReader_Other_Error = IccCardReader_Base_Error-999;
	
	//---- PinPad Error ----
	public final static int PinPad_Base_Error = -7000;
	/** 密钥不存在*/
	public final static int PinPad_No_Key_Error = PinPad_Base_Error - 1;                
	/** 密钥索引错，参数索引不在范围内*/
	public final static int PinPad_KeyIdx_Error = PinPad_Base_Error - 2;            
	/** 密钥写入时，源密钥的层次比目的密钥低*/
	public final static int PinPad_Derive_Error = PinPad_Base_Error - 3;           
	/**密钥验证失败*/
	public final static int PinPad_Check_Key_Fail = PinPad_Base_Error - 4;        
	/** 没输入PIN*/
	public final static int PinPad_No_Pin_Input = PinPad_Base_Error - 5;          
	/** 取消输入PIN*/
	public final static int PinPad_Input_Cancel = PinPad_Base_Error - 6;         
	/** 函数调用小于最小间隔时间*/
	public final static int PinPad_Wait_Interval = PinPad_Base_Error - 7;        
	/** KCV模式错，不支持*/
	public final static int PinPad_Check_Mode_Error = PinPad_Base_Error - 8;        
	/** 无权使用该密钥，当出现密钥标签不对，或者写入密钥时，源密钥类型的值大于目的密钥类型，都会返回该密钥*/
	public final static int PinPad_No_Right_Use = PinPad_Base_Error - 9;          
	/** 密钥类型错*/
	public final static int PinPad_Key_Type_Error = PinPad_Base_Error - 10;         
	/** 期望PIN的长度字符串错*/
	public final static int PinPad_ExpLen_Error = PinPad_Base_Error - 11;          
	/** 目的密钥索引错，不在范围内*/
	public final static int PinPad_Dstkey_Idx_Error = PinPad_Base_Error - 12;       
	/** 源密钥索引错，不在范围内*/
	public final static int PinPad_SrcKey_Idx_Error = PinPad_Base_Error - 13;       
	/** 密钥长度错*/
	public final static int PinPad_Key_Len_Error = PinPad_Base_Error - 14;          
	/** 输入PIN超时*/
	public final static int PinPad_Input_Timeout = PinPad_Base_Error - 15;        
	/** IC卡不存在*/
	public final static int PinPad_No_Icc = PinPad_Base_Error - 16;               
	/** IC卡未初始化*/
	public final static int PinPad_Icc_No_Init = PinPad_Base_Error - 17;          
	/** DUKPT组索引号错*/
	public final static int PinPad_Group_Idx_Error = PinPad_Base_Error - 18;        
	/** 指针参数非法为空*/
	public final static int PinPad_Param_Ptr_Null = PinPad_Base_Error - 19;       
	/** PED已锁*/
	public final static int PinPad_Locked = PinPad_Base_Error - 20;               
	/** PED通用错误*/
	public final static int PinPad_Ret_Error = PinPad_Base_Error - 21;                    
	/** 没有空闲的缓冲*/
	public final static int PinPad_Nomore_Buf = PinPad_Base_Error - 22;          
	/** 需要取得高级权限*/
	public final static int PinPad_Need_Admin = PinPad_Base_Error - 23;           
	/** DUKPT已经溢出*/
	public final static int PinPad_Dukpt_Overflow = PinPad_Base_Error - 24;       
	/** KCV校验失败*/
	public final static int PinPad_Kcv_Check_Fail = PinPad_Base_Error - 25;      
	/** 源密钥类型错*/
	public final static int PinPad_SrcKey_Type_Error = PinPad_Base_Error - 26;      
	/** 命令不支持*/
	public final static int PinPad_Unsupport_Cmd = PinPad_Base_Error - 27;            
	/** 通讯错误*/
	public final static int PinPad_Comm_Error = PinPad_Base_Error - 28;            
	/** 没有用户认证公钥*/
	public final static int PinPad_No_Uapuk = PinPad_Base_Error - 29;           
	/** 取系统敏感服务失败*/
	public final static int PinPad_Admin_Error = PinPad_Base_Error - 30;            
	/** PED处于下载非激活状态*/
	public final static int PinPad_Download_Disactive = PinPad_Base_Error - 31;     
	/** KCV 奇校验失败*/
	public final static int PinPad_Kcv_Odd_Check_Fail = PinPad_Base_Error - 32;    
	/** 读取PED数据失败*/
	public final static int PinPad_Ped_Data_Rw_Fail = PinPad_Base_Error - 33;      
	/** 卡操作错误(脱机明文、密文密码验证)*/
	public final static int PinPad_Icc_Cmd_Error = PinPad_Base_Error - 34;          
	/** 用户按CLEAR键退出输入PIN*/
	public final static int PinPad_Input_Clear = PinPad_Base_Error - 35;           
	/** PED存储空间不足*/
	public final static int PinPad_No_Free_Flash = PinPad_Base_Error - 36;        
	/** DUKPT KSN需要先加1*/
	public final static int PinPad_Dukpt_Need_Inc_Ksn = PinPad_Base_Error - 37;   
	/** KCVMODE错误*/
	public final static int PinPad_Kcv_Mode_Error = PinPad_Base_Error - 38;          
	/** NO KCV*/
	public final static int PinPad_Dukpt_No_Kcv = PinPad_Base_Error - 39;          
	/** 按FN/ATM4键取消PIN输入*/
	public final static int PinPad_Pin_Bypass_ByFunKey = PinPad_Base_Error - 40;   
	/** 数据MAC校验错*/
	public final static int PinPad_Mac_Error = PinPad_Base_Error - 41;               
	/** 数据CRC校验错*/
	public final static int PinPad_Crc_Error = PinPad_Base_Error - 42;  
	/** 密码键盘类型错 */
	public final static int PinPad_Type_Error = PinPad_Base_Error - 43;  
	/** 其他异常错误*/
	public final static int PinPad_Other_Error = PinPad_Base_Error-999;
	
	//---- EmvHandler Error ----
	public final static int EmvHandler_Base_Error = -8000;
	/**<请尝试其他通信界面*/
	public final static int Emv_Other_Interface = EmvHandler_Base_Error - 1;         
	/**<非接触QPBOC交易脱机接受*/
	public final static int Emv_Qpboc_Offline = EmvHandler_Base_Error - 2;         
	/**<非接触QPBOC交易联机*/
	public final static int Emv_Qpboc_Online = EmvHandler_Base_Error - 3;         
	 /**<非接触PBOC交易联机*/
	public final static int Emv_Pboc_Online = EmvHandler_Base_Error - 4;        
	/**<非接触MSD交易联机*/
	public final static int Emv_MSD_Online = EmvHandler_Base_Error - 5;         
	/**<电子现金脱机接受*/
	public final static int Emv_Ec_Accept = EmvHandler_Base_Error - 6;         
	/**<标准流程脱机接受*/
	public final static int Emv_Offline_Accept = EmvHandler_Base_Error - 7;         
	/**<交易中卡片被移开*/
	public final static int Emv_Card_Removed = EmvHandler_Base_Error -8;	
	/**<读卡失败*/
	public final static int Emv_Command_Fail = EmvHandler_Base_Error -9;  	
	/**<卡片已锁*/
	public final static int Emv_Card_Block = EmvHandler_Base_Error -10;   
	/**<参数错*/
	public final static int Emv_PARA_ERR = EmvHandler_Base_Error -11; 
	/**<无共同应用*/
	public final static int Emv_Candidatelist_Empty = EmvHandler_Base_Error -12;	
	/**<应用已锁*/
	public final static int Emv_App_Block = EmvHandler_Base_Error -13;
	/**<交易fallback */
	public final static int Emv_FallBack = EmvHandler_Base_Error -14;	
	/**<数据认证失败*/
	public final static int Emv_Auth_Fail = EmvHandler_Base_Error -15;   
	/**<应用尚未生效*/
	public final static int Emv_App_Ineffect = EmvHandler_Base_Error -16;  
	/**<应用已失效*/
	public final static int Emv_App_Expired = EmvHandler_Base_Error -17; 
	/**<持卡人验证失败*/
	public final static int Emv_Cvm_Fail = EmvHandler_Base_Error -18;   
	/**<交易应联机*/
	public final static int Emv_Online = EmvHandler_Base_Error -19;	
	/**<交易取消*/
	public final static int Emv_Cancel = EmvHandler_Base_Error -20;	
	/**<交易拒绝*/
	public final static int Emv_Declined = EmvHandler_Base_Error -21;
	/**<发卡行认证失败*/
	public final static int Emv_Arpc_Fail = EmvHandler_Base_Error -22;	
	/**<发卡行脚本执行失败*/
	public final static int Emv_Script_Fail = EmvHandler_Base_Error -23;	
	/**<应用不接受，可重新选择*/
	public final static int Emv_App_NoAccept = EmvHandler_Base_Error -24;  
	/**<电子现金脱机拒绝*/
	public final static int Emv_Ec_Decliend = EmvHandler_Base_Error -25;  
	/**其他错误异常*/
	public final static int Emv_Other_Error = EmvHandler_Base_Error -999;

	//------Network Error -----------
    //---Eth部分----
    public final static int NetWorkHandler_Base_Error = -9000;
    /**试图建立连接失败 */
    public final static int NetWorkHandler_Eth_Establish_Conn_Failed = NetWorkHandler_Base_Error - 1;
    /**连接已关闭 */
    public final static int NetWorkHandler_Eth_Conn_Closed = NetWorkHandler_Base_Error - 2;
   
    //--Wifi部分----
    /** 连接wifi热点失败 */
    public final static int NetWorkHandler_Wifi_Conn_Failed = NetWorkHandler_Base_Error - 3;
   
    //--移动数据部分----
    /** 添加新的APN失败 */
    public final static int NetWorkHandler_Mobile_Add_New_Apn_Failed = NetWorkHandler_Base_Error - 4;
    /** 连接指定APN失败 */
    public final static int NetWorkHandler_Mobile_Conn_Apn_Failed = NetWorkHandler_Base_Error - 5;
    /** 获取移动网络信息失败 */
    public final static int NetWorkHandler_Mobile_Info_Failed = NetWorkHandler_Base_Error - 6;
    /**ping测试失败, 无法到达serverIp*/
    public final static int NetWorkHandler_Cannot_Reach_serverIp = NetWorkHandler_Base_Error - 7;
    /**其它错误异常 */
    public final static int NetWorkHandler_Other_Error = NetWorkHandler_Base_Error - 999;
	
	//---- CardHandler Error -----
	//---- -10000 ~ -19900 划给卡片操作用
	//---- -10100 代表接触式CPU卡  -10200 - 非接触式CPU卡 共计支持99种卡片类型
	public final static int CardHandler_Base_Error = -10000;
	//---- 接触式IC卡返回码段
	public final static int Icc_Base_Error = CardHandler_Base_Error -100;
	/**交易中卡被拨出*/
	public final static int Icc_PullOut_Card = Icc_Base_Error - 1;
	/**奇偶错误*/
	public final static int Icc_Parity_Err = Icc_Base_Error - 2;	
	/**选择通道错误*/
	public final static int Icc_Channel_Err 	= Icc_Base_Error - 3;	
	/**发送数据太长(LC)*/
	public final static int Icc_Data_Len_TooLong 	= Icc_Base_Error - 4;	
	/**卡片协议错误(不为T＝0或T＝1)*/
	public final static int Icc_Protocol_Err 	= Icc_Base_Error - 5;	
	/**没有复位卡片*/
	public final static int Icc_No_Reset_Card 	= Icc_Base_Error - 6;	
	/**不能通信或没上电*/
	public final static int Icc_Not_Call 		= Icc_Base_Error - 7;
	/**其他异常错误*/
	public final static int Icc_Other_Error 		= Icc_Base_Error - 99;
	//---- 非接触式IC卡返回码段
	public final static int Picc_Base_Error = CardHandler_Base_Error -200;
	/**射频模块未开启*/
	public final static int Picc_Not_Open 			= Picc_Base_Error - 1; 		
	/**未搜寻到卡片(感应区内无指定类型的卡片) */
	public final static int Picc_Not_Searched_Card 	= Picc_Base_Error - 2;		
	/**感应区内卡片过多(出现通讯冲突)*/
	public final static int Picc_Card_Too_Many 		= Picc_Base_Error - 3; 		
	/**协议错误(卡片应答中出现违反协议规定的数据) */
	public final static int Picc_Protocol_Data_Err 		= Picc_Base_Error - 4;		
	/**卡片未激活*/
	public final static int Picc_Card_No_Activation = Picc_Base_Error - 5; 		
	/**多卡冲突 */
	public final static int Picc_Muti_Card_Err 		= Picc_Base_Error - 6;		
	/**协议错误 */
	public final static int Picc_Protocol_Err		= Picc_Base_Error - 7;		
	/**通信传输错误*/
	public final static int Picc_Io_Err 			= Picc_Base_Error - 8; 		
	/**卡片仍在感应区内*/
	public final static int Picc_Card_Sense_Err 	= Picc_Base_Error - 9; 	
	/**卡片状态错误(如A/B卡调用M1卡接口, 或M1卡调用PiccIsoCommand接口) */
	public final static int Picc_Card_Status_Err 	= Picc_Base_Error - 10;		
	/**接口芯片不存在或异常*/
	public final static int Picc_Not_Call 			= Picc_Base_Error - 11; 
	/**其他错误异常*/
	public final static int Picc_Other_Error 			= Picc_Base_Error - 99; 

	/**M1卡部分*/
	public final static int M1Card_Base_Error = CardHandler_Base_Error -300;
	/**M1卡认证失败 */
	public final static int M1Card_Verify_Err = M1Card_Base_Error - 1;		
	/**扇区未认证*/
	public final static int M1Card_Fan_Not_Verify 	= M1Card_Base_Error - 2; 	
	/**数值块数据格式有误 */
	public final static int M1Card_Data_Block_Err 	= M1Card_Base_Error - 3;		
	/**模块未开启*/
	public final static int M1Card_Not_Open 			= M1Card_Base_Error - 4;	
	/**卡片未激活 */
	public final static int M1Card_Card_Not_Activation 	= M1Card_Base_Error - 5;
	/**	卡片操作类型错  用于operateBlock入参operType检查 */
	public final static int M1Card_Card_OperType_Error	= M1Card_Base_Error - 6;
	/**其他错误异常 */
	public final static int M1Card_Other_Error 	= M1Card_Base_Error - 99;
	
	//---- CashBoxHandler Error -----
	public final static int CashBox_Base_Error = -20000;
	public final static int CashBox_Device_NOEXIST = CashBox_Base_Error - 1;
	public final static int CashBox_Device_OPENED = CashBox_Base_Error - 2;
	public final static int CashBox_Other_Error = CashBox_Base_Error -999;
}
