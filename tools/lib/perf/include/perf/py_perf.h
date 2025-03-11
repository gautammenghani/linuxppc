/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBPERF_PY_PERF_H
#define __LIBPERF_PY_PERF_H

#define PY_SSIZE_T_CLEAN
#include <stdlib.h>
#include <linux/perf_event.h>
#include <linux/kernel.h>
#include <perf/evlist.h>
#include <internal/evlist.h>
#include <perf/evsel.h>
#include <perf/threadmap.h>
#include <Python.h>

// perf_thread_map declaration
typedef struct {
	PyObject_HEAD
	struct perf_thread_map *thread_map;
} py_perf_thread_map;

static void py_perf_thread_map_dealloc(py_perf_thread_map *thread_map)
{
	free(thread_map->thread_map);
	Py_DECREF(thread_map);
	PyObject_Del((PyObject *)thread_map);
}

static PyTypeObject py_perf_thread_map_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name = "libperf.py_perf_thread_map",
	.tp_doc = "Perf thread map object",
	.tp_basicsize = sizeof(py_perf_thread_map),
	.tp_dealloc = (destructor)py_perf_thread_map_dealloc,
	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
};

// perf_evsel declarations
typedef struct {
	PyObject_HEAD
	struct perf_evsel *evsel;
} py_perf_evsel;

static void py_perf_evsel_dealloc(py_perf_evsel *evsel)
{
	Py_DECREF(evsel);
	PyObject_Del((PyObject *)evsel);
}

static PyTypeObject py_perf_evsel_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name = "libperf.py_perf_evsel",
	.tp_doc = "Perf evsel object",
	.tp_basicsize = sizeof(py_perf_evsel),
	.tp_dealloc = (destructor)py_perf_evsel_dealloc,
	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
};

// perf_evlist declarations
typedef struct {
	PyObject_HEAD
	struct perf_evlist *evlist;
} py_perf_evlist;

static void py_perf_evlist_dealloc(py_perf_evlist *evlist)
{
	free(evlist->evlist);
	Py_DECREF(evlist);
	PyObject_Del((PyObject *)evlist);
}

typedef struct {
    PyObject_HEAD
    PyObject *evlist;
    struct list_head *current;
} py_perf_evlist_iterator;

static PyObject *evlist_iterator_next(py_perf_evlist_iterator *iter) {

	py_perf_evsel *pyperf_evsel = PyObject_New(py_perf_evsel, &py_perf_evsel_type);
	struct list_head *head;

	if (((py_perf_evlist *)(iter->evlist))->evlist == NULL) {
		PyErr_SetString(PyExc_RuntimeError, "perf_evlist has been closed");
		return NULL;
	}
	head = &((py_perf_evlist *)(iter->evlist))->evlist->entries;
	if (iter->current == head) {
		PyErr_SetNone(PyExc_StopIteration);
		return NULL;
	}
	pyperf_evsel->evsel = list_entry(iter->current, struct perf_evsel, node);
	iter->current = iter->current->next;
	Py_INCREF(iter->evlist);

	return (PyObject *)pyperf_evsel;
}

static PyTypeObject py_perf_evlist_iterator_type = {
        PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "libperf.evlist_iterator_type",
        .tp_basicsize = sizeof(py_perf_evlist_iterator),
        .tp_itemsize = 0,
        .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
        .tp_doc = "evlist_iterator object",
        .tp_iter = PyObject_SelfIter,
        .tp_iternext = (iternextfunc) evlist_iterator_next,
};

static PyObject *py_perf_evlist_iter(py_perf_evlist *self) {
	py_perf_evlist_iterator *iter = PyObject_New(py_perf_evlist_iterator, &py_perf_evlist_iterator_type);
	if (!iter)
		return NULL;
	iter->current = self->evlist->entries.next;
	iter->evlist = (PyObject *)self;
	Py_INCREF(self);
	return (PyObject *)iter;
}

static PyTypeObject py_perf_evlist_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name = "libperf.py_perf_evlist",
	.tp_doc = "Perf evlist object",
	.tp_basicsize = sizeof(py_perf_evlist),
	.tp_dealloc = (destructor)py_perf_evlist_dealloc,
	.tp_iter = (getiterfunc) py_perf_evlist_iter,
	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
};

// perf_cpu_map declarations
typedef struct {
	PyObject_HEAD
	struct perf_cpu_map *map;
} py_perf_cpu_map;

static void py_perf_cpu_map_dealloc(py_perf_cpu_map *cpu_map)
{
	free(cpu_map->map);
	Py_DECREF(cpu_map);
	PyObject_Del((PyObject *)cpu_map);
}

static PyTypeObject py_perf_cpu_map_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name = "libperf.py_perf_cpu_map",
	.tp_doc = "Perf cpu_map object",
	.tp_basicsize = sizeof(py_perf_cpu_map),
	.tp_dealloc = (destructor)py_perf_cpu_map_dealloc,
	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
};

// perf_event_attr declarations
typedef struct {
	PyObject_HEAD
	struct perf_event_attr *attr;
} py_perf_event_attr;

#define PY_STRUCT_GET_SET_FUNC_LONG(name, c_type, element)					\
static PyObject *py_##name##_##element##_get(py_##name *self, void *closure)			\
{												\
	return PyLong_FromLong(self->c_type->element);						\
}												\
												\
static int py_##name##_##element##_set(py_##name *self, PyObject *value, void *closure)		\
{												\
												\
	if (!PyLong_Check(value))								\
		return -1;									\
												\
	self->c_type->element = PyLong_AsLong(value);						\
												\
	return 0;										\
}

#define GET_SET_DEF(name, element)								\
	{#element, (getter)py_##name##_##element##_get, (setter)py_##name##_##element##_set, NULL, NULL}

PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, type)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, size)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, config)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, sample_period)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, sample_freq)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, sample_type)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, read_format)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, disabled)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, inherit)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, pinned)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, exclusive)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, exclude_user)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, exclude_kernel)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, exclude_hv)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, exclude_idle)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, mmap)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, comm)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, freq)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, inherit_stat)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, enable_on_exec)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, task)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, watermark)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, precise_ip)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, mmap_data)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, sample_id_all)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, exclude_host)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, exclude_guest)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, exclude_callchain_kernel)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, exclude_callchain_user)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, mmap2)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, comm_exec)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, use_clockid)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, context_switch)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, write_backward)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, namespaces)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, ksymbol)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, bpf_event)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, aux_output)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, cgroup)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, text_poke)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, build_id)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, inherit_thread)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, remove_on_exec)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, sigtrap)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, __reserved_1)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, wakeup_events)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, wakeup_watermark)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, bp_type)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, bp_addr)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, kprobe_func)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, uprobe_path)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, config1)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, bp_len)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, kprobe_addr)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, probe_offset)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, config2)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, branch_sample_type)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, sample_regs_user)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, sample_stack_user)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, clockid)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, sample_regs_intr)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, aux_watermark)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, sample_max_stack)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, __reserved_2)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, aux_sample_size)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, __reserved_3)
PY_STRUCT_GET_SET_FUNC_LONG(perf_event_attr, attr, sig_data)


static PyGetSetDef py_perf_event_attr_getset[] = {
	GET_SET_DEF(perf_event_attr, type),
	GET_SET_DEF(perf_event_attr, size),
	GET_SET_DEF(perf_event_attr, config),
	GET_SET_DEF(perf_event_attr, sample_period),
	GET_SET_DEF(perf_event_attr, sample_freq),
	GET_SET_DEF(perf_event_attr, sample_type),
	GET_SET_DEF(perf_event_attr, read_format),
	GET_SET_DEF(perf_event_attr, disabled),
	GET_SET_DEF(perf_event_attr, inherit),
	GET_SET_DEF(perf_event_attr, pinned),
	GET_SET_DEF(perf_event_attr, exclusive),
	GET_SET_DEF(perf_event_attr, exclude_user),
	GET_SET_DEF(perf_event_attr, exclude_kernel),
	GET_SET_DEF(perf_event_attr, exclude_hv),
	GET_SET_DEF(perf_event_attr, exclude_idle),
	GET_SET_DEF(perf_event_attr, mmap),
	GET_SET_DEF(perf_event_attr, comm),
	GET_SET_DEF(perf_event_attr, freq),
	GET_SET_DEF(perf_event_attr, inherit_stat),
	GET_SET_DEF(perf_event_attr, enable_on_exec),
	GET_SET_DEF(perf_event_attr, task),
	GET_SET_DEF(perf_event_attr, watermark),
	GET_SET_DEF(perf_event_attr, precise_ip),
	GET_SET_DEF(perf_event_attr, mmap_data),
	GET_SET_DEF(perf_event_attr, sample_id_all),
	GET_SET_DEF(perf_event_attr, exclude_host),
	GET_SET_DEF(perf_event_attr, exclude_guest),
	GET_SET_DEF(perf_event_attr, exclude_callchain_kernel),
	GET_SET_DEF(perf_event_attr, exclude_callchain_user),
	GET_SET_DEF(perf_event_attr, mmap2),
	GET_SET_DEF(perf_event_attr, comm_exec),
	GET_SET_DEF(perf_event_attr, use_clockid),
	GET_SET_DEF(perf_event_attr, context_switch),
	GET_SET_DEF(perf_event_attr, write_backward),
	GET_SET_DEF(perf_event_attr, namespaces),
	GET_SET_DEF(perf_event_attr, ksymbol),
	GET_SET_DEF(perf_event_attr, bpf_event),
	GET_SET_DEF(perf_event_attr, aux_output),
	GET_SET_DEF(perf_event_attr, cgroup),
	GET_SET_DEF(perf_event_attr, text_poke),
	GET_SET_DEF(perf_event_attr, build_id),
	GET_SET_DEF(perf_event_attr, inherit_thread),
	GET_SET_DEF(perf_event_attr, remove_on_exec),
	GET_SET_DEF(perf_event_attr, sigtrap),
	GET_SET_DEF(perf_event_attr, __reserved_1),
	GET_SET_DEF(perf_event_attr, wakeup_events),
	GET_SET_DEF(perf_event_attr, wakeup_watermark),
	GET_SET_DEF(perf_event_attr, bp_type),
	GET_SET_DEF(perf_event_attr, bp_addr),
	GET_SET_DEF(perf_event_attr, kprobe_func),
	GET_SET_DEF(perf_event_attr, uprobe_path),
	GET_SET_DEF(perf_event_attr, config1),
	GET_SET_DEF(perf_event_attr, bp_len),
	GET_SET_DEF(perf_event_attr, kprobe_addr),
	GET_SET_DEF(perf_event_attr, probe_offset),
	GET_SET_DEF(perf_event_attr, config2),
	GET_SET_DEF(perf_event_attr, branch_sample_type),
	GET_SET_DEF(perf_event_attr, sample_regs_user),
	GET_SET_DEF(perf_event_attr, sample_stack_user),
	GET_SET_DEF(perf_event_attr, clockid),
	GET_SET_DEF(perf_event_attr, sample_regs_intr),
	GET_SET_DEF(perf_event_attr, aux_watermark),
	GET_SET_DEF(perf_event_attr, sample_max_stack),
	GET_SET_DEF(perf_event_attr, __reserved_2),
	GET_SET_DEF(perf_event_attr, aux_sample_size),
	GET_SET_DEF(perf_event_attr, __reserved_3),
	GET_SET_DEF(perf_event_attr, sig_data),
	{NULL}
};

static PyObject *py_perf_event_attr_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	py_perf_event_attr *self = (py_perf_event_attr *) type->tp_alloc(type, 0);

	if (!self)
		return NULL;

	self->attr = calloc(1, sizeof(struct perf_event_attr));

	return (PyObject *)self;
}

static void py_perf_event_attr_dealloc(py_perf_event_attr *ctr)
{
	free(ctr->attr);
	Py_DECREF(ctr);
	PyObject_Del((PyObject *)ctr);
}

static PyTypeObject py_perf_event_attr_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name = "libperf.py_perf_event_attr",
	.tp_doc = "perf event attribute structure object",
	.tp_basicsize = sizeof(py_perf_event_attr),
	.tp_new = py_perf_event_attr_new,
	.tp_dealloc = (destructor)py_perf_event_attr_dealloc,
	.tp_flags = Py_TPFLAGS_DEFAULT| Py_TPFLAGS_BASETYPE,
	.tp_getset = py_perf_event_attr_getset,
};

// perf_counts_values declarations
typedef struct {
	PyObject_HEAD
	struct perf_counts_values *values;
} py_perf_counts_values;

static void py_perf_counts_values_dealloc(py_perf_counts_values *values)
{
	free(values->values);
	Py_DECREF(values);
	PyObject_Del((PyObject *)values);
}

static PyObject * py_perf_counts_values_get_values(py_perf_counts_values *self, void *closure)
{
	PyObject *list = PyList_New(5);
	if (!list)
		return NULL;
	for (int i = 0; i < 5; i++) {
		PyList_SetItem(list, i, PyLong_FromLong(self->values->values[i]));
	}
	return list;
}

static int py_perf_counts_values_set_values(py_perf_counts_values *self, PyObject *value, void *closure)
{
	if (!PyLong_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "Values must be u64");
		return -1;
	}
	for(int i = 0; i < 5; i++) {
		self->values->values[i] = PyLong_AsLong(value);
	}
	return 0;
}
PY_STRUCT_GET_SET_FUNC_LONG(perf_counts_values, values, val)
PY_STRUCT_GET_SET_FUNC_LONG(perf_counts_values, values, ena)
PY_STRUCT_GET_SET_FUNC_LONG(perf_counts_values, values, run)
PY_STRUCT_GET_SET_FUNC_LONG(perf_counts_values, values, id)
PY_STRUCT_GET_SET_FUNC_LONG(perf_counts_values, values, lost)

static PyGetSetDef py_perf_counts_values_getsetters[] = {
	GET_SET_DEF(perf_counts_values, val),
	GET_SET_DEF(perf_counts_values, ena),
	GET_SET_DEF(perf_counts_values, run),
	GET_SET_DEF(perf_counts_values, id),
	GET_SET_DEF(perf_counts_values, lost),
	{"values", (getter)py_perf_counts_values_get_values, (setter)py_perf_counts_values_set_values,"values", NULL},
    {NULL}
};

static PyObject *py_perf_counts_values_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	py_perf_counts_values *self = (py_perf_counts_values *) type->tp_alloc(type, 0);

	if (!self)
		return NULL;

	self->values = calloc(1, sizeof(struct perf_counts_values));

	return (PyObject *)self;
}

static PyTypeObject py_perf_counts_values_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name = "libperf.py_perf_counts_values",
	.tp_doc = "Perf_counts_values object",
	.tp_basicsize = sizeof(py_perf_counts_values),
	.tp_new = py_perf_counts_values_new,
	.tp_dealloc = (destructor)py_perf_counts_values_dealloc,
	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
	.tp_getset = py_perf_counts_values_getsetters,
};

static void python_push_type(const char *name, PyObject *module, PyTypeObject *type)
{
	if (PyType_Ready(type) == -1)
		printf("python_push_type: failed to ready %s", name);

	Py_INCREF(type);
}

LIBPERF_API PyMODINIT_FUNC PyInit_libperf(void);

#endif /* __LIBPERF_PY_PERF_H */
