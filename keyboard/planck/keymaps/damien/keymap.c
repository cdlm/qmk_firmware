// This is the canonical layout file for the Quantum project. If you want to add another keyboard,
// this is the style you want to emulate.

#include "planck.h"
#include "backlight.h"

// Layer names
#define QWY 0
#define CMK 1
#define DVK 2
#define _L_ 3
#define _R_ 4
#define _X_ 5

// Make KC_TRNS less visually obtrusive, so a fully transparent row looks like this:
// {KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____}
#define KC_____ KC_TRNS

// Mac-specific interpretation of PC keys for screen brightness
#define KC_BRUP KC_SLCK
#define KC_BRDN KC_PAUS

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[QWY] = { /* Qwerty */
  {KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC},
  {KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT},
  {KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_SLSH, KC_COMM, KC_DOT,  KC_GRV },
  {KC_ESC,  KC_HYPR, KC_LALT, KC_LGUI, MO(_L_), MT(MOD_RSFT, KC_ENT),
                                                         KC_SPC,  MO(_R_), KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT}
},
[_R_] = { /* RIGHT */
  {KC_____, KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_____, KC_____, KC_KP_7, KC_KP_8, KC_KP_9, KC_____, KC_____},
  {KC_____, KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_____, KC_____, KC_KP_4, KC_KP_5, KC_KP_6, KC_____, KC_____},
  {KC_____, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_____, KC_KP_0, KC_KP_1, KC_KP_2, KC_KP_3, KC_PDOT, KC_____},
  {KC_____, KC_____, KC_____, KC_____, MO(_X_), KC_PENT, KC_R,    MO(_R_), KC_MPLY, KC_VOLD, KC_VOLU, KC_MUTE}
},
[_L_] = { /* LEFT */
  {KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_LBRC, KC_RBRC, KC_DEL },
  {KC_____, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_____},
  {KC_____, KC_UNDO, KC_CUT, KC_COPY, KC_PASTE, KC_____, KC_____, KC_____, KC_BSLS, KC_MINS, KC_EQL,  KC_____},
  {M(1),    KC_____, KC_____, KC_____, MO(_L_), KC_____, KC_L,    MO(_X_), KC_HOME, KC_PGDN, KC_PGUP, KC_END }
},
[_X_] = { /* BOTH */
  {KC_____, KC_____, KC_____, KC_____, DEBUG,   KC_____, KC_____, RESET,   KC_____, KC_____, KC_____, KC_____},
  {KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____},
  {KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____},
  {KC_____, KC_____, KC_____, KC_____, MO(_X_), KC_____, KC_X,    MO(_X_), KC_____, KC_____, KC_____, KC_____}
}
// [CMK] = { /* Colemak */
// {KC_TAB,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,    KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, KC_BSPC},
// {KC_ESC,  KC_A,    KC_R,    KC_S,    KC_T,    KC_D,    KC_H,    KC_N,    KC_E,    KC_I,    KC_O,    KC_QUOT},
// {KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT },
// {M(0),    KC_LCTL, KC_LALT, KC_LGUI, MO(_LW), KC_SPC,  KC_SPC,  MO(_RS), KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT}
// },
// [DVK] = { /* Dvorak */
// {KC_TAB,  KC_QUOT, KC_COMM, KC_DOT,  KC_P,    KC_Y,    KC_F,    KC_G,    KC_C,    KC_R,    KC_L,    KC_BSPC},
// {KC_ESC,  KC_A,    KC_O,    KC_E,    KC_U,    KC_I,    KC_D,    KC_H,    KC_T,    KC_N,    KC_S,    KC_SLSH},
// {KC_LSFT, KC_SCLN, KC_Q,    KC_J,    KC_K,    KC_X,    KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,    KC_ENT },
// {M(0),    KC_LCTL, KC_LALT, KC_LGUI, MO(_LW), KC_SPC,  KC_SPC,  MO(_RS), KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT}
// },
};

const uint16_t PROGMEM fn_actions[] = {

};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  // MACRODOWN only works in this function
  switch(id) {
  case 0:
    if (record->event.pressed) {
      register_code(KC_RSFT);
      backlight_step();
    } else {
      unregister_code(KC_RSFT);
    }
    break;
  case 1:
    println("plop");
    break;
  }
  return MACRO_NONE;
};
