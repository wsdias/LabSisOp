import os
import time
import BaseHTTPServer


HOST_NAME = '10.0.2.15'
PORT_NUMBER = 8000


class MyHandler(BaseHTTPServer.BaseHTTPRequestHandler):
    def do_HEAD(s):
        s.send_response(200)
        s.send_header("Content-type", "text/html")
        s.end_headers()
    def do_GET(s):
        """Respond to a GET request."""
        s.send_response(200)
        s.send_header("Content-type", "text/html")
        s.end_headers()
        s.wfile.write("<html><head><title>Target Information</title></head>")
        s.wfile.write("<body>")

        # Date
        s.wfile.write("<p>Date : ")
        s.wfile.write(os.popen('TZ=GMT+3 date | awk \'{print $3 " " $2 " " $6 ", " $4}\'').read())
        s.wfile.write("</p>")

        # Uptime
        s.wfile.write("<p>Uptime : ")
        s.wfile.write(os.popen('cat /proc/uptime | awk \'{print int($1) "s"}\'').read())
        s.wfile.write("</p>")

        # CPU Model
        s.wfile.write("<p>CPU Model")
        s.wfile.write(os.popen('cat /proc/cpuinfo | grep "model name"').read().strip('model name:'))
        s.wfile.write("</p>")

        # CPU Frequency
        s.wfile.write("<p>CPU Frequency : ")
        s.wfile.write(os.popen('cat /proc/cpuinfo | grep "cpu MHz" | awk \'{print $4}\'').read())
        s.wfile.write(" MHz</p>")

        # CPU  Usage
        s.wfile.write("<p>CPU Usage : ")
        s.wfile.write(os.popen('iostat -c | awk \'{print 100 - $7}\'').read().split('\n')[2])
        s.wfile.write("%</p>")

        # Total Memory
        s.wfile.write("<p>Total Memory : ")
        s.wfile.write(os.popen('cat /proc/meminfo | grep "MemTotal" | awk \'{print $2/1024}\'').read())
        s.wfile.write(" MB</p>")

        # Memory Usage
        s.wfile.write("<p>Memory Usage : ")
        a = os.popen('cat /proc/meminfo | grep "MemTotal" | awk \'{print $2/1024}\'').read()
        b = os.popen('cat /proc/meminfo | grep "MemAvailable" | awk \'{print $2/1024}\'').read()
        s.wfile.write(float(a)-float(b))
        s.wfile.write(" MB</p>")

        # System Version
        s.wfile.write("<p>System Version : ")
        s.wfile.write(os.popen('cat /proc/version').read())
        s.wfile.write("</p>")

        # System Processes
        s.wfile.write("<p>System Processes : ")
        s.wfile.write(os.popen('ps -A | awk \'{print "<p style=text-indent:5em;>" $1 " - " $3 " " $4 " " $5 "</p>"}\'').read())
        s.wfile.write("</p>")    

        s.wfile.write("</body></html>")


if __name__ == '__main__':
    server_class = BaseHTTPServer.HTTPServer
    httpd = server_class((HOST_NAME, PORT_NUMBER), MyHandler)
    print time.asctime(), "Server Starts - %s:%s" % (HOST_NAME, PORT_NUMBER)
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass
    httpd.server_close()
    print time.asctime(), "Server Stops - %s:%s" % (HOST_NAME, PORT_NUMBER)

