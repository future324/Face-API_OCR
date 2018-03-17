#include<opencv2/opencv.hpp>  
#include<iostream>  
#include<vector>  
#include<string>  
#include"curl/curl.h"
#include"Zbase64.h"
#pragma warning (push)
#pragma warning (disable: 4996)
#include"json/json.h"
#pragma warning (pop)
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
using namespace std;
using namespace cv;
string imgurl = "http://s9.sinaimg.cn/mw690/aebf3a60gd9ac980d72c8&690";
#define POSTURL "https://api-cn.faceplusplus.com/imagepp/v1/recognizetext?api_key=jlgTxl2mUrOoYrCOy39lKmCNsCmbZ0Nw&api_secret=ROU_X2AD0dtwV78NTu9mRqpqz4O89rVu"
#pragma comment(lib,"lib_json.lib")
#pragma comment(lib,"LIBCURL.LIB")
int writer(char *data, size_t size, size_t nmemb, string *writerData)
{
	if (writerData == NULL)
		return 0;
	int len = size*nmemb;
	writerData->append(data, len);
	return len;
}

int main()
{
	
	Mat src = imread("timg.jpg");
	Mat gray, binary,edge;
	cvtColor(src, gray, COLOR_BGR2GRAY);
	blur(gray, edge, Size(3, 3));
	threshold(edge, binary, 100, 180, THRESH_BINARY);
	devView(binary);

	vector<uchar> vecImg;                               //Mat 图片数据转换为vector<uchar>  
	vector<int> vecCompression_params;
	vecCompression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	vecCompression_params.push_back(90);
	imencode(".jpg", binary, vecImg, vecCompression_params);

	ZBase64 base64;
	String imgbase64 = base64.Encode(vecImg.data(), vecImg.size());     //实现图片的base64编码  

	//cout << imgbase64 << endl;
	//ofstream ofile;               //定义输出文件
	//ofile.open("myfile.txt");     //作为输出文件打开
	//ofile << imgbase64 << endl;   //标题写入文件
	CURL *curl;
	CURLcode res;
	string buffer;
	struct curl_slist *http_header = NULL;
	struct curl_httppost *formpost = 0;
	struct curl_httppost *lastptr = 0;
	curl = curl_easy_init();


	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "api_key",
		CURLFORM_COPYCONTENTS, "jlgTxl2mUrOoYrCOy39lKmCNsCmbZ0Nw",
		CURLFORM_END);
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "api_secret",
		CURLFORM_COPYCONTENTS, "ROU_X2AD0dtwV78NTu9mRqpqz4O89rVu",
		CURLFORM_END);
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "image_base64",
		CURLFORM_COPYCONTENTS, imgbase64.c_str(),
		CURLFORM_END);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

	curl_easy_setopt(curl, CURLOPT_URL, "https://api-cn.faceplusplus.com/imagepp/v1/recognizetext");
	curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

	res = curl_easy_perform(curl);

	curl_easy_cleanup(curl);


	Json::Value root;
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(buffer, root);

	if (!parsingSuccessful)
	{
		cout << "Failed to parse the data!" << endl;
		exit(0);
	}

	//cout << buffer << endl;
	ofstream ofile;              
	ofile.open("result.json");     
	ofile << buffer << endl;
	ShellExecute(NULL, "open", "jsonmannger.py", NULL, NULL, SW_SHOWNORMAL);
	waitKey(0);
	//system("pause");
	return 0;
}