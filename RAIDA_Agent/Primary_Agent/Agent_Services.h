#ifndef AGENT_SERVICES_H
#define AGENT_SERVICES_H
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <inttypes.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>
#include <stdint.h>
#include <math.h>

#include "RAIDA_Agent.h"

//--------------------------------------------------------------------
#define FRAME_TIME_OUT_SECS		1 
#define UDP_BUFF_SIZE 			65535
//------------------------------------------------------------------
#define REQUEST_HEADER_MAX 		    48
#define MAXLINE                     1024
#define RESPONSE_SIZE_MAX 		65535

#define VER 255
//-----------------------------------------------------------------
//Indexs at which the bytes start
#define REQ_CL  					0
#define REQ_SP  					1
#define REQ_RI  					2
#define REQ_SH  					3
#define REQ_CM  					4
#define REQ_VE  					6
#define REQ_CI  					7
#define REQ_RE  					9
#define REQ_EC  					12
#define REQ_FC  					14
#define REQ_EN  					16
#define REQ_ID                      17
#define REQ_SN                      19

#define REQ_NO_1  				9
#define REQ_NO_2  				10
#define REQ_NO_3  				11
#define REQ_NO_4  				12
#define REQ_NO_5  				13

#define REQ_NO_6				19
#define REQ_NO_7				20
#define REQ_NO_8				21

#define REQ_HEAD_MIN_LEN 		22
//-------------------------------------------------------------
#define TY_BYTES_CNT				1
#define TT_BYTES_CNT				1
#define RA_BYTES_CNT				1
#define DENOM_BYTES_CNT			1
#define PAGE_NO_BYTES_CNT		1
#define RAIDA_BYTE_CNT			1
#define ROWS_BYTES_CNT			1
#define YY_BYTES_CNT				1
#define MM_BYTES_CNT			1
#define DD_BYTES_CNT				1
#define COIN_TYPE_BYTES_CNT		1
#define MFS_BYTES_CNT			1
#define FREE_ID_BYTES_CNT			1
#define CMD_END_BYTES_CNT		2
#define SN_BYTES_CNT				3	
#define OWNER_ID_BYTES_CNT		3
#define RECORD_BYTES_CNT			3
#define KEY_ID_BYTES_CNT			3
#define TK_BYTES_CNT				4
#define MS_BYTES_CNT			4
#define AMT_BYTES_CNT			4
#define HS_BYTES_CNT				4
#define EN_BYTES_CNT				5
#define FREE_ID_SERV_LOCK_TIME		6
#define DT_BYTES_CNT				7	
#define RESP_BUFF_MIN_CNT			12
#define FIX_SRNO_MATCH_CNT		13

#define ENCRYPTION_CONFIG_BYTES  16
#define NOUNCE_BYTES_CNT         16

#define GUID_BYTES_CNT 			16
#define AN_BYTES_CNT 			16
#define PAN_BYTES_CNT 			16
#define PG_BYTES_CNT	 			16
#define KEY_BYTES_CNT			16
#define CH_BYTES_CNT				16
#define PWD_BYTES_CNT			16
#define LEGACY_RAIDA_TK_BYTES_CNT		22
#define MD_HASH_BYTES_CNT		32
#define META_DATA_BYTES_CNT		50

#define REQ_END					62
#define RESP_END                62

#define KEY_HASH_BYTES_CNT		64

//-------------------------------------------------
#define KEYS_MAX				100
#define ROWS_MUL_FACTOR			100
#define MAX_ROWS_SHOW_STAT		255	
#define FRAMES_MAX				100
#define EN_CODES_MAX			255
#define DEFAULT_YEAR			2000
#define COINS_MAX				2000
#define SECS_IN_DAY				60 * 60 *24				1

//------Indexs for Response Header----------------------------
#define RES_RI  						0
#define RES_SH  						1
#define RES_SS 	 					2
#define RES_EX 						3
#define RES_RE 						4
#define RES_EC 						6
#define RES_HS 						8

#define RESP_HEADER_MIN_LEN         12
//---------Status Error codes----------------------------------------
#define INVALID_CLOUD_ID 		   				1
#define RAIDA_ OFFLINE 			   			2
#define INVALID_FRAME_CNT 	 				15
#define INVALID_PACKET_LEN 	 				16
#define FRAME_TIME_OUT	 	 				17
#define WRONG_RAIDA 			 				18
#define INVALID_SPLIT_ID 		 				19
#define SHARD_NOT_AVAILABLE   					20
#define VALIDATE_TICKET_FOUND					21
#define VALIDATE_TICKET_NOT_FOUND				22
#define VALIDATE_TICKET_INVALID_RAIDA			23
#define INVALID_CMD 			  				24
#define COIN_ID_NOT_FOUND 	  				25
#define COIN_LIMIT_EXCEED			 	  		26
#define INVALID_EN_CODE		 	  			27
#define COIN_OWNER_ID_NOT_FOUND		 	  	28
#define POWN_AN_PAN_SAME					30
#define LEN_OF_BODY_CANT_DIV_IN_COINS			32
#define INVALID_END_OF_REQ					33
#define EMPTY_REQ_BODY						36
#define VALIDATE_TICKET_CLAIMED_EARLIER			37
#define VALIDATE_TICKET_CLAIMED				38
#define COIN_NO_NOT_FOUND					39
#define SN_ALL_READY_IN_USE					40
#define SERVICE_LOCKED						41

#define AGENT_GET_PAGE                      40
#define MIRROR_REPORT_CHANGES               45

#define FAILED_TO_AUTHENTICATE					64
#define PAGE_NOT_FOUND						66
#define BREAK_COUNTER_FEIT					70
#define BREAK_COINS_NOT_FOUND					72
#define BREAK_COINS_SUM_NOT_MATCH				74
#define BREAK_CANNOT_BREAK					76
#define JOIN_COUNTER_FEIT						80
#define JOIN_COINS_NOT_FOUND					82
#define JOIN_COINS_SUM_NOT_MATCH				84
#define JOIN_CANNOT_JOIN						86
#define FIX_ALL_TICKET_ZERO					90
#define LEGACY_RAIDA_TIME_OUT					100
#define LEGACY_RAIDA_FAIL						101

#define RAIDA_AGENT_PAGES_RETURNED          100
#define RAIDA_AGENT_NO_CHANGES              101
#define RAIDA_AGENT_PRIMARY_UP              102
#define RAIDA_AGENT_PRIMARY_DOWN            104
#define MIRROR_MESSAGE_RECEIVED             105
#define MIRROR_REPORT_RETURNED              106
#define MIRROR_REQUESTED_FILE_NOT_EXIST     107

#define IDENTIFY_COIN_FOUND					192
#define IDENTIFY_COIN_NOT_FOUND				193
#define SYNC_ADD_COIN_EXIST					200
#define FIND_ALL_NONE						208
#define FIND_ALL_AN							209
#define FIND_ALL_PA							210
#define FIND_MIXED							211
#define ALL_PASS							241
#define ALL_FAIL								242
#define MIX								243
#define SUCCESS								250
#define FAIL								251
#define NO_ERR_CODE	 		   				255
//----------Coin Converter Command Codes---------------------
#define NO_RESPONSE                    244
#define NO_TICKET_FOUND                245
//----------Command codes-----------------------------------------
#define CMD_COIN_CONVERTER 					215
#define CMD_ECHO							4

#define STATE_WAIT_START						1
#define STATE_START_RECVD					2
#define STATE_WAIT_END						3
#define STATE_END_RECVD						4
//--------------------------------------------------------------------------	
#define  UDP_RESPONSE 						0
#define  FIFO_RESPONSE 						1

//-----------RAIDA Agent Codes---------------------------------------

#define TABLE_ID_ANS                0
#define TABLE_ID_OWNERS             1
#define TABLE_ID_EMAIL_RECOVER      2
#define TABLE_ID_STATEMENTS         3 
#define TABLE_ID_LOSS_COIN          4

#define RAIDA_AGENT_TABLE_ID_BYTES_CNT          1
#define RAIDA_AGENT_COIN_ID_BYTES_CNT           2
#define RAIDA_AGENT_SN_NO_BYTES_CNT             4
#define TIMESTAMP_BYTES_CNT		            6
#define RAIDA_AGENT_FILE_ID_BYTES_CNT           7

#define RESP_BODY_END_BYTES                  2

#define AGENT_PRIMARY               1
#define AGENT_MIRROR                2
#define AGENT_WITNESS               3

#define SN_SIZE                     14
#define FILES_COUNT_MAX           10000  //9360
#define KEYS_COUNT                10000
#define AGENT_FRAMES_MAX           64



//-------------SERVICES----------------------
extern char execpath[256], file_path[500];
extern time_t t1;
extern struct sockaddr_in servaddr;
extern struct timeval timeout;
extern int sockfd;
extern fd_set select_fds;  
extern long time_stamp_before,time_stamp_after;
extern unsigned char udp_buffer[UDP_BUFF_SIZE],response[RESPONSE_SIZE_MAX], udp_response[MAXLINE];
extern unsigned int index_resp;

//--------------------------------------------

union conversion {
	uint32_t val32;
    unsigned int val;
    unsigned char byte2[2];
	unsigned char byte4[4];
};
extern union conversion byteObj;

//---------------MAIN--------------------------------------
void get_execpath();
void set_time_out(unsigned char);
int init_udp_socket();
int listen_request();
void process_request(unsigned int);
void send_err_resp_header(int);
unsigned char validate_request_header(unsigned char *,int );
unsigned char validate_request_body_general(unsigned int ,unsigned int ,int *);
void Send_Response(unsigned int );
void prepare_resp_header(unsigned char , int );
void prepare_udp_resp_body(unsigned char , unsigned char ); 
unsigned int prepare_resp_body(unsigned int , unsigned int , unsigned int , unsigned int  );
void get_ModifiedFiles(char * );
void  execute_Report_Changes(unsigned int );
void execute_Get_Page(unsigned int );
void Get_File_Contents(char []);




#endif
