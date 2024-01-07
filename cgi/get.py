#!/usr/bin/env python3
import cgi

print("Content-Type: text/html\n")
form = cgi.FieldStorage()

if "num1" in form and "num2" in form and "operacao" in form:
    try:
        num1 = float(form["num1"].value)
        num2 = float(form["num2"].value)
        operacao = form["operacao"].value

        resultado = None

        if operacao == "soma":
            resultado = num1 + num2
        elif operacao == "subtracao":
            resultado = num1 - num2
        elif operacao == "multiplicacao":
            resultado = num1 * num2
        elif operacao == "divisao":
            if num2 != 0:
                resultado = num1 / num2
            else:
                resultado = "Erro: Divisão por zero não é permitida!"

        # Exibe o resultado
        if resultado is not None:
            print(f"<p>O resultado de {num1} {operacao} {num2} é: {resultado}</p>")
    except ValueError:
        print("<p>Erro: Por favor, insira números válidos.</p>")
else:
    # Se não houver dados enviados, exibe o formulário
    print("O campos 'num1' ou 'num2' estão vazioas.")


# #!/usr/bin/env python3
# import cgi
# import cgitb
# import os

# cgitb.enable()  # for troubleshooting


# print(os.environ["QUERY_STRING"])


# query_params = parse_qs(query_string)

# # Access individual parameters
# param1 = query_params.get('n1', [''])[0]
# param2 = query_params.get('n2', [''])[0]

# print(param1,param2)
# print("Content-type: text/html\n")
# print("<html><head>")
# print("<title>CGI script output</title>")
# print("</head><body>")

# print("</body></html>")

# # from urllib.parse import urlparse
# # o = urlparse(my_url)
# # print(o)
# # print(o.query)