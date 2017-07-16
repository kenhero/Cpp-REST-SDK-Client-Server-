
#pragma once


#include "XMLLiteReaderDll.h"



#include <cpprest\http_client.h>
#include "pplx/pplxtasks.h"
#include <cpprest\filestream.h>
using namespace concurrency;

using namespace utility;
//using namespace concurrency::streams;

#pragma warning(disable : 4127)  // conditional expression is constant
#define CHKHR(stmt)             do { hr = (stmt); if (FAILED(hr)) goto CleanUp; } while(0)
#define HR(stmt)                do { hr = (stmt); quit(pFileStream,pReader); } while(0)
#define SAFE_RELEASE(I)         do { if (I){ I->Release(); } I = NULL; } while(0)

namespace XMLLite
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
		//auto s = std::make_shared<XmlNodeType>(nodeType);
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

	BOOL XMLReader::XmlFinder(const WCHAR* argv)
		//BOOL  XmlFinder(WCHAR* argv, const WCHAR* log, const WCHAR* Pass)
	{
		HRESULT hr = S_OK;
		IStream *pFileStream = NULL;
		IXmlReader *pReader = NULL;
		XmlNodeType nodeType;
		const WCHAR* pwszPrefix;
		const WCHAR* pwszLocalName;
		const WCHAR* pwszValue;
		BOOL result = FALSE;


		UINT cwchPrefix;




		//auto s = make_shared<IStream>(pFileStream);
		//auto t = make_shared<WCHAR>(argv);
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

				if (!StrCmpLogicalW(pwszLocalName, L"client"))
				{
					/*if (S_OK != (hr = Next(pReader)))
					//	if (S_OK != pReader->MoveToNextAttribute())
					{
					HR(hr);
					return hr;
					}
					*/
					//read login
					pReader->MoveToFirstAttribute();

					if (S_OK != (hr = Read(pReader, &nodeType)))
					{
						HR(hr);
						return result;
					}
					if (S_OK != (hr = Read(pReader, &nodeType)))
					{
						HR(hr);
						return result;
					}
					if (S_OK != Next(pReader))
					{
						HR(hr);
						return result;
					}



					if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
					{
						wprintf(L"Error getting login, error is %08.8lx", hr);
						return hr;
					}
					if (FAILED(hr = pReader->GetValue(&pwszValue, NULL)))
					{
						wprintf(L"Error getting value, error is %08.8lx", hr);
						return hr;
					}
					data.Log = std::wstring(pwszValue);


					//read password
					pReader->MoveToFirstAttribute();

					if (S_OK != (hr = Read(pReader, &nodeType)))
					{
						HR(hr);
						return result;
					}
					if (S_OK != (hr = Read(pReader, &nodeType)))
					{
						HR(hr);
						return result;
					}
					if (S_OK != Next(pReader))
					{
						HR(hr);
						return result;
					}

					if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
					{
						wprintf(L"Error getting login, error is %08.8lx", hr);
						return hr;
					}
					if (FAILED(hr = pReader->GetValue(&pwszValue, NULL)))
					{
						wprintf(L"Error getting value, error is %08.8lx", hr);
						return hr;
					}
					data.Pass = std::wstring(pwszValue);


					//read URL
					pReader->MoveToFirstAttribute();

					if (S_OK != (hr = Read(pReader, &nodeType)))
					{
						HR(hr);
						return result;
					}
					if (S_OK != (hr = Read(pReader, &nodeType)))
					{
						HR(hr);
						return result;
					}
					if (S_OK != Next(pReader))
					{
						HR(hr);
						return result;
					}

					if (FAILED(hr = pReader->GetLocalName(&pwszLocalName, NULL)))
					{
						wprintf(L"Error getting login, error is %08.8lx", hr);
						return hr;
					}
					if (FAILED(hr = pReader->GetValue(&pwszValue, NULL)))
					{
						wprintf(L"Error getting value, error is %08.8lx", hr);
						return hr;
					}
					data.URL = std::wstring(pwszValue);
					result = TRUE;
					HR(hr);
					return result;






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
