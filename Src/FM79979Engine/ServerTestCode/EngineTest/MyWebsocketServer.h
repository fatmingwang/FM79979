#pragma once



bool	WebSocketInit(int e_iPort = 9992);
int		WebSocketUpdate(float e_fElpaseTime);
bool	WebSocketDestroy();

bool	WebSocketSendData(SDLNetSocket e_pWebSocket, int e_iDataLength, char* e_pData);
bool	WebSocketSendDataToAll(int e_iDataLength, char* e_pData);


//https://github.com/eidheim/Simple-WebSocket-Server