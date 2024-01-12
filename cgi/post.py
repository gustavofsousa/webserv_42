#!/usr/bin/env python3
import os

import cgi

uploaded = cgi.FieldStorage()
path = os.path

# env_var = os.environ


def check_file() -> str:
    if "file" in uploaded:
        uploaded_file = uploaded["file"]
        if not path.exists("./cgi/tmp"):
            os.makedirs("./cgi/tmp")
        file_path = "./cgi/tmp/" + path.basename(uploaded_file.filename)
        open(file_path, "wb").write(uploaded_file.file.read())
        return os.getcwd() + "/cgi/tmp/" + path.basename(uploaded_file.filename)
    else:
        print( "Error: uploading failed")

response_body = [
    "HTTP/1.1 200 OK",
    "Content-type: text/html;charset=utf-8\n",
    "<html>",
    "<head>",
    "<title>Successful upload</title>",
    '<body>',
    '<h1>file uploaded to:</h1>'
    "</body>",
    "</html>",
]

[print(i) for i in response_body]
# for key, value in env_var.items():
#     print(f'{key}={value}')