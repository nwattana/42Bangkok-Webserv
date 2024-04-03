#!/usr/bin/env python3
import os

def get_env_as_dict():
  return dict(os.environ)

def get_value_by_key(key):
  env_dict = get_env_as_dict()
  value = env_dict.get(key)
  return value


if __name__ == "__main__":
  env_dict = get_env_as_dict()
  print(f"<p>{env_dict}")

  for i in env_dict.items():
    print(f"<h3>{i}</h3>")

  USER = get_value_by_key("USER")
  RAW_REQUEST = get_value_by_key("RAW_REQUEST")
  REQUEST_BODY = get_value_by_key("REQUEST_BODY")
  REQUEST_METHOD = get_value_by_key("REQUEST_METHOD")
  SERVER_PROTOCAL = get_value_by_key("SERVER_PROTOCAL")
  PATH_TO_FILE = get_value_by_key("PATH_TO_FILE")
  HOST_NAME = get_value_by_key("HOST_NAME")

  print(f"<h2>{USER} {RAW_REQUEST} {REQUEST_BODY} {REQUEST_METHOD} {SERVER_PROTOCAL} {PATH_TO_FILE} {HOST_NAME}</h2>")
  print(f"<h1>Hello Word!</h1>")

