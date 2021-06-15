<h1>Fractator 2077</h1>

*Created by S. Marousek (maroust1) | 2021/06/12*

![alt text](https://github.com/1000101cz/fractator_2077/blob/main/data/predict_1.png?raw=true "example")

<h2>Prerequisites (Debian)</h2>

*    libsdl2-dev

*    libsdl2-image-2.0-0

*    libsdl2-image-dev

*    sox

<h2>Run code:</h2>

Type `./Fractator_2077` in terminal.

You can use these execute options:

*      r - set width resolution

*      i - set height resolution

*      s - enable saving pc generated pictures to /tmp

*      p - setup pixel prediction

*      a - enable audio animation

Examples:

*    `./Fractator_2077` - (default settings)

*    `./Fractator_2077 -r -i -s -p` - (change picture width, height, prediction settings and save generated pictures)

*    `./Fractator_2077 -a -p` - (enable audio animation and set prediction)


<h2>Tips:</h2>

*    when zooming in to much, displayed picture is corrupted due to *double* data type accuracy

*    for smooth animation, dont set the distance between pixels (in prediction 10) on higher value than 15

*    use prediction (i recommend p12  with distace between pixels set to 9 and default resolution) for audio animation




tested on :computer: Debian, Ubuntu, Fedora, Manjaro, Raspbian | (x86_64 and ARM)    |   written in :atom: Atom
