#!/usr/bin/env python3
import sys
import os

def error_info_dict(error_code):
  error_data = {
    400: {
      "errno": 400,
      "param": "Bad Request",
      "details": "Your request couldn't be understood by the server. Please check the URL or form data for any typos."
    },
    401: {
      "errno": 401,
      "param": "Unauthorized",
      "details": "You tried to access a page that requires authorization (username and password). Please log in or check your credentials."
    },
    404: {
      "errno": 404,
      "param": "Page not found",
      "details": "The page you requested could not be found. It might not exist or have been moved."
    },
    405: {
      "errno": 405,
      "param": "Method Not Allowed",
      "details": "This method is not allowed for this page."
    },
    406: {
      "errno": 406,
      "param": "Not Acceptable",
      "details": "The server can't provide content in a format that meets your requirements."
    },
    407: {
      "errno": 407,
      "param": "Proxy Authentication Required",
      "details": "Proxy Authentication Required. You need to log in with your proxy server credentials to access this content."
    }
  }
  return error_data.get(error_code)

def generate_error_html(error_code):
  """Generates the HTML content for a error page."""
  error_data = error_info_dict(error_code)

  html_content = f"""
    <!DOCTYPE html>
    <html>
      <meta charset="UTF-8">
      <style>
        body {{
          margin: 0;
          padding: 0;
          font-family: sans-serif;
          background-color: #11284D;
          text-align: center;
          display: flex;
          flex-direction: column;
          justify-content: center;
          align-items: center;
        }}
        h1 {{
          font-size: 200px;
          font-weight: 700;
          line-height: 40px;
          top: 100px;
          color: #ffffff;
          -webkit-text-stroke-width: 8px;
          -webkit-text-stroke-color: rgb(90, 93, 97);
        }}
        h2 {{
          font-size: 40px;
          font-weight: 700;
          line-height: 30px;
          color: #ffffff;
          bottom: 30px;
          -webkit-text-stroke-width: 1px;
          -webkit-text-stroke-color: rgb(255, 255, 255);
          line-height: 80px;
        }}
        h3 {{
          font-size: 30px;
          font-weight: 700;
          line-height: 20px;
          letter-spacing: 1px;
          color: #76afdb;
          line-height-step: 10px;
          line-height: 50px;
        }}
        .text-box {{
          width: 70%;
          max-width: 100%;
          text-overflow: ellipsis;
        }}

        @media (max-width: 768px) {{
          h2 {{
            font-size: 30px;
            line-height: 35px;
          }}
          h3 {{
            font-size: 20px;
            line-height: 25px;
          }}
          .text-box {{
            width: 80%;
          }}
        }}
      </style>
      <body>
        <br>
        <div class="text-box">
          <h1>{error_code}</h1>
          <h2>Oops! {error_data['param']} &#128577; </h2>
          <h3>{error_data['details']}</h3>
        </div>
      </body>
    </html>
  """

  print(html_content)

# You can use this generated HTML content in your error handling logic (e.g., with a web framework)
# Main execution
if __name__ == "__main__":
  generate_error_html(400)
  sys.exit(1)
  # generate_error_html(401)
  # generate_error_html(404)
  # generate_error_html(405)
  # generate_error_html(406)
  # generate_error_html(407)
