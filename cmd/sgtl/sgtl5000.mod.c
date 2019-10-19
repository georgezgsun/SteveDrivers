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
	{ 0xe56a9336, __VMLINUX_SYMBOL_STR(snd_pcm_format_width) },
	{ 0xf9a482f9, __VMLINUX_SYMBOL_STR(msleep) },
	{ 0x815588a6, __VMLINUX_SYMBOL_STR(clk_enable) },
	{ 0x2e5810c6, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr1) },
	{ 0x5f44f804, __VMLINUX_SYMBOL_STR(snd_soc_dapm_get_enum_double) },
	{ 0x124cd469, __VMLINUX_SYMBOL_STR(i2c_del_driver) },
	{ 0x5714b586, __VMLINUX_SYMBOL_STR(snd_soc_dapm_get_volsw) },
	{ 0x7bf39a36, __VMLINUX_SYMBOL_STR(regmap_update_bits_base) },
	{ 0xb6e6d99d, __VMLINUX_SYMBOL_STR(clk_disable) },
	{ 0xf7802486, __VMLINUX_SYMBOL_STR(__aeabi_uidivmod) },
	{ 0x196f62da, __VMLINUX_SYMBOL_STR(snd_soc_put_volsw) },
	{ 0xe7950eff, __VMLINUX_SYMBOL_STR(regmap_read) },
	{ 0x8aa6a71f, __VMLINUX_SYMBOL_STR(snd_soc_get_volsw) },
	{ 0x9f8e7cf4, __VMLINUX_SYMBOL_STR(regulator_bulk_enable) },
	{ 0x2e7c0045, __VMLINUX_SYMBOL_STR(snd_soc_info_enum_double) },
	{ 0xb4c33cbd, __VMLINUX_SYMBOL_STR(snd_soc_dapm_kcontrol_dapm) },
	{ 0xe707d823, __VMLINUX_SYMBOL_STR(__aeabi_uidiv) },
	{ 0x27fd12f0, __VMLINUX_SYMBOL_STR(snd_soc_read) },
	{ 0xd1001298, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x6c64bf51, __VMLINUX_SYMBOL_STR(snd_soc_update_bits) },
	{ 0x64d75bb2, __VMLINUX_SYMBOL_STR(regulator_bulk_get) },
	{ 0xc4dd7043, __VMLINUX_SYMBOL_STR(snd_soc_dapm_put_volsw) },
	{ 0x8e865d3c, __VMLINUX_SYMBOL_STR(arm_delay_ops) },
	{ 0x2196324, __VMLINUX_SYMBOL_STR(__aeabi_idiv) },
	{ 0x59e5070d, __VMLINUX_SYMBOL_STR(__do_div64) },
	{ 0x57223539, __VMLINUX_SYMBOL_STR(snd_soc_info_volsw) },
	{ 0x857fbc4b, __VMLINUX_SYMBOL_STR(i2c_register_driver) },
	{ 0x9f19d656, __VMLINUX_SYMBOL_STR(_dev_info) },
	{ 0xb585b2eb, __VMLINUX_SYMBOL_STR(regulator_bulk_disable) },
	{ 0x12a38747, __VMLINUX_SYMBOL_STR(usleep_range) },
	{ 0xb74956b1, __VMLINUX_SYMBOL_STR(__devm_regmap_init_i2c) },
	{ 0xc4fab7f6, __VMLINUX_SYMBOL_STR(regulator_get_optional) },
	{ 0x7c9a7371, __VMLINUX_SYMBOL_STR(clk_prepare) },
	{ 0xc98ed1b3, __VMLINUX_SYMBOL_STR(regulator_get_voltage) },
	{ 0xbd0259aa, __VMLINUX_SYMBOL_STR(devm_clk_get) },
	{ 0x74272ba2, __VMLINUX_SYMBOL_STR(snd_soc_unregister_codec) },
	{ 0x1c059810, __VMLINUX_SYMBOL_STR(regulator_put) },
	{ 0xefd6cf06, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr0) },
	{ 0x7d2f4777, __VMLINUX_SYMBOL_STR(regulator_bulk_free) },
	{ 0x8e24a96a, __VMLINUX_SYMBOL_STR(of_property_read_variable_u32_array) },
	{ 0x5fdbbf56, __VMLINUX_SYMBOL_STR(snd_soc_register_codec) },
	{ 0xac1ca138, __VMLINUX_SYMBOL_STR(devm_kmalloc) },
	{ 0x53ca2270, __VMLINUX_SYMBOL_STR(regmap_write) },
	{ 0x276cc928, __VMLINUX_SYMBOL_STR(regcache_sync) },
	{ 0x10c9fcf5, __VMLINUX_SYMBOL_STR(regcache_cache_only) },
	{ 0x26626e5e, __VMLINUX_SYMBOL_STR(snd_soc_write) },
	{ 0xcb84b78d, __VMLINUX_SYMBOL_STR(snd_soc_dapm_put_enum_double) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("i2c:sgtl5000");
MODULE_ALIAS("of:N*T*Cfsl,t-sgtl5000");
MODULE_ALIAS("of:N*T*Cfsl,t-sgtl5000C*");

MODULE_INFO(srcversion, "10D3CF95C0C62123762191B");
