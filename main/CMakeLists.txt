
#
# Main component of TF Micro project 'micro_speech'.
#

idf_component_register(
    SRCS main.cc model.cc led.cc neurone.cc tasks.cc donnees.c
    PRIV_REQUIRES spi_flash esp_timer 
    REQUIRES driver esp-dsp
    )
