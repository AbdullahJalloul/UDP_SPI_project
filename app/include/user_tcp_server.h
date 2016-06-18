#ifndef __USER_TCP_SERVER__
#define __USER_TCP_SERVER__

#include "user_config.h"

#ifndef __ESPCONN_H__
typedef sint8 err_t;
enum espconn_state
{
	ESPCONN_NONE,
	ESPCONN_WAIT,  // ������� ��������
	ESPCONN_LISTEN, // ���������� �������, ���� rx
	ESPCONN_CONNECT, // ���������� �������, ���� rx  ��� tx
	ESPCONN_WRITE,
	ESPCONN_READ,
	ESPCONN_CLOSE // ���������� �����������
};
#endif

#ifndef TCP_SRV_MAX_CONNECTIONS
#define TCP_SRV_MAX_CONNECTIONS 7
#endif

#ifndef TCP_SRV_SERVER_PORT
#define TCP_SRV_SERVER_PORT 80
#endif

#ifndef DEBUGSOO
#define DEBUGSOO 3
#endif

#ifndef TCP_SRV_RECV_WAIT
#define TCP_SRV_RECV_WAIT  5
#endif

#ifndef TCP_SRV_END_WAIT
#define TCP_SRV_END_WAIT  5
#endif

#define TCP_SRV_CLOSE_WAIT 120 // 120/4 = 30 ���

#define TCP_SRV_MIN_HEAP_SIZE 14528  // ����� ������� �� 6Kb,
// ���� ����� �� TCP WIN * 2 + ����������� � ������ (1560*4*2=12480) + 2Kb = 14528

//--------------------------------------------------------------------------
// ��������� ����������
//
typedef struct _tcpsrv_conn_flags
{
	uint16 throttle_data_reception :1; //
	uint16 close_recv_sent :1; //
} tcpsrv_conn_flags;

typedef struct _TCP_SERV_CONN
{
	struct _tcpsrv_conn_flags flag;  // �����
	uint16 state;    	// pcb state
	struct _TCP_SERV_CFG *pcfg;      // ��������� �� ����
	uint16 recv_check;   // ���� ������ � tcpsrv_pool
	uint16 remote_port;  // ����� ����� �������
	union
	{              // ip ����� �������
		uint8 b[4];
		uint32 dw;
	} remote_ip;
	struct _TCP_SERV_CONN *next; // ��������� �� ��������� ���������
	struct tcp_pcb *pcb; // pcb
	os_timer_t ptimer;  // use tcpsrv_close_cb
	uint32 cntr; 	// ����������� sent data
	uint8 *ptrbuf; // buf ����������� sent data
	uint16 unrecved_bytes; // ������������ ��� ������ ���������� TCP WIN / This can be used to throttle data reception
} TCP_SERV_CONN;

//--------------------------------------------------------------------------
// ���������� �������
//
typedef void (*func_disconect_calback) (TCP_SERV_CONN *ts_conn); // ���������� �������
typedef err_t (*func_listen) (TCP_SERV_CONN *hconn); // ����� ������
typedef err_t (*func_received_data) (TCP_SERV_CONN *ts_conn, uint8 *pusrdata, uint16 length); // ������� length ����, ����� � ������ �� pusrdata
typedef err_t (*func_sent_callback) (TCP_SERV_CONN *ts_conn); // ���� ������ �������

typedef struct _tcpsrv_cfg_flags
{
	uint16 none :1; // ��� ����
} tcpsrv_cfg_flags;

//--------------------------------------------------------------------------
// ��������� ������������ tcp �������
//
typedef struct _TCP_SERV_CFG
{
	tcpsrv_cfg_flags flg;      // ����� ������������ (���� �� ������������)
	uint16 port;      	 // ����� �����, ��� = 0 ���������� �� 80.
	uint16 max_conn; // ������������ ���-�� ������������� ����������, ��� = 0 ���������� �� 5.
	uint16 conn_count; // ���-�� ������� ����������, ��� ������������� ����������� 0
	uint16 min_heap; // ����������� ������ heap ��� �������� ������ ����������, ��� = 0 ���������� �� 8192.
	uint16 time_wait_rec; // ����� (���) �������� ������� �� �������, �� ����-�������� ����������, ��� = 0 ���������� �� 5 ���.
	uint16 time_wait_cls; // ����� (���) �� ����-�������� ���������� ����� ������ ��� ��������, ��� = 0 ���������� �� 5 ���.
	TCP_SERV_CONN * conn_links; // ��������� �� ������� �������� ����������, ��� ������������� ��� ��������� �������� ���������� = NULL
	struct tcp_pcb *pcb;     // ��������� pcb [LISTEN]
	func_disconect_calback func_discon_cb; // ������� ���������� ����� �������� ����������, ���� = NULL - �� ����������
	func_listen func_listen; // ������� ���������� ��� ������������� �������, ���� = NULL - �� ����������
	func_sent_callback func_sent_cb; // ������� ���������� ����� �������� ������ ��� ������� ����� � ip ����� ��� �������� �������� ������
	func_received_data func_recv; // ������� ���������� ��� ������ ������
	struct _TCP_SERV_CFG *next; // ��������� ���������
} TCP_SERV_CFG;

//--------------------------------------------------------------------------
// ������
//
extern TCP_SERV_CFG *phcfg; // ��������� �� ������� TCP_SERV_CFG (������������ ��������)

//--------------------------------------------------------------------------
// �������
//
err_t tcpsrv_sent_data (TCP_SERV_CONN * ts_conn, uint8 *psent, uint16 length)
ICACHE_FLASH_ATTR; // �������� length ����
void tcpsrv_disconnect (TCP_SERV_CONN * ts_conn)
ICACHE_FLASH_ATTR;
void tcpsrv_print_remote_info (TCP_SERV_CONN *ts_conn)
ICACHE_FLASH_ATTR; // ������� remote_ip:remote_port [conn_count] os_printf("srv x.x.x.x:x [n] ")
TCP_SERV_CFG * hcfg_find_port (uint16 portn)
ICACHE_FLASH_ATTR; // ����� ������� �� ������ �����
void tcpsrv_unrecved_win (TCP_SERV_CONN *ts_conn)
ICACHE_FLASH_ATTR; // ������������ ������ TCP WIN, ���� ������������ ������ ���������� �������� ���� TCP

TCP_SERV_CFG *tcpsrv_init (uint16 portn);
bool tcpsrv_start (TCP_SERV_CFG *p);
bool tcpsrv_close (TCP_SERV_CFG *p);

#endif
