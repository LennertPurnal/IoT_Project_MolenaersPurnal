/* Header file includes. */
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

/* FreeRTOS header file. */
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

/* Standard C header file. */
#include <string.h>

/* Cypress secure socket header file. */
#include "cy_secure_sockets.h"

/* Wi-Fi connection manager header files. */
#include "cy_wcm.h"
#include "cy_wcm_error.h"

/* HTTP Client Library*/
// TODO: #include the http_client library api header file
#include "cy_http_client_api.h"

#include "http_methods.h"

void disconnect_callback(void *arg);

bool connected;

/*******************************************************************************
* Macros
********************************************************************************/
#define BUFFERSIZE							(2048 * 2)
#define SENDRECEIVETIMEOUT					(5000)
#define HTMLRESOURCE						"/UnixTime/tounix?date=now"
#define ANYTHINGRESOURCE					"/anything"


uint32_t get_unix_timestamp(){
	cy_rslt_t result;

	result = cy_http_client_init();

	if(result != CY_RSLT_SUCCESS){
		printf("HTTP Client Library Initialization Failed!\n");
		CY_ASSERT(0);
	}

	// Server Info
	cy_awsport_server_info_t serverInfo;
	(void) memset(&serverInfo, 0, sizeof(serverInfo));
	// TODO: Populate the server object's member data. Use the macros that you defined above.
	//const char *host_name = "httpbin.org";
	//uint16_t port = 80;
	serverInfo.host_name=SERVERHOSTNAME;
	serverInfo.port=SERVERPORT;

	// Disconnection Callback
	cy_http_disconnect_callback_t disconnectCallback = (void*)disconnect_callback;

	// Client Handle
	cy_http_client_t clientHandle;

	// Create the HTTP Client
	// TODO: Add the function call to create the http client
	result = cy_http_client_create(NULL, &serverInfo, disconnectCallback, NULL, &clientHandle);

	if(result != CY_RSLT_SUCCESS){
		printf("HTTP Client Creation Failed!\n");
		CY_ASSERT(0);
	}

	// Connect to the HTTP Server
	// TODO: Add the function call to connect to the http server. Use the macro for the send and receive timeouts.
	result = cy_http_client_connect(clientHandle, SENDRECEIVETIMEOUT, SENDRECEIVETIMEOUT);

	if(result != CY_RSLT_SUCCESS){
		printf("HTTP Client Connection Failed!\n");
		CY_ASSERT(0);
	}
	else{
		printf("\nConnected to HTTP Server Successfully\n\n");
		connected = true;
	}
	// Create Request
	uint8_t buffer[BUFFERSIZE];
	cy_http_client_request_header_t request;
	request.buffer = buffer;
	request.buffer_len = BUFFERSIZE;

	// TODO: Pick the correct method to perform a GET from the enumeration
	request.method = CY_HTTP_CLIENT_METHOD_GET;
	request.range_start = -1;
	request.range_end = -1;
	request.resource_path = HTMLRESOURCE;

	// Create Header
	cy_http_client_header_t header;
	uint32_t num_header = 1;
	// TODO: Populate the header field with "Host" and the header value with the name of the server you're connecting to.
	//       Use the macro for the server host name for the value entry.
	//       Don't forget the lengths!
	header.field = "Host";
	header.field_len = strlen("Host");
	header.value = SERVERHOSTNAME;
	header.value_len = strlen(SERVERHOSTNAME);
	num_header = 1;


	// TODO: Add the function call to write the headers
	result = cy_http_client_write_header(clientHandle, &request, &header, num_header);
	if(result != CY_RSLT_SUCCESS){
		printf("HTTP Client Header Write Failed!\n");
		CY_ASSERT(0);
	}

	if(result != CY_RSLT_SUCCESS){
		printf("HTTP Client Header Write Failed!\n");
		CY_ASSERT(0);
	}

	// Var to hold the servers responses
	cy_http_client_response_t response;

	// Send get request to /html resource
	if(connected){
		result = cy_http_client_send(clientHandle, &request, NULL, 0, &response);
		if(result != CY_RSLT_SUCCESS){
			printf("HTTP Client Send Failed!\n");
			CY_ASSERT(0);
		}
	}
	else{
		// Connect to the HTTP Server
		result = cy_http_client_connect(clientHandle, SENDRECEIVETIMEOUT, SENDRECEIVETIMEOUT);
		if(result != CY_RSLT_SUCCESS){
			printf("HTTP Client Connection Failed!\n");
			CY_ASSERT(0);
		}
		else{
			printf("\nConnected to HTTP Server Successfully\n\n");
			connected = true;
		}
		// Send get request to /html resource
		result = cy_http_client_send(clientHandle, &request, NULL, 0, &response);
		if(result != CY_RSLT_SUCCESS){
			printf("HTTP Client Send Failed!\n");
			CY_ASSERT(0);
		}
	}

	/*
	for(int i = 0; i < response.body_len; i++){
		printf("%c", response.body[i]);
	}
	*/
	char response_string[response.body_len-2];
	for(int i = 1; i < response.body_len-1; i++){
			//printf("%c", response.body[i]);
			response_string[i-1] = response.body[i]; //copy the characters to a string
	}
	response_string[response.body_len-1] = '\0';
	//printf("%s", response_string);
	//printf("\r\n");

	uint32_t response_int = (uint32_t) strtol(response_string, (char **)NULL, 10);
	printf("GET unix timestamp returned %lu \r\n", response_int);

	return response_int;
}

void disconnect_callback(void *arg){
    printf("Disconnected from HTTP Server\n");
    connected = false;
}
