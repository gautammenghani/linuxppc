/* SPDX-License-Identifier: GPL-2.0 */
#include <stdlib.h>
#include <perf/threadmap.h>
#include <perf/py_perf.h>

static PyObject * program_perf_thread_map__new_dummy(PyObject *self, PyObject *args)
{
	py_perf_thread_map *pythread_map;
	PyObject *obj;

	pythread_map = calloc(sizeof(py_perf_thread_map), 1);
	if (!pythread_map)
		Py_RETURN_FALSE;

	pythread_map->ptr = perf_thread_map__new_dummy();

	obj = Py_BuildValue("&O", pythread_map);
	return obj;
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

	return m;
}

