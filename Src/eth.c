//	----------------------------------------------------------------------------------------------------
//	COMPANY	NAME	:
//	PROJECT	NAME	:
//	FILE	NAME	:	eth.c
//	MAKE	BY		:
//	DESCRIPTION		:
//	CREATED DATE	:	2019.03.04
//	----------------------------------------------------------------------------------------------------


// Includes
#include "stm32f7xx.h"
#include "main.h"
#include "lwip.h"
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "lwip/memp.h"

#include "LCD.h"
#include "i2c.h"

#if LWIP_TCP

// Defines
#define BOARD_1				(0)
#define BOARD_2				(1)
#define READ_WORD_START_ADDR	((uint32_t)(0x00000000) + (BOARD_1*6))
#define READ_WORD_LENGTH		(0x06)
#define WRITE_WORD_START_ADDR	((uint32_t)(0x0000000C) + (BOARD_1*6))


// Variables
uint8_t g_arEthSrvrRxBuf[64] = {0, }, g_arEthSrvrTxBuf[128] = {0x53, };
uint8_t g_arEthClntRxBuf[64] = {0, }, g_arEthClntTxBuf[128] = {0x50, };
uint8_t g_deEthSrvrConnFlg = 0, g_deEthClntConnFlg = 0;
uint8_t g_deEthSrvrConnCnt = 0, g_deEthClntConnCnt = 0;
uint16_t g_deEthSrvrCmdFlg = 0, g_deEthClntCmdFlg = 0;
uint16_t g_deEthSrvrLength = 0, g_deEthClntLength = 0;
uint16_t g_deDataSrvrLeng = 0, g_deDataSrvrDvld = 0, g_deDataSrvrDvcd = 0, g_deDataSrvrDvct = 0, g_deDataSrvrCmd = 0;
uint16_t g_deDataClntLeng = 0, g_deDataClntDvld = 0, g_deDataClntDvcd = 0, g_deDataClntDvct = 0, g_deDataClntCmd = 0;


// Structures
enum tcp_server_states
{
  ES_SRVR_NONE = 0,
  ES_SRVR_ACCEPTED,
  ES_SRVR_RECEIVED,
  ES_SRVR_CLOSING
};
struct tcp_server
{
  u8_t state;             // current connection state //
  u8_t retries;
  struct tcp_pcb *pcb;    // pointer on the current tcp_pcb //
  struct pbuf *p;         // pointer on the received/to be transmitted pbuf //
};

static struct tcp_pcb *tcp_server_pcb;

struct tcp_pcb *g_stTPcb;
struct tcp_server *g_stEs;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t u1t_connected_check = 0;
uint8_t u1f_connect_cnt = 0;
uint8_t u1f_rxeth[50];
uint16_t u2f_Word_data[6];

uint8_t u1f_eth_process_flag = 0;

uint8_t u1f_mx_process_flag = 1;

u8_t  recev_buf[50];
__IO uint32_t message_count=0;

u8_t   data[100];

struct tcp_pcb *client_pcb;

enum tcp_client_states
{
  ES_CLNT_NOT_CONNECTED = 0,
  ES_CLNT_CONNECTED,
  ES_CLNT_RECEIVED,
  ES_CLNT_CLOSING,
};

struct tcp_client
{
  enum tcp_client_states state; /* connection status */
  struct tcp_pcb *pcb;          /* pointer on the current tcp_pcb */
  struct pbuf *p_tx;            /* pointer on pbuf to be transmitted */
};

struct tcp_client *cs;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//extern RealTimeData_t DNT_RTC;

// ↓↓↓↓↓↓↓ Ethernet 버퍼 관련 변수 ↓↓↓↓↓↓↓
// 수신 버퍼
uint8_t u1f_eth_rx_buff[100];
uint8_t u1f_eth_tx_buff[1200];
// 버퍼 카운터
uint16_t u2f_eth_cnt = 0;
// INDEX 카운터
uint8_t u1f_eth_index = 0;
// Bootloader 명령 체크 플래그
uint8_t u1f_bootjump_flag = 0;
// ↑↑↑↑↑↑↑ Ethernet 버퍼 관련 변수 ↑↑↑↑↑↑↑

// MC 프로토콜과 ICD 프로토콜 구분 Select
uint8_t u1f_eth_cnd_select = 0;

// DAQ_ICD01(명령) 수신 데이터 저장 구조체 

CMD1_DATA st_rcv_cmd1;				// 1. 파티클 데이터 요청
CMD_NONE_DATA st_rcv_cmd2;			// 2. 현재 설정값 요청
CMD_NONE_DATA st_rcv_cmd3;			// 3. 모듈 재시작
CMD_NONE_DATA st_rcv_cmd4;			// 4. 이벤트 로그 초기화
CMD5_DATA st_rcv_cmd5;				// 5. 모드 설정
CMD6_DATA st_rcv_cmd6;				// 6. 이물감지 기준 설정
CMD7_DATA st_rcv_cmd7;				// 7. 소리 설정
CMD8_DATA st_rcv_cmd8;				// 8. 가동시간 기준 설정
CMD9_DATA st_rcv_cmd9;				// 9. RS485 통신 설정
CMD10_DATA st_rcv_cmd10;			// 10. LAN 통신 SERVER 설정
CMD11_DATA st_rcv_cmd11;			// 11. LAN 통신 MAC/CLIENT 설정
CMD12_DATA st_rcv_cmd12;			// 12. WLAN 통신 SSID 설정
CMD13_DATA st_rcv_cmd13;			// 13. WLAN 통신 PASS 설정
CMD14_DATA st_rcv_cmd14;			// 14. WLAN 통신 SERVER 설정
CMD15_DATA st_rcv_cmd15;			// 15. WLAN 통신 MAC/CLIENT 설정
CMD16_DATA st_rcv_cmd16;			// 16. BLE 통신 SERVICE UUID 설정
CMD17_DATA st_rcv_cmd17;			// 17. BLE 통신 CHAR. UUID 설정
CMD18_DATA st_rcv_cmd18;			// 18. BLE 통신 SPARE UUID 설정
CMD19_DATA st_rcv_cmd19;			// 19. BLE 통신 PIN CODE 설정
CMD20_DATA st_rcv_cmd20;			// 20. 온습도감지 기준 설정
CMD21_DATA st_rcv_cmd21;			// 21. 온습도 보정치 설정
CMD_NONE_DATA st_rcv_cmd22;			// 22. 이벤트 로그 요청
CMD_NONE_DATA st_rcv_cmd23;			// 23. 일련번호 요청
CMD_NONE_DATA st_rcv_cmd24;			// 24. 버전정보 요청

// DAQ_ICD01(응답) 송신 데이터 저장 구조체 
ACK_DATA_REQ st_data_ack;				// 1. 파티클센서모듈 데이터 요청 응답
ACK_PARA_REQ st_para_ack;				// 2. 파티클센서모듈 현재 설정값 응답
ACK_CMD st_cmd_ack;						// 3. 제어명령 ACK
LOG_DATA_128EA log_data_ack;			// 4. 이벤트로그 ACK
ACK_PARA_SERIAL_NUM_REQ serial_num_ack;	// 5. 일련번호 ACK
ACK_PARA_VERSION_REQ version_ack;		// 6. 버전정보 ACK


void _______Below_this_line_is_the_response_send_related_function(void);

static void tcp_protocal(struct tcp_pcb *tpcb, struct tcp_server *es);
static void tcp_protocal_data(struct tcp_pcb *tpcb, struct tcp_server *es);
static void tcp_protocal_para(struct tcp_pcb *tpcb, struct tcp_server *es);
static void tcp_protocal_rep(struct tcp_pcb *tpcb, struct tcp_server *es);
static void tcp_protocal_log(struct tcp_pcb *tpcb, struct tcp_server *es);
static void tcp_protocal_serial(struct tcp_pcb *tpcb, struct tcp_server *es);
static void tcp_protocal_version(struct tcp_pcb *tpcb, struct tcp_server *es);

void _______Below_this_line_is_functions_related_to_the_command_structure(void);

static void tcp_rcv_particle_data_request(void);
static void tcp_rcv_eventlog_init(void);
static void tcp_rcv_mode_set(void);
static void tcp_rcv_alarm_set(void);
static void tcp_rcv_sound_set(void);
static void tcp_rcv_lifetime_set(void);
static void tcp_rcv_rs485_set(void);
static void tcp_rcv_lanserver_set(void);
static void tcp_rcv_lanmac_client_set(void);
static void tcp_rcv_wlanssid_set(void);
static void tcp_rcv_wlanpass_set(void);
static void tcp_rcv_wlanserver_set(void);
static void tcp_rcv_wlanmac_client_set(void);
static void tcp_rcv_bleservuuid_set(void);
static void tcp_rcv_blecharuuid_set(void);
static void tcp_rcv_blespareuuid_set(void);
static void tcp_rcv_blepincode_set(void);
static void tcp_rcv_thalarm_set(void);
static void tcp_rcv_thscale_set(void);

void _______Below_this_line_is_the_checksum_related_function(void);

static uint16_t tcp_checksum_creat(uint16_t u2t_cnt);
static uint16_t tcp_checksum_check(uint16_t u2t_cnt);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Functions
void ETH_INIT(void);
void ETH_SRVR_RECV(void);
void ETH_SRVR_TRANS(void);
void ETH_CLNT_RECV(void);
void ETH_CLNT_TRANS(void);

static err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err);
static err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static void tcp_server_error(void *arg, err_t err);
static err_t tcp_server_poll(void *arg, struct tcp_pcb *tpcb);
static err_t tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
static void tcp_server_send(struct tcp_pcb *tpcb, struct tcp_server *es);
static void tcp_server_connection_close(struct tcp_pcb *tpcb, struct tcp_server *es);

static err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static void tcp_client_connection_close(struct tcp_pcb *tpcb, struct tcp_client * es);
static err_t tcp_client_poll(void *arg, struct tcp_pcb *tpcb);
static err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
static void tcp_client_send(struct tcp_pcb *tpcb, struct tcp_client * es);
static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err);

static err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
{
	err_t ret_err;
	struct tcp_server *es;

	LWIP_UNUSED_ARG(arg);
	LWIP_UNUSED_ARG(err);

	// set priority for the newly accepted tcp connection newpcb //
	tcp_setprio(newpcb, TCP_PRIO_MIN);

	// allocate structure es to maintain tcp connection informations //
	es = (struct tcp_server *)mem_malloc(sizeof(struct tcp_server));
//	g_stEs = (struct tcp_server *)mem_malloc(sizeof(struct tcp_server));

	if (es != NULL)
	{
		es->state = ES_SRVR_ACCEPTED;
		es->pcb = newpcb;
		es->retries = 0;
		es->p = NULL;

		// pass newly allocated es structure as argument to newpcb //
		tcp_arg(newpcb, es);

		// initialize lwip tcp_recv callback function for newpcb  //
		tcp_recv(newpcb, tcp_server_recv);

		// initialize lwip tcp_err callback function for newpcb  //
		tcp_err(newpcb, tcp_server_error);

		// initialize lwip tcp_poll callback function for newpcb //
		tcp_poll(newpcb, tcp_server_poll, 0);

		ret_err = ERR_OK;
	}
	else
	{
		//  close tcp connection //
		tcp_server_connection_close(newpcb, es);
		// return memory error //
		ret_err = ERR_MEM;
	}
	return ret_err;
}

static err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
	struct tcp_server *es;
	err_t ret_err;

	LWIP_ASSERT("arg != NULL",arg != NULL);

	es = (struct tcp_server *)arg;

	// if we receive an empty tcp frame from client => close connection //
	if (p == NULL)
	{
		// remote host closed connection //
		es->state = ES_SRVR_CLOSING;
		if(es->p == NULL)
		{
			// we're done sending, close connection //
			tcp_server_connection_close(tpcb, es);
		}
		else
		{
			// we're not done yet //
			// acknowledge received packet //
			tcp_sent(tpcb, tcp_server_sent);

			// send remaining data//
			tcp_server_send(tpcb, es);
		}
		ret_err = ERR_OK;
	}
	// else : a non empty frame was received from client but for some reason err != ERR_OK //
	else if(err != ERR_OK)
	{
		// free received pbuf//
		if (p != NULL)
		{
			es->p = NULL;
			pbuf_free(p);
		}
		ret_err = err;
	}
	else if(es->state == ES_SRVR_ACCEPTED)
	{
		// first data chunk in p->payload //
		es->state = ES_SRVR_RECEIVED;
		
		// store reference to incoming pbuf (chain) //
		es->p = p;
		
		u1f_eth_cnd_select = 0;
		
		// initialize LwIP tcp_sent callback function //
		
	    MEMCPY((void*)g_arEthSrvrRxBuf, (void*)p->payload, p->tot_len);
		ETH_SRVR_RECV();
		
		if(u1f_eth_cnd_select == 1)
		{
			tcp_sent(tpcb, tcp_server_sent);
		
			tcp_server_send(tpcb, es);
		}
		
		else
		{
			tcp_protocal(tpcb, es);
		}
		
		ret_err = ERR_OK;
	}
	else if (es->state == ES_SRVR_RECEIVED)
	{
		// more data received from client and previous data has been already sent//
		if(es->p == NULL)
		{
			es->p = p;
			
			u1f_eth_cnd_select = 0;
			
			MEMCPY((void*)g_arEthSrvrRxBuf, (void*)p->payload, p->tot_len);
			ETH_SRVR_RECV();
			
			if(u1f_eth_cnd_select == 1)
			{
				tcp_server_send(tpcb, es);
			}
			
			else
			{
				tcp_protocal(tpcb, es);
			}
		}
		else
		{ 
			struct pbuf *ptr;
			
			// chain pbufs to the end of what we recv'ed previously  //
			ptr = es->p;
			pbuf_chain(ptr,p);
			
			MEMCPY((void*)g_arEthSrvrRxBuf, (void*)p->payload, p->tot_len);
			ETH_SRVR_RECV();
			
			tcp_server_send(tpcb, es);
							
		}
		ret_err = ERR_OK;
	}
	else if(es->state == ES_SRVR_CLOSING)
	{
		// odd case, remote side closing twice, trash data //
		tcp_recved(tpcb, p->tot_len);
		es->p = NULL;
		pbuf_free(p);
		ret_err = ERR_OK;
	}
	else
	{
		// unkown es->state, trash data  //
		tcp_recved(tpcb, p->tot_len);
		es->p = NULL;
		pbuf_free(p);
		ret_err = ERR_OK;
	}
	return ret_err;
}


static void tcp_server_error(void *arg, err_t err)
{
	struct tcp_server *es;

	LWIP_UNUSED_ARG(err);

	es = (struct tcp_server *)arg;
	if (es != NULL)
	{
		//  free es structure //
		mem_free(es);
	}
}

static err_t tcp_server_poll(void *arg, struct tcp_pcb *tpcb)
{
	err_t ret_err;
	struct tcp_server *es;

	es = (struct tcp_server *)arg;
	if (es != NULL)
	{
		if (es->p != NULL)
		{
			tcp_sent(tpcb, tcp_server_sent);
			// there is a remaining pbuf (chain) , try to send data //
			tcp_server_send(tpcb, es);
		}
		else
		{
			// no remaining pbuf (chain)  //
			if(es->state == ES_SRVR_CLOSING)
			{
				//  close tcp connection //
				tcp_server_connection_close(tpcb, es);
			}
		}
		ret_err = ERR_OK;
	}
	else
	{
		// nothing to be done //
		tcp_abort(tpcb);
		ret_err = ERR_ABRT;
	}
	return ret_err;
}

static err_t tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
	struct tcp_server *es;

	LWIP_UNUSED_ARG(len);

	es = (struct tcp_server *)arg;
	es->retries = 0;

	if(es->p != NULL)
	{
		// still got pbufs to send //
		tcp_sent(tpcb, tcp_server_sent);
		tcp_server_send(tpcb, es);
		//tcp_protocal(tpcb, es);
	}
	else
	{
		// if no more data to send and client closed connection//
		if(es->state == ES_SRVR_CLOSING)
		tcp_server_connection_close(tpcb, es);
	}
	return ERR_OK;
}

static void tcp_server_send(struct tcp_pcb *tpcb, struct tcp_server *es)
{
	struct pbuf *ptr;
	err_t wr_err = ERR_OK;

	ETH_SRVR_TRANS();

	while ((wr_err == ERR_OK) && (es->p != NULL) && (es->p->len <= tcp_sndbuf(tpcb)))
	{
		// get pointer on pbuf from es structure //
		ptr = es->p;

		// enqueue data for transmission //
		// MC
		if(u1f_eth_cnd_select == 1)
		{
			wr_err = tcp_write(tpcb, g_arEthSrvrTxBuf, g_deEthSrvrLength, 1);
		}
		// ICD
		else if(u1f_eth_cnd_select == 2)
		{
			wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);
		}
		else;

		asm(" NOP");

		if (wr_err == ERR_OK)
		{
			u16_t plen;
			u8_t freed;

			plen = ptr->len;

			// continue with next pbuf in chain (if any) //
			es->p = ptr->next;

			if(es->p != NULL)
			{
				// increment reference count for es->p //
				pbuf_ref(es->p);
			}

			// chop first pbuf from chain //
			do
			{
				// try hard to free pbuf //
				freed = pbuf_free(ptr);
			}
			while(freed == 0);

			// we can read more data now //
			tcp_recved(tpcb, plen);
		}
		else if(wr_err == ERR_MEM)
		{
			// we are low on memory, try later / harder, defer to poll //
			es->p = ptr;
		}
		else
		{
			// other problem ?? //
		}
	}
}

static void tcp_server_connection_close(struct tcp_pcb *tpcb, struct tcp_server *es)
{

  // remove all callbacks //
  tcp_arg(tpcb, NULL);
  tcp_sent(tpcb, NULL);
  tcp_recv(tpcb, NULL);
  tcp_err(tpcb, NULL);
  tcp_poll(tpcb, NULL, 0);

  // delete es structure //
  if (es != NULL)
  {
    mem_free(es);
  }

  // close tcp connection //
  tcp_close(tpcb);
}

/**
  * @brief  Connects to the TCP echo server
  * @param  None
  * @retval None
  */
void tcp_client_connect(void)
{
  ip_addr_t DestIPaddr;

  /* create new tcp pcb */
  client_pcb = tcp_new();

  if (client_pcb != NULL)
  {
    IP4_ADDR( &DestIPaddr, g_arMainLanDestIpNum[0], g_arMainLanDestIpNum[1], g_arMainLanDestIpNum[2], g_arMainLanDestIpNum[3]);

    /* connect to destination address/port */
    tcp_connect(client_pcb,&DestIPaddr,g_deMainLanDestPortNum,tcp_client_connected);
  }
  else
  {
    /* deallocate the pcb */
    memp_free(MEMP_TCP_PCB, client_pcb);
#ifdef SERIAL_DEBUG
    printf("\n\r can not create tcp pcb");
#endif
  }
}

/**
  * @brief Function called when TCP connection established
  * @param tpcb: pointer on the connection contol block
  * @param err: when connection correctly established err should be ERR_OK
  * @retval err_t: returned error
  */
static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err)
{
  struct tcp_client *es = NULL;

  if (err == ERR_OK)
  {
    /* allocate structure es to maintain tcp connection informations */
    es = (struct tcp_client *)mem_malloc(sizeof(struct tcp_client));

    if (es != NULL)
    {
      es->state = ES_CLNT_CONNECTED;
      es->pcb = tpcb;

      /* allocate pbuf */
      es->p_tx = pbuf_alloc(PBUF_TRANSPORT, strlen((char*)data) , PBUF_POOL);

      if (es->p_tx)
      {
        /* copy data to pbuf */
        pbuf_take(es->p_tx, (char*)data, strlen((char*)data));

        /* pass newly allocated es structure as argument to tpcb */
        tcp_arg(tpcb, es);

        /* initialize LwIP tcp_recv callback function */
        tcp_recv(tpcb, tcp_client_recv);

        /* initialize LwIP tcp_sent callback function */
        tcp_sent(tpcb, tcp_client_sent);

        /* initialize LwIP tcp_poll callback function */
        tcp_poll(tpcb, tcp_client_poll, 1);

        /* send data */
        tcp_client_send(tpcb,es);

        return ERR_OK;
      }
    }
    else
    {
      /* close connection */
      tcp_client_connection_close(tpcb, es);

      /* return memory allocation error */
      return ERR_MEM;
    }
  }
  else
  {
    /* close connection */
    tcp_client_connection_close(tpcb, es);
  }
  return err;
}

/**
  * @brief tcp_receiv callback
  * @param arg: argument to be passed to receive callback
  * @param tpcb: tcp connection control block
  * @param err: receive error code
  * @retval err_t: retuned error
  */
static err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
  struct tcp_client *es;
  err_t ret_err;

  es = (struct tcp_client *)arg;

	u1f_connect_cnt = 0;
  /* if we receive an empty tcp frame from server => close connection */
  if (p == NULL)
  {
    /* remote host closed connection */
    es->state = ES_CLNT_CLOSING;
    if(es->p_tx == NULL)
    {
       /* we're done sending, close connection */
       tcp_client_connection_close(tpcb, es);
    }
    else
    {
      /* send remaining data*/
    	tcp_sent(client_pcb, tcp_client_sent);
      tcp_client_send(tpcb, es);
    }
    ret_err = ERR_OK;
  }
  /* else : a non empty frame was received from echo server but for some reason err != ERR_OK */
  else if(err != ERR_OK)
  {
    /* free received pbuf*/
    if (p != NULL)
    {
      pbuf_free(p);
    }
    ret_err = err;
  }
  else if(es->state == ES_CLNT_CONNECTED)
  {
    /* Acknowledge data reception */
    tcp_recved(tpcb, p->tot_len);
    ETH_CLNT_RECV();
	cs->p_tx = p;

    MEMCPY((void*)u1f_rxeth, (void*)cs->p_tx->payload, cs->p_tx->tot_len);

    pbuf_free(p);
//    tcp_client_connection_close(tpcb, es);
    ret_err = ERR_OK;
  }

  /* data received when connection already closed */
  else
  {
    /* Acknowledge data reception */
    tcp_recved(tpcb, p->tot_len);
    ETH_CLNT_RECV();

    /* free pbuf and do nothing */
    pbuf_free(p);
    ret_err = ERR_OK;
  }
  return ret_err;
}

/**
  * @brief function used to send data
  * @param  tpcb: tcp control block
  * @param  es: pointer on structure of type client containing info on data
  *             to be sent
  * @retval None
  */
static void tcp_client_send(struct tcp_pcb *tpcb, struct tcp_client * es)
{
  struct pbuf *ptr;
  err_t wr_err = ERR_OK;

  while ((wr_err == ERR_OK) && (es->p_tx != NULL) && (es->p_tx->len <= tcp_sndbuf(tpcb)))
  {

    /* get pointer on pbuf from es structure */
    ptr = es->p_tx;

    /* enqueue data for transmission */
    wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);//g_deEthClntLength

    if (wr_err == ERR_OK)
    {
      /* continue with next pbuf in chain (if any) */
      es->p_tx = ptr->next;

      if(es->p_tx != NULL)
      {
        /* increment reference count for es->p */
        pbuf_ref(es->p_tx);
      }

      /* free pbuf: will free pbufs up to es->p (because es->p has a reference count > 0) */
      pbuf_free(ptr);
   }
   else if(wr_err == ERR_MEM)
   {
      /* we are low on memory, try later, defer to poll */
     es->p_tx = ptr;
   }
   else
   {
     /* other problem ?? */
   }
  }
}

/**
  * @brief  This function implements the tcp_poll callback function
  * @param  arg: pointer on argument passed to callback
  * @param  tpcb: tcp connection control block
  * @retval err_t: error code
  */
static err_t tcp_client_poll(void *arg, struct tcp_pcb *tpcb)
{
  err_t ret_err;
  struct tcp_client *es;

  es = (struct tcp_client*)arg;
  if (es != NULL)
  {
    if (es->p_tx != NULL)
    {
      /* there is a remaining pbuf (chain) , try to send data */
      tcp_client_send(tpcb, es);
    }
    else
    {
      /* no remaining pbuf (chain)  */
      if(es->state == ES_CLNT_CLOSING)
      {
        /* close tcp connection */
        tcp_client_connection_close(tpcb, es);
      }
    }
    ret_err = ERR_OK;
  }
  else
  {
    /* nothing to be done */
    tcp_abort(tpcb);
    ret_err = ERR_ABRT;
  }
  return ret_err;
}

/**
  * @brief  This function implements the tcp_sent LwIP callback (called when ACK
  *         is received from remote host for sent data)
  * @param  arg: pointer on argument passed to callback
  * @param  tcp_pcb: tcp connection control block
  * @param  len: length of data sent
  * @retval err_t: returned error code
  */
static err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
  struct tcp_client *es;

  LWIP_UNUSED_ARG(len);

  es = (struct tcp_client *)arg;

  if(es->p_tx != NULL)
  {
    /* still got pbufs to send */
    tcp_client_send(tpcb, es);
  }

  return ERR_OK;
}

/**
  * @brief This function is used to close the tcp connection with server
  * @param tpcb: tcp connection control block
  * @param es: pointer on client structure
  * @retval None
  */
static void tcp_client_connection_close(struct tcp_pcb *tpcb, struct tcp_client * es )
{
  /* remove callbacks */
  tcp_recv(tpcb, NULL);
  tcp_sent(tpcb, NULL);
  tcp_poll(tpcb, NULL,0);

  if (es != NULL)
  {
    mem_free(es);
  }

  /* close tcp connection */
  tcp_close(tpcb);
}

void ETH_INIT(void)
{
	memset((void*)(g_deRtcDateBuf), 0, 4);
	memset((void*)(g_deRtcTimeBuf), 0, 4);
	memset((void*)(&PM_UART.Cnt), 0, sizeof(PM_UART.Cnt));
	memset((void*)(&PM_UART.Alarm), 0, sizeof(PM_UART.Alarm));
	memset((void*)(&TH_I2C.Temperature), 0, sizeof(TH_I2C.Temperature));
	memset((void*)(&TH_I2C.Humidity), 0, sizeof(TH_I2C.Humidity));

	memcpy(&ipaddr, g_arMainLanModIpNum, 4);
	memcpy(&netmask, g_arMainLanModSubNum, 4);
	memcpy(&gw, g_arMainLanModGwNum, 4);

	netif_set_addr(&gnetif, &ipaddr, &netmask, &gw);
	netif_set_default(&gnetif);

	if((g_deMainModeLanNum & 1) == 1)
	{
		// create new tcp pcb //
		tcp_server_pcb = tcp_new();

		if (tcp_server_pcb != NULL)
		{
			err_t err;

			// bind _pcb to port 7 ( protocol) //
			err = tcp_bind(tcp_server_pcb, IP_ADDR_ANY, g_deMainLanModPortNum);

			if (err == ERR_OK)
			{
				// start tcp listening for _pcb //
				tcp_server_pcb = tcp_listen(tcp_server_pcb);

				// initialize LwIP tcp_accept callback function //
				tcp_accept(tcp_server_pcb, tcp_server_accept);
			}
			else
			{
				// deallocate the pcb //
				memp_free(MEMP_TCP_PCB, tcp_server_pcb);
			}
		}
	}

	if((g_deMainModeLanNum & 2) == 2)
	{
		ip_addr_t DestIPaddr;

		client_pcb = tcp_new();

		if (client_pcb != NULL)
		{
			IP4_ADDR(&DestIPaddr, g_arMainLanDestIpNum[0], g_arMainLanDestIpNum[1], g_arMainLanDestIpNum[2], g_arMainLanDestIpNum[3]);

			tcp_connect(client_pcb, &DestIPaddr, g_deMainLanDestPortNum, tcp_client_connected);
		}
		else
		{
			/* deallocate the pcb */
			memp_free(MEMP_TCP_PCB, client_pcb);
		}
		u1t_connected_check = 1;

		u1f_eth_process_flag = 1;
	}
}

void ETH_SRVR_RECV(void)
{
	uint16_t l_deLength = 0, l_deChksum = 0, l_deChksumCnt = 0;

	if((g_arEthSrvrRxBuf[0] == 0x53) && (g_arEthSrvrRxBuf[3] == 0x02))	// ACT Protocol
	{
		l_deLength = ((((uint16_t)g_arEthSrvrRxBuf[1]) << 8) | (((uint16_t)g_arEthSrvrRxBuf[2]) & 0xff));

		for(l_deChksumCnt = 1; l_deChksumCnt < (l_deLength+3) ; l_deChksumCnt++)
		{
			l_deChksum += g_arEthSrvrRxBuf[l_deChksumCnt];
		}

		if(l_deChksum == ((((uint16_t)g_arEthSrvrRxBuf[l_deLength+3]) << 8) | (((uint16_t)g_arEthSrvrRxBuf[l_deLength+4]) & 0xff)))
		{
			switch(((((uint16_t)g_arEthSrvrRxBuf[4]) << 8) | (((uint16_t)g_arEthSrvrRxBuf[5]) & 0xff)))
			{
				case 0x0001 :
					sTime.Hours = (uint8_t)g_arEthSrvrRxBuf[11];
					sTime.Minutes = (uint8_t)g_arEthSrvrRxBuf[12];
					sTime.Seconds = (uint8_t)g_arEthSrvrRxBuf[13]-1;
					HAL_RTC_SetTime(&hrtc,&sTime,RTC_FORMAT_BIN);
					sDate.Year = (uint8_t)((((uint16_t)g_arEthSrvrRxBuf[8])<<8|(uint16_t)g_arEthSrvrRxBuf[7])%100);
					sDate.Month = (uint8_t)g_arEthSrvrRxBuf[9];
					sDate.Date = (uint8_t)g_arEthSrvrRxBuf[10];
					HAL_RTC_SetDate(&hrtc,&sDate,RTC_FORMAT_BIN);
					g_deEthSrvrCmdFlg = 0x01;
					break;
				case 0x0101 :
				case 0x001b :		// Commanded Read Particle Measurement Result
					g_deEthSrvrCmdFlg = 0x1b;
					break;
				case 0x001c :		// Commanded Open / Close Particle Measurement
					g_deEthSrvrCmdFlg = 0x1c;
					break;
				case 0x001e :		// Commanded Read Software Version Number
					g_deEthSrvrCmdFlg = 0x1e;
					break;
				case 0x001f :		// Commanded Read Serial Number
					g_deEthSrvrCmdFlg = 0x1f;
					break;
				default :
					g_deEthSrvrCmdFlg = 0x00;
					break;
			}
		}
	}

	g_deDataSrvrCmd = ((((uint16_t)g_arEthSrvrRxBuf[12]) << 8) | (((uint16_t)g_arEthSrvrRxBuf[11]) & 0xff));
	g_deDataSrvrDvld = ((((uint16_t)g_arEthSrvrRxBuf[16]) << 8) | (((uint16_t)g_arEthSrvrRxBuf[15]) & 0xff));
	g_deDataSrvrDvcd = g_arEthSrvrRxBuf[17];
	g_deDataSrvrDvct = (((((uint16_t)g_arEthSrvrRxBuf[20]) << 8) | (((uint16_t)g_arEthSrvrRxBuf[19]) & 0xff)) << 1)+2;	// (shift is word to byte)

	if(g_deDataSrvrCmd == 0x0401)
	{
		g_deEthSrvrCmdFlg = 0x4d;
		ETH_SRVR_TRANS();
		
		u1f_eth_cnd_select = 1;
	}

	g_deEthSrvrConnFlg = 1;
	g_deEthSrvrConnCnt = 0;
}

void ETH_SRVR_TRANS(void)
{
	static uint8_t l_deDataCnt = 0;
	static uint8_t Heartbit = 0;
	uint16_t l_deLength = 0, l_deChksum = 0, l_deChksumCnt = 0;

//	uint16_t temp = 0, humi = 0;//temporary

	if((g_deMainModeLanNum & 1) == 1)
	{
		if(g_deEthSrvrCmdFlg == 0x4d)
		{
			Heartbit = (~Heartbit & 0x80);

			// to FDC
			g_arEthSrvrTxBuf[0] = 0xD0;		// 서브헤더(L)
			g_arEthSrvrTxBuf[1] = 0x00;		// 서브헤더(H)
			g_arEthSrvrTxBuf[2] = 0x00;		// 네트워크번호
			g_arEthSrvrTxBuf[3] = 0xFF;		// PLC번호
			g_arEthSrvrTxBuf[4] = 0xFF;		// 요구상대모듈 I/O번호(L)
			g_arEthSrvrTxBuf[5] = 0x03;		// 요구상대모듈 I/O번호(H)
			g_arEthSrvrTxBuf[6] = 0x00;		// 요구상대모듈 국번호
			g_arEthSrvrTxBuf[7] = (g_deDataSrvrDvct & 0xff);		// 데이터길이(L) [9] ~ [마지막까지 갯수]
			g_arEthSrvrTxBuf[8] = (g_deDataSrvrDvct >> 8);		// 데이터길이(H)
			g_arEthSrvrTxBuf[9] = 0x00;		// 종료코드(L)
			g_arEthSrvrTxBuf[10] = 0x00;	// 종료코드(H)

			g_arEthSrvrTxBuf[11] = (uint8_t)(PM_UART.Cnt.Pm005 & 0xFF);								// DATA START
			g_arEthSrvrTxBuf[12] = (uint8_t)((PM_UART.Cnt.Pm005 >> 8) & 0xFF);
			g_arEthSrvrTxBuf[13] = (uint8_t)((PM_UART.Cnt.Pm005 >> 16) & 0xFF);
			g_arEthSrvrTxBuf[14] = (uint8_t)((PM_UART.Cnt.Pm005 >> 24) & 0xFF);
			g_arEthSrvrTxBuf[15] = 0x00;
			g_arEthSrvrTxBuf[16] = 0x00;
			g_arEthSrvrTxBuf[17] = 0x00;
			g_arEthSrvrTxBuf[18] = 0x00;
			g_arEthSrvrTxBuf[19] = 0;//((!BResult.bit.FanosStatErr << 1) & (!BResult.bit.FanusStatErr << 1))|(!BResult.bit.Uart2ConnErr << 2)
					//|((!BResult.bit.FanosStatErr) & (!BResult.bit.FanusStatErr) & (!BResult.bit.Uart2ConnErr));
			g_arEthSrvrTxBuf[20] = Heartbit;
			g_arEthSrvrTxBuf[21] = 0x00;
			g_arEthSrvrTxBuf[22] = 0x00;

			g_deEthSrvrLength = g_deDataSrvrDvct+9;

			// MC 프로토콜 선택 (전송 버퍼 선택)
			u1f_eth_cnd_select = 1;
		}
		g_deEthSrvrCmdFlg = 0;
	}
}

void ETH_CLNT_RECV(void)
{

	g_deEthClntConnFlg = 1;
	g_deEthClntConnCnt = 0;
}

void ETH_CLNT_TRANS(void)
{
	static uint8_t l_deUart7Heartbit = 0;
	l_deUart7Heartbit ^= 1;

	if((g_deMainModeLanNum & 2) == 2)
	{
		if(u1t_connected_check == 1)
		{
			if(g_deMainLanPtclNum == 1)
			{
				g_deDataClntDvct = 24;

				g_arEthClntTxBuf[0] = 0x50;							// 서브헤더(L)
				g_arEthClntTxBuf[1] = 0x00;							// 서브헤더(H)

				g_arEthClntTxBuf[2] = 0x00;							// 네트워크번호
				g_arEthClntTxBuf[3] = 0xFF;							// PLC번호
				g_arEthClntTxBuf[4] = 0xFF;							// 요구상대모듈 I/O번호(L)
				g_arEthClntTxBuf[5] = 0x03;							// 요구상대모듈 I/O번호(H)
				g_arEthClntTxBuf[6] = 0x00;							// 요구상대모듈 국번호
				g_arEthClntTxBuf[7] = g_deDataClntDvct & 0xff;		// 패킷 데이터 길이 [9] ~ [마지막까지 갯수]//24
				g_arEthClntTxBuf[8] = g_deDataClntDvct >> 8;		// 데이터길이(H)
				g_arEthClntTxBuf[9] = 0x10;							// CPU감시타이머(L)
				g_arEthClntTxBuf[10] = 0x00;						// CPU감시타이머(H)

				g_arEthClntTxBuf[11] = 0x01;						// 커맨드(L)
				g_arEthClntTxBuf[12] = 0x14;						// 커맨드(H)
				g_arEthClntTxBuf[13] = 0x00;						// 서브커맨드(L)	// 워드단위 쓰기
				g_arEthClntTxBuf[14] = 0x00;						// 서브커맨드(H)
				g_arEthClntTxBuf[15] = g_deMainLanDvldNum & 0xFF;	// 선두 디바이스(L)
				g_arEthClntTxBuf[16] = g_deMainLanDvldNum >> 8;		// 선두 디바이스(M)
				g_arEthClntTxBuf[17] = 0x00;						// 선두 디바이스(H)
				g_arEthClntTxBuf[18] = g_deMainLanDvcdNum;			// 디바이스 코드
				g_arEthClntTxBuf[19] = 0x06;						// 디바이스 점수(L) //전송 데이터 길이 LSB [21] ~ [마지막까지 갯수] : 단위 Word(2Byte)
				g_arEthClntTxBuf[20] = 0x00;						// 디바이스 점수(H) //전송 데이터 길이 MSB

				g_arEthClntTxBuf[21] = (uint8_t)(PM_UART.Cnt.Pm005 & 0xFF);
				g_arEthClntTxBuf[22] = (uint8_t)((PM_UART.Cnt.Pm005 >> 8) & 0xFF);
				g_arEthClntTxBuf[23] = (uint8_t)((PM_UART.Cnt.Pm005 >> 16) & 0xFF);
				g_arEthClntTxBuf[24] = (uint8_t)((PM_UART.Cnt.Pm005 >> 24) & 0xFF);
				g_arEthClntTxBuf[25] = 0x00;
				g_arEthClntTxBuf[26] = 0x00;
				g_arEthClntTxBuf[27] = 0x00;
				g_arEthClntTxBuf[28] = 0x00;
				//				g_arEthClntTxBuf[29] = (!BResult.bit.Uart2ConnErr << 3)
				//											| ((!BResult.bit.FanStatErr << 2))
				//											| (!BResult.bit.Pm005StatWarn << 1)
				//											| ((!BResult.bit.FanStatErr) & (!BResult.bit.Uart2ConnErr) & (!BResult.bit.Pm005StatWarn));
								g_arEthClntTxBuf[29] = (!BError.bit.PmConnErr << 3)
															| ((!BError.bit.FanAsErr << 2))
															| (!BAlarm.bit.Pm005OpAlm << 1)
															| ((!BError.bit.PmConnErr) & (!BError.bit.FanAsErr) & (!BAlarm.bit.Pm005OpAlm));
				g_arEthClntTxBuf[30] = l_deUart7Heartbit << 7;
				g_arEthClntTxBuf[31] = 0x00;
				g_arEthClntTxBuf[32] = 0x00;

				g_deEthClntLength = g_deDataClntDvct+9;

				//tcp_client_send(client_pcb, cs);
				tcp_write(client_pcb, g_arEthClntTxBuf, g_deEthClntLength,1);
				tcp_output(client_pcb);
			}
		}
		g_deEthClntCmdFlg = 0;
	}
}
/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
//이 줄 아래에는 응답 전송 관련 기능이 있습니다.
void _______Below_this_line_is_the_response_send_related_function(void)
{}

/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
static void tcp_protocal(struct tcp_pcb *tpcb, struct tcp_server *es)
{
	
	u2f_eth_cnt = 0;

	struct pbuf *ptr;

	ptr = es->p;

	MEMCPY((void*)u1f_eth_rx_buff, (void*)ptr->payload, ptr->tot_len);

	// STX / ETX 체크
	if((u1f_eth_rx_buff[0] != 0x53) &&
	(u1f_eth_rx_buff[ptr->tot_len - 1] != 0x45))
	{
		for(uint16_t i = 0; i < ptr->tot_len; i++)
		{
			u1f_eth_rx_buff[i] = 0;
		    es->p = NULL;
		}
	}
	
	// LENGTH 체크
	uint16_t u2t_length_chk = 0;
	
	u2t_length_chk = (uint16_t)(u1f_eth_rx_buff[1] << 8);
	u2t_length_chk |= (uint16_t)(u1f_eth_rx_buff[2] & 0xFF);
	
	if(u2t_length_chk != (ptr->tot_len -6))
	{
		for(uint16_t i = 0; i < ptr->tot_len; i++)
		{
			u1f_eth_rx_buff[i] = 0;
		    es->p = NULL;
		}
	}
	
	// TYPE 체크
	if(u1f_eth_rx_buff[3] != 0x02)
	{
		for(uint16_t i = 0; i < ptr->tot_len; i++)
		{
			u1f_eth_rx_buff[i] = 0;
		    es->p = NULL;
		}
	}
	
	// CHECK SUM 체크
	if(CHECK_SUM_OK != tcp_checksum_check(ptr->tot_len))
	{
		for(uint16_t i = 0; i < ptr->tot_len; i++)
		{
			u1f_eth_rx_buff[i] = 0;
		    es->p = NULL;
		}
	}
	
	
	// CMD 체크 //

	// 데이터 요청
	if(u1f_eth_rx_buff[4] == CMD_ID_MSG_DATA_REQ)
	{
		// 1. 파티클 데이터 요청
		if(u1f_eth_rx_buff[5] == CMD_CODE_1)
		{
			// 파티클센서모듈 데이터 요청 응답 [54Byte]
			tcp_protocal_data(tpcb, es);
			tcp_rcv_particle_data_request();
		}
		
		// 22. 이벤트 로그 요청
		if(u1f_eth_rx_buff[5] == CMD_CODE_22)
		{
			// 이벤트로그 ACK [1024Byte]
			tcp_protocal_log(tpcb, es);
		}
	}
	// 설정값 요청
	if(u1f_eth_rx_buff[4] == CMD_ID_MSG_CONFIG_REQ)
	{
		// 2. 현재 설정값 요청
		if(u1f_eth_rx_buff[5] == CMD_CODE_2)
		{
			// 파티클센서모듈 현재 설정값 응답
			tcp_protocal_para(tpcb, es);
		}
		// 23. 일련번호 요청
		else if(u1f_eth_rx_buff[5] == CMD_CODE_23)
		{
			// 일련번호 요청 응답 [----Byte]
			tcp_protocal_serial(tpcb, es);
		}
		
		// 24. 버전정보 요청
		else if(u1f_eth_rx_buff[5] == CMD_CODE_24)
		{
			// 버전정보 요청 응답 [----Byte]
			tcp_protocal_version(tpcb, es);
		}
		else;
	}
	
	// 제어 명령
	if(u1f_eth_rx_buff[4] == CMD_ID_MSG_SYS_CMD)
	{
		// 3. 모듈 재시작 명령
		if(u1f_eth_rx_buff[5] == CMD_CODE_3)
		{
			// 제어명령 응답
			tcp_protocal_rep(tpcb, es);
			
			HAL_Delay(200);
			
			NVIC_SystemReset();
		}
		
		// 4. 이벤트 로그 초기화
		else if(u1f_eth_rx_buff[5] == CMD_CODE_4)
		{
			// 제어명령 응답
			tcp_protocal_rep(tpcb, es);
			tcp_rcv_eventlog_init();
		}
		
		// 5. 모드 설정
		else if(u1f_eth_rx_buff[5] == CMD_CODE_5)
		{
			// 제어명령 응답
			tcp_protocal_rep(tpcb, es);
			tcp_rcv_mode_set();
		}
		
		// 6. 이물감지 기준 설정
		else if(u1f_eth_rx_buff[5] == CMD_CODE_6)
		{
			// 제어명령 응답
			tcp_protocal_rep(tpcb, es);
			tcp_rcv_alarm_set();
		}
		
		// 7. 소리 설정
		else if(u1f_eth_rx_buff[5] == CMD_CODE_7)
		{
			// 제어명령 응답
			tcp_protocal_rep(tpcb, es);
			tcp_rcv_sound_set();
		}
		
		// 8. 가동시간 기준 설정
		else if(u1f_eth_rx_buff[5] == CMD_CODE_8)
		{
			// 제어명령 응답
			tcp_protocal_rep(tpcb, es);
			tcp_rcv_lifetime_set();
		}
		
		// 9. RS485 통신 설정
		else if(u1f_eth_rx_buff[5] == CMD_CODE_9)
		{
			// 제어명령 응답
			tcp_protocal_rep(tpcb, es);
			tcp_rcv_rs485_set();
		}
		
		// 10. LAN 통신 SERVER 설정
		else if(u1f_eth_rx_buff[5] == CMD_CODE_10)
		{
			// 제어명령 응답
			tcp_protocal_rep(tpcb, es);
			tcp_rcv_lanserver_set();
		}
		
		// 11. LAN 통신 MAC/CLIENT 설정
		else if(u1f_eth_rx_buff[5] == CMD_CODE_11)
		{
			// 제어명령 응답
			tcp_protocal_rep(tpcb, es);
			tcp_rcv_lanmac_client_set();
		}
		
		// 12. WLAN 통신 SSID 설정
		else if(u1f_eth_rx_buff[5] == CMD_CODE_12)
		{
			// 제어명령 응답
			tcp_protocal_rep(tpcb, es);
			tcp_rcv_wlanssid_set();
		}
		
		// 13. WLAN 통신 PASS 설정
		else if(u1f_eth_rx_buff[5] == CMD_CODE_13)
		{
			// 제어명령 응답
			tcp_protocal_rep(tpcb, es);
			tcp_rcv_wlanpass_set();
		}
		
		// 14. WLAN 통신 SERVER 설정
		else if(u1f_eth_rx_buff[5] == CMD_CODE_14)
		{
			// 제어명령 응답
			tcp_protocal_rep(tpcb, es);
			tcp_rcv_wlanserver_set();
		}
		
		// 15. WLAN 통신 MAC/CLIENT 설정
		else if(u1f_eth_rx_buff[5] == CMD_CODE_15)
		{
			// 제어명령 응답
			tcp_protocal_rep(tpcb, es);
			tcp_rcv_wlanmac_client_set();
		}
		
		// 16. BLE 통신 SERVICE UUID 설정
		else if(u1f_eth_rx_buff[5] == CMD_CODE_16)
		{
			// 제어명령 응답
			tcp_protocal_rep(tpcb, es);
			tcp_rcv_bleservuuid_set();
		}
		
		// 17. BLE 통신 CHAR. UUID 설정
		else if(u1f_eth_rx_buff[5] == CMD_CODE_17)
		{
			// 제어명령 응답
			tcp_protocal_rep(tpcb, es);
			tcp_rcv_blecharuuid_set();
		}
		
		// 18. BLE 통신 SPARE UUID 설정
		else if(u1f_eth_rx_buff[5] == CMD_CODE_18)
		{
			// 제어명령 응답
			tcp_protocal_rep(tpcb, es);
			tcp_rcv_blespareuuid_set();
		}
		
		// 19. BLE 통신 PIN CODE 설정
		else if(u1f_eth_rx_buff[5] == CMD_CODE_19)
		{
			// 제어명령 응답
			tcp_protocal_rep(tpcb, es);
			tcp_rcv_blepincode_set();
		}
		
		// 20. 온습도감지 기준 설정
		else if(u1f_eth_rx_buff[5] == CMD_CODE_20)
		{
			// 제어명령 응답
			tcp_protocal_rep(tpcb, es);
			tcp_rcv_thalarm_set();
		}
		
		// 21. 온습도 보정치 설정
		else if(u1f_eth_rx_buff[5] == CMD_CODE_21)
		{
			// 제어명령 응답
			tcp_protocal_rep(tpcb, es);
			tcp_rcv_thscale_set();
		}
		// 25. 온습도 보정치 설정
		else if(u1f_eth_rx_buff[5] == CMD_CODE_25)
		{
			// 제어명령 응답
			tcp_protocal_rep(tpcb, es);
			
			u1f_bootjump_flag = 1;
		}
		else;
	}
}


// 파티클센서모듈 데이터 요청 응답
/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
static void tcp_protocal_data(struct tcp_pcb *tpcb, struct tcp_server *es)
{
	struct pbuf *ptr;

	ptr = es->p;
	
	uint16_t u2t_chksum = 0;
	
	// STX = 0x53
	u1f_eth_tx_buff[u2f_eth_cnt++] = STX_HEAD;

	// LENTGH	[58Byte] + [4Byte] = [62Byte]
	u1f_eth_tx_buff[u2f_eth_cnt++] = 0x00;
	u1f_eth_tx_buff[u2f_eth_cnt++] = 0x3E;

	// TYPE 	파티클 = 0x02
	u1f_eth_tx_buff[u2f_eth_cnt++] = TYPE_PARTICLE;

	// CMD
	u1f_eth_tx_buff[u2f_eth_cnt++] = CMD_ID_MSG_CTRL_DATA;
	u1f_eth_tx_buff[u2f_eth_cnt++] = ACK_CODE_1;

	// INDEX Count
	u1f_eth_tx_buff[u2f_eth_cnt++] = u1f_eth_index;

	//↓↓↓ DATA [58 byte] ↓↓↓//
	
	// ID 
	st_data_ack.Id[0] = g_deMain485ModNum & 0x1f;
	u1f_eth_tx_buff[u2f_eth_cnt++] = st_data_ack.Id[0];
	
	// Date[3] - 1byte * 3
	for(uint8_t i = 0; i < 3; i ++)
	{
		st_data_ack.Date[i] = g_deRtcDateBuf[i];
		u1f_eth_tx_buff[u2f_eth_cnt++] = st_data_ack.Date[i];
	}
	
	// Time[4] - 1byte * 4
	for(uint8_t i = 0; i < 4; i ++)
	{
		st_data_ack.Time[i] = g_deRtcTimeBuf[i];
		u1f_eth_tx_buff[u2f_eth_cnt++] = st_data_ack.Time[i];
	}
		
	// UpTime[4] - 2byte * 4
	for(uint8_t i = 0; i < 4; i ++)
	{
		st_data_ack.Uptime[i] = g_arRtcUptime[i];
		u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_data_ack.Uptime[i] >> 8);
		u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_data_ack.Uptime[i] & 0xFF);
	}
	
	// PM_Data[6]
	st_data_ack.Pm[0] = PM_UART.Cnt.Pm003;
	st_data_ack.Pm[1] = PM_UART.Cnt.Pm005;
	st_data_ack.Pm[2] = PM_UART.Cnt.Pm010;
	st_data_ack.Pm[3] = PM_UART.Cnt.Pm025;
	st_data_ack.Pm[4] = PM_UART.Cnt.Pm050;
	st_data_ack.Pm[5] = PM_UART.Cnt.Pm100;
	
	// PM_Data[6] - 4byte * 6
	for(uint8_t h = 0; h < 6; h ++)
	{
		u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)((st_data_ack.Pm[h] >> 24) & 0xFF);
		u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)((st_data_ack.Pm[h] >> 16) & 0xFF);
		u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)((st_data_ack.Pm[h] >> 8) & 0xFF);
		u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_data_ack.Pm[h] & 0xFF);
	}
	
	// Status - 2byte
	st_data_ack.Status.u2t_status = BStatus.all;
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_data_ack.Status.u2t_status >> 8);
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_data_ack.Status.u2t_status & 0xFF);
	
	// Alarm - 2byte
	st_data_ack.Alarm.u2t_alarm = BAlarm.all;
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_data_ack.Alarm.u2t_alarm >> 8);
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_data_ack.Alarm.u2t_alarm & 0xFF);
	                                 
	// Error - 2byte
	st_data_ack.Error.u2t_error = BError.all;
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_data_ack.Error.u2t_error >> 8);
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_data_ack.Error.u2t_error & 0xFF);
	
	// Dio - 2byte
	st_data_ack.Dio.u2t_dio = BDio.all;
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_data_ack.Dio.u2t_dio >> 8);
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_data_ack.Dio.u2t_dio & 0xFF);
	
	// Reserved - 2byte * 5
	for(uint8_t k = 0; k < 5; k ++)
	{
		u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_data_ack.Reserved[k] >> 8);
		u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_data_ack.Reserved[k] & 0xFF);
	}

	//↑↑↑ DATA [58 byte] ↑↑↑//
	
	// Check sum
	u2t_chksum = tcp_checksum_creat(u2f_eth_cnt);
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)((u2t_chksum >> 8) & 0xFF);
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(u2t_chksum & 0xFF);

	// ETX
	u1f_eth_tx_buff[u2f_eth_cnt++] = 0x45;

	// INDEX 생성
	if(u1f_eth_index == 0xFF) {
		u1f_eth_index = 0;
	} else {
		u1f_eth_index++;
	}

	ptr->payload = u1f_eth_tx_buff;
	ptr->len = u2f_eth_cnt;

	u1f_eth_cnd_select = 2;

	tcp_server_send(tpcb, es);
}


// 파티클센서모듈 현재 설정값 응답
/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
static void tcp_protocal_para(struct tcp_pcb *tpcb, struct tcp_server *es)
{
	struct pbuf *ptr;

	ptr = es->p;
	
	uint16_t u2t_chksum = 0;
	
	// STX = 0x53
	u1f_eth_tx_buff[u2f_eth_cnt++] = STX_HEAD;

	// LENTGH	[208Byte] + [4Byte] = [212Byte]
	u1f_eth_tx_buff[u2f_eth_cnt++] = 0x00;
	u1f_eth_tx_buff[u2f_eth_cnt++] = 0xD4;

	// TYPE 	파티클 = 0x02
	u1f_eth_tx_buff[u2f_eth_cnt++] = TYPE_PARTICLE;

	// CMD
	u1f_eth_tx_buff[u2f_eth_cnt++] = CMD_ID_MSG_CTRL_PARA;
	u1f_eth_tx_buff[u2f_eth_cnt++] = ACK_CODE_2;

	// INDEX Count
	u1f_eth_tx_buff[u2f_eth_cnt++] = u1f_eth_index;

	//↓↓↓ DATA [206 byte] ↓↓↓//
	
	st_para_ack.Mode_AlarmEnable = g_deMainModeAlarmNum;
	st_para_ack.Mode_IoEnable = g_deMainModeIoNum;
	st_para_ack.Mode_485Enable = g_deMainMode485Num;
	st_para_ack.Mode_LanEnable = g_deMainModeLanNum;
	st_para_ack.Mode_WlssEnable = g_deMainModeThNum;
	st_para_ack.Mode_ThEnable = g_deMainModeWlssNum;
	st_para_ack.Mode_SmaEnable = g_deMainModeSmaNum;
	st_para_ack.Mode_DefEnable = g_deMainModeDefNum;
	
	st_para_ack.Pm_Alarm003Refer = g_arMainAlarmNum[1];
	st_para_ack.Pm_Alarm005Refer = g_arMainAlarmNum[2];
	st_para_ack.Pm_Alarm010Refer = g_arMainAlarmNum[3];
	st_para_ack.Pm_Alarm025Refer = g_arMainAlarmNum[4];
	st_para_ack.Pm_Alarm050Refer = g_arMainAlarmNum[5];
	st_para_ack.Pm_Alarm100Refer = g_arMainAlarmNum[6];
	
	st_para_ack.Sound_Type		= g_deMainSndTypNum;
	st_para_ack.Sound_Volume	= g_deMainSndVlmNum;
	
	st_para_ack.Dt_AlarmUptimeRefer = g_deMainLifetimeNum;
	
	st_para_ack.Rs485_BaudRate	=	g_deMain485BaudNum;
	st_para_ack.Rs485_DataSize	=	g_deMain485DataNum;
	st_para_ack.Rs485_Paritybit	=	g_deMain485PrtyNum;
	st_para_ack.Rs485_Stopbit	=	g_deMain485StopNum;
	st_para_ack.Rs485_Flowctrl	=	g_deMain485FlowNum;
	st_para_ack.Rs485_ModID		=	g_deMain485ModNum;
	st_para_ack.Rs485_DestID	=	g_deMain485DestNum;
	st_para_ack.Rs485_Protocol	=	g_deMain485PtclNum;
	st_para_ack.Rs485_Devcode	=	g_deMain485DvcdNum;
	st_para_ack.Rs485_Leaddev	=	g_deMain485DvldNum;
	
	memcpy(st_para_ack.Lan_ModIpAddr, g_arMainLanModIpNum,4);
	memcpy(st_para_ack.Lan_ModSubnet, g_arMainLanModSubNum,4);
	memcpy(st_para_ack.Lan_ModGateway, g_arMainLanModGwNum,4);
	st_para_ack.Lan_ModPort			= g_deMainLanModPortNum;
	memcpy(st_para_ack.Lan_ModMacAddr, g_arMainLanModMacNum,6);
	memcpy(st_para_ack.Lan_DestIpAddr, g_arMainLanDestIpNum,4);
	st_para_ack.Lan_DestPort		= g_deMainLanDestPortNum;
	st_para_ack.Lan_Protocol		= g_deMainLanPtclNum;
	st_para_ack.Lan_Devcode			= g_deMainLanDvcdNum;
	st_para_ack.Lan_Leaddev			= g_deMainLanDvldNum;
	
	
	st_para_ack.Th_AlarmOverTempWarnRefer = g_deMainThOtWrnNum;
	st_para_ack.Th_AlarmOverTempErrRefer = 	g_deMainThOtErrNum;
	st_para_ack.Th_AlarmOverHumiErrRefer = 	g_deMainThOhErrNum;
	st_para_ack.Th_TempSlope			 = 	g_deMainThTempSlpNum;
	st_para_ack.Th_TempOffset			 = 	g_deMainThTempOffNum;
	st_para_ack.Th_HumiSlope			 = 	g_deMainThHumiSlpNum;
	st_para_ack.Th_HumiOffset			 = 	g_deMainThHumiOffNum;
	
	
	
	u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Mode_AlarmEnable;									
	u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Mode_IoEnable;										
	u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Mode_485Enable;									
	u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Mode_LanEnable;									
	u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Mode_WlssEnable;									
	u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Mode_ThEnable;										
	u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Mode_SmaEnable;									
	u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Mode_DefEnable;									
	

	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Pm_Alarm003Refer >> 8);					
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Pm_Alarm003Refer & 0xFF);				

	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Pm_Alarm005Refer >> 8);					
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Pm_Alarm005Refer & 0xFF);				

	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Pm_Alarm010Refer >> 8);					
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Pm_Alarm010Refer & 0xFF);				
	
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Pm_Alarm025Refer >> 8);					
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Pm_Alarm025Refer & 0xFF);				

	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Pm_Alarm050Refer >> 8);					
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Pm_Alarm050Refer & 0xFF);				

	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Pm_Alarm100Refer >> 8);					
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Pm_Alarm100Refer & 0xFF);				


	u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Sound_Type;										
	u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Sound_Volume;										


	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)((st_para_ack.Dt_AlarmUptimeRefer >> 24) & 0xFF);		
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)((st_para_ack.Dt_AlarmUptimeRefer >> 16) & 0xFF);		
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)((st_para_ack.Dt_AlarmUptimeRefer >> 8) & 0xFF);		
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Dt_AlarmUptimeRefer & 0xFF);				


	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)((st_para_ack.Rs485_BaudRate >> 24) & 0xFF);			
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)((st_para_ack.Rs485_BaudRate >> 16) & 0xFF);			
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)((st_para_ack.Rs485_BaudRate >> 8) & 0xFF);			
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Rs485_BaudRate & 0xFF);					

	u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Rs485_DataSize;									
	u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Rs485_Paritybit;									
	u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Rs485_Stopbit;										
	u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Rs485_Flowctrl;									
	u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Rs485_Protocol;									
	u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Rs485_Devcode;										
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Rs485_Leaddev >> 8);						
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Rs485_Leaddev & 0xFF);					
	u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Rs485_ModID;										
	u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Rs485_DestID;										


	for(uint8_t i = 0; i < 4; i++)
		u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Lan_ModIpAddr[i];								
	for(uint8_t i = 0; i < 4; i++)
		u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Lan_ModSubnet[i];								
	for(uint8_t i = 0; i < 4; i++)
		u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Lan_ModGateway[i];								
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Lan_ModPort >> 8);						
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Lan_ModPort & 0xFF);						
	for(uint8_t i = 0; i < 6; i++)
		u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Lan_ModMacAddr[i];								
	for(uint8_t i = 0; i < 4; i++)
		u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Lan_DestIpAddr[i];								
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Lan_DestPort >> 8);						
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Lan_DestPort & 0xFF);					
	u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Lan_Protocol;										
	u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Lan_Devcode;										
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Lan_Leaddev >> 8);						
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Lan_Leaddev & 0xFF);						


	for(uint8_t i = 0; i < 16; i++)
		u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Wifi_Ssid[i];									
	for(uint8_t i = 0; i < 16; i++)
		u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Wifi_Password[i];								
	for(uint8_t i = 0; i < 4; i++)
		u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Wifi_ModIpAddr[i];								
	for(uint8_t i = 0; i < 4; i++)
		u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Wifi_ModSubnet[i];								
	for(uint8_t i = 0; i < 4; i++)
		u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Wifi_ModGateway[i];							
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Wifi_ModPort >> 8);						
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Wifi_ModPort & 0xFF);					
	for(uint8_t i = 0; i < 6; i++)
		u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Wifi_ModMacAddr[i];							
	for(uint8_t i = 0; i < 4; i++)
		u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Wifi_DestIpAddr[i];							
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Wifi_DestPort >> 8);						
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Wifi_DestPort & 0xFF);					
	u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Wifi_Protocol;										
	u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Wifi_Devcode;										
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Wifi_Leaddev >> 8);						
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Wifi_Leaddev & 0xFF);					


	for(uint8_t i = 0; i < 16; i++)
		u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Ble_ServUuid[i];								
	for(uint8_t i = 0; i < 16; i++)
		u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Ble_CharUuid[i];								
	for(uint8_t i = 0; i < 16; i++)
		u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Ble_SpareUuid[i];								
	for(uint8_t i = 0; i < 16; i++)
		u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Ble_Pincode[i];								

	u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Th_AlarmOverTempWarnRefer;							
	u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Th_AlarmOverTempErrRefer;							
	u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Th_AlarmOverHumiErrRefer;							
	u1f_eth_tx_buff[u2f_eth_cnt++] = st_para_ack.Th_Reserved;										
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Th_TempSlope >> 8);						
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Th_TempSlope & 0xFF);					
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Th_TempOffset >> 8);						
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Th_TempOffset & 0xFF);					
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Th_HumiSlope >> 8);						
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Th_HumiSlope & 0xFF);					
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Th_HumiOffset >> 8);						
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(st_para_ack.Th_HumiOffset & 0xFF);					
	
	//↑↑↑ DATA [206 byte] ↑↑↑//
	
	// Check sum
	u2t_chksum = tcp_checksum_creat(u2f_eth_cnt);
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)((u2t_chksum >> 8) & 0xFF);
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(u2t_chksum & 0xFF);

	// ETX
	u1f_eth_tx_buff[u2f_eth_cnt++] = 0x45;

	// INDEX 생성
	if(u1f_eth_index == 0xFF) {
		u1f_eth_index = 0;
	} else {
		u1f_eth_index++;
	}

	ptr->payload = u1f_eth_tx_buff;
	ptr->len = u2f_eth_cnt;

	u1f_eth_cnd_select = 2;
	
	tcp_server_send(tpcb, es);
}

// 제어명령 응답
/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
static void tcp_protocal_rep(struct tcp_pcb *tpcb, struct tcp_server *es)
{
	struct pbuf *ptr;

	ptr = es->p;
	
	uint16_t u2t_chksum = 0;
	
	// STX = 0x53
	u1f_eth_tx_buff[u2f_eth_cnt++] = STX_HEAD;

	// LENTGH	[8Byte] + [4Byte] = [12Byte]
	u1f_eth_tx_buff[u2f_eth_cnt++] = 0x00;
	u1f_eth_tx_buff[u2f_eth_cnt++] = 0x0C;

	// TYPE 	파티클 = 0x02
	u1f_eth_tx_buff[u2f_eth_cnt++] = TYPE_PARTICLE;

	// CMD
	u1f_eth_tx_buff[u2f_eth_cnt++] = CMD_ID_MSG_CTRL_REP;
	u1f_eth_tx_buff[u2f_eth_cnt++] = ACK_CODE_3;

	// INDEX Count
	u1f_eth_tx_buff[u2f_eth_cnt++] = u1f_eth_index;

	//↓↓↓ DATA [8 byte] ↓↓↓//
	u1f_eth_tx_buff[u2f_eth_cnt++] = 0;		// (Response code : 0-Done, 1-Error)
	u1f_eth_tx_buff[u2f_eth_cnt++] = 0;
	u1f_eth_tx_buff[u2f_eth_cnt++] = 0;
	u1f_eth_tx_buff[u2f_eth_cnt++] = 0;
	u1f_eth_tx_buff[u2f_eth_cnt++] = 0;
	u1f_eth_tx_buff[u2f_eth_cnt++] = 0;
	u1f_eth_tx_buff[u2f_eth_cnt++] = 0;
	u1f_eth_tx_buff[u2f_eth_cnt++] = 0;
	//↑↑↑ DATA [8 byte] ↑↑↑//
	
	// Check sum
	u2t_chksum = tcp_checksum_creat(u2f_eth_cnt);
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)((u2t_chksum >> 8) & 0xFF);
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(u2t_chksum & 0xFF);

	// ETX
	u1f_eth_tx_buff[u2f_eth_cnt++] = 0x45;

	// INDEX 생성
	if(u1f_eth_index == 0xFF) {
		u1f_eth_index = 0;
	} else {
		u1f_eth_index++;
	}

	ptr->payload = u1f_eth_tx_buff;
	ptr->len = u2f_eth_cnt;

	u1f_eth_cnd_select = 2;
	
	tcp_server_send(tpcb, es);
}

// 이벤트 로그 요청 응답
/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
static void tcp_protocal_log(struct tcp_pcb *tpcb, struct tcp_server *es)
{
	struct pbuf *ptr;

	ptr = es->p;
	
	uint16_t u2t_chksum = 0;
	
	// STX = 0x53
	u1f_eth_tx_buff[u2f_eth_cnt++] = STX_HEAD;

	// LENTGH	[1024Byte] + [4Byte] = [1028yte]
	u1f_eth_tx_buff[u2f_eth_cnt++] = 0x04;
	u1f_eth_tx_buff[u2f_eth_cnt++] = 0x04;

	// TYPE 	파티클 = 0x02
	u1f_eth_tx_buff[u2f_eth_cnt++] = TYPE_PARTICLE;

	// CMD
	u1f_eth_tx_buff[u2f_eth_cnt++] = CMD_ID_MSG_CTRL_DATA;
	u1f_eth_tx_buff[u2f_eth_cnt++] = ACK_CODE_4;

	// INDEX Count
	u1f_eth_tx_buff[u2f_eth_cnt++] = u1f_eth_index;

	//↓↓↓ DATA [1024 byte] ↓↓↓//
	
	// LOG DATA [128][8]
	for(uint8_t i = 0; i < 128; i ++)
	{
		for(uint8_t j = 0; j < 8; j ++)
		{
			
			u1f_eth_tx_buff[u2f_eth_cnt++] = log_data_ack.log_num[i][j];
		}
	}
	
	//↑↑↑ DATA [1024 byte] ↑↑↑//
	
	// Check sum
	u2t_chksum = tcp_checksum_creat(u2f_eth_cnt);
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)((u2t_chksum >> 8) & 0xFF);
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(u2t_chksum & 0xFF);

	// ETX
	u1f_eth_tx_buff[u2f_eth_cnt++] = 0x45;

	// INDEX 생성
	if(u1f_eth_index == 0xFF) {
		u1f_eth_index = 0;
	} else {
		u1f_eth_index++;
	}

	ptr->payload = u1f_eth_tx_buff;
	ptr->len = u2f_eth_cnt;

	u1f_eth_cnd_select = 2;
	
	tcp_server_send(tpcb, es);
}

// 일련번호 응답
/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
static void tcp_protocal_serial(struct tcp_pcb *tpcb, struct tcp_server *es)
{
	struct pbuf *ptr;

	ptr = es->p;
	
	uint16_t u2t_chksum = 0;
	
	// STX = 0x53
	u1f_eth_tx_buff[u2f_eth_cnt++] = STX_HEAD;

	// LENTGH	[20Byte] + [4Byte] = [24Byte]
	u1f_eth_tx_buff[u2f_eth_cnt++] = 0x00;
	u1f_eth_tx_buff[u2f_eth_cnt++] = 0x18;

	// TYPE 	파티클 = 0x02
	u1f_eth_tx_buff[u2f_eth_cnt++] = TYPE_PARTICLE;

	// CMD
	u1f_eth_tx_buff[u2f_eth_cnt++] = CMD_ID_MSG_CTRL_PARA;
	u1f_eth_tx_buff[u2f_eth_cnt++] = ACK_CODE_5;

	// INDEX Count
	u1f_eth_tx_buff[u2f_eth_cnt++] = u1f_eth_index;

	//↓↓↓ DATA [20 byte] ↓↓↓//
	
	for(uint8_t i = 0; i < 10; i++)
		u1f_eth_tx_buff[u2f_eth_cnt++] = serial_num_ack.Sn_Module[i];
		
	for(uint8_t i = 0; i < 5; i++)
	{
		u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(serial_num_ack.Sn_Sensor[i] >> 8);
		u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(serial_num_ack.Sn_Sensor[i] & 0xFF);
	}
	//↑↑↑ DATA [20 byte] ↑↑↑//
	
	// Check sum
	u2t_chksum = tcp_checksum_creat(u2f_eth_cnt);
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)((u2t_chksum >> 8) & 0xFF);
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(u2t_chksum & 0xFF);

	// ETX
	u1f_eth_tx_buff[u2f_eth_cnt++] = 0x45;

	// INDEX 생성
	if(u1f_eth_index == 0xFF) {
		u1f_eth_index = 0;
	} else {
		u1f_eth_index++;
	}

	ptr->payload = u1f_eth_tx_buff;
	ptr->len = u2f_eth_cnt;
	
	tcp_server_send(tpcb, es);
}

// 버전정보 응답
/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
static void tcp_protocal_version(struct tcp_pcb *tpcb, struct tcp_server *es)
{
	struct pbuf *ptr;

	ptr = es->p;
	
	uint16_t u2t_chksum = 0;
	
	// STX = 0x53
	u1f_eth_tx_buff[u2f_eth_cnt++] = STX_HEAD;

	// LENTGH	[20Byte] + [4Byte] = [24Byte]
	u1f_eth_tx_buff[u2f_eth_cnt++] = 0x00;
	u1f_eth_tx_buff[u2f_eth_cnt++] = 0x18;

	// TYPE 	파티클 = 0x02
	u1f_eth_tx_buff[u2f_eth_cnt++] = TYPE_PARTICLE;

	// CMD
	u1f_eth_tx_buff[u2f_eth_cnt++] = CMD_ID_MSG_CTRL_PARA;
	u1f_eth_tx_buff[u2f_eth_cnt++] = ACK_CODE_6;

	// INDEX Count
	u1f_eth_tx_buff[u2f_eth_cnt++] = u1f_eth_index;

	//↓↓↓ DATA [20 byte] ↓↓↓//
	
	for(uint8_t i = 0; i < 10; i++)
		u1f_eth_tx_buff[u2f_eth_cnt++] = version_ack.Ver_ModuleSW[i];
		
	for(uint8_t i = 0; i < 10; i++)
		u1f_eth_tx_buff[u2f_eth_cnt++] = version_ack.Ver_SensorSW[i];

	//↑↑↑ DATA [20 byte] ↑↑↑//
	
	// Check sum
	u2t_chksum = tcp_checksum_creat(u2f_eth_cnt);
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)((u2t_chksum >> 8) & 0xFF);
	u1f_eth_tx_buff[u2f_eth_cnt++] = (uint8_t)(u2t_chksum & 0xFF);

	// ETX
	u1f_eth_tx_buff[u2f_eth_cnt++] = 0x45;

	// INDEX 생성
	if(u1f_eth_index == 0xFF) {
		u1f_eth_index = 0;
	} else {
		u1f_eth_index++;
	}
	
	ptr->payload = u1f_eth_tx_buff;
	ptr->len = u2f_eth_cnt;

	u1f_eth_cnd_select = 2;
	
	tcp_server_send(tpcb, es);
}

/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
void _______Below_this_line_is_functions_related_to_the_command_structure(void)
{}

// 수신 패킷 분석 
// 1. 파티클 데이터 요청
/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
static void tcp_rcv_particle_data_request(void)
{
	st_rcv_cmd1.u2t_year	= ((uint16_t)u1f_eth_rx_buff[7] << 8) |
	                          ((uint16_t)u1f_eth_rx_buff[8]);
	                           
	st_rcv_cmd1.u1t_month	= u1f_eth_rx_buff[9];
	st_rcv_cmd1.u1t_day		= u1f_eth_rx_buff[10];
	st_rcv_cmd1.u1t_hour	= u1f_eth_rx_buff[11];
	st_rcv_cmd1.u1t_minute	= u1f_eth_rx_buff[12];
	st_rcv_cmd1.ut_second	= u1f_eth_rx_buff[13];
}

// 4. 이벤트 로그 초기화
/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
static void tcp_rcv_eventlog_init(void)
{
	memset(CPU_I2C.LogData, 0xff, 1024);

	g_arI2c1TxBuf[1] = CPU_I2C.ClearFlag;
	memcpy(&g_arI2c1TxBuf[20], &CPU_I2C.LogCnt, 1);
	memcpy(&g_arI2c1TxBuf[256], &CPU_I2C.LogData, 1024);

	HAL_I2C_Mem_Write(&hi2c1, 0xa1, 1, 2, g_arI2c1TxBuf+1, 1, 100);				// ClearFlag
	HAL_Delay(10);
	HAL_I2C_Mem_Write(&hi2c1, 0xa1, 20, 2, g_arI2c1TxBuf+20, 1, 100);			// LogCnt
	HAL_Delay(10);

	HAL_I2C_Mem_Write(&hi2c1, 0xa1, 256, 2, g_arI2c1TxBuf+256, 64, 100);		// LogData0
	HAL_Delay(10);
	HAL_I2C_Mem_Write(&hi2c1, 0xa1, 320, 2, g_arI2c1TxBuf+320, 64, 100);		// LogData1
	HAL_Delay(10);
	HAL_I2C_Mem_Write(&hi2c1, 0xa1, 384, 2, g_arI2c1TxBuf+384, 64, 100);		// LogData2
	HAL_Delay(10);
	HAL_I2C_Mem_Write(&hi2c1, 0xa1, 448, 2, g_arI2c1TxBuf+448, 64, 100);		// LogData3
	HAL_Delay(10);

	HAL_I2C_Mem_Write(&hi2c1, 0xa1, 512, 2, g_arI2c1TxBuf+512, 64, 100);		// LogData4
	HAL_Delay(10);
	HAL_I2C_Mem_Write(&hi2c1, 0xa1, 576, 2, g_arI2c1TxBuf+576, 64, 100);		// LogData5
	HAL_Delay(10);
	HAL_I2C_Mem_Write(&hi2c1, 0xa1, 640, 2, g_arI2c1TxBuf+640, 64, 100);		// LogData6
	HAL_Delay(10);
	HAL_I2C_Mem_Write(&hi2c1, 0xa1, 704, 2, g_arI2c1TxBuf+704, 64, 100);		// LogData7
	HAL_Delay(10);

	HAL_I2C_Mem_Write(&hi2c1, 0xa1, 768, 2, g_arI2c1TxBuf+768, 64, 100);		// LogData8
	HAL_Delay(10);
	HAL_I2C_Mem_Write(&hi2c1, 0xa1, 832, 2, g_arI2c1TxBuf+832, 64, 100);		// LogData9
	HAL_Delay(10);
	HAL_I2C_Mem_Write(&hi2c1, 0xa1, 896, 2, g_arI2c1TxBuf+896, 64, 100);		// LogData10
	HAL_Delay(10);
	HAL_I2C_Mem_Write(&hi2c1, 0xa1, 960, 2, g_arI2c1TxBuf+960, 64, 100);		// LogData11
	HAL_Delay(10);

	HAL_I2C_Mem_Write(&hi2c1, 0xa1, 1024, 2, g_arI2c1TxBuf+1024, 64, 100);		// LogData8
	HAL_Delay(10);
	HAL_I2C_Mem_Write(&hi2c1, 0xa1, 1088, 2, g_arI2c1TxBuf+1088, 64, 100);		// LogData9
	HAL_Delay(10);
	HAL_I2C_Mem_Write(&hi2c1, 0xa1, 1152, 2, g_arI2c1TxBuf+1152, 64, 100);		// LogData10
	HAL_Delay(10);
	HAL_I2C_Mem_Write(&hi2c1, 0xa1, 1216, 2, g_arI2c1TxBuf+1216, 64, 100);		// LogData11
	HAL_Delay(10);
}

// 5. 모드 설정
/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
static void tcp_rcv_mode_set(void)
{
	st_rcv_cmd5.Mode_AlarmEnable 	= u1f_eth_rx_buff[7];
	st_rcv_cmd5.Mode_IOEnable 		= u1f_eth_rx_buff[8];
	st_rcv_cmd5.Mode_485Enable 		= u1f_eth_rx_buff[9];
	st_rcv_cmd5.Mode_LanEnable 		= u1f_eth_rx_buff[10];
	st_rcv_cmd5.Mode_WlssEnable 	= u1f_eth_rx_buff[11];
	st_rcv_cmd5.Mode_ThEnable 		= u1f_eth_rx_buff[12];
	st_rcv_cmd5.Mode_SmaEnable 		= u1f_eth_rx_buff[13];
	st_rcv_cmd5.Mode_DefEnable 		= u1f_eth_rx_buff[14];
	
	g_deMainModeAlarmNum			= st_rcv_cmd5.Mode_AlarmEnable;
	g_deMainModeIoNum				= st_rcv_cmd5.Mode_IOEnable;
	g_deMainMode485Num				= st_rcv_cmd5.Mode_485Enable;
	g_deMainModeLanNum				= st_rcv_cmd5.Mode_LanEnable;
	g_deMainModeThNum				= st_rcv_cmd5.Mode_WlssEnable;
	g_deMainModeWlssNum				= st_rcv_cmd5.Mode_ThEnable;
	g_deMainModeSmaNum				= st_rcv_cmd5.Mode_SmaEnable;
	g_deMainModeDefNum				= st_rcv_cmd5.Mode_DefEnable;
	
	CPU_I2C.ModeAlarm = g_deMainModeAlarmNum;
	CPU_I2C.ModeIo = g_deMainModeIoNum;
	CPU_I2C.Mode485 = g_deMainMode485Num;
	CPU_I2C.ModeLan = g_deMainModeLanNum;
	CPU_I2C.ModeWlss = g_deMainModeWlssNum;
	CPU_I2C.ModeTh = g_deMainModeThNum;
	CPU_I2C.ModeSma = g_deMainModeSmaNum;
	CPU_I2C.ModeDef = g_deMainModeDefNum;
	
	I2C1_WRITE();

	// 화면갱신 추가
	g_deRedrawingFlg = 1;
}

// 6. 이물감지 기준 설정
/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
static void tcp_rcv_alarm_set(void)
{
	// uint16_t형 변환 저장
	st_rcv_cmd6.Pm_Alarm003Refer = ((uint16_t)u1f_eth_rx_buff[7] << 8) |
	                               ((uint16_t)u1f_eth_rx_buff[8]);
	                               
	st_rcv_cmd6.Pm_Alarm005Refer = ((uint16_t)u1f_eth_rx_buff[9] << 8) |
	                               ((uint16_t)u1f_eth_rx_buff[10]);
	                               
	st_rcv_cmd6.Pm_Alarm010Refer = ((uint16_t)u1f_eth_rx_buff[11] << 8) |
	                               ((uint16_t)u1f_eth_rx_buff[12]);
	                               
	st_rcv_cmd6.Pm_Alarm025Refer = ((uint16_t)u1f_eth_rx_buff[13] << 8) |
	                               ((uint16_t)u1f_eth_rx_buff[14]);
	                               
	st_rcv_cmd6.Pm_Alarm050Refer = ((uint16_t)u1f_eth_rx_buff[15] << 8) |
	                               ((uint16_t)u1f_eth_rx_buff[16]);
	                               
	st_rcv_cmd6.Pm_Alarm100Refer = ((uint16_t)u1f_eth_rx_buff[17] << 8) |
	                               ((uint16_t)u1f_eth_rx_buff[18]);
	                               
	g_arMainAlarmNum[1]		=	st_rcv_cmd6.Pm_Alarm003Refer;
	g_arMainAlarmNum[2]		=	st_rcv_cmd6.Pm_Alarm005Refer;
	g_arMainAlarmNum[3]		=	st_rcv_cmd6.Pm_Alarm010Refer;
	g_arMainAlarmNum[4]		=	st_rcv_cmd6.Pm_Alarm025Refer;
	g_arMainAlarmNum[5]		=	st_rcv_cmd6.Pm_Alarm050Refer;
	g_arMainAlarmNum[6]		=	st_rcv_cmd6.Pm_Alarm100Refer;
	
	CPU_I2C.Alarm003 = g_arMainAlarmNum[1];
	CPU_I2C.Alarm005 = g_arMainAlarmNum[2];
	CPU_I2C.Alarm010 = g_arMainAlarmNum[3];
	CPU_I2C.Alarm025 = g_arMainAlarmNum[4];
	CPU_I2C.Alarm050 = g_arMainAlarmNum[5];
	CPU_I2C.Alarm100 = g_arMainAlarmNum[6];
	
	I2C1_WRITE();
}

// 7. 소리 설정
/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
static void tcp_rcv_sound_set(void)
{
	st_rcv_cmd7.Sound_Type		= u1f_eth_rx_buff[7];
	st_rcv_cmd7.Sound_Volume 	= u1f_eth_rx_buff[8];
	
	g_deMainSndTypNum	=	st_rcv_cmd7.Sound_Type;
	g_deMainSndVlmNum	=	st_rcv_cmd7.Sound_Volume;

	CPU_I2C.SndTyp = g_deMainSndTypNum;
	CPU_I2C.SndVlm = g_deMainSndVlmNum;
	
	I2C1_WRITE();
}

// 8. 가동시간 기준 설정
/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
static void tcp_rcv_lifetime_set(void)
{
	// uint32_t형 변환 저장
	st_rcv_cmd8.Dt_AlarmUptimeRefer = (((uint32_t)u1f_eth_rx_buff[7] << 24) |
                                       ((uint32_t)u1f_eth_rx_buff[8] << 16) |
                                       ((uint32_t)u1f_eth_rx_buff[9] << 8) |
                                       (uint32_t)(u1f_eth_rx_buff[10] & 0xFF));
                                       
	g_deMainLifetimeNum = st_rcv_cmd8.Dt_AlarmUptimeRefer;
	
	CPU_I2C.AlarmLifetime = g_deMainLifetimeNum;
	
	I2C1_WRITE();
}

// 9. RS485 통신 설정
/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
static void tcp_rcv_rs485_set(void)
{
	// 수신데이터 저장
	st_rcv_cmd9.Rs485_BaudRate 	= (((uint32_t)u1f_eth_rx_buff[7] << 24) |
	                                ((uint32_t)u1f_eth_rx_buff[8] << 16) |
	                                ((uint32_t)u1f_eth_rx_buff[9] << 8) |
	                                (uint32_t)(u1f_eth_rx_buff[10] & 0xFF));
	st_rcv_cmd9.Rs485_DataSize 	=	u1f_eth_rx_buff[11];
	st_rcv_cmd9.Rs485_Paritybit =	u1f_eth_rx_buff[12];
	st_rcv_cmd9.Rs485_Stopbit 	=	u1f_eth_rx_buff[13];
	st_rcv_cmd9.Rs485_Flowctrl 	=	u1f_eth_rx_buff[14];
	st_rcv_cmd9.Rs485_Protocol 	=	u1f_eth_rx_buff[15];
	st_rcv_cmd9.Rs485_Devcode 	=	u1f_eth_rx_buff[16];
	st_rcv_cmd9.Rs485_Leaddev 	=	((uint16_t)u1f_eth_rx_buff[17] << 8) |
	                                ((uint16_t)u1f_eth_rx_buff[18]);
	st_rcv_cmd9.Rs485_ModID 	=	u1f_eth_rx_buff[19];
	st_rcv_cmd9.Rs485_DestID 	=	u1f_eth_rx_buff[20];
	
	// 저장 데이터 -> 동작 데이터로 대입
	g_deMain485BaudNum			=	st_rcv_cmd9.Rs485_BaudRate;
	g_deMain485DataNum			=	st_rcv_cmd9.Rs485_DataSize;
	g_deMain485PrtyNum			=	st_rcv_cmd9.Rs485_Paritybit;
	g_deMain485StopNum			=	st_rcv_cmd9.Rs485_Stopbit;
	g_deMain485FlowNum			=	st_rcv_cmd9.Rs485_Flowctrl;
	g_deMain485PtclNum			=	st_rcv_cmd9.Rs485_Protocol;
	g_deMain485ModNum			=	st_rcv_cmd9.Rs485_ModID;
	g_deMain485DvldNum			=	st_rcv_cmd9.Rs485_Leaddev;
	g_deMain485DestNum			=	st_rcv_cmd9.Rs485_DestID;
	g_deMain485DvcdNum			=	st_rcv_cmd9.Rs485_Devcode;

	// EEPROM에 저장
	CPU_I2C.Rs485Baud = g_deMain485BaudNum;
	CPU_I2C.Rs485Data = g_deMain485DataNum;
	CPU_I2C.Rs485Prty = g_deMain485PrtyNum;
	CPU_I2C.Rs485Stop = g_deMain485StopNum;
	CPU_I2C.Rs485Flow = g_deMain485FlowNum;
	CPU_I2C.Rs485Mod = g_deMain485ModNum;
	CPU_I2C.Rs485Dest = g_deMain485DestNum;
	CPU_I2C.Rs485Ptcl = g_deMain485PtclNum;
	CPU_I2C.Rs485Dvcd = g_deMain485DvcdNum;
	CPU_I2C.Rs485Dvld = g_deMain485DvldNum;
	
	I2C1_WRITE();
	
	HAL_Delay(200);
	
	NVIC_SystemReset();
}

// 10. LAN 통신 SERVER 설정
/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
static void tcp_rcv_lanserver_set(void)
{
	// 수신데이터 저장
	st_rcv_cmd10.Lan_ModIpAddr[0]	=	u1f_eth_rx_buff[7];
	st_rcv_cmd10.Lan_ModIpAddr[1]	=	u1f_eth_rx_buff[8];
	st_rcv_cmd10.Lan_ModIpAddr[2]	=	u1f_eth_rx_buff[9];
	st_rcv_cmd10.Lan_ModIpAddr[3]	=	u1f_eth_rx_buff[10];
	
	st_rcv_cmd10.Lan_ModSubnet[0]	=	u1f_eth_rx_buff[11];
	st_rcv_cmd10.Lan_ModSubnet[1]	=	u1f_eth_rx_buff[12];
	st_rcv_cmd10.Lan_ModSubnet[2]	=	u1f_eth_rx_buff[13];
	st_rcv_cmd10.Lan_ModSubnet[3]	=	u1f_eth_rx_buff[14];
	
	st_rcv_cmd10.Lan_ModGateway[0]	=	u1f_eth_rx_buff[15];
	st_rcv_cmd10.Lan_ModGateway[1]	=	u1f_eth_rx_buff[16];
	st_rcv_cmd10.Lan_ModGateway[2]	=	u1f_eth_rx_buff[17];
	st_rcv_cmd10.Lan_ModGateway[3]	=	u1f_eth_rx_buff[18];
	
	st_rcv_cmd10.Lan_ModPort		=	((uint16_t)u1f_eth_rx_buff[19] << 8) |
	                                    ((uint16_t)u1f_eth_rx_buff[20]);
	// 저장 데이터 -> 동작 데이터로 대입
	for(uint8_t i = 0; i < 4; i++)
	{
		g_arMainLanModIpNum[i]		=	st_rcv_cmd10.Lan_ModIpAddr[i];
		g_arMainLanModSubNum[i]		=	st_rcv_cmd10.Lan_ModSubnet[i];
		g_arMainLanModGwNum[i]		=	st_rcv_cmd10.Lan_ModGateway[i];
	}
	
	g_deMainLanModPortNum			=	st_rcv_cmd10.Lan_ModPort;
	
	// EEPROM에 저장
	memcpy(&CPU_I2C.LanModIp, g_arMainLanModIpNum, 4);
	memcpy(&CPU_I2C.LanModSub, g_arMainLanModSubNum, 4);
	memcpy(&CPU_I2C.LanModGw, g_arMainLanModGwNum, 4);
	CPU_I2C.LanModPort= g_deMainLanModPortNum;
	
	I2C1_WRITE();
	
	HAL_Delay(200);
	
	NVIC_SystemReset();
}

// 11. LAN 통신 MAC/CLIENT 설정
/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
static void tcp_rcv_lanmac_client_set(void)
{
	st_rcv_cmd11.Lan_ModMacAddr[0]	=	u1f_eth_rx_buff[7];
	st_rcv_cmd11.Lan_ModMacAddr[1]	=	u1f_eth_rx_buff[8];
	st_rcv_cmd11.Lan_ModMacAddr[2]	=	u1f_eth_rx_buff[9];
	st_rcv_cmd11.Lan_ModMacAddr[3]	=	u1f_eth_rx_buff[10];
	st_rcv_cmd11.Lan_ModMacAddr[4]	=	u1f_eth_rx_buff[11];
	st_rcv_cmd11.Lan_ModMacAddr[5]	=	u1f_eth_rx_buff[12];
	
	st_rcv_cmd11.Lan_DestIpAddr[0]	=	u1f_eth_rx_buff[13];
	st_rcv_cmd11.Lan_DestIpAddr[1]	=	u1f_eth_rx_buff[14];
	st_rcv_cmd11.Lan_DestIpAddr[2]	=	u1f_eth_rx_buff[15];
	st_rcv_cmd11.Lan_DestIpAddr[3]	=	u1f_eth_rx_buff[16];
	
	st_rcv_cmd11.Lan_DestPort		=	((uint16_t)u1f_eth_rx_buff[17] << 8) |
	                                    ((uint16_t)u1f_eth_rx_buff[18]);
	                                    
	st_rcv_cmd11.Lan_Protocol		=	u1f_eth_rx_buff[19];
	st_rcv_cmd11.Lan_Devcode		=	u1f_eth_rx_buff[20];
	
	st_rcv_cmd11.Lan_Leaddev		=	((uint16_t)u1f_eth_rx_buff[21] << 8) |
	                                    ((uint16_t)u1f_eth_rx_buff[22]);
	
	for(uint8_t i = 0; i < 6; i++)
	{
		g_arMainLanModMacNum[i]		=	st_rcv_cmd11.Lan_ModMacAddr[i];
	}
	for(uint8_t j = 0; j < 4; j++)
	{
		g_arMainLanDestIpNum[j]		=	st_rcv_cmd11.Lan_DestIpAddr[j];
	}
	g_deMainLanDestPortNum			=	st_rcv_cmd11.Lan_DestPort;
	g_deMainLanPtclNum				=	st_rcv_cmd11.Lan_Protocol;
	g_deMainLanDvcdNum				=	st_rcv_cmd11.Lan_Devcode;
	g_deMainLanDvldNum				=	st_rcv_cmd11.Lan_Leaddev;

	memcpy(&CPU_I2C.LanModMac, g_arMainLanModMacNum, 6);
	memcpy(&CPU_I2C.LanDestIp, g_arMainLanDestIpNum, 4);
	CPU_I2C.LanDestPort = g_deMainLanDestPortNum;
	CPU_I2C.LanPtcl = g_deMainLanPtclNum;
	CPU_I2C.LanDvcd = g_deMainLanDvcdNum;
	CPU_I2C.LanDvld = g_deMainLanDvldNum;

	I2C1_WRITE();
	
	HAL_Delay(200);
	
	NVIC_SystemReset();
}

// 12. WLAN 통신 SSID 설정
/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
static void tcp_rcv_wlanssid_set(void)
{
	st_rcv_cmd12.Wifi_Ssid[0]	=	(int8_t)u1f_eth_rx_buff[7];
	st_rcv_cmd12.Wifi_Ssid[1]	=	(int8_t)u1f_eth_rx_buff[8];
	st_rcv_cmd12.Wifi_Ssid[2]	=	(int8_t)u1f_eth_rx_buff[9];
	st_rcv_cmd12.Wifi_Ssid[3]	=	(int8_t)u1f_eth_rx_buff[10];
	st_rcv_cmd12.Wifi_Ssid[4]	=	(int8_t)u1f_eth_rx_buff[11];
	st_rcv_cmd12.Wifi_Ssid[5]	=	(int8_t)u1f_eth_rx_buff[12];
	st_rcv_cmd12.Wifi_Ssid[6]	=	(int8_t)u1f_eth_rx_buff[13];
	st_rcv_cmd12.Wifi_Ssid[7]	=	(int8_t)u1f_eth_rx_buff[14];
	st_rcv_cmd12.Wifi_Ssid[8]	=	(int8_t)u1f_eth_rx_buff[15];
	st_rcv_cmd12.Wifi_Ssid[9]	=	(int8_t)u1f_eth_rx_buff[16];
	st_rcv_cmd12.Wifi_Ssid[10]	=	(int8_t)u1f_eth_rx_buff[17];
	st_rcv_cmd12.Wifi_Ssid[11]	=	(int8_t)u1f_eth_rx_buff[18];
	st_rcv_cmd12.Wifi_Ssid[12]	=	(int8_t)u1f_eth_rx_buff[19];
	st_rcv_cmd12.Wifi_Ssid[13]	=	(int8_t)u1f_eth_rx_buff[20];
	st_rcv_cmd12.Wifi_Ssid[14]	=	(int8_t)u1f_eth_rx_buff[21];
	st_rcv_cmd12.Wifi_Ssid[15]	=	(int8_t)u1f_eth_rx_buff[22];
}

// 13. WLAN 통신 PASS 설정
/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
static void tcp_rcv_wlanpass_set(void)
{
	st_rcv_cmd13.Wifi_Password[0]	=	(int8_t)u1f_eth_rx_buff[7];
	st_rcv_cmd13.Wifi_Password[1]	=	(int8_t)u1f_eth_rx_buff[8];
	st_rcv_cmd13.Wifi_Password[2]	=	(int8_t)u1f_eth_rx_buff[9];
	st_rcv_cmd13.Wifi_Password[3]	=	(int8_t)u1f_eth_rx_buff[10];
	st_rcv_cmd13.Wifi_Password[4]	=	(int8_t)u1f_eth_rx_buff[11];
	st_rcv_cmd13.Wifi_Password[5]	=	(int8_t)u1f_eth_rx_buff[12];
	st_rcv_cmd13.Wifi_Password[6]	=	(int8_t)u1f_eth_rx_buff[13];
	st_rcv_cmd13.Wifi_Password[7]	=	(int8_t)u1f_eth_rx_buff[14];
	st_rcv_cmd13.Wifi_Password[8]	=	(int8_t)u1f_eth_rx_buff[15];
	st_rcv_cmd13.Wifi_Password[9]	=	(int8_t)u1f_eth_rx_buff[16];
	st_rcv_cmd13.Wifi_Password[10]	=	(int8_t)u1f_eth_rx_buff[17];
	st_rcv_cmd13.Wifi_Password[11]	=	(int8_t)u1f_eth_rx_buff[18];
	st_rcv_cmd13.Wifi_Password[12]	=	(int8_t)u1f_eth_rx_buff[19];
	st_rcv_cmd13.Wifi_Password[13]	=	(int8_t)u1f_eth_rx_buff[20];
	st_rcv_cmd13.Wifi_Password[14]	=	(int8_t)u1f_eth_rx_buff[21];
	st_rcv_cmd13.Wifi_Password[15]	=	(int8_t)u1f_eth_rx_buff[22];
}

// 14. WLAN 통신 PASS 설정
/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
static void tcp_rcv_wlanserver_set(void)
{
	st_rcv_cmd14.Wifi_ModIpAddr[0]	=	u1f_eth_rx_buff[7];
	st_rcv_cmd14.Wifi_ModIpAddr[1]	=	u1f_eth_rx_buff[8];
	st_rcv_cmd14.Wifi_ModIpAddr[2]	=	u1f_eth_rx_buff[9];
	st_rcv_cmd14.Wifi_ModIpAddr[3]	=	u1f_eth_rx_buff[10];
	
	st_rcv_cmd14.Wifi_ModSubnet[0]	=	u1f_eth_rx_buff[11];
	st_rcv_cmd14.Wifi_ModSubnet[1]	=	u1f_eth_rx_buff[12];
	st_rcv_cmd14.Wifi_ModSubnet[2]	=	u1f_eth_rx_buff[13];
	st_rcv_cmd14.Wifi_ModSubnet[3]	=	u1f_eth_rx_buff[14];
	
	st_rcv_cmd14.Wifi_ModGateway[0]	=	u1f_eth_rx_buff[15];
	st_rcv_cmd14.Wifi_ModGateway[1]	=	u1f_eth_rx_buff[16];
	st_rcv_cmd14.Wifi_ModGateway[2]	=	u1f_eth_rx_buff[17];
	st_rcv_cmd14.Wifi_ModGateway[3]	=	u1f_eth_rx_buff[18];
	
	st_rcv_cmd14.Wifi_ModPort		=	((uint16_t)u1f_eth_rx_buff[19] << 8) |
	                                    ((uint16_t)u1f_eth_rx_buff[20]);
}

// 15. WLAN 통신 MAC/CLIENT 설정
/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
static void tcp_rcv_wlanmac_client_set(void)
{
	st_rcv_cmd15.Wifi_ModMacAddr[0]		=	u1f_eth_rx_buff[7];
	st_rcv_cmd15.Wifi_ModMacAddr[1]		=	u1f_eth_rx_buff[8];
	st_rcv_cmd15.Wifi_ModMacAddr[2]		=	u1f_eth_rx_buff[9];
	st_rcv_cmd15.Wifi_ModMacAddr[3]		=	u1f_eth_rx_buff[10];
	st_rcv_cmd15.Wifi_ModMacAddr[4]		=	u1f_eth_rx_buff[11];
	st_rcv_cmd15.Wifi_ModMacAddr[5]		=	u1f_eth_rx_buff[12];
	
	st_rcv_cmd15.Wifi_DestIpAddr[0]		=	u1f_eth_rx_buff[13];
	st_rcv_cmd15.Wifi_DestIpAddr[1]		=	u1f_eth_rx_buff[14];
	st_rcv_cmd15.Wifi_DestIpAddr[2]		=	u1f_eth_rx_buff[15];
	st_rcv_cmd15.Wifi_DestIpAddr[3]		=	u1f_eth_rx_buff[16];
	
	st_rcv_cmd15.Wifi_DestPort			=	((uint16_t)u1f_eth_rx_buff[17] << 8) |
	                                        ((uint16_t)u1f_eth_rx_buff[18]);

	st_rcv_cmd15.Wifi_Protocol			=	u1f_eth_rx_buff[19];
	st_rcv_cmd15.Wifi_Devcode			=	u1f_eth_rx_buff[20];
	
	st_rcv_cmd15.Wifi_Leaddev			=	((uint16_t)u1f_eth_rx_buff[21] << 8) |
		                                    ((uint16_t)u1f_eth_rx_buff[22]);
}

// 16. BLE 통신 SERVICE UUID 설정
/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
static void tcp_rcv_bleservuuid_set(void)
{
	st_rcv_cmd16.Ble_ServUuid[0]	=	u1f_eth_rx_buff[7];
	st_rcv_cmd16.Ble_ServUuid[1]	=	u1f_eth_rx_buff[8];
	st_rcv_cmd16.Ble_ServUuid[2]	=	u1f_eth_rx_buff[9];
	st_rcv_cmd16.Ble_ServUuid[3]	=	u1f_eth_rx_buff[10];
	st_rcv_cmd16.Ble_ServUuid[4]	=	u1f_eth_rx_buff[11];
	st_rcv_cmd16.Ble_ServUuid[5]	=	u1f_eth_rx_buff[12];
	st_rcv_cmd16.Ble_ServUuid[6]	=	u1f_eth_rx_buff[13];
	st_rcv_cmd16.Ble_ServUuid[7]	=	u1f_eth_rx_buff[14];
	st_rcv_cmd16.Ble_ServUuid[8]	=	u1f_eth_rx_buff[15];
	st_rcv_cmd16.Ble_ServUuid[9]	=	u1f_eth_rx_buff[16];
	st_rcv_cmd16.Ble_ServUuid[10]	=	u1f_eth_rx_buff[17];
	st_rcv_cmd16.Ble_ServUuid[11]	=	u1f_eth_rx_buff[18];
	st_rcv_cmd16.Ble_ServUuid[12]	=	u1f_eth_rx_buff[19];
	st_rcv_cmd16.Ble_ServUuid[13]	=	u1f_eth_rx_buff[20];
	st_rcv_cmd16.Ble_ServUuid[14]	=	u1f_eth_rx_buff[21];
	st_rcv_cmd16.Ble_ServUuid[15]	=	u1f_eth_rx_buff[22];
}

// 17. BLE 통신 CHAR. UUID 설정
/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
static void tcp_rcv_blecharuuid_set(void)
{
	st_rcv_cmd17.Ble_CharUuid[0]	=	u1f_eth_rx_buff[7];
	st_rcv_cmd17.Ble_CharUuid[1]	=	u1f_eth_rx_buff[8];
	st_rcv_cmd17.Ble_CharUuid[2]	=	u1f_eth_rx_buff[9];
	st_rcv_cmd17.Ble_CharUuid[3]	=	u1f_eth_rx_buff[10];
	st_rcv_cmd17.Ble_CharUuid[4]	=	u1f_eth_rx_buff[11];
	st_rcv_cmd17.Ble_CharUuid[5]	=	u1f_eth_rx_buff[12];
	st_rcv_cmd17.Ble_CharUuid[6]	=	u1f_eth_rx_buff[13];
	st_rcv_cmd17.Ble_CharUuid[7]	=	u1f_eth_rx_buff[14];
	st_rcv_cmd17.Ble_CharUuid[8]	=	u1f_eth_rx_buff[15];
	st_rcv_cmd17.Ble_CharUuid[9]	=	u1f_eth_rx_buff[16];
	st_rcv_cmd17.Ble_CharUuid[10]	=	u1f_eth_rx_buff[17];
	st_rcv_cmd17.Ble_CharUuid[11]	=	u1f_eth_rx_buff[18];
	st_rcv_cmd17.Ble_CharUuid[12]	=	u1f_eth_rx_buff[19];
	st_rcv_cmd17.Ble_CharUuid[13]	=	u1f_eth_rx_buff[20];
	st_rcv_cmd17.Ble_CharUuid[14]	=	u1f_eth_rx_buff[21];
	st_rcv_cmd17.Ble_CharUuid[15]	=	u1f_eth_rx_buff[22];
}

// 18. BBLE 통신 SPARE UUID 설정
/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
static void tcp_rcv_blespareuuid_set(void)
{
	st_rcv_cmd18.Ble_SpareUuid[0]	=	u1f_eth_rx_buff[7];
	st_rcv_cmd18.Ble_SpareUuid[1]	=	u1f_eth_rx_buff[8];
	st_rcv_cmd18.Ble_SpareUuid[2]	=	u1f_eth_rx_buff[9];
	st_rcv_cmd18.Ble_SpareUuid[3]	=	u1f_eth_rx_buff[10];
	st_rcv_cmd18.Ble_SpareUuid[4]	=	u1f_eth_rx_buff[11];
	st_rcv_cmd18.Ble_SpareUuid[5]	=	u1f_eth_rx_buff[12];
	st_rcv_cmd18.Ble_SpareUuid[6]	=	u1f_eth_rx_buff[13];
	st_rcv_cmd18.Ble_SpareUuid[7]	=	u1f_eth_rx_buff[14];
	st_rcv_cmd18.Ble_SpareUuid[8]	=	u1f_eth_rx_buff[15];
	st_rcv_cmd18.Ble_SpareUuid[9]	=	u1f_eth_rx_buff[16];
	st_rcv_cmd18.Ble_SpareUuid[10]	=	u1f_eth_rx_buff[17];
	st_rcv_cmd18.Ble_SpareUuid[11]	=	u1f_eth_rx_buff[18];
	st_rcv_cmd18.Ble_SpareUuid[12]	=	u1f_eth_rx_buff[19];
	st_rcv_cmd18.Ble_SpareUuid[13]	=	u1f_eth_rx_buff[20];
	st_rcv_cmd18.Ble_SpareUuid[14]	=	u1f_eth_rx_buff[21];
	st_rcv_cmd18.Ble_SpareUuid[15]	=	u1f_eth_rx_buff[22];
}

// 19. BLE 통신 PIN CODE 설정
/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
static void tcp_rcv_blepincode_set(void)
{
	st_rcv_cmd19.Ble_Pincode[0]		=	(int8_t)u1f_eth_rx_buff[7];
	st_rcv_cmd19.Ble_Pincode[1]		=	(int8_t)u1f_eth_rx_buff[8];
	st_rcv_cmd19.Ble_Pincode[2]		=	(int8_t)u1f_eth_rx_buff[9];
	st_rcv_cmd19.Ble_Pincode[3]		=	(int8_t)u1f_eth_rx_buff[10];
	st_rcv_cmd19.Ble_Pincode[4]		=	(int8_t)u1f_eth_rx_buff[11];
	st_rcv_cmd19.Ble_Pincode[5]		=	(int8_t)u1f_eth_rx_buff[12];
	st_rcv_cmd19.Ble_Pincode[6]		=	(int8_t)u1f_eth_rx_buff[13];
	st_rcv_cmd19.Ble_Pincode[7]		=	(int8_t)u1f_eth_rx_buff[14];
	st_rcv_cmd19.Ble_Pincode[8]		=	(int8_t)u1f_eth_rx_buff[15];
	st_rcv_cmd19.Ble_Pincode[9]		=	(int8_t)u1f_eth_rx_buff[16];
	st_rcv_cmd19.Ble_Pincode[10]	=	(int8_t)u1f_eth_rx_buff[17];
	st_rcv_cmd19.Ble_Pincode[11]	=	(int8_t)u1f_eth_rx_buff[18];
	st_rcv_cmd19.Ble_Pincode[12]	=	(int8_t)u1f_eth_rx_buff[19];
	st_rcv_cmd19.Ble_Pincode[13]	=	(int8_t)u1f_eth_rx_buff[20];
	st_rcv_cmd19.Ble_Pincode[14]	=	(int8_t)u1f_eth_rx_buff[21];
	st_rcv_cmd19.Ble_Pincode[15]	=	(int8_t)u1f_eth_rx_buff[22];
}

// 20. 온습도감지 기준 설정
/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
static void tcp_rcv_thalarm_set(void)
{
	st_rcv_cmd20.Th_AlarmOverTempWarnRefer	=	(int8_t)u1f_eth_rx_buff[7];
	st_rcv_cmd20.Th_AlarmOverTempErrRefer	=	(int8_t)u1f_eth_rx_buff[8];
	st_rcv_cmd20.Th_AlarmOverHumiErrRefer	=	(int8_t)u1f_eth_rx_buff[9];
}

// 21. 온습도 보정치 설정
/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
static void tcp_rcv_thscale_set(void)
{
	st_rcv_cmd21.Th_TempSlope		=	((uint16_t)u1f_eth_rx_buff[7] << 8) |
	                                    ((uint16_t)u1f_eth_rx_buff[8]);
	st_rcv_cmd21.Th_TempOffset		=	((uint16_t)u1f_eth_rx_buff[9] << 8) |
	                                    ((uint16_t)u1f_eth_rx_buff[10]);
	st_rcv_cmd21.Th_HumiSlope		=	((uint16_t)u1f_eth_rx_buff[11] << 8) |
	                                    ((uint16_t)u1f_eth_rx_buff[12]);
	st_rcv_cmd21.Th_HumiOffset		=	((uint16_t)u1f_eth_rx_buff[13] << 8) |
	                                    ((uint16_t)u1f_eth_rx_buff[14]);
}


/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
void _______Below_this_line_is_the_checksum_related_function(void)
{}
// 체크섬 생성 함수
/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
static uint16_t tcp_checksum_creat(uint16_t u2t_cnt)
{
	uint32_t u4t_sum_data = 0;
	uint16_t u2t_rtn = 0;

	//STX 제외  : LENGTH ~ DATA 까지 체크섬 계산
	for(uint16_t i = 1; i < u2t_cnt; i++)
	{
		u4t_sum_data += (uint32_t)u1f_eth_tx_buff[i];
	}

	u2t_rtn = (uint16_t)u4t_sum_data;

	return u2t_rtn;
}

// 수신 패킷 체크섬 체크 함수
/****************************************************************************/
/*	Overview	:															*/
/*	Return value:	void													*/
/****************************************************************************/
static uint16_t tcp_checksum_check(uint16_t u2t_cnt)
{
	uint32_t u4t_sum_data_r = 0;
	uint16_t u1t_chksum[2] = {0,0};
	uint16_t u2t_chksum = 0;
	
	//STX 제외  : LENGTH ~ DATA 까지 체크섬 계산
	for(uint16_t i = 1; i < (u2t_cnt - 3); i++)
	{
		u4t_sum_data_r += (uint32_t)u1f_eth_rx_buff[i];
	}
	
	u2t_chksum = (uint16_t)u4t_sum_data_r;
	
	u1t_chksum[0] = (uint8_t)((u2t_chksum >> 8) & 0xFF);
	u1t_chksum[1] = (uint8_t)(u2t_chksum & 0xFF);
	
	if(u1t_chksum[0] != u1f_eth_rx_buff[u2t_cnt-3])
	{
		return CHECK_SUM_ERROR;
	}
	
	if(u1t_chksum[1] != u1f_eth_rx_buff[u2t_cnt-2])
	{
		return CHECK_SUM_ERROR;
	}
	
	return CHECK_SUM_OK;
}


#endif
