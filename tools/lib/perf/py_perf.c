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
	//py_perf_thread_map *pythread_map = NULL;

	/*if (!PyArg_ParseTuple(args, "O", &pythread_map)) {
		return NULL;
	}*/

	pythread_map->thread_map = perf_thread_map__new_dummy();
	if (!pythread_map->thread_map) {
		Py_DECREF(pythread_map);
		return Py_None;
	}

	return Py_BuildValue("O", pythread_map);
}

static PyObject *program_perf_thread_map__set_pid(PyObject *self, PyObject *args)
{
	py_perf_thread_map *pythread_map = NULL;
	int idx, pid;

	if (!PyArg_ParseTuple(args, "Oii", &pythread_map, &idx, &pid)) {
		return NULL;
	}

	perf_thread_map__set_pid(pythread_map->thread_map, idx, pid);

	return Py_None;
}

static PyObject *program_perf_evlist__new(PyObject *self, PyObject *args)
{
	py_perf_evlist *pyperf_evlist = PyObject_New(py_perf_evlist, &py_perf_evlist_type);

	pyperf_evlist->evlist = perf_evlist__new();
	if (!pyperf_evlist->evlist) {
		Py_DECREF(pyperf_evlist);
		return Py_None;
	}

	return Py_BuildValue("O", pyperf_evlist);
}

static PyObject *program_perf_evsel__new(PyObject *self, PyObject *args)
{
	struct perf_event_attr *attr;
	PyObject *py_attr;
	py_perf_evsel *pyperf_evsel = PyObject_New(py_perf_evsel, &py_perf_evsel_type);

	if (!PyArg_ParseTuple(args, "O", &py_attr)) {
		return NULL;
	}

	attr = (py_attr == Py_None)? NULL: ((py_perf_event_attr *)py_attr)->attr;
	pyperf_evsel->evsel = perf_evsel__new(attr);
	if (!pyperf_evsel->evsel) {
		Py_DECREF(pyperf_evsel);
		return Py_None;
	}

	return Py_BuildValue("O", pyperf_evsel);
}

static PyObject *program_perf_evlist__add(PyObject *self, PyObject *args)
{
	struct perf_evsel *evsel;
	struct perf_evlist *evlist;
	PyObject *pyevlist, *pyevsel;

	if (!PyArg_ParseTuple(args, "OO", &pyevlist, &pyevsel)) {
		return NULL;
	}

	//Py_INCREF(pyevlist);
	//Py_INCREF(pyevsel);
	evsel = (pyevsel == Py_None)? NULL: ((py_perf_evsel *)pyevsel)->evsel;
	evlist = (pyevlist == Py_None)? NULL: ((py_perf_evlist *)pyevlist)->evlist;
	perf_evlist__add(evlist, evsel);

	return Py_None;
}

static PyObject *program_perf_evlist__set_maps(PyObject *self, PyObject *args)
{
	struct perf_thread_map *thread_map;
	struct perf_evlist *evlist;
	struct perf_cpu_map *cpu_map;
	PyObject *pyevlist, *pythread_map, *pycpu_map;

	if (!PyArg_ParseTuple(args, "OOO", &pyevlist, &pycpu_map, &pythread_map)) {
		return NULL;
	}

	//Py_INCREF(pyevlist);
	//Py_INCREF(pythread_map);
	//Py_INCREF(pycpu_map);
	evlist = (pyevlist == Py_None)? NULL: ((py_perf_evlist *)pyevlist)->evlist;
	cpu_map = (pycpu_map == Py_None)? NULL: ((py_perf_cpu_map *)pycpu_map)->ptr;
	thread_map = (pythread_map == Py_None)? NULL: ((py_perf_thread_map *)pythread_map)->thread_map;
	perf_evlist__set_maps(evlist, cpu_map, thread_map);

	return Py_None;
}

static PyObject *program_perf_evlist__open(PyObject *self, PyObject *args)
{
	struct perf_evlist *evlist;
	PyObject *pyevlist;

	if (!PyArg_ParseTuple(args, "O", &pyevlist)) {
		return NULL;
	}

	//Py_INCREF(pyevlist);
	//Py_INCREF(pythread_map);
	//Py_INCREF(pycpu_map);
	evlist = (pyevlist == Py_None)? NULL: ((py_perf_evlist *)pyevlist)->evlist;

	return Py_BuildValue("i", perf_evlist__open(evlist));
}

static PyObject *program_perf_evlist__enable(PyObject *self, PyObject *args)
{
	struct perf_evlist *evlist;
	PyObject *pyevlist;

	if (!PyArg_ParseTuple(args, "O", &pyevlist)) {
		return NULL;
	}

	evlist = (pyevlist == Py_None)? NULL: ((py_perf_evlist *)pyevlist)->evlist;

	perf_evlist__enable(evlist);

	return Py_None;
}

static PyObject *program_perf_evlist__disable(PyObject *self, PyObject *args)
{
	struct perf_evlist *evlist;
	PyObject *pyevlist;

	if (!PyArg_ParseTuple(args, "O", &pyevlist)) {
		return NULL;
	}

	evlist = (pyevlist == Py_None)? NULL: ((py_perf_evlist *)pyevlist)->evlist;

	perf_evlist__disable(evlist);

	return Py_None;
}

static PyObject *program_perf_evsel__read(PyObject *self, PyObject *args)
{
	PyObject *pyevsel, *pyperf_counts_values;
	struct perf_evsel *evsel;
	struct perf_counts_values *values;
	int cpu_map_idx, thread;

	if (!PyArg_ParseTuple(args, "OiiO", &pyevsel, &cpu_map_idx, &thread, &pyperf_counts_values)) {
		return NULL;
	}

	evsel = (pyevsel == Py_None)? NULL: ((py_perf_evsel *)pyevsel)->evsel;
	values = (pyevsel == Py_None)? NULL: ((py_perf_counts_values *)pyperf_counts_values)->values;

	return Py_BuildValue("i", perf_evsel__read(evsel, cpu_map_idx, thread, values));
}

static PyObject *program_perf_evlist__close(PyObject *self, PyObject *args)
{
	struct perf_evlist *evlist;
	PyObject *pyevlist;

	if (!PyArg_ParseTuple(args, "O", &pyevlist)) {
		return NULL;
	}

	evlist = (pyevlist == Py_None)? NULL: ((py_perf_evlist *)pyevlist)->evlist;

	perf_evlist__close(evlist);

	return Py_None;
}

static PyObject *program_perf_evlist__delete(PyObject *self, PyObject *args)
{
	struct perf_evlist *evlist;
	PyObject *pyevlist;

	if (!PyArg_ParseTuple(args, "O", &pyevlist)) {
		return NULL;
	}

	evlist = (pyevlist == Py_None)? NULL: ((py_perf_evlist *)pyevlist)->evlist;

	perf_evlist__delete(evlist);

	return Py_None;
}

static PyObject *program_perf_thread_map__put(PyObject *self, PyObject *args)
{
	struct perf_thread_map *map;
	PyObject *pyperf_thread_map;

	if (!PyArg_ParseTuple(args, "O", &pyperf_thread_map)) {
		return NULL;
	}

	map = (pyperf_thread_map == Py_None)? NULL: ((py_perf_thread_map *)pyperf_thread_map)->thread_map;

	perf_thread_map__put(map);

	return Py_None;
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
	{"perf_thread_map__set_pid", program_perf_thread_map__set_pid, METH_VARARGS, "Set PID for a thread map"},
	{"perf_evlist__new", program_perf_evlist__new, METH_VARARGS, "Create a perf evlist"},
	{"perf_evsel__new", program_perf_evsel__new, METH_VARARGS, "Create a perf evsel"},
	{"perf_evlist__add", program_perf_evlist__add, METH_VARARGS, "Add evsel to evlist"},
	{"perf_evlist__set_maps", program_perf_evlist__set_maps, METH_VARARGS, "perf_evlist__set_maps"},
	{"perf_evlist__open", program_perf_evlist__open, METH_VARARGS, "perf_evlist__set_maps"},
	{"perf_evlist__enable", program_perf_evlist__enable, METH_VARARGS, "perf_evlist__enable"},
	{"perf_evlist__disable", program_perf_evlist__disable, METH_VARARGS, "perf_evlist__disable"},
	{"perf_evsel__read", program_perf_evsel__read, METH_VARARGS, "perf_evsel__read"},
	{"perf_evlist__close", program_perf_evlist__close, METH_VARARGS, "perf_evlist__close"},
	{"perf_evlist__delete", program_perf_evlist__delete, METH_VARARGS, "perf_evlist__delete"},
	{"perf_thread_map__put", program_perf_thread_map__put, METH_VARARGS, "perf_thread_map__put"},
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
	python_push_type("py_perf_evlist", m, &py_perf_evlist_type);
	python_push_type("evlist_iterator", m, &evlist_iterator_type);
	python_push_type("py_perf_evsel", m, &py_perf_evsel_type);
	python_push_type("py_perf_cpu_map", m, &py_perf_cpu_map_type);
	python_push_type("py_perf_event_attr", m, &py_perf_event_attr_type);
	python_push_type("py_perf_counts_values", m, &py_perf_counts_values_type);

	PyModule_AddObject(m, "perf_event_attr", (PyObject *) & py_perf_event_attr_type);
	PyModule_AddObject(m, "perf_counts_values", (PyObject *) & py_perf_counts_values_type);

	return m;
}

