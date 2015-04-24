#include<iostream>
#include <CurlService.hpp>
#include <stdio.h>
#include <string.h>
#include <queue>
#include <sstream>
#include <fstream>
using namespace std;
#define NUMT 4
 
pthread_mutex_t request_queue_mutex_;
std::queue<Curl_Request> request_queue;
int put_request(Curl_Request& request){
	pthread_mutex_lock(&request_queue_mutex_);
	request_queue.push(request);
	pthread_mutex_unlock(&request_queue_mutex_);
	return 0;
	}
int get_request(Curl_Request& request){
	if(request_queue.size() > 0 ){
		pthread_mutex_lock(&request_queue_mutex_);
		request = request_queue.front();
		request_queue.pop();
	    pthread_mutex_unlock(&request_queue_mutex_);
		return 0;
	}
	else{
		std::cout<<"The request queue is empty" <<std::endl;
		return -1;
		}
	}
 
int pull_one_url(CurlService* service){
	Curl_Request request;
	while(get_request(request)!= -1 ){
		Fetch_Result* result =  new Fetch_Result;
		result->header = new string();
		result->html = new string();
		request.fetch_result = result;
		service -> Exec_Request(request);
		}
	fprintf(stderr, "no request,return\n");
	return 0;
	}
 
static void *start_one_curl(void* null)
{
    CurlService* service =  new CurlService();
    service -> Curl_Init();
	pull_one_url(service);
  	return NULL;
}
int load_seeds(const char* filename){
	std::ifstream ifs(filename);
	std::string line;
	std::istringstream iss;
	std::string url;
	std::string enable_redir;
	std::string redir_depth;
	std::string  fetch_type;
	while(std::getline (ifs, line)){
		if(line.empty())
			continue;
		iss.clear();
		iss.str(line);
		if (iss >> url >> enable_redir >>  redir_depth >>fetch_type){
			Curl_Params* params = new Curl_Params ;
			//istringstream(enable_redir) >> boolalpha >> (params -> enable_redir);
			//istringstream(redir_depth) >> (params -> redir_depth);
			params -> fetch_type = Get;//todo
			Curl_Request request;
			//request.url = url;
			request.url = "http://www.baidu.com";
			params -> enable_redir =  true;
			params -> redir_depth  = 1;
			params -> fetch_type = Get;
			request.curl_params = params;
		    put_request(request);
		}
		continue;
	}
	return 0;
}
int main(){
	int i;
	int error;
	load_seeds("seeds.txt");
	pthread_t tid[NUMT];
	for(i=0; i< NUMT; i++) {
		error = pthread_create(&tid[i],
                           NULL, /* default attributes please */ 
                           start_one_curl,
                           NULL);
    if(0 != error)
      fprintf(stderr, "Couldn't run thread number %d, errno %d\n", i, error);
    else
      fprintf(stderr, "Thread %d run\n", i);
   }
  for(i=0; i< NUMT; i++) {
    error = pthread_join(tid[i], NULL);
    fprintf(stderr, "Thread %d terminated\n", i);
  }
 
  return 0;
}
