#include "HTTP.h"

using namespace std;

// HTTP Header + Footer information.
#define HTTP_GET	"GET "
#define HTTP_POST	"POST "
#define HTTP_FOOTER(host_name)	 ((string)" HTTP/1.0\r\nHost: " + host_name + (string)"\r\n\r\n")

enum HTTP_REQUEST_TYPE
{
	GET,
	POST
};

// Returns from the function with a negative attitude :)
#define FAIL()	return "failed"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	Escapes a given string for transfer via HTTP.
/// 
/// @author	Clark Gaebel
/// @date	7/2/2009
/// 
/// @param	toEscape	The character string that needs escaping.
/// 
/// @return				The fully escaped string.
///
/// @remarks			This function takes the liberty of preventing SQL Injection by parsing out
///						all single quotes.
////////////////////////////////////////////////////////////////////////////////////////////////////
string HTTPEscape(const string& toEscape)
{
	static const string prefixes[2] = {	"%", "%0" };

	string retVal;
	char target[3];

	size_t len = toEscape.length();
	retVal.reserve(len*3);

	for(size_t i = 0; i < len; ++i)
	{
		// SQL injection prevention :)
		if(toEscape[i] != '\'')
		{
			ZeroMemory(target, sizeof(target));
			_itoa_s(toEscape[i], target, sizeof(target), 16);
			retVal += prefixes[target[1] == NULL] + target;
		}
	}

	return retVal;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	Does an HTTP request.
/// 
/// @author	Clark Gaebel
/// @date	7/2/2009
/// 
/// @param	url	The url of the webpage we're requesting. 
/// @param	req	The request type. 
/// 
/// @return		The returned string from the server, minus HTTP header information.
////////////////////////////////////////////////////////////////////////////////////////////////////
static string HTTPRequest(std::string url, HTTP_REQUEST_TYPE req)
{
	struct sockaddr_in addr = { 0 };
	struct hostent *host = NULL;
	string hostname;

	// If the URL begins with http://, remove it.
	if(url.find("http://") == 0)
		url.erase(0, 7);

	// Connect to the host.
	hostname = url.substr(0, url.find('/', 0));
	if((host = gethostbyname(hostname.c_str())) == NULL)
		FAIL();

	url.erase(0, url.find('/', 0));
	if(!host)
		FAIL();

	addr.sin_family	= AF_INET;
	addr.sin_port	= htons(80);
	addr.sin_addr	= *(struct in_addr*)host->h_addr;

	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(serverSocket <= 0)
	{
		shutdown(serverSocket, SD_BOTH);
		closesocket(serverSocket);
		FAIL();
	}

	if(connect(serverSocket, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		shutdown(serverSocket, SD_BOTH);
		closesocket(serverSocket);
		FAIL();
	}

	string toSend = ((req == GET) ? HTTP_GET : HTTP_POST) + url + HTTP_FOOTER(hostname);

	// If we're sending more than 2147483647 bytes (2 GB) of data, we could have a problem here.
	if(send(serverSocket, toSend.c_str(), (int)toSend.length(), NULL) <= 0)
	{
		shutdown(serverSocket, SD_BOTH);
		closesocket(serverSocket);
		FAIL();
	}

	// Get the page.
	char recvBuffer[8096];	// Temporary storage for the "recv" function.
	string recieved;		// The full string we recieve from the server.
	int bytesRecieved;		// The number of bytes we recieved from the server.

	do {
		ZeroMemory(recvBuffer, sizeof(recvBuffer));
		bytesRecieved = recv(serverSocket, recvBuffer, sizeof(recvBuffer), NULL);
		recieved += recvBuffer;

	// Keep recieving until we get no data.
	} while(bytesRecieved > 0);

	// Clean shit up, and return the data we've recieved.
	shutdown(serverSocket, SD_BOTH);
	closesocket(serverSocket);

	if(recieved.empty())
		FAIL();

	return recieved.substr(recieved.find("\r\n\r\n") + 4);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	Does an HTTP GET on the provided URL. 
/// 
/// @author	Clark Gaebel
/// @date	7/2/2009
/// 
/// @param	url	The URL of the page we're requesting.
/// 
/// @return		The returned string from the server, minus HTTP header information.
////////////////////////////////////////////////////////////////////////////////////////////////////
string HTTPGet(std::string url)
{
	return HTTPRequest(url, GET);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	Does an HTTP POST on the provided URL.  
/// 
/// @author	Clark Gaebel
/// @date	7/2/2009
/// 
/// @param	url	The URL of the page we're requesting.
/// 
/// @return	The returned string from the server, minus HTTP header information.
////////////////////////////////////////////////////////////////////////////////////////////////////
string HTTPPost(std::string url)
{
	return HTTPRequest(url, POST);
}