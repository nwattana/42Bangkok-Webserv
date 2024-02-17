all:
	echo "no default target"

build:
	docker build -t nginx-sample .

run:
	docker run -p 8989:80 --name nginx-sample nginx-sample

stop:
	docker stop nginx-sample
	docker rm nginx-sample

re: stop build run
