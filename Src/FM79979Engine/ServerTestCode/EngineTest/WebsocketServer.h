#pragma once

bool	WebSocketInit();
int		WebSocketUpdate(float e_fElpaseTime);
bool	WebSocketDestroy();

bool	WebSocketSendData(SDLNetSocket e_pWebSocket, int e_iDataLength, char* e_pData);
bool	WebSocketSendDataToAll(int e_iDataLength, char* e_pData);