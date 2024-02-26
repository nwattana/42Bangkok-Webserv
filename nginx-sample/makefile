all:
	echo "no default target"

build:
	docker build -t nginx-sample .

run:
	docker run --rm -p 8989:80 --name nginx-sample nginx-sample

re: build run
