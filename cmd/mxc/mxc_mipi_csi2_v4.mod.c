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
	{ 0xa24dd6c7, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x815588a6, __VMLINUX_SYMBOL_STR(clk_enable) },
	{ 0xb6e6d99d, __VMLINUX_SYMBOL_STR(clk_disable) },
	{ 0x5e3a0a3e, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x556e4390, __VMLINUX_SYMBOL_STR(clk_get_rate) },
	{ 0x9c98ac29, __VMLINUX_SYMBOL_STR(__platform_driver_register) },
	{ 0xf6149621, __VMLINUX_SYMBOL_STR(of_property_read_variable_u8_array) },
	{ 0x83f76c59, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x79c5a9f0, __VMLINUX_SYMBOL_STR(ioremap) },
	{ 0x84b183ae, __VMLINUX_SYMBOL_STR(strncmp) },
	{ 0x42f4a70a, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0x2f3bdc31, __VMLINUX_SYMBOL_STR(platform_get_resource) },
	{ 0x310917fe, __VMLINUX_SYMBOL_STR(sort) },
	{ 0xbf8ed6ec, __VMLINUX_SYMBOL_STR(module_put) },
	{ 0x14037c3, __VMLINUX_SYMBOL_STR(kmem_cache_alloc) },
	{ 0x7c9a7371, __VMLINUX_SYMBOL_STR(clk_prepare) },
	{ 0xbd0259aa, __VMLINUX_SYMBOL_STR(devm_clk_get) },
	{ 0x822137e2, __VMLINUX_SYMBOL_STR(arm_heavy_mb) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xedc03953, __VMLINUX_SYMBOL_STR(iounmap) },
	{ 0xefd6cf06, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr0) },
	{ 0x11039834, __VMLINUX_SYMBOL_STR(platform_driver_unregister) },
	{ 0x8e24a96a, __VMLINUX_SYMBOL_STR(of_property_read_variable_u32_array) },
	{ 0x57c05c47, __VMLINUX_SYMBOL_STR(try_module_get) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "F832F3B982D52024A665F57");
