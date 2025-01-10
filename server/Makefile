TARGET_DIR = bin

HEADERS = src/utils/*.h \
		src/net/*.h

SOURCES = src/server.cpp \
	src/utils/expressionUtils.cpp \
	src/utils/SimpleJsonBuilder.cpp \
	src/utils/jsonUtils.cpp \
	src/utils/threadUtils.cpp \
	src/net/networkHandlers.cpp \
	src/net/socket.cpp


all: clean configure build

configure:
	@if ! [ -d ./${TARGET_DIR}/ ]; then \
	mkdir ${TARGET_DIR}; \
	fi

build:
	g++ -o ${TARGET_DIR}/server ${SOURCES} -I ${HEADERS}

clean:
	rm -rf ${TARGET_DIR}
