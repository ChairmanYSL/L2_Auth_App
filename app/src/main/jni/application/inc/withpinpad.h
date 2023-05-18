#ifndef AFX_WITHPINPAD_H
#define AFX_WITHPINPAD_H

#define SOH 0x01	
#define ETX 0x03
#define EOT 0x04
#define TEXT 0x10
#define ORDER 0x11         
#define KEY_TEXT 0X12      
#define KEY_ORDER 0X13     
#define EXT_TEXT  0x14     
#define KEY_EXTEXT 0x15    
#define SIM_TEXT  0x16     
#define SIM_ORDER 0x17     
#define SIM_EXTEXT 0x20    
#define BASE_TEXT 0X18     
#define BASE_ORDER 0X19    
#define BASE_EXT_TEXT  0x20     
#define MODEMPRODATA 0XB0	

#define STAKE_TEXT 0x18						
#define STAKE_ORDER 0x19					
#define STAKE_TEXTEX 0x20					

#define MAGDATA			0X31	
#define PRINTDATA		0X4A  
#define PRINTDOT 		0X4B	
#define PRINTPAGE 	0X4C	
#define SYNCHCLOCK	0x50	
#define MODEMTIMESTATE 0X60
#define MODEMDATA 	0X61 
#define MODEMOUT 		0X6A	
#define MODEMINI 		0X6D	
#define SOFTVER			0xb1	
#define PARADOWN		0x4E	
#define SIMRESPON		0x71	
#define RSTSIMFAIL	0X72	
#define SIMIN				0x76	
#define SIMOUT			0x77	
#define SIMRESETACK	0x78	
#define RESETSIM 		0X7B	
#define OPERASIM 		0X7C	
#define ENDSIM 			0X7D	
#define HARDCONFIG 	0xA4	


#define TEXTEX 0x14				
#define KEY_TEXTEX 	0x15	
#define SIM_TEXTEX 0x20		

#define COM_QPBOC_NO 18

#define COM_PAD_NO 0x02 

#define POS_PINMODE					0xE9		
#define POS_TRADEAMOUNT				0x8A	
#define PAD_PINCODE					0xE5		
#define PAD_REQUESTRANDOM			0x82		
#ifndef DATAOK
#define DATAOK                      0x20	  
#endif
#define TOPWDPAD   			        0x00      
#define POS_RANDOM					0x8B
#define CANCELKEY 	                0X8C	

#define ICC_TEXT                0X12
#define ICC_ORDER            	0X13
#define ICC_TEXTEX           	0x15

#define ICC_SIM_TEXT                    0x16
#define ICC_SIM_ORDER                   0x17
#define ICC_SIM_TEXTEX                  0x24

#endif

