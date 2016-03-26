// This is the canonical layout file for the Quantum project. If you want to add
// another keyboard, this is the style you want to emulate.

#include "planck.h"
#include "backlight.h"
#include "action_layer.h"
#include "timer.h"

// Layer names
#define QWY 0
#define CMK 1
#define DVK 2
#define _L_ 3
#define _R_ 4
#define _X_ 5

// Make KC_TRNS less visually obtrusive to make actual mappings more obvious
#define _______ KC_TRNS

// Mac-specific interpretation of PC keys for screen brightness
#define KC_BRUP KC_SLCK
#define KC_BRDN KC_PAUS

// short names for long key action
#define SPC_SFT MT(MOD_RSFT, KC_SPC)
#define TAB_ALT MT(MOD_RALT, KC_TAB)
#define _R__ENT LT(_R_, KC_ENT)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [QWY] = { /* Qwerty base layer */
    {KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC},
    {KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_GRV },
    {KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_MINS, KC_QUOT},
    {KC_ESC,  KC_HYPR, KC_LALT, KC_LGUI, M(_L_),  KC_RALT, KC_SPC,  M(_R_),  KC_SLSH, KC_BSLS, KC_EQL,  KC_ENT }
  },
  [_L_] = { /* LEFT: numbers and occasional keys */
    {_______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______},
    {_______, _______, _______, _______, _______, _______, _______, KC_LBRC, KC_LPRN, KC_RPRN, KC_RBRC, _______},
    {_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______},
    {_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______}
  },
  [_R_] = { /* RIGHT: navigation */
    {_______, KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_DEL },
    {_______, KC_F5,   KC_F6,   KC_F7,   KC_F8,   _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_PENT},
    {_______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   _______, _______, _______, _______, _______, _______, _______},
    {_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______}
  },
  [_X_] = { /* BOTH: layer for non-typing commands (media, keyboard setup, etc) */
    {BL_STEP, _______, _______, _______, _______, _______, _______, _______, _______, KC_VOLD, KC_VOLU, KC_MUTE},
    {_______, _______, _______, _______, _______, DEBUG,   RESET,   _______, _______, KC_MPRV, KC_MNXT, KC_MPLY},
    {_______, _______, _______, _______, _______, KC_BTLD, _______, _______, _______, _______, _______, _______},
    {KC_PWR,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______}
  }
};

const uint16_t PROGMEM fn_actions[] = {

};

/* based on the tri-layer code by Jack
   https://www.reddit.com/r/olkb/comments/42hs00/debug_support_console_activation_how_is_it/czaqyys
*/

int combi_layers = 2;

void update_combi_layer(keyrecord_t *record, int combi_layer, int single_layer) {
  if (record->event.pressed) {
    layer_on(single_layer);
    combi_layers--;
  } else {
    layer_off(single_layer);
    combi_layers++;
  }

  if (combi_layers <= 0) {
    layer_on(combi_layer);
  } else {
    layer_off(combi_layer);
  }
}

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt) {
  // TODO how to pass opt? macro id should not be layer id…
  switch(id) {
  case _L_:
  case _R_:
    update_combi_layer(record, _X_, id);
    break;
  }
  return MACRO_NONE;
}

/* backlight animation experiment
 */

#define ANIM_FPS 50

extern const uint16_t backlight_gamma[]; // values below
static uint16_t animation_timer;
static uint16_t animate = 0;
void animate_backlight(uint16_t delta);

void matrix_init_user(void) {
  animation_timer = timer_read();
}

void matrix_scan_user(void) { // called approx. every millisecond
  uint16_t delta = timer_elapsed(animation_timer);
  if (delta > (1000 / ANIM_FPS)) {
    animation_timer = timer_read();
    animate_backlight(delta);
  }
}

#define CHANNEL OCR1C

void backlight_adjust(uint16_t level) {
  // Drive LED pin low to prevent backlight blink on lowest level
  PORTB &= ~(_BV(PORTB7));
  CHANNEL = level;
  // Turn PWM control off for brightness zero (and back on)
  if (level == 0) {
    TCCR1A &= ~(_BV(COM1C1));
  } else {
    TCCR1A |= _BV(COM1C1);
  }
}

// linear sawtooth / ramp animation function
void step_ramp(uint16_t *value, uint16_t delta) {
  *value += delta << 3;
}

// continuously decay to zero
// animate should be reset to a high value at each keypress fot this to have any interest :)
void step_decay(uint16_t *value, uint16_t delta) {
  uint16_t decay = *value >> 6;
  *value = (*value >= decay) ? *value - decay : 0;
}

void animate_backlight(uint16_t delta) {
  // step_ramp(&animate, delta);
  step_decay(&animate, delta);

  uint8_t interp_index = animate >> 8;
  uint8_t interp_mu = animate & 0xFF;

  uint16_t brght_inf = (interp_index == 0) ? 0 : pgm_read_word(&backlight_gamma[interp_index - 1]);
  uint16_t brght_sup = pgm_read_word(&backlight_gamma[interp_index]);

  uint16_t brightness = brght_inf + ((interp_mu * (brght_sup - brght_inf)) >> 8);

  backlight_adjust(brightness);
}

const uint16_t PROGMEM backlight_gamma[] = { // values generated using gamma.rb
  28,    57,    85,    113,   142,   170,   199,   227,   255,   284,   312,   340,   369,   397,   426,   454,
  482,   511,   539,   567,   595,   625,   655,   686,   718,   751,   785,   821,   857,   894,   933,   972,
  1012,  1054,  1097,  1141,  1186,  1232,  1279,  1328,  1378,  1429,  1481,  1535,  1590,  1646,  1703,  1762,
  1822,  1883,  1946,  2010,  2076,  2143,  2211,  2281,  2352,  2425,  2500,  2575,  2653,  2731,  2812,  2894,
  2977,  3062,  3149,  3237,  3327,  3419,  3512,  3607,  3704,  3802,  3902,  4004,  4108,  4213,  4320,  4429,
  4540,  4652,  4767,  4883,  5001,  5121,  5243,  5367,  5493,  5621,  5751,  5882,  6016,  6152,  6289,  6429,
  6571,  6715,  6861,  7009,  7159,  7312,  7466,  7623,  7782,  7943,  8106,  8272,  8439,  8609,  8781,  8956,
  9133,  9312,  9493,  9677,  9863,  10052, 10243, 10436, 10632, 10830, 11030, 11234, 11439, 11647, 11858, 12071,
  12286, 12504, 12725, 12948, 13174, 13403, 13634, 13868, 14104, 14343, 14585, 14830, 15077, 15327, 15579, 15835,
  16093, 16354, 16618, 16885, 17154, 17426, 17702, 17980, 18261, 18545, 18831, 19121, 19414, 19710, 20008, 20310,
  20615, 20922, 21233, 21547, 21864, 22184, 22507, 22833, 23163, 23495, 23831, 24170, 24512, 24857, 25206, 25558,
  25913, 26271, 26632, 26997, 27366, 27737, 28112, 28490, 28872, 29257, 29645, 30037, 30432, 30831, 31233, 31639,
  32048, 32461, 32877, 33297, 33720, 34147, 34578, 35012, 35450, 35891, 36336, 36785, 37237, 37693, 38153, 38616,
  39083, 39554, 40029, 40507, 40990, 41476, 41966, 42460, 42957, 43459, 43964, 44473, 44987, 45504, 46025, 46550,
  47079, 47612, 48149, 48690, 49235, 49785, 50338, 50895, 51457, 52022, 52592, 53166, 53744, 54326, 54912, 55503,
  56097, 56696, 57300, 57907, 58519, 59135, 59755, 60380, 61009, 61642, 62280, 62922, 63569, 64220, 64875, 65535
};
