#!/usr/bin/env python3
import os, sys
import mimetypes

SUCCESS = 0
FAILURE = 1


def html_list(items):
    strs = ""
    strs += "<ul>"
    for item in items:
        strs += "<li>{}</li>".format(item)
    strs += "</ul>"
    return strs


def html_table(items):
    strs = ""
    strs += "<table>"
    for item in items:
        strs += "<tr style=>"
        for i in item:
            strs += "<td>{}</td>".format(i)
        strs += "</tr>"
    strs += "</table>"
    return strs


file_extension_to_mime = {
    '.html': 'text/html',
    '.htm': 'text/html',
    '.css': 'text/css',
    '.js': 'application/javascript',
    '.json': 'application/json',
    '.xml': 'application/xml',
    '.jpg': 'image/jpeg',
    '.jpeg': 'image/jpeg',
    '.png': 'image/png',
    '.gif': 'image/gif',
    '.bmp': 'image/bmp',
    '.ico': 'image/x-icon',
    '.svg': 'image/svg+xml',
    '.pdf': 'application/pdf',
    '.doc': 'application/msword',
    '.docx': 'application/vnd.openxmlformats-officedocument.wordprocessingml.document',
    '.xls': 'application/vnd.ms-excel',
    '.xlsx': 'application/vnd.openxmlformats-officedocument.spreadsheetml.sheet',
    '.ppt': 'application/vnd.ms-powerpoint',
    '.pptx': 'application/vnd.openxmlformats-officedocument.presentationml.presentation'
}

def html_page(content):
    strs = ""
    strs += "<!DOCTYPE html>"
    strs += "<html>"
    strs += "<head>"
    strs += "<style> h1{border-style:solid; background-color: green;} td{border: 5px solid red;} </style>"
    strs += "<title >Python CGI</title>"
    strs += "</head>"
    strs += "<body>"
    strs += "<h1>Python CGI</h1>"
    strs += content
    strs += "</body>"
    strs += "</html>"
    return strs


class HttpRespone:
    GENERIC_HEADERS = [
        "Content-Type",
        "Content-Length",
    ]
    headers_dict = {
        "Content-Type": "text/html",
        "Content-Length": 0,
    }

    HTTP_CODE = {
        100: "Continue",
        101: "Switching Protocols",
        102: "Processing",
        103: "Early Hints",
        200: "OK",
        201: "Created",
        202: "Accepted",
        203: "Non-Authoritative Information",
        204: "No Content",
        205: "Reset Content",
        206: "Partial Content",
        207: "Multi-Status",
        208: "Already Reported",
        226: "IM Used",
        300: "Multiple Choices",
        301: "Moved Permanently",
        302: "Found",
        303: "See Other",
        304: "Not Modified",
        305: "Use Proxy",
        306: "Switch Proxy",
        307: "Temporary Redirect",
        308: "Permanent Redirect",
        400: "Bad Request",
        401: "Unauthorized",
        402: "Payment Required",
        403: "Forbidden",
        404: "Not Found",
        405: "Method Not Allowed",
        406: "Not Acceptable",
        407: "Proxy Authentication Required",
        408: "Request Timeout",
        409: "Conflict",
        410: "Gone",
        411: "Length Required",
        412: "Precondition Failed",
        413: "Payload Too Large",
        414: "URI Too Long",
        415: "Unsupported Media Type",
        416: "Range Not Satisfiable",
        417: "Expectation Failed",
        418: "I'm a teapot",
        421: "Misdirected Request",
        422: "Unprocessable Entity",
        423: "Locked",
        424: "Failed Dependency",
        425: "Too Early",
        426: "Upgrade Required",
        428: "Precondition Required",
        429: "Too Many Requests",
        431: "Request Header Fields Too Large",
        451: "Unavailable For Legal Reasons",
        500: "Internal Server Error",
        501: "Not Implemented",
        502: "Bad Gateway",
        503: "Service Unavailable",
        504: "Gateway Timeout",
        505: "HTTP Version Not Supported",
        506: "Variant Also Negotiates",
        507: "Insufficient Storage",
        508: "Loop Detected",
        510: "Not Extended",
        511: "Network Authentication Required",
    }

    def __init__(self, status_code, body):
        self.status_code = status_code
        self.body = body

    def set_header(self, key, value):
        self.headers_dict[key] = value



if __name__ == "__main__":
    method = os.environ.get("REQUEST_METHOD")
    request = os.environ.get("PATH_TO_FILE")
    file_request = os.environ.get("PATH_TO_FILE")

    body = html_page(html_table([i for i in os.environ.items()]))

    status_line = "HTTP/1.1 200 OK\r\n"
    header = "Content-Type: text/html\r\n"
    header += "Content-Length: {}\r\n".format(len(body))

    print("{}{}\r\n{}".format(status_line, header, body))
