#ifndef __E131_CONSTANTS_H__
#define __E131_CONSTANTS_H__
//E1.31Constants.h



//DEFINE CONSTANTS FOR ROOT LAYER
//Preamble Size
//Preamble Size Define RLP Preamble Size
//2 (0,1)
//0x0010   **
#define RLP_PREAMBLE_SIZE 2
#define RLP_PREAMBLE_SIZE_VALID 0x0010

//Post-amble Size
//RLP Post-amble Size.
//2 (1,2)
//0x0000
#define RLP_POST_AMBLE_SIZE 2
#define RLP_POST_AMBLE_SIZE_VALID 0x0000
//ACN Packet Identifier
//Identifies this packet as E1.17
//12 (4-15)
//0x41 0x53 0x43 0x2d 0x45
//0x31 0x2e 0x31 0x37 0x00
//0x00 0x00
#define ACN_PACKET_IDENTIFIER 4

//2 (16-17)
//Low 12 bits = PDU length
//High 4 bits = 0x7
//#define PROTOCOL_FLAGS_AND_LENGTH 16

//Vector Identifies RLP Data as 1.31
//Protocol PDU
//0x00000004
//4 (18-21)
//0x00000004  **

#define IDX_VECTOR_PROTOCOL 18
#define E1_31_VECTOR_VAL 0x00000004

//CID Sender's
//CID
//VALUE: Sender's unique ID
//16 (22-37)
//#define IDX_SENDER_CID 22

//END ROOT LAYER

//DEFINE CONSTANTS FOR E1.31 Framing Layer

//2 (38-39)
//Low 12 bits = PDU length
//High 4 bits = 0x7
//#define E1_31_FRAMING_FLAGS_AND_LENGTH  38

//(40-43) 4
//Vector
//Identifies 1.31 data as DMP Protocol PDU
//0x00000002  **
#define E1_31_FRAMING_VECTOR 40
#define E1_31_FRAMING_VECTOR_VALID  0x00000002



//(44-107) 64
//Source Name
//User Assigned Name ofSource
//UTF-8 [UTF-8] encoded
//string, null-terminated
//#define E1_31_SOURCE_NAME 44
//

//(108) 1
//Priority
//Data priority if multiple sources
//0-200, default of 100
//#define E1_31_FRAMING_PRIORITY 108

//(109-110) 2
// Reserved
// Reserved
// Transmitter Shall Send 0 Receivers Shall Ignore
//#define E1_31_FRAMING_RESERVED 109
//(111) 1
//Sequence Number
//Sequence Number To detect duplicate or out of order packets.
//#define E1_31_FRAMING_SEQ_NO 111
//

//(112) 1
//Options
//Options Flags
//Bit 7 = Preview_Data
//Bit 6 = Stream_Terminated
//#define E1_31_FRAMING_OPTIONS_FLAGS 112

//(113-114) 2
// Universe
// Universe Number Identifier for a distinct stream of DMX Data *
#define E1_31_FRAMING_UNIVERSE_ID 113

//////////DMP Layer (See Section 7)
//(115-116) 2
// Flags and Length
// Protocol flags and length
//Low 12 bits = PDU length
//High 4 bits = 0x7
//#define E1_31_DMP_FLAGS_LENGTH 115


//(117) 1
//Vector
// Identifies DMP Set Property Message PDU
//0x02  * *
#define E1_31_DMP_VECTOR 117
#define E1_31_DMP_VECTOR_VALID 0x02
//(118) 1
// Address Type & Data Type
//Identifies format of address and data
//0xa1 * *
#define E1_31_DMP_ADDRESS_TYPE_DATA_TYPE 118
#define E1_31_DMP_ADDRESS_TYPE_DATA_TYPE_VALID 0xA1

//(119-120) 2
//First Property Address
//Indicates DMX START Code is at DMP address 0
//0x0000 **
#define E1_31_DMP_FIRST_PROP_ADDR 119
#define E1_31_DMP_FIRST_PROP_ADDR_VALID 0x0000

//(121-122) 2
// Address Increment
// Indicates each property is 1 octet
// 0x0001  **
#define E1_31_DMP_ADDRESS_INCREMENT 121
#define E1_31_DMP_ADDRESS_INCREMENT_VALID 0x0001
//(123-124) 2
//Property value count
//Indicates 1+ the number of slots in packet
//0x0001 -- 0x0201
#define E1_31_DMP_PROP_VAL_CNT 123 //*

//(125-637) 1-513
//Property values
//DMX512-A START Code + data START Code + Data
#define E1_31_DMP_START 125
#define E1_31_DMP_FIRST 126 //*

#endif //__E131_CONSTANTS_H__
