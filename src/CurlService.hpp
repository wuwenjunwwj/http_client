#include <curl/curl.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
enum Fetch_Type{
	Get = 0,
	Post
	};
struct Curl_Params{
	bool enable_redir;
	int  redir_depth;
	Fetch_Type fetch_type;
};
struct Fetch_Result{
	std::string* header;
	std::string* html;
	int statu_code;
	};
struct Curl_Request{
	std::string url;
	const char* post_data;
	Curl_Params* curl_params;
	Fetch_Result* fetch_result;
	};
class CurlService{
	public:
		int Curl_Init();
		int curl_cleanup();
		void Set_Params(Curl_Params params);
		int Exec_Request(Curl_Request);
	private:
		CURL *curl_handle;
		CURLcode Curl_Post(Curl_Request& request);
		CURLcode curlservice_easy_perform(Curl_Request& request);
		CURLcode Curl_Get(Curl_Request& request);
		FILE* pagefile;

	};
