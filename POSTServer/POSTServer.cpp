// POSTServer.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "POSTServer.h"
#include "XmlLiteServerDll.h"
#include "AsincMailDll.h"
#include <ppltasks.h>
#include <atomic>
using namespace std;
using namespace concurrency;
using namespace Concurrency::streams;
using namespace AsyncMailDll;
using namespace XMLLiteServer;

using concurrency::streams::file_stream;
using concurrency::streams::basic_istream;

#define TRACE(msg)            wcout << msg
#define TRACE_ACTION(a, k, v) wcout << a << L" (" << k << L", " << v << L")\n"

std::atomic<int> suffix(0);

namespace POSTServer
{

	void Server::handle_error(pplx::task<void>& t)
	{
		try
		{
			t.get();
		}
		catch (...)
		{
			// Ignore the error, Log it if a logger is available 
		}
	}



	void Server::handle_get(http_request request)
	{
		TRACE(L"\nhandle GET\n");


		request.reply(status_codes::OK, U("Hello, GET reply!"));
	}



	pplx::task<void> Server::open()
	{
		return m_listener.open().then([](pplx::task<void> t) { handle_error(t); });
	}

	pplx::task<void> Server::close()
	{
		return m_listener.close().then([](pplx::task<void> t) { handle_error(t); });
	}

	Server::Server(utility::string_t url) : m_listener(url)
	{

		m_listener.support(methods::POST, [this](http_request request) {this->handle_post(request); });
		m_listener.support(methods::GET, [this](http_request request) {this->handle_get(request); });

	}




	//Input file stream reading

	std::vector<char> Server::readFile(const string& inPath)
	{
		ifstream file(inPath, ios::binary | ios::ate);
		size_t length = (size_t)file.tellg();
		std::vector<char> buffer(length);
		file.seekg(0, std::ios::beg);
		file.read(&buffer[0], length);
		return buffer;
	}

	//Output file stream writing
	size_t Server::writeFile(const vector<char>& buffer, const string& outPath)
	{
		ofstream file(outPath, ios::app);
		file.write(&buffer[0], buffer.size());
		return (size_t)file.tellp();
	}


	//Task for copying a file in async mode
	Concurrency::task<size_t> Server::ppl_copyFile(const string& inFile, const string& outFile)
	{
		return Concurrency::create_task([inFile]()
		{
			return Server::readFile(inFile);
		})
			.then([outFile](const vector<char>& buffer)
		{
			return Server::writeFile(buffer, outFile);
		});

	}




	void ManageClient(string_t downloaded)
	{
		BOOL Result;
		wstring strData;
		wifstream inf;
		const wstring Config_file = L"SetupClients.xml";
		inf.open(downloaded, std::ios::beg);
		if (inf.fail())
		{
			cerr << " Error reading file from client" << endl;
			return;
		}
		data_t ClientInfo;

		getline(inf, strData);
		ClientInfo.memUsed = strData;
		getline(inf, strData);
		getline(inf, strData);
		ClientInfo.NumProcess = strData;
		getline(inf, strData);
		ClientInfo.CpuUsed = strData;
		getline(inf, strData);
		ClientInfo.Log = strData;
		getline(inf, strData);
		ClientInfo.Pass = strData;


		Result = XMLReader::XmlFinder(Config_file.c_str(), ClientInfo.Log.c_str(), ClientInfo.Pass.c_str());
		if (!Result)
		{
			std::wcout << L"client unknown";
		}
		else
		{
			if ((std::stoi(ClientInfo.NumProcess) > stoi(XMLReader::data.NumProcess)) || (stoi(ClientInfo.CpuUsed) > stoi(XMLReader::data.CPULimit)) || (stoi(ClientInfo.memUsed) > stoi(XMLReader::data.memLimit)))
			{
				//system("   \"C:\\Users\\BernardinoBruno\\Documents\\Visual Studio 2013\\Projects\\CSmtp_v2_2_ssl\\Debug\\AsincMail.exe \"   ");
				AsyncMail::SendMail(XMLReader::data.Mail.c_str());
			}

			//convert wstring to string
			//	std::string DestFile((const char*)&ClientInfo.Log, sizeof(wchar_t) / sizeof(char)*ClientInfo.Log.size());
			std::string DestFile(ClientInfo.Log.begin(), ClientInfo.Log.end());
			DestFile.append(".txt");
			try
			{
				std::string source(downloaded.begin(), downloaded.end());
				Server::ppl_copyFile(source, DestFile).then([](pplx::task<size_t> t)
				{
					try
					{
						t.get();
					}
					catch (...)
					{
						cout << "error copying file" << "\n";
					}
				});
			}
			catch (exception const & e)
			{
				wcout << e.what() << endl;
			}
		}


	}


	//Method to handle POST service
	pplx::task<void> Server::handle_post(http_request request)
	{
		TRACE("\nhandle POST\n");
		auto fileBuffer = std::make_shared<Concurrency::streams::basic_ostream<unsigned char>>();
		string_t download = L"download";
		suffix++;
		std::wstringstream wss;
		wss << suffix << (".txt");

		download.append(wss.str());

		try
		{
			auto Previous_task = co_await concurrency::streams::fstream::open_ostream(download, std::ios_base::out | std::ios_base::binary);

			*fileBuffer = Previous_task;
			try
			{
				request.body().read_to_end(fileBuffer->streambuf()).get();
				fileBuffer->flush();
			}
			catch (const exception&)
			{
				wcout << L"<exception>" << std::endl;
				//return pplx::task_from_result();
			}
			Previous_task.close();



			fileBuffer->close();
			//Previous_task.get();

			ManageClient(download);
			try
			{
				request.reply(status_codes::OK, U("Hello, World!")).get();

			}
			catch (const exception& e)
			{
				wcout << e.what() << endl;
			}

			//stream.get().close();
		}
		catch (const exception& e)
		{
			wcout << e.what() << endl;
		}

		//request.extract_string(); vedere se usare questa oppure quella di sotto
		/*try
		{
		request.body().read_to_end(stream.streambuf()).wait();
		stream.close().get();
		}
		catch (http_exception const & e)
		{
		wcout << e.what() << endl;
		}
		*/
		
		//return;


	}

}
