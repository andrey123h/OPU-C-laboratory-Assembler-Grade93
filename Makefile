$(shell mkdir -p ObjectFiles)
$(shell mkdir -p ~/Desktop/maman14)

~/Desktop/maman14/maman14: ObjectFiles/assembler.o ObjectFiles/ass_structs.o ObjectFiles/conversions.o ObjectFiles/data_struct.o ObjectFiles/error.o ObjectFiles/file_handle.o ObjectFiles/first_pass.o ObjectFiles/label_structs.o ObjectFiles/macr_struct.o ObjectFiles/lexer.o ObjectFiles/preprocess.o ObjectFiles/second_pass.o
	gcc -o ~/Desktop/maman14/"maman14" ./ObjectFiles/assembler.o ./ObjectFiles/ass_structs.o ./ObjectFiles/conversions.o ./ObjectFiles/data_struct.o ./ObjectFiles/error.o ./ObjectFiles/file_handle.o ./ObjectFiles/first_pass.o ./ObjectFiles/label_structs.o ./ObjectFiles/macr_struct.o ./ObjectFiles/lexer.o ./ObjectFiles/preprocess.o ./ObjectFiles/second_pass.o -lm

ObjectFiles/assembler.o: assembler.c macr_struct.h preprocess.h errors.h file_handle.h first_pass.h
	gcc -ansi -pedantic -Wall -c assembler.c -o ObjectFiles/assembler.o

ObjectFiles/ass_structs.o: ass_structs.c ass_globals.h ass_structs.h
	gcc -ansi -pedantic -Wall -c ass_structs.c -o ObjectFiles/ass_structs.o

ObjectFiles/conversions.o: conversions.c ass_globals.h errors.h conversions.h
	gcc -ansi -pedantic -Wall -c conversions.c -o ObjectFiles/conversions.o -lm

ObjectFiles/data_struct.o: data_struct.c data_struct.h
	gcc -ansi -pedantic -Wall -c data_struct.c -o ObjectFiles/data_struct.o

ObjectFiles/error.o: error.c errors.h
	gcc -ansi -pedantic -Wall -c error.c -o ObjectFiles/error.o

ObjectFiles/file_handle.o: file_handle.c errors.h file_handle.h ass_globals.h
	gcc -ansi -pedantic -Wall -c file_handle.c -o ObjectFiles/file_handle.o

ObjectFiles/first_pass.o: first_pass.c ass_globals.h label_structs.h file_handle.h ass_structs.h conversions.h first_pass.h errors.h lexer.h second_pass.h data_struct.h macr_struct.h
	gcc -ansi -pedantic -Wall -c first_pass.c -o ObjectFiles/first_pass.o

ObjectFiles/label_structs.o: label_structs.c errors.h ass_globals.h conversions.h file_handle.h label_structs.h
	gcc -ansi -pedantic -Wall -c label_structs.c -o ObjectFiles/label_structs.o

ObjectFiles/macr_struct.o: macr_struct.c macr_struct.h errors.h
	gcc -ansi -pedantic -Wall -c macr_struct.c -o ObjectFiles/macr_struct.o

ObjectFiles/lexer.o: lexer.c errors.h ass_globals.h lexer.h file_handle.h conversions.h data_struct.h label_structs.h
	gcc -ansi -pedantic -Wall -c lexer.c -o ObjectFiles/lexer.o

ObjectFiles/preprocess.o: preprocess.c macr_struct.h errors.h preprocess.h file_handle.h ass_globals.h
	gcc -ansi -pedantic -Wall -c preprocess.c -o ObjectFiles/preprocess.o

ObjectFiles/second_pass.o: second_pass.c ass_globals.h label_structs.h second_pass.h file_handle.h ass_structs.h conversions.h errors.h data_struct.h
	gcc -ansi -pedantic -Wall -c second_pass.c -o ObjectFiles/second_pass.o

clean:
	rm -rf ObjectFiles
	rm -f ~/Desktop/maman14/maman14
