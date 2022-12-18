# WCAF

> Wjtje's Cooperative Arduino Framework

## Features

- Component based
- Logging support
- Low memory footprint
- Buildin interval support

## Class Diagram

```mermaid
classDiagram

  class GPIO {
    #uint8_t gpio_
    #bool inverted_
    +GPIO(gpio)
    +GPIO(gpio, inverted)
    +set_mode(mode)
    +write_digital(state)
    +write_analog(state)
    +read_digital() bool
    +read_analog() int
    +get_gpio() uint8_8
  }

  class Component {
    <<Interface>>
    +setup()
    +loop()
    +get_tag() const char *
  }

  Component <|-- Application
  class Application {
    #List~Component *~ components_
    +register_component(component)
  }

  Component <|-- Logger
  class Logger {
    +pre_setup()
    +set_buffer_size(size)
    +set_buffer_size(baud_rate)
    +print(tag, line, format_addr, format_size, args)
  }

  Component <|-- Interval
  class Interval {
    #uint32_t last_interval_
    +set_interval(millis)
    +set_callback(callback)
    +set_parent(parent)
  }

  Component <|-- Led
  GPIO .. Led
  class Led {
    +set_gpio(gpio)
    +set_duration(duration)
    +turn_on()
    +turn_off()
    +set_state(state)
    +get_state(): float
  }

  Component <|-- Button
  GPIO .. Button
  class Button {
    #bool last_button_state_
    #bool status_
    #uint32_t last_button_time_

    +set_gpio(gpio)
    +set_debounce(time)
    +set_argument(argument)
    +set_on_press(lambda)
    +set_on_release(lambda)
  }
```

## License

The MIT License (MIT) - Copyright (c) 2022 Wouter van der Wal
