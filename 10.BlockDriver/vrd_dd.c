#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/fs.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/vmalloc.h>
#include <linux/hdreg.h>
#include <linux/blkpg.h>
#include <linux/blkdev.h>
#include <linux/uaccess.h>

#define VRD_DEV_NAME    "vrd"
#define VRD_DEV_MAJOR   "300"
#define VRD_MAX_DEVICE  1

#define VRD_SECTOR_SIZE 512
#define VRD_SIZE    (4*1024*1024)
#define VRD_SECTOR_TOTAL    (VRD_SIZE/VRD_SECTOR_SIZE)

typedef struct {
    unsigned char   *data;
    struct request_queue    *queue;
    struct gendisk  *gd;
} vrd_device;
char    *vdisk;
vrd_device device;

int vrd_init(void);
void vrd_exit(void);
static unsigned int vrd_make_request(struct request_queue*, struct bio*);
int vrd_open(struct block_device*, fmode_t);
void vrd_release(struct gendisk*, fmode_t);
int vrd_ioctl(struct block_device*, fmode_t, unsigned, unsigned long);

static struct block_device_operations vrd_fops = {
    .owner  = THIS_MODULE,
    .open    = vrd_open,
    .release    = vrd_release,
    .ioctl  = vrd_ioctl  
};

static unsigned int vrd_make_request(struct request_queue *q, struct bio *bio) {
    vrd_device *pdevice;
    char *pVHDDData;
    char *pBuffer;
    struct bio_vec ibvec;
    struct bvec_iter iter;

    if( ((bio->bi_iter.bi_sector* VRD_SECTOR_SIZE) + bio->bi_iter.bi_size) > VRD_SIZE ) {
        bio_endio(bio);
        return -1;
    }

    pdevice = (vrd_device*)bio->bi_disk->private_data;
    pVHDDData = pdevice->data + (bio->bi_iter.bi_sector* VRD_SECTOR_SIZE);
    bio_for_each_segment(ibvec, bio, iter) {
        pBuffer = kmap(ibvec.bv_page) + ibvec.bv_offset;
        switch(bio_data_dir(bio)) {
            case READ :
                memcpy(pBuffer, pVHDDData, ibvec.bv_len);
                break;
            case WRITE :
                memcpy(pVHDDData, pBuffer, ibvec.bv_len);
                break;
            default :
                bio_endio(bio);
                return -1;
        }
        kunmap(ibvec.bv_page);
        pVHDDData += ibvec.bv_len;
    }
    bio_endio(bio);
    return 0;
}

int vrd_open(struct block_device *bdev, fmode_t mode_t) {
    printk(KERN_INFO "device open\n");
    return 0;
}

void vrd_release(struct gendisk *gd, fmode_t mode_t) {
    printk(KERN_INFO "device release\n");
}

int vrd_ioctl(struct block_device *bdev, fmode_t mode_t, unsigned cmd, unsigned long error) {
    return -ENOTTY;
}

int vrd_init(void) {
    vdisk = vmalloc(VRD_SIZE);

    int state = register_blkdev(VRD_DEV_MAJOR, VRD_DEV_NAME);
    if(state < 0) {
        printk(KERN_INFO "driver build failed..\n");
        return -1;
    }

    device.data = vdisk;
    device.gd = alloc_disk(1);
    device.queue = blk_alloc_queue(GFP_KERNEL);
    blk_queue_make_request(device.queue, &vrd_make_request);

    device.gd->major    = VRD_DEV_MAJOR;
    device.gd->first_minor  = 0;
    device.gd->fops = &vrd_fops;
    device.gd->queue    = device.queue;
    device.gd->private_data = &device;
    snprintf(device.gd->disk_name, 10, "vrd");
    set_capacity(device.gd, VRD_SECTOR_TOTAL);

    add_disk(device.gd);

    printk(KERN_INFO "blk_device_driver init");
    return 0;
}

void vrd_exit(void) {
    del_gendisk(device.gd);
    put_disk(device.gd);
    unregister_blkdev(VRD_DEV_MAJOR, VRD_DEV_NAME);
    vfree(vdisk);
    printk(KERN_INFO "blk_device_driver exit\n");
}

module_init(vrd_init);
module_exit(vrd_exit);
MODULE_LICENSE("GPL");
