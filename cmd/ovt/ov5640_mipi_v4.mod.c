#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x3c1b4d25, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xb077e70a, __VMLINUX_SYMBOL_STR(clk_unprepare) },
	{ 0xf2b40b9b, __VMLINUX_SYMBOL_STR(i2c_master_send) },
	{ 0xa0bf6e76, __VMLINUX_SYMBOL_STR(device_remove_file) },
	{ 0xf9a482f9, __VMLINUX_SYMBOL_STR(msleep) },
	{ 0x9ca14873, __VMLINUX_SYMBOL_STR(regulator_set_voltage) },
	{ 0x815588a6, __VMLINUX_SYMBOL_STR(clk_enable) },
	{ 0x2e5810c6, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr1) },
	{ 0x124cd469, __VMLINUX_SYMBOL_STR(i2c_del_driver) },
	{ 0x70056e77, __VMLINUX_SYMBOL_STR(regulator_disable) },
	{ 0xf6b912b1, __VMLINUX_SYMBOL_STR(i2c_transfer) },
	{ 0x2ef5bcbf, __VMLINUX_SYMBOL_STR(gpio_to_desc) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xe707d823, __VMLINUX_SYMBOL_STR(__aeabi_uidiv) },
	{ 0xfa2a45e, __VMLINUX_SYMBOL_STR(__memzero) },
	{ 0xd1001298, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0xc683c541, __VMLINUX_SYMBOL_STR(v4l2_int_device_register) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x20c55ae0, __VMLINUX_SYMBOL_STR(sscanf) },
	{ 0x7d7dabf3, __VMLINUX_SYMBOL_STR(v4l2_int_device_unregister) },
	{ 0x298543c4, __VMLINUX_SYMBOL_STR(devm_gpio_request_one) },
	{ 0x857fbc4b, __VMLINUX_SYMBOL_STR(i2c_register_driver) },
	{ 0xcf89b090, __VMLINUX_SYMBOL_STR(device_create_file) },
	{ 0xd070b10e, __VMLINUX_SYMBOL_STR(devm_regulator_get) },
	{ 0x7c9a7371, __VMLINUX_SYMBOL_STR(clk_prepare) },
	{ 0x35b6efe, __VMLINUX_SYMBOL_STR(of_get_named_gpio_flags) },
	{ 0xbd0259aa, __VMLINUX_SYMBOL_STR(devm_clk_get) },
	{ 0x32f92b4b, __VMLINUX_SYMBOL_STR(gpiod_set_raw_value) },
	{ 0x99248e0b, __VMLINUX_SYMBOL_STR(dev_warn) },
	{ 0xefd6cf06, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr0) },
	{ 0x8e24a96a, __VMLINUX_SYMBOL_STR(of_property_read_variable_u32_array) },
	{ 0x848570d4, __VMLINUX_SYMBOL_STR(regulator_enable) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=v4l2-int-device";

MODULE_ALIAS("of:N*T*Cisl,isl79987_mipi");
MODULE_ALIAS("of:N*T*Cisl,isl79987_mipiC*");
MODULE_ALIAS("i2c:isl79987_mipi");

MODULE_INFO(srcversion, "27AC93420E8FA70BF8B7B0B");
