#!/bin/bash

curl -X POST \
	-d "name=John" \
	-d "age=30" \
	-i \
	http://localhost:8080/files/1