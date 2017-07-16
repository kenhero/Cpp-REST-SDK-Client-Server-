

#include "UnitTest_MyClient.h"

//Death Test:	Verify a statement will cause process to exit in a expected fashion


using namespace std;
using namespace web::http;
using namespace web::http::client;
using namespace Concurrency::streams;
using namespace std::chrono;
using namespace utility;


using namespace TManager;
using namespace XMLLite;

void function1() {

	std::cerr << "bad thing happened";
	exit(1);
}

TEST(testClient, GETtest)
{
	using concurrency::streams::file_stream;
	using concurrency::streams::basic_istream;

	//	wstring baseUrl = L"http://localhost:3557";

	uri_builder uri(L"http://kenhero2.homepc.it:3557/");
	auto addr = uri.to_string();

	http_client client1(addr);

	/*	client1.request(methods::GET, addr).then([=](http_response response)
	{
	printf("Response status code %u returned.\n", response.status_code());
	//return pplx::task_from_result(true);
	if (response.status_code() == status_codes::OK)
	{
	wstring output = response.extract_utf16string().get();
	wcout << output << endl;
	}
	}).wait();
	*/

	auto response = client1.request(methods::GET, addr).get();

	EXPECT_EQ(200, response.status_code());


}


TEST(testClient, POSTtest)
{
	using concurrency::streams::file_stream;
	using concurrency::streams::basic_istream;
	// teorically i should test TaskManager::ClientInfo() method ,for example if 5 and 6 raw is my login and password inside out.txt file!!!
	TaskManager::CLientInfo();
	// Open stream to file.
	wstring filePath = L"out.txt";
	file_stream<unsigned char>::open_istream(filePath).then([=](pplx::task<basic_istream<unsigned char>> previousTask)
	{
		try
		{
			auto fileStream = previousTask.get();

			// Make HTTP request with the file stream as the body.
			http_client client(XMLReader::data.URL);

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



int _tmain(int argc, _TCHAR* argv[])
{
	//testClient_firsttest_Test a;
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	std::string line;
	std::wcout << U("Press any key to continue \n");
	std::getline(std::cin, line);
}
