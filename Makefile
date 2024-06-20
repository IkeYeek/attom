gap_buffer_test: _gap_buffer_test
	targets/gap_buffer_test
_gap_buffer_test: 
	gcc gap_buffer_test.c gap_buffer.c -o targets/gap_buffer_test -g
