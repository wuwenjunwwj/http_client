#include "CurlService.hpp"
using namespace std;
#define MAXHEADLEN 2000

static size_t writer_callback(void *ptr, size_t size, size_t nmemb, void *stream){
		char *buff = new char[size*nmemb+1];
		std::string* html = (std::string*)stream;
		memcpy(buff, ptr, size * nmemb);
    	buff[size * nmemb] = 0;
		memcpy(buff, ptr, size * nmemb);
    	std::string s = buff;
		html->append(buff);
		const char* content_type="Content-Type";
		const char* content="text/html";
		if(s.find(content_type) != string::npos && s.find (content, 6, 15) != string::npos){
			cout<<s<<endl;
			}
		delete[] buff;
		return nmemb * size;
}
int CurlService::Curl_Init(){
	curl_handle = curl_easy_init();
	curl_easy_setopt(curl_handle, CURLOPT_HEADER,1L);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION,writer_callback);
	return 0;

}
int CurlService::Exec_Request(Curl_Request request){
	std::cout<<"in  Exec_request"<<endl;

 	if(curl_handle) {
	 curl_easy_setopt(curl_handle, CURLOPT_URL, request.url.c_str());
	 curl_easy_setopt(curl_handle, CURLOPT_HEADERDATA, ((request.fetch_result)->header));
	 curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, ((request.fetch_result)->html));
	 if( (request.curl_params)->fetch_type == Get ) Curl_Get(request);
	 else 
		 if ((request.curl_params)->fetch_type == Post) Curl_Get(request);
	 }
	 return 0;
}


void CurlService::Set_Params(Curl_Params params){
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, (long)params.enable_redir); //alow redirect
    curl_easy_setopt(curl_handle, CURLOPT_MAXREDIRS, (long)params.redir_depth); //redirect depth
	}
int CurlService::curl_cleanup(){
	if(curl_handle){
		curl_easy_cleanup(curl_handle);
		return 0;
		}
	return -1;
	}

CURLcode CurlService::curlservice_easy_perform(Curl_Request& request){
	curl_easy_perform(curl_handle);
	cout<< *((request.fetch_result)->header)<<endl;
	cout<< "------------in curl perform"<<(request.url)<<endl;
	return CURLE_OK;
	}
CURLcode CurlService::Curl_Post(Curl_Request& request){
 	if(curl_handle) {
    curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, request.post_data);
 	curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, strlen(request.post_data));
	return curlservice_easy_perform(request);
	}
	return CURLE_COULDNT_CONNECT;
}

CURLcode CurlService::Curl_Get(Curl_Request& request){
 	if(curl_handle) {
		std::cout<<"in  get"<<endl;
		return curlservice_easy_perform(request);
	}
	return CURLE_COULDNT_CONNECT;
}
