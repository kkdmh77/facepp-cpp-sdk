#include "detection.h"

detection::detection()
{
}

detection::~detection()
{
}

void detection::detect(bool init)
{
	pplx::task<void> requestTask = concurrency::streams::fstream::open_ostream(U("results.json")).then([=](concurrency::streams::ostream outFile)
	{
		http_client client(API_SERVER);

		query.append_path(U("/detection/detect"));
		query.append_query(U("api_key"), API_KEY.c_str());
		query.append_query(U("api_secret"), API_SECRET.c_str());
		query.append_query(U("url"), "http://www.faceplusplus.com/static/img/demo/1.jpg");

#if __DEBUG__
		wcout << query.to_string() << endl;
#endif
		return client.request(methods::POST, query.to_string());
	})

		.then([=](http_response response)
	{
		cout << "Received response status code " << response.status_code() << endl;

		jsonResult = response.extract_json();
		result = jsonResult.get();

		wofstream res_json("results.json");
		res_json << jsonResult.get() << endl;
	});

	try
	{
		requestTask.wait();
	}
	catch (const std::exception &e)
	{
		cout << "Error exception " << e.what() << endl;
	}
}
