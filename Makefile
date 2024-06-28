# This a docker container merely for development

project_name=psn-gm

default: docker-build docker-run

docker-build:
	docker build -t $(project_name)-dev:latest .

docker-run:
	docker run --rm -it $(project_name)-dev:latest /bin/bash
