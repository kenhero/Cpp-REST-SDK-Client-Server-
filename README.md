Note:
Before building install cpprestsdk from Microsoft.
The C++ REST SDK is a Microsoft project for cloud-based client-server communication in native code using a modern asynchronous C++ API design. This project aims to help C++ developers connect to and interact with services.
With vcpkg on Windows

PS> vcpkg install cpprestsdk cpprestsdk:x64-windows

Also install EASendMail SMTP Component from https://www.emailarchitect.net/easendmail and copy easendmailobj.tlh on AsincMailDll Folder.

Client Server application with cpprest sdk Casablanca framework
Create a WMI based sysem monitoring and alert notification [url removed, login to view] using a service that could monitor multiple client machines.

Functional Specifications:

Create a client data to collect system data

1) 1. Client is a console application meant to be executed in a recurring schedule(every 5 minutes) using windows task scheduler.

[url removed, login to view] the application is run, it collects the following data, sends the data to the service and then terminates. If it is unable to perform the whole cycle , then it must log errors on console and exit with an error code.

1) List of running processes each with process name, memory consumed, CPU utilization.

2)Total consumed and free system memory.

3)Overall CPU system %

3 .The application identifies itself to the service with the client name and password.

4. The service connection URL,client name and password are configured in the client config file.

Create a cloud based high performance RESTful web service.

1) [url removed, login to view] service must be secured with login/password based security.

2. Each login is unique and identifies a specific client machine

3. Each client is configured in the service config xml file something like this

<client-login="xxx" pass="xxx" mail="asa@asda">

<alert type="memory" limit="50%" />

<alert type="CPU" limit="20%" />

<alert type="process" limit="50%" />

</client>

[url removed, login to view] service receives data collected by multiple clients and based upon the alert configuration each client sends notification.

[url removed, login to view] notification is sent to the client mail addresss using SMTP. Use a simple text mail format with some detail about the alert.

[url removed, login to view] service then dumps the received data to log file per client

Technical Specifications :

The following list of technical specifications :

1. 1) The service

1. Should be created using casablanca C++ REST sdk

2. Should use an SMTP library to send mails using SMTP

2)For the application

[url removed, login to view] application would be installed on hundreds of machines in an application

2. No user interface for the application.

3) Input validations and constraints wherever necessary.
