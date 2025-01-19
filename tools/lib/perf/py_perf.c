/* SPDX-License-Identifier: GPL-2.0 */
#include "include/perf/py_perf.h"
#include <stdlib.h>
#include <perf/threadmap.h>
#include <perf/py_perf.h>
#include <errno.h>
#include <internal/threadmap.h>

static PyObject *program_perf_thread_map__new_dummy(PyObject *self, PyObject *args)
{
	py_perf_thread_map *pythread_map = PyObject_New(py_perf_thread_map, &py_perf_thread_map_type);

	pythread_map->ptr = perf_thread_map__new_dummy();

	return Py_BuildValue("O", pythread_map);
}

PyMethodDef libperf_methods[] = {
	{"perf_thread_map__new_dummy", program_perf_thread_map__new_dummy, METH_VARARGS, "Create a dummy thread map function variable"},
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

