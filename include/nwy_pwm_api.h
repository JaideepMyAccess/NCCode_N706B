#ifndef __NWY_PWM_H__
#define __NWY_PWM_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    NWY_PWM_0 = 0,
    NWY_PWM_1,
    NWY_PWM_2,
    NWY_PWM_3,
    NWY_PWM_4,
    NWY_PWM_5,
    NWY_PWM_6,
    NWY_PWM_7,
    NWY_PWM_8,
    NWY_PWM_9,
    NWY_PWM_10,
    NWY_PWM_11,
    NWY_PWM_12,
    NWY_PWM_13,
    NWY_PWM_14,
    NWY_PWM_15,
    NWY_PWM_MAX
}nwy_pwm_id_t;

typedef struct {
    nwy_pwm_id_t id;
    uint16_t arr;
    uint16_t psc;
    uint16_t duty;
}nwy_pwm_t;

#define NWY_PWM0 "PWM0"
#define NWY_PWM1 "PWM1"
#define NWY_PWM2 "PWM2"
#define NWY_PWM3 "PWM3"

nwy_pwm_t* nwy_pwm_init(char *name,uint32_t freq,uint32_t duty);
bool nwy_pwm_start(nwy_pwm_t *d);
bool nwy_pwm_stop(nwy_pwm_t *d);
int nwy_pwm_deinit(nwy_pwm_t *d);

#ifdef __cplusplus
   }
#endif

#endif // __NWY_PWM_H__


