Created by Stepan Marousek (maroust1) | 2021/05/16

List of required SDL2 packages ( for Ubuntu / Debian / Raspbian )
   -libsdl2-dev
   -libsdl2-image-2.0-0
   -libsdl2-image-dev

EXECUTE OPTIONS
      -r             set width resolution
      -i             set height resolution
      -s             enable saving pc generated pictures to /tmp
      -p             setup pixel prediction

      examples: ./Fractator_2077                        (default settings)
                ./Fractator_2077 -i -r -s -p            (change picture width and height and save pc generated pictures)


TIPS
       - when zooming in to much, displayed picture is corrupted due to 'double' data type accuracy
