#include <emscripten/emscripten.h>
#include <emscripten/websocket.h>
#include "GameNetWork.h"
#include "../Common/Log/FMLog.h"

FATMING_CORE::cGameNetwork*g_pForWebSocket = nullptr;
EMSCRIPTEN_WEBSOCKET_T	g_iWebSocketFD = -1;

EM_BOOL EMSDK_OnOpen(int eventType, const EmscriptenWebSocketOpenEvent* websocketEvent, void* userData)
{
	FMLOG("WebSocket onopen");
	g_pForWebSocket->SetNetWorkStatus(FATMING_CORE::eNetWorkStatus::eNWS_CONNECTED);
	g_pForWebSocket->SetWebSocketOpen(g_iWebSocketFD);
	//EMSCRIPTEN_RESULT result;
	//result = emscripten_websocket_send_utf8_text(websocketEvent->socket, "hoge");
	//if (result) 
	//{
	//	printf("Failed to emscripten_websocket_send_utf8_text(): %d\n", result);
	//}
	//{
	//	tutorial::AddressBook l_AddressBook;
	//	int size = l_AddressBook.ByteSizeLong();
	//	for (int i = 0; i < 3; ++i)
	//	{
	//		auto l_pPeople1 = l_AddressBook.add_people();
	//		l_pPeople1->set_id(i);
	//		l_pPeople1->set_name("2266");
	//	}
	//	auto l_peopleSize = l_AddressBook.people_size();
	//	size = l_AddressBook.ByteSizeLong();
	//	//network buffer 
	//	char* buffer = (char*)alloca(size);
	//	l_AddressBook.SerializeToArray(buffer, size);
	//	EMSCRIPTEN_RESULT l_Result = emscripten_websocket_send_binary(websocketEvent->socket, buffer, size);
	//	FMLOG("send size:%d", size);
	//}
	return EM_TRUE;
}
EM_BOOL EMSDK_OnError(int eventType, const EmscriptenWebSocketErrorEvent* websocketEvent, void* userData)
{

	//printf("onerror:%d", eventType);
	FMLOG("WebSocket OnError:%d", eventType);
	return EM_TRUE;
}

EM_BOOL EMSDK_OnClose(int eventType, const EmscriptenWebSocketCloseEvent* websocketEvent, void* userData) 
{
	FMLOG("websocket onclose:%d", eventType);
	g_pForWebSocket->SetNetWorkStatus(FATMING_CORE::eNetWorkStatus::eNWS_LOST_CONNECTION);
	g_iWebSocketFD = -1;
	return EM_TRUE;
}

EM_BOOL EMSDK_OnMessage(int eventType, const EmscriptenWebSocketMessageEvent* websocketEvent, void* userData)
{
	if (websocketEvent->numBytes < 1)
	{
		emscripten_websocket_close(websocketEvent->socket, 1000, "websocketEvent->numBytes <= 1");
		g_pForWebSocket->SetNetWorkStatus(FATMING_CORE::eNetWorkStatus::eNWS_LOST_CONNECTION);
		return EM_TRUE;
	}
	FATMING_CORE::sNetworkReceivedPacket*l_pNetworkReceivedPacket = new FATMING_CORE::sNetworkReceivedPacket();
	l_pNetworkReceivedPacket->iSize = websocketEvent->numBytes;
	l_pNetworkReceivedPacket->pData = new char[l_pNetworkReceivedPacket->iSize];
	memcpy(l_pNetworkReceivedPacket->pData, websocketEvent->data, websocketEvent->numBytes);
	g_pForWebSocket->AddNewWebSocket(l_pNetworkReceivedPacket);
	return EM_TRUE;
}

bool	EMSDK_SendData(char* e_pData, int e_iSize)
{
	if (g_iWebSocketFD != -1)
	{
		return emscripten_websocket_send_binary(g_iWebSocketFD, e_pData, e_iSize);
	}
	return false;
}

void	EMSDK_DoWebSocketConnect(FATMING_CORE::cGameNetwork* e_pForWebSocket, const char* e_strURL)
{
	g_pForWebSocket = e_pForWebSocket;
	EmscriptenWebSocketCreateAttributes l_Att = { e_strURL,nullptr,EM_TRUE };
	FMLOG("websocket try to connect with %s", e_strURL);
	EMSCRIPTEN_WEBSOCKET_T l_WebSocketFD = emscripten_websocket_new(&l_Att);
	g_iWebSocketFD = l_WebSocketFD;
	emscripten_websocket_set_onopen_callback(l_WebSocketFD, 0, EMSDK_OnOpen);
	emscripten_websocket_set_onerror_callback(l_WebSocketFD, 0, EMSDK_OnError);
	emscripten_websocket_set_onclose_callback(l_WebSocketFD, 0, EMSDK_OnClose);
	emscripten_websocket_set_onmessage_callback(l_WebSocketFD, 0, EMSDK_OnMessage);
	g_pForWebSocket->SetNetWorkStatus(FATMING_CORE::eNetWorkStatus::eNWS_TRY_TO_CONNECT);
}