compile: gap_buffer.c atom_pad.c
	gcc gap_buffer.c atom_pad.c -o targets/attom.o -g 
gap_buffer_test: _gap_buffer_test
	targets/gap_buffer_test
_gap_buffer_test: 
	gcc gap_buffer_test.c gap_buffer.c -o targets/gap_buffer_test -g
