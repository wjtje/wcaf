# WCAF <!-- omit in toc -->

> Wjtje's Cooperative Arduino Framework

## Table of Contents <!-- omit in toc -->

- [Features](#features)
- [Class Diagram](#class-diagram)
- [Example program](#example-program)
- [Using built-in helpers](#using-built-in-helpers)
  - [GPIO](#gpio)
  - [Optional](#optional)
- [Using built-in components](#using-built-in-components)
  - [Button Component](#button-component)
  - [Led Component](#led-component)
  - [Interval Component](#interval-component)
- [Difference between Arduino and ESP8266](#difference-between-arduino-and-esp8266)
- [License](#license)

## Features

- Component based
- Logging support
- Low memory footprint
- Built-in interval support

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

## Example program

This is a basic program that will print `Hello World!` to the console every second.

```cpp
#include <Arduino.h>
#include <wcaf/components/application/application.h>
#include <wcaf/components/interval/interval.h>
#include <wcaf/components/logger/logger.h>

using namespace wcaf;

// Setup global variables
application::Application application_;
logger::Logger logger_;

// Set logger tag name
static const char* TAG = "Main";

void setup() {
  // Setup the logger
  logger_.set_buffer_size(128);
  logger_.set_baud_rate(9600);
  logger_.pre_setup();
  application_.register_component(&logger_);

  // Create an interval component
  interval::Interval* interval_ = new interval::Interval();
  interval_->set_interval(1000);
  interval_->set_callback([]() {
    // This code runs every second
    WCAF_LOG("Hello World!")
  });
  application_.register_component(interval_);

  application_.setup();
}

void loop() { application_.loop(); }
```

This code will result in the follow logs

```log
[Application:009]: Setting up 2 component(s)
[Application:012]: Setting up Logger
[Application:012]: Setting up Interval
[Main:025]: Hello World!
[Main:025]: Hello World!
[Main:025]: Hello World!
```

## Using built-in helpers

WCAF includes a few helpers that are used by multiple components and can be used in custom components.

### GPIO

The GPIO helper will read and write digital or analog values to a specific pin. It can also handle if the pin is inverted.

**Example:**

```cpp
#include <wcaf/helpers/gpio.h>

auto gpio_ = new gpio::GPIO(6);
auto value = gpio_->read_digital();
```

**Functions:**

```cpp
// Constructors
GPIO(uint8_t gpio)
GPIO(uint8_t gpio, bool inverted)

// Functions
void set_mode(uint8_t mode)

void write_digital(bool state)
bool read_digital()

void write_analog(int state)
int read_analog()
```

### Optional

This helper will help you while working with optional values.

**Example:**

```cpp
#include <wcaf/helpers/optional.h>

optional::Optional<int> state_;

if (state_.has_value()) {
  // Do some with the state
  WCAF_LOG("We have liftoff! %i", state_.value());
}
```

## Using built-in components

WCAF includes a few components to get you started. These components include the Button Component, Led Component, and Interval Component.

### Button Component

The Button Component will read a digital value and debouce it before calling the on press and on release callback.

**Example:**

```cpp
#include <wcaf/components/button/button.h>

auto button_ = new button::Button();
button_->set_gpio(new gpio::GPIO(5, true));
button_->set_on_release([]() {
  WCAF_LOG("I've been released!");
});
```

**Functions:**

```cpp
void set_gpio(gpio::GPIO *gpio)
void set_debounce(uint32_t debouce)
// For callback function on Arduino see bottom of the page
void set_on_press(std::function<void()> &&lambda)
void set_on_release(std::function<void()> &&lambda)
```

### Led Component

The Led Component will send a 'smooth' pwm value to a gpio pin to change the state in the specified duration.

**Example:**

```cpp
auto led_ = new led::Led();
led_->set_gpio(new gpio::GPIO(2, true));
application_.register_component(led_);

// Somewhere else in your code
led_->turn_on();
```

**Functions:**

```cpp
void set_gpio(gpio::GPIO *gpio)
void set_duration(uint32_t duration)
void set_interval(uint32_t interval)

void set_state(float state);
float get_state();

void turn_on();
void turn_off();
void toggle();

void start_blinking();
void stop_blinking();
void toggle_blinking();
```

### Interval Component

The Interval Component will execute the callback function every x milliseconds.

**Example:**

```cpp
interval::Interval* interval_ = new interval::Interval();
interval_->set_interval(1000);
interval_->set_callback([]() {
  // This code runs every second
  WCAF_LOG("Hello World!")
});
application_.register_component(interval_);
```

**Functions:**

```cpp
void set_interval(uint32_t interval)
// For callback function on Arduino see bottom of the page
void set_callback(std::function<void()> &&callback)
```

## Difference between Arduino and ESP8266

Due to the Arduino not having support for the default cpp libraries a few small things are different between the platforms. The main difference is that the Arduino doesn't have support for lambda function, but the included components have a "janky" way arround this. The following example shows how you can use the `set_argument` function to pass a pointer into a callback function.

```cpp
// Arduino
auto led_ = new led::Led();
led_->set_gpio(new gpio::GPIO(2, true));
application_.register_component(led_);

auto button_ = new button::Button();
button_->set_gpio(new gpio::GPIO(5, true));
button_->set_argument(led_)
button_->set_on_release([](void *argument) {
  auto led_ = (led::Led *)argument;
  WCAF_LOG("The led state is: %0.1f", led_.get_state());
});
application_.register_component(button_);

// ESP8266
auto led_ = new led::Led();
led_->set_gpio(new gpio::GPIO(2, true));
application_.register_component(led_);

auto button_ = new button::Button();
button_->set_gpio(new gpio::GPIO(5, true));
button_->set_on_release([led_]() {
  WCAF_LOG("The led state is: %0.1f", led_.get_state());
});
application_.register_component(button_);
```

## License

The MIT License (MIT) - Copyright (c) 2022 Wouter van der Wal
