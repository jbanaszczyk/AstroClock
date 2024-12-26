import gc
import time

import board
import busio
import displayio
from adafruit_display_shapes.circle import Circle
from adafruit_display_shapes.line import Line
from adafruit_st7789 import ST7789
from busdisplay import BusDisplay
from fourwire import FourWire


def get_ram_size_free():
    gc.collect()
    ram_free = gc.mem_free()
    return ram_free


def get_ram_size_total() -> int:
    ram_free = get_ram_size_free()
    ram_allocated = gc.mem_alloc()
    ram_total = ram_free + ram_allocated
    return ram_total


def calculate_hand_position(length, angle):
    import math
    radians = math.radians(angle - 90)  # Obrót tarczy o 90 stopni
    x = int(center_x + length * math.cos(radians))
    y = int(center_y + length * math.sin(radians))
    return x, y


def init_display() -> BusDisplay:
    displayio.release_displays()
    tft_cs = board.GP17
    tft_dc = board.GP16
    spi_mosi = board.GP19
    spi_clk = board.GP18
    spi = busio.SPI(spi_clk, spi_mosi)
    backlight = board.GP20
    display_bus = FourWire(spi, command=tft_dc, chip_select=tft_cs)
    display = ST7789(
        display_bus,
        rotation=0,
        width=240,
        height=320,
        backlight_pin=backlight,
        auto_refresh=False,
        brightness=0.8,
    )
    return display


display = init_display()

###########################################

splash = displayio.Group()
display.root_group = splash

WHITE = 0xFFFFFF
BLACK = 0x000000
RED = 0xFF0000
BLUE = 0xFF0080

center_x = display.width // 2
center_y = display.height // 2

clock_radius = 100
clock_face = Circle(center_x, center_y, clock_radius, outline=WHITE, fill=BLACK)
splash.append(clock_face)

while True:
    current_time = time.localtime()
    hours = current_time.tm_hour % 12
    minutes = current_time.tm_min
    seconds = current_time.tm_sec

    # Usunięcie starej grupy wskazówek
    if len(splash) > 1:
        splash.pop()

    hands_group = displayio.Group()

    hour_angle = 360 * (hours / 12) + (minutes / 60) * 30
    hour_length = clock_radius // 2
    hour_x, hour_y = calculate_hand_position(hour_length, hour_angle)
    hour_hand = Line(center_x, center_y, hour_x, hour_y, color=RED)
    hands_group.append(hour_hand)

    minute_angle = 360 * (minutes / 60)
    minute_length = clock_radius - 10
    minute_x, minute_y = calculate_hand_position(minute_length, minute_angle)
    minute_hand = Line(center_x, center_y, minute_x, minute_y, color=WHITE)
    hands_group.append(minute_hand)

    second_angle = 360 * (seconds / 60)
    second_length = clock_radius - 10
    second_x, second_y = calculate_hand_position(second_length, second_angle)
    second_hand = Line(center_x, center_y, second_x, second_y, color=BLUE)
    hands_group.append(second_hand)

    splash.append(hands_group)

    display.refresh()

    print(get_ram_size_free())

    time.sleep(1 / 3)

