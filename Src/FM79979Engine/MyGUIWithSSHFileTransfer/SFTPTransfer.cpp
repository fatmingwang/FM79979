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
	sock = (int)socket(AF_INET, SOCK_STREAM, 0);
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

// Utility function to check if a path is a directory (for remote deletion)
bool is_remote_directory(LIBSSH2_SFTP* sftp_session, const std::string& remote_path)
{
	LIBSSH2_SFTP_ATTRIBUTES attrs;
	if (libssh2_sftp_stat(sftp_session, remote_path.c_str(), &attrs) == 0)
	{
		return (attrs.permissions & LIBSSH2_SFTP_S_IFDIR);
	}
	return false;
}

// Delete a remote file using SFTP
void delete_remote_file(LIBSSH2_SFTP* sftp_session, const std::string& remote_path)
{
	int rc = libssh2_sftp_unlink(sftp_session, remote_path.c_str());
	if (rc != 0)
	{
		std::cerr << "Failed to delete remote file: " << remote_path << std::endl;
	}
	else
	{
		std::cout << "Deleted remote file: " << remote_path << std::endl;
	}
}

// Recursively delete a remote directory
void delete_remote_directory(LIBSSH2_SFTP* sftp_session, const std::string& remote_dir)
{
	LIBSSH2_SFTP_HANDLE* sftp_handle = libssh2_sftp_opendir(sftp_session, remote_dir.c_str());
	if (!sftp_handle)
	{
		std::cerr << "Failed to open remote directory: " << remote_dir << std::endl;
		return;
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

		if (attrs.permissions & LIBSSH2_SFTP_S_IFDIR)
		{
			// Recursively delete subdirectory
			delete_remote_directory(sftp_session, remote_path);
		}
		else
		{
			// Delete file
			delete_remote_file(sftp_session, remote_path);
		}
	}

	// Close directory handle
	libssh2_sftp_closedir(sftp_handle);

	// Once the directory is empty, delete the directory itself
	int rc = libssh2_sftp_rmdir(sftp_session, remote_dir.c_str());
	if (rc != 0)
	{
		std::cerr << "Failed to delete remote directory: " << remote_dir << std::endl;
	}
	else
	{
		std::cout << "Deleted remote directory: " << remote_dir << std::endl;
	}
}



bool list_remote_directory(LIBSSH2_SFTP* sftp_session, const std::string& remote_dir, std::vector<std::string>& e_FilesVector, std::vector<std::string>& e_DirectoriesVector)
{
	LIBSSH2_SFTP_HANDLE* sftp_handle = libssh2_sftp_opendir(sftp_session, remote_dir.c_str());
	if (!sftp_handle)
	{
		std::cerr << "Failed to open remote directory: " << remote_dir << std::endl;
		return false;
	}

	char buffer[2048];
	LIBSSH2_SFTP_ATTRIBUTES attrs;

	std::cout << "Contents of remote directory: " << remote_dir << std::endl;

	while (libssh2_sftp_readdir(sftp_handle, buffer, sizeof(buffer), &attrs) > 0)
	{
		std::string name = buffer;

		// Skip "." and ".."
		if (name == "." || name == "..")
		{
			continue;
		}

		if (attrs.permissions & LIBSSH2_SFTP_S_IFDIR)
		{
			e_DirectoriesVector.push_back(name);
			std::cout << "[DIR]  " << name << std::endl;
		}
		else
		if (attrs.permissions & LIBSSH2_SFTP_S_IFREG)
		{
			e_FilesVector.push_back(name);
			std::cout << "[FILE] " << name << std::endl;
		}
		else
		{
			std::cout << "[OTHER] " << name << std::endl;
		}
	}

	libssh2_sftp_closedir(sftp_handle);
	return true;
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

	// Extract the directory part of the path
	std::filesystem::path file_path(local_path);
	std::filesystem::path dir_path = file_path.parent_path();
	if (!dir_path.empty() && !std::filesystem::exists(dir_path))
	{
		std::filesystem::create_directories(dir_path);
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
	sock = (int)socket(AF_INET, SOCK_STREAM, 0);
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
	sEnvData	m_EnvData;
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
			//this->m_fCompleteFunction("");
		}
		return l_bResult;
	}

	bool	DoDownloadFile(const std::string& remote_dir, const std::string& local_dir)
	{
		if (!m_bConnectedOk)
		{
			return false;
		}
		//return upload_file(m_pSFTPSession, local_path, remote_path);
		return download_file(m_pSFTPSession, remote_dir, local_dir);
	}

	bool	DoListRemoteDirectory(const std::string& remote_dir, std::vector<std::string>& e_FilesVector, std::vector<std::string>& e_DirectoriesVector)
	{
		if (!m_bConnectedOk)
		{
			return false;
		}
		return list_remote_directory(m_pSFTPSession, remote_dir, e_FilesVector, e_DirectoriesVector);
	}

	bool	DoDownloadDirectory(const std::string& remote_dir, const std::string& local_dir)
	{
		if (!m_bConnectedOk)
		{
			return false;
		}
		//download_directory(LIBSSH2_SFTP * sftp_session, const std::string & remote_dir, const std::string & local_dir)
		return download_directory(m_pSFTPSession, remote_dir, local_dir);
	}

	bool		DoConnect()
	{
		m_bConnectedOk = false;
		//int sock;
		//struct sockaddr_in server_addr;
		// Create socket
		m_iSock = (int)socket(AF_INET, SOCK_STREAM, 0);
		if (m_iSock < 0)
		{
			perror("Socket creation failed");
			return false;
		}

		m_ServerAddr.sin_family = AF_INET;
		m_ServerAddr.sin_port = htons(this->m_EnvData.m_iPort);
		//#pragma comment(lib, "Ws2_32.lib")
		inet_pton(AF_INET, this->m_EnvData.m_strTargetIP.c_str(), &m_ServerAddr.sin_addr);

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
		if (libssh2_userauth_password(m_pSession, this->m_EnvData.m_strRemoteUserName.c_str(), this->m_EnvData.m_strRemotePassword.c_str()))
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

std::map<eEnv, sLIBSSH2SocketData*>* g_pLIBSSH2SocketDataMap = nullptr;

void AssignEnvData(const eEnv e_eEnvType, sEnvData e_EnvData)
{
	if (!g_pLIBSSH2SocketDataMap)
	{
		LibSSH2Init();
	}
	sLIBSSH2SocketData* l_pLIBSSH2SocketData = nullptr;
	auto l_IT = g_pLIBSSH2SocketDataMap->find(e_eEnvType);
	if (l_IT == g_pLIBSSH2SocketDataMap->end())
	{
		l_pLIBSSH2SocketData = new sLIBSSH2SocketData();
		g_pLIBSSH2SocketDataMap->insert({ e_eEnvType, l_pLIBSSH2SocketData });
	}
	if (l_pLIBSSH2SocketData)
	{
		l_pLIBSSH2SocketData->m_EnvData = e_EnvData;
		if (l_pLIBSSH2SocketData->m_EnvData.m_strRemotePassword.length() == 0)
		{
			l_pLIBSSH2SocketData->m_EnvData.m_strRemotePassword = "diresoft999000";
		}
	}
}

void LibSSH2Init()
{
	if (!g_pLIBSSH2SocketDataMap)
	{
		// Initialize libssh2
		auto l_Result = libssh2_init(0);
		g_pLIBSSH2SocketDataMap = new std::map<eEnv, sLIBSSH2SocketData*>();
		SDLNet_Init();
	}
}

bool UploadFileOrDirectory(const std::string& e_strLocalPath, const std::string& e_strRemotePath, std::vector<eEnv>& e_eEnvTypeVector, f_CompleteFunction e_fCompleteFunction)
{
	if (!g_pLIBSSH2SocketDataMap)
	{
		return false;
	}
	bool l_bIsDirectory = fs::is_directory(e_strLocalPath);
	std::function<void()> l_Function = [l_bIsDirectory, e_eEnvTypeVector, e_strLocalPath, e_strRemotePath, e_fCompleteFunction]()
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
					UT::ErrorMsgByFormat("connect to %s failed", GetEnvName(l_Type).c_str());
					break;
				}
			}
			else
			{

			}
		}
		if (e_fCompleteFunction)
		{
			e_fCompleteFunction("");
		}
	};
	std::thread t(l_Function);
	t.detach();
	return true;
	return false;
}

bool DownloadFileOrDirectory(const std::string& e_strRemotePath, const std::string& e_strLocalFilePath, std::vector<eEnv>& e_eEnvTypeVector, f_CompleteFunction e_fCompleteFunction)
{
	if (!g_pLIBSSH2SocketDataMap)
	{
		return false;
	}
	//cCPP11Thread*l_pCPP11Thread = new cCPP11Thread();
	bool l_bIsDirectory = e_strLocalFilePath.find(".") != std::string::npos? false: true;
	std::function<void()> l_Function = [l_bIsDirectory, e_eEnvTypeVector, e_strLocalFilePath, e_strRemotePath, e_fCompleteFunction]()
	{
		std::string l_strInfo;
		for (auto l_Type : e_eEnvTypeVector)
		{
			auto l_IT = g_pLIBSSH2SocketDataMap->find(l_Type);
			if (l_IT != g_pLIBSSH2SocketDataMap->end())
			{
				//if (0)
				if (l_IT->second->DoConnect())
				{
					auto l_strDiectory = l_IT->second->m_EnvData.m_strRemoteDirectory;
					l_strDiectory += "/" + e_strRemotePath;
					if (l_bIsDirectory)
					{
						l_IT->second->DoDownloadDirectory(l_strDiectory, e_strLocalFilePath);
					}
					else
					{
						if (!l_IT->second->DoDownloadFile(l_strDiectory, e_strLocalFilePath))
						{
							auto l_strInnerInfo = UT::ComposeMsgByFormat("download %s failed", l_strDiectory.c_str());
							l_strInfo += l_strInnerInfo;
							l_strInfo += "\n";
							UT::ErrorMsgByFormat(l_strInnerInfo.c_str());
							break;
						}
					}
				}
				else
				{
					auto l_strInnerInfo = UT::ComposeMsgByFormat("connect to %s failed", GetEnvName(l_Type).c_str());
					l_strInfo += l_strInnerInfo;
					l_strInfo += "\n";
					UT::ErrorMsgByFormat(l_strInnerInfo.c_str());
					break;
				}
			}
			else
			{

			}
		}
		if (e_fCompleteFunction)
		{
			if (l_strInfo.length() == 0)
			{
				l_strInfo = e_strRemotePath + " download to " + e_strLocalFilePath + " finished";
			}
			e_fCompleteFunction(l_strInfo.c_str());
			
		}
	};
	std::thread t(l_Function);
	t.detach();
	//l_pCPP11Thread->ThreadDetach(l_Function, "qoo");
	return true;
}

bool	ListRemoteDirectory(eEnv e_EnvType,const std::string& e_strRemotePath, f_GetDirectoryContentCompleteFunction e_fGetDirectoryContentCompleteFunction)
{
	if (!g_pLIBSSH2SocketDataMap)
	{
		return false;
	}
	//cCPP11Thread*l_pCPP11Thread = new cCPP11Thread();
	bool l_bIsDirectory = e_strRemotePath.find(".") != std::string::npos ? false : true;
	if (!l_bIsDirectory)
	{
		return false;
	}
	std::function<void()> l_Function = [e_EnvType, e_strRemotePath, e_fGetDirectoryContentCompleteFunction]()
	{
		std::vector<std::string> l_FilesVector;
		std::vector<std::string> l_DirectoriesVector;
		std::string l_strInfo;
		auto l_IT = g_pLIBSSH2SocketDataMap->find(e_EnvType);
		if (l_IT != g_pLIBSSH2SocketDataMap->end())
		{
			//if (0)
			if (l_IT->second->DoConnect())
			{
				auto l_strDiectory = l_IT->second->m_EnvData.m_strRemoteDirectory;
				l_strDiectory += "/" + e_strRemotePath;
				l_IT->second->DoListRemoteDirectory(l_strDiectory, l_DirectoriesVector, l_FilesVector);
			}
			else
			{
				auto l_strInnerInfo = UT::ComposeMsgByFormat("connect to %s failed", GetEnvName(e_EnvType).c_str());
				l_strInfo += l_strInnerInfo;
				l_strInfo += "\n";
				UT::ErrorMsgByFormat(l_strInnerInfo.c_str());
			}
		}
		else
		{

		}
		if (e_fGetDirectoryContentCompleteFunction)
		{
			if (l_strInfo.length() == 0)
			{
				//l_strInfo = e_strRemotePath + " download to " + e_strLocalFilePath + " finished";
			}
			
			e_fGetDirectoryContentCompleteFunction(l_FilesVector, l_DirectoriesVector);
		}
	};
	std::thread t(l_Function);
	t.detach();
	//l_pCPP11Thread->ThreadDetach(l_Function, "qoo");
	return true;
}

void LibSSH2ShutDown()
{
	if (g_pLIBSSH2SocketDataMap)
	{
		SAFE_DELETE(g_pLIBSSH2SocketDataMap);
	}
	libssh2_exit();
	SDLNet_Quit();
}

std::string GetEnvName(eEnv e_eEnv)
{
	if (e_eEnv == eE_DEV)
	{
		return "Dev";
	}
	else
	if (e_eEnv == eE_UAT)
	{
		return "Uat";
	}
	else
	if (e_eEnv == eE_SIT)
	{
		return "Sit";
	}
	else
	if (e_eEnv == eE_FUN)
	{
		return "Funplay";
	}
	else
	if (e_eEnv == eE_PROD)
	{
		return "Prod";
	}
	return std::string("unknown");
}