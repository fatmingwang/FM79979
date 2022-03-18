import http.server, ssl
import mimetypes
from http.server import HTTPServer, SimpleHTTPRequestHandler, test
import sys
server_address = ('0.0.0.0', 443)

class CORSRequestHandler (SimpleHTTPRequestHandler):
    def end_headers (self):
	    self.send_header('Cross-Origin-Opener-Policy', 'same-origin')
	    self.send_header('Cross-Origin-Embedder-Policy', 'require-corp')
	    SimpleHTTPRequestHandler.end_headers(self)

httpd = http.server.HTTPServer(server_address, CORSRequestHandler)

httpd.extensions_map=({
    '.manifest': 'text/cache-manifest',
    '.html': 'text/html',
    '.png': 'image/png',
    '.jpg': 'image/jpg',
    '.svg': 'image/svg+xml',
    '.css': 'text/css',
    '.js': 'application/javascript',
    '': 'application/octet-stream',# Default
    })

#https://stackoverflow.com/questions/61729822/strict-mime-type-checking-is-enabled-refused-to-execute-script-from-url-its
mimetypes.add_type("text/javascript", ".js", True)
#httpd.cgi_directories = ["/"]
httpd.socket = ssl.wrap_socket(httpd.socket,
                               server_side=True,
                               certfile='localhost.pem',
                               ssl_version=ssl.PROTOCOL_TLS)
print("ready to start CORS server")
httpd.serve_forever()