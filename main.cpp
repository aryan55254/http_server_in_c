#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <unordered_map>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 8192
#define ROOT_DIR "www"

// Map file extensions to MIME types
const std::unordered_map<std::string, std::string> mime_types = {
    {".html", "text/html"},
    {".css", "text/css"},
    {".js", "application/javascript"},
    {".png", "image/png"},
    {".jpg", "image/jpeg"},
    {".gif", "image/gif"},
    {".ico", "image/x-icon"}
};

std::string get_mime_type(const std::string& path) {
    size_t dot_pos = path.rfind('.');
    if (dot_pos != std::string::npos) {
        std::string ext = path.substr(dot_pos);
        auto it = mime_types.find(ext);
        if (it != mime_types.end()) return it->second;
    }
    return "application/octet-stream";
}

std::string url_decode(const std::string& str) {
    std::string res;
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '%') {
            if (i + 3 <= str.length()) {
                int c;
                sscanf(str.c_str() + i + 1, "%2x", &c);
                res += static_cast<char>(c);
                i += 2;
            } else {
                res += '%';
            }
        } else if (str[i] == '+') {
            res += ' ';
        } else {
            res += str[i];
        }
    }
    return res;
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    ssize_t bytes_read = read(client_socket, buffer, BUFFER_SIZE - 1);
    if (bytes_read <= 0) {
        close(client_socket);
        return;
    }

    std::istringstream request_stream(buffer);
    std::string method, uri, version;
    request_stream >> method >> uri >> version;

    std::cout << "Request: " << method << " " << uri << "\n";

    if (method != "GET") {
        std::string response =
            "HTTP/1.1 405 Method Not Allowed\r\n"
            "Content-Length: 0\r\n"
            "Connection: close\r\n\r\n";
        write(client_socket, response.c_str(), response.size());
        close(client_socket);
        return;
    }

    // Prevent path traversal attacks
    std::string decoded_uri = url_decode(uri);
    std::string file_path = std::string(ROOT_DIR) + decoded_uri;
    if (file_path.back() == '/') file_path += "index.html";

    std::ifstream file(file_path, std::ios::binary);
    if (!file) {
        std::string response =
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 13\r\n\r\n"
            "404 Not Found";
        write(client_socket, response.c_str(), response.size());
        close(client_socket);
        return;
    }

    // Read file content
    std::ostringstream file_buffer;
    file_buffer << file.rdbuf();
    std::string body = file_buffer.str();

    std::string mime = get_mime_type(file_path);
    std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\n"
             << "Content-Type: " << mime << "\r\n"
             << "Content-Length: " << body.size() << "\r\n"
             << "Connection: close\r\n\r\n"
             << body;

    write(client_socket, response.str().c_str(), response.str().size());
    close(client_socket);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is running on http://localhost:" << PORT << "/\n";

    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            continue;
        }
        handle_client(new_socket);
    }

    return 0;
}
