#pragma once

#define SET 1
#define NOTSET 0

#define SUCCESS 1
#define FAILURE 0

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1


#define ERROR -1
#define DEFAULT 0

/*
** Server Config
*/
// index
#define S_INDEX 1
// lister = port
#define S_LISTEN 2
// server_name hostname
#define S_SERVER_NAME 3
// root_dir
#define S_ROOT 4

#define S_LOCATION 5
#define S_DENY 7
// auto_index allowdirectorylisting
#define S_AUTO_INDEX 14
// error_page
#define S_ERROR_PAGE 9
// client_max_size
#define S_CLIENT_MAX_SIZE 10

// not used
#define S_UPLOAD_FILE 11
// not used
#define S_UPLOAD_PATH 12
#define S_METHOD_ALLOW 13


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
#define L_CGI_SCRIPT 11
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


/*
* Color
*/

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"
#define RESET "\033[0m"


/*
* default thing
*/

#define DEFAULT_PORT "8080"
#define DEFAULT_SERVER_NAME "localhost"
#define DEFAULT_CLIENT_MAX_SIZE 3000000
#define DEFAULT_ROOT_DIR "./www/"
