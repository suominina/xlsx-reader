CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g `pkg-config --cflags xlsxwriter`
LIBS = ./libxlsxwriter/libxlsxwriter.a -lz `pkg-config --libs xlsxwriter`

main: main.c xlsx_drone/ext/sxmlc.c xlsx_drone/ext/sxmlsearch.c xlsx_drone/ext/zip.c xlsx_drone/src/xlsx_drone.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

writer: writer.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
