CC = gcc
CFLAGS = -Wall -ansi -pedantic -o $@ -g
assembler:  global_helpers.o code_image.o data_image.o assembler.o first_pass.o second_pass.o table.o files.o pre_processor.o is_valid_line.o identify.o commands.o
	$(CC) $(CFLAGS) $^
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< 

