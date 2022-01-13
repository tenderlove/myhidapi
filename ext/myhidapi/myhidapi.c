#include <ruby.h>
#include <hidapi.h>
#include <stdlib.h>

VALUE mMyHIDAPI;
VALUE cMyHIDAPIHandle;

static void dealloc(void * ptr)
{
    hid_device *handle = (hid_device *)ptr;
    hid_close(handle);
}

static const rb_data_type_t myhidapi_handle_type = {
    "MyHIDAPI/Handle",
    {0, dealloc, 0,},
    0, 0,
#ifdef RUBY_TYPED_FREE_IMMEDIATELY
    RUBY_TYPED_FREE_IMMEDIATELY,
#endif
};

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
    devs = hid_enumerate(NUM2USHORT(vendor_id), NUM2USHORT(product_id));
    cur_dev = devs;
    while (cur_dev) {
	rb_ary_push(devices, rb_funcall(mMyHIDAPI, rb_intern("build_device_info"), 8,
		INT2NUM(cur_dev->vendor_id),
		INT2NUM(cur_dev->product_id),
		rb_str_new2(cur_dev->path),
		rb_wcstombs(cur_dev->serial_number),
		rb_wcstombs(cur_dev->manufacturer_string),
		rb_wcstombs(cur_dev->product_string),
		INT2NUM(cur_dev->usage),
		INT2NUM(cur_dev->interface_number)
		));
	cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);

    return devices;
}

static VALUE
rb_hid_open(VALUE mod, VALUE vid, VALUE pid)
{
    hid_device *handle;

    handle = hid_open(NUM2USHORT(vid), NUM2USHORT(pid), NULL);

    if (handle) {
	return TypedData_Wrap_Struct(cMyHIDAPIHandle, &myhidapi_handle_type, handle);
    } else {
	return Qfalse;
    }
}

static VALUE
rb_hid_open_path(VALUE mod, VALUE path)
{
    hid_device *handle;

    handle = hid_open_path(StringValueCStr(path));

    if (handle) {
	return TypedData_Wrap_Struct(cMyHIDAPIHandle, &myhidapi_handle_type, handle);
    } else {
	return Qfalse;
    }
}

static VALUE
rb_hid_write(VALUE self, VALUE str)
{
    hid_device *handle;
    int written;

    TypedData_Get_Struct(self, hid_device, &myhidapi_handle_type, handle);

    written = hid_write(handle, (unsigned char *)StringValuePtr(str), RSTRING_LEN(str));

    if (written >= 0) {
	return INT2NUM(written);
    } else {
	return Qfalse;
    }
}

static VALUE
rb_hid_set_nonblocking(VALUE self, VALUE val)
{
    hid_device *handle;
    TypedData_Get_Struct(self, hid_device, &myhidapi_handle_type, handle);
    if (!hid_set_nonblocking(handle, NUM2INT(val))) {
	return Qtrue;
    } else {
	return Qnil;
    }
}

static VALUE
rb_hid_read(VALUE self, VALUE size)
{
    hid_device *handle;
    unsigned char * buf;
    int read;
    VALUE ret;

    TypedData_Get_Struct(self, hid_device, &myhidapi_handle_type, handle);

    buf = xcalloc(NUM2SIZET(size), sizeof(unsigned char));

    read = hid_read(handle, buf, NUM2SIZET(size));

    if (read > 0) {
	ret = rb_str_new((char *)buf, read);
    } else {
	ret = Qnil;
    }
    xfree(buf);
    return ret;
}

static VALUE
rb_hid_read_timeout(VALUE self, VALUE size, VALUE timeout_ms)
{
    hid_device *handle;
    unsigned char * buf;
    int read;
    VALUE ret;

    TypedData_Get_Struct(self, hid_device, &myhidapi_handle_type, handle);

    buf = xcalloc(NUM2SIZET(size), sizeof(unsigned char));

    read = hid_read_timeout(handle, buf, NUM2SIZET(size), NUM2INT(timeout_ms));

    if (read > 0) {
	ret = rb_str_new((char *)buf, read);
    } else {
	ret = Qnil;
    }
    xfree(buf);
    return ret;
}

static VALUE
rb_manufacturer(VALUE self)
{
    hid_device *handle;
    TypedData_Get_Struct(self, hid_device, &myhidapi_handle_type, handle);

    wchar_t buffer[BUF_SIZE];
    hid_get_manufacturer_string(handle, buffer, BUF_SIZE);

    return rb_wcstombs(buffer);
}

static VALUE
rb_product(VALUE self)
{
    hid_device *handle;
    TypedData_Get_Struct(self, hid_device, &myhidapi_handle_type, handle);

    wchar_t buffer[BUF_SIZE];
    hid_get_product_string(handle, buffer, BUF_SIZE);

    return rb_wcstombs(buffer);
}

void Init_myhidapi() {
    mMyHIDAPI = rb_define_module("MyHIDAPI");
    cMyHIDAPIHandle = rb_define_class_under(mMyHIDAPI, "Handle", rb_cObject);
    rb_undef_alloc_func(cMyHIDAPIHandle);
    rb_define_singleton_method(mMyHIDAPI, "enumerate", enumerate, 2);
    rb_define_singleton_method(mMyHIDAPI, "open", rb_hid_open, 2);
    rb_define_singleton_method(mMyHIDAPI, "open_path", rb_hid_open_path, 1);
    rb_define_method(cMyHIDAPIHandle, "write", rb_hid_write, 1);
    rb_define_method(cMyHIDAPIHandle, "read", rb_hid_read, 1);
    rb_define_method(cMyHIDAPIHandle, "read_timeout", rb_hid_read_timeout, 2);
    rb_define_method(cMyHIDAPIHandle, "set_nonblocking", rb_hid_set_nonblocking, 1);
    rb_define_method(cMyHIDAPIHandle, "manufacturer", rb_manufacturer, 0);
    rb_define_method(cMyHIDAPIHandle, "product", rb_product, 0);
}
