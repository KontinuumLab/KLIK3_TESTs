/*
 * Copyright (c) 2024 Marcel Licence
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Dieses Programm ist Freie Software: Sie können es unter den Bedingungen
 * der GNU General Public License, wie von der Free Software Foundation,
 * Version 3 der Lizenz oder (nach Ihrer Wahl) jeder neueren
 * veröffentlichten Version, weiter verteilen und/oder modifizieren.
 *
 * Dieses Programm wird in der Hoffnung bereitgestellt, dass es nützlich sein wird, jedoch
 * OHNE JEDE GEWÄHR,; sogar ohne die implizite
 * Gewähr der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
 * Siehe die GNU General Public License für weitere Einzelheiten.
 *
 * Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
 * Programm erhalten haben. Wenn nicht, siehe <https://www.gnu.org/licenses/>.
 */

/**
 * @file ml_synth_sampler_example.ino
 * @author Marcel Licence
 * @date 26.11.2021
 *
 * @brief   This is the main project file of the soundfont, wav-files loading sampler project
 *          It should be compatible with ESP32, Raspberry Pi Pico / RP2040, others my follow in future
 */


#ifdef __CDT_PARSER__
#include <cdt.h>
#endif


#include "config.h"

#include "wav_to_sampler.h"
#include "sf_to_sampler.h"

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>


/*
 * Library can be found on https://github.com/marcel-licence/ML_SynthTools
 */
#include <ml_types.h>

#ifdef REVERB_ENABLED
#include <ml_reverb.h>
#endif
#ifdef MAX_DELAY_Q
#include <ml_delay_q.h>
#endif
#include <ml_phaser.h>
#include <ml_tremolo.h>
#include <ml_lfo.h>
#include <ml_system.h>
#include <ml_vibrato.h>
#include <ml_pitch_shifter.h>
#include <ml_utils.h>


#ifdef OLED_OSC_DISP_ENABLED
#include <ml_scope.h>
#endif

/*
 * following modules will be available in version 1.2.0
 */
#include <ml_sampler.h>
#include <ml_soundfont.h>


#include "fs/fs_access.h"

/* centralized modules */
#define ML_SYNTH_INLINE_DECLARATION
#include <ml_inline.h>
#undef ML_SYNTH_INLINE_DECLARATION


#if (defined ARDUINO_GENERIC_F407VGTX) || (defined ARDUINO_DISCO_F407VG)
#include <Wire.h> /* todo remove, just for scanning */
#endif


float lfo1_buffer[SAMPLE_BUFFER_SIZE];
ML_LFO lfo1(SAMPLE_RATE, lfo1_buffer, SAMPLE_BUFFER_SIZE);

float lfo2_buffer[SAMPLE_BUFFER_SIZE];
float lfo2_buffer_scale[SAMPLE_BUFFER_SIZE];
ML_LFO lfo2(SAMPLE_RATE, lfo2_buffer, SAMPLE_BUFFER_SIZE);

#ifdef REVERB_ENABLED
ML_Tremolo tremolo(SAMPLE_RATE);
#endif

ML_Vibrato vibrato(SAMPLE_RATE);

ML_PitchShifter pitchShifter(SAMPLE_RATE);

char shortName[] = "ML_Sampler";


#ifdef SAMPLER_DYNAMIC_BUFFER_SIZE
int16_t *sample_buffer;
#endif



// ############### MY VARIABLES ###################

#include <EEPROM.h>
#include <Wire.h>
#include "Adafruit_MPR121.h"
// ESP-MIDI:
#include <BLEMIDI_Transport.h>
#include <hardware/BLEMIDI_ESP32.h>
BLEMIDI_CREATE_INSTANCE("KLIK3_HANDPAN", MIDI)

bool isConnected = false;
int MIDIchannel = 1;

Adafruit_MPR121 capA = Adafruit_MPR121();
uint16_t MPRbinaryA = 0;
uint16_t lastMPRbinaryA = 0;
uint16_t mprReadings[9];
uint16_t lastMprReadings[9];
uint16_t mprReadingsMin[9] = {250, 250, 250, 250, 250, 250, 250, 250, 250};
uint16_t mprReadingsMax[9] = {100, 100, 100, 100, 100, 100, 100, 100, 100};



// PINS:

int audioOn = 1;
int midiPin = 27;

// Extension Pins:
int scaleBtn1 = 17; //39;
int scaleBtn2 = 16; //34;
int calBtn = 3; //35;

int scaleLED1 = 32;
int scaleLED2 = 33;
int scaleLED3 = 25;
int calLED = 26;

int instBtn = 2; //12; // is this the correct pin?




int ledBrightness = 20;
unsigned long ledPinTimer;
int scaleLedOn = 0;

int scaleBtn1Pressed = 0;
int scaleBtn2Pressed = 0;
int lastScaleBtn1Pressed = 0;
int lastScaleBtn2Pressed = 0;

int currentScale = 0;

unsigned long currentTime = 0;
unsigned long lastTime = 0;

int scale[9] = {2, 9, 14, 15, 18, 19, 21, 22, 26};

#include "scales.h"
#include "memory.h"
#include "calibration.h"
#include "synthSelect.h"
#include "klikInstrument.h"


//############# END MY VARIABLES ###################


// void scaleBtn1Isr(){
//   //analogWrite(scaleLED1, digitalRead(scaleBtn1)*10);
// }

// void scaleBtn2Isr(){
//   //analogWrite(scaleLED2, digitalRead(scaleBtn2)*10);
// }

// void calBtnIsr(){
//   if(digitalRead(calBtn) == 0){
//     calibration();
//   }
// }



/**
    @brief This function contains the setup routines.
 */
void setup(void)
{
    /*
     * this code runs once
     */
    Serial.begin(SERIAL_BAUDRATE);
    //while (!Serial);
    delay(1000);
    Serial.printf("Hallo\n");
    delay(1000);
    Serial.printf("ml_sampler\n");

#if defined ESP_IDF_VERSION_MAJOR and  defined ESP_IDF_VERSION_MINOR and defined ESP_IDF_VERSION_PATCH
    /* @see https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/misc_system_api.html#sdk-version */
    Serial.printf("ESP IDF Version: %d, %d, %d\n", ESP_IDF_VERSION_MAJOR, ESP_IDF_VERSION_MINOR, ESP_IDF_VERSION_PATCH);
#endif
#ifdef CHIP_FEATURE_EMB_FLASH
    Serial.printf("Chip has embedded flash memory.\n");
#endif
#ifdef CHIP_FEATURE_WIFI_BGN
    Serial.printf("Chip has 2.4GHz WiFi.\n");
#endif
#ifdef CHIP_FEATURE_BLE
    Serial.printf("Chip has Bluetooth LE.\n");
#endif
#ifdef CHIP_FEATURE_BT
    Serial.printf("Chip has Bluetooth Classic.\n");
#endif
#ifdef CHIP_FEATURE_IEEE802154
    Serial.printf("Chip has IEEE 802.15.4.\n");
#endif
#ifdef CHIP_FEATURE_EMB_PSRAM
    Serial.printf("Chip has embedded psram.\n");
#endif


#ifdef ML_BOARD_SETUP
    Board_Setup();
#else
    Audio_Setup();

    /*
     * setup midi module / rx port
     */
    Midi_Setup();

#ifdef MIDI_USB_ENABLED
    Midi_Usb_Setup();
#endif
#endif


#ifdef ESP32
#ifndef ML_BOARD_SETUP
    Midi_Setup();
#endif
#endif


#ifdef BLINK_LED_PIN
    Blink_Setup();
    Blink_Fast(3);
#endif


    Serial.println();
    delay(250);

    Serial.printf("Loading data\n");


    Serial.printf("Sampler Example\n");


    Sampler_Init(SAMPLE_RATE);

#ifdef SAMPLER_STATIC_BUFFER_SAMPLE_CNT
    static Q1_14 buffer[SAMPLER_STATIC_BUFFER_SAMPLE_CNT];
    Sampler_UseStaticBuffer(buffer, SAMPLER_STATIC_BUFFER_SAMPLE_CNT);
#endif


#ifdef ESP32
#ifndef BOARD_HAS_PSRAM
#warning PSRAM required for this project to work
#endif
    if (ESP.getFreePsram() == 0)
    {
        Serial.printf("PSRAM has no free Memory! Please ensure PSRAM is enabled and also available on your ESP32");
        while (true)
        {
            delay(1000);
        }
    }

    uint32_t storageBytes = ESP.getFreePsram() ;

    Serial.printf("Try to allocate %d bytes\n", storageBytes);

    static uint8_t *storage = (uint8_t *)ps_calloc(storageBytes, 1);
    if (storage == NULL)
    {
        Serial.printf("Not able to allocate the complete PSRAM buffer!\nNow trying to reduce the allocation buffer size...\n");

        /*
         * for some reason using a newer ESP32 board library you cannot allocate the complete PSRAM memory
         * this loop will decrease the buffer size and repeat the allocation step again until it is successful
         */
        while ((storage == NULL))
        {
            storageBytes -= 1;
            storage = (uint8_t *)ps_calloc(storageBytes, 1);
        }
        Serial.printf("alloced %" PRIu32 " bytes\n", storageBytes);
    }

    Sampler_SetSampleBuffer(storage, storageBytes);
#endif


#ifdef MAX_DELAY_Q
    /*
     * Prepare a buffer which can be used for the delay
     */
#ifdef ESP32
    static int16_t *delBuffer1 = (int16_t *)malloc(sizeof(int16_t) * MAX_DELAY_Q);
    static int16_t *delBuffer2 = (int16_t *)malloc(sizeof(int16_t) * MAX_DELAY_Q);
#else
    static int16_t delBuffer1[MAX_DELAY_Q];
    static int16_t delBuffer2[MAX_DELAY_Q];
#endif
    DelayQ_Init2(delBuffer1, delBuffer2, MAX_DELAY_Q);
#endif

    /*
     * Here is the interesting part loading some samples
     */


    Serial.printf("Loading data\n");

#ifdef SAMPLER_DYNAMIC_BUFFER_SIZE
    sample_buffer = (int16_t *)malloc(SAMPLER_DYNAMIC_BUFFER_SIZE);
    if (sample_buffer == NULL)
    {
        Serial.printf("Malloc failed for sample buffer");
    }
#endif


//############## MY SETUP ##################
  EEPROM.begin(256);
  Wire.begin(21, 22);
  if (!capA.begin(0x5A)) {
    Serial.println("MPR121 on 0x5A NOT found");
  }
  else {
    Serial.println("MPR121 on 0x5A active");
  }

  capA.writeRegister(0x2E, 0b11111111);
  capA.writeRegister(0x32, 0b11111111);
  capA.writeRegister(0x35, 0b11111111);
  capA.setThresholds(50, 40);


  pinMode(instBtn, INPUT_PULLUP);
  pinMode(midiPin, INPUT_PULLUP);
  pinMode(scaleBtn1, INPUT_PULLUP);
  pinMode(scaleBtn2, INPUT_PULLUP);
  pinMode(calBtn, INPUT_PULLUP);
  audioOn = digitalRead(midiPin);

  if(audioOn == 0){
    MIDI.begin();
    BLEMIDI.setHandleConnected(OnConnected);
    BLEMIDI.setHandleDisconnected(OnDisconnected);
  }

  delay(100); 
  loadSettings();
  for(int i = 0; i < 9; i++){
    scale[i] = scales[i]; //Get notes from memory scale   
  }
  //attachInterrupt(scaleBtn1, scaleBtn1Isr, CHANGE);
  //attachInterrupt(scaleBtn2, scaleBtn2Isr, CHANGE);
  //attachInterrupt(calBtn, calBtnIsr, CHANGE);

  analogWrite(scaleLED1, 0);
  analogWrite(scaleLED2, 0);
  analogWrite(scaleLED3, 0);
  analogWrite(calLED, 0);


//############ END MY SETUP #################


    FS_Setup();

    Sampler_AllNotesOff();

    Serial.printf("setup done!\n");
    // From Marcel:
       // WavToSmpl_FileToSingleNote(FS_ID_LITTLEFS, "/PappRohrSample.wav", W2S_ALL_NOTES);
        WavToSmpl_FileToSingleNote(FS_ID_LITTLEFS, "/Klank-5.wav", W2S_ALL_NOTES);
        //WavToSmpl_FileToSingleNote(FS_ID_LITTLEFS, "/hang-drum-3_klik3.wav", W2S_ALL_NOTES);
        
        //WavToSmpl_FileToSingleNote(FS_ID_LITTLEFS, "/F3_16bit_klik3.wav", W2S_ALL_NOTES);


        //SF2ToSmpl_LoadAllSamplesFromSF(FS_ID_LITTLEFS, "/Trumpet.sf2");
        Sampler_InstrumentDone();
    //Sampler_NoteOn(0, 69, 127);


#ifdef MIDI_STREAM_PLAYER_ENABLED
    MidiStreamPlayer_Init();
    char midiFile[] = "/esp32_sampler_demo.mid";
    //char midiFile[] = "/esp32_sampler_demo2.mid";
    MidiStreamPlayer_PlayMidiFile_fromLittleFS(midiFile, 2);
#endif

#ifdef ESP32
    Core0TaskInit();
#else
    //#error only supported by ESP32 platform
#endif
}


#ifdef ESP32
/*
 * Core 0
 */
/* this is used to add a task to core 0 */
TaskHandle_t Core0TaskHnd;


bool setup_toggle = false;

inline
void Core0TaskInit(void)
{
    /* we need a second task for the terminal output */
    xTaskCreatePinnedToCore(Core0Task, "CoreTask0", 8000, NULL, 4, &Core0TaskHnd, 0);
}

void Core0TaskSetup(void)
{
#ifdef STRIP_ENABLED
    // StripSetup();
#endif


    /*
     * init your stuff for core0 here
     */

#ifdef ADC_ENABLED
    AdcSetup();
#endif
}

void Core0TaskLoop(void)
{
  if(digitalRead(calBtn) == 0){
    calibration();
  }
  if(digitalRead(instBtn) == 0){
    synthSelect();
  }
  klikInstrument();
  //LEDctrl();
  scaleAdjust();

  if(scaleLedOn == 1 && millis() - 1000 > ledPinTimer){
    scaleLedOn = 0;
    analogWrite(scaleLED1, 0);
    analogWrite(scaleLED2, 0);
    analogWrite(scaleLED3, 0);
  }

}

void Core0Task(void *parameter)
{
    Core0TaskSetup();

    while (true)
    {
        Core0TaskLoop();

        /* this seems necessary to trigger the watchdog */
        // delay(1);
        // yield();
    }
}
#endif /* ESP32 */


/**
    @brief This function will be called every second.
        In case of an overload the function will not called so often
  */
void loop_1Hz(void)
{
#ifdef BLINK_LED_PIN
    Blink_Process();
#endif
}

float preAmp = 0.125f;
float postAmp = 1.0f;
float bypassAmp = 1.0f;
float rotLevel = 1.0f;
bool hq_enabled = true;
float inputGain = 1.0f;

/**
    @brief This function contains the mainloop
 */
void loop(void)
{
    static int loop_cnt_1hz = 0; /*!< counter to allow 1Hz loop cycle */

    loop_cnt_1hz += SAMPLE_BUFFER_SIZE;

    if (loop_cnt_1hz >= SAMPLE_RATE)
    {
        loop_cnt_1hz -= SAMPLE_RATE;
        loop_1Hz();
    }

    /*
     * MIDI processing
     */
    Midi_Process();

#ifdef MIDI_VIA_USB_ENABLED
    UsbMidi_ProcessSync();
#endif

#ifdef MIDI_STREAM_PLAYER_ENABLED
    MidiStreamPlayer_Tick(SAMPLE_BUFFER_SIZE);
#endif

#ifdef MIDI_BLE_ENABLED
    midi_ble_loop();
#endif

#ifdef USB_HOST_ENABLED
    Usb_Host_Midi_loop();
#endif

#ifdef MIDI_USB_ENABLED
    Midi_Usb_Loop();
#endif

    Status_Loop(240000);
    Status_LoopMain();

    /*
     * And finally the audio stuff
     */
    Q1_14 left[SAMPLE_BUFFER_SIZE], right[SAMPLE_BUFFER_SIZE];

    memset(left, 0, sizeof(left));
    memset(right, 0, sizeof(right));



    Sampler_Process(left, right, SAMPLE_BUFFER_SIZE);


#ifdef MAX_DELAY_Q
    /*
     * post process delay
     */
    DelayQ_Process_Buff(&left[0].s16, &right[0].s16, &left[0].s16, &right[0].s16, SAMPLE_BUFFER_SIZE);
#endif

    /*
     * Output the audio
     */
    Audio_Output(left, right);

}

/*
 * MIDI callbacks
 */
inline void AppBtn(uint8_t param, uint8_t value)
{
    if (value > 0)
    {

        switch (param)
        {
#ifdef ESP32_AUDIO_KIT
        case 0:
            ac101_setSourceMic();
            break;
        case 1:
            ac101_setSourceLine();
            break;
#endif
#if 0
        case 2:
            hq_enabled = true;
            break;
        case 3:
            hq_enabled = false;
            break;
#endif
        case 7:
            Sampler_DecSample();
            break;
        case 8:
            Sampler_IncSample();
            break;

        default:
            SoundFontSamplerCtrl(param + 9);
        }
    }
}

inline void AppBtnB(uint8_t param, uint8_t value)
{
    if (value > 0)
    {
        SoundFontSamplerCtrl(param);
    }
}

void AppSetInputGain(uint8_t unused __attribute__((unused)), uint8_t value)
{
    inputGain = log2fromU7(value, -16, 5);
    Status_ValueChangedFloat("Input", "Gain", inputGain);
}

#ifdef REVERB_ENABLED
void AppReverb_SetLevel(uint8_t param __attribute__((unused)), uint8_t value)
{
    Reverb_SetLevel(param, floatFromU7(value));
}
#endif

void AppTremolo_SetDepth(uint8_t param __attribute__((unused)), uint8_t value)
{
#ifdef REVERB_ENABLED
    tremolo.setDepth(floatFromU7(value));
#else
    (void)value;
#endif
}

void AppVibrato_SetDepth(uint8_t param __attribute__((unused)), uint8_t value)
{
    vibrato.setDepth(floatFromU7(value));
}

void AppVibrato_SetIntensity(uint8_t param __attribute__((unused)), uint8_t value)
{
    vibrato.setIntensity(floatFromU7(value));
}

/**
 * @brief
 *
 * @param unused
 * @param value
 */
void App_DelayQ_SetLength(uint8_t unused __attribute__((unused)), uint8_t value)
{
#ifdef MAX_DELAY_Q
    uint32_t val32 = value;
    val32 *= 189;
    DelayQ_SetLength(unused, val32);
#endif
}

/**
 * @brief
 *
 * @param slider
 * @param value
 */
void App_SliderSawCtrl(uint8_t slider, uint8_t value)
{
    Serial.printf("slider %d: %d\n", slider, value);
}


/**
 * @brief Implemented callback to display debug information of sampler module
 *
 */
void Sampler_DebugInt(const char *str __attribute__((unused)), uint32_t num __attribute__((unused)))
{
    //Serial.printf("%s: %" PRIu32 "\n", str, num);
}

void Sampler_DebugFloat(const char *str __attribute__((unused)), float num __attribute__((unused)))
{
    //Serial.printf("%s: %f\n", str, num);
}

void Lfo1_SetSpeed(uint8_t unused __attribute__((unused)), uint8_t value)
{
    float f = log10fromU7(value, -2, 3);
    lfo1.setFrequency(f);
    lfo2.setFrequency(f);
    Status_ValueChangedFloat("Lfo", "Frequency", f);
}

void PitchShifter_SetSpeed(uint8_t unused __attribute__((unused)), uint8_t value)
{
    float f = value;

    f -= 64.0f;

    f /= 12.0f;
    f = pow(2, f);

    pitchShifter.setSpeed(-f);
}

void PitchShifter_SetMix(uint8_t unused __attribute__((unused)), uint8_t value)
{
    pitchShifter.setMix(floatFromU7(value));
}

void PitchShifter_SetFeedback(uint8_t unused __attribute__((unused)), uint8_t value)
{
    pitchShifter.setFeedback(floatFromU7(value));
}

void OnConnected() {
  isConnected = true;
  //  digitalWrite(LED_BUILTIN, HIGH);
}

// -----------------------------------------------------------------------------
// Device disconnected
// -----------------------------------------------------------------------------
void OnDisconnected() {
  isConnected = false;
  //  digitalWrite(LED_BUILTIN, LOW);
}
