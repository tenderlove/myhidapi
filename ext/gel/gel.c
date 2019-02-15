#include <ruby.h>
#include <hidapi.h>
#include <stdlib.h>

VALUE mGel;

#define BUF_SIZE 4096

static VALUE
rb_wcstombs(wchar_t *str)
{
    char buf[BUF_SIZE];
    int len;
    len = wcstombs(buf, str, BUF_SIZE);
    if (len > 0) {
	return rb_str_new(buf, len);
    } else {
	return Qnil;
    }
}

static VALUE
enumerate(VALUE mod, VALUE vendor_id, VALUE product_id)
{
    VALUE devices;
    struct hid_device_info *devs, *cur_dev;

    devices = rb_ary_new();
    devs = hid_enumerate(NUM2SHORT(vendor_id), NUM2SHORT(product_id));
    cur_dev = devs;
    while (cur_dev) {
	rb_ary_push(devices, rb_funcall(mGel, rb_intern("build_device_info"), 6,
		INT2NUM(cur_dev->vendor_id),
		INT2NUM(cur_dev->product_id),
		rb_str_new2(cur_dev->path),
		rb_wcstombs(cur_dev->serial_number),
		rb_wcstombs(cur_dev->manufacturer_string),
		rb_wcstombs(cur_dev->product_string)
		));
	cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);

    return devices;
}

static VALUE
rb_hid_open(VALUE vid, VALUE pid)
{
    hid_device *handle;

    handle = hid_open(NUM2SHORT(vid), NUM2SHORT(pid), NULL);
    if (handle) {
	return Qtrue;
    } else {
	return Qfalse;
    }
}

void Init_gel() {
    mGel = rb_define_module("Gel");
    rb_define_singleton_method(mGel, "enumerate", enumerate, 2);
    rb_define_singleton_method(mGel, "open", rb_hid_open, 2);
}
