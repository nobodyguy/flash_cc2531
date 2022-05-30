
#ifndef GPIO_H
#define GPIO_H

#include <gpiod.h>

#define GPIO_INPUT 0
#define GPIO_OUTPUT 1

#define GPIO_LOW 0
#define GPIO_HIGH 1

#define GPIO_CONSUMER "flash_cc2531"

#define GPIO_PINS 3 // DD, DC, RST

typedef struct gpio_pin_mapping
{
  int pin;
  struct gpiod_line *line;
} gpio_pin_mapping;

typedef struct gpio_configuration
{
  struct gpiod_chip *chip;
  struct gpio_pin_mapping pin_mappings[GPIO_PINS];
} gpio_configuration;

static gpio_configuration gpio_config;

gpio_pin_mapping get_pin_mapping(int pin)
{
  int i, pos = -1;
  for (i = 0; i < GPIO_PINS; i++)
  {
    if (gpio_config.pin_mappings[i].pin == pin)
    {
      pos = i;
      break;
    }
  }
  return gpio_config.pin_mappings[pos];
}

int gpio_init(const char *chipname)
{
  gpio_config.chip = gpiod_chip_open(chipname);
  for (int i = 0; i < GPIO_PINS; i++)
  {
    gpio_config.pin_mappings[i] = (gpio_pin_mapping){.pin = -1, .line = NULL};
  }
  return 1;
}

void gpio_deinit()
{
  for (int i = 0; i < GPIO_PINS; i++)
  {
    gpiod_line_release(gpio_config.pin_mappings[i].line);
  }

  // in newer libgpiod: gpiod_chip_unref(gpio_config.chip);
  gpiod_chip_close(gpio_config.chip);
}

void gpio_pin_configure(int pin, int mode)
{
  int i, pos = -1;
  for (i = 0; i < GPIO_PINS; i++)
  {
    // first init of the pin
    if (gpio_config.pin_mappings[i].pin == -1)
    {
      gpio_config.pin_mappings[i].pin = pin;
      gpio_config.pin_mappings[i].line = gpiod_chip_get_line(gpio_config.chip, pin);
    }

    // already initialized pin, only change mode
    if (gpio_config.pin_mappings[i].pin == pin)
    {
      if (mode == GPIO_INPUT)
      {
        gpiod_line_request_input(gpio_config.pin_mappings[i].line, GPIO_CONSUMER);
      }
      else
      {
        gpiod_line_request_output(gpio_config.pin_mappings[i].line, GPIO_CONSUMER, GPIO_LOW);
      }
      break;
    }
  }
}

int gpio_pin_get(int pin)
{
  gpio_pin_mapping pin_mapping = get_pin_mapping(pin);
  return gpiod_line_get_value(pin_mapping.line);
}

void gpio_pin_set(int pin, int value)
{
  gpio_pin_mapping pin_mapping = get_pin_mapping(pin);
  gpiod_line_set_value(pin_mapping.line, value);
}

#endif
