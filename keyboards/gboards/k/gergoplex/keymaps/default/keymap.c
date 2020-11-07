/* Good on you for modifying your layout! if you don't have
 * time to read the QMK docs, a list of keycodes can be found at
 *
 * https://github.com/qmk/qmk_firmware/blob/master/docs/keycodes.md
 *
 * There's also a template for adding new layers at the bottom of this file!
 */

#include QMK_KEYBOARD_H
#include "g/keymap_combo.h"

#define BASE 0 // default layer
#define NAVI 1 // nav layer
#define SYMB 2 // symbol layer
#define NUMB 3 // number layer
#define FUN  4 // function layer

/* Combomap
 *
 * ,-----------------------------.       ,--------------------------------.
 * |      |    ESC    |     |     |      |     |    ESC    |    BSLH      |
 * |-----+-----+-----+-----+------|      |--------------------------------|
 * |      |   BSPC   ENT    |     |      |    LES   COLN  GRT    |        |
 * |-----+-----+-----+--RMB+-LMB--+        |--------------------------------|
 * |      |   MINS    |     |     |      |    QUO   UNDR   |     |        |
 * `------+-----+-----+------+----'        `--------------------------------'
 *  .-------------------------.           .-----------------.
 *  |        |       |        |           |        |    |   |
 *  '-------------------------'           '-----------------'
 */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[BASE] = LAYOUT_gergoplex(
KC_Q,               KC_W,               KC_E,               KC_R,               KC_T,                  KC_Y,                KC_U,               KC_I,                  KC_O,                 KC_P,
KC_A,               KC_S,               KC_D,               KC_F,               KC_G,                  KC_H,                KC_J,               KC_K,                  KC_L,                 LT(FUN,KC_SCLN),
MT(MOD_LSFT,KC_Z),  MT(MOD_LALT,KC_X),  MT(MOD_LCTL,KC_C),  MT(MOD_LGUI,KC_V),  MEH_T(KC_B),           MEH_T(KC_N),         MT(MOD_LGUI,KC_M),  MT(MOD_LCTL,KC_COMM),  MT(MOD_LALT,KC_DOT),  MT(MOD_RSFT,KC_SLSH),
LT(NUMB,KC_TAB),    LT(SYMB,KC_ESC),    LT(NAVI,KC_SPC),    LT(NAVI,KC_ENT),    MT(MOD_LCTL,KC_BSPC),  MT(MOD_LGUI,KC_DEL)
),
[NAVI] = LAYOUT_gergoplex(
KC_NO,  KC_NO,    KC_UP,    KC_NO,         KC_BTN3,  KC_VOLD,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_VOLU,
KC_NO,  KC_LEFT,  KC_DOWN,  KC_RIGHT,      KC_BTN4,  KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  KC_MUTE,
KC_NO,  KC_NO,    KC_NO,    KC_NO,         KC_BTN5,  KC_MS_L,  KC_MS_D,  KC_MS_U,  KC_MS_R,  KC_NO,
KC_NO,  KC_NO,    KC_SPC,   LGUI(KC_SPC),  KC_BTN1,  KC_BTN2
),
[SYMB] = LAYOUT_gergoplex(
KC_NO,  KC_NO,    KC_UP,    KC_NO,     KC_NO,    KC_PLUS,  KC_EXLM,  KC_AT,    KC_HASH,  KC_PIPE,
KC_NO,  KC_LEFT,  KC_DOWN,  KC_RIGHT,  KC_NO,    KC_UNDS,  KC_DLR,   KC_PERC,  KC_CIRC,  KC_DQUO,
KC_NO,  KC_NO,    KC_NO,    KC_NO,     KC_SPC,   KC_TILD,  KC_AMPR,  KC_ASTR,  KC_LPRN,  KC_RPRN,
KC_NO,  KC_NO,    KC_NO,    KC_ENT,    KC_LCBR,  KC_RCBR
),
[NUMB] = LAYOUT_gergoplex(
KC_NO,  KC_NO,    KC_UP,    KC_NO,     KC_NO,    KC_EQL,   KC_1,  KC_2,  KC_3,  KC_BSLS,
KC_NO,  KC_LEFT,  KC_DOWN,  KC_RIGHT,  KC_NO,    KC_MINS,  KC_4,  KC_5,  KC_6,  KC_QUOT,
KC_NO,  KC_NO,    KC_NO,    KC_NO,     KC_SPC,   KC_GRV,   KC_7,  KC_8,  KC_9,  KC_0,
KC_NO,  KC_NO,    KC_NO,    KC_ENT,    KC_LBRC,  KC_RBRC
),
[FUN] = LAYOUT_gergoplex(
KC_PSCR,  KC_F1,   KC_F2,   KC_F3,  KC_F12,  KC_VOLD,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_VOLU,
KC_SLCK,  KC_F4,   KC_F5,   KC_F6,  KC_F11,  KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  KC_MUTE,
KC_PAUS,  KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,
KC_CAPS,  KC_INS,  KC_SPC,  KC_NO,  RESET,   KC_NO
)
};
