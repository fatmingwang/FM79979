#include "stdafx.h"
#include "openssl/include/libssh2.h"
#include "openssl/include/libssh2_sftp.h"
#include <libssh2.h>
#include <libssh2_sftp.h>
#include <iostream>
#include <fstream>
#include <filesystem>
//#include <sys/socket.h>
//#include <arpa/inet.h>
//#include <unistd.h>

namespace fs = std::filesystem;

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 22
#define USERNAME "your_username"
#define PASSWORD "your_password"

// Upload a file using SFTP
void upload_file(LIBSSH2_SFTP* sftp_session, const std::string& local_path, const std::string& remote_path) {
    std::ifstream local_file(local_path, std::ios::binary);
    if (!local_file.is_open()) {
        std::cerr << "Failed to open local file: " << local_path << std::endl;
        return;
    }

    LIBSSH2_SFTP_HANDLE* sftp_handle = libssh2_sftp_open(sftp_session, remote_path.c_str(),
        LIBSSH2_FXF_WRITE | LIBSSH2_FXF_CREAT | LIBSSH2_FXF_TRUNC,
        LIBSSH2_SFTP_S_IRWXU);
    if (!sftp_handle) {
        std::cerr << "Failed to open remote file: " << remote_path << std::endl;
        local_file.close();
        return;
    }

    char buffer[1024];
    while (!local_file.eof()) {
        local_file.read(buffer, sizeof(buffer));
        ssize_t bytes_written = libssh2_sftp_write(sftp_handle, buffer, local_file.gcount());
        if (bytes_written < 0) {
            std::cerr << "Failed to write to remote file: " << remote_path << std::endl;
            break;
        }
    }

    local_file.close();
    libssh2_sftp_close(sftp_handle);
}

// Create a remote directory
void create_remote_directory(LIBSSH2_SFTP* sftp_session, const std::string& remote_dir) {
    int rc = libssh2_sftp_mkdir(sftp_session, remote_dir.c_str(), LIBSSH2_SFTP_S_IRWXU);
    if (rc != 0 && rc != LIBSSH2_ERROR_SFTP_PROTOCOL) { // Ignore if the directory already exists
        std::cerr << "Failed to create remote directory: " << remote_dir << std::endl;
    }
}

// Recursively upload a directory and its contents
void upload_directory(LIBSSH2_SFTP* sftp_session, const std::string& local_dir, const std::string& remote_dir) {
    create_remote_directory(sftp_session, remote_dir);

    for (const auto& entry : fs::directory_iterator(local_dir)) {
        std::string local_path = entry.path().string();
        std::string remote_path = remote_dir + "/" + entry.path().filename().string();

        if (entry.is_directory()) {
            // Recursively upload subdirectory
            upload_directory(sftp_session, local_path, remote_path);
        }
        else if (entry.is_regular_file()) {
            // Upload file
            upload_file(sftp_session, local_path, remote_path);
        }
    }
}

int UploadDirectorySample() {
    int sock;
    struct sockaddr_in server_addr;

    // Initialize libssh2
    libssh2_init(0);

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    //#pragma comment(lib, "Ws2_32.lib")
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Connect to the SSH server
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to SSH server failed");
        return -1;
    }

    // Create SSH session
    LIBSSH2_SESSION* session = libssh2_session_init();
    if (!session) {
        std::cerr << "Unable to create SSH session" << std::endl;
        return -1;
    }

    // Establish the SSH connection
    if (libssh2_session_handshake(session, sock)) {
        std::cerr << "Failed SSH handshake" << std::endl;
        libssh2_session_free(session);
        return -1;
    }

    // Authenticate via username and password
    if (libssh2_userauth_password(session, USERNAME, PASSWORD)) {
        std::cerr << "Authentication failed" << std::endl;
        libssh2_session_disconnect(session, "Normal Shutdown");
        libssh2_session_free(session);
        return -1;
    }

    // Initialize SFTP session
    LIBSSH2_SFTP* sftp_session = libssh2_sftp_init(session);
    if (!sftp_session) {
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
void download_file(LIBSSH2_SFTP* sftp_session, const std::string& remote_path, const std::string& local_path) {
    LIBSSH2_SFTP_HANDLE* sftp_handle = libssh2_sftp_open(sftp_session, remote_path.c_str(),
        LIBSSH2_FXF_READ, LIBSSH2_SFTP_S_IRWXU);
    if (!sftp_handle) {
        std::cerr << "Failed to open remote file: " << remote_path << std::endl;
        return;
    }

    std::ofstream local_file(local_path, std::ios::binary);
    if (!local_file.is_open()) {
        std::cerr << "Failed to open local file: " << local_path << std::endl;
        libssh2_sftp_close(sftp_handle);
        return;
    }

    char buffer[1024];
    ssize_t bytes_read;
    while ((bytes_read = libssh2_sftp_read(sftp_handle, buffer, sizeof(buffer))) > 0) {
        local_file.write(buffer, bytes_read);
    }

    if (bytes_read < 0) {
        std::cerr << "Error reading remote file: " << remote_path << std::endl;
    }

    local_file.close();
    libssh2_sftp_close(sftp_handle);
}

// Recursively download a directory from the remote server
void download_directory(LIBSSH2_SFTP* sftp_session, const std::string& remote_dir, const std::string& local_dir) {
    // Create local directory if it doesn't exist
    if (!fs::exists(local_dir)) {
        fs::create_directories(local_dir);
    }

    LIBSSH2_SFTP_HANDLE* sftp_handle = libssh2_sftp_opendir(sftp_session, remote_dir.c_str());
    if (!sftp_handle) {
        std::cerr << "Failed to open remote directory: " << remote_dir << std::endl;
        return;
    }

    char buffer[512];
    LIBSSH2_SFTP_ATTRIBUTES attrs;

    while (libssh2_sftp_readdir(sftp_handle, buffer, sizeof(buffer), &attrs) > 0) {
        std::string name = buffer;

        // Skip "." and ".."
        if (name == "." || name == "..") {
            continue;
        }

        std::string remote_path = remote_dir + "/" + name;
        std::string local_path = local_dir + "/" + name;

        if (attrs.permissions & LIBSSH2_SFTP_S_IFDIR) {
            // Recursively download subdirectory
            download_directory(sftp_session, remote_path, local_path);
        }
        else if (attrs.permissions & LIBSSH2_SFTP_S_IFREG) {
            // Download file
            download_file(sftp_session, remote_path, local_path);
        }
    }

    libssh2_sftp_closedir(sftp_handle);
}

int DownloadDirectory()
{
    int sock;
    struct sockaddr_in server_addr;

    // Initialize libssh2
    libssh2_init(0);

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Connect to the SSH server
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to SSH server failed");
        return -1;
    }

    // Create SSH session
    LIBSSH2_SESSION* session = libssh2_session_init();
    if (!session) {
        std::cerr << "Unable to create SSH session" << std::endl;
        return -1;
    }

    // Establish the SSH connection
    if (libssh2_session_handshake(session, sock)) {
        std::cerr << "Failed SSH handshake" << std::endl;
        libssh2_session_free(session);
        return -1;
    }

    // Authenticate via username and password
    if (libssh2_userauth_password(session, USERNAME, PASSWORD)) {
        std::cerr << "Authentication failed" << std::endl;
        libssh2_session_disconnect(session, "Normal Shutdown");
        libssh2_session_free(session);
        return -1;
    }

    // Initialize SFTP session
    LIBSSH2_SFTP* sftp_session = libssh2_sftp_init(session);
    if (!sftp_session) {
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