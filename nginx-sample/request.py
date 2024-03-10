import requests

port 8989
url = 'http://localhost:32768'

response = requests.get(url)

print("head : ", response.headers)
print("repone content : ", response.content)
print("encoding std : ", response.encoding)
print("reponse code : ", response.status_code)
print("reponse text : ", response.text)
