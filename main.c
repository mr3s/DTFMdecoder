
#include <pjlib.h>
#include <pjlib-util.h>
#include <pjmedia.h>
#include <pjmedia-codec.h>
#include <pjsip.h>
#include <pjsip_simple.h>
#include <pjsip_ua.h>
#include <pjsua-lib/pjsua.h>

  
 #define THIS_FILE "APP"
 
 #define SIP_DOMAIN "127.0.0.1"
 #define SIP_USER "6001"
 #define SIP_PASSWD "user"
 #define PORT 5038
 
 /* Callback called by the library upon receiving incoming call */
static void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data *rdata)
{
	pjsua_call_info ci;
 	PJ_UNUSED_ARG(acc_id);
	PJ_UNUSED_ARG(rdata);
 
	pjsua_call_get_info(call_id, &ci);

	PJ_LOG(3,(THIS_FILE, "Incoming call from %.*s!!",
	(int)ci.remote_info.slen,
	ci.remote_info.ptr));

	/* Automatically answer incoming calls with 200/OK */
	pjsua_call_answer(call_id, 200, NULL, NULL);
}
 
 /* Callback called by the library when call's state has changed */
static void on_call_state(pjsua_call_id call_id, pjsip_event *e)
{
	pjsua_call_info ci;

	PJ_UNUSED_ARG(e);

	pjsua_call_get_info(call_id, &ci);
	PJ_LOG(3,(THIS_FILE, "Call %d state=%.*s", call_id,
	(int)ci.state_text.slen,
	ci.state_text.ptr));
}
 
 /* Callback called by the library when call's media state has changed */
static void on_call_media_state(pjsua_call_id call_id)
{
	pjsua_call_info ci;
	pjsua_call_get_info(call_id, &ci);
 
	if (ci.media_status == PJSUA_CALL_MEDIA_ACTIVE) {
	// When media is active, connect call to sound device.
		pjsua_conf_connect(ci.conf_slot, 0);
		pjsua_conf_connect(0, ci.conf_slot);
	}
}
 
 /* Display error and exit application */
static void error_exit(const char *title, pj_status_t status)
{
	pjsua_perror(THIS_FILE, title, status);
	pjsua_destroy();
	exit(1);
}

 /*
  * main()
  *
  * argv[1] may contain URL to call.
  */
 int main(int argc, char *argv[])
 {

 	pjsua_acc_id acc_id;
	pj_status_t status;
	pjsua_config cfg;
	pjsua_logging_config log_cfg;
	pjsua_transport_config UDPcfg;
 


	/* Create pjsua first! */
	status = pjsua_create();
	if (status != PJ_SUCCESS) error_exit("Error in pjsua_create()", status);
	 
	  /* If argument is specified, it's got to be a valid SIP URL */
	if (argc > 1) {
	  	status = pjsua_verify_url(argv[1]);
		if (status != PJ_SUCCESS) error_exit("Invalid URL in argv", status);
	}
 
  /* Init pjsua */
  
	pjsua_config_default(&cfg);
	cfg.cb.on_incoming_call = &on_incoming_call;
	cfg.cb.on_call_media_state = &on_call_media_state;
	cfg.cb.on_call_state = &on_call_state;

	pjsua_logging_config_default(&log_cfg);
	log_cfg.console_level = 4;

	status = pjsua_init(&cfg, &log_cfg, NULL);
	if (status != PJ_SUCCESS) error_exit("Error in pjsua_init()", status);



 	
 
  /* Add UDP transport. */

	pjsua_transport_config_default(&UDPcfg);
	UDPcfg.port = PORT;
	status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &UDPcfg, NULL);
	if (status != PJ_SUCCESS) error_exit("Error creating transport", status); 
	  /* Initialization is done, now start pjsua */
	status = pjsua_start();
	if (status != PJ_SUCCESS) error_exit("Error starting pjsua", status);
	 
	  /* Register to SIP server by creating SIP account. */
	pjsua_acc_config  ACCcfg;
	pjsua_acc_config_default(&ACCcfg);
	ACCcfg.id = pj_str("sip:" SIP_USER "@" SIP_DOMAIN);
	ACCcfg.reg_uri = pj_str("sip:" SIP_DOMAIN);
	ACCcfg.cred_count = 1;
	ACCcfg.cred_info[0].realm = pj_str(SIP_DOMAIN);
	ACCcfg.cred_info[0].scheme = pj_str("digest");
	ACCcfg.cred_info[0].username = pj_str(SIP_USER);
	ACCcfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
	ACCcfg.cred_info[0].data = pj_str(SIP_PASSWD);

	status = pjsua_acc_add(&ACCcfg, PJ_TRUE, &acc_id);
	if (status != PJ_SUCCESS) error_exit("Error adding account", status);
 
	  /* If URL is specified, make call to the URL. */
	if (argc > 1) {
		pj_str_t uri = pj_str(argv[1]);
	  	status = pjsua_call_make_call(acc_id, &uri, 0, NULL, NULL, NULL);
	  	if (status != PJ_SUCCESS) error_exit("Error making call", status);
	}
	 
	  /* Wait until user press "q" to quit. */
	for (;;) {
	  	char option[10];
	 
	  	puts("Press 'h' to hangup all calls, 'q' to quit");
	  	if (fgets(option, sizeof(option), stdin) == NULL) {
	  		puts("EOF while reading stdin, will quit now..");
	  		break;
		}
	 
	  	if (option[0] == 'q')
	  		break;
	 
	  	if (option[0] == 'h')
	  		pjsua_call_hangup_all();
	}
	 
	  /* Destroy pjsua */
	pjsua_destroy();
	 
	  	return 0;
 }