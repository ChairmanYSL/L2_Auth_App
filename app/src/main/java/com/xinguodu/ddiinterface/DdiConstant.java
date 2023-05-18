package com.xinguodu.ddiinterface;

/**
 * 常量
 * Created by jinniu on 2015/9/6.
 */
public class DdiConstant {
    public static final int FONT_TYPE_HZ1212 = 0;
    public static final int FONT_TYPE_HZ1616 = 1;
    public static final int FONT_TYPE_HZ2020 = 2;
    public static final int FONT_TYPE_HZ2424 = 3;
    public static final int FONT_TYPE_HZ3232 = 4;
    public static final int FONT_TYPE_HZ4848 = 5;
    public static final int FONT_TYPE_NOHZ = 6;//不存在的字库，用于测试黑块

    public static final int FONT_TYPE_ASC6X12 = 0;
    public static final int FONT_TYPE_ASC8X16 = 1;
    public static final int FONT_TYPE_ASC1224 = 2;
    public static final int FONT_TYPE_ASC1632 = 3;
    public static final int FONT_TYPE_ASC2448 = 4;
    public static final int FONT_TYPE_ASC6X8 = 5;
    public static final int FONT_TYPE_ASC1624 = 6;
    public static final int FONT_TYPE_NOASC = 7;//不存在的字库，用于测试黑块

    public static final int FONT_HORIZONTAL = 1;//横库
    public static final int FONT_VERTICAL = 2;//纵库

    public static final int FONT_NATIVE_ZOOM_BASE = 0; //基本字号
    public static final int FONT_NATIVE_ZOOM_GENERAL = 3; //整倍放大

    public static final int FONT_ALIGN_LEFT = 0; //左对齐
    public static final int FONT_ALIGN_RIGHT = 1; //右对齐
    public static final int FONT_ALIGN_CENTER = 2; //居中对齐

    public static final int PRINT_CONENT_STRING = 0;//字符串
    public static final int PRINT_CONENT_BITMAP = 1;//图片

    public static final int FONT_SIZE_SMALL = 0; //小
    public static final int FONT_SIZE_MIDDLE = 1;//中
    public static final int FONT_SIZE_LARGE = 2;//大

    public static final int FONT_SIZE_16 = 16;
    public static final int FONT_SIZE_20 = 20;
    public static final int FONT_SIZE_24 = 24;
    public static final int FONT_SIZE_28 = 28;
    public static final int FONT_SIZE_32 = 32;


/*    //画笔支持的字体类型
    public static final Typeface FONT_TYPE_DEFAULT = Typeface.DEFAULT; //常规字体类型
    public static final Typeface FONT_TYPE_DEFAULT_BOLD = Typeface.DEFAULT_BOLD; //黑体字体类型
    public static final Typeface FONT_TYPE_MONOSPACE = Typeface.MONOSPACE; //等宽字体类型
    public static final Typeface FONT_TYPE_SANS_SERIF = Typeface.SANS_SERIF; //sans serif字体类型
    public static final Typeface FONT_TYPE_SERIF = Typeface.SERIF; //serif字体类型*/

    public static final int FONT_TYPE_SONG = 0; //宋体
    public static final int FONT_TYPE_BLANK = 1; //黑体
    public static final int FONT_TYPE_YOUYUAN = 2; //幼圆

    public static final int DDI_THMPRN_CTL_VER = 0;
    public static final int DDI_THMPRN_CTL_GRAY = 1;
    public static final int DDI_THMPRN_CTL_LINEGAP = 2;
    public static final int DDI_THMPRN_CTL_COLGAP = 3;

    public static final int DDI_OK = 0;
    public static final int DDI_ERR = -1;
    public static final int DDI_ETIMEOUT = -2;
    public static final int DDI_EBUSY = -3;
    public static final int DDI_ENODEV = -4;
    public static final int DDI_EACCES = -5;
    public static final int DDI_EINVAL = -6;
    public static final int DDI_EIO = -7;
    public static final int DDI_EDATA = -8;
    public static final int DDI_EPROTOCOL = -9;
    public static final int DDI_ETRANSPORT = -10;
    public static final int DDI_DRAWERR = -11;


    public static final int DDI_RF_CTL_VER = 0;//获取非接读卡器设备版本
    public static final int DDI_RF_CTL_SAK = 1;//获取选择应答 SAK 值
    public static final int DDI_RF_CTL_UID = 2;//卡片 ID
    public static final int DDI_RF_CTL_MF_AUTH = 3;//Mifare 卡认证
    public static final int DDI_RF_CTL_MF_READ_RAW = 4;//读取原始二进制数据
    public static final int DDI_RF_CTL_MF_WRITE_RAW = 5;//写原始二进制数据
    public static final int DDI_RF_CTL_MF_READ_VALUE = 6;//读取块值
    public static final int DDI_RF_CTL_MF_WRITE_VALUE = 7;//写块值
    public static final int DDI_RF_CTL_MF_INC_VALUE = 8;//增值操作
    public static final int DDI_RF_CTL_MF_DEC_VALUE = 9;//减值操作
    public static final int DDI_RF_CTL_MF_BACKUP_VALUE = 10;//块值备份
    public static final int DDI_RF_CTL_SET_PARAM = 11;//设置非接控制芯片参数
    public static final int DDI_RF_CTL_GET_PARAM = 12;//读非接控制芯片参数

    public static final int DDI_KEYVALUE = 0;
    public static final int DDI_BYPASSPIN = -3;
    public static final int DDI_INPUTCANCEL = -5;
    public static final int DDI_TIMEOUT = -2;

}
