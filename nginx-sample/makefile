all:
	echo "no default target"

build:
	touch build
	docker build -t nginx-sample .

run:
	touch run
	docker run --rm -p 8989:80 --name nginx-sample nginx-sample

check-config: rm build
	docker exec -it nginx-sample nginx -t

rm:
	rm -f build
	rm -f run


re: rm build run
