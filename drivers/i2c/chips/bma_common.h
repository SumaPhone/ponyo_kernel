#ifndef __BMA_COMMON_H__
#define __BMA_COMMON_H__

typedef struct
{
  signed short XVal;
  signed short YVal;
  signed short ZVal;
} MON_XYZ_INT, *PMON_XYZINT;

typedef struct
{
  MON_XYZ_INT xyz;
  signed short scalex;
  signed short scaley;
  signed short scalez;
  unsigned int resvered;
} MON_CAL_VAL, *PMON_CAL_VAL;

#endif
