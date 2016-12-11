/******************************************************************************
 * Copyright 2013-2014 Espressif Systems
 *
*******************************************************************************/
#include "ets_sys.h"
#include "os_type.h"
#include "osapi.h"
#include "mem.h"
#include "user_interface.h"

#include "espconn.h"
#include "user_json.h"
#include "user_devicefind.h"

const char *device_find_request = "Are You ESP8266 Device?";

const char *device_find_response_ok = "Yes,I'm ESP8266!";

/*---------------------------------------------------------------------------*/
LOCAL struct espconn ptrespconn;

LOCAL os_timer_t test_timer;
LOCAL struct espconn user_udp_espconn;

const char *ESP8266_MSG = "I'm ESP8266 ";
   
/*---------------------------------------------------------------------------*/
LOCAL struct espconn send_espconn;


/******************************************************************************
 * FunctionName : user_devicefind_recv
 * Description  : Processing the received udp packet
 * Parameters   : arg -- Additional argument to pass to the callback function
 *                pusrdata -- The received data (or NULL when the connection has been closed!)
 *                length -- The length of received data
 * Returns      : none
*******************************************************************************/
LOCAL void ICACHE_FLASH_ATTR user_udp_recv (void *arg, char *pusrdata, unsigned short length)
{
    char DeviceBuffer[40] = {0};
    char Device_mac_buffer[60] = {0};

    struct ip_info ipconfig;
    char hwaddr[6];

	if (wifi_get_opmode() != STATION_MODE)
	{
		wifi_get_ip_info(SOFTAP_IF, &ipconfig);
		wifi_get_macaddr(SOFTAP_IF, hwaddr);

		if (!ip_addr_netcmp((struct ip_addr *)ptrespconn.proto.udp->remote_ip, &ipconfig.ip, &ipconfig.netmask))
		{
			//udp packet is received from ESP8266 station
			wifi_get_ip_info(STATION_IF, &ipconfig);
			wifi_get_macaddr(STATION_IF, hwaddr);
		}
		else
		{
			//udp packet is received from ESP8266 softAP
		}
    }
	else
	{
		//udp packet is received from ESP8266 station
		wifi_get_ip_info(STATION_IF, &ipconfig);
		wifi_get_macaddr(STATION_IF, hwaddr);
	}

    if (pusrdata == NULL)
	{
        return;
	}
   

    if (length == os_strlen(device_find_request) &&
            os_strncmp(pusrdata, device_find_request, os_strlen(device_find_request)) == 0)
    {
        //received device find message
       
        os_sprintf(DeviceBuffer, "%s" MACSTR " " IPSTR, device_find_response_ok,
                   MAC2STR(hwaddr), IP2STR(&ipconfig.ip));

        os_printf("%s\n", DeviceBuffer);
        length = os_strlen(DeviceBuffer);
      
      //if received "Are You ESP8266 ?" , response "Yes,I'm ESP8266!" + ESP8266 mac + ESP8266 ip
        espconn_sent (&ptrespconn, DeviceBuffer, length);
      
    }
	else
	{
		//received some other data
    } 
}

/******************************************************************************
 * FunctionName : user_devicefind_init
 * Description  : create a udp listening
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR user_udp_init (void)
{
    ptrespconn.type = ESPCONN_UDP;
    ptrespconn.proto.udp = (esp_udp *)os_zalloc(sizeof(esp_udp));
    ptrespconn.proto.udp->local_port = 1025;  // ESP8266 udp port
    espconn_regist_recvcb (&ptrespconn, user_udp_recv); // register a udp packet receiving callback
    espconn_create (&ptrespconn);   // create udp
}

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void user_init (void)
{
	os_printf("SDK version:%s\n", system_get_sdk_version());

	//Set softAP + station mode
	wifi_set_opmode(STATIONAP_MODE);

	// Create udp listening.
	user_udp_init();

	//ESP8266 connect to router
	user_set_station_config();
}

 /******************************************************************************
      * FunctionName : user_udp_send
      * Description  : udp send data
      * Parameters  : none
      * Returns      : none
 *******************************************************************************/
 LOCAL void ICACHE_FLASH_ATTR user_udp_send (void)
 {
     char DeviceBuffer[40] = {0};
     char hwaddr[6];
     struct ip_info ipconfig;

     const char udp_remote_ip[4] = { 255, 255, 255, 255}; 
     os_memcpy (user_udp_espconn.proto.udp->remote_ip, udp_remote_ip, 4); // ESP8266 udp remote IP need to be set everytime we call espconn_sent
     user_udp_espconn.proto.udp->remote_port = 1112;  // ESP8266 udp remote port need to be set everytime we call espconn_sent

     wifi_get_macaddr (STATION_IF, hwaddr);
 
     os_sprintf (DeviceBuffer, "%s" MACSTR "!" , ESP8266_MSG, MAC2STR(hwaddr));
 
     espconn_sent (&user_udp_espconn, DeviceBuffer, os_strlen(DeviceBuffer));
   
 }
 
 /******************************************************************************
      * FunctionName : user_udp_sent_cb
      * Description  : udp sent successfully
      * Parameters  : arg -- Additional argument to pass to the callback function
      * Returns      : none
 *******************************************************************************/
LOCAL void ICACHE_FLASH_ATTR user_udp_sent_cb (void *arg)
	{
		struct espconn *pespconn = arg;

		os_printf ("user_udp_send successfully !!!\n");

		//disarm timer first
		os_timer_disarm (&test_timer);

		//re-arm timer to check ip
		os_timer_setfn (&test_timer, (os_timer_func_t *)user_udp_send, NULL); // only send next packet after prev packet sent successfully
		os_timer_arm (&test_timer, 1000, 0);
	}


 /******************************************************************************
 * FunctionName : user_check_ip
 * Description  : check whether get ip addr or not
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR user_check_ip (void)
{
    struct ip_info ipconfig;

   //disarm timer first
    os_timer_disarm(&test_timer);

   //get ip info of ESP8266 station
    wifi_get_ip_info (STATION_IF, &ipconfig);

    if (wifi_station_get_connect_status () == STATION_GOT_IP && ipconfig.ip.addr != 0)
   {
		os_printf("got ip !!! \r\n");

		wifi_set_broadcast_if (STATIONAP_MODE); // send UDP broadcast from both station and soft-AP interface

		user_udp_espconn.type = ESPCONN_UDP;
		user_udp_espconn.proto.udp = (esp_udp *)os_zalloc(sizeof(esp_udp));
		user_udp_espconn.proto.udp->local_port = espconn_port();  // set a available  port

		const char udp_remote_ip[4] = {255, 255, 255, 255}; 

		os_memcpy(user_udp_espconn.proto.udp->remote_ip, udp_remote_ip, 4); // ESP8266 udp remote IP

		user_udp_espconn.proto.udp->remote_port = 1112;  // ESP8266 udp remote port

		espconn_regist_recvcb (&user_udp_espconn, NULL); // register a udp packet receiving callback
		espconn_regist_sentcb (&user_udp_espconn, user_udp_sent_cb); // register a udp packet sent callback

		espconn_create(&user_udp_espconn);   // create udp

		user_udp_send ();   // send udp data

    }
	else
	{
        if ((wifi_station_get_connect_status() == STATION_WRONG_PASSWORD ||
                wifi_station_get_connect_status() == STATION_NO_AP_FOUND ||
                wifi_station_get_connect_status() == STATION_CONNECT_FAIL))
        {
			os_printf ("connect fail !!! \r\n");
        }
		else
		{
           //re-arm timer to check ip
            os_timer_setfn (&test_timer, (os_timer_func_t *)user_check_ip, NULL);
            os_timer_arm (&test_timer, 100, 0);
        }
    }
}


/******************************************************************************
 * FunctionName : user_set_station_config
 * Description  : set the router info which ESP8266 station will connect to
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR user_set_station_config (void)
{
	// Wifi configuration
	char ssid[32] = SSID;
	char password[64] = PASSWORD;
	struct station_config stationConf;

	//need not mac address
	stationConf.bssid_set = 0;

	//Set ap settings
	os_memcpy(&stationConf.ssid, ssid, 32);
	os_memcpy(&stationConf.password, password, 64);
	wifi_station_set_config(&stationConf);

	//set a timer to check whether got ip from router succeed or not.
	os_timer_disarm (&test_timer);
	os_timer_setfn (&test_timer, (os_timer_func_t *)user_check_ip, NULL);
	os_timer_arm (&test_timer, 100, 0);
}




/*****************************************************************************
* callback return receive data
*****************************************************************************/
LOCAL void ICACHE_FLASH_ATTR user_udp_recv_cb (void *arg, char *pusrdata, unsigned short length)
 {
     
     os_printf ("recv udp data: %s\n", pusrdata);
     struct espconn *pesp_conn = arg;
     
       remot_info *premot = NULL;
       sint8 value = ESPCONN_OK;
	   
       if (espconn_get_connection_info(pesp_conn,&premot,0) == ESPCONN_OK){
             pesp_conn->proto.udp->remote_port = premot->remote_port;
             pesp_conn->proto.udp->remote_ip[0] = premot->remote_ip[0];
             pesp_conn->proto.udp->remote_ip[1] = premot->remote_ip[1];
             pesp_conn->proto.udp->remote_ip[2] = premot->remote_ip[2];
             pesp_conn->proto.udp->remote_ip[3] = premot->remote_ip[3];
             espconn_sent (pesp_conn, pusrdata, os_strlen(pusrdata));
       }
 }

