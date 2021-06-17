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
  json_t* js;

  char line[STR_SIZE];

  FILE *fp;
  fp = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
  fgets(line, STR_SIZE, fp);
  fclose(fp);

  double temp = atoi(line) * 0.001;
  char tempStr[STR_SIZE];

  sprintf(tempStr, "%0.2f °C", temp);

  js = json_pack("{s:s}", "Temperatura", tempStr);
 
  ulfius_set_json_body_response(response, 200, js);

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