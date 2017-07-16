#include "stdafx.h"
#include "POSTServer.h"
//#include "XmlLiteReader.h"
//#include "AsincMail.h"



//#include <ppltasks.h>
using namespace std;
using namespace concurrency;
using namespace POSTServer;
//using namespace Concurrency::streams;

//using concurrency::streams::file_stream;
//using concurrency::streams::basic_istream;

#define TRACE(msg)            wcout << msg
#define TRACE_ACTION(a, k, v) wcout << a << L" (" << k << L", " << v << L")\n"



int _tmain(int argc, _TCHAR* argv[])
{
	uri_builder uri(L"http://kenhero2.homepc.it:3557/");
	//uri_builder uri(L"http://localhost:80/");
	auto addr = uri.to_string();
	Server listener(addr);

	
	try
	{
		listener
			.open()
			.then([&listener](){TRACE(L"\nstarting to listen\n"); })
			.wait();

		//while (true);
	}
	catch (exception const & e)
	{
		wcout << e.what() << endl;
	}

	std::string line;
	std::wcout << U("Hit Enter to close the listener.");
	std::getline(std::cin, line);

	listener.close().wait();



	return 0;

}
