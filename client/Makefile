TARGET_DIR = bin

HEADERS = src/exceptions/*.h \
	src/utils/*.h

SOURCES = src/client.cpp \
	src/exceptions/InvalidCharacterException.cpp \
	src/utils/commandLineUtils.cpp \
	src/utils/expressionUtils.cpp


all: clean configure build

configure:
	@if ! [ -d ./${TARGET_DIR}/ ]; then \
	mkdir ${TARGET_DIR}; \
	fi

build:
	g++ -o ${TARGET_DIR}/client ${SOURCES} -I ${HEADERS}

clean:
	rm -rf ${TARGET_DIR}

help:
	@echo "Доступные команды:"
	@echo "make all       - Выполнить полный процесс сборки"
	@echo "make configure - Настраивает необходимые директории для сборки, если их нет"
	@echo "make clean     - Очищает все временные директории"
	@echo "make help      - Вывести информацию о Makefile"
