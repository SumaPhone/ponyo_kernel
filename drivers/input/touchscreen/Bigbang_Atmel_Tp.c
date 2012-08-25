/* drivers/input/touchscreen/qt602240.c
 *
 * Quantum TSP driver.
 *
 * Copyright (C) 2009 Samsung Electronics Co. Ltd.
 *
 */
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/earlysuspend.h>
#include <linux/hrtimer.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <mach/gpio.h>
#include <mach/vreg.h>
#include <linux/device.h>
#include <linux/vrpanel.h>
#include <linux/slab.h>
#include <linux/Atmel.h>
//#include <mach/gpio-v1.h>

int board_hw_revision = 2; // minhyo0614

#define ENABLE_IRQ 1
#define TRACES 0

#define TSP_SDA 124
#define TSP_SCL 123
#define TSP_INT 112

/* for i2c */
#define I2C_M_WR 0
#define I2C_MAX_SEND_LENGTH     300
#define QT602240_I2C_ADDR 	0x4A

//#define IRQ_TOUCH_INT IRQ_EINT(20)

#define DEBUG printk("[QT] %s/%d\n",__func__,__LINE__)

/* CHJ Define start  */
#define DEBUG_CHJ 0
#define READ_MESSAGE_LENGTH 6

#define QT602240_DEBUG
#if TRACES
#define REPORT( touch, width, x, y) \
{	if(y <= 800){\
		input_report_abs(ts_global->input_dev, ABS_MT_TOUCH_MAJOR, touch ); \
		input_report_abs(ts_global->input_dev, ABS_MT_WIDTH_MAJOR, width ); \
		input_report_abs(ts_global->input_dev, ABS_MT_POSITION_X, x); \
		input_report_abs(ts_global->input_dev, ABS_MT_POSITION_Y, y); \
		printk("x = %d, y = %d, w = %d, z = %d\n", x, y, width, touch);\
		input_mt_sync(ts_global->input_dev); \
	}else if(touch !=0){\
		VrpCallback((TouchSampleValidFlag|TouchSampleDownFlag), x, y);	\
	}else{\
		VrpCallback(TouchSampleValidFlag, 0, 0);	\
	}\
}
#else
#define REPORT( touch, width, x, y) \
{	if(y <= 800){\
		input_report_abs(ts_global->input_dev, ABS_MT_TOUCH_MAJOR, touch ); \
		input_report_abs(ts_global->input_dev, ABS_MT_WIDTH_MAJOR, width ); \
		input_report_abs(ts_global->input_dev, ABS_MT_POSITION_X, x); \
		input_report_abs(ts_global->input_dev, ABS_MT_POSITION_Y, y); \
		input_mt_sync(ts_global->input_dev); \
	}else if(touch !=0){\
		VrpCallback((TouchSampleValidFlag|TouchSampleDownFlag), x, y);	\
	}else{\
		VrpCallback(TouchSampleValidFlag, 0, 0);	\
	}\
}

#endif
/* CHJ Define end */

// print message
#if defined(QT602240_DEBUG) || defined(DEBUG)
#define DEBUG_MSG(p, x...)			printk("[QT602240]:[%s] ", __func__); printk(p, ## x);		
#else
#define DEBUG_MSG(p, x...)
#endif

#define PRINT_MSG(p, x...)		{ printk("[QT602240]:[%s] ", __func__); printk(p, ## x); }

#ifdef QT602240_DEBUG
#define ENTER_FUNC	{ printk("[TSP] +%s\n", __func__); }
#define LEAVE_FUNC	{ printk("[TSP] -%s\n", __func__); }
#else
#define ENTER_FUNC
#define LEAVE_FUNC
#endif


#define MAX_X  480
#define MAX_Y  800 

static struct workqueue_struct *qt602240_wq;
struct qt602240_ts_data *ts_global;

#ifdef CONFIG_HAS_EARLYSUSPEND
static void qt602240_ts_early_suspend(struct early_suspend *h);
static void qt602240_ts_late_resume(struct early_suspend *h);
#endif
/* \brief Defines CHANGE line active mode. */
#define CHANGELINE_ASSERTED 0

/* This sets the I2C frequency to 400kHz (it's a feature in I2C driver that the
   actual value needs to be double that). */
#define I2C_SPEED                   800000u

#define CONNECT_OK                  1u
#define CONNECT_ERROR               2u

#define READ_MEM_OK                 1u
#define READ_MEM_FAILED             2u

#define MESSAGE_READ_OK             1u
#define MESSAGE_READ_FAILED         2u

#define WRITE_MEM_OK                1u
#define WRITE_MEM_FAILED            2u

#define CFG_WRITE_OK                1u
#define CFG_WRITE_FAILED            2u

#define I2C_INIT_OK                 1u
#define I2C_INIT_FAIL               2u

#define CRC_CALCULATION_OK          1u
#define CRC_CALCULATION_FAILED      2u

#define ID_MAPPING_OK               1u
#define ID_MAPPING_FAILED           2u

#define ID_DATA_OK                  1u
#define ID_DATA_NOT_AVAILABLE       2u

/*! \brief Returned by get_object_address() if object is not found. */
#define OBJECT_NOT_FOUND   0u

/*! Address where object table starts at touch IC memory map. */
#define OBJECT_TABLE_START_ADDRESS      7U

/*! Size of one object table element in touch IC memory map. */
#define OBJECT_TABLE_ELEMENT_SIZE       6U

/*! Offset to RESET register from the beginning of command processor. */
#define RESET_OFFSET                        0u

/*! Offset to BACKUP register from the beginning of command processor. */
#define BACKUP_OFFSET           1u

/*! Offset to CALIBRATE register from the beginning of command processor. */
#define CALIBRATE_OFFSET        2u

/*! Offset to REPORTALL register from the beginning of command processor. */
#define REPORTATLL_OFFSET       3u

/*! Offset to DEBUG_CTRL register from the beginning of command processor. */
#define DEBUG_CTRL_OFFSET       4u

/*! Offset to DIAGNOSTIC register from the beginning of command processor. */
#define DIAGNOSTIC_OFFSET       5u


enum driver_setup_t {DRIVER_SETUP_OK, DRIVER_SETUP_INCOMPLETE};



#define OBJECT_LIST__VERSION_NUMBER     0x10

#define RESERVED_T0                               0u
#define RESERVED_T1                               1u
#define DEBUG_DELTAS_T2                           2u
#define DEBUG_REFERENCES_T3                       3u
#define DEBUG_SIGNALS_T4                          4u
#define GEN_MESSAGEPROCESSOR_T5                   5u
#define GEN_COMMANDPROCESSOR_T6                   6u
#define GEN_POWERCONFIG_T7                        7u
#define GEN_ACQUISITIONCONFIG_T8                  8u
#define TOUCH_MULTITOUCHSCREEN_T9                 9u
#define TOUCH_SINGLETOUCHSCREEN_T10               10u
#define TOUCH_XSLIDER_T11                         11u
#define TOUCH_YSLIDER_T12                         12u
#define TOUCH_XWHEEL_T13                          13u
#define TOUCH_YWHEEL_T14                          14u
#define TOUCH_KEYARRAY_T15                        15u
#define PROCG_SIGNALFILTER_T16                    16u
#define PROCI_LINEARIZATIONTABLE_T17              17u
#define SPT_COMCONFIG_T18                         18u
#define SPT_GPIOPWM_T19                           19u
#define PROCI_GRIPFACESUPPRESSION_T20             20u
#define RESERVED_T21                              21u
#define PROCG_NOISESUPPRESSION_T22                22u
#define TOUCH_PROXIMITY_T23                           23u
#define PROCI_ONETOUCHGESTUREPROCESSOR_T24        24u
#define SPT_SELFTEST_T25                          25u
#define DEBUG_CTERANGE_T26                        26u
#define PROCI_TWOTOUCHGESTUREPROCESSOR_T27        27u
#define SPT_CTECONFIG_T28                         28u
#define SPT_GPI_T29                               29u
#define SPT_GATE_T30                              30u
#define TOUCH_KEYSET_T31                          31u
#define TOUCH_XSLIDERSET_T32                      32u
#define RESERVED_T33                              33u
#define GEN_MESSAGEBLOCK_T34                      34u
#define SPT_GENERICDATA_T35                       35u
#define RESERVED_T36                              36u
#define DEBUG_DIAGNOSTIC_T37                      37u
#define SPT_USERDATA_T38                          38u

#define Enable_global_interrupt()        
#define Disable_global_interrupt()
/* -------------------- End of definition block --------------------------*/




/* -------------------- Type define block --------------------------------*/

typedef struct 
{
	uint8_t data[8];
}userdata_t38_config_t;


typedef struct
{
	uint8_t reset;       /*!< Force chip reset             */
	uint8_t backupnv;    /*!< Force backup to eeprom/flash */
	uint8_t calibrate;   /*!< Force recalibration          */
	uint8_t reportall;   /*!< Force all objects to report  */
	uint8_t debugctrl;   /*!< Turn on output of debug data */
	uint8_t diagnostic;  /*!< Controls the diagnostic object */
} gen_commandprocessor_t6_config_t;

typedef struct
{
	uint8_t idleacqint;    /*!< Idle power mode sleep length in ms           */
	uint8_t actvacqint;    /*!< Active power mode sleep length in ms         */
	uint8_t actv2idleto;   /*!< Active to idle power mode delay length in ms */
} gen_powerconfig_t7_config_t;

typedef struct
{
	uint8_t ctrl;    /*!< Idle power mode sleep length in ms           */
	uint8_t command;    /*!< Active power mode sleep length in ms         */
} spt_commsconfig_t18_config_t;

typedef struct
{
	uint8_t chrgtime;          /*!< Burst charge time                      */
	uint8_t Reserved;  
	uint8_t tchdrift;          /*!< Touch drift compensation period        */
	uint8_t driftst;           /*!< Drift suspend time                     */
	uint8_t tchautocal;        /*!< Touch automatic calibration delay in ms*/
	uint8_t sync;              /*!< Measurement synchronisation control    */
	uint8_t atchcalst;
	uint8_t atchcalsthr;      
	uint8_t atchfrccalthr;		/* for firmware ver2.0 */
	uint8_t atchfrccalratio;  /* for firmware ver2.0 */    
} gen_acquisitionconfig_t8_config_t;

typedef struct
{
	uint8_t ctrl;            /*!< Main configuration field           */
	uint8_t xorigin;         /*!< Object x start position on matrix  */
	uint8_t yorigin;         /*!< Object y start position on matrix  */
	uint8_t xsize;           /*!< Object x size (i.e. width)         */
	uint8_t ysize;           /*!< Object y size (i.e. height)        */
	uint8_t akscfg;          /*!< Adjacent key suppression config     */
	uint8_t blen;            /*!< Burst length for all object channels*/
	uint8_t tchthr;          /*!< Threshold for all object channels   */
	uint8_t tchdi;           /*!< Detect integration config           */
	uint8_t orientate; 
	uint8_t mrgtimeout; 
	uint8_t movhysti;   /*!< Movement hysteresis setting used after touchdown */
	uint8_t movhystn;   /*!< Movement hysteresis setting used once dragging   */
	uint8_t movfilter;  /*!< Position filter setting controlling the rate of  */
	uint8_t numtouch;   /*   to track */
	uint8_t mrghyst;    /*	 *   to stop oscillation */
	uint8_t mrgthr;     /*!< considered one touch */
	uint8_t tchamphyst;          /*!< TBD */
	uint16_t xres;
	uint16_t yres;
	uint8_t xloclip;
	uint8_t xhiclip;
	uint8_t yloclip;
	uint8_t yhiclip;
	uint8_t xedgectrl;
	uint8_t xedgedist;
	uint8_t yedgectrl;
	uint8_t yedgedist;
	uint8_t jumplimit;
	uint8_t tchhyst;		/* for firmware ver2.0 */
} touch_multitouchscreen_t9_config_t;

typedef struct
{
	/* Key Array Configuration */
	uint8_t ctrl;               /*!< Main configuration field           */

	/* Physical Configuration */
	uint8_t xorigin;           /*!< Object x start position on matrix  */
	uint8_t yorigin;           /*!< Object y start position on matrix  */
	uint8_t xsize;             /*!< Object x size (i.e. width)         */
	uint8_t ysize;             /*!< Object y size (i.e. height)        */

	/* Detection Configuration */
	uint8_t akscfg;             /*!< Adjacent key suppression config     */
	uint8_t blen;               /*!< Burst length for all object channels*/
	uint8_t tchthr;             /*!< Threshold for all object channels   */
	uint8_t tchdi;              /*!< Detect integration config           */
	uint8_t reserved[2];        /*!< Spare x2 */

} touch_keyarray_t15_config_t;

typedef struct
{
	uint8_t  ctrl;
	uint16_t xoffset;
	uint8_t  xsegment[16];
	uint16_t yoffset;
	uint8_t  ysegment[16];

} proci_linearizationtable_t17_config_t;

typedef struct
{
	/* GPIOPWM Configuration */
	uint8_t ctrl;             /*!< Main configuration field           */
	uint8_t reportmask;       /*!< Event mask for generating messages to
							   *   the host */
	uint8_t dir;              /*!< Port DIR register   */
	uint8_t pullup;           /*!< Port pull-up per pin enable register */
	uint8_t out;              /*!< Port OUT register*/
	uint8_t wake;             /*!< Port wake on change enable register  */
	uint8_t pwm;              /*!< Port pwm enable register    */
	uint8_t per;              /*!< PWM period (min-max) percentage*/
	uint8_t duty[4];          /*!< PWM duty cycles percentage */
	uint8_t trigger[4];

} spt_gpiopwm_t19_config_t;

typedef struct
{
	uint8_t ctrl;
	uint8_t xlogrip;
	uint8_t xhigrip;
	uint8_t ylogrip;
	uint8_t yhigrip;
	uint8_t maxtchs;
	uint8_t RESERVED2;
	uint8_t szthr1;
	uint8_t szthr2;
	uint8_t shpthr1;
	uint8_t shpthr2;
	uint8_t supextto;
} proci_gripfacesuppression_t20_config_t;

/*! ==PROCG_NOISESUPPRESSION_T22==
  The T22 NOISESUPPRESSION object provides frequency hopping acquire control,
  outlier filtering and grass cut filtering.
 */
/*! \brief 
  This structure is used to configure the NOISESUPPRESSION object and
  should be made accessible to the host controller.
 */

typedef struct
{
	uint8_t ctrl;                 /* LCMASK ACMASK */
	uint8_t reserved;
	uint8_t reserved1;
	int16_t gcaful;               /* LCMASK */
	int16_t gcafll;               /* LCMASK */
	uint8_t actvgcafvalid;        /* LCMASK */
	uint8_t noisethr;             /* LCMASK */
	uint8_t reserved2;
	uint8_t freqhopscale;
	uint8_t freq[5u];             /* LCMASK ACMASK */
	uint8_t idlegcafvalid;        /* LCMASK */
} procg_noisesuppression_t22_config_t;

/*! ==TOUCH_PROXIMITY_T23==
  The T23 Proximity is a proximity key made of upto 16 channels
 */
/*! \brief
  This structure is used to configure the prox object and should be made
  accessible to the host controller.
 */
 #if 0
typedef struct
{
	uint8_t ctrl;               /*!< ACENABLE LCENABLE Main configuration field           */
	uint8_t xorigin;           /*!< ACMASK LCMASK Object x start position on matrix  */
	uint8_t yorigin;           /*!< ACMASK LCMASK Object y start position on matrix  */
	uint8_t xsize;             /*!< ACMASK LCMASK Object x size (i.e. width)         */
	uint8_t ysize;             /*!< ACMASK LCMASK Object y size (i.e. height)        */
	uint8_t reserved;
	uint8_t blen;               /*!< ACMASK Burst length for all object channels*/
	uint16_t tchthr;             /*!< LCMASK Threshold    */
	uint8_t tchdi;              /*!< Detect integration config           */
	uint8_t average;            /*!< LCMASK Sets the filter length on the prox signal */
	uint16_t rate;               /*!< Sets the rate that prox signal must exceed */

} touch_proximity_t23_config_t;
#endif
typedef struct
{
	uint8_t ctrl;    /*!< Idle power mode sleep length in ms           */
	uint8_t xorigin;/*!< Active power mode sleep length in ms         */
	uint8_t yorigin;
	uint8_t xsize;
	uint8_t ysize;
	uint8_t reserved;
	uint8_t blen;
	uint16_t fxddthr;
	uint8_t fxddi;
	uint8_t average;
	uint16_t mvnullrate;
	uint16_t mvdthr;
} touch_proximity_t23_config_t;
typedef struct
{
	uint8_t  ctrl;
	uint8_t  numgest;
	uint16_t gesten;
	uint8_t  pressproc;
	uint8_t  tapto;
	uint8_t  flickto;
	uint8_t  dragto;
	uint8_t  spressto;
	uint8_t  lpressto;
	uint8_t  rptpressto;
	uint16_t flickthr;
	uint16_t dragthr;
	uint16_t tapthr;
	uint16_t throwthr;
} proci_onetouchgestureprocessor_t24_config_t;


#if MULTITOUCH_ENABLE_CHJ   // for multitouch enable
typedef struct
{
	uint16_t upsiglim;              /* LCMASK */
	uint16_t losiglim;              /* LCMASK */

} siglim_t;

/*! = Config Structure = */

typedef struct
{
	uint8_t  ctrl;                 /* LCENABLE */
	uint8_t  cmd;
#if(NUM_OF_TOUCH_OBJECTS)
	siglim_t siglim[NUM_OF_TOUCH_OBJECTS];   /* LCMASK */
#endif

} spt_selftest_t25_config_t;
#else
typedef struct 
{
	uint16_t upsiglim;
	uint16_t losiglim;
}siglim_t;

typedef struct
{
	uint8_t  ctrl;
	uint8_t  cmd;
	siglim_t siglim[4];
} spt_selftest_t25_config_t;
#endif

typedef struct
{
	uint8_t  ctrl;          /*!< Bit 0 = object enable, bit 1 = report enable */
	uint8_t  numgest;       /*!< Runtime control for how many two touch gestures
							 *   to process */
	uint8_t reserved;
	uint8_t gesten;        /*!< Control for turning particular gestures on or
							*  off */
	uint8_t  rotatethr;
	uint16_t zoomthr;

} proci_twotouchgestureprocessor_t27_config_t;

/*! ==SPT_CTECONFIG_T28==
  The T28 CTECONFIG object provides controls for the CTE.
 */

/*! \brief 
  This structure is used to configure the CTECONFIG object and
  should be made accessible to the host controller.
 */

typedef struct
{
	uint8_t ctrl;          /*!< Ctrl field reserved for future expansion */
	uint8_t cmd;           /*!< Cmd field for sending CTE commands */
	uint8_t mode;          /*!< LCMASK CTE mode configuration field */
	uint8_t idlegcafdepth; /*!< LCMASK The global gcaf number of averages when idle */
	uint8_t actvgcafdepth; /*!< LCMASK The global gcaf number of averages when active */
	uint8_t voltage; 
} spt_cteconfig_t28_config_t;

typedef struct
{
	char * config_name;
//	proximity_t23_config_t proximity_config;
	spt_commsconfig_t18_config_t comms_config;
	gen_powerconfig_t7_config_t power_config;
	gen_acquisitionconfig_t8_config_t acquisition_config;
	touch_multitouchscreen_t9_config_t touchscreen_config;
	touch_keyarray_t15_config_t keyarray_config;
	proci_gripfacesuppression_t20_config_t gripfacesuppression_config;
	proci_linearizationtable_t17_config_t linearization_config;
 	spt_gpiopwm_t19_config_t gpiopwm_config;
	spt_selftest_t25_config_t selftest_config;
	procg_noisesuppression_t22_config_t noise_suppression_config;
	proci_onetouchgestureprocessor_t24_config_t onetouch_gesture_config;
	proci_twotouchgestureprocessor_t27_config_t twotouch_gesture_config;
	spt_cteconfig_t28_config_t cte_config;
	touch_proximity_t23_config_t proximity_config;
	userdata_t38_config_t userdata_config;
} all_config_setting;

typedef struct
{
	int State;
	int x;
	int y;
	int Wx;
	int Wy;
	int press;
} dec_input;
#if 0
typedef struct
{
	int x;
	int y;
	int press;
} dec_input;
#endif
/*! \brief Object table element struct. */
typedef struct
{
	uint8_t object_type;     /*!< Object type ID. */
	uint16_t i2c_address;    /*!< Start address of the obj config structure. */
	uint8_t size;            /*!< Byte length of the obj config structure -1.*/
	uint8_t instances;       /*!< Number of objects of this obj. type -1. */
	uint8_t num_report_ids;  /*!< The max number of touches in a screen,
							  *  max number of sliders in a slider array, etc.*/
} object_t;

typedef struct
{
	uint8_t family_id;            /* address 0 */
	uint8_t variant_id;           /* address 1 */

	uint8_t version;              /* address 2 */
	uint8_t build;                /* address 3 */

	uint8_t matrix_x_size;        /* address 4 */
	uint8_t matrix_y_size;        /* address 5 */

	uint8_t num_declared_objects; /* address 6 */
} info_id_t;

typedef struct
{
	/*! Info ID struct. */
	info_id_t info_id;

	/*! Pointer to an array of objects. */
	object_t *objects;

	/*! CRC field, low bytes. */
	uint16_t CRC;

	/*! CRC field, high byte. */
	uint8_t CRC_hi;
} info_block_t;


typedef struct
{
	uint8_t object_type;         /*!< Object type. */
	uint8_t instance;        /*!< Instance number. */
} report_id_map_t;



static info_block_t *info_block;

static report_id_map_t *report_id_map;

volatile uint8_t read_pending;

static int max_report_id = 0;

uint8_t max_message_length;

uint16_t message_processor_address;

/*! Command processor address. */
static uint16_t command_processor_address;

/*! Flag indicating if driver setup is OK. */
static enum driver_setup_t driver_setup = DRIVER_SETUP_INCOMPLETE;

/*! Pointer to message handler provided by main app. */
//static void (*application_message_handler)(uint8_t *, uint8_t);

/*! Message buffer holding the latest message received. */
uint8_t *quantum_msg;

/*! \brief The current address pointer. */
//static U16 address_pointer;

/* flag to indicate if last calibration has been confirmed good */
static uint8_t cal_check_flag = 0u;

struct i2c_client *tsp_cl;	// hsil
int tsp_off = 0;
EXPORT_SYMBOL(tsp_off);
//extern int tsp_recover_flg;

/*------------------------------ functions prototype -----------------------------------*/
static int quantum_touch_probe(void);

uint8_t init_touch_driver(uint8_t I2C_address/*, void (*handler)(void)*/);
uint8_t get_version(uint8_t *version);
uint8_t get_family_id(uint8_t *family_id);
uint8_t get_build_number(uint8_t *build);
uint8_t get_variant_id(uint8_t *variant);
uint8_t calculate_infoblock_crc(uint32_t *crc_pointer);
uint8_t report_id_to_type(uint8_t report_id, uint8_t *instance);
uint8_t type_to_report_id(uint8_t object_type, uint8_t instance);
uint8_t write_power_config(gen_powerconfig_t7_config_t cfg);
uint8_t get_max_report_id(void);


uint8_t  write_userdata_config(userdata_t38_config_t cfg);
uint8_t write_comms_config(spt_commsconfig_t18_config_t cfg);

uint8_t write_gpio_config(uint8_t instance, spt_gpiopwm_t19_config_t cfg);
uint8_t write_acquisition_config(gen_acquisitionconfig_t8_config_t cfg);
uint8_t write_multitouchscreen_config(uint8_t instance, touch_multitouchscreen_t9_config_t cfg);
uint16_t get_object_address(uint8_t object_type, uint8_t instance);
uint8_t write_linearization_config(uint8_t instance, proci_linearizationtable_t17_config_t cfg);
uint8_t write_twotouchgesture_config(uint8_t instance, proci_twotouchgestureprocessor_t27_config_t cfg);
uint8_t write_onetouchgesture_config(uint8_t instance, proci_onetouchgestureprocessor_t24_config_t cfg);
uint8_t write_gripsuppression_config(uint8_t instance, proci_gripfacesuppression_t20_config_t cfg);
uint8_t write_CTE_config(spt_cteconfig_t28_config_t cfg);
uint8_t write_noisesuppression_config(uint8_t instance, procg_noisesuppression_t22_config_t cfg);
uint8_t write_selftest_config(uint8_t instance, spt_selftest_t25_config_t cfg);
uint8_t backup_config(void);
uint8_t write_proximity_config(uint8_t instance, touch_proximity_t23_config_t cfg);
uint8_t calibrate_chip(void);
int set_all_config( all_config_setting config );
uint8_t write_keyarray_config(uint8_t instance, touch_keyarray_t15_config_t cfg);
static void qt602240_ts_work_func(struct work_struct *work);	//void get_message(void);
uint8_t read_id_block(info_id_t *id);
U8 read_mem(U16 start, U8 size, U8 *mem);
U8 read_U16(U16 start, U16 *mem);
U8 write_mem(U16 start, U8 size, U8 *mem);
uint8_t get_object_size(uint8_t object_type);
uint8_t write_simple_config(uint8_t object_type, uint8_t instance, void *cfg);
uint32_t CRC_24(uint32_t crc, uint8_t byte1, uint8_t byte2);
int qt602240_i2c_read(u16 reg,unsigned char *rbuf, int buf_size);
//void touch_hw_rst( int );
void read_all_register(void);
uint8_t report_all(void);
void check_chip_calibration(unsigned char one_touch_input_flag);

static int qt602240_ts_suspend(struct i2c_client *client, pm_message_t mesg);
static int qt602240_ts_resume(struct i2c_client *client);
uint8_t reset_chip(void);

/*------------------------------ for tunning ATmel - start ----------------------------*/

static int config_set_enable = 0;  // 0 for disable, 1 for enable
//all_config_setting config_normal = { "config_normal" , 0 };
all_config_setting config_normal;	// = { "config_normal" , 0,0, 0,0,0,0,0, 0,0,0,0,0 };
static dec_input id2 = { 0 };
static dec_input id3 = { 0 };
static dec_input tmp2 = { 0 };
static dec_input tmp3 = { 0 };
static int rst_cnt[11] = { 0 };

#if 0
static int CAL_THR = 10;
static int result_cal[100]={0};	
#endif
/*------------------------------ for tunning ATmel - end ----------------------------*/

/* firmware 2009.09.24 CHJ - start 1/2 */
#define QT602240_I2C_BOOT_ADDR 0x24
#define QT_WAITING_BOOTLOAD_COMMAND 0xC0
#define QT_WAITING_FRAME_DATA       0x80
#define QT_FRAME_CRC_CHECK          0x02
#define QT_FRAME_CRC_PASS           0x04
#define QT_FRAME_CRC_FAIL           0x03

#define TS_UPDATE_FIRMWARE_VERSION 	0x21	//0x10 // minhyo100614

uint8_t QT_Boot(unsigned char firmup);
static int firmware_ret_val = -1;

unsigned char firmware_602240[] = {
//#include "ATMXT140_Varient_6_VF.0_AB.h"	//#include "Firmware16_EE.h"
//#include "ATMXT140__APP_v2_1_AA.h"	// ver2.1
#include "ATMXT140__APP_v2_0_AA.h"	// ver2.0
//#include "mXT140__APP_v1_0_AC_.h"		// rollback
//#include "mXT140__APP_vF_0_AC_.h"		// ver 1.0 final
};
/* firmware 2009.09.24 CHJ - end 1/2 */

/* firmware 2009.09.24 CHJ - start 2/2 */

uint8_t boot_reset(void)
{
	uint8_t data = 0xA5u;
	return(write_mem(command_processor_address + RESET_OFFSET, 1, &data));
}

uint8_t read_boot_state(u8 *data)
{
	struct i2c_msg rmsg;
	int ret;

	rmsg.addr = QT602240_I2C_BOOT_ADDR ;
	rmsg.flags = I2C_M_RD;
	rmsg.len = 1;
	rmsg.buf = data;
	ret = i2c_transfer(ts_global->client->adapter, &rmsg, 1);
	printk("[TSP] %s, %d\n", __func__, __LINE__ );

	if ( ret < 0 )
	{
		printk("[CHJ] %s,%d - Fail!!!! ret = %d\n", __func__, __LINE__, ret );
		return READ_MEM_FAILED; 
	}	
	else 
	{
		return READ_MEM_OK;
	}
}

uint8_t boot_unlock(void)
{
	struct i2c_msg wmsg;
	int ret;
	unsigned char data[2];

	printk("[CHJ] %s - start, %d\n", __func__, __LINE__ );

	data[0] = 0xDC;
	data[1] = 0xAA;

	wmsg.addr = QT602240_I2C_BOOT_ADDR ;
	wmsg.flags = I2C_M_WR;
	wmsg.len = 2;
	wmsg.buf = data;

	ret = i2c_transfer(ts_global->client->adapter, &wmsg, 1);

	if ( ret >= 0 )
		return WRITE_MEM_OK;
	else 
		return WRITE_MEM_FAILED; 
}

uint8_t boot_write_mem(uint16_t ByteCount, unsigned char * Data )
{
	struct i2c_msg wmsg;
	int ret;

	wmsg.addr = QT602240_I2C_BOOT_ADDR ;
	wmsg.flags = I2C_M_WR;
	wmsg.len = ByteCount;
	wmsg.buf = Data;

	ret = i2c_transfer(ts_global->client->adapter, &wmsg, 1);
	if ( ret >= 0 )
	{
		return WRITE_MEM_OK;
	}	
	else 
	{
		printk("[CHJ] %s,%d - Fail!!!!\n", __func__, __LINE__ );
		return WRITE_MEM_FAILED; 
	}
}

uint8_t QT_Boot(unsigned char firmup)
{
	unsigned char boot_status;
	unsigned char boot_ver;
	unsigned long f_size;
	unsigned long int character_position;
	unsigned int frame_size = 0;
	unsigned int next_frame;
	unsigned int crc_error_count;
	unsigned char rd_msg;
	unsigned int retry_cnt;
	unsigned int size1,size2;
	unsigned int i;

	// pointer 
	unsigned char *firmware_data ;

	firmware_ret_val = -1;

	// disable_irq for firmware update
	disable_irq(ts_global->client->irq);
	firmware_data = firmware_602240;
	//    firmware_data = &my_code;

	f_size = sizeof(firmware_602240);
	//f_size = sizeof(my_code);

	if(firmup ==1)
		{
		//Step 1 : Boot Reset
		if(boot_reset() == WRITE_MEM_OK)
			printk("[TSP] boot_reset is complete!\n");
		else 	
		{
			printk("[TSP] boot_reset is not complete!\n");
			return 0;	
		}
	}
	i=0;

	for(retry_cnt = 0; retry_cnt < 30; retry_cnt++)
	{
		printk("[TSP] %s,retry = %d\n", __func__, retry_cnt );

		mdelay(60);

		if( (read_boot_state(&boot_status) == READ_MEM_OK) && (boot_status & 0xC0) == 0xC0) 
		{
			boot_ver = boot_status & 0x3F;
			crc_error_count = 0;
			character_position = 0;
			next_frame = 0;

			while(1)
			{
				mdelay(60);
				if(read_boot_state(&boot_status) == READ_MEM_OK)
				{
					if((boot_status & QT_WAITING_BOOTLOAD_COMMAND) == QT_WAITING_BOOTLOAD_COMMAND)
					{
						mdelay(60);
						if(boot_unlock() == WRITE_MEM_OK)
						{
							mdelay(60);
						}
						else
						{
							printk("[TSP] %s - boot_unlock fail!!%d\n", __func__, __LINE__ );
						}
					}
					else if((boot_status & 0xC0) == QT_WAITING_FRAME_DATA)
					{
						printk("[TSP] %s, sending packet %dth\n", __func__,i);
						/* Add 2 to frame size, as the CRC bytes are not included */
						size1 =  *(firmware_data+character_position);
						size2 =  *(firmware_data+character_position+1)+2;
						frame_size = (size1<<8) + size2;
						msleep(200);

						/* Exit if frame data size is zero */
						next_frame = 1;
						boot_write_mem(frame_size, (firmware_data +character_position));
						i++;
						msleep(100);

					}
					else if(boot_status == QT_FRAME_CRC_CHECK)
					{
						mdelay(60);
					}
					else if(boot_status == QT_FRAME_CRC_PASS)
					{
						if( next_frame == 1)
						{
							/* Ensure that frames aren't skipped */
							character_position += frame_size;
							next_frame = 0;
						}
					}
					else if(boot_status  == QT_FRAME_CRC_FAIL)
					{
						printk("[TSP] %s, crc fail %d times\n", __func__, crc_error_count);
						crc_error_count++;
					}
					if(crc_error_count > 10)
					{
						printk("[TSP] %s, crc fail %d times! firmware update fail!!!!\n", __func__, crc_error_count );
						return 0;
						//return QT_FRAME_CRC_FAIL;
					}
				}
				else
				{
					/* Complete Firmware Update!!!!! */
					printk("[TSP] %s, Complete update, Check the new Version!====================\n", __func__);
					/* save new slave address */
					//QT_i2c_address = I2C_address;
					mdelay(60);

					/* Poll device */
					if(read_mem(0, 1, &rd_msg) == READ_MEM_OK)
					{
						quantum_touch_probe();

						if (driver_setup != DRIVER_SETUP_OK)
						{
							printk("[TSP] Error:after firmware update, DRIVER_SETUP fail!!!\n");

						}
						else
						{
							printk("[TSP] after firmware update, DRIVER_SETUP success!!!================\n");

							printk("[TSP] QT602240 Firmware Ver.\n");
							printk("[TSP] version = %x\n", info_block->info_id.version);
							if (ts_global->use_irq)
								enable_irq(ts_global->client->irq);
							else            
								hrtimer_start(&ts_global->timer, ktime_set(0, 12500000), HRTIMER_MODE_REL);


							return 1;
						}	
					}
					printk("[TSP] %s, After firmware update, read_mem fail - line %d\n", __func__, __LINE__ );
					return 0;
				}
			}
		}
		else
		{
			printk("[TSP] read_boot_state() or (boot_status & 0xC0) == 0xC0) is fail!!!\n");
			return 0;
		}

	}
	return 0;
}

/* firmware 2009.09.24 CHJ - end 2/2 */

/*------------------------------ function block -----------------------------------*/

static unsigned int touch_present = 0;
/*------------------------------ main block -----------------------------------*/
static int quantum_touch_probe(void)
{
	U8 report_id, MAX_report_ID;
	U8 object_type, instance;
	U32 crc;//, stored_crc;
	int tmp = -1;
	U8 version = -1, family_id, variant, build;

	DEBUG;

	if (init_touch_driver( QT602240_I2C_ADDR) == DRIVER_SETUP_OK)
	{
		printk("\n[TSP] Touch device found\n");
		touch_present = 1;
	}
	else
	{
		printk("\n[TSP][ERROR] Touch device NOT found\n");
		touch_present = 0;
		return -1;
	}

	/* Get and show the version information. */
	get_family_id(&family_id);
	get_variant_id(&variant);
	get_version(&version);
	get_build_number(&build);

	/* Disable interrupts from touch chip during config. We might miss initial
	 *     * calibration/reset messages, but the text we are ouputting here doesn't
	 *         * get cluttered with touch chip messages. */

	if(calculate_infoblock_crc((uint32_t *)&crc) != CRC_CALCULATION_OK)
	{
		/* "Calculating CRC failed, skipping check!\n" */
		printk("\n[TSP][ERROR] line : %d\n", __LINE__);
		touch_present = 0;
		return -1;
	}



	/* Test the report id to object type / instance mapping: get the maximum
	 *     * report id and print the report id map. */
	MAX_report_ID = get_max_report_id();
	for (report_id = 1; report_id <= MAX_report_ID; report_id++)
	{
		object_type = report_id_to_type(report_id, &instance);
	}

	/* Find the report ID of the first key array (instance 0). */
	report_id = type_to_report_id(TOUCH_KEYARRAY_T15, 0);
#if 1
	config_normal.proximity_config.ctrl = 0;
	config_normal.proximity_config.xorigin = 0;
	config_normal.proximity_config.yorigin = 0;
	config_normal.proximity_config.xsize = 0;
	config_normal.proximity_config.ysize = 0;
	config_normal.proximity_config.reserved = 0;
	config_normal.proximity_config.blen = 0;
	config_normal.proximity_config.fxddthr = 0;
	config_normal.proximity_config.fxddi = 0;
	config_normal.proximity_config.average = 0;
	config_normal.proximity_config.mvnullrate = 0;
	config_normal.proximity_config.mvdthr = 0;

	config_normal.userdata_config.data[0] = 0;
	config_normal.userdata_config.data[1] = 0;
	config_normal.userdata_config.data[2] = 0;
	config_normal.userdata_config.data[3] = 0;
	config_normal.userdata_config.data[4] = 0;
	config_normal.userdata_config.data[5] = 0;
	config_normal.userdata_config.data[6] = 0;
	config_normal.userdata_config.data[7] = 0;

#endif
/*************************************power config start***********************************/
	config_normal.power_config.idleacqint = 32;//255;//48;
	config_normal.power_config.actvacqint = 10;//255;
	config_normal.power_config.actv2idleto = 50; 

/*************************************power config end***********************************/
/*************************************acquisition config start***********************************/
	config_normal.acquisition_config.chrgtime = 12;//8;
	config_normal.acquisition_config.Reserved = 0; 
	config_normal.acquisition_config.tchdrift = 5;   
	config_normal.acquisition_config.driftst = 20;//1;
//	config_normal.acquisition_config.tchautocal = 20;
	config_normal.acquisition_config.tchautocal = 0; 
	config_normal.acquisition_config.sync = 0; 
	config_normal.acquisition_config.atchcalst = 0; 
	config_normal.acquisition_config.atchcalsthr = 0; // --------- 45;//15;//0;
	config_normal.acquisition_config.atchfrccalthr = 10;     
	config_normal.acquisition_config.atchfrccalratio = -25; 
/*************************************acquisition config end***********************************/
/**************************************touchscreen config start*****************************/
	config_normal.touchscreen_config.ctrl = 131;   // former value => 143; 
	config_normal.touchscreen_config.xorigin = 0; 
	config_normal.touchscreen_config.yorigin = 0; 
	config_normal.touchscreen_config.xsize = 18;    // . 
	config_normal.touchscreen_config.ysize = 11; // . 
	config_normal.touchscreen_config.akscfg = 0; 
	config_normal.touchscreen_config.blen = 16; //32;
	config_normal.touchscreen_config.tchthr = 50;//70;  // 1.2T 20100720 from eplus
	config_normal.touchscreen_config.tchdi = 2; 
	config_normal.touchscreen_config.orientate = 3;//0;  
	config_normal.touchscreen_config.mrgtimeout = 0; 
	config_normal.touchscreen_config.movhysti = 0; 
	config_normal.touchscreen_config.movhystn = 0; 
	config_normal.touchscreen_config.movfilter = 0; 
	config_normal.touchscreen_config.numtouch= 2;
	config_normal.touchscreen_config.mrghyst = 0; 
	config_normal.touchscreen_config.mrgthr = 0; 
	config_normal.touchscreen_config.tchamphyst = 0; //former value => 10;
	config_normal.touchscreen_config.xres = 890;//956;    // MAX_Y-1. LCD.
	config_normal.touchscreen_config.yres = 479;//504;    // MAX_X-1
	config_normal.touchscreen_config.xloclip = 0;   //0; edge.
	config_normal.touchscreen_config.xhiclip = 0; 
	config_normal.touchscreen_config.yloclip = 0;
	config_normal.touchscreen_config.yhiclip = 0;
	config_normal.touchscreen_config.xedgectrl= 0;  //0; edge
	config_normal.touchscreen_config.xedgedist= 0;  //0; edge.
	config_normal.touchscreen_config.yedgectrl= 0;
	config_normal.touchscreen_config.yedgedist= 0;
	config_normal.touchscreen_config.jumplimit= 0;
	config_normal.touchscreen_config.tchhyst = 0;
/**************************************touchscreen config end*****************************/
/**************************************kerarray config start*****************************/

	config_normal.keyarray_config.ctrl = 0;
	config_normal.keyarray_config.xorigin = 0;
	config_normal.keyarray_config.xsize = 0;
	config_normal.keyarray_config.yorigin = 0;
	config_normal.keyarray_config.ysize = 0;
	config_normal.keyarray_config.akscfg = 0;
	config_normal.keyarray_config.blen = 0;
	config_normal.keyarray_config.tchthr = 0;
	config_normal.keyarray_config.tchdi = 0;
	config_normal.keyarray_config.reserved[0] = 0;
	config_normal.keyarray_config.reserved[1] = 0;
/**************************************kerarray config end*****************************/

	config_normal.linearization_config.ctrl = 0;
	config_normal.twotouch_gesture_config.ctrl = 0;

	config_normal.onetouch_gesture_config.ctrl = 0;
	config_normal.onetouch_gesture_config.numgest = 0;
	config_normal.onetouch_gesture_config.gesten = 3;
	config_normal.onetouch_gesture_config.pressproc = 0;
	config_normal.onetouch_gesture_config.tapto = 0;
	config_normal.onetouch_gesture_config.flickto = 0;
	config_normal.onetouch_gesture_config.dragto = 0;
	config_normal.onetouch_gesture_config.spressto = 0;
	config_normal.onetouch_gesture_config.lpressto = 0;
	config_normal.onetouch_gesture_config.rptpressto = 0;
	config_normal.onetouch_gesture_config.flickthr = 0;
	config_normal.onetouch_gesture_config.dragthr = 0;
	config_normal.onetouch_gesture_config.tapthr = 0;
	config_normal.onetouch_gesture_config.throwthr = 0;

	config_normal.noise_suppression_config.ctrl = 1;
	config_normal.noise_suppression_config.gcaful = 0;//25;
	config_normal.noise_suppression_config.gcafll = 0;//65511;
	config_normal.noise_suppression_config.actvgcafvalid = 0;//3;
	config_normal.noise_suppression_config.noisethr = 35;
	config_normal.noise_suppression_config.freqhopscale = 0;
	config_normal.noise_suppression_config.freq[0] = 18;//6;//0;
	config_normal.noise_suppression_config.freq[1] = 21;    //15;//11;//255;
	config_normal.noise_suppression_config.freq[2] = 29;    //25;//16;//12;
	config_normal.noise_suppression_config.freq[3] = 255;   //35;//19;//15;
	config_normal.noise_suppression_config.freq[4] = 255;//21;//20;
	config_normal.noise_suppression_config.idlegcafvalid = 0;//3;
	config_normal.noise_suppression_config.reserved2 = 0;//3;
	config_normal.noise_suppression_config.reserved1 = 0;//3;
	config_normal.noise_suppression_config.reserved = 0;//3;

	config_normal.selftest_config.ctrl = 0;
	config_normal.selftest_config.cmd = 0;
	config_normal.selftest_config.siglim[0].upsiglim = 0;
	config_normal.selftest_config.siglim[0].upsiglim = 0;
	config_normal.selftest_config.siglim[1].upsiglim = 0;
	config_normal.selftest_config.siglim[1].upsiglim = 0;
	config_normal.selftest_config.siglim[2].upsiglim = 0;
	config_normal.selftest_config.siglim[2].upsiglim = 0;


	config_normal.gripfacesuppression_config.ctrl = 0;  // disable grip, enable face
	config_normal.gripfacesuppression_config.ctrl = 0;  // disable grip, enable face
	config_normal.gripfacesuppression_config.xlogrip = 0 ;
	config_normal.gripfacesuppression_config.xhigrip = 0 ;
	config_normal.gripfacesuppression_config.ylogrip = 0 ;
	config_normal.gripfacesuppression_config.yhigrip = 0 ;
	config_normal.gripfacesuppression_config.maxtchs = 0 ;
	config_normal.gripfacesuppression_config.RESERVED2 = 0 ;
	config_normal.gripfacesuppression_config.szthr1 = 0 ;
	config_normal.gripfacesuppression_config.szthr2 = 0 ;
	config_normal.gripfacesuppression_config.shpthr1 = 0 ;
	config_normal.gripfacesuppression_config.shpthr2 = 0 ;
	config_normal.gripfacesuppression_config.supextto = 0 ;

	config_normal.cte_config.ctrl = 0;
	config_normal.cte_config.cmd = 0;
	config_normal.cte_config.mode = 2;
//	config_normal.cte_config.mode = 1;
	config_normal.cte_config.idlegcafdepth = 8;
	config_normal.cte_config.actvgcafdepth = 8;
	config_normal.cte_config.voltage = 10; // minhyo20100716 10;//30;   // This value will be not set at Ver 1.4

	config_normal.gpiopwm_config.ctrl = 0;
	config_normal.gpiopwm_config.reportmask = 0;
	config_normal.gpiopwm_config.dir = 0;
	config_normal.gpiopwm_config.pullup = 0;
	config_normal.gpiopwm_config.out = 0;
	config_normal.gpiopwm_config.wake = 0;
	config_normal.gpiopwm_config.pwm = 0;
	config_normal.gpiopwm_config.per = 0;
	config_normal.gpiopwm_config.duty[0] = 0;
	config_normal.gpiopwm_config.duty[1] = 0;
	config_normal.gpiopwm_config.duty[2] = 0;
	config_normal.gpiopwm_config.duty[3] = 0;
	config_normal.gpiopwm_config.trigger[0] = 0;
	config_normal.gpiopwm_config.trigger[1] = 0;
	config_normal.gpiopwm_config.trigger[2] = 0;
	config_normal.gpiopwm_config.trigger[3] = 0;


	config_normal.comms_config.ctrl = 0;
	config_normal.comms_config.command = 0;
	/* set all configuration */
/**********************************************************************************************/
	tmp = set_all_config( config_normal );
/**********************************************************************************************/

	if( tmp == 1 )
		printk("[TSP] set all configs success : %d\n", __LINE__);
	else
	{
		printk("[TSP] set all configs fail : error : %d\n", __LINE__);
///		touch_hw_rst( 1 );  // TOUCH HW RESET No.1
		tmp = set_all_config( config_normal );

		if( tmp == 1 )
			printk("[TSP] set all configs success : %d\n", __LINE__);
		else
			printk("[TSP] 2nd set all configs fail : error : %d\n", __LINE__);
	}	
	
	return 0;
}

int INT_clear( void )
{
	int ret = -1;

	ret = qt602240_i2c_read( message_processor_address, quantum_msg , 1);

	if ( ret < 0 )
		printk("[TSP] %s : i2c fail : %d\n", __func__ , __LINE__ );

	return ret;
}

/*------------------------------ Sub functions -----------------------------------*/
/*!
  \brief Initializes touch driver.

  This function initializes the touch driver: tries to connect to given 
  address, sets the message handler pointer, reads the info block and object
  table, sets the message processor address etc.

  @param I2C_address is the address where to connect.
  @param (*handler) is a pointer to message handler function.
  @return DRIVER_SETUP_OK if init successful, DRIVER_SETUP_FAILED 
  otherwise.
 */
uint8_t init_touch_driver(uint8_t I2C_address/*, void (*handler)(void)*/)
{
	int i;

	int current_report_id = 0;

	uint8_t tmp;
	uint16_t current_address;
	uint16_t crc_address;
	object_t *object_table;
	info_id_t *id;

	uint32_t *CRC;

	uint8_t status;


	printk("[QT] %s start\n",__func__);

	/* Try to connect. */
	/* Read the info block data. */
	id = (info_id_t *) kmalloc(sizeof(info_id_t), GFP_KERNEL | GFP_ATOMIC);
	if (id == NULL)
	{
		return(DRIVER_SETUP_INCOMPLETE);
	}



	if (read_id_block(id) != 1)
	{
		printk("[TSP][ERROR] 2\n");
//		kfree(id);
		return(DRIVER_SETUP_INCOMPLETE);
	}  

	/* Read object table. */

	object_table = (object_t *) kmalloc(id->num_declared_objects * sizeof(object_t), GFP_KERNEL | GFP_ATOMIC);
	if (object_table == NULL)
	{
		printk("[TSP][ERROR] 3\n");
		return(DRIVER_SETUP_INCOMPLETE);
	}


	/* Reading the whole object table block to memory directly doesn't work cause sizeof object_t
	   isn't necessarily the same on every compiler/platform due to alignment issues. Endianness
	   can also cause trouble. */

	current_address = OBJECT_TABLE_START_ADDRESS;

	for (i = 0; i < id->num_declared_objects; i++)
	{
		status = read_mem(current_address, 1, &(object_table[i]).object_type);
		if (status != READ_MEM_OK)
		{
			printk("[TSP][ERROR] 4\n");
			return(DRIVER_SETUP_INCOMPLETE);
		}
		current_address++;

		status = read_U16(current_address, &object_table[i].i2c_address);
		if (status != READ_MEM_OK)
		{
			printk("[TSP][ERROR] 5\n");
			return(DRIVER_SETUP_INCOMPLETE);
		}
		current_address += 2;

		status = read_mem(current_address, 1, (U8*)&object_table[i].size);
		if (status != READ_MEM_OK)
		{
			printk("[TSP][ERROR] 6\n");
			return(DRIVER_SETUP_INCOMPLETE);
		}
		current_address++;

		status = read_mem(current_address, 1, &object_table[i].instances);
		if (status != READ_MEM_OK)
		{
			printk("[TSP][ERROR] 7\n");
			return(DRIVER_SETUP_INCOMPLETE);
		}
		current_address++;

		status = read_mem(current_address, 1, &object_table[i].num_report_ids);
		if (status != READ_MEM_OK)
		{
			printk("[TSP][ERROR] 8\n");
			return(DRIVER_SETUP_INCOMPLETE);
		}
		current_address++;

		max_report_id += object_table[i].num_report_ids;

		/* Find out the maximum message length. */
		if (object_table[i].object_type == GEN_MESSAGEPROCESSOR_T5)
		{
			max_message_length = object_table[i].size + 1;
		}
	}

	/* Check that message processor was found. */
	if (max_message_length == 0)
	{
		printk("[TSP][ERROR] 9\n");
		kfree(object_table);
		return(DRIVER_SETUP_INCOMPLETE);
	}

	/* Read CRC. */

	CRC = (uint32_t *) kmalloc(sizeof(info_id_t), GFP_KERNEL | GFP_ATOMIC);
	if (CRC == NULL)
	{
		printk("[TSP][ERROR] 10\n");
		kfree(object_table);
		return(DRIVER_SETUP_INCOMPLETE);
	}



	info_block = kmalloc(sizeof(info_block_t), GFP_KERNEL | GFP_ATOMIC);
	if (info_block == NULL)
	{
		printk("err\n");
		kfree(object_table);
		kfree(CRC);
		return(DRIVER_SETUP_INCOMPLETE);
	}


	info_block->info_id = *id;

	info_block->objects = object_table;

	crc_address = OBJECT_TABLE_START_ADDRESS + 
		id->num_declared_objects * OBJECT_TABLE_ELEMENT_SIZE;

	status = read_mem(crc_address, 1u, &tmp);
	if (status != READ_MEM_OK)
	{
		printk("[TSP][ERROR] 11\n");
		return(DRIVER_SETUP_INCOMPLETE);
	}
	info_block->CRC = tmp;

	status = read_mem(crc_address + 1u, 1u, &tmp);
	if (status != READ_MEM_OK)
	{
		printk("[TSP][ERROR] 12\n");
		return(DRIVER_SETUP_INCOMPLETE);
	}
	info_block->CRC |= (tmp << 8u);

	status = read_mem(crc_address + 2, 1, &info_block->CRC_hi);
	if (status != READ_MEM_OK)
	{
		printk("[TSP][ERROR] 13\n");
		return(DRIVER_SETUP_INCOMPLETE);
	}

	/* Store message processor address, it is needed often on message reads. */
	message_processor_address = get_object_address(GEN_MESSAGEPROCESSOR_T5, 0);
	//   printk("%s message_processor_address = %d\n",__FUNCTION__, message_processor_address );

	if (message_processor_address == 0)
	{
		printk("[TSP][ERROR] 14 !!\n");
		return(DRIVER_SETUP_INCOMPLETE);
	}

	/* Store command processor address. */
	command_processor_address = get_object_address(GEN_COMMANDPROCESSOR_T6, 0);
	if (command_processor_address == 0)
	{
		printk("[TSP][ERROR] 15\n");
		return(DRIVER_SETUP_INCOMPLETE);
	}

	quantum_msg = kmalloc(max_message_length, GFP_KERNEL | GFP_ATOMIC);
	if (quantum_msg == NULL)
	{
		printk("[TSP][ERROR] 16\n");
		return(DRIVER_SETUP_INCOMPLETE);
	}

	/* Allocate memory for report id map now that the number of report id's 
	 * is known. */

	report_id_map = kmalloc(sizeof(report_id_map_t) * max_report_id, GFP_KERNEL | GFP_ATOMIC);

	if (report_id_map == NULL)
	{
		printk("[TSP][ERROR] 17\n");
		return(DRIVER_SETUP_INCOMPLETE);
	}


	/* Report ID 0 is reserved, so start from 1. */

	report_id_map[0].instance = 0;
	report_id_map[0].object_type = 0;
	current_report_id = 1;

	for (i = 0; i < id->num_declared_objects; i++)
	{
		if (object_table[i].num_report_ids != 0)
		{
			int instance;
			for (instance = 0; 
					instance <= object_table[i].instances; 
					instance++)
			{
				int start_report_id = current_report_id;
				for (; 
						current_report_id < 
						(start_report_id + object_table[i].num_report_ids);
						current_report_id++)
				{
					report_id_map[current_report_id].instance = instance;
					report_id_map[current_report_id].object_type = 
						object_table[i].object_type;
				}
			}
		}
	}
	driver_setup = DRIVER_SETUP_OK;

	return(DRIVER_SETUP_OK);
}

int set_all_config( all_config_setting config )
{
	int ret=-1;
	static int one_time_flag = 1;

	printk("[TSP] %s set!, set condition = %s\n", __func__, config.config_name );

	/* Write power config to chip. */
	if (write_power_config(config.power_config) != CFG_WRITE_OK)
	{
		/* "Power config write failed!\n" */
		printk("\n[TSP][ERROR] line : %d\n", __LINE__);
		return ret;
	}
	if (write_userdata_config(config.userdata_config) != CFG_WRITE_OK)
	{
		/* "Power config write failed!\n" */
		printk("\n[TSP][ERROR] line : %d\n", __LINE__);
		return ret;
	}
	if (write_comms_config(config.comms_config) != CFG_WRITE_OK)
	{
		/* "Power config write failed!\n" */
		printk("\n[TSP][ERROR] line : %d\n", __LINE__);
		return ret;
	}

	/* Write acquisition config to chip. */
	if (write_acquisition_config(config.acquisition_config) != CFG_WRITE_OK)
	{
		/* "Acquisition config write failed!\n" */
		printk("\n[TSP][ERROR] line : %d\n", __LINE__);
		return ret;
	}

	/* Write touchscreen (1st instance) config to chip. */
	if (write_multitouchscreen_config(0, config.touchscreen_config) != CFG_WRITE_OK)
	{
		/* "Multitouch screen config write failed!\n" */
		printk("\n[TSP][ERROR] line : %d\n", __LINE__);
		return ret;
	}

	if (write_gpio_config(0, config.gpiopwm_config) != CFG_WRITE_OK)
	{
		printk("\n[TSP][ERROR] line : %d\n", __LINE__);
		return ret;
		
	}
	if (write_proximity_config(0, config.proximity_config) != CFG_WRITE_OK)
	{
		printk("\n[TSP][ERROR] line : %d\n", __LINE__);
		return ret;
		
	}
	/* Write grip suppression config to chip. */
	if (get_object_address(PROCI_GRIPFACESUPPRESSION_T20, 0) != OBJECT_NOT_FOUND)
	{
		if (write_gripsuppression_config(0, config.gripfacesuppression_config) != CFG_WRITE_OK)
		{
			/* "Grip suppression config write failed!\n" */
			printk("\n[TSP][ERROR] line : %d\n", __LINE__);
			return ret;
		}
	}

	/* Write CTE config to chip. */
	if (get_object_address(SPT_CTECONFIG_T28, 0) != OBJECT_NOT_FOUND)
	{
		if (write_CTE_config(config.cte_config) != CFG_WRITE_OK)
		{
			/* "CTE config write failed!\n" */
			printk("\n[TSP][ERROR] line : %d\n", __LINE__);
			return ret;
		}
	}

	/* Write Noise suppression config to chip. */
	if (get_object_address(PROCG_NOISESUPPRESSION_T22, 0) != OBJECT_NOT_FOUND)
	{
		if (write_noisesuppression_config(0,config.noise_suppression_config) != CFG_WRITE_OK)
		{
			/* "CTE config write failed!\n" */
			printk("\n[TSP][ERROR] line : %d\n", __LINE__);
			return ret;
		}
	}

	/* Backup settings to NVM. */
	if ( one_time_flag )	
	{
		one_time_flag--;
/**********************************************************************************************/
		if ( backup_config() != WRITE_MEM_OK) 
		{
			/* "Failed to backup, exiting...\n" */
			printk("\n[TSP][ERROR] line : %d\n", __LINE__);
			return ret;
		}
/**********************************************************************************************/
		printk("[TSP] backup one time\n");
		msleep(500);

		/* Calibrate the touch IC. */
		if (reset_chip() != WRITE_MEM_OK)
		{
			printk("Failed to s/w reset, exiting...\n");
			return ret;
		}
	}
	msleep(100);
	if (calibrate_chip() != WRITE_MEM_OK)
	{
		printk("Failed to calibrate, exiting...\n");
		return ret;
	}

	return 1;

}

void read_all_register()
{
	U16 addr = 0;
	U8 msg;
	U16 calc_addr = 0;

	for(addr = 0 ; addr < 1273 ; addr++)
	{
		calc_addr = addr;


		if(read_mem(addr, 1, &msg) == READ_MEM_OK)
		{
			printk("(0x%2x)", msg);
			if( (addr+1) % 10 == 0)
			{
				printk("\n");
				printk("%2d : ", addr+1);
			}

		}else
		{
			printk("\n\n[TSP][ERROR] %s() read fail !! \n", __FUNCTION__);
		}
	}
}


// Sleep ���¿��� TA ���� �� wakeup �� battery ���� �������.
// battery ���� skip �ϵ��� �ϰ� resume �Լ����� Ʃ�װ� �����ϵ��� ��.
// battery ���� TA ���º����� ������ �ϰ�, Ʃ�װ��� ��ġ����̹����� �����ǵ��� ��.

//extern int tsp_charger_type_status;
int tsp_charger_type_status = 0; 

//#if USE_TS_TA_DETECT_CHANGE_REG 
			// for TSP Lagging Issue : caused by Acryle
static int pre_ta_status = 0;
int set_tsp_for_tunning_param(int state)
{
	uint16_t object_address;
	uint8_t *tmp;
	uint8_t status;
		if(state)
		{	
			config_normal.touchscreen_config.tchthr = 70;	//30;	//35;  
			config_normal.noise_suppression_config.ctrl = 5;//131;   //MEDIAN
			printk("[TSP] TA Detect!!!\n");

			object_address = get_object_address(TOUCH_MULTITOUCHSCREEN_T9, 0);
	
			if (object_address == 0)
			{
				printk("\n[TSP][ERROR] TOUCH_MULTITOUCHSCREEN_T9 object_address : %d\n", __LINE__);
#if 0
				if (ts_global->use_irq)
					enable_irq(ts_global->client->irq);
				else            
					hrtimer_start(&ts_global->timer, ktime_set(0, 12500000), HRTIMER_MODE_REL);
#endif

				return -1;
			}
			tmp= &config_normal.touchscreen_config.tchthr;
			status = write_mem(object_address+7, 1, tmp);	
			
			if (status == WRITE_MEM_FAILED)
			{
			    printk("\n[TSP][ERROR] TOUCH_MULTITOUCHSCREEN_T9 write_mem : %d\n", __LINE__);
			}
#if 1	
			/* Write Noise suppression config to chip. */
			if (get_object_address(PROCG_NOISESUPPRESSION_T22, 0) != OBJECT_NOT_FOUND)
			{
				if (write_noisesuppression_config(0,config_normal.noise_suppression_config) != CFG_WRITE_OK)
				{
					/* "CTE config write failed!\n" */
					printk("\n[TSP][ERROR] line : %d\n", __LINE__);
#if 0
					if (ts_global->use_irq)
						enable_irq(ts_global->client->irq);
					else            
						hrtimer_start(&ts_global->timer, ktime_set(0, 12500000), HRTIMER_MODE_REL);

#endif
					return -1;
				}
			}
#else
			object_address = get_object_address(PROCG_NOISESUPPRESSION_T22, 0);
	
			if (object_address == 0)
			{
				printk("\n[TSP][ERROR] PROCG_NOISESUPPRESSION_T22 object_address : %d\n", __LINE__);
#if 0
				if (ts_global->use_irq)
					enable_irq(client->irq);
				else            
					hrtimer_start(&ts_global->timer, ktime_set(0, 12500000), HRTIMER_MODE_REL);
#endif

				return -1;
			}
			tmp= &config_normal.noise_suppression_config.noisethr ;
			status = write_mem(object_address+8, 1, tmp);	
			
			if (status == WRITE_MEM_FAILED)
			{
	    	printk("\n[TSP][ERROR] PROCG_NOISESUPPRESSION_T22 write_mem : %d\n", __LINE__);
			}	
#endif		
			pre_ta_status = 1;
	  }

	  else
	  {
			// normal status ok..
			config_normal.touchscreen_config.tchthr = 70;  
//			config_normal.touchscreen_config.movfilter = 15;
	    config_normal.noise_suppression_config.ctrl = 5;//131;    
//	    config_normal.noise_suppression_config.noisethr = 20;       	
/*	    config_normal.noise_suppression_config.noisethr = 20;          	
			config_normal.noise_suppression_config.gcaful = 0;//25;             
			config_normal.noise_suppression_config.gcafll = 0;//65511;             
			config_normal.noise_suppression_config.actvgcafvalid = 0;//3;  
			config_normal.noise_suppression_config.idlegcafvalid = 0;//3;       
	*/		
			printk("[TSP] TA NON-Detect!!!\n");

			object_address = get_object_address(TOUCH_MULTITOUCHSCREEN_T9, 0);
	
			if (object_address == 0)
			{
			  printk("\n[TSP][ERROR] TOUCH_MULTITOUCHSCREEN_T9 object_address : %d\n", __LINE__);
			  if (ts_global->use_irq)
				  enable_irq(ts_global->client->irq);
			  else            
				  hrtimer_start(&ts_global->timer, ktime_set(0, 12500000), HRTIMER_MODE_REL);
				return -1;
			}
			tmp= &config_normal.touchscreen_config.tchthr;
			status = write_mem(object_address+7, 1, tmp);	
			
			if (status == WRITE_MEM_FAILED)
			{
			  printk("\n[TSP][ERROR] TOUCH_MULTITOUCHSCREEN_T9 write_mem : %d\n", __LINE__);
			}
#if 1	
			/* Write Noise suppression config to chip. */
			if (get_object_address(PROCG_NOISESUPPRESSION_T22, 0) != OBJECT_NOT_FOUND)
			{
				if (write_noisesuppression_config(0,config_normal.noise_suppression_config) != CFG_WRITE_OK)
				{
					/* "CTE config write failed!\n" */
					printk("\n[TSP][ERROR] line : %d\n", __LINE__);
					if (ts_global->use_irq)
						enable_irq(ts_global->client->irq);
					else            
						hrtimer_start(&ts_global->timer, ktime_set(0, 12500000), HRTIMER_MODE_REL);

					return -1;
				}
			}
#else
			object_address = get_object_address(PROCG_NOISESUPPRESSION_T22, 0);
	
			if (object_address == 0)
			{
				printk("\n[TSP][ERROR] PROCG_NOISESUPPRESSION_T22 object_address : %d\n", __LINE__);
				if (ts_global->use_irq)
					enable_irq(client->irq);
				else            
					hrtimer_start(&ts_global->timer, ktime_set(0, 12500000), HRTIMER_MODE_REL);
				return -1;
			}
			tmp= &config_normal.noise_suppression_config.noisethr ;
			status = write_mem(object_address+8, 1, tmp);	
			
			if (status == WRITE_MEM_FAILED)
			{
			   printk("\n[TSP][ERROR] PROCG_NOISESUPPRESSION_T22 write_mem : %d\n", __LINE__);
			}	
#endif
			pre_ta_status = 0;
		}        

		return 0;
}

int set_tsp_for_ta_detect(int state)
{
	if( pre_ta_status != state )
	{
		if( touch_present )
		{
//				printk("[TSP] [1] set_tsp_for_ta_detect!!!\n");
			if( driver_setup == DRIVER_SETUP_OK )
			{
//				printk("[TSP] [2] set_tsp_for_ta_detect!!!\n");
//				disable_irq(ts_global->client->irq);
				set_tsp_for_tunning_param( state );
//				enable_irq(ts_global->client->irq);
			}
		}
	}
//	else
//			printk("[TSP] TA status is not changed : pre_ta_status[%d] state[%d]\n",pre_ta_status,state );
	
	return 1;
}
EXPORT_SYMBOL(set_tsp_for_ta_detect);
//#endif


//static int freqErrorCount = 0;
//static int continue_freq_error = 0;

/*!
 * \brief Reads message from the message processor.
 * 
 * This function reads a message from the message processor and calls
 * the message handler function provided by application earlier.
 *
 * @return MESSAGE_READ_OK if driver setup correctly and message can be read 
 * without problems, or MESSAGE_READ_FAILED if setup not done or incorrect, 
 * or if there's a previous message read still ongoing (this can happen if
 * we are both polling the CHANGE line and monitoring it with interrupt that
 * is triggered by falling edge).
 * 
 */

static void qt602240_ts_work_func(struct work_struct *work)	//void get_message(void)
{
	struct qt602240_ts_data *ts = container_of(work, struct qt602240_ts_data, work);

	unsigned int x = 0, y = 0;
	int size = -1;
	static int check_flag=0;
	uint8_t touch_message_flag = 0;
	unsigned char g_one_touch_input_flag=0;

	struct i2c_msg rmsg[2];
	unsigned char data[2];
//printk("88888888888888888888888888\n\n");

	if(driver_setup == DRIVER_SETUP_INCOMPLETE)
		goto TS_WORK_FUNC_EXIT;

	/* 
	 * quantum_msg[0] : message ID 
	 * quantum_msg[1] : status 
	 * quantum_msg[2] : x position
	 * quantum_msg[3] : y position
	 * quantum_msg[4] : x & y position
	 * quantum_msg[5] : size
	 * quantum_msg[6] : Touch amplitude
	 * quantum_msg[7] : Touch vector
	 */

	quantum_msg[0]= 0;  // clear

	rmsg[0].addr = ts->client->addr;
	rmsg[0].flags = I2C_M_WR;
	rmsg[0].len = 2;
	rmsg[0].buf = data;
	data[0] = message_processor_address & 0x00ff;
	data[1] = message_processor_address >> 8;
	rmsg[1].addr = ts->client->addr;
	rmsg[1].flags = I2C_M_RD;
	rmsg[1].len = READ_MESSAGE_LENGTH;
	rmsg[1].buf = quantum_msg;
	size = i2c_transfer(ts->client->adapter, rmsg, 2);
	if ( size != 2)
	{
		printk("[TSP] Error code i2c_transfer : %d\n", __LINE__ );
		goto TS_WORK_FUNC_EXIT;
	}
#if TRACES
	printk("[quantum_msg[0] = 0x%x]\n", quantum_msg[0]);
#endif
#if 0
printk("++++++++++++++++++++++++++\n");
printk("[quantum_msg[0] = 0x%x]\n", quantum_msg[0]);
printk("[quantum_msg[1] = 0x%x]\n", quantum_msg[1]);
printk("[quantum_msg[2] = 0x%x]\n", quantum_msg[2]);
printk("[quantum_msg[3] = 0x%x]\n", quantum_msg[3]);
printk("[quantum_msg[4] = 0x%x]\n", quantum_msg[4]);
printk("[quantum_msg[5] = 0x%x]\n", quantum_msg[5]);
printk("[quantum_msg[6] = 0x%x]\n", quantum_msg[6]);
printk("[quantum_msg[7] = 0x%x]\n", quantum_msg[7]);
printk("++++++++++++++++++++++++++\n");
#endif
	/* 
	 * quantum_msg[1] & 0x80 : 10000000 -> DETECT 
	 * quantum_msg[1] & 0x40 : 01000000 -> PRESS
	 * quantum_msg[1] & 0x20 : 00100000 -> RELEASE
	 * quantum_msg[1] & 0x10 : 00010000 -> MOVE
	 * quantum_msg[1] & 0x08 : 00001000 -> VECTOR
	 * quantum_msg[1] & 0x04 : 00000100 -> AMP
	 * quantum_msg[1] & 0x02 : 00000010 -> SUPPRESS
	 */

	/* for ID=2 & 3, these are valid inputs. */
	if ((quantum_msg[0]==2)||(quantum_msg[0]==3))
	{
		x = quantum_msg[2]; 
		x = x << 2;
		x = x | (quantum_msg[4] >> 6);


		y = quantum_msg[3];
		y = y << 2;
		y = y | ((quantum_msg[4] & 0xC)  >> 2);
		size = quantum_msg[5];
#if TRACES
		printk("__________[x = %d][y = %d][size = %d]______________\n\n", x, y, size);
#endif
		/* for Single Touch input */
		if((id2.press+id3.press)!= 2)
		{
			/* case.1 - case 10010000 -> DETECT & MOVE */
			if( ( quantum_msg[1] & 0x90 ) == 0x90 )
			{
				touch_message_flag = 1;
				REPORT( 1, size , x, y);
#if TRACES
				printk("DETECT & MOVE\n");
#endif
			}
			/* case.2 - 11000000 -> DETECT & PRESS */
			else if( ( quantum_msg[1] & 0xC0 ) == 0xC0  ) 
			{
				touch_message_flag = 1;

				if (quantum_msg[0] % 2)
					id3.press = 1;   // for ID=3
				else
					id2.press = 1;	 // for ID=2

				if ( (id2.press + id3.press) == 2 )
				{
					if ( quantum_msg[0] % 2)
					{
						REPORT( 1, size, tmp2.x, tmp2.y );
					}  // for ID=3
					else 
					{
						REPORT( 1, size, tmp3.x, tmp3.y );
					}  // for ID=2
					REPORT( 1, size, x, y );

					if (quantum_msg[0] % 2)
					{
						id2.x = tmp2.x;	// for ID=3
						id2.y = tmp2.y;
						id3.x = x;
						id3.y = y;
					}
					else
					{
						id2.x = x; 		// for ID=2
						id2.y = y;
						id3.x = tmp3.x;
						id3.y = tmp3.y;
					}
				}
				else
				{
					
					g_one_touch_input_flag = 1;
					REPORT( 1, size, x, y );
				}
#if TRACES
				printk("DETECT & PRESS\n");
#endif				
			}
			/* case.3 - case 00100000 -> RELEASE */
			else if( ((quantum_msg[1] & 0x20 ) == 0x20))   
			{
				REPORT( 0, 1, x, y );

				if (quantum_msg[0] % 2)
					id3.press = 0; 	// for ID=3
				else
					id2.press = 0;	// for ID=2
#if TRACES
				printk("RELEASE \n");
#endif
			}

			input_sync(ts->input_dev);

			if (quantum_msg[0] % 2)
			{
				tmp3.x = x; 	// for ID=3
				tmp3.y = y;
			}
			else
			{
				tmp2.x = x;		// for ID=2
				tmp2.y = y;
			}
		}
		/* for Two Touch input */
		else if( id2.press && id3.press )
		{
			if ( quantum_msg[0] % 2 )
			{
				id3.x = x; // for x, ID=3
				id3.y = y; // for y
			}
			else
			{
				id2.x = x; // for x, ID=2
				id2.y = y; // for y
			}

			if( ((quantum_msg[1] & 0x20 ) == 0x20))   // release
			{
				if (quantum_msg[0] % 2)
					id3.press = 0;	// for ID=3
				else
					id2.press = 0;	// for ID=2
			}	

			if ( id2.y && id3.y ) // report
			{
				REPORT( id2.press, size, id2.x, id2.y );
				REPORT( id3.press, size, id3.x, id3.y );

				input_sync(ts->input_dev);

				id2.y = 0;  // clear
				id3.y = 0;
			}
			else if ((quantum_msg[0]==3)  // report one real value and one dummy value
					|| (quantum_msg[0]==2 && check_flag) // id2 is reported twice
					|| ((quantum_msg[1] & 0x20) == 0x20 )) // any one is released
			{
				if( id3.y == 0 )
				{
					REPORT( id2.press , size , id2.x, id2.y);
					REPORT( id3.press , size , tmp3.x, tmp3.y);
					input_sync(ts->input_dev);
					id2.y = 0;  // clear
				}
				else if( id2.y == 0 )
				{
					REPORT( id2.press , size , tmp2.x, tmp2.y);
					REPORT( id3.press , size , id3.x, id3.y);
					input_sync(ts->input_dev);
					id3.y = 0; // clear
				}
			}
			else if ( quantum_msg[0] == 2 ) // for ID=2
			{
				check_flag=1;
			}

			if ( quantum_msg[0] % 2 ) 
			{
				tmp3.x = x;	 	// for ID=3
				tmp3.y = y;	
				check_flag=0; 
			}
			else
			{
				tmp2.x = x;	   	// for ID=2
				tmp2.y = y;
			}
#if TRACES
				printk("Two_Touch\n");
#endif
		}
	}
	/* case.4 - Palm Touch & Unknow sate */
//	else if ( quantum_msg[0] == 14 ) // mXT224 �� �����.
	else if ( quantum_msg[0] == 12 )	// mXT140 �� �����.
	{
		if((quantum_msg[1]&0x01) == 0x00)   
		{ 
#if TRACES
			printk("[TSP] Palm Touch! - %d released, Ver. %x\n", quantum_msg[1], info_block->info_id.version );	
#endif

			id2.press=0;  // touch history clear
			id3.press=0;
		}
		else
		{
#if TRACES
			printk("[TSP] Palm Touch! - %d suppressed\n", quantum_msg[1]);	
#endif
			touch_message_flag = 1;
			//Palm ����ڵ� �߰�.
			mdelay(100);
			if ( report_all() != WRITE_MEM_OK )
			{
				printk("\n[TSP][ERROR] line : %d\n", __LINE__);
			}
		}
	}	
	else if ( quantum_msg[0] == 1 )
	{
		if ( (quantum_msg[1] & 0x10)  == 0x10)
		{
#if TRACES
			printk("[TSP] Calibrating : msg[0] = %d, msg[1] = %d\n", quantum_msg[0], quantum_msg[1] );
#endif
		}
		else if((quantum_msg[1] & 0x10) == 0x00)	
		{
#if TRACES
			printk("[TSP] msg[0] = %d, msg[1] = %d\n", quantum_msg[0], quantum_msg[1] );
#endif
		}
	}
	else if ( quantum_msg[0] == 0 )
	{
		printk("[TSP] Error : %d - What happen to TSP chip?\n", __LINE__ );

///		touch_hw_rst( 4 );  // TOUCH HW RESET No.4
	}
	else
	{
		printk("*************************************\n**********************\n");
//		printk("[TSP] Dump state, msg[0]=%d, [1]=%d\n", quantum_msg[0],  quantum_msg[1] );		
#if 0
		printk("[TSP] Dump state, msg[0]=0x%X, [1]=0x%X, [2]=0x%X, [3]=0x%X, [4]=0x%X, [5]=0x%X\n",
			 quantum_msg[0],  quantum_msg[1], quantum_msg[2],  quantum_msg[3], quantum_msg[4], quantum_msg[5] );		
#endif
/*
		for (i=0; i<5; i++)
		{
			if ( (quantum_msg[i] == 0xFF) && (quantum_msg[i+1] == 0xFF) )
			{
				printk("%s : TSP FF ERROR!!!\n", __func__);
				if (reset_chip() != WRITE_MEM_OK)                 
				{
					printk("Failed to s/w reset, exiting...\n");
				}
				msleep(100);
				break;
			}
		}
*/
	}
	
	/* if chip has recently calibrated we must confirm if the cal is good
	 * in case of calibration with a palm on the screen */
	memset( quantum_msg ,0xFF, READ_MESSAGE_LENGTH); // clear
TS_WORK_FUNC_EXIT:
	if (ts_global->use_irq)
		enable_irq(ts_global->client->irq);
	else
		hrtimer_start(&ts_global->timer, ktime_set(0,12500000), HRTIMER_MODE_REL);

//	ts_global->power(1);
//	printk("- \n");
}

/*!
 * \brief Resets the chip.
 * 
 *  This function will send a reset command to touch chip.
 *
 * @return WRITE_MEM_OK if writing the command to touch chip was successful.
 * 
 */
uint8_t reset_chip(void)
{
	uint8_t data = 1u;
	int ret = WRITE_MEM_OK;

	cal_check_flag = 1u;
	write_mem(command_processor_address + RESET_OFFSET, 1, &data);
	/* set flag for calibration lockup recovery if cal command was successful */
	//		msleep(65);
	return ret;
}

/*!
 * \brief Backup the chip.
 * 
 *  This function will send a backup command to touch chip.
 *
 * @return WRITE_MEM_OK if writing the command to touch chip was successful.
 * 
 */
uint8_t backup_chip(void)
{
	uint8_t data = 0x55u;
	return(write_mem(command_processor_address + BACKUP_OFFSET, 1, &data));
}

/*!
 * \brief Calibrates the chip.
 * 
 * This function will send a calibrate command to touch chip.
 * Whilst calibration has not been confirmed as good, this function will set
 * the ATCHCALST and ATCHCALSTHR to zero to allow a bad cal to always recover
 * 
 * @return WRITE_MEM_OK if writing the command to touch chip was successful.
 * 
 */
uint8_t calibrate_chip(void)
{
	uint8_t data = 1u;
	int ret = WRITE_MEM_OK;
	uint8_t atchcalst, atchcalsthr;

	/* change calibration suspend settings to zero until calibration confirmed good */
	/* store normal settings */
	atchcalst = config_normal.acquisition_config.atchcalst;
	atchcalsthr = config_normal.acquisition_config.atchcalsthr;

	/* resume calibration must be performed with zero settings */
	config_normal.acquisition_config.atchcalst = 0;
	config_normal.acquisition_config.atchcalsthr = 0; 

	printk("[TSP] reset acq atchcalst=%d, atchcalsthr=%d\n", config_normal.acquisition_config.atchcalst, config_normal.acquisition_config.atchcalsthr );

	/* Write temporary acquisition config to chip. */
	if (write_acquisition_config(config_normal.acquisition_config) != CFG_WRITE_OK)
	{
		/* "Acquisition config write failed!\n" */
		printk("\n[TSP][ERROR] line : %d\n", __LINE__);
		ret = WRITE_MEM_FAILED; /* calling function should retry calibration call */
	}

	/* restore settings to the local structure so that when we confirm the 
	 * cal is good we can correct them in the chip */
	/* this must be done before returning */
	config_normal.acquisition_config.atchcalst = atchcalst;
	config_normal.acquisition_config.atchcalsthr = atchcalsthr;

	/* send calibration command to the chip */
	if(ret == WRITE_MEM_OK)
	{
		/* change calibration suspend settings to zero until calibration confirmed good */
		ret = write_mem(command_processor_address + CALIBRATE_OFFSET, 1, &data);

		/* set flag for calibration lockup recovery if cal command was successful */
		if(ret == WRITE_MEM_OK)
		{ 
			/* set flag to show we must still confirm if calibration was good or bad */
			cal_check_flag = 1u;
		}
	}



	msleep(120);

	return ret;
}


static unsigned int qt_time_point=0;
static unsigned int qt_time_diff=0;
static unsigned int qt_timer_state =0;
static int check_abs_time(void)
{

	if (!qt_time_point)
		return 0;

	qt_time_diff = jiffies_to_msecs(jiffies) - qt_time_point;
	if(qt_time_diff >0)
		return 1;
	else
		return 0;
	
}

uint8_t good_check_flag = 0; 

/*!
 * \brief Used to ensure that calibration was good
 *
 * This function will check if the last calibration was good.
 * 
 * It should be called on every touch message whilst 'cal_check_flag' is set,
 * it will recalibrate the chip if the calibration is bad. If the calibration
 * is good it will restore the ATCHCALST and ATCHCALSTHR settings in the chip 
 * so that we do not get water issues.
 *
 *
 */
void check_chip_calibration(unsigned char one_touch_input_flag)
{
	uint8_t data_buffer[100] = { 0 };
	uint8_t try_ctr = 0;
	uint8_t data_byte = 0xF3; /* dianostic command to get touch flags */
	uint16_t diag_address;
	uint8_t tch_ch = 0, atch_ch = 0;
	uint8_t check_mask;
	uint8_t i;
	uint8_t j;
	uint8_t x_line_limit;
	uint8_t status;

	/* we have had the first touchscreen or face suppression message 
	 * after a calibration - check the sensor state and try to confirm if
	 * cal was good or bad */

	/* get touch flags from the chip using the diagnostic object */
	/* write command to command processor to get touch flags - 0xF3 Command required to do this */
	write_mem(command_processor_address + DIAGNOSTIC_OFFSET, 1, &data_byte);
	/* get the address of the diagnostic object so we can get the data we need */
	diag_address = get_object_address(DEBUG_DIAGNOSTIC_T37,0);

	msleep(10);  

	/* read touch flags from the diagnostic object - clear buffer so the while loop can run first time */
	memset( data_buffer , 0xFF, sizeof( data_buffer ) );

	/* wait for diagnostic object to update */
	while(!((data_buffer[0] == 0xF3) && (data_buffer[1] == 0x00)))
	{
		/* wait for data to be valid  */
		if(try_ctr > 10)
		{
			/* Failed! */
			printk("[TSP] Diagnostic Data did not update!!\n");
			qt_timer_state = 0;
			break;
		}
		msleep(2); 
		try_ctr++; /* timeout counter */
		status = read_mem(diag_address, 2,data_buffer);
		printk("[TSP] Waiting for diagnostic data to update, try %d\n", try_ctr);
	}


	/* data is ready - read the detection flags */
	status = read_mem(diag_address, 82,data_buffer);

	/* data array is 20 x 16 bits for each set of flags, 2 byte header, 40 bytes for touch flags 40 bytes for antitouch flags*/

	/* count up the channels/bits if we recived the data properly */
	if((data_buffer[0] == 0xF3) && (data_buffer[1] == 0x00))
	{

		/* mode 0 : 16 x line, mode 1 : 17 etc etc upto mode 4.*/
		x_line_limit = 16 + config_normal.cte_config.mode;
		if(x_line_limit > 20)
		{
			/* hard limit at 20 so we don't over-index the array */
			x_line_limit = 20;
		}

		/* double the limit as the array is in bytes not words */
		x_line_limit = x_line_limit << 1;

		/* count the channels and print the flags to the log */
		for(i = 0; i < x_line_limit; i+=2) /* check X lines - data is in words so increment 2 at a time */
		{
			/* print the flags to the log - only really needed for debugging */
			//printk("[TSP] Detect Flags X%d, %x%x, %x%x \n", i>>1,data_buffer[3+i],data_buffer[2+i],data_buffer[43+i],data_buffer[42+i]);

			/* count how many bits set for this row */
			for(j = 0; j < 8; j++)
			{
				/* create a bit mask to check against */
				check_mask = 1 << j;

				/* check detect flags */
				if(data_buffer[2+i] & check_mask)
				{
					tch_ch++;
				}
				if(data_buffer[3+i] & check_mask)
				{
					tch_ch++;
				}

				/* check anti-detect flags */
				if(data_buffer[42+i] & check_mask)
				{
					atch_ch++;
				}
				if(data_buffer[43+i] & check_mask)
				{
					atch_ch++;
				}
			}
		}


		/* print how many channels we counted */
		printk("[TSP] Flags Counted channels: t:%d a:%d \n", tch_ch, atch_ch);

		/* send page up command so we can detect when data updates next time,
		 * page byte will sit at 1 until we next send F3 command */
		data_byte = 0x01;
		write_mem(command_processor_address + DIAGNOSTIC_OFFSET, 1, &data_byte);

		/* process counters and decide if we must re-calibrate or if cal was good */      
		if((tch_ch>0) && (atch_ch == 0))
		{
				/* cal was good - don't need to check any more */
				if(!check_abs_time())
					qt_time_diff=501;	
	//			else
	//				dprintk(" CURRENT time diff = %d, qt_timer_state = %d\n", qt_time_diff, qt_timer_state);
				
				if(qt_timer_state == 1)
				{
					if(qt_time_diff > 500)
					{
						printk("[TSP] calibration was good\n");
						cal_check_flag = 0;
						good_check_flag = 0;
						qt_timer_state =0;
						qt_time_point = jiffies_to_msecs(jiffies);

						printk("[TSP] reset acq atchcalst=%d, atchcalsthr=%d\n", config_normal.acquisition_config.atchcalst, config_normal.acquisition_config.atchcalsthr );
						/* Write normal acquisition config back to the chip. */
						if (write_acquisition_config(config_normal.acquisition_config) != CFG_WRITE_OK)
						{
							/* "Acquisition config write failed!\n" */
							printk("\n[TSP][ERROR] line : %d\n", __LINE__);
							// MUST be fixed
						}
					}
					else 
					{
						cal_check_flag = 1;
					}
				}
				else
				{
					qt_timer_state=1;
					qt_time_point = jiffies_to_msecs(jiffies);
					cal_check_flag=1;
				}
		}
		else if(atch_ch >= 8)		//jwlee add 0325
		{
			printk("[TSP] calibration was bad\n");
			/* cal was bad - must recalibrate and check afterwards */
			calibrate_chip();
			qt_timer_state=0;
			qt_time_point = jiffies_to_msecs(jiffies);
		}
		else {
			printk("[TSP] calibration was not decided yet\n");
			/* we cannot confirm if good or bad - we must wait for next touch  message to confirm */
			cal_check_flag = 1u;
			/* Reset the 100ms timer */
			qt_timer_state=0;//0430 hugh 1 --> 0
			qt_time_point = jiffies_to_msecs(jiffies);
		}
	}
}

/*!
 * \brief Backups config area.
 * 
 * This function will send a command to backup the configuration to
 * non-volatile memory.
 * 
 * @return WRITE_MEM_OK if writing the command to touch chip was successful.
 * 
 */
uint8_t backup_config(void)
{
	/* Write 0x55 to BACKUPNV register to initiate the backup. */
	uint8_t data = 0x55u;

	return(write_mem(command_processor_address + BACKUP_OFFSET, 1, &data));
}

uint8_t report_all(void)
{
	uint8_t data = 1u;
	int ret = write_mem(command_processor_address + REPORTATLL_OFFSET, 1, &data);

	msleep(20);
	return ret;
}

/*!
 * \brief Reads the id part of info block.
 * 
 * Reads the id part of the info block (7 bytes) from touch IC to 
 * info_block struct.
 *
 */
uint8_t read_id_block(info_id_t *id)
{
	uint8_t status;

	printk("[QT] before read id->family_id : 0x%x\n",id->family_id);
	id->family_id = 0x0;
	status = read_mem(0, 1, (void *) &id->family_id);
	if (status != READ_MEM_OK)
	{
		return(status);
	}else
	{
		printk("[TSP] family_id = 0x%x\n\n", id->family_id);
	}

	status = read_mem(1, 1, (void *) &id->variant_id);
	if (status != READ_MEM_OK)
	{
		return(status);
	}else
	{
		printk("[TSP] variant_id = 0x%x\n\n", id->variant_id);
	}

	status = read_mem(2, 1, (void *) &id->version);
	if (status != READ_MEM_OK)
	{
		return(status);
	}else
	{
		printk("[TSP] version = 0x%x\n\n", id->version);
	}

	status = read_mem(3, 1, (void *) &id->build);
	if (status != READ_MEM_OK)
	{
		return(status);
	}else
	{
		printk("[TSP] build = 0x%x\n\n", id->build);
	}

	status = read_mem(4, 1, (void *) &id->matrix_x_size);
	if (status != READ_MEM_OK)
	{
		return(status);
	}else
	{
		printk("[TSP] matrix_x_size = 0x%x\n\n", id->matrix_x_size);
	}

	status = read_mem(5, 1, (void *) &id->matrix_y_size);
	if (status != READ_MEM_OK)
	{
		return(status);
	}else
	{
		printk("[TSP] matrix_y_size = 0x%x\n\n", id->matrix_y_size);
	}

	status = read_mem(6, 1, (void *) &id->num_declared_objects);
	if (status != READ_MEM_OK)
	{
		return(status);
	}else
	{
		printk("[TSP] num_declared_objects = 0x%x\n\n", id->num_declared_objects);
	}

	return(status);
}

/*!
 * \brief  This function retrieves the FW version number.
 * 
 * This function retrieves the FW version number.
 * 
 * @param *version pointer where to save version data.
 * @return ID_DATA_OK if driver is correctly initialized and 
 * id data can be read, ID_DATA_NOT_AVAILABLE otherwise.
 * 
 */
uint8_t get_version(uint8_t *version)
{
	if (info_block)
	{
		*version = info_block->info_id.version;
	}
	else
	{
		return(ID_DATA_NOT_AVAILABLE);
	}

	return (ID_DATA_OK);
}

/*!
 * \brief  This function retrieves the FW family id number.
 * 
 * This function retrieves the FW family id number.
 * 
 * @param *family_id pointer where to save family id data.
 * @return ID_DATA_OK if driver is correctly initialized and 
 * id data can be read, ID_DATA_NOT_AVAILABLE otherwise.
 * 
 */
uint8_t get_family_id(uint8_t *family_id)
{
	if (info_block)
	{
		*family_id = info_block->info_id.family_id;
	}
	else
	{
		return(ID_DATA_NOT_AVAILABLE);
	}

	return (ID_DATA_OK);
}

/*!
 * \brief  This function retrieves the FW build number.
 * 
 * This function retrieves the FW build number.
 * 
 * @param *build pointer where to save the build number data.
 * @return ID_DATA_OK if driver is correctly initialized and 
 * id data can be read, ID_DATA_NOT_AVAILABLE otherwise.
 * 
 */
uint8_t get_build_number(uint8_t *build)
{
	if (info_block)
	{
		*build = info_block->info_id.build;
	}
	else
	{
		return(ID_DATA_NOT_AVAILABLE);
	}

	return (ID_DATA_OK);
}

/*!
 * \brief  This function retrieves the FW variant number.
 * 
 * This function retrieves the FW variant id number.
 * 
 * @param *variant pointer where to save the variant id number data.
 * @return ID_DATA_OK if driver is correctly initialized and 
 * id data can be read, ID_DATA_NOT_AVAILABLE otherwise.
 * 
 */
uint8_t get_variant_id(uint8_t *variant)
{
	if (info_block)
	{
		*variant = info_block->info_id.variant_id;
	}
	else
	{
		return(ID_DATA_NOT_AVAILABLE);
	}

	return (ID_DATA_OK);
}

/*!
 * \brief Writes multitouchscreen config. 
 * 
 * 
 * This function will write the given configuration to given multitouchscreen
 * instance number.
 * 
 * @param instance the instance number of the multitouchscreen.
 * @param cfg multitouchscreen config struct.
 * @return 1 if successful.
 * 
 */
uint8_t write_multitouchscreen_config(uint8_t instance, 
		touch_multitouchscreen_t9_config_t cfg)
{
	uint16_t object_address;
	uint8_t *tmp;
	uint8_t status;
	uint8_t object_size;
	object_size = get_object_size(TOUCH_MULTITOUCHSCREEN_T9);
	if (object_size == 0)
	{
		return(CFG_WRITE_FAILED);
	}

	tmp = (uint8_t *) kmalloc(object_size, GFP_KERNEL | GFP_ATOMIC);
	if (tmp == NULL)
	{
		return(CFG_WRITE_FAILED);
	}


	/* 18 elements at beginning are 1 byte. */
	memcpy(tmp, &cfg, 18);

	/* Next two are 2 bytes. */

	*(tmp + 18) = (uint8_t) (cfg.xres &  0xFF);
	*(tmp + 19) = (uint8_t) (cfg.xres >> 8);

	*(tmp + 20) = (uint8_t) (cfg.yres &  0xFF);
	*(tmp + 21) = (uint8_t) (cfg.yres >> 8);

	/* And the last 4 1 bytes each again. */

	*(tmp + 22) = cfg.xloclip;
	*(tmp + 23) = cfg.xhiclip;
	*(tmp + 24) = cfg.yloclip;
	*(tmp + 25) = cfg.yhiclip;

	*(tmp + 26) = cfg.xedgectrl;
	*(tmp + 27) = cfg.xedgedist;
	*(tmp + 28) = cfg.yedgectrl;
	*(tmp + 29) = cfg.yedgedist;
	*(tmp + 30) = cfg.jumplimit;   // Touch move issue fix 

	object_address = get_object_address(TOUCH_MULTITOUCHSCREEN_T9, 
			instance);

	if (object_address == 0)
	{
		kfree(tmp);
		return(CFG_WRITE_FAILED);
	}

	status = write_mem(object_address, object_size, tmp);
	kfree(tmp);

	return(status);
}


/*!
 * \brief Writes GPIO/PWM config. 
 * 
 * @param instance the instance number which config to write.
 * @param cfg GPIOPWM config struct.
 * 
 * @return 1 if successful.
 * 
 */
uint8_t write_gpio_config(uint8_t instance, spt_gpiopwm_t19_config_t cfg)
{
	return(write_simple_config(SPT_GPIOPWM_T19, instance, (void *) &cfg));
}

/*!
 * \brief Writes GPIO/PWM config. 
 * 
 * @param instance the instance number which config to write.
 * @param cfg GPIOPWM config struct.
 * 
 * @return 1 if successful.
 * 
 */
uint8_t write_linearization_config(uint8_t instance, 
		proci_linearizationtable_t17_config_t cfg)
{
	uint16_t object_address;
	uint8_t *tmp;
	uint8_t status;   
	uint8_t object_size;

	object_size = get_object_size(PROCI_LINEARIZATIONTABLE_T17);
	if (object_size == 0)
	{
		return(CFG_WRITE_FAILED);
	}

	tmp = (uint8_t *) kmalloc(object_size, GFP_KERNEL | GFP_ATOMIC);

	if (tmp == NULL)
	{
		return(CFG_WRITE_FAILED);
	}


	*(tmp + 0) = cfg.ctrl;
	*(tmp + 1) = (uint8_t) (cfg.xoffset & 0x00FF);
	*(tmp + 2) = (uint8_t) (cfg.xoffset >> 8);

	memcpy((tmp+3), &cfg.xsegment, 16);

	*(tmp + 19) = (uint8_t) (cfg.yoffset & 0x00FF);
	*(tmp + 20) = (uint8_t) (cfg.yoffset >> 8);

	memcpy((tmp+21), &cfg.ysegment, 16);

	object_address = get_object_address(PROCI_LINEARIZATIONTABLE_T17, 
			instance);

	if (object_address == 0)
	{
		kfree(tmp);
		return(CFG_WRITE_FAILED);
	}

	status = write_mem(object_address, object_size, tmp);
	kfree(tmp);

	return(status);
}

/*!
 * \brief Writes two touch gesture processor config. 
 * 
 * @param instance the instance number which config to write.
 * @param cfg two touch gesture config struct.
 * 
 * @return 1 if successful.
 * 
 */
uint8_t write_twotouchgesture_config(uint8_t instance, 
		proci_twotouchgestureprocessor_t27_config_t cfg)
{
	uint16_t object_address;
	uint8_t *tmp;
	uint8_t status;   
	uint8_t object_size;

	object_size = get_object_size(PROCI_TWOTOUCHGESTUREPROCESSOR_T27);
	if (object_size == 0)
	{
		return(CFG_WRITE_FAILED);
	}

	tmp = (uint8_t *) kmalloc(object_size, GFP_KERNEL | GFP_ATOMIC);

	if (tmp == NULL)
	{
		return(CFG_WRITE_FAILED);
	}

	*(tmp + 0) = cfg.ctrl;
	*(tmp + 1) = 0;

	*(tmp + 2) = 0;
	*(tmp + 3) = cfg.gesten; 

	*(tmp + 4) = cfg.rotatethr;

	*(tmp + 5) = (uint8_t) (cfg.zoomthr & 0x00FF);
	*(tmp + 6) = (uint8_t) (cfg.zoomthr >> 8);

	object_address = get_object_address(PROCI_TWOTOUCHGESTUREPROCESSOR_T27, 
			instance);

	if (object_address == 0)
	{
		kfree(tmp);
		return(CFG_WRITE_FAILED);
	}

	status = write_mem(object_address, object_size, tmp);
	kfree(tmp);

	return(status);
}


/*!
 * \brief Writes one touch gesture processor config. 
 * 
 * @param instance the instance number which config to write.
 * @param cfg one touch gesture config struct.
 * 
 * @return 1 if successful.
 * 
 */
uint8_t write_onetouchgesture_config(uint8_t instance, 
		proci_onetouchgestureprocessor_t24_config_t cfg)
{
	uint16_t object_address;
	uint8_t *tmp;
	uint8_t status;   
	uint8_t object_size;

	object_size = get_object_size(PROCI_ONETOUCHGESTUREPROCESSOR_T24);
	if (object_size == 0)
	{
		return(CFG_WRITE_FAILED);
	}

	tmp = (uint8_t *) kmalloc(object_size, GFP_KERNEL | GFP_ATOMIC);
	if (tmp == NULL)
	{
		return(CFG_WRITE_FAILED);
	}


	*(tmp + 0) = cfg.ctrl;
	*(tmp + 1) = cfg.numgest;

	*(tmp + 2) = (uint8_t) (cfg.gesten & 0x00FF);
	*(tmp + 3) = (uint8_t) (cfg.gesten >> 8);

	*(tmp + 4) = (uint8_t) cfg.pressproc;
	*(tmp + 5) = (uint8_t) cfg.tapto;
	*(tmp + 6) = (uint8_t) cfg.flickto;
	*(tmp + 7) = (uint8_t) cfg.dragto;
	*(tmp + 8) = (uint8_t) cfg.spressto;
	*(tmp + 9) = (uint8_t) cfg.lpressto;
	*(tmp + 10) = (uint8_t) cfg.rptpressto;

	*(tmp + 11) = (uint8_t) (cfg.flickthr & 0x00FF);
	*(tmp + 12) = (uint8_t) (cfg.flickthr >> 8);

	*(tmp + 13) = (uint8_t) (cfg.dragthr & 0x00FF);
	*(tmp + 14) = (uint8_t) (cfg.dragthr >> 8);

	*(tmp + 15) = (uint8_t) (cfg.tapthr & 0x00FF);
	*(tmp + 16) = (uint8_t) (cfg.tapthr >> 8);

	*(tmp + 17) = (uint8_t) (cfg.throwthr & 0x00FF);
	*(tmp + 18) = (uint8_t) (cfg.throwthr >> 8);

	object_address = get_object_address(PROCI_ONETOUCHGESTUREPROCESSOR_T24, 
			instance);

	if (object_address == 0)
	{
		kfree(tmp);
		return(CFG_WRITE_FAILED);
	}

	status = write_mem(object_address, object_size, tmp);
	kfree(tmp);

	return(status);
}

/*!
 * \brief Writes selftest config. 
 * 
 * @param instance the instance number which config to write.
 * @param cfg selftest config struct.
 * 
 * @return 1 if successful.
 * 
 */
uint8_t write_selftest_config(uint8_t instance, 
		spt_selftest_t25_config_t cfg)
{
	uint16_t object_address;
	uint8_t *tmp;
	uint8_t status;   
	uint8_t object_size;

	object_size = get_object_size(SPT_SELFTEST_T25);
	if (object_size == 0)
	{
		return(CFG_WRITE_FAILED);
	}

	tmp = (uint8_t *) kmalloc(object_size, GFP_KERNEL | GFP_ATOMIC);


	if (tmp == NULL)
	{
		return(CFG_WRITE_FAILED);
	}

	*(tmp + 0) = cfg.ctrl;
	*(tmp + 1) = cfg.cmd;

	*(tmp + 2) = (uint8_t) (cfg.siglim[0].upsiglim & 0x00FF);
	*(tmp + 3) = (uint8_t) (cfg.siglim[0].upsiglim >> 8);
	*(tmp + 4) = (uint8_t) (cfg.siglim[0].losiglim & 0x00FF);
	*(tmp + 5) = (uint8_t) (cfg.siglim[0].losiglim >> 8);

	*(tmp + 6) = (uint8_t) (cfg.siglim[1].upsiglim & 0x00FF);
	*(tmp + 7) = (uint8_t) (cfg.siglim[1].upsiglim >> 8);
	*(tmp + 8) = (uint8_t) (cfg.siglim[1].losiglim & 0x00FF);
	*(tmp + 9) = (uint8_t) (cfg.siglim[1].losiglim >> 8);

	*(tmp + 10) = (uint8_t) (cfg.siglim[2].upsiglim & 0x00FF);
	*(tmp + 11) = (uint8_t) (cfg.siglim[2].upsiglim >> 8);
	*(tmp + 12) = (uint8_t) (cfg.siglim[2].losiglim & 0x00FF);
	*(tmp + 13) = (uint8_t) (cfg.siglim[2].losiglim >> 8);
//xxx
	printk("[TSP] %s, %d\n", __func__, cfg.ctrl );
	printk("[TSP] %s, %d\n", __func__, cfg.cmd);
	printk("[TSP] %s, %d\n", __func__, cfg.siglim[0].upsiglim);
	printk("[TSP] %s, %d\n", __func__, cfg.siglim[0].losiglim);

	object_address = get_object_address(SPT_SELFTEST_T25, 
			instance);

	if (object_address == 0)
	{
		kfree(tmp);
		return(CFG_WRITE_FAILED);
	}

	status = write_mem(object_address, object_size, tmp);
	kfree(tmp);

	return(status);
}

/*!
 * \brief Writes key array config. 
 * 
 * @param instance the instance number which config to write.
 * @param cfg key array config struct.
 * 
 * @return 1 if successful.
 * 
 */
uint8_t write_keyarray_config(uint8_t instance, 
		touch_keyarray_t15_config_t cfg)
{
	return(write_simple_config(TOUCH_KEYARRAY_T15, instance, (void *) &cfg));
}

/*!
 * \brief Writes grip suppression config. 
 * 
 * @param instance the instance number indicating which config to write.
 * @param cfg grip suppression config struct.
 * 
 * @return 1 if successful.
 * 
 */
uint8_t write_gripsuppression_config(uint8_t instance, 
		proci_gripfacesuppression_t20_config_t cfg)
{
	return(write_simple_config(PROCI_GRIPFACESUPPRESSION_T20, instance, 
				(void *) &cfg));
}

/*!
 * \brief Writes a simple config struct to touch chip.
 *
 * Writes a simple config struct to touch chip. Does not necessarily 
 * (depending on how structs are stored on host platform) work for 
 * configs that have multibyte variables, so those are handled separately.
 * 
 * @param object_type object id number.
 * @param instance object instance number.
 * @param cfg pointer to config struct.
 * 
 * @return 1 if successful.
 * 
 */
uint8_t write_simple_config(uint8_t object_type, 
		uint8_t instance, void *cfg)
{
	uint16_t object_address;
	uint8_t object_size;

	object_address = get_object_address(object_type, instance);
	object_size = get_object_size(object_type);

	if ((object_size == 0) || (object_address == 0))
	{
		return(CFG_WRITE_FAILED);
	}

	return (write_mem(object_address, object_size, cfg));
}


/*!
 * \brief Writes power config. 
 * 
 * @param cfg power config struct.
 * 
 * @return 1 if successful.
 * 
 */
uint8_t write_power_config(gen_powerconfig_t7_config_t cfg)
{
	return(write_simple_config(GEN_POWERCONFIG_T7, 0, (void *) &cfg));
}

uint8_t write_userdata_config(userdata_t38_config_t cfg)
{
	return(write_simple_config(SPT_USERDATA_T38, 0, (void *) &cfg));
}

uint8_t write_comms_config(spt_commsconfig_t18_config_t cfg)
{
	return(write_simple_config(SPT_COMCONFIG_T18, 0, (void *) &cfg));
}
#if 0
uint8_t write_proximity_config(proximity_t23_config_t cfg)
{
	return(write_simple_config(TOUCH_PROXIMITY_T23, 0, (void *) &cfg));
}
#endif
/*!
 * \brief Writes noise suppression config.
 *
 * @param instance the instance number indicating which config to write.
 * @param cfg noise suppression config struct.
 *
 * @return 1 if successful.
 *
 */

uint8_t write_noisesuppression_config(uint8_t instance,
		procg_noisesuppression_t22_config_t cfg)
{
	uint16_t object_address;
	uint8_t *tmp;
	uint8_t status;
	uint8_t object_size;
	object_size = get_object_size(PROCG_NOISESUPPRESSION_T22);
	if (object_size == 0)
	{
		return(CFG_WRITE_FAILED);
	}

	tmp = (uint8_t *) kmalloc(object_size, GFP_KERNEL | GFP_ATOMIC);
	if (tmp == NULL)
	{
		return(CFG_WRITE_FAILED);
	}

	*(tmp + 0) = (uint8_t) cfg.ctrl;
	*(tmp + 1) = (uint8_t) cfg.reserved;
	*(tmp + 2) = (uint8_t) cfg.reserved1;
	*(tmp + 3) = (uint8_t) (cfg.gcaful &  0xFF);
	*(tmp + 4) = (uint8_t) (cfg.gcaful >> 8);

	*(tmp + 5) = (uint8_t) (cfg.gcafll &  0xFF);
	*(tmp + 6) = (uint8_t) (cfg.gcafll >> 8);

	*(tmp + 7) = cfg.actvgcafvalid;
	*(tmp + 8) = cfg.noisethr;
	*(tmp + 9) = cfg.reserved2;
	*(tmp + 10) = cfg.freqhopscale;

	*(tmp + 11) = cfg.freq[0];
	*(tmp + 12) = cfg.freq[1];
	*(tmp + 13) = cfg.freq[2];
	*(tmp + 14) = cfg.freq[3];
	*(tmp + 15) = cfg.freq[4];

	*(tmp + 16) = cfg.idlegcafvalid;

	object_address = get_object_address(PROCG_NOISESUPPRESSION_T22, 
			instance);

	if (object_address == 0)
	{
		kfree(tmp);
		return(CFG_WRITE_FAILED);
	}

	status = write_mem(object_address, object_size, tmp);
	kfree(tmp);

	return(status);
}

/*!
 * \brief Writes CTE config. 
 * 
 * Writes CTE config, assumes that there is only one instance of 
 * CTE config objects.
 *
 * @param cfg CTE config struct.
 * 
 * @return 1 if successful.
 * 
 */
uint8_t write_CTE_config(spt_cteconfig_t28_config_t cfg)
{
	return(write_simple_config(SPT_CTECONFIG_T28, 0, (void *) &cfg));
}


/*!
 * \brief Writes acquisition config. 
 * 
 * @param cfg acquisition config struct.
 * 
 * @return 1 if successful.
 * 
 */
uint8_t write_acquisition_config(gen_acquisitionconfig_t8_config_t cfg)
{
	return(write_simple_config(GEN_ACQUISITIONCONFIG_T8, 0, (void *) &cfg));
}



uint8_t write_proximity_config(uint8_t instance, touch_proximity_t23_config_t cfg)
{
	uint16_t object_address;
	uint8_t *tmp;
	uint8_t status;
	uint8_t object_size;

	object_size = get_object_size(TOUCH_PROXIMITY_T23);
	if (object_size == 0)
	{
		return(CFG_WRITE_FAILED);
	}
	tmp = (uint8_t *) kmalloc(object_size, GFP_KERNEL | GFP_ATOMIC);
	if (tmp == NULL)
	{
		return(CFG_WRITE_FAILED);
	}

	memset(tmp,0,object_size);

	*(tmp + 0) = cfg.ctrl;
	*(tmp + 1) = cfg.xorigin;
	*(tmp + 2) = cfg.yorigin;
	*(tmp + 3) = cfg.xsize;
	*(tmp + 4) = cfg.ysize;
	*(tmp + 5) = cfg.reserved;
	*(tmp + 6) = cfg.blen;
	*(tmp + 7) = (uint8_t) (cfg.fxddthr & 0x00FF);
	*(tmp + 8) = (uint8_t) (cfg.fxddthr >> 8);

	*(tmp + 9) = cfg.fxddi;
	*(tmp + 10) = cfg.average;

	*(tmp + 11) = (uint8_t) (cfg.mvnullrate & 0x00FF);
	*(tmp + 12) = (uint8_t) (cfg.mvnullrate >> 8);
	*(tmp + 13) = (uint8_t) (cfg.mvdthr & 0x00ff);
	*(tmp + 14) = (uint8_t) (cfg.mvdthr >> 8);

	object_address = get_object_address(TOUCH_PROXIMITY_T23,
			instance);

	if (object_address == 0)
	{
		kfree(tmp);
		return(CFG_WRITE_FAILED);
	}

	status = write_mem(object_address, object_size, tmp);
	kfree(tmp);

	return(status);
}


/*!
 * \brief Returns the start address of the selected object.
 * 
 * Returns the start address of the selected object and instance 
 * number in the touch chip memory map.  
 *
 * @param object_type the object ID number.
 * @param instance the instance number of the object.
 * 
 * @return object address, or OBJECT_NOT_FOUND if object/instance not found.
 * 
 */
uint16_t get_object_address(uint8_t object_type, uint8_t instance)
{
	object_t obj;
	uint8_t object_table_index = 0;
	uint8_t address_found = 0;
	uint16_t address = OBJECT_NOT_FOUND;

	object_t *object_table;
	object_table = info_block->objects;

	while ((object_table_index < info_block->info_id.num_declared_objects) && !address_found)
	{
		obj = object_table[object_table_index];
		/* Does object type match? */
		if (obj.object_type == object_type)
		{

			address_found = 1;

			/* Are there enough instances defined in the FW? */
			if (obj.instances >= instance)
			{
				address = obj.i2c_address + (obj.size + 1) * instance;
			}
		}
		object_table_index++;
	}

	return(address);
}


/*!
 * \brief Returns the size of the selected object in the touch chip memory map.
 * 
 * Returns the size of the selected object in the touch chip memory map.
 *
 * @param object_type the object ID number.
 * 
 * @return object size, or OBJECT_NOT_FOUND if object not found.
 * 
 */
uint8_t get_object_size(uint8_t object_type)
{
	object_t obj;
	uint8_t object_table_index = 0;
	uint8_t object_found = 0;
	//uint16_t 
	uint8_t size = OBJECT_NOT_FOUND;

	object_t *object_table;
	object_table = info_block->objects;

	while ((object_table_index < info_block->info_id.num_declared_objects) &&
			!object_found)
	{
		obj = object_table[object_table_index];
		/* Does object type match? */
		if (obj.object_type == object_type)
		{
			object_found = 1;
			size = obj.size + 1;
		}
		object_table_index++;
	}

	return(size);
}


/*!
 * \brief Return report id of given object/instance.
 * 
 *  This function will return a report id corresponding to given object type
 *  and instance, or 
 * 
 * @param object_type the object type identifier.
 * @param instance the instance of object.
 * 
 * @return report id, or 255 if the given object type does not have report id,
 * of if the firmware does not support given object type / instance.
 * 
 */
uint8_t type_to_report_id(uint8_t object_type, uint8_t instance)
{
	uint8_t report_id = 1;
	int8_t report_id_found = 0;

	while((report_id <= max_report_id) && (report_id_found == 0))
	{
		if((report_id_map[report_id].object_type == object_type) &&
				(report_id_map[report_id].instance == instance))
		{
			report_id_found = 1;
		}
		else
		{
			report_id++;	
		}
	}
	if (report_id_found)
	{
		return(report_id);
	}
	else
	{
		return(ID_MAPPING_FAILED);
	}
}

/*!
 * \brief Maps report id to object type and instance.
 * 
 *  This function will return an object type id and instance corresponding
 *  to given report id.
 * 
 * @param report_id the report id.
 * @param *instance pointer to instance variable. This function will set this
 *        to instance number corresponding to given report id.
 * @return the object type id, or 255 if the given report id is not used
 * at all.
 * 
 */
uint8_t report_id_to_type(uint8_t report_id, uint8_t *instance)
{
	if (report_id <= max_report_id)
	{
		*instance = report_id_map[report_id].instance;
		return(report_id_map[report_id].object_type);
	}
	else
	{
		return(ID_MAPPING_FAILED);
	}
}

/*! 
 * \brief Return the maximum report id in use in the touch chip.
 * 
 * @return maximum_report_id 
 * 
 */
uint8_t get_max_report_id(void)
{
	return (max_report_id);
}


/*------------------------------ I2C Driver block -----------------------------------*/



int qt602240_i2c_write(u16 reg, u8 *read_val, unsigned int len)
{
	struct i2c_msg wmsg;
	unsigned char data[I2C_MAX_SEND_LENGTH];
	int ret,i;

	if(len+2 > I2C_MAX_SEND_LENGTH)
	{
		printk("[TSP][ERROR] %s() data length error\n", __FUNCTION__);
		return -ENODEV;
	}

	wmsg.addr = ts_global->client->addr;
	wmsg.flags = I2C_M_WR;
	wmsg.len = len + 2;
	wmsg.buf = data;

	data[0] = reg & 0x00ff;
	data[1] = reg >> 8;

	for (i = 0; i < len; i++)
	{
		data[i+2] = *(read_val+i);
	}

	ret = i2c_transfer(ts_global->client->adapter, &wmsg, 1);
	if ( ret < 0 )
	{
		printk("[TSP] Error code : %d\n", __LINE__ );
///		touch_hw_rst( 5 );  // TOUCH HW RESET No.5
		return -1;
	}

	return ret;
}

int qt602240_i2c_read(u16 reg,unsigned char *rbuf, int buf_size)
{
	struct i2c_msg rmsg;
	int ret=-1;
	unsigned char data[2];


//	printk("[TSP] %s addr=%X \n",__func__ , ts_global->client->addr );

	rmsg.addr = ts_global->client->addr;
	rmsg.flags = I2C_M_WR;
	rmsg.len = 2;
	rmsg.buf = data;
	data[0] = reg & 0x00ff;
	data[1] = reg >> 8;
	ret = i2c_transfer(ts_global->client->adapter, &rmsg, 1);

	//	printk("%s, %d, %d = ",__func__,data[0], data[1]);

	if(ret>=0) {
		rmsg.flags = I2C_M_RD;
		rmsg.len = buf_size;
		rmsg.buf = rbuf;
		ret = i2c_transfer(ts_global->client->adapter, &rmsg, 1);
	}
	else 
	{
		printk("[TSP] Error code : %d\n", __LINE__ );
///		touch_hw_rst( 6 );  // TOUCH HW RESET No.6
		return -1;
	}

	return ret;
}
/* ------------------------- ????????????? -----------------*/
/*!
 * \brief Initializes the I2C interface.
 *
 * @param I2C_address_arg the touch chip I2C address.
 *
 */

/*! \brief Maxtouch Memory read by I2C bus */
U8 read_mem(U16 start, U8 size, U8 *mem)
{
	//	char *read_buf;
	int ret;

	//   read_buf = (char *)kmalloc(size, GFP_KERNEL | GFP_ATOMIC);
	memset(mem,0xFF,size);
	ret = qt602240_i2c_read(start,mem,size);
	if(ret < 0) {
		printk("%s, %d : i2c read failed\n",__func__, __LINE__);
		return(READ_MEM_FAILED);
	} 

	return(READ_MEM_OK);
}

U8 read_U16(U16 start, U16 *mem)
{
//	U8 tmp;
	U8 status;

	status = read_mem(start, 2, (U8 *) mem);

	return (status);
}

U8 write_mem(U16 start, U8 size, U8 *mem)
{
	int ret;

	ret = qt602240_i2c_write(start,mem,size);
	if(ret < 0) 
		return(WRITE_MEM_FAILED);
	else
		return(WRITE_MEM_OK);
}


/*
 * Message handler that is called by the touch chip driver when messages
 * are received.
 * 
 * This example message handler simply prints the messages as they are
 * received to USART1 port of EVK1011 board.
 */

/*------------------------------ CRC calculation  block -----------------------------------*/
/*!
 * \brief Returns the stored CRC sum for the info block & object table area.
 *
 * @return the stored CRC sum for the info block & object table.
 *
 */
 /*
uint32_t get_stored_infoblock_crc(void)
{
	uint32_t crc;

	crc = (uint32_t) (((uint32_t) info_block->CRC_hi) << 16);
	crc = crc | info_block->CRC;
	return(crc);
}
*/

/*!
 * \brief Calculates the CRC sum for the info block & object table area,
 * and checks it matches the stored CRC.
 *
 * Global interrupts need to be on when this function is called
 * since it reads the info block & object table area from the touch chip.
 *
 * @param *crc_pointer Pointer to memory location where
 *        the calculated CRC sum for the info block & object
 *        will be stored.
 * @return the CRC_CALCULATION_FAILED if calculation doesn't succeed, of
 *         CRC_CALCULATION_OK otherwise.
 *
 */
uint8_t calculate_infoblock_crc(uint32_t *crc_pointer)
{
	uint32_t crc = 0;
	//uint16_t 
	uint8_t crc_area_size;
	uint8_t *mem;
	uint8_t i;
	uint8_t status;
	/* 7 bytes of version data, 6 * NUM_OF_OBJECTS bytes of object table. */
	crc_area_size = 7 + info_block->info_id.num_declared_objects * 6;

	mem = (uint8_t *) kmalloc(crc_area_size, GFP_KERNEL | GFP_ATOMIC);
	if (mem == NULL)
	{
		return(CRC_CALCULATION_FAILED);
	}

	status = read_mem(0, crc_area_size, mem);

	if (status != READ_MEM_OK)
	{
		return(CRC_CALCULATION_FAILED);
	}

	i = 0;
	while (i < (crc_area_size - 1))
	{
		crc = CRC_24(crc, *(mem + i), *(mem + i + 1));
		i += 2;
	}

	crc = CRC_24(crc, *(mem + i), 0);

	/* Return only 24 bit CRC. */
	*crc_pointer = (crc & 0x00FFFFFF);
	return(CRC_CALCULATION_OK);
}

/*!
 * \brief CRC calculation routine.
 *
 * @param crc the current CRC sum.
 * @param byte1 1st byte of new data to add to CRC sum.
 * @param byte2 2nd byte of new data to add to CRC sum.
 * @return crc the new CRC sum.
 *
 */
uint32_t CRC_24(uint32_t crc, uint8_t byte1, uint8_t byte2)
{
	static const uint32_t crcpoly = 0x80001B;
	uint32_t result;
	uint16_t data_word;

	data_word = (uint16_t) ((uint16_t) (byte2 << 8u) | byte1);
	result = ((crc << 1u) ^ (uint32_t) data_word);

	if (result & 0x1000000)
	{
		result ^= crcpoly;
	}

	return(result);
}

static enum hrtimer_restart qt602240_ts_timer_func(struct hrtimer *timer)
{
	struct qt602240_ts_data *ts = container_of(timer, struct qt602240_ts_data, timer);
#if TRACES
	printk("qt602240_ts_timer_func\n"); 
#endif

	queue_work(qt602240_wq, &ts->work);

	hrtimer_start(&ts_global->timer, ktime_set(0, 12500000), HRTIMER_MODE_REL);
	return HRTIMER_NORESTART;
}


static irqreturn_t qt602240_ts_irq_handler(int irq, void *dev_id)
{
	struct qt602240_ts_data *ts = dev_id;

#if TRACES
	printk("qt602240_ts_irq_handler\n");
#endif

	disable_irq_nosync(ts_global->client->irq);
	queue_work(qt602240_wq, &ts->work);
	return IRQ_HANDLED;
}
struct device *ts_dev;
/* -------------------------------- sys fs - end -------------------------------- */


static int qt602240_ts_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int ret = 0;
	struct qt602240_ts_data *ts;

	printk("[TSP] %s, %d\n", __func__, __LINE__ );

	
	tsp_cl = client;	// hsil

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		printk(KERN_ERR "qt602240_ts_probe: need I2C_FUNC_I2C\n");
		ret = -ENODEV;
		goto err_check_functionality_failed;
	}

	ts = kzalloc(sizeof(*ts), GFP_KERNEL);
	if (ts == NULL) {
		ret = -ENOMEM;
		goto err_alloc_data_failed;
	}

	qt_time_point = jiffies_to_msecs(jiffies);
	
	INIT_WORK(&ts->work, qt602240_ts_work_func);
	ts->client = client;
	ts->power = ((struct qt602240_ts_data *)(client->dev.platform_data))->power;
	i2c_set_clientdata(client, ts);

	if (ts->power) {
		ret = ts->power(1);
		if (ret < 0) {
			printk(KERN_ERR "qt602240_ts_probe power on failed\n");
			goto err_power_failed;
		}
	}
	msleep(100);

	ts_global = ts;

	ts->input_dev = input_allocate_device();
	if (ts->input_dev == NULL) {
		ret = -ENOMEM;
		printk(KERN_ERR "qt602240_ts_probe: Failed to allocate input device\n");
		goto err_input_dev_alloc_failed;
	}
	ts->input_dev->name = "quantom-touchscreen";
	set_bit(EV_SYN, ts->input_dev->evbit);
	set_bit(EV_KEY, ts->input_dev->evbit);
	set_bit(EV_ABS, ts->input_dev->evbit);

	printk(KERN_INFO "qt602240_ts_probe: max_x: %d, max_y: %d\n", MAX_X, MAX_Y);
	input_set_abs_params(ts->input_dev, ABS_MT_POSITION_X, 0, MAX_X, 0, 0);
	input_set_abs_params(ts->input_dev, ABS_MT_POSITION_Y, 0, MAX_Y, 0, 0);
	input_set_abs_params(ts->input_dev, ABS_MT_TOUCH_MAJOR, 0, 255, 0, 0);
	input_set_abs_params(ts->input_dev, ABS_MT_WIDTH_MAJOR, 0, 15, 0, 0);

	/* ts->input_dev->name = ts->keypad_info->name; */
	ret = input_register_device(ts->input_dev);
	if (ret) {
		printk(KERN_ERR "qt602240_ts_probe: Unable to register %s input device\n", ts->input_dev->name);
		goto err_input_register_device_failed;
	}

	if(quantum_touch_probe() != 0){
		printk("quantum_touch_probe failed\n");
		goto err_quantum_touch_probe;
	}
printk("((((((((((((((((((((    irq = %d    )))))))))))))))))))", client->irq);

//	client->irq = 0;
	VrpInit(ts->input_dev, NULL);
	if (client->irq) {
//		ret = request_irq(client->irq, qt602240_ts_irq_handler, IRQF_DISABLED , client->name, ts);
		ret = request_irq(client->irq, qt602240_ts_irq_handler, IRQF_TRIGGER_LOW , client->name, ts);
		if (ret == 0){
			ts->use_irq = 1;
			ts_global->use_irq = 1;
		}else{
			dev_err(&client->dev, "request_irq failed\n");
			goto err_request_irq;
		}	
	}else{

//	printk("[TSP][end] %s, irq=%d\n", __func__, client->irq );

		hrtimer_init(&ts_global->timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
		ts_global->timer.function = qt602240_ts_timer_func;
		hrtimer_start(&ts_global->timer, ktime_set(1, 0), HRTIMER_MODE_REL);
	}
#ifdef CONFIG_HAS_EARLYSUSPEND
	ts->early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 1;
	ts->early_suspend.suspend = qt602240_ts_early_suspend;
	ts->early_suspend.resume  = qt602240_ts_late_resume;
	register_early_suspend(&ts->early_suspend);
#endif

	printk(KERN_INFO "qt602240_ts_probe: Start touchscreen %s in %s mode , irq = %d\n", ts->input_dev->name, ts_global->use_irq ? "interrupt" : "polling", client->irq);
#if 0
	INT_clear();	
	ts->power(1);
	qt602240_ts_irq_handler(0, ts_global);
//	qt602240_ts_suspend(client, PMSG_SUSPEND);

//	qt602240_ts_resume(client);

	disable_irq(client->irq);
	enable_irq(client->irq);
	qt602240_ts_irq_handler(0, ts_global);
#endif
	return 0;
err_request_irq:
err_input_register_device_failed:
	input_free_device(ts->input_dev);
err_quantum_touch_probe:
err_input_dev_alloc_failed:
	if (ts->power) {
		ret = ts->power(0);
	}
//err_detect_failed:
err_power_failed:
	kfree(ts);
err_alloc_data_failed:
err_check_functionality_failed:
	return ret;
}

static int qt602240_ts_remove(struct i2c_client *client)
{
	struct qt602240_ts_data *ts = i2c_get_clientdata(client);
#ifdef CONFIG_HAS_EARLYSUSPEND
	unregister_early_suspend(&ts->early_suspend);
#endif	/* CONFIG_HAS_EARLYSUSPEND */
	if(ts_global->use_irq){
		free_irq(ts_global->client->irq, 0);
	}else{
		hrtimer_cancel(&ts_global->timer);
	}
	input_unregister_device(ts->input_dev);
	kfree(ts);

	return 0;
}

static int qt602240_ts_suspend(struct i2c_client *client, pm_message_t mesg)
{
	struct qt602240_ts_data *ts = i2c_get_clientdata(client);
	gen_powerconfig_t7_config_t power_config_sleep = {0};

	ENTER_FUNC;

//	if( touch_present )
//	{
		if (ts_global->use_irq)
			disable_irq(client->irq);
		else
			hrtimer_cancel(&ts_global->timer);

		qt_timer_state = 0;
		driver_setup = DRIVER_SETUP_INCOMPLETE;
		config_set_enable = 0; // 0 for disable, 1 for enable

		id2.press=0;  // touch history clear
		id3.press=0;

		/* Write power config to chip. */
		if (write_power_config(power_config_sleep) != CFG_WRITE_OK)
		{
			/* "Power config write failed!\n" */
			printk("\n[TSP][ERROR] line : %d\n", __LINE__);
		}

//	}

	//++    
	ts->power( 0 );
	//--    

	LEAVE_FUNC;

	return 0;
}

static int qt602240_ts_resume(struct i2c_client *client)
{
	struct qt602240_ts_data *ts = i2c_get_clientdata(client);
	gen_powerconfig_t7_config_t power_config_sleep = {0};

	//boot_reset();
	ENTER_FUNC;
	if(1)
	{
		ts->power(1);
		msleep(100);
		//--

		config_set_enable = 1; // 0 for disable, 1 for enable
		power_config_sleep.idleacqint = 32;//255;
		power_config_sleep.actvacqint = 16;//255;
		power_config_sleep.actv2idleto = 50;

		/* Write power config to chip. */
		if (write_power_config(power_config_sleep) != CFG_WRITE_OK)
		{
			/* "Power config write failed!\n" */
			printk("\n[TSP][ERROR] line : %d\n", __LINE__);
			msleep(50);
			if (write_power_config(power_config_sleep) != CFG_WRITE_OK)
			{
				/* "Power config write failed!\n" */
				printk("\n[TSP][ERROR] line : %d\n", __LINE__);
				//			touch_hw_rst( 7 );  // TOUCH HW RESET No.7
			}
		}

		good_check_flag=0;
		mdelay(20);
		if ( gpio_get_value(TSP_INT) )
		{
			printk("[TSP] %d - INT is High!!\n", __LINE__ );
			mdelay(20);
			if ( gpio_get_value(TSP_INT) )
			{	
				printk("[TSP] %d - INT is High!! Twice!\n", __LINE__ );
				mdelay(30);
				if ( gpio_get_value(TSP_INT) )
				{	
					printk("[TSP] Error : %d - INT is High!! Three!\n", __LINE__ );
					//				touch_hw_rst( 10 );  // TOUCH HW RESET No.10
				}
			}
		}
		{
			set_tsp_for_tunning_param(1);
		}

		printk("[TSP] Dump state, last rst=%d\n#1=%d,#2=%d,#3=%d,#4=%d,#5=%d,#6=%d,#7=%d,#8=%d,#9=%d,#10=%d\n",
				rst_cnt[0],rst_cnt[1],rst_cnt[2],rst_cnt[3],rst_cnt[4],rst_cnt[5],
				rst_cnt[6],rst_cnt[7],rst_cnt[8],rst_cnt[9],rst_cnt[10] );

		driver_setup = DRIVER_SETUP_OK;
		if (!ts_global->use_irq)
			hrtimer_start(&ts_global->timer, ktime_set(1, 0), HRTIMER_MODE_REL);
		else
			enable_irq(ts_global->client->irq);
		INT_clear( );

	}

	LEAVE_FUNC;

	return 0;
}

#ifdef CONFIG_HAS_EARLYSUSPEND
static void qt602240_ts_early_suspend(struct early_suspend *h)
{
	struct qt602240_ts_data *ts;
	ts = container_of(h, struct qt602240_ts_data, early_suspend);
	tsp_off = 1;
	qt602240_ts_suspend(ts->client, PMSG_SUSPEND);
}

static void qt602240_ts_late_resume(struct early_suspend *h)
{
	struct qt602240_ts_data *ts;
	ts = container_of(h, struct qt602240_ts_data, early_suspend);
	tsp_off = 0;
	qt602240_ts_resume(ts->client);
}
#endif

void tsp_recover(void)
{
	qt602240_ts_resume(tsp_cl);
}
EXPORT_SYMBOL(tsp_recover);

static const struct i2c_device_id qt602240_ts_id[] = {
//	{ "qt602240-ts", 1 },
	{ MXT140_NAME, 1 },
	{ }
};

static struct i2c_driver qt602240_ts_driver = {
	.probe		= qt602240_ts_probe,
	.remove		= qt602240_ts_remove,
#ifndef CONFIG_HAS_EARLYSUSPEND
	.suspend	= qt602240_ts_suspend,
	.resume		= qt602240_ts_resume,
#endif
	.id_table	= qt602240_ts_id,
	.driver = {
		.name	= MXT140_NAME,
	},
};

static int __devinit qt602240_ts_init(void)
{
	printk("[TSP] %s\n", __func__ );
	qt602240_wq = create_singlethread_workqueue("qt602240_wq");
	if (!qt602240_wq)
		return -ENOMEM;
	return i2c_add_driver(&qt602240_ts_driver);
}

static void __exit qt602240_ts_exit(void)
{
	i2c_del_driver(&qt602240_ts_driver);
	if (qt602240_wq)
		destroy_workqueue(qt602240_wq);
}


module_init(qt602240_ts_init);
module_exit(qt602240_ts_exit);

MODULE_DESCRIPTION("Quantum Touchscreen Driver");
MODULE_LICENSE("GPL");




