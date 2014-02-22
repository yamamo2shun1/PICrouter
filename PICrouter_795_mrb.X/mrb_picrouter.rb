##
# Copylight (C) 2013, Shunichi Yamamoto, tkrworks.net
#
# This file is part of PICrouter.
#
# PICrouter is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option ) any later version.
#
# PICrouter is distributed in the hope that it will be useful,
# but WITHIOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with PICrouter. if not, see <http:/www.gnu.org/licenses/>.
#
# mrb_picrouter.rb,v.0.2.3 2014/02/22
#

def send_osc_task
  onboard_led(1, 'toggle')
end

current_usb_mode = 'HOST'

led_state = 0
eth_state = 0
host_initialized = 0

# Port I/O Initialization
init_io_ports

# Timer5 Initialization
config_timer5(8000)

case current_usb_mode
when 'DEVICE'
  6.times do
    onboard_led(1, 'toggle')
    delay_1ms(200)
  end

  # USB Device Initialization
  usb_device_init
when 'HOST'
  6.times do
    onboard_led(2, 'toggle')
    delay_1ms(200)
  end
end

# Open Sound Control Initalization
init_osc_config('picrouter', '/std', 8080, 8000)

loop do
  eth_state = network_tasks(eth_state)

  get_osc_packet
  if process_osc_packet
    if compare_osc_prefix('/std')
      process_standard_messages
    elsif compare_osc_prefix('/midi')
      process_midi_messages
    elsif compare_osc_prefix('/cdc')
      process_cdc_messages
    elsif compare_osc_prefix('/sys')
      process_system_messages
=begin
      if compare_osc_address('/onboard/led')
        index = get_int_arg_at_index(0)
        state = get_string_arg_at_index(1)
        onboard_led(index + 1, state)
      elsif compare_osc_address('/print')
        value = get_int_arg_at_index(0)

        set_osc_address('/sys', '/print/return')
        set_osc_typetag('ifisf')
        add_osc_int_arg(value)
        add_osc_float_arg(3.14)
        add_osc_int_arg(15)
        add_osc_string_arg('mruby')
        add_osc_float_arg(0.2)

        flush_osc_message
      end
=end
    end
  end

  case current_usb_mode
  when 'DEVICE'
    usb_device_tasks
    hid_ctrl_task
  when 'HOST'
    if host_initialized == 0
      host_initialized = usb_host_init
    else
      usb_host_tasks
      convert_midi_to_osc
      usb_cdc_rxtx_handler
    end
  end
end
