#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/list.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>

dev_t dev_num;
struct cdev chrdev;
struct class *chrcls;
struct packet_type pt;
struct net_device *nd;
int eth_packet_capture(struct sk_buff *skb, struct net_device *nd, struct packet_type *pt, struct net_device *nd2) {
  unsigned char *ptr = skb_mac_header(skb);
  printk(KERN_ALERT "dst: %02x %02x %02x %02x %02x %02x\n", ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);
  printk(KERN_ALERT "src: %02x %02x %02x %02x %02x %02x\n", ptr[6], ptr[7], ptr[8], ptr[9], ptr[10], ptr[11]);
  return 0;
}

ssize_t dev_show(struct device *dev, struct device_attribute *attr, char *buf) {
  
  return 0;
}
ssize_t dev_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count) {

  return 0;
}
struct file_operations fops = {
  .owner = THIS_MODULE,
};
struct device_attribute dattr = {
  .attr = {
	.name = "kmodtut",
	.mode = S_IWUSR | S_IRUSR,	
  },
  .show = dev_show,
  .store = dev_store
};
struct device *dev;

static int kmodtut_init(void) {
  nd = dev_get_by_name(&init_net, "wlp2s0");
  pt.type = htons(ETH_P_ALL);
  pt.func = eth_packet_capture;
  pt.dev = nd;
  dev_add_pack(&pt);
  alloc_chrdev_region(&dev_num, 0, 1, "kmodtut");
  register_chrdev_region(dev_num, 1, "kmodtut");
  cdev_init(&chrdev, &fops);
  cdev_add(&chrdev, dev_num, 1);
  chrcls = class_create(THIS_MODULE, "kmodtut");
  dev = device_create(chrcls, NULL, dev_num, NULL, "kmodtut");
  device_create_file(dev, &dattr);
  
  return 0;
}

static void kmodtut_exit(void) {
  device_remove_file(dev, &dattr);
  device_destroy(chrcls, dev_num);
  class_destroy(chrcls);
  cdev_del(&chrdev);
  unregister_chrdev_region(dev_num, 1);
  dev_remove_pack(&pt);
}

module_init(kmodtut_init);
module_exit(kmodtut_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("kmj");
