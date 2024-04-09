#!/usr/bin/env python3
import os
# import sys
# import urllib.parse
# import time 

SUCCESS = 0
FAILURE = 1

base_url = f"http://{os.environ.get('HOST_NAME')}:{os.environ.get('SERVER_PORT')}{os.environ.get('CGI_ROUTE')}"

def html_list(items):
    strs = ""
    strs += "<ul>\n"
    for item in items:
        strs += """
        <li>
            {}
        </li>
        """.format(item)
    strs += "\n</ul>"
    return strs


def html_table(items):
    strs = ""
    strs += "<table>"
    for item in items:
        strs += """<tr style="color:green" >"""
        for i in item:
            strs += """
            <td>
                {}
            </td>""".format(i)
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
    """
        <!DOCTYPE html>
        <html>
        <head>
    """
    strs += "<style>\n"
    # insert style
    strs += generate_style()
    strs += """
    </style>
        <title>
            Python CGI
        </title>
    </head>
    """
    strs += """ 
    <body>
        <h1>Python CGI</h1>

    """
    strs += content
    strs += generate_calculator()
    strs += generate_result()
    strs += "</body>"
    strs += "</html>"
    return strs


def generate_style():
    return """
        h1  {
            border: 1px solid rgb(135, 169, 34);
            background-color: green;
            color: rgb(247, 246, 187);
            padding: 10px;
        } 

        td {
            border: 1px solid rgb(135, 169, 34);
            padding: 5px;
        } 

        * {
            background-color: rgb(17, 66, 50);
            color:rgb(252, 220, 42)
        }
    """

def generate_calculator():
    host_url  =f"{base_url}test_post/"
    return f"""
        <h1>Calculator</h1>
        <form action="{host_url}" method="POST">
            <label for="num1">Number 1:</label>
            <input type="number" id="num1" name="num1" required>
            <br><br>
            <label for="operator">Operator:</label>
            <select id="operator" name="operator" required>
                <option value="add">+</option>
                <option value="sub">-</option>
                <option value="mul">*</option>
                <option value="div">/</option>
            </select>
            <br><br>
            <label for="num2">Number 2:</label>
            <input type="number" id="num2" name="num2" required>
            <br>
            <br>
            <input type="submit" value="Calculate">
        </form>
        """

def generate_result():
    request = os.environ.get("RAW_REQUEST", None)
    if request is None:
        return ""
    result = None
    result = calculator(request.split("\n")[-1])
    return f"""
        <h2>The answer is ...</h2>
        <h2>{result}</h2>
    """

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

    def __init__(self, status_code, body=""):
        self.status_code = status_code
        self.body = body

    def set_header(self, key, value):
        self.headers_dict[key] = value
    
    def generate_respone(self, code=200):
        headers = ""
        self.set_header("Content-Length", len(self.body))
        for key, value in self.headers_dict.items():
            headers += f"{key}: {value}\r\n"
        res = f"""HTTP/1.1 {code} {HttpRespone.HTTP_CODE[code]}\r\n{headers}\r\n"""
        return res

    def set_body(self, body):
        self.body = body
    
    def append_body(self, body):
        self.body += body
    
    def print_respone(self):
        print(self.generate_respone())
        print(self.body)
        print("\r\n")
    
    @classmethod
    def generate_simple_respone(cls, code, body=""):
        size = len(body)
        return f"""HTTP/1.1 {code} {HttpRespone.HTTP_CODE[code]}\r\nContent-Type: text/html\r\nContent-Length:{size}\r\n\r\n{body}\r\n"""


def serve_env():
    body = html_page(html_table([i for i in os.environ.items()]))
    return HttpRespone.generate_simple_respone(200, body)


def directory_listing(path):
    def list_files_and_folders(directory):
        files_and_folders = os.listdir(directory)
        file_list = []
        folder_list = []
        for item in files_and_folders:
            item_path = os.path.join(directory, item)
            if os.path.isfile(item_path):
                file_list.append(item)
            elif os.path.isdir(item_path):
                folder_list.append(item)
        return file_list, folder_list

    def generate_style_tag():
        return """
        <link href="https://fonts.googleapis.com/icon?family=Material+Icons" rel="stylesheet">
        <style>
            * {
                margin: 0;
                padding: 0;
                box-sizing: border-box;
                background-color: rgb(52, 73, 85);
            }

            .flex-containner {
                display: flex;
                justify-content: space-between;
            }

            ul {
                list-style-type:disc;
                width: 80%;
            }


            .file {
                margin-left: 10%;
            }


            .delete-butt{
                font-size: 1rem;
                color: red;
            }

            .delete-butt:hover
            {
                font-size: 2rem;
                cursor: pointer;
            }

            a{
                font-size: 1.5rem;
                text-decoration: none;
                color: #78A083;
                padding-left: 0.5rem;
            }

            .normal-font-color {
                color: #78A083;
            }

            a:hover{
                font-size: 2rem;
                color: #35374B;
            }

            li {
                border-bottom: rgb(120, 160, 131) solid 1px;
                height: 3rem;
                align-items:center;
            }

            h1{
                background-color: rgb(80, 114, 123);
                padding: 1rem 1rem 1rem 1rem;
                color:rgb(52, 73, 85);
            }

            .space-size{
                width: 50%;
            }

            .middle-text {
            }

            .upload-item {
                margin-top: 5rem;
                margin-left:4rem;
                font-size: 1.5rem;
            }
            input {
                color:rgb(53, 55, 75);
                border-radius:4px;
                background:rgb(120, 160, 131);
                padding: 0.5rem;
                margin: 0.3rem;
            }
            button {
                background:rgb(120, 160, 131);
                color:rgb(53, 55, 75);
                padding: 0.5rem;
                border-radius:4px;
            }
            button:hover {
                background:rgb(80, 114, 123);
                color:rgb(53, 55, 75);
                padding: 0.5rem;
                font-size: 1.5rem;
                border-radius:4px;
                cursor: pointer;
            }
        </style>
        """


    def generate_html(directory):
        files, folders = list_files_and_folders(directory)
        html = "<html><head>"
        html += generate_style_tag()
        html += "</head>"
        html += f"<h1>From: {directory}</h1><br><ul>"  # Add this line
        for folder in folders:
            folder_path = "./" + folder + "/"
            html += f"""
            <li class="file flex-containner normal-font-color">
                <div class="space-size" >
                    <i class="material-icons">&#xe2c8;</i>
                    <a class="middle-text" href='{folder_path}'>{folder}</a>
                </div>
                <div >
                    Folder
                </div>
                <div class="delete-butt material-icons">
                    &#xe92b;
                </div>
            </li>
            """
        for file in files:
            file_path = "./" + file
            html += f"""
                <li class="file flex-containner normal-font-color">
                    <div class="space-size">
                        <i class="material-icons">&#xe2c4;</i>
                        <a class="middle-text" href='{file_path}' download>{file}</a>
                    </div>
                    <div>
                        File
                    </div>
                    <div class="delete-butt material-icons">&#xe92b;
                    </div>
                </li>
            """
        html += "</ul>"
        html += generate_script_delete_file()
        html += "</body>\n</html>"
        return html


    def generate_script_delete_file():
        return """
        <script>
                // Add event listener to all delete buttons
                document.querySelectorAll('.delete-butt').forEach(button => {
                    //console.log(button.parentElement.childNodes[1].childNodes[3].href);
                    const node_href = button.parentElement.childNodes[1].childNodes[3].href;
                    
                    button.addEventListener('click', function() {
                        fetch( node_href, {
                            method: 'DELETE',
                        })
                        .then(response => {
                            if (!response.ok) {
                                throw new Error('Not OK na');
                            }
                            button.parentElement.remove();
                        })
                        .catch(errpr=> {
                            console.error("ariva");
                        })
                    });
                });
        </script>
        """

    html = generate_html(path)
    return HttpRespone.generate_simple_respone(200, html)


def parse_request():
    raw_request = os.environ.get("RAW_REQUEST")
    content = """ <h2> Request </h2> """ + html_list(raw_request.split("\r\n"))
    request_list = raw_request.split("\r\n")[0].split(" ")
    for item in request_list:
        content += f"<h2> {item} </h2>"
    if request_list.__len__() != 3:
        return HttpRespone.generate_simple_respone(400)
    if request_list[0] not in ["GET" , "POST"]:
        return HttpRespone.generate_simple_respone(405)
    body = html_page(content)
    return HttpRespone.generate_simple_respone(200, body)


class HttpRequest:
    server_protocol = "HTTP/1.1"
    path_to_file = os.environ.get("PATH_TO_FILE")
    server_port = os.environ.get("SERVER_PORT")
    server_name = os.environ.get("SERVER_NAME")
    directory_listing = os.environ.get("AUTO_INDEX", "disabled")
    _cwd = os.environ.get("ROOT_DIR")

    def __init__(self, raw_request):
        self.raw_request = raw_request
        self.method = ""
        self.path = ""
        self.http_version = ""
        self.headers = {}
        self.body = ""
        self.parse_request()

    def parse_request(self):
        request_lines = self.raw_request.split("\r\n")
        request_line = request_lines[0].split(" ")
        self.method = request_line[0]
        self.path = request_line[1]
        self.http_version = request_line[2]
        for line in request_lines[1:]:
            if line == "":
                break
            key, value = line.split(": ")
            self.headers[key] = value
        if self.method == "POST":
            self.body = request_lines[-1]

    def get_header(self, key):
        return self.headers.get(key, None)
    
    def get_header_str(self):
        return html_list([f"{key} ------ {value}" for key, value in self.headers.items()])
    
    def str_request_info(self):
        t_body = self.body if len(self.body) != 0 else 'Empty body'
        return f"""Method: {self.method}<br>Path: {self.path}<br>Http Version: {self.http_version}<br>Headers: {self.get_header_str()}<br>Body: 
        {t_body}"""
    
def serve_static_file(file_path):
    try:
        with open(file_path,"rb") as f:
            file_content = f.read().decode("utf-8")
            file_extension = os.path.splitext(file_path)[1]
            mime_type = file_extension_to_mime.get(file_extension, "text/plain")
            response = HttpRespone(200)
            response.set_header("Content-Type", mime_type)
            response.set_body(file_content)
            response.print_respone()
            print(file_content)
            print("\r\n")
    except FileNotFoundError:
        return HttpRespone.generate_simple_respone(404, f"File not found: {file_path}")
    except Exception as e:
        return HttpRespone.generate_simple_respone(500, str(e))

def calculator(data_str):
    data = os.environ.get("RAW_REQUEST", None)
    if data is None:
        return "Error: No data received"
    body = data.split("\r\n\r\n")[-1]
    arg = body.split("&")
    m={}
    for i in arg:
        v =i.split("=")
        m[v[0]] = v[1]
    body += html_table(m.items())
    num1 = int(m.get("num1", "0"))
    num2 = int(m.get("num2", "0"))
    operator = m.get("operator", "add")
    if operator == "add":
        result = num1 + num2
    elif operator == "sub":
        result = num1 - num2
    elif operator == "mul":
        result = num1 * num2
    elif operator == "div":
        if num2 != 0:
            result = num1 / num2
        else:
            result = "Error: Division by zero"
    else:
        result = "Error: Invalid operator :" + operator
    
    body += f"<h2>Result: {result}</h2>"
    return body


if __name__ == "__main__":
    try:

        request = os.environ.get("RAW_REQUEST")
        a = HttpRequest(request)
        print(serve_env())


        if a.method == "GET":
            if a.path_to_file.split("/")[-1] == "test_post":
                print(serve_env())
        if a.method == "POST":
            if a.path_to_file.split("/")[-1] == "test_post":
                print(serve_env())
        exit(0)
    except Exception as e:
        exit(1)
