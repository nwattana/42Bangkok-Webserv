#!/usr/bin/env python3

import os

def show_error(error_code):
  """Reads the error file and displays a 404 error page."""
  new_path = os.getcwd() + "/www/error/"  + str(error_code) + ".html"

  # Read the error file (modify the path as needed)
  with open(new_path, "r") as error_file:
    error_html = error_file.read()

  # Print the error content
  print(error_html)

# Main execution
if __name__ == "__main__":
  show_error(400)
  # show_error(401)
  # show_error(404)
  # show_error(405)
  # show_error(406)
  # show_error(407)
