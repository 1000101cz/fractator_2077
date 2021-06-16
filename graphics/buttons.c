/**********************************************
 * name:    FRACTATOR 2077                    *
 * author:  STEPAN MAROUSEK                   *
 * date:    2021/6/15                         *
 **********************************************/

#include "buttons.h"

/* layout 0 */
#include "buttons/compute.h"
#include "buttons/animate.h"
#include "buttons/settings.h"
#include "buttons/fractal.h"
#include "buttons/exit.h"

/* layout 1 */
#include "buttons/back.h"
#include "buttons/size.h"
#include "buttons/zoom.h"
#include "buttons/saves.h"
#include "buttons/move.h"

/* layout 2 */
#include "buttons/xMinus.h"
#include "buttons/xPlus.h"
#include "buttons/yMinus.h"
#include "buttons/yPlus.h"

/* layout 3 */
#include "buttons/plus.h"
#include "buttons/minus.h"

/* layout 4 */
#include "buttons/OnOff.h"
#include "buttons/PpmPng.h"

/* layout 5 */
#include "buttons/prediction.h"
#include "buttons/values.h"

/* layout 6 */
#include "buttons/0.h"
#include "buttons/1.h"
#include "buttons/2.h"
#include "buttons/3.h"

/* layout 7 */
#include "buttons/4.h"
#include "buttons/5.h"
#include "buttons/10.h"

/* layout 8 */
#include "buttons/11.h"
#include "buttons/12.h"
#include "buttons/13.h"

/* layout 9 */
#include "buttons/iterPlus.h"
#include "buttons/iterMinus.h"
#include "buttons/cPlus.h"
#include "buttons/cMinus.h"

/* layout 10 */
#include "buttons/arrowRight.h"
#include "buttons/arrowLeft.h"
#include "buttons/arrowUp.h"
#include "buttons/arrowDown.h"


uint8_t buttonOne(int position, int menuPosition)
{
  uint8_t returnValue = 255;
  switch (menuPosition){
    case 0:
      returnValue = buttonCompute[position];
      break;

    case 1:
      returnValue = buttonSize[position];
      break;

    case 2:
      returnValue = buttonXPlus[position];
      break;

    case 3:
      returnValue = buttonPlus[position];
      break;

    case 4:
      returnValue = buttonOnOff[position];
      break;

    case 5:
      returnValue = buttonPrediction[position];
      break;

    case 6:
      returnValue = button0[position];
      break;

    case 7:
      returnValue = buttonArrowLeft[position];
      break;

    case 8:
      returnValue = buttonArrowLeft[position];
      break;

    case 9:
      returnValue = buttonIterPlus[position];
      break;

    case 10:
      returnValue = buttonArrowLeft[position];
      break;

  }
  return returnValue;
}

uint8_t buttonTwo(int position, int menuPosition)
{
  uint8_t returnValue = 255;
  switch (menuPosition){
    case 0:
      returnValue = buttonAnimate[position];
      break;

    case 1:
      returnValue = buttonZoom[position];
      break;

    case 2:
      returnValue = buttonXMinus[position];
      break;

    case 3:
      returnValue = buttonMinus[position];
      break;

    case 4:
      returnValue = buttonPpmPng[position];
      break;

    case 5:
      returnValue = buttonValues[position];
      break;

    case 6:
      returnValue = button1[position];
      break;

    case 7:
      returnValue = button4[position];
      break;

    case 8:
      returnValue = button11[position];
      break;

    case 9:
      returnValue = buttonIterMinus[position];
      break;

    case 10:
      returnValue = buttonArrowUp[position];
      break;

  }
  return returnValue;
}

uint8_t buttonThree(int position, int menuPosition)
{
  uint8_t returnValue = 255;
  switch (menuPosition){
    case 0:
      returnValue = buttonSettings[position];
      break;

    case 1:
      returnValue = buttonSaves[position];
      break;

    case 2:
      returnValue = buttonYPlus[position];
      break;

    case 3:
      returnValue = buttonBack[position];
      break;

    case 4:
      returnValue = buttonBack[position];
      break;

    case 5:
      returnValue = buttonBack[position];
      break;

    case 6:
      returnValue = button2[position];
      break;

    case 7:
      returnValue = button5[position];
      break;

    case 8:
      returnValue = button12[position];
      break;

    case 9:
      returnValue = buttonCPlus[position];
      break;

    case 10:
      returnValue = buttonArrowDown[position];
      break;

  }
  return returnValue;
}

uint8_t buttonFour(int position, int menuPosition)
{
  uint8_t returnValue = 255;
  switch (menuPosition){
    case 0:
      returnValue = buttonFractal[position];
      break;

    case 1:
      returnValue = buttonMove[position];
      break;

    case 2:
      returnValue = buttonYMinus[position];
      break;

    case 3:
      returnValue = 0;
      break;

    case 4:
      returnValue = 0;
      break;

    case 5:
      returnValue = 0;
      break;

    case 6:
      returnValue = button3[position];
      break;

    case 7:
      returnValue = button10[position];
      break;

    case 8:
      returnValue = button13[position];
      break;

    case 9:
      returnValue = buttonCMinus[position];
      break;

    case 10:
      returnValue = buttonArrowRight[position];
      break;

  }
  return returnValue;
}

uint8_t buttonFive(int position, int menuPosition)
{
  uint8_t returnValue = 255;
  switch (menuPosition){
    case 0:
      returnValue = buttonExit[position];
      break;

    case 1:
      returnValue = buttonBack[position];
      break;

    case 2:
      returnValue = buttonBack[position];
      break;

    case 3:
      returnValue = 0;
      break;

    case 4:
      returnValue = 0;
      break;

    case 5:
      returnValue = 0;
      break;

    case 6:
      returnValue = buttonArrowRight[position];
      break;

    case 7:
      returnValue = buttonArrowRight[position];
      break;

    case 8:
      returnValue = buttonBack[position];
      break;

    case 9:
      returnValue = buttonBack[position];
      break;

    case 10:
      returnValue = buttonBack[position];
      break;

  }
  return returnValue;
}
