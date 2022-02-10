#include "_Plugin_Helper.h"
#ifdef USES_P200

// #######################################################################################################
// ################################## Plugin 200: VE.DIRECT ##############################################
// #######################################################################################################

# define PLUGIN_200
# define PLUGIN_ID_200         200
# define PLUGIN_NAME_200       "VE.Direct [Testing]"
# define PLUGIN_VALUENAME1_200 "Block"

boolean Plugin_200_init = false;

boolean Plugin_200(uint8_t function, struct EventStruct *event, String& string)
{
  boolean success = false;

  switch (function)
  {
    case PLUGIN_DEVICE_ADD:
    {
      Device[++deviceCount].Number           = PLUGIN_ID_200;
      Device[deviceCount].Type               = DEVICE_TYPE_SINGLE;
      Device[deviceCount].VType              = Sensor_VType::SENSOR_TYPE_STRING;
      Device[deviceCount].Ports              = 0;
      Device[deviceCount].PullUpOption       = false;
      Device[deviceCount].InverseLogicOption = false;
      Device[deviceCount].FormulaOption      = false;
      Device[deviceCount].DecimalsOnly       = false;
      Device[deviceCount].ValueCount         = 1;
      Device[deviceCount].SendDataOption     = true;
      Device[deviceCount].TimerOption        = false;
      Device[deviceCount].GlobalSyncOption   = true;
      break;
    }

    case PLUGIN_GET_DEVICENAME:
    {
      string = F(PLUGIN_NAME_200);
      break;
    }

    case PLUGIN_GET_DEVICEVALUENAMES:
    {
      strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[0], PSTR(PLUGIN_VALUENAME1_200));
      break;
    }

   case PLUGIN_GET_DEVICEGPIONAMES:
    {
      event->String1 = formatGpioName_input(F("RX (VE.Direct)"));
      break;
    }

    case PLUGIN_INIT:
    {
      Plugin_200_init = true;

      success = true;
      break;
    }

    case PLUGIN_WEBFORM_LOAD:
    {
      // @giig1967g: set current task value for taking actions after changes in the task gpio
      const uint32_t key = createKey(PLUGIN_ID_200, CONFIG_PIN1);

      auto it = globalMapPortStatus.find(key);
      if (it != globalMapPortStatus.end()) {
        it->second.previousTask = event->TaskIndex;
      }

      success = true;
      break;
    }

    case PLUGIN_WEBFORM_SAVE:
    {
      // check if a task has been edited and remove 'task' bit from the previous pin
      for (std::map<uint32_t, portStatusStruct>::iterator it = globalMapPortStatus.begin(); it != globalMapPortStatus.end(); ++it) {
        if ((it->second.previousTask == event->TaskIndex) && (getPluginFromKey(it->first) == PLUGIN_ID_200)) {
          globalMapPortStatus[it->first].previousTask = -1;
          removeTaskFromPort(it->first);
          break;
        }
      }
      success = true;
      break;
    }

  }
  return success;
}

#endif // USES_P200
