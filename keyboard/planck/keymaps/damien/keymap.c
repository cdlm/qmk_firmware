// This is the canonical layout file for the Quantum project. If you want to add another keyboard,
// this is the style you want to emulate.

#include "planck.h"
#include "backlight.h"
#include "action_layer.h"

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
  {KC_____, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_____},
  {KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_LBRC, KC_LPRN, KC_RPRN, KC_RBRC, KC_____},
  {KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____},
  {KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____}
},
[_R_] = { /* RIGHT: navigation */
  {KC_____, KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_____, KC_____, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_DEL },
  {KC_____, KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_____, KC_____, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_PENT},
  {KC_____, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____},
  {KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____}
},
[_X_] = { /* BOTH: layer for non-typing commands (media, keyboard setup, etc) */
  {BL_STEP, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_VOLD, KC_VOLU, KC_MUTE},
  {KC_____, KC_____, KC_____, KC_____, KC_____, DEBUG,   RESET,   KC_____, KC_____, KC_MPRV, KC_MNXT, KC_MPLY},
  {KC_____, KC_____, KC_____, KC_____, KC_____, KC_BTLD, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____},
  {KC_PWR,  KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____, KC_____}
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

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt) { // TODO how to pass opt? macro id should not be layer id…
  switch(id) {
  case _L_:
  case _R_:
    update_combi_layer(record, _X_, id);
    break;
  }
  return MACRO_NONE;
}

// TODO check matrix_scan_user to animate lighting
