import os

datahora = os.popen('date').read()

s.wfile.write("<html><head><title>Title goes here.</title></head>")
s.wfile.write("<body><p>This is a test.</p>")

s.wfile.write("<p>Data e Hora: %s</p>" % datahora)

s.wfile.write("</body></html>")