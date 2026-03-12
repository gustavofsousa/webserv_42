<h1 align="center">webserv</h1>

<p align="center">
  <img src="https://img.shields.io/badge/language-C++-blue.svg" alt="Language">
  <img src="https://img.shields.io/badge/standard-C++98-orange.svg" alt="C++98">
  <img src="https://img.shields.io/badge/42-School-black.svg" alt="42 School">
  <img src="https://img.shields.io/badge/status-complete-brightgreen.svg" alt="Status">
</p>

<p align="center">
  A production-like HTTP/1.1 server built from scratch in C++ — handling multiple connections, CGI execution, and static file serving.
</p>

<p align="center">
  <a href="README.pt-br.md">🇧🇷 Leia em Português</a>
</p>

---

## Why this project matters

> "This project taught me how web servers actually work — how HTTP requests are parsed byte by byte, how non-blocking I/O with select/poll allows a single process to handle thousands of simultaneous connections, and what CGI really is. After this project, concepts like Nginx config files, reverse proxies, and connection pooling stopped being magic — they became engineering decisions I now understand deeply."

This is one of the most demanding projects at 42. Building a functioning HTTP server from scratch — without any networking library — demonstrates a real understanding of the protocol and OS-level I/O that powers the entire web.

---

## What was built

An HTTP/1.1 server supporting:

### Core HTTP
- `GET`, `POST`, `DELETE` methods
- Static file serving with correct MIME types
- Custom error pages (400, 403, 404, 405, 500...)
- Chunked transfer encoding
- Persistent connections (`keep-alive`)

### Configuration
- NGINX-inspired configuration file parser
- Multiple virtual servers (different ports/host)
- Per-route configuration: allowed methods, redirects, root, index, autoindex
- Upload directory configuration
- Max body size limit

### CGI
- Full CGI/1.1 execution — runs Python, PHP, or any script
- Environment variables set per the CGI spec
- POST body forwarded to CGI stdin
- CGI stdout returned as HTTP response

### Connection management
- Non-blocking I/O with `select()`
- Multiple simultaneous client connections
- Graceful timeout and connection close

---

## A standout technical detail

The server is structured around a clear three-layer model: `Webserv` (event loop + socket management), `Connection` (per-client state machine), and `Client` + `Request`/`Response` (HTTP parsing and generation). This separation of concerns — where each class has a single, well-defined responsibility — is the same architecture pattern used by nginx and most production HTTP servers. It demonstrates mature software design thinking in C++.

---

## Getting Started

```bash
git clone https://github.com/gustavofsousa/webserv_42.git
cd webserv_42
make
```

### Running

```bash
./webserv config/default.conf
```

Open your browser at `http://localhost:8080`

### Configuration example

```nginx
server {
    listen      8080;
    server_name localhost;
    root        ./static_pages;
    index       index.html;

    location / {
        allowed_methods GET POST;
        autoindex       on;
    }

    location /upload {
        allowed_methods POST;
        upload_dir      ./uploads;
    }

    location /cgi-bin {
        allowed_methods GET POST;
        cgi_extension   .py;
    }
}
```

### Testing

```bash
# GET request
curl http://localhost:8080/

# POST request
curl -X POST -d "name=gustavo" http://localhost:8080/form

# DELETE request
curl -X DELETE http://localhost:8080/file.txt
```

---

## Project structure

```
webserv_42/
├── source/
│   ├── main.cpp
│   ├── core/
│   │   ├── Webserv.cpp       # Event loop (select), socket management
│   │   ├── Server.cpp        # Virtual server + config binding
│   │   └── Connection.cpp    # Per-client connection state
│   ├── http/
│   │   ├── Request.cpp       # HTTP request parser
│   │   ├── Response.cpp      # HTTP response builder
│   │   ├── Client.cpp        # Client state machine
│   │   ├── CGI.cpp           # CGI execution
│   │   └── cgi/              # CGI helper scripts
│   └── config/               # Config file parser
├── static_pages/             # Static HTML/CSS files
└── index.html
```

---

## Skills demonstrated

- HTTP/1.1 protocol implementation (parsing, headers, status codes)
- Non-blocking I/O with `select()` system call
- TCP socket programming (`socket`, `bind`, `listen`, `accept`)
- CGI/1.1 specification implementation
- NGINX-style configuration parsing
- Multi-client connection management
- Object-oriented design patterns in C++ (state machine, layered architecture)

---

## License

This project was developed as part of the [42 School](https://42.fr) curriculum.

---

<p align="center">Made with ☕ at 42 Rio de Janeiro</p>
