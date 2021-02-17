#include "stdafx.h"
#include "GameApp.h"
#include "../../Core/GameplayUT/OpenGL/GLSL/ToneMapping.h"
#include "../../Core/GameplayUT/OpenGL/GLSL/TunnelEffect.h"
#include "../../Core/Network/happyhttp.h"
#include "TestShader.h"
int g_iCount = 0;
std::string g_strHappyHttpTest;
// invoked when response headers have been received
void OnBegin(const happyhttp::Response* r, void* userdata)
{
	printf("BEGIN (%d %s)\n", r->getstatus(), r->getreason());
	g_strHappyHttpTest += "OnBegine:";
	g_strHappyHttpTest += ValueToString(r->getstatus());
	g_strHappyHttpTest += ",";
	g_strHappyHttpTest += ValueToString(r->getreason());
	g_strHappyHttpTest += "\n";
	g_iCount = 0;
}

// invoked to process response body data (may be called multiple times)
void OnData(const happyhttp::Response* r, void* userdata, const unsigned char* data, int n)
{
	//fwrite(data, 1, n, stdout);
	g_strHappyHttpTest += "OnData:";
	g_strHappyHttpTest += ValueToString((char*)data);
	g_iCount += n;
}

// invoked when response is complete
void OnComplete(const happyhttp::Response* r, void* userdata)
{
	printf("COMPLETE (%d bytes)\n", g_iCount);
	g_strHappyHttpTest += "OnComplete:";
	g_strHappyHttpTest += ValueToString(g_iCount);
}


void TestGET()
{
	happyhttp::Connection conn("www.scumways.com", 80);
	//happyhttp::Connection conn("www.google.com", 80);
	conn.setcallbacks(OnBegin, OnData, OnComplete, 0);

	//conn.request("GET", "/happyhttp/test.php");
	conn.request("GET", "/");

	while (conn.outstanding())
		conn.pump();
	if (g_strHappyHttpTest.length())
	{
		//UT::ErrorMsg(g_strHappyHttpTest.c_str(), "qoo");
	}
}

void Test2()
{
	g_strHappyHttpTest = "";
	printf("-----------------Test2------------------------\n");
	// POST using high-level request interface

	const char* headers[] =
	{
		"Connection", "close",
		"Content-type", "application/x-www-form-urlencoded",
		"Accept", "text/plain",
		0
	};

	const char* body = "answer=42&name=Bubba";

	happyhttp::Connection conn("www.scumways.com", 80);
	conn.setcallbacks(OnBegin, OnData, OnComplete, 0);
	conn.request("POST",
		"/happyhttp/test.php",
		headers,
		(const unsigned char*)body,
		(int)strlen(body));

	while (conn.outstanding())
		conn.pump();
	if (g_strHappyHttpTest.length())
	{
		//UT::ErrorMsg(g_strHappyHttpTest.c_str(), "qoo");
	}
}

void TestGET3()
{
	happyhttp::Connection conn("192.168.1.101", 80);
	//happyhttp::Connection conn("www.google.com", 80);
	conn.setcallbacks(OnBegin, OnData, OnComplete, 0);
	{
		//conn.request("GET", "/");
		//while (conn.outstanding())
		//	conn.pump();
		//if (g_strHappyHttpTest.length())
		//{

		//}
		//int a = 0;
	}
	{
		const char* headers[] =
		{
			"Connection", "close",
			"Content-type", "application/x-www-form-urlencoded",
			"Accept", "text/plain",
			0
		};
		//http://192.168.1.101/charge?cTime=5000
		const char* body = "?cTime=5000";
		//conn.request("GET", "/charge", headers, (const unsigned char*)body, strlen(body));
		//conn.request("GET", "/charge", 0, (const unsigned char*)body, strlen(body));
		conn.request("GET", "/charge?cTime=5000", 0, 0, 0);
		//conn.request("GET", "/charge", headers,0,0);
		while (conn.outstanding())
			conn.pump();
		if (g_strHappyHttpTest.length())
		{

		}
		int a = 0;
	}
}