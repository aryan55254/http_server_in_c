
## 🌐 Simple HTTP Server in C++

A lightweight, basic HTTP server written in C++ that serves static files from a directory and supports basic routing. This is a great starting point to learn about networking, sockets, and how web servers work under the hood.

---

### 📦 Features

- ✅ Serves static HTML, CSS, JS, and image files
- ✅ Handles HTTP `GET` requests
- ✅ Decodes URL-encoded characters (e.g., `%20`, `+`)
- ✅ Returns correct MIME types for common file extensions
- ✅ Prevents path traversal attacks (`../`)
- ✅ Shows 404 Not Found and 405 Method Not Allowed responses
- ✅ Runs on port `8080` by default

---

### 🔧 Requirements

- C++17 compiler (like `g++`)
- POSIX-compatible system (Linux or macOS; Windows users can use WSL)

---

### 🚀 Running the Server

#### 1. Clone the repository (if applicable)
```bash
git clone https://github.com/aryan55254/http_server_in_cpp.git
```

#### 2. Create the `www` folder with some HTML files
```bash
mkdir www
nano www/index.html     # Add HTML content
nano www/about.html     # Add more pages
```

Example `index.html`:
```html
<!DOCTYPE html>
<html>
<head><title>Home</title></head>
<body><h1>Hello from C++ HTTP Server!</h1></body>
</html>
```

#### 3. Compile the server
```bash
g++ main.cpp -o http_server
```

#### 4. Run the server
```bash
./http_server
```

You should see:
```
Server is running on http://localhost:8080/
```

#### 5. Open in browser
Visit:
```
http://localhost:8080/
http://localhost:8080/about.html
```

---

### 📁 Directory Structure

```
http-server/
├── README.md
├── main.cpp          # The C++ source code for the HTTP server
└── www/
    ├── index.html    # Default home page
    └── about.html    # Example additional page
```

---

### 🧠 How It Works

The server listens on port `8080` and handles incoming HTTP GET requests. It maps each request to a file inside the `www/` directory and returns an appropriate HTTP response including:

- Status line (`HTTP/1.1 200 OK`)
- Headers like `Content-Type` and `Content-Length`
- The actual file data (HTML, image binary, etc.)

If the requested file doesn't exist, it returns a `404 Not Found`. For unsupported methods like `POST`, it responds with `405 Method Not Allowed`.

---


### 📝 License

MIT License 

---
