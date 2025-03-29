default:
	swig -python  raw_pylibperf.swg
	gcc -fPIC -c raw_pylibperf_wrap.c -I/usr/include/python3.12 -I/usr/include -lperf
	gcc -shared raw_pylibperf_wrap.o -lperf -o _raw_pylibperf.so

