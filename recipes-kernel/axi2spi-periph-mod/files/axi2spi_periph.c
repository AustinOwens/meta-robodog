#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/uaccess.h>

// SPI Status Register (SPISR) shifts
#define SPISR_RX_EMPTY_SHIFT 0
#define SPISR_RX_FULL_SHIFT 1
#define SPISR_TX_EMPTY_SHIFT 2
#define SPISR_TX_FULL_SHIFT 3
#define SPISR_MODF_SHIFT 4
#define SPISR_SLAVE_MODE_SELECT_SHIFT 5
#define SPISR_CPOL_CPHA_ERROR_SHIFT 6
#define SPISR_SLAVE_MODE_ERROR_SHIFT 7
#define SPISR_MSB_ERROR_SHIFT 8
#define SPISR_LOOP_BACK_ERROR_SHIFT 9
#define SPISR_COMMAND_ERROR_SHIFT 10

// SPI Control Register (SPICR) shifts
#define SPICR_LOOP_SHIFT 0
#define SPICR_SPE_SHIFT 1
#define SPICR_MASTER_SHIFT 2
#define SPICR_CPOL_SHIFT 3
#define SPICR_CPHA_SHIFT 4
#define SPICR_TX_FIFO_RESET_SHIFT 5
#define SPICR_RX_FIFO_RESET_SHIFT 6
#define SPICR_MANUAL_SS_ASSERTION_ENABLE_SHIFT 7
#define SPICR_MASTER_TRANSACTION_INHIBIT_SHIFT 8
#define SPICR_LSB_FIRST_SHIFT 9

// IP Interrupt Enable Register (IPIER) shifts
#define IPIER_MODF_SHIFT 0
#define IPIER_SLAVE_MODF_SHIFT 1
#define IPIER_DTR_EMPTY_SHIFT 2
#define IPIER_DTR_UNDERRUN_SHIFT 3
#define IPIER_DRR_FULL_SHIFT 4
#define IPIER_DRR_OVERRUNN_SHIFT 5
#define IPIER_TX_FIFO_HALF_EMPTY_SHIFT 6
#define IPIER_SLAVE_MODE_SELECT_SHIFT 7
#define IPIER_DRR_NOT_EMPTY_SHIFT 8
#define IPIER_CPOL_CPHA_ERROR_SHIFT 9
#define IPIER_SLAVE_MODE_ERROR_SHIFT 10
#define IPIER_MSB_ERROR_SHIFT 11
#define IPIER_LOOP_BACK_ERROR_SHIFT 12
#define IPIER_COMMAND_ERROR_SHIFT 13

// Device Global Interrupt Enable Register (DGIER) shifts
#define DGIER_GIE_SHIFT 31

static int axi2spi_periph_open(struct inode *inode, struct file *file);
static int axi2spi_periph_release(struct inode *inode, struct file *file);
static long axi2spi_periph_ioctl(struct file *file, unsigned int cmd,
                                 unsigned long arg);
static ssize_t axi2spi_periph_read(struct file *file, char __user *buf,
                                   size_t count, loff_t *offset);
static ssize_t axi2spi_periph_write(struct file *file, const char __user *buf,
                                    size_t count, loff_t *offset);
void axi2spi_set_spidtr_val(uint32_t value);
uint32_t axi2spi_get_spidtr_val(void);
static int axi2spi_periph_uevent(struct device *dev,
                                 struct kobj_uevent_env *env);

static const struct file_operations axi2spi_periph_fops = {
    .owner = THIS_MODULE,
    .open = axi2spi_periph_open,
    .release = axi2spi_periph_release,
    .unlocked_ioctl = axi2spi_periph_ioctl,
    .read = axi2spi_periph_read,
    .write = axi2spi_periph_write,
};

struct axi2spi_periph_regs {
  uint8_t res1[28];  // Offset 0x00
  uint8_t dgier[4];  // Offset 0x1C
  uint8_t ipisr[4];  // Offset 0x20
  uint8_t res2[4];   // Offset 0x24
  uint8_t ipier[4];  // Offset 0x28
  uint8_t res3[20];  // Offset 0x2C
  uint8_t srr[4];    // Offset 0x40
  uint8_t res4[28];  // Offset 0x44
  uint8_t spicr[4];  // Offset 0x60
  uint8_t spisr[4];  // Offset 0x64
  uint8_t spidtr[4]; // Offset 0x68
  uint8_t spidrr[4]; // Offset 0x6C
  uint8_t spissr[4]; // Offset 0x70
  uint8_t txfifo[4]; // Offset 0x74
  uint8_t rxfifo[4]; // Offset 0x78
};

struct axi2spi_periph_dev {
  struct platform_device *pdev;
  dev_t dev_num;
  struct class *class;
  struct device *device;
  struct cdev cdev;
  struct axi2spi_periph_regs __iomem *regs;
};

uint32_t spidtr_val = 0;

void axi2spi_set_spidtr_val(uint32_t value) { spidtr_val = value; }
EXPORT_SYMBOL(axi2spi_set_spidtr_val);

uint32_t axi2spi_get_spidtr_val(void) { return spidtr_val; }
EXPORT_SYMBOL(axi2spi_get_spidtr_val);

static int axi2spi_periph_uevent(struct device *dev,
                                 struct kobj_uevent_env *env) {
  add_uevent_var(env, "DEVMODE=%#o", 0666);
  return 0;
}

static int axi2spi_periph_open(struct inode *inode, struct file *file) {
  struct axi2spi_periph_dev *dev;

  printk("axi2spi_periph: Device open\n");

  dev = container_of(inode->i_cdev, struct axi2spi_periph_dev, cdev);
  file->private_data = dev;

  return 0;
}

static int axi2spi_periph_release(struct inode *inode, struct file *file) {
  printk("axi2spi_periph: Device close\n");
  return 0;
}

static long axi2spi_periph_ioctl(struct file *file, unsigned int cmd,
                                 unsigned long arg) {
  printk("axi2spi_periph: Device ioctl\n");
  return 0;
}

static ssize_t axi2spi_periph_read(struct file *file, char __user *buf,
                                   size_t count, loff_t *offset) {
  // struct axi2spi_periph_dev *dev = file->private_data;
  char data[14];
  size_t datalen;

  // Convert spidtr_val to string
  datalen = snprintf(data, sizeof(data), "0x%x\n", spidtr_val);

  // Check if we've already returned the message to user space
  if (*offset >= datalen) {
    return 0; // EOF
  }

  printk("axi2spi_periph: Device read\n");

  // Adjust the count based on the remaining bytes to read
  if (count > datalen - *offset) {
    count = datalen - *offset;
  }

  if (copy_to_user(buf, data, count)) {
    return -EFAULT;
  }

  // Update the offset by the number of bytes read
  *offset += count;

  return count;
}

static ssize_t axi2spi_periph_write(struct file *file, const char __user *buf,
                                    size_t count, loff_t *ppos) {
  struct axi2spi_periph_dev *dev = file->private_data;
  //uint32_t spicr_val;
  char kbuf[12];
  uint32_t usr_val;
  int ret;

  if (count >= sizeof(kbuf))
    return -EINVAL;

  if (copy_from_user(kbuf, buf, count))
    return -EFAULT;

  // Null terminate the string
  kbuf[count] = '\0';

  // Convert string to integer
  ret = kstrtouint(kbuf, 0, &usr_val);
  if (ret)
    return ret;

  // Update spidtr value for spidtr_irq_handler
  printk(KERN_INFO "axi2spi_periph: Previous spidtr_val: 0x%x\n", spidtr_val);
  spidtr_val = usr_val;
  printk(KERN_INFO "axi2spi_periph: Current spidtr_val: 0x%x\n", spidtr_val);

  // Flush DTR
  //spicr_val = ioread32(&dev->regs->spicr);
  //spicr_val |= (1U << SPICR_TX_FIFO_RESET_SHIFT);
  //iowrite32(spicr_val, &dev->regs->spicr);
  iowrite32(spidtr_val, &dev->regs->spidtr);
  //iowrite32((1U << IPIER_DTR_EMPTY_SHIFT), &dev->regs->ipisr);

  //printk(KERN_INFO "axi2spi_periph: YOOO: 0x%x\n", spicr_val);

  return count;
}

static irqreturn_t spidtr_irq_handler(int irq, void *dev_id) {
  struct axi2spi_periph_dev *dev = (struct axi2spi_periph_dev *)dev_id;
  uint32_t ipisr_val = ioread32(&dev->regs->ipisr);
  uint32_t spisr_val = ioread32(&dev->regs->spisr);
  //uint32_t rx_fifo_len;
  //uint32_t tx_fifo_len;
  
  //printk(KERN_INFO "axi2spi_periph (IRQ): BEFORE: ipisr_val = 0x%x | spisr_val = 0x%x\n", ipisr_val, spisr_val);

  // If DRR FIFO is empty and DRR_NOT_EMPTY IRQ flag is high, clear DRR_NOT_EMPTY IRQ flag
  //if ((spisr_val & (1U << SPISR_RX_EMPTY_SHIFT)) && (ipisr_val & (1U << IPIER_DRR_NOT_EMPTY_SHIFT))) {
  //    // Clear DRR_NOT_EMPTY IRQ flag
  //    iowrite32((1U << IPIER_DRR_NOT_EMPTY_SHIFT), &dev->regs->ipisr);

  //    // Debug print
  //    printk(KERN_INFO "axi2spi_periph (IRQ): RX_EMPTY and DRR_NOT_EMPTY\n");
  //}

  // Push DRR to DTR FIFO
  uint32_t spidrr_val = ioread32(&dev->regs->spidrr);
  iowrite32(spidrr_val, &dev->regs->spidtr);

  // Debug print
  //tx_fifo_len = ioread32(&dev->regs->txfifo) + 1U;
  //rx_fifo_len = ioread32(&dev->regs->rxfifo) + 1U;
  //printk(KERN_INFO "axi2spi_periph (IRQ): DRR_NOT_EMPTY, filling DTR with 0x%x. rx_fifo_len = %u | tx_fifo_len = %u\n",
  //        spidrr_val, rx_fifo_len, tx_fifo_len);

  iowrite32((1U << IPIER_DRR_NOT_EMPTY_SHIFT), &dev->regs->ipisr);
  printk(KERN_INFO "axi2spi_periph (IRQ): spidrr_val = 0x%x\n", spidrr_val);
  printk(KERN_INFO "axi2spi_periph (IRQ): ipisr_val  = 0x%x\n", ipisr_val);
  printk(KERN_INFO "axi2spi_periph (IRQ): spisr_val  = 0x%x\n\n", spisr_val);

  //ipisr_val = ioread32(&dev->regs->ipisr);
  //spisr_val = ioread32(&dev->regs->spisr);

  //printk(KERN_INFO "axi2spi_periph (IRQ): AFTER: ipisr_val = 0x%x | spisr_val = 0x%x\n", ipisr_val, spisr_val);

  //// If DTR is empty, load DTR with spidtr_val
  //else if (ipisr_val & (1U << IPIER_DTR_EMPTY_SHIFT)) {
  //    printk("5\n");

  //    // Initialize first 32 bits of DTR with spidtr_val
  //    iowrite32(spidtr_val, &dev->regs->spidtr);

  //    // Clear DTR_EMPTY IRQ flag
  //    iowrite32((1U << IPIER_DTR_EMPTY_SHIFT), &dev->regs->ipisr);

  //    // Debug print
  //    printk(KERN_INFO "axi2spi_periph (IRQ): DTR_EMPTY, filling DTR with 0x%x\n", spidtr_val);
  //    spisr_val = ioread32(&dev->regs->spisr);
  //    printk(KERN_INFO "axi2spi_periph (IRQ): DTR_EMPTY, spisr = 0x%x\n", spisr_val);

  //}

  return IRQ_HANDLED;
}

static int axi2spi_periph_probe(struct platform_device *pdev) {
  struct axi2spi_periph_dev *dev;
  struct resource *res;
  int irq_num;
  int err = 0;
  uint32_t spicr_val;
  uint32_t ipier_val;
  uint32_t dgier_val;

  printk(KERN_INFO "axi2spi_periph: Probe called\n");

  // ALLOCATE DEVICE STRUCTURE
  dev = devm_kzalloc(&pdev->dev, sizeof(*dev), GFP_KERNEL);
  if (!dev)
    return -ENOMEM;

  dev->pdev = pdev;
  platform_set_drvdata(pdev, dev);

  res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
  printk(KERN_INFO
         "axi2spi_periph: Got physical address: 0x%pa, size (bytes): %llu\n",
         &res->start, (unsigned long long)resource_size(res));

  dev->regs = devm_ioremap_resource(&pdev->dev, res);
  if (IS_ERR(dev->regs)) {
    printk(KERN_ERR "axi2spi_periph: Failed to remap IO memory, error: %ld\n",
           PTR_ERR(dev->regs));
    return PTR_ERR(dev->regs);
  }

  printk(KERN_INFO
         "axi2spi_periph: Remapped IO memory to virtual address: 0x%px\n",
         dev->regs);

  // REGISTER IRQ

  // Get IRQ number; 0 for the first IRQ in device tree
  irq_num = platform_get_irq(pdev, 0);
  if (irq_num < 0) {
    return -ENODEV;
  }
  printk(KERN_INFO "axi2spi_periph: Registering translated IRQ: 0x%x\n",
         irq_num);

  if (request_irq(irq_num, &spidtr_irq_handler, 0, "spidtr_irq_handler", dev)) {
    printk(KERN_ERR "Failed to register IRQ\n");
    return -EIO;
  }

  // SET SOFTWARE RESET REGISTER (SRR)
  printk(KERN_INFO "axi2spi_periph: Set SRR");
  iowrite32(0x0000000A, &dev->regs->srr);

  // CONFIGURE SPI CONTROL REGISTER (SPICR)
  spicr_val = ioread32(&dev->regs->spicr);
  printk(KERN_INFO "axi2spi_periph: Previous SPICR: 0x%x\n", spicr_val);

  spicr_val |= (1U << SPICR_SPE_SHIFT);
  iowrite32(spicr_val, &dev->regs->spicr);
  printk(KERN_INFO "axi2spi_periph: Current SPICR: 0x%x\n",
         ioread32(&dev->regs->spicr));

  // CONFIGURE IP INTERRUPT ENABLE REGISTER (IPIER)
  ipier_val = ioread32(&dev->regs->ipier);
  printk(KERN_INFO "axi2spi_periph: Previous IPIER: 0x%x\n", ipier_val);

  //ipier_val |= (1U << IPIER_DTR_EMPTY_SHIFT);
  ipier_val |= (1U << IPIER_DRR_NOT_EMPTY_SHIFT);
  iowrite32(ipier_val, &dev->regs->ipier);
  printk(KERN_INFO "axi2spi_periph: Current IPIER: 0x%x\n",
         ioread32(&dev->regs->ipier));

  // TURN ON DEVICE GLOBAL INTERRUPT ENABLE REGISTER (DGIER)
  dgier_val = ioread32(&dev->regs->dgier);
  printk(KERN_INFO "axi2spi_periph: Previous DGIER: 0x%x\n", dgier_val);

  dgier_val |= (1U << DGIER_GIE_SHIFT);
  iowrite32(dgier_val, &dev->regs->dgier);
  printk(KERN_INFO "axi2spi_periph: Current DGIER: 0x%x\n",
         ioread32(&dev->regs->dgier));

  // INITIALIZE FIRST 32 BITS OF DTR WITH SPIDTR_VAL (SPIDTR)
  //printk(KERN_INFO "axi2spi_periph: Previous SPIDTR: 0x%x\n",
  //       ioread32(&dev->regs->spidtr));
  //iowrite32(spidtr_val, &dev->regs->spidtr);
  //printk(KERN_INFO "axi2spi_periph: Current SPIDTR: 0x%x\n",
  //       ioread32(&dev->regs->spidtr));

  // INITIALIZE CHAR DEVICE
  err = alloc_chrdev_region(&dev->dev_num, 0, 1, "axi2spi_periph");
  if (err < 0)
    return err;

  cdev_init(&dev->cdev, &axi2spi_periph_fops);
  dev->cdev.owner = THIS_MODULE;

  err = cdev_add(&dev->cdev, dev->dev_num, 1);
  if (err < 0)
    return err;

  dev->class = class_create(THIS_MODULE, "axi2spi_periph");
  if (IS_ERR(dev->class)) {
    cdev_del(&dev->cdev);
    unregister_chrdev_region(dev->dev_num, 1);
    return PTR_ERR(dev->class);
  }
  dev->class->dev_uevent = axi2spi_periph_uevent;

  dev->device =
      device_create(dev->class, NULL, dev->dev_num, NULL, "axi2spi_periph");
  if (IS_ERR(dev->device)) {
    class_destroy(dev->class);
    cdev_del(&dev->cdev);
    unregister_chrdev_region(dev->dev_num, 1);
    return PTR_ERR(dev->device);
  }

  printk(KERN_INFO
         "axi2spi_periph: Character device set up with dev_num: %d, %d\n",
         MAJOR(dev->dev_num), MINOR(dev->dev_num));

  return 0;
}

static int axi2spi_periph_remove(struct platform_device *pdev) {
  struct axi2spi_periph_dev *dev = platform_get_drvdata(pdev);

  // Set Software Reset Register (SRR)
  printk(KERN_INFO "axi2spi_periph: Set SRR");
  iowrite32(0x0000000A, &dev->regs->srr);

  // Unregister device
  printk(KERN_INFO "axi2spi_periph: Unregister device...");

  device_destroy(dev->class, dev->dev_num);
  class_destroy(dev->class);
  cdev_del(&dev->cdev);
  unregister_chrdev_region(dev->dev_num, 1);
  free_irq(platform_get_irq(pdev, 0), dev);

  printk(KERN_INFO "axi2spi_periph: Device removed\n");

  return 0;
}

static const struct of_device_id axi2spi_periph_dt_ids[] = {
    {
        .compatible = "morphtech,axi2spi_periph",
    },
    {/* sentinel */}};
MODULE_DEVICE_TABLE(of, axi2spi_periph_dt_ids);

static struct platform_driver axi2spi_periph_driver = {
    .probe = axi2spi_periph_probe,
    .remove = axi2spi_periph_remove,
    .driver =
        {
            .name = "axi2spi_periph",
            .of_match_table = axi2spi_periph_dt_ids,
        },
};

module_platform_driver(axi2spi_periph_driver);

MODULE_AUTHOR("Austin T. Owens");
MODULE_DESCRIPTION("Linux driver for the AXI Quad SPI v3.2 Xilinx IP core");
MODULE_LICENSE("GPL v2");
