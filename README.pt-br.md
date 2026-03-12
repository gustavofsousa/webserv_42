<h1 align="center">webserv</h1>

<p align="center">
  <img src="https://img.shields.io/badge/linguagem-C++-blue.svg" alt="Linguagem">
  <img src="https://img.shields.io/badge/padrão-C++98-orange.svg" alt="C++98">
  <img src="https://img.shields.io/badge/42-School-black.svg" alt="42 School">
  <img src="https://img.shields.io/badge/status-completo-brightgreen.svg" alt="Status">
</p>

<p align="center">
  Um servidor HTTP/1.1 profissional construído do zero em C++ — com múltiplas conexões, execução CGI e servimento de arquivos estáticos.
</p>

<p align="center">
  <a href="README.md">🇺🇸 Read in English</a>
</p>

---

## Por que este projeto importa

> "Este projeto me ensinou como servidores web realmente funcionam — como requisições HTTP são parseadas byte a byte, como I/O não-bloqueante com select/poll permite que um único processo gerencie milhares de conexões simultâneas, e o que CGI realmente é. Depois desse projeto, conceitos como arquivos de configuração do Nginx, reverse proxies e connection pooling pararam de ser mágica — se tornaram decisões de engenharia que agora entendo profundamente."

Este é um dos projetos mais desafiadores da 42. Construir um servidor HTTP funcional do zero — sem nenhuma biblioteca de rede — demonstra um entendimento real do protocolo e do I/O no nível do OS que alimenta toda a web.

---

## O que foi construído

Um servidor HTTP/1.1 suportando:

### HTTP Core
- Métodos `GET`, `POST`, `DELETE`
- Servimento de arquivos estáticos com MIME types corretos
- Páginas de erro customizadas (400, 403, 404, 405, 500...)
- Codificação de transferência chunked
- Conexões persistentes (`keep-alive`)

### Configuração
- Parser de arquivo de configuração inspirado no NGINX
- Múltiplos servidores virtuais (portas/hosts diferentes)
- Configuração por rota: métodos permitidos, redirects, root, index, autoindex
- Configuração de diretório de upload
- Limite de tamanho máximo de body

### CGI
- Execução CGI/1.1 completa — roda Python, PHP ou qualquer script
- Variáveis de ambiente definidas conforme a especificação CGI
- Body POST encaminhado para stdin do CGI
- Stdout do CGI retornado como resposta HTTP

### Gerenciamento de conexões
- I/O não-bloqueante com `select()`
- Múltiplas conexões simultâneas de clientes
- Timeout gracioso e fechamento de conexão

---

## Um detalhe técnico que se destaca

O servidor é estruturado em um modelo de três camadas claro: `Webserv` (event loop + gerenciamento de sockets), `Connection` (máquina de estados por cliente), e `Client` + `Request`/`Response` (parsing e geração HTTP). Essa separação de responsabilidades — onde cada classe tem uma responsabilidade única e bem definida — é o mesmo padrão de arquitetura usado pelo nginx e pela maioria dos servidores HTTP em produção. Demonstra pensamento maduro de design de software em C++.

---

## Como usar

```bash
git clone https://github.com/gustavofsousa/webserv_42.git
cd webserv_42
make
```

### Executando

```bash
./webserv config/default.conf
```

Abra o navegador em `http://localhost:8080`

### Exemplo de configuração

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

### Testando

```bash
# Requisição GET
curl http://localhost:8080/

# Requisição POST
curl -X POST -d "nome=gustavo" http://localhost:8080/form

# Requisição DELETE
curl -X DELETE http://localhost:8080/arquivo.txt
```

---

## Estrutura do projeto

```
webserv_42/
├── source/
│   ├── main.cpp
│   ├── core/
│   │   ├── Webserv.cpp       # Event loop (select), gerenciamento de sockets
│   │   ├── Server.cpp        # Servidor virtual + binding de config
│   │   └── Connection.cpp    # Estado de conexão por cliente
│   ├── http/
│   │   ├── Request.cpp       # Parser de requisição HTTP
│   │   ├── Response.cpp      # Construtor de resposta HTTP
│   │   ├── Client.cpp        # Máquina de estados do cliente
│   │   ├── CGI.cpp           # Execução CGI
│   │   └── cgi/              # Scripts auxiliares CGI
│   └── config/               # Parser do arquivo de configuração
├── static_pages/             # Arquivos HTML/CSS estáticos
└── index.html
```

---

## Habilidades demonstradas

- Implementação do protocolo HTTP/1.1 (parsing, headers, status codes)
- I/O não-bloqueante com chamada de sistema `select()`
- Programação de sockets TCP (`socket`, `bind`, `listen`, `accept`)
- Implementação da especificação CGI/1.1
- Parsing de configuração estilo NGINX
- Gerenciamento de múltiplas conexões de clientes
- Padrões de design orientado a objetos em C++ (máquina de estados, arquitetura em camadas)

---

## Licença

Este projeto foi desenvolvido como parte do currículo da [42 School](https://42.fr).

---

<p align="center">Feito com ☕ na 42 Rio de Janeiro</p>
