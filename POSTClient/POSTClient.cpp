// POSTClient.cpp 
//

#include "stdafx.h"

#pragma once

#include "targetver.h"

#include "POSTCLIENT.h"

using namespace std;
using namespace web::http;
using namespace web::http::client;
using namespace Concurrency::streams;
using namespace std::chrono;
using namespace utility;


using namespace TManager;
using namespace XMLLite;


// Upload a file to an HTTP server.
pplx::task<void> UploadFileToHttpServerAsync()
{
	using concurrency::streams::file_stream;
	using concurrency::streams::basic_istream;

	

	// To run this example, you must have a file named out.txt in the current folder.
	// Alternatively, you can use the following code to create a stream from a text string.
	// std::string s("abcdefg");
	// auto ss = concurrency::streams::stringstream::open_istream(s);

	// Open stream to file.
	wstring filePath = L"out.txt";
	auto previousTask = co_await file_stream<unsigned char>::open_istream(filePath);
	try
	{
		auto fileStream = previousTask;

		// Make HTTP request with the file stream as the body.
		http_client client(XMLReader::data.URL);



		//uri_builder uri(L"http://kenhero2.homepc.it:3557/");
		//auto addr = uri.to_string();

		//http_client client(addr);






		auto previousTask2 = co_await client.request(methods::POST, filePath, fileStream);
		fileStream.close();

		std::wostringstream ss;
		try
		{
			auto httpResponse = previousTask2;
			ss << L"Server returned returned status code " << httpResponse.status_code() << L"." << std::endl;

			if (httpResponse.status_code() == status_codes::OK)
			{
				auto previousTask3 = co_await httpResponse.extract_utf16string();
				auto response = previousTask3;
				wcout << response << endl;
				return;
						
			}


		}
		catch (const http_exception& e)
		{
			ss << e.what() << std::endl;
		}
		catch (const std::runtime_error& e)
		{
			ss << e.what() << std::endl;
		}
		catch (const exception& e)
		{
			ss << e.what() << std::endl;
		}
			std::wcout << ss.str();

			
	}
	catch (const std::runtime_error& e)
	{
		std::wostringstream ss;
		ss << e.what() << std::endl;
		std::wcout << ss.str();

		// Return an empty task.
		return ;
	}
	

	/* Sample output:
	The request must be resent
	*/
}

//read file func
vector<char> readFile(const string& inPath)
{
	ifstream file(inPath, ios::binary | ios::ate);
	size_t length = (size_t)file.tellg();
	vector<char> buffer(length);
	file.seekg(0, std::ios::beg);
	file.read(&buffer[0], length);
	return buffer;
}

//write file func
size_t writeFile(const vector<char>& buffer, const string& outPath)
{
	ofstream file(outPath, ios::app);
	file.write(&buffer[0], buffer.size());
	return (size_t)file.tellp();
}







int _tmain(int argc, _TCHAR* argv[])
{
	using concurrency::streams::file_stream;
	using concurrency::streams::basic_istream;

	//	wstring baseUrl = L"http://localhost:3557";

	uri_builder uri(L"http://kenhero2.homepc.it:3557/");
	auto addr = uri.to_string();

	http_client client1(addr);

	client1.request(methods::GET, addr).then([=](http_response response)
	{
		printf("Response status code %u returned.\n", response.status_code());
		//return pplx::task_from_result(true);
		if (response.status_code() == status_codes::OK)
		{
			wstring output = response.extract_utf16string().get();
			wcout << output << endl;
		}
	}).wait();


	/*

	.then([=](pplx::task<size_t>)
	{
	return pplx::task_from_result(true);
	});



	*/


	TaskManager::CLientInfo();

	// synch POST request 
	/*
	wstring baseUrl = XMLReader::data.URL;
	http_client httpClient(baseUrl);
	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	try
	{


	wstring filePath = L"..\\MemoryUsageInfo\\out.txt";
	Concurrency::streams::basic_istream<unsigned char> inputStream = file_stream<unsigned char>::open_istream(filePath).get();

	auto httpResponse = httpClient.request(methods::POST, L"out.txt", inputStream).get();

	if (httpResponse.status_code() == status_codes::OK)
	{
	wstring output = httpResponse.extract_utf16string().get();
	wcout << output << endl;
	}


	//UploadFileToHttpServerAsync().wait();
	}
	catch (const std::exception& e)
	{
	std::wcout << e.what() << endl;
	}

	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(t2 - t1).count();
	cout << "duration time primo put,credo sincrono =:" << duration << endl;
	*/

	//auto t1 = high_resolution_clock::now();

	UploadFileToHttpServerAsync().wait();
	//auto t2 = high_resolution_clock::now();
	// auto duration = duration_cast<microseconds>(t2 - t1).count();
	//cout << "duration time asynch put =:" << duration << endl;


	return 0;
}
































