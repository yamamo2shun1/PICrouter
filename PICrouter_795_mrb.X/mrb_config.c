/*
 * Copylight (C) 2013, Shunichi Yamamoto, tkrworks.net
 *
 * This file is part of PICrouter.
 *
 * PICrouter is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option ) any later version.
 *
 * PICrouter is distributed in the hope that it will be useful,
 * but WITHIOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PICrouter. if not, see <http:/www.gnu.org/licenses/>.
 *
 * mrb_config.c,v.0.1.0 2013/11/18
 */

#include "mrb_config.h"

void mrb_init_define_method(void)
{
    mrb_define_method(mrb, mrb->object_class, "blink_led", mrb_blink_led, ARGS_NONE());

    mrb_define_method(mrb, mrb->object_class, "send_note", mrb_send_note, ARGS_REQ(3));
}

// for test use
mrb_value mrb_blink_led(mrb_state* mrb, mrb_value self)
{
    LED_1_On();
    LED_2_On();

    DelayMs(200);

    LED_1_Off();
    LED_2_Off();

    DelayMs(200);

    return mrb_nil_value();
}

mrb_value mrb_send_note(mrb_state* mrb, mrb_value self)
{
    mrb_int num, vel, ch;

    mrb_get_args(mrb, "iii", &num, &vel, &ch);

    // USB Tasks
    USBDeviceTasks();
    USBControlTask();

    sendNote(num, vel, ch);

    return mrb_nil_value();
}
