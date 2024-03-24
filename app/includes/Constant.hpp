#pragma once

#define SET 1
#define NOTSET 0

#define SUCCESS 1

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1


#define ERROR -1
#define DEFAULT 0

/*
** Server Config
*/
#define S_INDEX 1
#define S_LISTEN 2
#define S_SERVER_NAME 3
#define S_ROOT 4
#define S_LOCATION 5
#define S_DENY 7
#define S_AUTOINDEX 8
#define S_ERROR_PAGE 9
#define S_CLIENT_MAX_SIZE 10
#define S_UPLOAD_FILE 11
#define S_UPLOAD_PATH 12
#define S_METHOD_ALLOW 13
#define S_AUTO_INDEX 14


/*
* Location Config
*/
#define L_ROOT_DIR 1
#define L_DENY 3
#define L_AUTOINDEX 4
#define L_ERROR_PAGE 5
#define L_CLIENT_MAX_SIZE 6
#define L_UPLOAD_FILE 7
#define L_UPLOAD_PATH 8
#define L_METHOD_ALLOW 9
#define L_CGI_HANDLER 2
#define L_CGI_PATH 11
#define L_INDEX 12

/*
* Http Method
*/
#define GET 1
#define POST 2
#define PUT 4
#define DELETE 8
#define HEAD 16
#define OPTIONS 32
#define TRACE 64
#define CONNECT 128
