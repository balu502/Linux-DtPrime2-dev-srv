#include "progerror.h"

//----------------------------------------------------------------------------------------
// Error processing on working time of device
//----------------------------------------------------------------------------------------
TDevErrors::TDevErrors(QObject *parent) : QObject(parent)
{
  cntOptErr=cntAmpErr=cntMotErr=cntTftErr=cntBulkErr=cntNWErr=0;
  map=0;
  devErrorsText.clear();
  bitError.reset();
}
TDevErrors::~TDevErrors()
{
}
void TDevErrors::setDevError(DevErrors err)
{
  bitError.set((int)err);

  switch(err)
  {
    case USB_CONNECTION_ERROR:  break;
    case OPEN_OPT_ERROR: cntOptErr++ ; map|=0x1000; break;
    case OPEN_AMP_ERROR: cntAmpErr++;  map|=0x2000;break;
    case OPEN_MOT_ERROR: cntMotErr++;  map|=0x4000;break;
    case OPEN_TFT_ERROR: cntTftErr++;  map|=0x8000;break;
    case RW_OPT_ERROR:   cntOptErr++;  map|=0x1000; break;
    case RW_AMP_ERROR:   cntAmpErr++;  map|=0x2000;break;
    case RW_MOT_ERROR:   cntMotErr++;  map|=0x4000;break;
    case RW_TFT_ERROR:   cntTftErr++;  map|=0x8000;break;
    case TOUT_OPT_ERROR: cntOptErr++;  map|=0x1000; break;
    case TOUT_AMP_ERROR: cntAmpErr++;  map|=0x2000;break;
    case TOUT_MOT_ERROR: cntMotErr++;  map|=0x4000;break;
    case TOUT_TFT_ERROR: cntTftErr++;  map|=0x8000;break;
    case INCORRECT_CAN_DEVICE_ERROR: ; break;
    case RW_USBBULK_ERROR: cntBulkErr++; map|=0x0800; break ;
    case GETFRAME_ERROR: cntNWErr++; break;
    default:;
  }
}

//-----------------------------------------------------------------------------
//---Return simple error for server answer
//-----------------------------------------------------------------------------
short int TDevErrors::analyseError(void)
{
  short int ret=NONE_ERR;
  if(bitError.any()){
    for(unsigned int i=0;i<bitError.size();i++){
      if(bitError.test(i))
        switch(i)
        {
          case USB_CONNECTION_ERROR:     ret = USB_ERR;  break;
          case OPEN_OPT_ERROR:           ret = CAN_ERR; break;
          case OPEN_AMP_ERROR:           ret = CAN_ERR; break;
          case OPEN_MOT_ERROR:           ret = CAN_ERR; break;
          case OPEN_TFT_ERROR:           ret = CAN_ERR; break;
          case RW_OPT_ERROR:             ret = CAN_ERR; break;
          case RW_AMP_ERROR:             ret = CAN_ERR; break;
          case RW_MOT_ERROR:             ret = CAN_ERR; break;
          case RW_TFT_ERROR:             ret = CAN_ERR; break;
          case TOUT_OPT_ERROR:           ret = CAN_ERR; break;
          case TOUT_AMP_ERROR:           ret = CAN_ERR; break;
          case TOUT_MOT_ERROR:           ret = CAN_ERR; break;
          case TOUT_TFT_ERROR:           ret = CAN_ERR; break;
          case INCORRECT_CAN_DEVICE_ERROR:     ret = CAN_ERR; break;
          case RW_USBBULK_ERROR:         ret = USBBULK_ERR; break ;
          case CMD_USBBULK_ERROR:        ret = DEVHW_ERR; break;
          case GETFRAME_ERROR:           ret = DATAFRAME_ERR; break;
          case OPENINRUN_ERROR:          ret = OPENINRUN_ERR ; break;
          case MOTORALARM_ERROR:         ret = MOTORALARM_ERR ; break;
          case STARTWITHOPENCOVER_ERROR: ret = STARTONOPEN_ERR ; break;
          case PRERUNFAULT_ERROR:        ret = PRERUNFAULT_ERR ; break; //set flag 9 bit for full fault
          case PRERUNSEMIFAULT_ERROR:    ret = PRERUNSEMIFAULT_ERR ; break;
          case MOTORDRVLED_ERROR:        ret = CAN_ERR; break;
          case MEASHIGHTUBE_ERROR:       ret = MEASHT_ERR; break;
          case GETAMPPROG_ERROR:         ret = GETAMPPROG_ERR; break;
          default: if(ret==NONE_ERR)     ret = UNKNOWN_ERR;
        }
    }
  }
  ret|=(map&0xf800);
  return ret;
}
// large list of error
QStringList TDevErrors::readDevTextErrorList(void)
{
  devErrorsText.clear();
  if(bitError.any()){
    for(unsigned int i=0;i<bitError.size();i++)
      if(bitError.test(i)) {
        switch(i)
        {
          case USB_CONNECTION_ERROR: devErrorsText.append(tr("USB Connection error.")); break;
          case OPEN_OPT_ERROR: devErrorsText.append(tr("Open optical CAN channel error.")); break;
          case OPEN_AMP_ERROR: devErrorsText.append(tr("Open temperature CAN channel error.")); break;
          case OPEN_MOT_ERROR: devErrorsText.append(tr("Open motor CAN channel error.")); break;
          case OPEN_TFT_ERROR: devErrorsText.append(tr("Open TFT CAN channel error. ")); break;
          case RW_OPT_ERROR:   devErrorsText.append(tr("Optical conrtoller R/W error.")); break;
          case RW_AMP_ERROR:   devErrorsText.append(tr("Temperature conrtoller R/W error.")); break;
          case RW_MOT_ERROR:   devErrorsText.append(tr("Motor conrtoller R/W error.")); break;
          case RW_TFT_ERROR:   devErrorsText.append(tr("TFT conrtoller R/W error.")); break;
          case TOUT_OPT_ERROR:   devErrorsText.append(tr("Optical conrtoller timeout error.")); break;
          case TOUT_AMP_ERROR:   devErrorsText.append(tr("Temperature conrtoller timeout error.")); break;
          case TOUT_MOT_ERROR:   devErrorsText.append(tr("Motor conrtoller timeout error.")); break;
          case TOUT_TFT_ERROR:   devErrorsText.append(tr("TFT conrtoller timeout error.")); break;
          case INCORRECT_CAN_DEVICE_ERROR: devErrorsText.append(tr("Set incorrect can device.")); break;
          case RW_USBBULK_ERROR: devErrorsText.append(tr("Usb bulk R/W error.")); break;
          case CMD_USBBULK_ERROR: devErrorsText.append(tr("Device hardware internal error.")); break;
          case GETFRAME_ERROR: devErrorsText.append(tr("Get data from network don't correspond of requset.")); break;
          case OPENINRUN_ERROR: devErrorsText.append(tr("Can't open cover in RUN time.")); break;
          case MOTORALARM_ERROR: devErrorsText.append(tr("Motor device alarm.")); break;
          case STARTWITHOPENCOVER_ERROR: devErrorsText.append(tr("Close cover before start RUN.")); break;
          case PRERUNFAULT_ERROR: devErrorsText.append(tr("Found unremovable error before RUN.")); break;
          case PRERUNSEMIFAULT_ERROR: devErrorsText.append(tr("Found removable error before RUN.")); break;
          case PRERUNMEMTEST_ERROR: devErrorsText.append(tr("Memory test error befor RUN.")); break;
          case PRERUNANALYSE_ERROR:devErrorsText.append(tr("Tset get valid measure data error befor RUN."));  break;
          case MOTORDRVLED_ERROR: devErrorsText.append(tr("Found error in setup of LED filters.")); break;
          case MEASHIGHTUBE_ERROR: devErrorsText.append(tr("Found error on measure high of tube.")); break;
          case GETAMPPROG_ERROR: devErrorsText.append(tr("Found error when get amplification program for simple mode.")); break;
          default:devErrorsText.append(tr("Unknown Error."));
        }
        devErrorsText.last().append(tr(" Error code ")+QString("%1").arg(i));
      }
  }
  return devErrorsText;
}
