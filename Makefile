CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g #`pkg-config --cflags xlsxwriter`
LIBS = ./libxlsxwriter/libxlsxwriter.a -lz #`pkg-config --libs xlsxwriter`

main: main.c lib/ext/sxmlc.c lib/ext/sxmlsearch.c lib/ext/zip.c lib/src/xlsx_drone.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

writer: writer.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
