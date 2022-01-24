/*
 *  ======== main.c ========
 *
 *
 */

#include <xdc/std.h>

#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>

#include <ti/sysbios/knl/Task.h>

#include <xdc/cfg/global.h>

#include <math.h>

#include "F28x_Project.h"
#include "gui.h"
/*
 * Global variables
 */
double AMPLITUDE = 0;
double FREQUENCY = 0;
int SHAPE = 0;
long double RMS = 0;
long double AVG = 0;
int RECT_SIGN = 1;
#define ARR_SIZE 1000
double arr_table[ARR_SIZE] = {0.0};
int INDX = 0;
int INDX_START = 0;
int INDX_END = 0;
unsigned long TICK = 0;
/*
 * Set time interval and samples per second
 *
 * 10 us
 */
double TIM_INTV = 0.00001; // in second
long int SAMPLE_RATE = 100000;  // samples per second
/*
 * Declaration
 */
void timer_isr();
void idle_generator();
void swi_call();
void calculate_samples(double omega, double delta_time);
double calculate_rect(double omega, double delta_time);
double calculate_triangle(double omega, double delta_time);
double calculate_avg(int chg_sign_indx_start, int chg_sign_indx_end);
double calculate_rms(int chg_sign_indx_start, int chg_sign_indx_end);
int detect_sign_change(double arr[], int start);
/*
 *  ======== main ========
 */
Int main()
{ 
    /*
     * use ROV->SysMin to view the characters in the circular buffer
     */
    System_printf("enter main()\n");
    gui_init(&gui);
    BIOS_start();    /* does not return */
    return(0);
}
/*
 * HWI Timer
 */
void timer_isr() {
    AMPLITUDE = gui.control.amp;
    FREQUENCY = gui.control.freq;
    SHAPE = gui.control.shape;
    if (TICK < 4294967295) {
        TICK++;
    } else {
        TICK = 0;
    }
    if (INDX == ARR_SIZE) {
        Swi_post(SwiGen);
        INDX = 0;
    } else if (INDX < ARR_SIZE) {
        double omega = 2.0 * M_PI * FREQUENCY;
        double delta_time = 1.0 / SAMPLE_RATE;
        calculate_samples(omega, delta_time);
    }
}
/*
 * IDLE Task
 */
void idle_generator() {
    if (gui.control.refresh) {
        gui.control.refresh = 0;
        Semaphore_post(taskGen);
    }
}
/*
 * Task
 */
void task_call() {
    // refresh graph
    while (1) {
        Semaphore_pend(taskGen, BIOS_WAIT_FOREVER);
        int indx = INDX;
        int smp_start = 0;
        int smp = 0;
        int smp_indx = indx + 1;
        for (smp = 0; smp < ARR_SIZE; smp++) {
            if (smp_indx < ARR_SIZE) {
                gui.indicator.graph[smp] = arr_table[smp_indx];
                smp_indx++;
            } else {
                gui.indicator.graph[smp] = arr_table[smp_start];
                smp_start++;
            }
        }
    }
}
/*
 * SWI
 */
void swi_call() {
    // Detect one period, calculate AVG and RMS
    int chg_sign_indx_start = 0;
    int chg_sign_indx_end = 0;
    chg_sign_indx_start = detect_sign_change(arr_table, 1);
    chg_sign_indx_end = detect_sign_change(arr_table, 0);
    INDX_START = chg_sign_indx_start;
    INDX_END = chg_sign_indx_end;
    AVG = calculate_avg(chg_sign_indx_start, chg_sign_indx_end);
    RMS = calculate_rms(chg_sign_indx_start, chg_sign_indx_end);


    gui.indicator.rms = RMS;
    gui.indicator.avg = AVG;
}
/*
 * SWI
 *
 */
void calculate_samples(double omega, double delta_time) {
    switch (SHAPE) {
        case 0:
            arr_table[INDX] = sin(omega * TICK * delta_time) * AMPLITUDE;
            break;
        case 1:
            arr_table[INDX] = calculate_triangle(omega, delta_time) * AMPLITUDE;
            break;
        case 2: {
            arr_table[INDX] = calculate_rect(omega, delta_time) * AMPLITUDE;
            break;
        }
    }
    INDX++;
}
/*
 * Generate Triangle
 */
double calculate_triangle(double omega, double delta_time) {
    //double period = (int)((1 / omega) / delta_time) + 1.0;
    double period = (double) SAMPLE_RATE / (double) FREQUENCY;
    double x = fabs(2.0*((TICK/period) - floor((TICK/period) + 0.5)));
    return (2.0 * x) - 1.0;
}
/*
 * Generate Rectangle
 */
double calculate_rect(double omega, double delta_time) {
    double sinval = sin(omega * TICK * delta_time);
    if (sinval >= 0) {
        return 1.0;
    } else {
        return -1.0;
    }
}
/*
 * Find index of full period
 */
int detect_sign_change(double arr[], int start) {
    int sample = 0;
    if (start) {
        double prev = arr[0];
        for (sample = 1; sample < ARR_SIZE; sample++) {
            if ((prev > 0) & (arr_table[sample] <= 0)) {
                return sample;
            }
            else if ((prev < 0) & (arr[sample] >= 0)) {
                return sample;
            } else {
                prev = arr[sample];
            }
        }
    } else {
        double prev = arr[ARR_SIZE - 1];
        for (sample = (ARR_SIZE - 2); sample > 0; sample--) {
            if ((prev > 0) & (arr_table[sample] <= 0)) {
                return sample;
            }
            else if ((prev < 0) & (arr[sample] >= 0)) {
                return sample;
            } else {
                prev = arr[sample];
            }
        }
    }
    return 0;
}
/*
 * Average
 *
 * Symmetric signal -> AVG ~ 0
 */
double calculate_avg(int chg_sign_indx_start, int chg_sign_indx_end) {
    long double sum = 0;
    int sample = 0;
    for (sample = chg_sign_indx_start; sample <= chg_sign_indx_end; sample++) {
        sum += arr_table[sample];
    }
    return (sum / (long double)(chg_sign_indx_end - chg_sign_indx_start + 1));
}
/*
 * RMS
 *
 * Sine:        A/sqrt(2) -> 1/sqrt(2) = 0.7071
 * Square:      A -> 1
 * Triangle:    A/sqrt(3) -> 1/sqrt(3) = 0.57735
 *
 */
double calculate_rms(int chg_sign_indx_start, int chg_sign_indx_end) {
    long double sum = 0;
    int sample = 0;
    for (sample = chg_sign_indx_start; sample <= chg_sign_indx_end; sample++) {
        sum += pow(arr_table[sample], 2);
    }
    return (sqrt(sum / (long double)(chg_sign_indx_end - chg_sign_indx_start + 1)));
}
