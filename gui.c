/*
 * gui.c
 *
 *  Created on: Dec 22, 2021
 *      Author: Fabbio
 */
#include "gui.h"
/*
 * Global struct for GUI access
 */
struct gui_t gui;
/*
 * Initialize or reset GUI with or without controls
 */
void gui_init(struct gui_t *self)
{
    /*
     * Controls
     */
    self->control.amp = 1.0;
    self->control.freq = 400.0;
    self->control.shape = 0;
    self->control.refresh = 0;
    /*
     * Indicators
     */
    self->indicator.graph = 0;
    self->indicator.rms = 0.0;
    self->indicator.avg = 0.0;
}
