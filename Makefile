.PHONY: build run

build:
	docker build -t openmined/pyyashe .

run:
	docker run -i -t openmined/pyyashe python3 -c "import example; example.test()"
