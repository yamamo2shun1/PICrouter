current_usb_mode = 'HOST'

led_state = 0
eth_state = 0
host_initialized = 0

o = Object.new

# Port I/O Initialization
o.init_io_ports

# Open Sound Control Initalization
o.init_osc_config('/std', 8080, 8000)

case current_usb_mode
when 'DEVICE'
  3.times do
    o.onboard_led(1, 'on')
    o.delay_1ms(200)
    o.onboard_led(1, 'off')
    o.delay_1ms(200)
  end

  # USB Device Initialization
  o.usb_device_init
when 'HOST'
  3.times do
    o.onboard_led(2, 'on')
    o.delay_1ms(200)
    o.onboard_led(2, 'off')
    o.delay_1ms(200)
  end
end

loop do
  # Test code to check mruby's operation
  if led_state == 0
    o.output_port('c14', 'high')
    led_state = 1
    #o.delay_10us(1)
  else
    o.output_port('c14', 'low')
    led_state = 0
    #o.delay_10us(1)
  end

  eth_state = o.network_tasks(eth_state)
  o.receive_osc_task

  case current_usb_mode
  when 'DEVICE'
    o.usb_device_tasks
    o.hid_ctrl_task
  when 'HOST'
    if host_initialized == 0
      host_initialized = o.usb_host_init
    else
      o.usb_host_tasks
      o.convert_midi_to_osc
      o.usb_cdc_rxtx_handler
    end
  end
end
