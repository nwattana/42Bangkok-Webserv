#!/usr/bin/python3

def handle_call():
	# print env
	import os
	import sys
	print("argv: ", sys.argv)
	print(os.environ)
	

if __name__ == '__main__':
	handle_call()
	exit(0)