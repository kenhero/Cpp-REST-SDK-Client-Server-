// UmitTest_MyServer.cpp : definisce il punto di ingresso dell'applicazione console.
//
#pragma once

#include "stdafx.h"

// UnitTest_MyServer.cpp : definisce il punto di ingresso dell'applicazione console.
//

#include "UnitTest_MyServer.h"

#include "POSTServer.cpp"

using namespace std;
using namespace concurrency;
using namespace Concurrency::streams;
using namespace AsyncMailDll;
using namespace XMLLiteServer;
using namespace POSTServer;

using namespace TManager;
using namespace XMLLite;

using concurrency::streams::file_stream;
using concurrency::streams::basic_istream;



#define TRACE(msg)            wcout << msg
//std::atomic<int> suffix(0);


TEST(testClient, POSTtest)
{
	using concurrency::streams::file_stream;
	using concurrency::streams::basic_istream;

	TaskManager::CLientInfo();
	// Open stream to file.
	wstring filePath = L"out.txt";
	file_stream<unsigned char>::open_istream(filePath).then([=](pplx::task<basic_istream<unsigned char>> previousTask)
	{
		try
		{
			auto fileStream = previousTask.get();

			// Make HTTP request with the file stream as the body.
			web::http::client::http_client client(XMLLite::XMLReader::data.URL);

			return client.request(methods::POST, filePath, fileStream).then([fileStream](pplx::task<http_response> previousTask)
			{
				fileStream.close();

				std::wostringstream ss;
				try
				{
					auto httpResponse = previousTask.get();
					//ss << L"Server returned returned status code " << httpResponse.status_code() << L"." << std::endl;
					EXPECT_EQ(status_codes::OK, httpResponse.status_code());

					if (httpResponse.status_code() == status_codes::OK)
					{
						httpResponse.extract_utf16string().then([=](pplx::task<utility::string_t> previousTask)
						{
							auto response = previousTask.get();
							//wcout << response << endl;
							std::string res(response.begin(), response.end());
							EXPECT_EQ("Hello, World!", res);
						});
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

			});
		}
		catch (const std::runtime_error& e)
		{
			std::wostringstream ss;
			ss << e.what() << std::endl;
			std::wcout << ss.str();

			// Return an empty task.
			return pplx::task_from_result();
		}
	});




}






TEST(testServer, POSTtest)
{











	/*
	uri_builder uri(L"http://kenhero2.homepc.it:3557/");
	//uri_builder uri(L"http://localhost:80/");
	auto addr = uri.to_string();
	POSTServer::Server listener(addr);


	try
	{
		listener
			.open()
			.then([&listener]() {TRACE(L"\nstarting to listen\n"); })
			.wait();

		//while (true);
	}
	catch (exception const & e)
	{
		wcout << e.what() << endl;
	}
	wcout << "test string" << endl;
	std::string line;
	std::wcout << U("Hit Enter to close the listener.");
	std::getline(std::cin, line);

	listener.close().wait();

	*/
	wcout << "test string" << endl;

	
}

pplx::task<void> InitServer()
{
	
		uri_builder uri(L"http://kenhero2.homepc.it:3557/");
		//uri_builder uri(L"http://localhost:80/");
		auto addr = uri.to_string();
		POSTServer::Server listener(addr);


		try
		{
			return listener
				.open()
				.then([&listener]() {TRACE(L"\nstarting to listen\n"); });
				
				

			//while (true);
		}
		catch (exception const & e)
		{
			wcout << e.what() << endl;
			return pplx::task_from_result();
		}

		
		


	
}


int _tmain(int argc, _TCHAR* argv[])
{

	InitServer().then([&]{
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	
	});
	std::string line;
	std::wcout << U("Press any key to continue \n");
	std::getline(std::cin, line);
	//auto t = InitServer().wait();
	

	/*
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	std::string line;
	std::wcout << U("Press any key to continue \n");
	std::getline(std::cin, line);
	*/

}


