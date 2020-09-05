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
#define MOUR 1 // mouse layer
#define NAVR 2 // nav layer
#define NSSL 4 // symbol layer
#define NSL  5 // number layer
#define FUNL 5 // function layer

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
KC_Q,               KC_W,               KC_E,               KC_R,               KC_T,             KC_Y,            KC_U,               KC_I,                  KC_O,                 KC_P,
KC_A,               KC_S,               KC_D,               KC_F,               KC_G,             KC_H,            KC_J,               KC_K,                  KC_L,                 KC_SCLN,
MT(MOD_LSFT,KC_Z),  MT(MOD_LALT,KC_X),  MT(MOD_LCTL,KC_C),  MT(MOD_LGUI,KC_V),  MEH_T(KC_B),      MEH_T(KC_N),     MT(MOD_LGUI,KC_M),  MT(MOD_LCTL,KC_COMM),  MT(MOD_LALT,KC_DOT),  MT(MOD_RSFT,KC_SLSH),
LT(MOUR,KC_TAB),    LT(NAVR,KC_SPC),    KC_SPC,             LT(NSSL,KC_ENT),    LT(NSL,KC_BSPC),  LT(FUNL,KC_DEL)
),
[NAVR] = LAYOUT_gergoplex(
KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_ESC,   KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_VOLU,
KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  KC_VOLD,
KC_LSFT,  KC_LALT,  KC_LCTL,  KC_LGUI,  KC_MEH,   KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_MUTE,
KC_NO,    KC_NO,    KC_NO,    KC_ENT,   KC_BSPC,  KC_DEL
),
[MOUR] = LAYOUT_gergoplex(
KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    LGUI(KC_A),  LGUI(KC_Z),  LGUI(KC_X),  LGUI(KC_C),  LGUI(KC_V),
KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_WH_L,     KC_WH_D,     KC_WH_U,     KC_WH_R,     KC_NO,
KC_LSFT,  KC_LALT,  KC_LCTL,  KC_LGUI,  KC_MEH,   KC_MS_L,     KC_MS_D,     KC_MS_U,     KC_MS_R,     KC_NO,
KC_NO,    KC_NO,    KC_NO,    KC_BTN3,  KC_BTN1,  KC_BTN2
),
[FUNL] = LAYOUT_gergoplex(
KC_PSCR,  KC_F1,   KC_F2,   KC_F3,  KC_F12,  KC_NO,   KC_NO,    KC_NO,    KC_NO,    KC_NO,
KC_SLCK,  KC_F4,   KC_F5,   KC_F6,  KC_F11,  KC_NO,   KC_NO,    KC_NO,    KC_NO,    KC_NO,
KC_PAUS,  KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_MEH,  KC_LGUI,  KC_LCTL,  KC_LALT,  KC_LSFT,
KC_CAPS,  KC_SPC,  KC_SPC,  KC_NO,  KC_NO,   KC_NO
),
[NSL] = LAYOUT_gergoplex(
KC_BSLS,  KC_1,     KC_2,    KC_3,   KC_EQL,   KC_NO,   KC_NO,    KC_NO,    KC_NO,    KC_NO,
KC_QUOT,  KC_4,     KC_5,    KC_6,   KC_MINS,  KC_NO,   KC_NO,    KC_NO,    KC_NO,    KC_NO,
KC_GRV,   KC_7,     KC_8,    KC_9,   KC_0,     KC_MEH,  KC_LGUI,  KC_LCTL,  KC_LALT,  KC_LSFT,
KC_LBRC,  KC_RBRC,  KC_SPC,  KC_NO,  KC_NO,    KC_NO
),
[NSSL] = LAYOUT_gergoplex(
KC_PIPE,  KC_EXLM,  KC_AT,    KC_HASH,  KC_PLUS,  KC_NO,   KC_NO,    KC_NO,    KC_NO,    KC_NO,
KC_DQUO,  KC_DLR,   KC_PERC,  KC_CIRC,  KC_UNDS,  KC_NO,   KC_NO,    KC_NO,    KC_NO,    KC_NO,
KC_TILD,  KC_AMPR,  KC_ASTR,  KC_LPRN,  KC_RPRN,  KC_MEH,  KC_LGUI,  KC_LCTL,  KC_LALT,  KC_LSFT,
KC_LCBR,  KC_RCBR,  KC_SPC,   KC_NO,    KC_NO,    KC_NO
)
};
