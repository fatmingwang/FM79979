#https://stackoverflow.com/questions/21956683/enable-access-control-on-simple-http-server
#https://stackoverflow.com/questions/61729822/strict-mime-type-checking-is-enabled-refused-to-execute-script-from-url-its
import sys
import http.server, ssl
import mimetypes
from http.server import HTTPServer, SimpleHTTPRequestHandler, test

server_address = ('0.0.0.0', 80)
mimetypes.add_type("text/javascript", ".js", True)

##server_address = ('0.0.0.0', 8000)

#class CORSRequestHandler (SimpleHTTPRequestHandler):
#    def end_headers (self):
#	    self.send_header('Cross-Origin-Opener-Policy', 'same-origin')
#	    self.send_header('Cross-Origin-Embedder-Policy', 'require-corp')
#	    SimpleHTTPRequestHandler.end_headers(self)


#if __name__ == '__main__':
#    test(CORSRequestHandler, HTTPServer, port=8000)



class CORSRequestHandler (SimpleHTTPRequestHandler):
    def end_headers (self):
        self.send_header('Cross-Origin-Opener-Policy', 'same-origin')
        self.send_header('Cross-Origin-Embedder-Policy', 'require-corp')
        SimpleHTTPRequestHandler.end_headers(self)

httpd = http.server.HTTPServer(server_address, CORSRequestHandler)
httpd.serve_forever() 
#if __name__ == '__main__':
#    test(CORSRequestHandler, HTTPServer,('0.0.0.0',8000))