
// POSTServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

using namespace Concurrency::streams;

using concurrency::streams::file_stream;
using concurrency::streams::basic_istream;

#define TRACE(msg)            wcout << msg
#define TRACE_ACTION(a, k, v) wcout << a << L" (" << k << L", " << v << L")\n"





void handle_post(http_request request)
{
	TRACE("\nhandle POST\n");

	auto stream = concurrency::streams::fstream::open_ostream(
		U("uploaded.txt"),
		std::ios_base::out | std::ios_base::binary).get();

	//request.extract_string(); vedere se usare questa oppure quella di sotto
	try
	{
		request.body().read_to_end(stream.streambuf()).wait();
		stream.close().get();
	}
	catch (http_exception const & e)
	{
		wcout << e.what() << endl;
	}
	request.reply(status_codes::OK, U("Hello, World!"));

}










int _tmain(int argc, _TCHAR* argv[])
{
	uri_builder uri(L"http://localhost:80/");
	auto addr = uri.to_string();
	http_listener listener(addr);



	listener.support(methods::POST, handle_post);

	try
	{
		listener
			.open()
			.then([&listener](){TRACE(L"\nstarting to listen\n"); })
			.wait();

		while (true);
	}
	catch (exception const & e)
	{
		wcout << e.what() << endl;
	}




	system("   \"C:\\Users\\BernardinoBruno\\Documents\\Visual Studio 2013\\Projects\\CSmtp_v2_2_ssl\\Debug\\AsincMail.exe \"   ");

	return 0;






}

