/*
 *    DTV tuner linux driver
 *
 * Usage:    DTV tuner driver by i2c for linux
 *
 *
 * This software program is licensed subject to the GNU General Public License
 * (GPL).Version 2,June 1991, available at http://www.fsf.org/copyleft/gpl.html
 *
 */

#ifndef _DTVD_TUNER_DRIVER_H_
#define _DTVD_TUNER_DRIVER_H_

#define D_DTVD_TUNER_CHAR_MAJOR         143
#define D_DTVD_TUNER_CHAR_MINOR         0

#define D_DTVD_TUNER_DRV_NAME           "dtvtuner"

int dtvd_tuner_main( void *);
int dtvd_tuner_open( struct inode *, struct file  * );

ssize_t dtvd_tuner_read( struct file *file, char __user *buffer, size_t count, loff_t *f_pos );

int dtvd_tuner_ioctl( struct inode *, struct file *file, unsigned int, unsigned long );
int dtvd_tuner_close( struct inode *, struct file  * );
int dtvd_tuner_init_module(void);

extern const struct file_operations tdtvd_tuner_fops;   

#endif 

