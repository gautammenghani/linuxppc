/* SPDX-License-Identifier: GPL-2.0 */
#include "include/perf/py_perf.h"
#include <stdlib.h>
#include <perf/threadmap.h>
#include <perf/py_perf.h>
#include <errno.h>
#include <internal/threadmap.h>

static PyObject *program_perf_thread_map__new_dummy(PyObject *self, PyObject *args)
{
	//py_perf_thread_map *pythread_map = PyObject_New(py_perf_thread_map, &py_perf_thread_map_type);
	py_perf_thread_map *pythread_map = NULL;

	if (!PyArg_ParseTuple(args, "O", &pythread_map)) {
		return NULL;
	}

	pythread_map->ptr = perf_thread_map__new_dummy();
	if (!pythread_map->ptr) {
		Py_DECREF(pythread_map);
		return Py_None;
	}

	return Py_BuildValue("O", pythread_map);
}

static int libperf_print(enum libperf_print_level level,
			  const char *fmt, va_list ap)
{
	return vfprintf(stderr, fmt, ap);
}

static PyObject *program_libperf_init(PyObject *self, PyObject *args)
{
       libperf_init(libperf_print);
       return Py_None;
}

PyMethodDef libperf_methods[] = {
	{"perf_thread_map__new_dummy", program_perf_thread_map__new_dummy, METH_VARARGS, "Create a dummy thread map function variable"},
	{"libperf_init", program_libperf_init, METH_VARARGS, "libperf init"},
	{NULL, NULL, 0, NULL}
};

struct PyModuleDef libperf = {
	PyModuleDef_HEAD_INIT,
	"libperf",
	"Create a dummy thread map function variable",
	-1,
	libperf_methods
};

PyMODINIT_FUNC PyInit_libperf(void) {
	PyObject *m = PyModule_Create(&libperf);

	if (!m)
		return NULL;

	python_push_type("py_perf_thread_map", m, &py_perf_thread_map_type);

	return m;
}

