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
	{ 0xd31ccb06, __VMLINUX_SYMBOL_STR(of_machine_is_compatible) },
	{ 0x92b57248, __VMLINUX_SYMBOL_STR(flush_work) },
	{ 0x2d3385d3, __VMLINUX_SYMBOL_STR(system_wq) },
	{ 0x265ad50, __VMLINUX_SYMBOL_STR(ipu_get_soc) },
	{ 0xa0bf6e76, __VMLINUX_SYMBOL_STR(device_remove_file) },
	{ 0xa24dd6c7, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x815588a6, __VMLINUX_SYMBOL_STR(clk_enable) },
	{ 0xfbc626af, __VMLINUX_SYMBOL_STR(param_ops_int) },
	{ 0x2e5810c6, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr1) },
	{ 0x484fbf21, __VMLINUX_SYMBOL_STR(ipu_check_task) },
	{ 0xc755e3aa, __VMLINUX_SYMBOL_STR(ipu_queue_task) },
	{ 0x3218d1ac, __VMLINUX_SYMBOL_STR(video_device_release) },
	{ 0x1c9e3e9d, __VMLINUX_SYMBOL_STR(ipu_csi_set_window_pos) },
	{ 0xfa25d867, __VMLINUX_SYMBOL_STR(arm_dma_ops) },
	{ 0x7bf39a36, __VMLINUX_SYMBOL_STR(regmap_update_bits_base) },
	{ 0x4aee4c6b, __VMLINUX_SYMBOL_STR(video_usercopy) },
	{ 0x6b720a95, __VMLINUX_SYMBOL_STR(v4l2_device_unregister) },
	{ 0xb6e6d99d, __VMLINUX_SYMBOL_STR(clk_disable) },
	{ 0xc3dcf963, __VMLINUX_SYMBOL_STR(ipu_csi_window_size_crop) },
	{ 0xb57b6586, __VMLINUX_SYMBOL_STR(ipu_unlink_channels) },
	{ 0xf7802486, __VMLINUX_SYMBOL_STR(__aeabi_uidivmod) },
	{ 0x92350382, __VMLINUX_SYMBOL_STR(mipi2_csi2_get_error2) },
	{ 0x6b06fdce, __VMLINUX_SYMBOL_STR(delayed_work_timer_fn) },
	{ 0x1afae5e7, __VMLINUX_SYMBOL_STR(down_interruptible) },
	{ 0x4205ad24, __VMLINUX_SYMBOL_STR(cancel_work_sync) },
	{ 0x6781b1b8, __VMLINUX_SYMBOL_STR(ipu_clear_buffer_ready) },
	{ 0x185b33f4, __VMLINUX_SYMBOL_STR(__video_register_device) },
	{ 0x8fdf772a, __VMLINUX_SYMBOL_STR(init_timer_key) },
	{ 0x5e3a0a3e, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x3955857d, __VMLINUX_SYMBOL_STR(ipu_csi_set_window_size) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0x9c98ac29, __VMLINUX_SYMBOL_STR(__platform_driver_register) },
	{ 0xf4fa543b, __VMLINUX_SYMBOL_STR(arm_copy_to_user) },
	{ 0xe2d5255a, __VMLINUX_SYMBOL_STR(strcmp) },
	{ 0x1767309b, __VMLINUX_SYMBOL_STR(v4l2_device_register) },
	{ 0x1329f46a, __VMLINUX_SYMBOL_STR(ipu_disp_set_window_pos) },
	{ 0xc631580a, __VMLINUX_SYMBOL_STR(console_unlock) },
	{ 0x5f78f185, __VMLINUX_SYMBOL_STR(ipu_init_channel_buffer) },
	{ 0x275ef902, __VMLINUX_SYMBOL_STR(__init_waitqueue_head) },
	{ 0xe707d823, __VMLINUX_SYMBOL_STR(__aeabi_uidiv) },
	{ 0xfa2a45e, __VMLINUX_SYMBOL_STR(__memzero) },
	{ 0xe8e64fbc, __VMLINUX_SYMBOL_STR(bytes_per_pixel) },
	{ 0x90577242, __VMLINUX_SYMBOL_STR(ipu_csi_enable_mclk) },
	{ 0x45d63fe1, __VMLINUX_SYMBOL_STR(cancel_delayed_work) },
	{ 0xaf0d7129, __VMLINUX_SYMBOL_STR(video_device_alloc) },
	{ 0xd1001298, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0xf657d55c, __VMLINUX_SYMBOL_STR(ipu_enable_channel) },
	{ 0x51d559d1, __VMLINUX_SYMBOL_STR(_raw_spin_unlock_irqrestore) },
	{ 0xc683c541, __VMLINUX_SYMBOL_STR(v4l2_int_device_register) },
	{ 0x99f29177, __VMLINUX_SYMBOL_STR(mipi2_csi2_get_error1) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xa7c4cbab, __VMLINUX_SYMBOL_STR(video_unregister_device) },
	{ 0xa1c76e0a, __VMLINUX_SYMBOL_STR(_cond_resched) },
	{ 0xb542b2ad, __VMLINUX_SYMBOL_STR(v4l2_int_ioctl_1) },
	{ 0x9b7e44dc, __VMLINUX_SYMBOL_STR(ipu_disable_csi) },
	{ 0x84b183ae, __VMLINUX_SYMBOL_STR(strncmp) },
	{ 0xfbaaf01e, __VMLINUX_SYMBOL_STR(console_lock) },
	{ 0x73e20c1c, __VMLINUX_SYMBOL_STR(strlcpy) },
	{ 0x16305289, __VMLINUX_SYMBOL_STR(warn_slowpath_null) },
	{ 0x42f4a70a, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0xbb26dcfb, __VMLINUX_SYMBOL_STR(ipu_clear_irq) },
	{ 0xf473ffaf, __VMLINUX_SYMBOL_STR(down) },
	{ 0xc353a348, __VMLINUX_SYMBOL_STR(ipu_update_channel_buffer) },
	{ 0x462c6416, __VMLINUX_SYMBOL_STR(dma_release_from_coherent) },
	{ 0xe8afe264, __VMLINUX_SYMBOL_STR(ipu_channel_request) },
	{ 0x7d7dabf3, __VMLINUX_SYMBOL_STR(v4l2_int_device_unregister) },
	{ 0x3cd877e9, __VMLINUX_SYMBOL_STR(of_match_device) },
	{ 0x622598b1, __VMLINUX_SYMBOL_STR(init_wait_entry) },
	{ 0x96d5dbf9, __VMLINUX_SYMBOL_STR(dma_alloc_from_coherent) },
	{ 0x5030799, __VMLINUX_SYMBOL_STR(ipu_free_irq) },
	{ 0xcf89b090, __VMLINUX_SYMBOL_STR(device_create_file) },
	{ 0x78c57281, __VMLINUX_SYMBOL_STR(ipu_channel_disable) },
	{ 0xeabb45cc, __VMLINUX_SYMBOL_STR(mipi_csi2_pixelclk_enable) },
	{ 0x14037c3, __VMLINUX_SYMBOL_STR(kmem_cache_alloc) },
	{ 0xbd90b419, __VMLINUX_SYMBOL_STR(video_devdata) },
	{ 0x85d472aa, __VMLINUX_SYMBOL_STR(queue_delayed_work_on) },
	{ 0x1000e51, __VMLINUX_SYMBOL_STR(schedule) },
	{ 0xd62c833f, __VMLINUX_SYMBOL_STR(schedule_timeout) },
	{ 0x7c9a7371, __VMLINUX_SYMBOL_STR(clk_prepare) },
	{ 0xee6b71c4, __VMLINUX_SYMBOL_STR(syscon_regmap_lookup_by_compatible) },
	{ 0xc2395fd6, __VMLINUX_SYMBOL_STR(registered_fb) },
	{ 0x9c0bd51f, __VMLINUX_SYMBOL_STR(_raw_spin_lock) },
	{ 0x598542b2, __VMLINUX_SYMBOL_STR(_raw_spin_lock_irqsave) },
	{ 0x3db6077b, __VMLINUX_SYMBOL_STR(fb_set_var) },
	{ 0xd85cd67e, __VMLINUX_SYMBOL_STR(__wake_up) },
	{ 0x2dbb1097, __VMLINUX_SYMBOL_STR(ipu_csi_get_sensor_protocol) },
	{ 0x344b7739, __VMLINUX_SYMBOL_STR(prepare_to_wait_event) },
	{ 0xfa5290a7, __VMLINUX_SYMBOL_STR(mipi_csi2_get_status) },
	{ 0x4f68e5c9, __VMLINUX_SYMBOL_STR(do_gettimeofday) },
	{ 0x624fe1, __VMLINUX_SYMBOL_STR(ipu_csi_get_window_size) },
	{ 0x5b14a1d9, __VMLINUX_SYMBOL_STR(mipi_csi2_get_info) },
	{ 0xed439d3, __VMLINUX_SYMBOL_STR(mipi2_csi2_dphy_status) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x8e501ff8, __VMLINUX_SYMBOL_STR(remap_pfn_range) },
	{ 0x9d669763, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0xc58b6609, __VMLINUX_SYMBOL_STR(ipu_enable_csi) },
	{ 0xce71d062, __VMLINUX_SYMBOL_STR(mipi_csi2_pixelclk_disable) },
	{ 0x4be7fb63, __VMLINUX_SYMBOL_STR(up) },
	{ 0x9d28c19c, __VMLINUX_SYMBOL_STR(ipu_select_buffer) },
	{ 0x4a19df0b, __VMLINUX_SYMBOL_STR(v4l2_int_ioctl_0) },
	{ 0x1cfb04fa, __VMLINUX_SYMBOL_STR(finish_wait) },
	{ 0xe958ff9b, __VMLINUX_SYMBOL_STR(mipi2_csi2_get_info) },
	{ 0x7017868f, __VMLINUX_SYMBOL_STR(ipu_channel_free) },
	{ 0xefd6cf06, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr0) },
	{ 0xd73468df, __VMLINUX_SYMBOL_STR(ipu_csi_init_interface) },
	{ 0xb2d48a2e, __VMLINUX_SYMBOL_STR(queue_work_on) },
	{ 0xca54fee, __VMLINUX_SYMBOL_STR(_test_and_set_bit) },
	{ 0x11039834, __VMLINUX_SYMBOL_STR(platform_driver_unregister) },
	{ 0x8e24a96a, __VMLINUX_SYMBOL_STR(of_property_read_variable_u32_array) },
	{ 0x49ebacbd, __VMLINUX_SYMBOL_STR(_clear_bit) },
	{ 0x84ac38df, __VMLINUX_SYMBOL_STR(ipu_link_channels) },
	{ 0xa74fc382, __VMLINUX_SYMBOL_STR(ipu_request_irq) },
	{ 0x6c61ce70, __VMLINUX_SYMBOL_STR(num_registered_fb) },
	{ 0x75a985e2, __VMLINUX_SYMBOL_STR(fb_blank) },
	{ 0xd7453a3e, __VMLINUX_SYMBOL_STR(mipi_csi2_get_datatype) },
	{ 0xe914e41e, __VMLINUX_SYMBOL_STR(strcpy) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=mxc_mipi_csi2_v4";

MODULE_ALIAS("platform:t-v4l2-capture-imx5");
MODULE_ALIAS("platform:t-v4l2-capture-imx6");
MODULE_ALIAS("of:N*T*Cfsl,t-imx6q-v4l2-capture");
MODULE_ALIAS("of:N*T*Cfsl,t-imx6q-v4l2-captureC*");

MODULE_INFO(srcversion, "0F7641E86320C958334F019");
