/*
 * gui.h
 *
 *  Created on: Dec 22, 2021
 *      Author: Fabbio
 */
#ifndef GUI_H_
#define GUI_H_
/*
 * GUI structure
 */

struct gui_controls_t
{
    double freq;
    double amp;
    int shape;
    int refresh;
};

struct gui_indicators_t
{
    char *display;
};

struct gui_t
{
    struct gui_controls_t control;
    struct gui_indicators_t indicator;
};

extern struct gui_t gui;

/*
 * Initialize GUI
 */
void gui_init(struct gui_t *self);

#endif /* GUI_H_ */
