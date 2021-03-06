#include <linux/module.h> 
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include "vchar_driver.h"

#include <linux/random.h>
#include <asm/uaccess.h>

#define DRIVER_AUTHOR "Nguyen Hai-Son <nhson17@apcs.vn>"
#define DRIVER_DESC   "Character device driver that generates random number on read"

typedef struct vchar_dev {
	unsigned char * control_regs;
	unsigned char * status_regs;
	unsigned char * data_regs;
} vchar_dev_t;

struct _vchar_drv {
	dev_t dev_num;
 	struct class *dev_class;
	struct device *dev;
	vchar_dev_t * vchar_hw;
	struct cdev *vcdev;
} vchar_drv;

/****************************** device specific - START *****************************/
/* ham khoi tao thiet bi */
int vchar_hw_init(vchar_dev_t *hw) {
	char * buf;
	buf = kzalloc(NUM_DEV_REGS * REG_SIZE, GFP_KERNEL);
	if (!buf) {
		return -ENOMEM;
	}
	
	hw->control_regs = buf;
	hw->status_regs = hw->control_regs + NUM_CTRL_REGS;
	hw->data_regs = hw->status_regs + NUM_STS_REGS;

	hw->control_regs[CONTROL_ACCESS_REG] = 0x03;
	hw->status_regs[DEVICE_STATUS_REG] = 0x03;

 	return 0;
}

/* ham giai phong thiet bi */
void vchar_hw_exit(vchar_dev_t *hw) {
	kfree(hw->control_regs);
}
/* ham doc tu cac thanh ghi du lieu cua thiet bi */

/* ham ghi vao cac thanh ghi du lieu cua thiet bi */

/* ham doc tu cac thanh ghi trang thai cua thiet bi */

/* ham ghi vao cac thanh ghi dieu khien cua thiet bi */

/* ham xu ly tin hieu ngat gui tu thiet bi */

/******************************* device specific - END *****************************/

/******************************** OS specific - START *******************************/
/* cac ham entry points */
static unsigned int randomNumber; //Non-negative number

static int vchar_driver_open(struct inode *i, struct file *f) {
	printk("Handle opened event \n");
	return 0;
}

static int vchar_driver_release(struct inode *i, struct file *f) {
	printk("Handle closed event\n");
	return 0;
}

static ssize_t vchar_driver_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
	get_random_bytes(&randomNumber, sizeof(char));
	printk("Handle read event, Random number is (%u)\n", randomNumber);
	if (len < 4)
	{
		printk("Failed\n");
		return -EFAULT;
	}
	if (randomNumber != 0) {
		//copy int value to charater buffer
		memcpy(buffer, (char*)&randomNumber, sizeof(unsigned int));
		return 0;
	}
	else
	{
		*(buffer++) = '0';
		*buffer = '\0';
		return 0;
	}

}

static struct file_operations fops = {
	.owner = THIS_MODULE,
        .open = vchar_driver_open,
        .release = vchar_driver_release,
	.read = vchar_driver_read
};

/* ham khoi tao driver */
static int __init vchar_driver_init(void)
{	
	int ret = 0;
	/* cap phat device number */	
	vchar_drv.dev_num = 0;
	ret = alloc_chrdev_region(&vchar_drv.dev_num, 0, 1, "vchar_device");
	if (ret < 0) {
		printk("failed to register device number dynamically\n");
		goto failed_register_devnum;	
	}
	printk("allocated device number (%d, %d)\n", MAJOR(vchar_drv.dev_num), MINOR(vchar_drv.dev_num));

	/* tao device file */
	vchar_drv.dev_class = class_create(THIS_MODULE, "class_vchar_dev");
	if (vchar_drv.dev_class == NULL) {
		printk("failed to create a device class\n");
		goto failed_create_class;
	}

	vchar_drv.dev = device_create(vchar_drv.dev_class, NULL, vchar_drv.dev_num, NULL, "vchar_dev");
	if (IS_ERR(vchar_drv.dev)) {
		printk("failed to create a device\n");
		goto failed_create_device;
	}

	/* cap phat bo nho cho cac cau truc du lieu cua driver va khoi tao */
	vchar_drv.vchar_hw = kzalloc(sizeof(vchar_dev_t), GFP_KERNEL);
	if (!vchar_drv.vchar_hw) {
		printk("failed to allocate data structure of the driver\n");
		ret = -ENOMEM;
		goto failed_allocate_structure;	
	}

	/* khoi tao thiet bi vat ly */
	ret = vchar_hw_init(vchar_drv.vchar_hw);
	if(ret < 0) {
		printk("failed to initialize a virtual character device\n");
		goto failed_init_hw;	
	}	

	/* dang ky cac entry point voi kernel */
	vchar_drv.vcdev = cdev_alloc();
	if(vchar_drv.vcdev == NULL) {
		printk("failed to allocate cdev structure\n");
		goto failed_allocate_cdev;	
	}	
	cdev_init(vchar_drv.vcdev, &fops);
	ret = cdev_add(vchar_drv.vcdev, vchar_drv.dev_num, 1);
	if (ret < 0) {
		printk("failed to add a char device to the system\n");
		goto failed_allocate_cdev;	
	}
	/* dang ky ham xu ly ngat */

	printk("Initialize vchar driver successfully\n");
	return 0;

failed_allocate_cdev:
	vchar_hw_exit(vchar_drv.vchar_hw);
failed_init_hw:
	kfree(vchar_drv.vchar_hw);
failed_allocate_structure:
	device_destroy(vchar_drv.dev_class, vchar_drv.dev_num);
failed_create_device:
	class_destroy(vchar_drv.dev_class);
failed_create_class:
	unregister_chrdev_region(vchar_drv.dev_num, 1);
failed_register_devnum:
	return ret;
}

/* ham ket thuc driver */
static void __exit vchar_driver_exit(void)
{
	/* huy dang ky xu ly ngat */

	/* huy dang ky entry point voi kernel */
	cdev_del(vchar_drv.vcdev);
	/* giai phong thiet bi vat ly */
	vchar_hw_exit(vchar_drv.vchar_hw);
	/* giai phong bo nho da cap phat cau truc du lieu cua driver */
	kfree(vchar_drv.vchar_hw);
	/* xoa bo device file */
	device_destroy(vchar_drv.dev_class, vchar_drv.dev_num);
	class_destroy(vchar_drv.dev_class);

	/* giai phong device number */
	unregister_chrdev_region(vchar_drv.dev_num, 1);

	printk("Exit vchar driver\n");
}
/********************************* OS specific - END ********************************/

module_init(vchar_driver_init);
module_exit(vchar_driver_exit);

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR(DRIVER_AUTHOR); 
MODULE_DESCRIPTION(DRIVER_DESC); 
MODULE_SUPPORTED_DEVICE("dummydevice"); 
