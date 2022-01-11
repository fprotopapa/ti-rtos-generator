/*
 *  ======== main.c ========
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
double TIM_INTV = 0.1;

double arr_table[1000] = {0.0};
int indx = 0;


void timer_isr();
void idle_generator();
void swi_call();

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
    if (indx >= 1000) {
        indx = 0;
        Swi_post(SwiGen);
    } else {
        double omega = 2 * M_PI * FREQUENCY;
        // https://math.stackexchange.com/questions/1578241/ways-to-generate-triangle-wave-function
        switch (SHAPE) {
            case 0:
                arr_table[indx] = sin(omega) * AMPLITUDE;
                break;
            case 1:
                arr_table[indx] = asin(cos(omega)) * AMPLITUDE;
                break;
            case 2: {
                double dc = (1 / FREQUENCY) / 2;
                double time = (indx * TIM_INTV);
                int cycle_cnt = (int) time / dc;
                double time_passed = time - (cycle_cnt * 2 * TIM_INTV);
                arr_table[indx] = time_passed < dc ? 0 : AMPLITUDE;
                break;
            }
        }
        indx++;
    }
}


void idle_generator() {
    if (gui.control.refresh) {
        AMPLITUDE = gui.control.amp;
        FREQUENCY = gui.control.freq;
        SHAPE = gui.control.shape;
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
}

void task_call() {

    // refresh graph
    while (1) {
        Semaphore_pend(taskGen, BIOS_WAIT_FOREVER);

    }
}
