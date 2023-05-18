#ifndef SDKPED_H
#define SDKPED_H

//the mode of requesting PIN
typedef enum
{
    SDK_PED_IC_ONLINE_PIN,                                              //IC card online PIN
    SDK_PED_IC_OFFLINE_PIN,                                             //IC card offline PIN(plaintext)
    SDK_PED_MAG_PIN                                                     //magnetic stripe card PIN
} SDK_PED_PIN_MODE;

//error return code
typedef enum
{
    SDK_PED_PIN_FORMAT_ERR = (-105), //password format error
    SDK_PED_TPK_ERR = (-106),        //PIN key error
    SDK_PED_TAK_ERR = (-107),        //MAC key error
    SDK_PED_FLUSHWK_ERR = (-108),    //write working key
    SDK_PED_NOPIN = (-109)           //no password return
}
SDK_PED_RETURN;


#endif
