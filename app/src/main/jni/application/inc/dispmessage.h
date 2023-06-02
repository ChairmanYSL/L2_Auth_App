#ifndef AFX_DISPMESSAGE_H
#define AFX_DISPMESSAGE_H

#define DISP_OK "Success"
#define DISP_ERR "Fail"
#define DISP_EXITERR	"Press any Key exit"

//menu
#define DISP_APPSTARTUP "Starting up"

#define DISP_MENU1LINE1_EN			" EXPRESSPAY L2 TEST  "
#define DISP_MENU1LINE2_EN			"1.SALE   2.OTHERTRANS"
#define DISP_MENU1LINE3_EN			"3.CHECKSUM 4.AID     "
#define DISP_MENU1LINE4_EN			"5.Loop               "

#define DISP_SETMENU1LINE1_EN		" SET PARAMETERS MENU1"
#define DISP_SETMENU1LINE2_EN		"1.DATE     2.TERMINAL"
#define DISP_SETMENU1LINE3_EN		"3.SETTLE   4.PRINT   "
#define DISP_SETMENU1LINE4_EN		"5.BATCH MODE         "
#define DISP_SETMENU1LINE5_EN		"6.SYS INIT 7.IF PRINT"



#define DISP_SALEMENU	"1.消费"
#define DISP_SETMENU1 "参数设置1"
#define DISP_SETMENU1LINE1 "1.时间设置 2.内核版本"
#define DISP_SETMENU1LINE2 "3.批上送   4.参数更新"
#define DISP_SETMENU1LINE3 "5.查看TVR  6.打印开关"
#define DISP_SETMENU1LINE4 "7.AID参数  8.初始参数"

#define DISP_SETMENU2 "参数设置2"
#define DISP_SETMENU2LINE1 "1.读卡记录 2.应用类型"
#define DISP_SETMENU2LINE2 "3.交易类型 4.强制联机"
#define DISP_SETMENU2LINE3 "5.数据捕获 6.国密算法"
#define DISP_SETMENU2LINE4 "7.清记录   8.通讯超时"

#define DISP_SETMENU3 "参数设置3"
#define DISP_SETMENU3LINE1 "1.qPBOC参数设置"
#define DISP_SETMENU3LINE2 "2.圈存日志"
#define DISP_SETMENU3LINE3 "3.电子现金余额"
#define DISP_SETMENU3LINE4 "4.自动测试"

#define DISP_SETMENU4 "参数设置4"
#define DISP_SETMENU4LINE1 "1.货币代码"

#define DISP_BCTCHOSTMENU "BCTC Param Download"
#define DISP_BCTCHOSTMENU1 "1.CAPK 2.AID"
#define DISP_BCTCHOSTMENU2 "3.Terminal Param"
#define DISP_BCTCHOSTMENU3 "4.Black List"
#define DISP_BCTCHOSTMENU4 "5.Revo PK 6.Init All Data"
#define DISP_BCTCHOSTMENU5 "7.TCP 8.Communicate Type"

#define DISP_SETTIMEMENU 	"设置时钟"
#define DISP_SETTIMEMENU1 	"当前系统时间"
#define DISP_SETTIMEMENU2 	"设置新的时间"
#define DISP_SETTIMEMENU3 	"时间格式错误"

#define DISP_INPUTCURRENCYCODE "请输入货币代码:"

#define DISP_SETECLIMIT "请输入电子现金限额:"
#define DISP_SETCLTRADELIMIT "请输入非接交易限额:"
#define DISP_SETCLOFFLINELIMIT "请输入非接脱机交易限额:"
#define DISP_SETCLCVMLIMIT "请输入CVM限额:"

#define DISP_SETSWITCH	"Func Switch Setting"
#define DISP_SETYESORNO "0 - No  1 - Yes "
#define DISP_SETSWITCHGM "是否支持国密算法"
#define DISP_SETPRINT	"是否打开打印"
#define DISP_SETFORCEONLINE	"是否强制联机"
#define DISP_SETBATCHMODE	"数据捕获方式"
#define DISP_SETBATCHMODE1	"0-ODC  1-BDC"
#define DISP_SETHOSTMODE	"测试后台"
#define DISP_SETHOSTMODE1	"0-XGD  1-BCTC"
#define DISP_SETINITALL		"初始化所有参数"
#define DISP_SETQUICS		"是否支持QUICS"
#define DISP_SETCLPBOC 		"是否支持非接PBOC"
#define DISP_SETONLINEODA 	"是否支持联机ODA"
#define DISP_SETAUTOTEST	"Whether Enable RandNumTest"
#define DISP_SETOUTOTESTTIME	"AutoTest Interval"
#define DISP_SETHOSTTIMEOUT	"Host Time Out"
#define DISP_SETQPBOCTRANSROAD	"非接交易通道"


//trade
#define DISP_INPUTAMOUNT "Input Amount:"
#define DISP_INPUTOTHERAMOUNT "Input CashBack Amount:"
#define DISP_INPUTCLEAR "输错请按[清除]键"
#define DISP_PLSWAIT "请稍候..."
#define DISP_AMOUNT "amt"
#define DISP_PLS "请"
#define DISP_SWIPECARD "刷卡、"
#define DISP_INSERTCARD "插卡、"
#define DISP_WAVECARD "挥卡、"

#define DISP_PROCESSING "Processing"

#define DISP_READCARDOK "Card Read Ok"
#define DISP_REMOVECARD "Please Remove Card"
#define DISP_AUTHORIZING "Authorizing,Pls wait"

#define DISP_INPUTPWD "Please enter your pin"
#define DISP_INPUTPWDBYPASS	" "
#define DISP_ERR_READCARD "Read Card Fail"
#define DISP_ERR_MORECARD "Multi Card"
#define DISP_CONFIRMAMT "Pls Confirm"
#define DISP_SELECTAID	"Aid Select"
#define DISP_USEBANKCARD	"请使用银行卡"


//result
#define DISP_MORECARD	DISP_ERR_MORECARD
#define DISP_ERREXIT	"Press any key exit"
#define DISP_CARDEXPIRED	"卡片过有效期"
#define DISP_TRADETERMINATE	"trans terminate"

#define DISP_PROCESSERROR "Processing Error"
#define DISP_PLEASEINSERTCARD "Please Insert Card"
#define DISP_INSERTSWIPECARD "Insert, Swipe"
#define DISP_ORTRYANOTHERCARD "or Try Another Card"

#define DISP_USEOTHERINTERFACE "Use Other Interface"
#define DISP_SEEPHONE	"Pls See Phone"
#define DISP_REPUTCARD	"Present Card Again"
#define DISP_ONLINEAPPROVE	"Online Approve"
#define DISP_ONLINEDECLINE	"Online Decline"
#define DISP_OFFLINEDECLINE	"Offline Decline"
#define DISP_OFFLINEAPPROVE	"Offline Approve"
#define DISP_NOTACCEPT	"Not Accepted"
#define DISP_BLACK	"黑名单,交易拒绝"
#define DISP_ECBALANCE	"脱机余额"
#define DISP_NOTACCEPTRETRY1	"不接受"
#define DISP_NOTACCEPTRETRY2	"请重试"
#define DISP_TERMINATELOG	"终止查询交易日志"
#define DISP_USEICCINTERFACE "请使用接触式界面"


//host
#define DISP_COMMUTOHOST	"Connecting Host"
#define DISP_NOTRECEIVEHOST	"No Response"
#define DISP_ONLINERECEIVEERR	"Online Error"
#define DISP_ONLINESWITCHOFFLINE	"Change to Offline"

//aid list
#define DISP_AIDLIST	"AID LIST"
#define DISP_NOAIDLIST	"NO AID"
#define DISP_AIDMENU	"AID参数设置"
#define DISP_AIDMENU1	"1.设置TAC  2.TRM参数 "
#define DISP_AIDMENU2	"3.交易类型 4.应用版本"
#define DISP_AIDMENU3	"5.缺省DDOL 6.ASI设置 "
#define DISP_AIDMENU4	"7.联机密码 8.初始参数"

//batch
#define DISP_NOBATCHDATA	"No Batch Data"

//transtype
#define DISP_TRANSTYPEMENU1 "1.现金"
#define DISP_TRANSTYPEMENU2 "2.商品"
#define DISP_TRANSTYPEMENU3 "3.服务"
#define DISP_TRANSTYPEMENU4 "4.返现"
#define DISP_TRANSTYPEMENU5 "5.查询"
#define DISP_TRANSTYPEMENU6 "6.转账"
#define DISP_TRANSTYPEMENU7 "7.管理"
#define DISP_TRANSTYPEMENU8 "8.存款"
#define DISP_TRANSTYPEMENU9 "9.支付"

//icc trade log
#define DISP_LOG_MENU1	"交易日期:"
#define DISP_LOG_MENU2	"交易时间:"
#define DISP_LOG_MENU3	"金额:"
#define DISP_LOG_MENU4	"其他金额:"
#define DISP_LOG_MENU5	"终端国家代码:"
#define DISP_LOG_MENU6	"交易货币代码:"
#define DISP_LOG_MENU7	"商户名称:"
#define DISP_LOG_MENU8	"交易类型:"
#define DISP_LOG_MENU9	"交易计数器:"
#define DISP_LOG_9CMEN1 "商品/服务"
#define DISP_LOG_9CMEN2 "现金"
#define DISP_LOG_9CMEN3 "返现"
#define DISP_LOG_9CMEN4 "查询"
#define DISP_LOG_9CMEN5 "转账"
#define DISP_LOG_9CMEN6 "管理"
#define DISP_LOG_9CMEN7 "存款"
#define DISP_LOG_9CMEN8 "支付"
#define DISP_LOG_LOAD1 	"圈存前金额:"
#define DISP_LOG_LOAD2 	"圈存后金额:"

//mag
#define DISP_MAG_CARDNO		"PAN:"
#define DISP_MAG_CARDEXP	"ExpDate:"
#define DISP_MAG_CARDSERC	"ServiceCode:"
#define DISP_MAG_ICCCARD	"请使用IC卡"

//print
#define DISP_PRINTING		"正在打印"
#define DISP_PRINTSHEET		"-------签购单-------"
#define DISP_PRINTMERNAME	"商户名:"
#define DISP_PRINTMERNO		"商户号:"
#define DISP_PRINTTERNO		"终端号:"
#define DISP_PRINTCARDNO	"卡号:"
#define DISP_PRINTAMT		"金额:"
#define DISP_PRINTSIGN		"请签名______"
#define DISP_PRINTTIME		"交易时间:"
#define DISP_PRINTECBAL		"脱机交易余额:"
//pinpad
#define DISP_ECBALSEEPINPAD	"余额请看密码键盘"

//onlind oda
#define DISP_OLODAFDDAOK		"FDDA认证成功"
#define DISP_OLODAFDDAERR		"FDDA认证失败"
#define DISP_OLODASDAOK			"SDA认证成功"
#define DISP_OLODASDAERR		"SDA认证失败"
#define DISP_OLODAERR			"ODA失败"

#endif



