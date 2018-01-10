// AsincMail.cpp : Defines the entry point for the console application.
//


#include "AsincMailDll.h"

namespace AsyncMailDll
{


	int AsyncMail::SendMail(const WCHAR *address)
	{
		::CoInitialize(NULL);
		IMailPtr oSmtp = NULL;
		oSmtp.CreateInstance("EASendMailObj.Mail");
		oSmtp->LicenseCode = _T("TryIt");

		// Set your sender email address
		oSmtp->FromAddr = _T("dillon1977@hotmail.com");
		// Add recipient email address
		oSmtp->AddRecipientEx(address, 0);

		// Set email subject
		oSmtp->Subject = _T("email from Visual C++ project in asynchronous mode ");

		// Set email body
		oSmtp->BodyText = _T("this is a test email sent from Visual C++ asynchronously");

		// Your SMTP server address
		oSmtp->ServerAddr = _T("smtp.live.com");

		// User and password for ESMTP authentication, if your server doesn't
		// require User authentication, please remove the following codes.
		oSmtp->UserName = _T("dillon1977@hotmail.com");
		oSmtp->Password = _T("S3rr4nd4!");

		// If your SMTP server requires SSL connection, please add this line
		oSmtp->SSL_init();

		_tprintf(_T("Start to send email ...\r\n"));

		// Attach event connection pointer
		CMailEvents oEvents;
		oEvents.DispEventAdvise(oSmtp.GetInterfacePtr());
		oEvents.m_bFinished = FALSE;
		oEvents.m_bError = FALSE;
		// Set asynchronous mode
		oSmtp->Asynchronous = 1;
		oSmtp->SendMail();

		// Waiting for email sending ...
		while (!oEvents.m_bFinished)
		{
			MSG msg;
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					return 0;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			// You can do other thing here
			::Sleep(10);
		}
		// Detach event connection pointer
		oEvents.DispEventUnadvise(oSmtp.GetInterfacePtr());
		if (!oEvents.m_bError)
		{
			_tprintf(_T("email was sent successfully!\r\n"));
		}
		else
		{
			_tprintf(_T("failed to send email with the following error: %s\r\n"),
				(const TCHAR*)oEvents.m_lastError);
		}
		if (oSmtp != NULL)
			oSmtp.Release();
		return 0;
	}
}