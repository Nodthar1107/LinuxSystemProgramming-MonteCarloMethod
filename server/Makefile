TARGET_DIR = bin

HEADERS = src/utils/*.h \
		src/net/*.h \
		src/console/console.h

SOURCES = src/server.cpp \
	src/utils/commandLineUtils.cpp \
	src/utils/expressionUtils.cpp \
	src/utils/SimpleJsonBuilder.cpp \
	src/utils/jsonUtils.cpp \
	src/net/networkHandlers.cpp \
	src/net/socket.cpp \
	src/console/console.cpp \


all: clean configure build

configure:
	@if ! [ -d ./${TARGET_DIR}/ ]; then \
	mkdir ${TARGET_DIR}; \
	fi

build:
	g++ -o ${TARGET_DIR}/server ${SOURCES} -I ${HEADERS}

clean:
	rm -rf ${TARGET_DIR}

help:
	@echo "Доступные команды:"
	@echo "make all       - Выполнить полный процесс сборки"
	@echo "make configure - Настраивает необходимые директории для сборки, если их нет"
	@echo "make clean     - Очищает все временные директории"
	@echo "make help      - Вывести информацию о Makefile"
