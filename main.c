/*
 *  ======== main.c ========
 *  Todo: Add cycle detection, Add graph
 */

#include <xdc/std.h>

#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>

#include <ti/sysbios/knl/Task.h>

#include <xdc/cfg/global.h>

#include <math.h>

#include "F28x_Project.h"
#include "gui.h"

double AMPLITUDE = 0;
double FREQUENCY = 0;
int SHAPE = 0;
int RECT_CYCLE = 0;
long double RMS = 0;
long double AVG = 0;
int RECT_SIGN = 1;
// Set time interval and samples per second
double TIM_INTV = 0.00001; // in second
long int SAMPLES_PER_S = 100000;  // samples per second
#define ARR_SIZE 1000
double arr_table[ARR_SIZE] = {0.0};
int indx = 0;


void timer_isr();
void idle_generator();
void swi_call();
void calculate_samples(double omega, double delta_time);
double calculate_rect(double omega);
double calculate_triangle(double omega, double delta_time);
double calculate_avg(int chg_sign_indx);
double calculate_rms(int chg_sign_indx);

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

void timer_isr() {
    if (indx == 0) {
        RECT_SIGN = 1;
        AMPLITUDE = gui.control.amp;
        FREQUENCY = gui.control.freq;
        SHAPE = gui.control.shape;
    }
    if (indx == ARR_SIZE) {
        Swi_post(SwiGen);
        indx++;
    } else if (indx < ARR_SIZE) {
        double omega = 2.0 * M_PI * FREQUENCY;
        double delta_time = 1.0 / SAMPLES_PER_S;
        calculate_samples(omega, delta_time);

    } else {
        indx = 0;
    }
}
// https://www.exstrom.com/siggen/prog/square.c
// https://en.wikipedia.org/wiki/Triangle_wave

void calculate_samples(double omega, double delta_time) {
    switch (SHAPE) {
        case 0:
            arr_table[indx] = sin(omega * indx * delta_time) * AMPLITUDE;
            break;
        case 1:
            arr_table[indx] = calculate_triangle(omega, delta_time) * AMPLITUDE * (2 / M_PI);//-asin(cos(omega * indx * delta_time)) * AMPLITUDE * (2 / M_PI);
            break;
        case 2: {
            arr_table[indx] = calculate_rect(omega) * -AMPLITUDE;
            break;
        }
    }
    indx++;
}
//int TRIANGLE_SIGN = 1;
double SIN_VAL_DIS = 0;
double calculate_triangle(double omega, double delta_time) {

    double sin_val = sin(omega * indx * delta_time);
    SIN_VAL_DIS = sin_val;
    return asin(sin_val);
}

double calculate_rect(double omega) {
    unsigned long int m = SAMPLES_PER_S /omega;//(2.0* FREQUENCY);
    if( indx % m == 0) {
        RECT_SIGN = -RECT_SIGN;
    }
    return RECT_SIGN;
}

void idle_generator() {
    if (gui.control.refresh) {
        gui.control.refresh = 0;
        Semaphore_post(taskGen);
    }
}

int INDEX_FOUND = 0;
void swi_call() {
    // Detect one period, calculate AVG and RMS

    int sample = 0;
    int chg_sign_indx = 0;
    double prev = arr_table[0];
    for (sample = 1; sample < (ARR_SIZE - 1); sample++) {
        if (prev < 0 & arr_table[sample] >= 0) {
            chg_sign_indx = sample;
            break;
        } else {
            prev = arr_table[sample];
        }
    }
    INDEX_FOUND = chg_sign_indx;
    if (SHAPE != 1) {
        RMS = calculate_rms(chg_sign_indx);
        AVG = calculate_avg(chg_sign_indx);
    }

    gui.indicator.rms = RMS;
    gui.indicator.avg = AVG;
}

// https://www.electronics-tutorials.ws/accircuits/rms-voltage.html
double calculate_avg(int chg_sign_indx) {
    long double sum = 0;
    int sample = 0;
    for (sample = 0; sample < chg_sign_indx; sample++) {
        sum += arr_table[sample];
    }
    return (sum / (double)chg_sign_indx);
}

double calculate_rms(int chg_sign_indx) {
    long double sum = 0;
    int sample = 0;
    for (sample = 0; sample < chg_sign_indx; sample++) {
        sum += pow(arr_table[sample], 2);
    }
    return (sqrt(sum / (double)chg_sign_indx));
}

void task_call() {
    // refresh graph
    while (1) {
        Semaphore_pend(taskGen, BIOS_WAIT_FOREVER);

        int sample = 0;
        for (sample = 0; sample < ARR_SIZE; sample++) {
            gui.indicator.graph = arr_table[sample];
            DELAY_US(10000);
        }
        gui.indicator.graph = 0;
    }
}


