#ifdef XMLLITEREADERDLL_EXPORTS
#define XMLLITEREADERDLL_API __declspec(dllexport) 
#else
#define XMLLITEREADERDLL_API __declspec(dllimport) 
#endif



#pragma once

#include <ole2.h>
#include <xmllite.h>
#include <stdio.h>
#include <shlwapi.h>
#include <wchar.h>
#include <string>

#include <tchar.h>
#include <iostream>


namespace XMLLite
{
	/**
	*  XMLReader Class
	*
	*  Defines the required function to parse and get data from XML File
	**/
	class XMLReader {
	public:


		XMLReader();
		~XMLReader();

		static BOOL XMLLITEREADERDLL_API XmlFinder(const WCHAR* argv);
		/**
		*  Get the data from the XML Files
		*
		*  Returns Boolean if find it
		*/

		static 	struct  data_t
		{

			std::wstring Log;
			std::wstring Pass;
			std::wstring URL;


		}XMLLITEREADERDLL_API data;


	};

}