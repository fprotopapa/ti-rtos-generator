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
double RMS = 0;
double AVG = 0;
// Set time interval and samples per second
double TIM_INTV = 0.00001; // in second
int SAMPLES_PER_S = 100000;  // samples per second
#define ARR_SIZE 1000
double arr_table[ARR_SIZE] = {0.0};
int indx = 0;


void timer_isr();
void idle_generator();
void swi_call();
void calculate_samples(double omega, double delta_time);

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
    AMPLITUDE = gui.control.amp;
    FREQUENCY = gui.control.freq;
    SHAPE = gui.control.shape;
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

void calculate_samples(double omega, double delta_time) {
    // https://math.stackexchange.com/questions/1578241/ways-to-generate-triangle-wave-function
    switch (SHAPE) {
        case 0:
            arr_table[indx] = sin(omega * indx * delta_time) * AMPLITUDE;
            break;
        case 1:
            arr_table[indx] = asin(cos(omega * indx * delta_time)) * AMPLITUDE;
            break;
        case 2: {
            double dc = (1 / FREQUENCY) / 6;
            double time = (indx * TIM_INTV);
            int cycle_cnt = (int) time / dc;
            double time_passed = time - (cycle_cnt * 2 * TIM_INTV);
            arr_table[indx] = time_passed < dc ? 0 : AMPLITUDE;
            break;
        }
    }
    //gui.indicator.graph = arr_table[indx];
    indx++;
}

void idle_generator() {
    if (gui.control.refresh) {
        gui.control.refresh = 0;
        Semaphore_post(taskGen);
    }
}

void swi_call() {
    // cycle det, avg, rms
    switch (SHAPE) {
         case 0:
             AVG = 0;
             RMS = AMPLITUDE / sqrt(2);
             break;
         case 1:
             AVG = 0.5 * AMPLITUDE;
             RMS = AMPLITUDE / sqrt(3);
             break;
         case 2:
             AVG = AMPLITUDE / ((1/FREQUENCY)/2);
             RMS = AMPLITUDE;
             break;
     }
    gui.indicator.rms = RMS;
    gui.indicator.avg = AVG;
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


