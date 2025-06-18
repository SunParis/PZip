.PHONY: all build_dir configure build clean push

all: build_dir configure build

build_dir:
	@if [ ! -d "build" ]; then \
	    mkdir build; \
	fi

configure: build_dir
	@cd build && cmake ..

build: configure
	@cd build && make -j4

clean:
	@if [ -d "build" ]; then \
	    cd build && make clean; \
	fi

push:
	@if [ -n "$(ARGS)" ]; then \
	    git add .; \
		git commit -m "$(ARGS)"; \
		git push; \
	else \
	    git add .; \
		git commit -m "Update sth"; \
		git push; \
	fi
