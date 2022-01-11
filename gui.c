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
    self->control.amp = 0.0;
    self->control.freq = 0.0;
    self->control.shape = 0;
    self->control.refresh = 0;
    /*
     * Indicators
     */
    self->indicator.display = " ";
}
