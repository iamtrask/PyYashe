.PHONY: run_example compile_and_run build run dev shell

compile:
	swig -c++ -python -py3 example.i
	python3 setup.py install

run_example:
	python3 -c "import example; example.test()"

compile_and_run: compile
	make run_example

# Build the Docker environment
build:
	docker build -t openmined/pyyashe .

# Run the source code inside the Docker environment
run:
	docker run -i -t openmined/pyyashe make run_example

# Run *my* source code with the Docker environment
dev:
	docker run -i -t -v "$(PWD):/PyYashe" openmined/pyyashe make compile_and_run

# Open a shell inside the Docker environment, for debugging
shell:
	docker run -i -t -v "$(PWD):/PyYashe" openmined/pyyashe sh

