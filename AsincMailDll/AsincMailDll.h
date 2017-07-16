#ifdef ASINCMAILDLL_EXPORTS
#define ASINCMAILDLL_API __declspec(dllexport) 
#else
#define ASINCMAILDLL_API __declspec(dllimport) 
#endif


#ifndef ASINCMAIL_H
#define ASINCMAIL_H
#pragma once

#include <stdio.h>
#include <tchar.h>
#include <atlbase.h>
#include <atlcom.h>
#include "easendmailobj.tlh"

using namespace EASendMailObjLib;
#define IDC_SRCIMAIL 1
static _ATL_FUNC_INFO OnClosed = { CC_STDCALL, VT_EMPTY, 0 };
static _ATL_FUNC_INFO OnSending = { CC_STDCALL, VT_EMPTY, 2, { VT_I4, VT_I4 } };
static _ATL_FUNC_INFO OnError = { CC_STDCALL, VT_EMPTY, 2, { VT_I4, VT_BSTR } };
static _ATL_FUNC_INFO OnConnected = { CC_STDCALL, VT_EMPTY, 0 };
static _ATL_FUNC_INFO OnAuthenticated = { CC_STDCALL, VT_EMPTY, 0 };
class CMailEvents :public IDispEventSimpleImpl<IDC_SRCIMAIL,
	CMailEvents,
	&__uuidof(_IMailEvents)>
{
public:
	CMailEvents(){};
	BEGIN_SINK_MAP(CMailEvents)
		SINK_ENTRY_INFO(IDC_SRCIMAIL, __uuidof(_IMailEvents), 1,
		&CMailEvents::OnClosedHandler, &OnClosed)
		SINK_ENTRY_INFO(IDC_SRCIMAIL, __uuidof(_IMailEvents), 2,
		&CMailEvents::OnSendingHandler, &OnSending)
		SINK_ENTRY_INFO(IDC_SRCIMAIL, __uuidof(_IMailEvents), 3,
		&CMailEvents::OnErrorHandler, &OnError)
		SINK_ENTRY_INFO(IDC_SRCIMAIL, __uuidof(_IMailEvents), 4,
		&CMailEvents::OnConnectedHandler, &OnConnected)
		SINK_ENTRY_INFO(IDC_SRCIMAIL, __uuidof(_IMailEvents), 5,
		&CMailEvents::OnAuthenticatedHandler, &OnAuthenticated)
	END_SINK_MAP()
public:
	BOOL    m_bError;
	BOOL    m_bFinished;
	_bstr_t m_lastError;
protected:
	HRESULT __stdcall OnClosedHandler()
	{
		m_bFinished = TRUE;
		return S_OK;
	}
	HRESULT __stdcall OnSendingHandler(long nSent, long nTotalSize)
	{
		_tprintf(_T("Sending %d/%d ...\r\n"), nSent, nTotalSize);
		return S_OK;
	}
	HRESULT __stdcall OnErrorHandler(long nErrorCode, BSTR ErrorMessage)
	{
		m_bFinished = TRUE;
		m_bError = TRUE;
		m_lastError = ErrorMessage;
		return S_OK;
	}
	HRESULT __stdcall OnConnectedHandler()
	{
		_tprintf(_T("Connected\r\n"));
		return S_OK;
	}
	HRESULT __stdcall OnAuthenticatedHandler()
	{
		_tprintf(_T("Authenticated\r\n"));
		return S_OK;
	}
};

namespace AsyncMailDll
{
	/**
	*  AsyncMail Class
	*
	*  Defines the required function to send an async mail
	**/
	class AsyncMail {
	public:


		AsyncMail();
		~AsyncMail();

		static int ASINCMAILDLL_API SendMail(const WCHAR *address);




	};
}
#endif