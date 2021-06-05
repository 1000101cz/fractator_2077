Created by Stepan Marousek (maroust1) | 2021/05/16

List of required SDL2 packages ( for Ubuntu / Debian / Raspbian )
   -libsdl2-dev
   -libsdl2-image-2.0-0
   -libsdl2-image-dev

Versions
  -amd64
      -main version for Linux pc (amd64)
      -tested on Ubuntu 21.04
      -baud speed: 115200
  -arm
      -for Linux ARM architectures
      -tested on Raspberry Pi 3 B+
      -baud speed: 38400

EXECUTE OPTIONS
      /dev/ttyX      set serial port location   ( /dev/ttyACM0 by default )
      -r             set width resolution  (this options disables communication with Nucleo)
      -i             set height resolution (this options disables communication with Nucleo)
      -s             enable saving pc generated pictures to /tmp

      examples: ./Fractator_2077 /dev/ttyACM1 -s      (specify serial link for Nucleo and save pc generated pictures)
                ./Fractator_2077 -i -r -s             (change picture width and height and save pc generated pictures)
                ./Fractator_2077                      (default settings)


TIPS
       - dont forget to call set_compute ('s' key) after start and after changing fractal specifications to send new data to Nucleo
       - when zooming in to much, displayed picture is corrupted due to 'double' data type accuracy
