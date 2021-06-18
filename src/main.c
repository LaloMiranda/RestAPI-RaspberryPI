#include "../include/main.h"


#define PORT 8080
#define STR_SIZE 256

int main(void) {
  struct _u_instance instance;

  // Initialize instance with the port number
  if (ulfius_init_instance(&instance, PORT, NULL, NULL) != U_OK) {
    fprintf(stderr, "Error ulfius_init_instance, abort\n");
    return(1);
  }

  // Endpoint list declaration
  ulfius_add_endpoint_by_val(&instance, "GET", "/helloworld", NULL, 0, &callback_hello_world, NULL);
  ulfius_add_endpoint_by_val(&instance, "GET", "state", NULL, 0, &callback_get_users, NULL);
  ulfius_set_default_endpoint(&instance, &callback_users_default, NULL);

  // Start the framework
  if (ulfius_start_framework(&instance) == U_OK) {
    printf("Start framework on port %d\n", instance.port);
    /*while(1);*/getchar();
  } else {
    fprintf(stderr, "Error starting framework\n");
  }
  printf("End framework\n");

  ulfius_stop_framework(&instance);
  ulfius_clean_instance(&instance);

  return 0;
}

int callback_hello_world (const struct _u_request * request, struct _u_response * response, void * user_data) {
  ulfius_set_string_body_response(response, 200, "Hello World!");
  return U_CALLBACK_CONTINUE;
}

int callback_get_users(const struct _u_request * request, struct _u_response * response, void * user_data){
  //Creo un jsonArray
  json_t* jsonArray = json_array();
  //Estructuras de json para carga de datos
  json_t* js, *jsResponse;

  
  char* tempStr;
  tempStr = getCurrentTemp();
  js = json_pack("{s:s}", "Temperature", tempStr);
  json_array_append(jsonArray, js);

  tempStr = getMemUsage();
  js = json_pack("{s:s}", "RAM usage", tempStr);
  json_array_append(jsonArray, js);

  tempStr = getCPULoad();
  js = json_pack("{s:s}", "CPU usage", tempStr);
  json_array_append(jsonArray, js);
  

  
  jsResponse = json_pack("{s:o}", "Data", jsonArray);
 
  ulfius_set_json_body_response(response, 200, jsResponse);

  //ulfius_set_string_body_response(response, 404, "Here will be my state");
  return U_CALLBACK_CONTINUE;
}

int callback_users_default (const struct _u_request * request, struct _u_response * response, void * user_data){
  ulfius_set_string_body_response(response, 404, "Not Found");
  return U_CALLBACK_CONTINUE;
}

char* getCurrentTime(){
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);

  char* currentTime;
  currentTime = malloc(sizeof(char)*256);

  memset(currentTime, '\0', STR_SIZE);
  sprintf(currentTime, "%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

  return currentTime;
}

char* getCurrentTemp(){
  char line[STR_SIZE];

  FILE *fp;
  fp = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
  if(fp == NULL){
    return NULL;
  }
  fgets(line, STR_SIZE, fp);
  fclose(fp);

  double temp = atoi(line) * 0.001;
  char* tempStr;
  tempStr = malloc(sizeof(char)*STR_SIZE);

  sprintf(tempStr, "%0.2f °C", temp);

  return tempStr;
}

char* getMemUsage(){
  char line[STR_SIZE];
  float totalMem, freeMem;

  char* tempStr;
  tempStr = malloc(sizeof(char)*STR_SIZE);

  FILE *fp;
  fp = fopen("/proc/meminfo", "r");
  if(fp == NULL){
    return NULL;
  }
  fgets(line, STR_SIZE, fp);
  totalMem = (float)atoi(getNWord(line, " ", 2)) / 1048576;
  fgets(line, STR_SIZE, fp);
  freeMem = (float)atoi(getNWord(line, " ", 2)) / 1048576;
  fclose(fp);

  sprintf(tempStr, "%0.2f %%", freeMem/totalMem);
  return tempStr;
}

char* getCPULoad() {
  char line[STR_SIZE];
  double load;

  char* tempStr;
  tempStr = malloc(sizeof(char)*STR_SIZE);

  FILE *fp;
  fp = fopen("/proc/loadavg", "r");
  if(fp == NULL){
    return NULL;
  }
  fgets(line, STR_SIZE, fp);
  load = atof(getNWord(line, " ", 1)) * 100 / get_nprocs();
	fclose(fp);

	sprintf(tempStr,"%0.2f %%", load);
  return tempStr;
}

char* getNWord(char* line, const char* delim, int N){
  char* token;
  token = strtok(line, delim);
  N--;
  while(N > 0){
    token = strtok(NULL, delim);
    N--;
  }
  return token;
}