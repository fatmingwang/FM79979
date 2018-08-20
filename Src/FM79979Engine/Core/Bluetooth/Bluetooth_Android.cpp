#include "../stdafx.h"
#include "Bluetooth.h"
#ifdef ANDROID
#include "../Android/JNIUtil.h"
#include "../GameplayUT/GameApp.h"
namespace FATMING_CORE
{
	bool	g_bRegBluetoothNativeFunction = false;

	void BluetoothReceivedBuffer(JNIEnv *env, jobject obj, jint e_iSize, jbyteArray e_jbyteArray)
	{
		cBluetoothSinglton*l_pBluetoothSinglton = cBluetoothSinglton::GetInstance();
		auto l_pData = jbyteArray_to_unsigned_char_array(e_jbyteArray, env);
		l_pBluetoothSinglton->Android_AddBluetoothPacket(e_iSize, (char*)l_pData);
		delete l_pData;
	}
	//0 for nothing,1 for connected,2 for connect failed,3 for lost connection.
	void BluetoothConnectResult(JNIEnv *env, jobject obj, jint e_iResult)
	{
		//eBTCS_CONNECTED,
		//eBTCS_CONNECT_FAILED,
		//eBTCS_LOST_CONNECTION,
		//eBTCS_TRYING_TO_CONNECT,
		//eBTCS_SELECT_DEVICE,
		cBluetoothSinglton*l_pBluetoothSinglton = cBluetoothSinglton::GetInstance();
		l_pBluetoothSinglton->m_eBluetoothConnectionStatus = (eBluetoothConnectionStatus)e_iResult;
		if (l_pBluetoothSinglton->m_eBluetoothConnectionStatus == eBTCS_CONNECTED)
		{
			if (l_pBluetoothSinglton->m_ConnectionConnectedFunction)
			{
				l_pBluetoothSinglton->m_ConnectionConnectedFunction(nullptr);
			}
		}
		else
		if (l_pBluetoothSinglton->m_eBluetoothConnectionStatus == eBTCS_CONNECT_FAILED || 
			l_pBluetoothSinglton->m_eBluetoothConnectionStatus == eBTCS_LOST_CONNECTION)
		{
			if (l_pBluetoothSinglton->m_ConnectionLostFunction)
			{
				l_pBluetoothSinglton->m_ConnectionLostFunction(nullptr);
			}
		}
		//else
		//if (l_pBluetoothSinglton->m_eBluetoothConnectionStatus == eBTCS_LOST_CONNECTION)
		//{
		//	if (l_pBluetoothSinglton->m_ConnectionLostFunction)
		//	{
		//		l_pBluetoothSinglton->m_ConnectionLostFunction(nullptr);
		//	}
		//}
	}




	TYPDE_DEFINE_MARCO(cBluetoothSinglton)
	sBluetoothPacket::sBluetoothPacket(int e_iSize, char*e_pData)
	{
		pData = new char[e_iSize];
		iSize = e_iSize;
		memcpy(pData, e_pData, e_iSize);
	}

	sBluetoothPacket::~sBluetoothPacket()
	{
		SAFE_DELETE(pData);
	}

	cBluetoothSinglton::cBluetoothSinglton()
	{
		m_ClientConnectionLostFunction = nullptr;
		m_ClientConnectionAddFunction = nullptr;//windows only
		m_ConnectionLostFunction = nullptr;
		m_ConnectionConnectedFunction = nullptr;
		m_eBluetoothConnectionStatus = eBTCS_NONE;
		m_pAllSocketToListenClientMessage = nullptr;
		m_LocalSocket.channel = INVALID_SOCKET;
		m_LocalSocket.ready = false;
		m_bLostConnection = false;
		if (!g_bRegBluetoothNativeFunction)
		{
			g_bRegBluetoothNativeFunction = true;
			RegisterNativeFunction("util/bluetooth/BluetoothChatService", "BluetoothReceivedBuffer", "(I[B)V", (void*)BluetoothReceivedBuffer);
			RegisterNativeFunction("util/bluetooth/BluetoothChatService", "BluetoothConnectResult", "(I)V", (void*)BluetoothConnectResult);
		}
	}

	cBluetoothSinglton::~cBluetoothSinglton()
	{
		Disconnect();
		CallStaticVoidMethod1("BluetoothChatFragment", "BluetoothShutdown", "()V");
	}

	void cBluetoothSinglton::Init()
	{
		Disconnect();
	}

	void cBluetoothSinglton::Update(float e_fElpaseTime)
	{
		if (m_eBluetoothConnectionStatus == eBTCS_CONNECT_FAILED ||
			m_eBluetoothConnectionStatus == eBTCS_LOST_CONNECTION)
		{
			static float l_fWaitSecondToGo = 0.f;
			l_fWaitSecondToGo += 0.16f;
			if (l_fWaitSecondToGo >= 1.f)
			{
				l_fWaitSecondToGo = 0.f;
				CreateAsClient(L"");
			}
		}
	}

	void	cBluetoothSinglton::Render(int e_iPosX, int e_iPosY)
	{
		auto l_strDescription = g_strConnectionStatus[m_eBluetoothConnectionStatus];
		cGameApp::RenderFont(Vector2(e_iPosX, e_iPosY), l_strDescription);
	}


	bool cBluetoothSinglton::CreateAsServer(const wchar_t*e_strServerName)
	{
		return false;
	}

	bool cBluetoothSinglton::CreateAsClient(const wchar_t*e_strServerName)
	{
		m_eBluetoothConnectionStatus = eBTCS_SELECT_DEVICE;
		return CallStaticVoidMethod2("util/bluetooth/BluetoothChatFragment", "BluetoothStart", "(Landroid/app/Activity;)V", (void*)cGameApp::m_spANativeActivity->clazz, g_pMainThreadJNIUtilData, true);
	}

	void cBluetoothSinglton::Disconnect()
	{
		if (m_LocalSocket.channel != INVALID_SOCKET)
		{
			this->m_bLeaveThread = true;
			Sleep(10);
			closesocket(m_LocalSocket.channel);
			CloseThreadAndWaitUntilFinish();
			size_t l_uiSize = m_ConnectedSocketVector.size();
			for (size_t i = 0; i < l_uiSize; i++)
			{
				if (SOCKET_ERROR == closesocket(m_ConnectedSocketVector[i]->channel))
				{

				}
			}
			m_LocalSocket.channel = INVALID_SOCKET;
		}
		DELETE_VECTOR(m_BluetoothPacketVector);
		DELETE_VECTOR(m_ConnectedSocketVector);
	}

	bool cBluetoothSinglton::SetDeviceName(const wchar_t * e_strName)
	{
		return false;
	}

	bool	cBluetoothSinglton::CreateSocksetToListenData()
	{
		return true;
	}

	void cBluetoothSinglton::ServerUpdate(float e_fElpaseTime)
	{
		m_bLeaveThread = true;
	}

	void cBluetoothSinglton::ClientUpdate(float e_fElpaseTime)
	{
		m_bLeaveThread = true;
	}

	void cBluetoothSinglton::GetReceivedData(std::vector<sBluetoothPacket*>* e_pDataVector)
	{
		if (e_pDataVector)
		{
			cPP11MutexHolder l_cPP11MutexHolder(m_BluetoothPacketVectorMutex);
			size_t l_uiSize = m_BluetoothPacketVector.size();
			for (size_t i = 0; i < l_uiSize; i++)
			{
				e_pDataVector->push_back(m_BluetoothPacketVector[i]);
			}
			m_BluetoothPacketVector.clear();
		}
	}

	void cBluetoothSinglton::FindDevices()
	{
	}

	void cBluetoothSinglton::SendDataToAllClient(int e_iLength, char * e_pData)
	{
	}

	void cBluetoothSinglton::SendDataToServer(int e_iLength, char*e_pData)
	{
		if (this->m_eBluetoothConnectionStatus == eBTCS_CONNECTED)
		{
			jbyteArray l_Array = unsigned_array_to_jbyteArray((unsigned char*)e_pData, e_iLength, cGameApp::m_spThreadEnv);
			bool l_bResult = CallStaticVoidMethod2("util/bluetooth/BluetoothChatFragment", "BluetoothSendBuffer", "([B)V", l_Array, g_pMainThreadJNIUtilData, true);
		}
	}


	void cBluetoothSinglton::Android_AddBluetoothPacket(int e_iSize, char*e_pData)
	{
		sBluetoothPacket*l_pBluetoothPacket = new sBluetoothPacket(e_iSize, e_pData);
		{
			cPP11MutexHolder l_cPP11MutexHolder(m_BluetoothPacketVectorMutex);
			m_BluetoothPacketVector.push_back(l_pBluetoothPacket);
		}
	}
}
//ANDROID
#endif