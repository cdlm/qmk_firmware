// This is the canonical layout file for the Quantum project. If you want to add
// another keyboard, this is the style you want to emulate.

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

// TODO check matrix_scan_user to animate lighting
