#include "stdafx.h"
#include "openssl/include/libssh2.h"
#include "openssl/include/libssh2_sftp.h"
#include <libssh2.h>
#include <libssh2_sftp.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "SFTPTransfer.h"
//#include <sys/socket.h>
//#include <arpa/inet.h>
//#include <unistd.h>

namespace fs = std::filesystem;


// Upload a file using SFTP
bool upload_file(LIBSSH2_SFTP* sftp_session, const std::string& local_path, const std::string& remote_path)
{
	std::ifstream local_file(local_path, std::ios::binary);
	if (!local_file.is_open())
	{
		std::cerr << "Failed to open local file: " << local_path << std::endl;
		return false;
	}

	LIBSSH2_SFTP_HANDLE* sftp_handle = libssh2_sftp_open(sftp_session, remote_path.c_str(),
														 LIBSSH2_FXF_WRITE | LIBSSH2_FXF_CREAT | LIBSSH2_FXF_TRUNC,
														 LIBSSH2_SFTP_S_IRWXU);
	if (!sftp_handle)
	{
		std::cerr << "Failed to open remote file: " << remote_path << std::endl;
		local_file.close();
		return false;
	}

	char buffer[1024];
	while (!local_file.eof())
	{
		local_file.read(buffer, sizeof(buffer));
		ssize_t bytes_written = libssh2_sftp_write(sftp_handle, buffer, local_file.gcount());
		if (bytes_written < 0)
		{
			std::cerr << "Failed to write to remote file: " << remote_path << std::endl;
			return false;
		}
	}

	local_file.close();
	libssh2_sftp_close(sftp_handle);
	return true;
}

// Create a remote directory
void create_remote_directory(LIBSSH2_SFTP* sftp_session, const std::string& remote_dir)
{
	int rc = libssh2_sftp_mkdir(sftp_session, remote_dir.c_str(), LIBSSH2_SFTP_S_IRWXU);
	if (rc != 0 && rc != LIBSSH2_ERROR_SFTP_PROTOCOL)
	{ // Ignore if the directory already exists
		std::cerr << "Failed to create remote directory: " << remote_dir << std::endl;
	}
}

// Recursively upload a directory and its contents
bool upload_directory(LIBSSH2_SFTP* sftp_session, const std::string& local_dir, const std::string& remote_dir)
{
	create_remote_directory(sftp_session, remote_dir);

	for (const auto& entry : fs::directory_iterator(local_dir))
	{
		std::string local_path = entry.path().string();
		std::string remote_path = remote_dir + "/" + entry.path().filename().string();

		if (entry.is_directory())
		{
			// Recursively upload subdirectory
			return upload_directory(sftp_session, local_path, remote_path);
		}
		else if (entry.is_regular_file())
		{
			// Upload file
			return upload_file(sftp_session, local_path, remote_path);
		}
	}
	return true;
}

int UploadDirectorySample()
{
	const char* SERVER_IP = "127.0.0.1";
	const int SERVER_PORT = 22;
	const char* USERNAME = "your_username";
	const char* PASSWORD = "your_password";
	int sock;
	struct sockaddr_in server_addr;

	// Initialize libssh2
	libssh2_init(0);

	// Create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		perror("Socket creation failed");
		return -1;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	//#pragma comment(lib, "Ws2_32.lib")
	inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

	// Connect to the SSH server
	if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("Connection to SSH server failed");
		return -1;
	}

	// Create SSH session
	LIBSSH2_SESSION* session = libssh2_session_init();
	if (!session)
	{
		std::cerr << "Unable to create SSH session" << std::endl;
		return -1;
	}

	// Establish the SSH connection
	if (libssh2_session_handshake(session, sock))
	{
		std::cerr << "Failed SSH handshake" << std::endl;
		libssh2_session_free(session);
		return -1;
	}

	// Authenticate via username and password
	if (libssh2_userauth_password(session, USERNAME, PASSWORD))
	{
		std::cerr << "Authentication failed" << std::endl;
		libssh2_session_disconnect(session, "Normal Shutdown");
		libssh2_session_free(session);
		return -1;
	}

	// Initialize SFTP session
	LIBSSH2_SFTP* sftp_session = libssh2_sftp_init(session);
	if (!sftp_session)
	{
		std::cerr << "Unable to start SFTP session" << std::endl;
		libssh2_session_disconnect(session, "Normal Shutdown");
		libssh2_session_free(session);
		return -1;
	}

	// Upload a directory recursively
	std::string local_dir = "path_to_local_directory";
	std::string remote_dir = "path_to_remote_directory";
	upload_directory(sftp_session, local_dir, remote_dir);

	// Clean up
	libssh2_sftp_shutdown(sftp_session);
	libssh2_session_disconnect(session, "Normal Shutdown");
	libssh2_session_free(session);
	close(sock);

	libssh2_exit();
	return 0;
}



// Download a file from the remote server
bool download_file(LIBSSH2_SFTP* sftp_session, const std::string& remote_path, const std::string& local_path)
{
	LIBSSH2_SFTP_HANDLE* sftp_handle = libssh2_sftp_open(sftp_session, remote_path.c_str(),
														 LIBSSH2_FXF_READ, LIBSSH2_SFTP_S_IRWXU);
	if (!sftp_handle)
	{
		std::cerr << "Failed to open remote file: " << remote_path << std::endl;
		return false;
	}

	std::ofstream local_file(local_path, std::ios::binary);
	if (!local_file.is_open())
	{
		std::cerr << "Failed to open local file: " << local_path << std::endl;
		libssh2_sftp_close(sftp_handle);
		return false;
	}

	char buffer[1024];
	ssize_t bytes_read;
	while ((bytes_read = libssh2_sftp_read(sftp_handle, buffer, sizeof(buffer))) > 0)
	{
		local_file.write(buffer, bytes_read);
	}

	if (bytes_read < 0)
	{
		std::cerr << "Error reading remote file: " << remote_path << std::endl;
	}

	local_file.close();
	libssh2_sftp_close(sftp_handle);
	return true;
}

// Recursively download a directory from the remote server
bool download_directory(LIBSSH2_SFTP* sftp_session, const std::string& remote_dir, const std::string& local_dir)
{
	// Create local directory if it doesn't exist
	if (!fs::exists(local_dir))
	{
		fs::create_directories(local_dir);
	}

	LIBSSH2_SFTP_HANDLE* sftp_handle = libssh2_sftp_opendir(sftp_session, remote_dir.c_str());
	if (!sftp_handle)
	{
		std::cerr << "Failed to open remote directory: " << remote_dir << std::endl;
		return false;
	}

	char buffer[512];
	LIBSSH2_SFTP_ATTRIBUTES attrs;

	while (libssh2_sftp_readdir(sftp_handle, buffer, sizeof(buffer), &attrs) > 0)
	{
		std::string name = buffer;

		// Skip "." and ".."
		if (name == "." || name == "..")
		{
			continue;
		}

		std::string remote_path = remote_dir + "/" + name;
		std::string local_path = local_dir + "/" + name;

		if (attrs.permissions & LIBSSH2_SFTP_S_IFDIR)
		{
			// Recursively download subdirectory
			download_directory(sftp_session, remote_path, local_path);
		}
		else if (attrs.permissions & LIBSSH2_SFTP_S_IFREG)
		{
			// Download file
			download_file(sftp_session, remote_path, local_path);
		}
	}

	libssh2_sftp_closedir(sftp_handle);
	return true;
}

int DownloadDirectorySample()
{
	const char* SERVER_IP = "127.0.0.1";
	const int SERVER_PORT = 22;
	const char* USERNAME = "your_username";
	const char* PASSWORD = "your_password";
	int sock;
	struct sockaddr_in server_addr;

	// Initialize libssh2
	libssh2_init(0);

	// Create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		perror("Socket creation failed");
		return -1;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

	// Connect to the SSH server
	if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("Connection to SSH server failed");
		return -1;
	}

	// Create SSH session
	LIBSSH2_SESSION* session = libssh2_session_init();
	if (!session)
	{
		std::cerr << "Unable to create SSH session" << std::endl;
		return -1;
	}

	// Establish the SSH connection
	if (libssh2_session_handshake(session, sock))
	{
		std::cerr << "Failed SSH handshake" << std::endl;
		libssh2_session_free(session);
		return -1;
	}

	// Authenticate via username and password
	if (libssh2_userauth_password(session, USERNAME, PASSWORD))
	{
		std::cerr << "Authentication failed" << std::endl;
		libssh2_session_disconnect(session, "Normal Shutdown");
		libssh2_session_free(session);
		return -1;
	}

	// Initialize SFTP session
	LIBSSH2_SFTP* sftp_session = libssh2_sftp_init(session);
	if (!sftp_session)
	{
		std::cerr << "Unable to start SFTP session" << std::endl;
		libssh2_session_disconnect(session, "Normal Shutdown");
		libssh2_session_free(session);
		return -1;
	}

	// Download a directory recursively
	std::string remote_dir = "path_to_remote_directory";
	std::string local_dir = "path_to_local_directory";
	download_directory(sftp_session, remote_dir, local_dir);

	// Clean up
	libssh2_sftp_shutdown(sftp_session);
	libssh2_session_disconnect(session, "Normal Shutdown");
	libssh2_session_free(session);
	close(sock);

	libssh2_exit();
	return 0;
}

struct sLIBSSH2SocketData
{
	int m_iSock;
	sockaddr_in m_ServerAddr;
	LIBSSH2_SESSION* m_pSession = nullptr;
	LIBSSH2_SFTP* m_pSFTPSession = nullptr;
	std::string m_strLocalDirectory = "path_to_local_directory";
	std::string m_strRemoteDirectory = "path_to_remote_directory";

	std::string m_strUserName = "path_to_local_directory";
	std::string m_strPassword = "path_to_remote_directory";
	std::string m_strIP = "127.0.0.1";
	int			m_iPort = 44;
	bool		m_bConnectedOk = false;
	f_CompleteFunction	m_fCompleteFunction;
	sLIBSSH2SocketData()
	{
	}
	~sLIBSSH2SocketData()
	{
		Close();
	}

	void	Close()
	{
		// Clean up
		if (m_pSFTPSession)
		{
			libssh2_sftp_shutdown(m_pSFTPSession);
			m_pSFTPSession = nullptr;
		}
		if (m_pSession)
		{
			libssh2_session_disconnect(m_pSession, "Normal Shutdown");
			libssh2_session_free(m_pSession);
			m_pSession = nullptr;
		}
		close(m_iSock);
		m_iSock = 0;
	}


	bool	DoUploadFile(const std::string& local_path, const std::string& remote_path)
	{
		if (!m_bConnectedOk)
		{
			return false;
		}
		return upload_file(m_pSFTPSession, local_path, remote_path);
	}

	bool	DoUploadDirectory(const std::string& local_dir, const std::string& remote_dir)
	{
		if (!m_bConnectedOk)
		{
			return false;
		}
		bool l_bResult = upload_directory(m_pSFTPSession, local_dir, remote_dir);
		if (this->m_fCompleteFunction)
		{
			this->m_fCompleteFunction();
		}
		return l_bResult;
	}

	bool	DoDownloadFile(const std::string& local_dir, const std::string& remote_dir)
	{
		if (!m_bConnectedOk)
		{
			return false;
		}
		//return upload_file(m_pSFTPSession, local_path, remote_path);
		return download_file(m_pSFTPSession, remote_dir, local_dir);
	}

	bool	DoDownloadDirectory(const std::string& local_dir, const std::string& remote_dir)
	{
		if (!m_bConnectedOk)
		{
			return false;
		}
		//download_directory(LIBSSH2_SFTP * sftp_session, const std::string & remote_dir, const std::string & local_dir)
		return download_directory(m_pSFTPSession, remote_dir,local_dir);
	}

	bool		DoConnect()
	{
		m_bConnectedOk = false;
		//int sock;
		//struct sockaddr_in server_addr;
		// Create socket
		m_iSock = socket(AF_INET, SOCK_STREAM, 0);
		if (m_iSock < 0)
		{
			perror("Socket creation failed");
			return false;
		}

		m_ServerAddr.sin_family = AF_INET;
		m_ServerAddr.sin_port = htons(m_iPort);
		//#pragma comment(lib, "Ws2_32.lib")
		inet_pton(AF_INET, m_strIP.c_str(), &m_ServerAddr.sin_addr);

		// Connect to the SSH server
		if (connect(m_iSock, (struct sockaddr*)&m_ServerAddr, sizeof(m_ServerAddr)) < 0)
		{
			perror("Connection to SSH server failed");
			return false;
		}

		// Create SSH session
		m_pSession = libssh2_session_init();
		if (!m_pSession)
		{
			std::cerr << "Unable to create SSH session" << std::endl;
			return false;
		}

		// Establish the SSH connection
		if (libssh2_session_handshake(m_pSession, m_iSock))
		{
			std::cerr << "Failed SSH handshake" << std::endl;
			libssh2_session_free(m_pSession);
			return false;
		}

		// Authenticate via username and password
		if (libssh2_userauth_password(m_pSession, this->m_strUserName.c_str(),this->m_strPassword.c_str()))
		{
			std::cerr << "Authentication failed" << std::endl;
			libssh2_session_disconnect(m_pSession, "Normal Shutdown");
			libssh2_session_free(m_pSession);
			return false;
		}

		// Initialize SFTP session
		m_pSFTPSession = libssh2_sftp_init(m_pSession);
		if (!m_pSFTPSession)
		{
			std::cerr << "Unable to start SFTP session" << std::endl;
			libssh2_session_disconnect(m_pSession, "Normal Shutdown");
			libssh2_session_free(m_pSession);
			return false;
		}
		m_bConnectedOk = true;
		return true;
	}
};

std::map<eEnvType, sLIBSSH2SocketData*>* g_pLIBSSH2SocketDataMap = nullptr;

void AssignEnvData(const char* e_strUserName, const char* e_strPW, const char* e_strIP, const int e_iPort, const eEnvType e_eEnvType)
{
	if (!g_pLIBSSH2SocketDataMap)
	{
		LibSSH2Init();
	}
	sLIBSSH2SocketData*l_pLIBSSH2SocketData = nullptr;
	auto l_IT = g_pLIBSSH2SocketDataMap->find(e_eEnvType);
	if (l_IT == g_pLIBSSH2SocketDataMap->end())
	{
		l_pLIBSSH2SocketData = new sLIBSSH2SocketData();
		g_pLIBSSH2SocketDataMap->insert({ e_eEnvType, l_pLIBSSH2SocketData });
	}
	if (l_pLIBSSH2SocketData)
	{
		l_pLIBSSH2SocketData->m_strIP = e_strIP;
		l_pLIBSSH2SocketData->m_iPort = e_iPort;
		l_pLIBSSH2SocketData->m_strUserName = e_strUserName;
		l_pLIBSSH2SocketData->m_strPassword = e_strPW;
	}
}

void LibSSH2Init()
{
	if (!g_pLIBSSH2SocketDataMap)
	{
		// Initialize libssh2
		auto l_Result = libssh2_init(0);
		g_pLIBSSH2SocketDataMap = new std::map<eEnvType, sLIBSSH2SocketData*>();

	}
}

bool UploadFileOrDirectory(const std::string& e_strLocalPath, const std::string& e_strRemotePath, std::vector<eEnvType>& e_eEnvTypeVector, f_CompleteFunction e_fCompleteFunction)
{
	if (!g_pLIBSSH2SocketDataMap)
	{
		return false;
	}
	bool l_bIsDirectory = fs::is_directory(e_strLocalPath);
	std::function<void(float)> l_Function = [l_bIsDirectory,e_eEnvTypeVector, e_strLocalPath, e_strRemotePath, e_fCompleteFunction](float e_fElapsedTime)
	{
		for (auto l_Type : e_eEnvTypeVector)
		{
			auto l_IT = g_pLIBSSH2SocketDataMap->find(l_Type);
			if (l_IT != g_pLIBSSH2SocketDataMap->end())
			{
				if (l_IT->second->DoConnect())
				{
					if (l_bIsDirectory)
					{
						l_IT->second->DoUploadDirectory(e_strLocalPath, e_strRemotePath);
					}
					else
					{
						l_IT->second->DoUploadFile(e_strLocalPath, e_strRemotePath);
					}
					
				}
				else
				{
					UT::ErrorMsgByFormat("connect to %d failed", l_Type);
					if (e_fCompleteFunction)
					{
						e_fCompleteFunction();
					}
					//return false;
				}
			}
			else
			{

			}
		}
		if (e_fCompleteFunction)
		{
			e_fCompleteFunction();
		}
	};
	cCPP11Thread	l_cCPP11Thread;
	l_cCPP11Thread.ThreadDetach(l_Function, "qoo");
	return true;
	return false;
}

bool DownloadFileOrDirectory(const std::string& e_strLocalPath, const std::string& e_strRemotePath, std::vector<eEnvType>& e_eEnvTypeVector, f_CompleteFunction e_fCompleteFunction)
{
	if (!g_pLIBSSH2SocketDataMap)
	{
		return false;
	}
	if (fs::is_directory(e_strLocalPath))
	{
		for (auto l_Type : e_eEnvTypeVector)
		{
			auto l_IT = g_pLIBSSH2SocketDataMap->find(l_Type);
			if (l_IT != g_pLIBSSH2SocketDataMap->end())
			{
				l_IT->second->DoUploadDirectory(e_strLocalPath, e_strRemotePath);
			}
			else
			{

			}
		}
		return true;
	}
	else
	{

	}
	return false;
}

void LibSSH2ShutDown()
{
	if (g_pLIBSSH2SocketDataMap)
	{
		SAFE_DELETE(g_pLIBSSH2SocketDataMap);
	}
	libssh2_exit();
}
