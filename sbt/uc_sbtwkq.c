/**************************************************************************
**  Copyright (c) 2013 GalaxyWind, Ltd.
**
**  Project: 思博特温控器
**  File:    uc_sbtwkq.c
**  Author:  wangdawei
**  Date:    09/16/2015
**
**  Purpose:
**    网络通讯.
**************************************************************************/


/* Include files. */
#include "uc_tt_common.h"

#include "stlc_list.h"
#include "cl_thread.h"
#include "uc_push.h"
#include "types_def.h"
#include "udp_ctrl.h"
#include "uc_sbtwkq.h"
#include "sbtwkq_uart.h"
/* Macro constant definitions. */



/* Type definitions. */
 

/* Local function declarations. */

/* Macro API definitions. */


/* Global variable declarations. */ 
static int sbtwkq_get_tlv_attr_stream(u_int8_t **buff, u_int8_t *buf, u_int8_t len)
{
	
	uc_tlv_t *tlv = NULL;
	u_int32_t tlv_len = 0;
	uc_tlv_t *tlv_head = NULL;
	int i = 0;
	
	tlv_len = len + sizeof(uc_tlv_t);
	tlv_head = (uc_tlv_t *)cl_calloc(tlv_len, 1);
	if (NULL == tlv_head){
		return 0;
	}

	tlv = tlv_head;
	
	tlv->type = htons(UCT_TT_RAW_CHIPDATA);
	tlv->len = htons(len);
	memcpy(tlv_val(tlv), buf, len);

	*buff = tlv_head;
	return tlv_len;

}
static void sbtwkq_do_allstate(uc_tt_common_app2chip_t * net_param)
{
	u_int8_t action = net_param->action;
	ucp_obj_t *uobj = net_param->uobj;
	u_int8_t *obj_stream = NULL;
	u_int32_t obj_stream_len = 0;

	//只支持Get方法
	if (action != UCA_GET){
		uc_do_set_reply(uobj, ERR_CMD_INVALID);
		return;
	}

	obj_stream_len = sbtwkq_get_tlv_attr_stream(&obj_stream, sbtwkq_last_stat.data, 
							sbtwkq_last_stat.data_len);

	if (obj_stream){
		uc_put_elemet(uci_send_buf, MAX_UC_PKT_SIZE, &uci_send_index, 
						uobj->objct,uobj->sub_objct, uobj->attr, obj_stream_len, obj_stream);	
		cl_free(obj_stream);
	}
} 
static void sbtwkq_do_cmd_get(uc_tt_common_app2chip_t *net_param)
{
	ucp_obj_t *uobj = net_param->uobj;
	u_int8_t *obj_stream = NULL;
	u_int32_t obj_stream_len = 0;

	obj_stream_len = sbtwkq_get_tlv_attr_stream(&obj_stream, sbtwkq_last_stat.data, 
							sbtwkq_last_stat.data_len);
	
	if (obj_stream){
		uc_put_elemet(uci_send_buf, MAX_UC_PKT_SIZE, &uci_send_index, 
						uobj->objct,uobj->sub_objct, uobj->attr, obj_stream_len, obj_stream);
		cl_free(obj_stream);
	}
}
static void sbtwkq_do_cmd_set(uc_tt_common_app2chip_t * net_param)
{
	ucp_obj_t *uobj = net_param->uobj;
	u_int16_t param_len = uobj->param_len;
	uc_tlv_t *tlv = NULL;
	u_int16_t remain = 0;
	if (param_len < sizeof(uc_tlv_t)){
		SBTWKQ_INFO("sbtwkq set param error.(param_len=%d, uc_tlv_t=%d)\r\n", param_len, sizeof(uc_tlv_t));
		return;
	}
	remain = param_len;
	
	tlv = (uc_tlv_t *)&uobj[1];
	tlv->type = ntohs(tlv->type);
	tlv->len = ntohs(tlv->len);

	while(remain >= (sizeof(uc_tlv_t) + tlv->len)){ 
		sbtwkq_uart_request_add(&tlv[1], tlv->len);
		remain = remain - sizeof(uc_tlv_t) - tlv->len;
		if (remain < sizeof(uc_tlv_t))
			break;
		
		tlv = tlv_next(tlv);
		tlv->type = ntohs(tlv->type);
		tlv->len = ntohs(tlv->len);
	}
} 
static void sbtwkq_do_cmd(uc_tt_common_app2chip_t *net_param)
{
	u_int8_t action = net_param->action;
	ucp_obj_t *uobj = net_param->uobj;

	switch(action){
		case UCA_GET:
			SBTWKQ_INFO("sbtwkq_do_cmd get!\n");
			sbtwkq_do_cmd_get(net_param);
			break;
		case UCA_SET:
			SBTWKQ_INFO("sbtwkq_do_cmd set!\n");
			sbtwkq_do_cmd_set(net_param);
			uc_do_set_reply(uobj, ERR_NONE);
			break;
		default:
			uc_do_set_reply(uobj, ERR_CMD_INVALID);
	}
}
static void sbtwkq_do_tt(int event, void *data, int data_len)
{
	uc_tt_common_app2chip_t *net_param = NULL;
	ucp_obj_t *uobj = NULL;
	SBTWKQ_INFO("sbtwkq_do_tt!\n");
	if(data == NULL || data_len < sizeof(uc_tt_common_app2chip_t)){
		SBTWKQ_INFO("sbtwkq_do_tt net param invalid\r\n");
		return;
	}
	net_param = (uc_tt_common_app2chip_t *)data;
	uobj = net_param->uobj;
	
	switch(uobj->attr) {
		case UCAT_IA_TT_ALLSTATE:
			SBTWKQ_INFO("sbtwkq_do_tt:UCAT_IA_TT_ALLSTATE!\n");
			sbtwkq_do_allstate(net_param);
			break;
		case UCAT_IA_TT_CMD:
			SBTWKQ_INFO("sbtwkq_do_tt:UCAT_IA_TT_CMD!\n");
			sbtwkq_do_cmd(net_param);
			break;
		default:
			uc_do_set_reply(uobj, ERR_CMD_INVALID);	
			break;
	}

}

static pkt_t * sbtwkq_push_pkt(ucds_session_t *s)
{
	pkt_t *pkt = NULL;
	int total_len = 0, buf_len;
	ucp_ctrl_t *uc;
	u_int8_t *buf = NULL;
	int uci_send_index = 0;
	u_int8_t *obj_stream = NULL;
	u_int32_t obj_stream_len = 0;

	buf_len = sizeof(ucp_obj_t) + sbtwkq_last_stat.data_len + sizeof(uc_tlv_t);
	total_len = sizeof(ucp_ctrl_t) + buf_len;
	
	pkt = uc_pkt_new(CMD_UDP_CTRL, total_len, true, s->is_enc, 0, s->client_sid, 
				ucd.client.device_sid, s->my_request_id);
	if (!pkt) {
		return NULL;
	}

	uc = get_ucp_payload(pkt, ucp_ctrl_t);
	uc->action = UCA_PUSH;
	uc->count = 1;
	//填充属性值
	buf = (u_int8_t*)&(uc[1]);
	obj_stream_len = sbtwkq_get_tlv_attr_stream(&obj_stream, sbtwkq_last_stat.data,
						sbtwkq_last_stat.data_len);

	if (obj_stream){
		_uc_put_elemet(buf,  buf_len, &uci_send_index, 
			UCOT_IA, UCSOT_IA_TT, UCAT_IA_TT_ALLSTATE, obj_stream_len, obj_stream);
		cl_free(obj_stream);
	}
	return pkt;
} 


void sbtwkq_init(void)
{
	SBTWKQ_INFO("sbtwkq_init!\n");
	sbtwkq_uart_init();
	memset(&sbtwkq_last_stat, 0, sizeof(sbtwkq_last_stat));
	ucd_event_add(EVENT_UC_APP2CHIP, sbtwkq_do_tt);	
	uc_push_init(sbtwkq_push_pkt);
}

