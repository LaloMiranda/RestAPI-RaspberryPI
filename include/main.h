#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <ulfius.h>
#include <pwd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sysinfo.h>

/**
 * Callback function for the web application on /helloworld url call
 */
int callback_hello_world (const struct _u_request * request, struct _u_response * response, void * user_data);

/**
 * @brief  Callback function for the web application on /api/users in order to display user id's and names
*/ 
int callback_get_users(const struct _u_request * request, struct _u_response * response, void * user_data);

/**
 * @brief Callback function for the web application in order to redirect to display Error 404
*/
int callback_users_default (const struct _u_request * request, struct _u_response * response, void * user_data);

/**
 * @brief Get currentTime
 * 
 * @return char* 
 */
char* getCurrentTime();

/**
 * @brief Get the Current Temperature of the Raspi
 * 
 * @return char* 
 */
char* getCurrentTemp();

char* getMemUsage();

char* getNWord(char* line, const char* delim, int N);

char* getCPULoad();
#endif