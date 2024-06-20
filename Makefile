compile: gap_buffer.c attom_pad.c attom_pad_editor.c attom.c 
	gcc gap_buffer.c attom_pad.c attom_pad_editor.c attom.c -o targets/attom -g -lncurses
gap_buffer_test: _gap_buffer_test
	targets/gap_buffer_test
_gap_buffer_test: 
	gcc gap_buffer_test.c gap_buffer.c -o targets/gap_buffer_test -g
