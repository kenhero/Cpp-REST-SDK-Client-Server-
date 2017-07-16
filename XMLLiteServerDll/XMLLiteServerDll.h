#ifdef XMLLITESERVERDLL_EXPORTS
#define XMLLITESERVERDLL_API __declspec(dllexport) 
#else
#define XMLLITESERVERDLL_API __declspec(dllimport) 
#endif



#ifndef XMLLITE_READER_H
#define XMLLITE_READER_H

#pragma once


#include <ole2.h>
#include <xmllite.h>
#include <stdio.h>
#include <shlwapi.h>
#include <wchar.h>
#include <string>



#pragma warning(disable : 4127)  // conditional expression is constant
#define CHKHR(stmt)             do { hr = (stmt); if (FAILED(hr)) goto CleanUp; } while(0)
#define HR(stmt)                do { hr = (stmt); quit(pFileStream,pReader); } while(0)
#define SAFE_RELEASE(I)         do { if (I){ I->Release(); } I = NULL; } while(0)


namespace XMLLiteServer
{
	/**
	*  XMLReader Class
	*
	*  Defines the required function to parse and get data from XML File
	**/
	class XMLReader {
	private:


	public:


		XMLReader();
		~XMLReader();

		static BOOL XMLLITESERVERDLL_API XmlFinder(const WCHAR* argv, const WCHAR* Log, const WCHAR* Pass);
		/**
		*  Get the data from the XML Files
		*
		*  Returns Boolean if find it
		*/

		static 	struct data_t
		{
			std::wstring NumProcess;
			std::wstring Log;
			std::wstring Pass;
			std::wstring Mail;
			std::wstring memLimit;
			std::wstring CPULimit;

		}XMLLITESERVERDLL_API data;

	};
}
#endif