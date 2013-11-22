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
 * mrb_config.h,v.0.1.0 2013/11/18
 */

#ifndef MRB_CONFIG_H
#define	MRB_CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "mrbconf.h"
#include "mruby.h"

#include "mruby/irep.h"
#include "mruby/string.h"
#include "mruby/value.h"

    void mrb_init_define_method(void);

// for test use
    mrb_value mrb_blink_led(mrb_state* mrb, mrb_value self);

    mrb_value mrb_sendNote(mrb_state* mrb, mrb_value self);

#ifdef	__cplusplus
}
#endif

#endif	/* MRB_CONFIG_H */

