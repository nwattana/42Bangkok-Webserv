#!/usr/bin/python3

def handle_call():
	with open('../asset/index.html', 'r') as f:
		print(f.read())

if __name__ == '__main__':
	handle_call()