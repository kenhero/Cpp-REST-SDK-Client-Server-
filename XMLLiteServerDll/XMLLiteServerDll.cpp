

#include "XmlLiteServerDll.h"
//#include "pplx/pplxtasks.h"
#include <ppltasks.h>
#include <cpprest/filestream.h>

using namespace concurrency;

using namespace utility;
//using namespace concurrency::streams;


namespace XMLLiteServer
{

	XMLReader::XMLReader()
	{
	}

	XMLReader::~XMLReader()
	{
	}

	//static BOOL XMLReader::XmlFinder(WCHAR* argv, const WCHAR* log, const WCHAR* Pass);
	XMLReader::data_t XMLReader::data;


	HRESULT Next(IXmlReader *pReader)
	{
		task<HRESULT> NextAttribute([&]()
		{
			return pReader->MoveToNextAttribute();
		});

		return NextAttribute.get();
	}
	HRESULT Read(IXmlReader *pReader, XmlNodeType *nodeType)
	{
		//auto s = make_shared<XmlNodeType>(nodeType);
		task<HRESULT> ReadNodeXML([&]()
		{
			return pReader->Read(nodeType);
		});

		return ReadNodeXML.get();
	}
	void quit(IStream *pFileStream, IXmlReader *pReader)
	{
		SAFE_RELEASE(pFileStream);
		SAFE_RELEASE(pReader);

	}



	/*
	HRESULT CreateStream(WCHAR* argv, IStream *pFileStream){

	//auto s = make_shared<IStream>(pFileStream);
	//auto t = make_shared<WCHAR>(argv);
	task<HRESULT> CreateStreamOnFile([&]()
	{
	return SHCreateStreamOnFile(argv, STGM_READ, &pFileStream);
	});


	}

	*/
	//auto s = make_shared<IStream>(pFileStream);
	//auto t = make_shared<WCHAR>(argv);
	/*
	HRESULT CreateStream(const WCHAR* argv, IStream *pFileStream)
	{
	task<HRESULT> CreateStreamOnFile([&argv, &pFileStream]()
	{
	return SHCreateStreamOnFile(argv, STGM_READ, &pFileStream);
	});
	return CreateStreamOnFile.get();
	}
	*/
	BOOL XMLReader::XmlFinder(const WCHAR* argv, const WCHAR* Log, const WCHAR* Pass)

	{
		HRESULT hr = S_OK;
		IStream *pFileStream = NULL;
		IXmlReader *pReader = NULL;
		XmlNodeType nodeType;
		const WCHAR* pwszPrefix;
		const WCHAR* pwszLocalName;
		const WCHAR* pwszValue;
		const WCHAR* LoginName;
		const WCHAR* LoginValue;
		const WCHAR* PasswordName;
		const WCHAR* PasswordValue;
		BOOL result = FALSE;


		UINT cwchPrefix;




		//Proviamo a metterci la istruzione classica per vedere se non crasha
		//Open read-only input stream ,it shoud be asynch
		/*	if (FAILED(hr = CreateStream(argv, pFileStream)))
		{
		wprintf(L"Error creating file reader, error is %08.8lx", hr);
		HR(hr);
		}
		*/
		task<HRESULT> CreateStreamOnFile([&argv, &pFileStream]()
		{
			return SHCreateStreamOnFile(argv, STGM_READ, &pFileStream);
		});


		//Open read-only input stream ,it shoud be asynch
		if (FAILED(hr = CreateStreamOnFile.get()))
		{
			wprintf(L"Error creating file reader, error is %08.8lx", hr);
			HR(hr);
		}

		if (FAILED(hr = CreateXmlReader(__uuidof(IXmlReader), (void**)&pReader, NULL)))
		{
			wprintf(L"Error creating xml reader, error is %08.8lx", hr);
			HR(hr);
		}

		if (FAILED(hr = pReader->SetProperty(XmlReaderProperty_DtdProcessing, DtdProcessing_Prohibit)))
		{
			wprintf(L"Error setting XmlReaderProperty_DtdProcessing, error is %08.8lx", hr);
			HR(hr);
		}

		if (FAILED(hr = pReader->SetInput(pFileStream)))
		{
			wprintf(L"Error setting input for reader, error is %08.8lx", hr);
			HR(hr);
		}



		//read until there are no more nodes
		while (S_OK == (hr = Read(pReader, &nodeType)))
			//	while (S_OK == (hr = pReader->Read(&nodeType)))
		{
			switch (nodeType)
			{
			case XmlNodeType_XmlDeclaration:
				wprintf(L"XmlDeclaration\n");
				//	if (S_OK != pReader->MoveToNextAttribute())
				if (S_OK != Next(pReader))
					break;
			case XmlNodeType_Element:
				if (FAILED(hr = pReader->GetPrefix(&pwszPrefix, &cwchPrefix)))
				{
					wprintf(L"Error getting prefix, error is %08.8lx", hr);
					HR(hr);
				}
				if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
				{
					wprintf(L"Error getting local name, error is %08.8lx", hr);
					HR(hr);
				}
				if (cwchPrefix > 0)
					wprintf(L"Element: %s:%s\n", pwszPrefix, pwszLocalName);
				else
					wprintf(L"Element: %s\n", pwszLocalName);

				if (!StrCmpLogicalW(pwszLocalName, L"client-login"))
				{
					if (S_OK != (hr = Next(pReader)))
						//	if (S_OK != pReader->MoveToNextAttribute())
					{
						HR(hr);
						return hr;
					}

					//read login
					if (FAILED(hr = pReader->GetLocalName(&LoginName, NULL)))
					{
						wprintf(L"Error getting local name, error is %08.8lx", hr);
						return hr;
					}
					if (FAILED(hr = pReader->GetValue(&LoginValue, NULL)))
					{
						wprintf(L"Error getting value, error is %08.8lx", hr);
						return hr;
					}

					//read password
					if (S_OK != Next(pReader))
						return hr;

					if (FAILED(hr = pReader->GetLocalName(&PasswordName, NULL)))
					{
						wprintf(L"Error getting local name, error is %08.8lx", hr);
						return hr;
					}
					if (FAILED(hr = pReader->GetValue(&PasswordValue, NULL)))
					{
						wprintf(L"Error getting value, error is %08.8lx", hr);
						return hr;
					}


					if ((wcscmp(LoginValue, Log) == 0) && (wcscmp(PasswordValue, Pass) == 0))
					{
						//found record!
						//read email
						if (S_OK != Next(pReader))
							return hr;

						if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
						{
							wprintf(L"Error getting local name, error is %08.8lx", hr);
							return hr;
						}
						if (FAILED(hr = pReader->GetValue(&pwszValue, NULL)))
						{
							wprintf(L"Error getting value, error is %08.8lx", hr);
							return hr;
						}

						data.Mail = std::wstring(pwszValue);

						//read memory limit

						if (S_OK != (hr = Read(pReader, &nodeType)))
							return hr;
						if (S_OK != (hr = Read(pReader, &nodeType)))
							return hr;

						pReader->MoveToFirstAttribute();

						if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
						{
							wprintf(L"Error getting local name, error is %08.8lx", hr);
							return hr;
						}
						if (FAILED(hr = pReader->GetValue(&pwszValue, NULL)))
						{
							wprintf(L"Error getting value, error is %08.8lx", hr);
							return hr;
						}
						if (S_OK != Next(pReader))
							HR(hr);


						if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
						{
							wprintf(L"Error getting local name, error is %08.8lx", hr);
							return hr;
						}
						if (FAILED(hr = pReader->GetValue(&pwszValue, NULL)))
						{
							wprintf(L"Error getting value, error is %08.8lx", hr);
							return hr;
						}


						data.memLimit = std::wstring(pwszValue);

						//read CPU limit
						if (S_OK != (hr = Read(pReader, &nodeType)))
							return hr;
						if (S_OK != (hr = Read(pReader, &nodeType)))
							return hr;

						pReader->MoveToFirstAttribute();

						if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
						{
							wprintf(L"Error getting local name, error is %08.8lx", hr);
							return hr;
						}
						if (FAILED(hr = pReader->GetValue(&pwszValue, NULL)))
						{
							wprintf(L"Error getting value, error is %08.8lx", hr);
							return hr;
						}

						if (S_OK != Next(pReader))
							HR(hr);


						if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
						{
							wprintf(L"Error getting local name, error is %08.8lx", hr);
							return hr;
						}
						if (FAILED(hr = pReader->GetValue(&pwszValue, NULL)))
						{
							wprintf(L"Error getting value, error is %08.8lx", hr);
							return hr;
						}

						data.CPULimit = std::wstring(pwszValue);


						//read processes number
						if (S_OK != (hr = Read(pReader, &nodeType)))
							return hr;
						if (S_OK != (hr = Read(pReader, &nodeType)))
							return hr;

						pReader->MoveToFirstAttribute();

						if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
						{
							wprintf(L"Error getting local name, error is %08.8lx", hr);
							return hr;
						}
						if (FAILED(hr = pReader->GetValue(&pwszValue, NULL)))
						{
							wprintf(L"Error getting value, error is %08.8lx", hr);
							return hr;
						}

						if (S_OK != Next(pReader))
							HR(hr);


						if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
						{
							wprintf(L"Error getting local name, error is %08.8lx", hr);
							return hr;
						}
						if (FAILED(hr = pReader->GetValue(&pwszValue, NULL)))
						{
							wprintf(L"Error getting value, error is %08.8lx", hr);
							return hr;
						}

						//wmemcpy(data.NumProcess, pwszValue, 50);
						data.NumProcess = std::wstring(pwszValue);

						result = TRUE;
						HR(hr);
						return result;

					}
					else
					{
						//go to next client-login brother
						if (S_OK != (hr = pReader->MoveToElement()))
							HR(hr);
						for (unsigned int i = 0; i < 11; ++i)
						{
							if (S_OK != (hr = Read(pReader, &nodeType)))
								HR(hr);
						}

					}

				}

				if (pReader->IsEmptyElement())
					wprintf(L" (empty)");
				break;
			case XmlNodeType_EndElement:
				if (FAILED(hr = pReader->GetPrefix(&pwszPrefix, &cwchPrefix)))
				{
					wprintf(L"Error getting prefix, error is %08.8lx", hr);
					HR(hr);
				}
				if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
				{
					wprintf(L"Error getting local name, error is %08.8lx", hr);
					HR(hr);
				}
				if (cwchPrefix > 0)
					wprintf(L"End Element: %s:%s\n", pwszPrefix, pwszLocalName);
				else
					wprintf(L"End Element: %s\n", pwszLocalName);
				break;
			case XmlNodeType_Text:
			case XmlNodeType_Whitespace:
				if (FAILED(hr = pReader->GetValue(&pwszValue, NULL)))
				{
					wprintf(L"Error getting value, error is %08.8lx", hr);
					HR(hr);
				}
				wprintf(L"Text: >%s<\n", pwszValue);
				break;
			case XmlNodeType_CDATA:
				if (FAILED(hr = pReader->GetValue(&pwszValue, NULL)))
				{
					wprintf(L"Error getting value, error is %08.8lx", hr);
					HR(hr);
				}
				wprintf(L"CDATA: %s\n", pwszValue);
				break;
			case XmlNodeType_ProcessingInstruction:
				if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
				{
					wprintf(L"Error getting name, error is %08.8lx", hr);
					HR(hr);
				}
				if (FAILED(hr = pReader->GetValue(&pwszValue, NULL)))
				{
					wprintf(L"Error getting value, error is %08.8lx", hr);
					HR(hr);
				}
				wprintf(L"Processing Instruction name:%s value:%s\n", pwszLocalName, pwszValue);
				break;
			case XmlNodeType_Comment:
				if (FAILED(hr = pReader->GetValue(&pwszValue, NULL)))
				{
					wprintf(L"Error getting value, error is %08.8lx", hr);
					HR(hr);
				}
				wprintf(L"Comment: %s\n", pwszValue);
				break;
			case XmlNodeType_DocumentType:
				wprintf(L"DOCTYPE is not printed\n");
				break;
			}
		}


		return result;

	}
}
