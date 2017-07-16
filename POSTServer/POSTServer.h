#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include <cpprest\filestream.h>
#include <Windows.h>
#include <exception>
#include <cpprest\http_listener.h>

//#include <future>
#ifdef _RESUMABLE_FUNCTIONS_SUPPORTED
#include <experimental/resumable>
#endif

#include <pplawait.h>

using namespace std;
using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

namespace POSTServer
{

	struct data_t
	{
		std::wstring NumProcess;
		std::wstring Log;
		std::wstring Pass;
		std::wstring memUsed;
		std::wstring CpuUsed;

	};



	//
	// This class implements the PostServer server functionality.
	// It maintains a http_listener to listen for requests.

	//
	class Server
	{
	public:
		Server() {}
		Server(utility::string_t url);

		pplx::task<void> open();
		pplx::task<void> close();

		void handle_get(web::http::http_request message);
		pplx::task<void> handle_post(web::http::http_request message);
		static Concurrency::task<size_t> ppl_copyFile(const string& inFile, const string& outFile);
		// Error handlers
		static void handle_error(pplx::task<void>& t);

	private:
		static std::vector<char> readFile(const string& inPath);
		static size_t writeFile(const vector<char>& buffer, const string& outPath);


		// HTTP listener 
		web::http::experimental::listener::http_listener m_listener;


	};

}





//static std::vector<char> readFile(const string& inPath);
//static size_t writeFile(const vector<char>& buffer, const string& outPath);
//static Concurrency::task<size_t> ppl_copyFile(const string& inFile, const string& outFile);

































