#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/charge_pump.h>

#define CLIP_VALUE(g,l,h)  {                        \
                                if ((g) > (h))      \
                                {                   \
                                    (g) = (h);      \
                                }                   \
                                else if ((g) < (l)) \
                                {                   \
                                    (g) = (l);      \
                                }                   \
                            }

#define KTD_DEF_MAX_LV (32)

typedef struct {
    uint Id;
    uint MaxLv; //lightness level
    uint DesLv; //lightness level
    uint CurLv; //lightness level
    struct platform_device *Device;
} KtdPumpRes_t;

static KtdPumpRes_t KtdRes = {
    .MaxLv = KTD_DEF_MAX_LV,
};

static DEFINE_SPINLOCK(atom_lock);

extern void msm_gpio_find_out(const unsigned gpio, void __iomem **out,	unsigned *offset);
extern void msm_gpio_set_bit(unsigned n, void *regs);
extern void msm_gpio_clr_bit(unsigned n, void *regs);

static void Ktdc(KtdPumpRes_t *pstRes, u32 n) {
	void __iomem *reg;
    unsigned long irq_flags;
    u32 i, offset;
    
    msm_gpio_find_out(pstRes->Id, &reg, &offset);
    
    printk("Ktdc n:%d\n", n);

    spin_lock_irqsave(&atom_lock, irq_flags);

    for (i = 0; i < n; i++) {
        msm_gpio_set_bit(offset, reg);
        udelay(10);
        msm_gpio_clr_bit(offset, reg);
        udelay(10);
    }
    
    msm_gpio_set_bit(offset, reg);
    spin_unlock_irqrestore(&atom_lock, irq_flags);
}

uint Ktd259Lv(uint Lv) {    
    uint Bank;

    CLIP_VALUE(Lv, 0, 100);
    Bank = Lv * KtdRes.MaxLv / 100;
    CLIP_VALUE(Bank, 1, KtdRes.MaxLv);
    return Bank;
}

void Ktd259Set(KtdPumpRes_t *pstRes) {
    uint Delta;

    printk("CurLv:%d, DesLv:%d\n", pstRes->CurLv, pstRes->DesLv);

    // Notes:
    // Lv is lightness level
    // Delta is IC pulse number
    // Min of the Lv map to Max of the Delta, they are reverse.
    if (pstRes->DesLv > pstRes->CurLv) {
        Delta = pstRes->MaxLv - (pstRes->DesLv - pstRes->CurLv);
        Ktdc(pstRes, Delta);
    }
    else if (pstRes->DesLv < pstRes->CurLv) {
        Delta = pstRes->CurLv - pstRes->DesLv;
        Ktdc(pstRes, Delta);
    }
    else {
        gpio_set_value(pstRes->Id, 1);
    }
    pstRes->CurLv = pstRes->DesLv;
}

void Ktd259PowerOn(KtdPumpRes_t *pstRes) {
    gpio_set_value(pstRes->Id, 1);
    pstRes->CurLv = pstRes->DesLv = pstRes->MaxLv;
    udelay(500);
}

void Ktd259PowerOff(KtdPumpRes_t *pstRes) {
    
    gpio_set_value(pstRes->Id, 0);
    mdelay(4);
}

static void _ChargePumpPowerOn(void) {
    Ktd259PowerOn(&KtdRes);
}

static void _ChargePumpPowerOff(void) {
    Ktd259PowerOff(&KtdRes);
}

static void _ChargePumpSetDispLightLv(uint Lv) {
    KtdRes.DesLv = Ktd259Lv(Lv);
    Ktd259Set(&KtdRes);
}

static void _ChargePumpSetKeyLightLv(uint Lv) {
}

static ChargePumpIf_t _ChargePumpIf = {
    .PowerOn = &_ChargePumpPowerOn,
    .PowerOff = &_ChargePumpPowerOff,
    .SetDispLightLv = &_ChargePumpSetDispLightLv,
    .SetKeyLightLv = &_ChargePumpSetKeyLightLv,
};

static int __init Ktd259Probe(struct platform_device *pdev) {

    int rc;
    struct resource *res;
    
    KtdRes.Device = pdev;

    do {
        res = platform_get_resource_byname(KtdRes.Device, IORESOURCE_IO, "ctrl");
        if (!res) {
            printk(KERN_ERR "%s:%d get ctrl pin fail\n",
                    __func__, __LINE__);
            rc = -EIO;
            break;
        }
        KtdRes.Id = res->start;
        printk(KERN_INFO "%s: get ctrl pin %d\n", __func__, KtdRes.Id);
        
        rc = gpio_request(KtdRes.Id, "ctrl_pin");
        if (rc)
            break;
        gpio_direction_output(KtdRes.Id, 0);
        Ktd259PowerOff(&KtdRes);

        KtdRes.CurLv = KtdRes.DesLv = KtdRes.MaxLv;
        
        ChargePumpRegIf(&_ChargePumpIf);
        
    } while(0);

    return rc;
}

static struct platform_driver __refdata Ktd259River = {
    .probe  = Ktd259Probe,
    .driver = {
        .name   = "ktd259",
    },
};

int __init Ktd259Init(void) 
{
    int ret;

    ret = platform_driver_register(&Ktd259River);
    
    return ret;
}

void __exit Ktd259Exit(void) 
{
    platform_driver_unregister(&Ktd259River);
}

module_init(Ktd259Init);
module_exit(Ktd259Exit);

void ChargePumpTest(void) {
    Ktdc(&KtdRes, 2);
}
EXPORT_SYMBOL(ChargePumpTest);

