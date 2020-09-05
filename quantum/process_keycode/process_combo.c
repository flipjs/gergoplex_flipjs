/* Copyright 2016 Jack Humbert
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "print.h"
#include "process_combo.h"
#include "action_tapping.h"

#ifndef COMBO_VARIABLE_LEN
__attribute__((weak)) combo_t key_combos[COMBO_COUNT] = {};
#else
extern combo_t  key_combos[];
extern int      COMBO_LEN;
#endif

__attribute__((weak)) void process_combo_event(uint16_t combo_index, bool pressed) {}

#ifdef COMBO_MUST_HOLD_PER_COMBO
__attribute__((weak)) bool get_combo_must_hold(uint16_t index, combo_t *combo) { return false; }
#endif

#ifdef COMBO_TERM_PER_COMBO
__attribute__((weak)) uint16_t get_combo_term(uint16_t index, combo_t *combo) { return COMBO_TERM; }
#endif

static uint16_t timer                 = 0;
static uint16_t  prepared_combo_index = -1;
static bool     is_active             = true;
static bool     b_combo_enable        = true;  // defaults to enabled
static combo_t  *prepared_combo       = NULL;
static uint16_t longest_term          = 0;
#define COMBO_PREPARED (prepared_combo && !prepared_combo->disabled)

static uint8_t buffer_size = 0;
static keyrecord_t key_buffer[MAX_COMBO_LENGTH];

static inline void send_combo(uint16_t keycode, bool pressed, uint16_t combo_index) {
    if (keycode) {
        keyrecord_t record = {
            .event = {
                .key = {.col = 254, .row = 254},
                .time = timer_read()|1,
                .pressed = pressed,
            },
            .keycode = keycode,
        };
#ifndef NO_ACTION_TAPPING
        action_tapping_process(record);
#else
        process_record(record);
#endif
    } else {
        process_combo_event(combo_index, pressed);
    }
}

void clear_combos(bool clear_state) {
    uint16_t index = 0;
    prepared_combo = NULL;
    prepared_combo_index = -1;
    longest_term = 0;
#ifndef COMBO_VARIABLE_LEN
    for (index = 0; index < COMBO_COUNT; ++index) {
#else
    for (index = 0; index < COMBO_LEN; ++index) {
#endif
        combo_t *combo = &key_combos[index];
        if (!combo->active)
            combo->disabled = false;
        if (clear_state && !combo->active) {
            combo->state = 0;
        }
    }
}

static inline void dump_key_buffer(bool emit) {
    clear_combos(emit);
    if (buffer_size == 0) {
        return;
    }

    if (emit) {
        for (uint8_t i = 0; i < buffer_size; i++) {
#ifndef NO_ACTION_TAPPING
            action_tapping_process(key_buffer[i]);
#else
            process_record(&key_buffer[i]);
#endif
        }
    }

    buffer_size = 0;
}

void fire_combo(void) {
    send_combo(prepared_combo->keycode, true, prepared_combo_index);
    prepared_combo->active = true;
    dump_key_buffer(false);
}

#define ALL_COMBO_KEYS_ARE_DOWN (((1 << count) - 1) == combo->state)
#define ONLY_ONE_KEY_IS_DOWN !(combo->state & (combo->state - 1))
#define KEY_NOT_YET_RELEASED ((1 << index) & combo->state)
#define KEY_STATE_DOWN(key)         \
    do {                            \
        combo->state |= (1 << key); \
    } while (0)
#define KEY_STATE_UP(key)            \
    do {                             \
        combo->state &= ~(1 << key); \
    } while (0)

static bool process_single_combo(combo_t *combo, uint16_t keycode, keyrecord_t *record, uint16_t combo_index) {
    uint8_t count = 0;
    uint16_t index = -1;
    /* Find index of keycode and number of combo keys */
    for (const uint16_t *keys = combo->keys;; ++count) {
        uint16_t key = pgm_read_word(&keys[count]);
        if (keycode == key) index = count;
        if (COMBO_END == key) break;
    }

    /* Continue processing if key isn't part of current combo.
     * Also disable current combo and reset its state. */
    if (-1 == (int8_t)index) {
        if (!combo->active) { /* do not clear a combo that has been fired */
            combo->disabled = true;
        }
        return false;
    }

    bool is_combo_active = is_active & !combo->disabled;

    if (record->event.pressed) {
        uint16_t time = COMBO_TERM;
#if defined(COMBO_TERM_PER_COMBO)
        time = get_combo_term(combo_index, combo);
#elif defined(COMBO_MUST_HOLD_PER_COMBO)
        if (get_combo_must_hold(combo_index, combo)) time = COMBO_MOD_TERM;
#elif defined(COMBO_MUST_HOLD_MODS)
        if (KEYCODE_IS_MOD(combo->keycode)) time = COMBO_MOD_TERM;
#endif
        if (!combo->active) {
            KEY_STATE_DOWN(index);
            if (longest_term < time) {
                longest_term = time;
            }
        }
        if (is_combo_active) {
            if (ALL_COMBO_KEYS_ARE_DOWN) { /* Combo was pressed */
                /* Save the combo so we can fire it after COMBO_TERM */

                /* Don't prepare this combo if its combo term has passed. */
                if (timer_elapsed(timer) < time) {
                    prepared_combo = combo;
                    prepared_combo_index = combo_index;
                }
            }
        }
    } else {
        if (!combo->active && ALL_COMBO_KEYS_ARE_DOWN) { /* First key quickly released */
            if (COMBO_PREPARED
#if defined(COMBO_MUST_HOLD_PER_COMBO)
                    && !get_combo_must_hold(prepared_combo_index, prepared_combo)
#elif defined(COMBO_MUST_HOLD_MODS)
                    && !KEYCODE_IS_MOD(prepared_combo->keycode)
#endif
                ) {
                /* Fire non-mod combo immediately if it was released inside COMBO_TERM */
                fire_combo();
            } else {
                /* combo wasn't tappable, dump key buffer */
                is_combo_active = false;
            }
        } else if (combo->active && ONLY_ONE_KEY_IS_DOWN && KEY_NOT_YET_RELEASED) { /* last key released */
            send_combo(combo->keycode, false, combo_index);
            combo->active = false;
            is_combo_active = true;
        } else if (combo->active && KEY_NOT_YET_RELEASED) { /* first or middle key released */
            is_combo_active = true;
        } else {
            /* continue processing without immediately returning */
            is_combo_active = false;
        }

        KEY_STATE_UP(index);
    }

    return is_combo_active;
}

#define NO_COMBO_KEYS_ARE_DOWN (0 == combo->state)

bool process_combo(uint16_t keycode, keyrecord_t *record) {
    bool is_combo_key          = false;
    bool no_combo_keys_pressed = true;

    if (keycode == CMB_ON && record->event.pressed) {
        combo_enable();
        return true;
    }

    if (keycode == CMB_OFF && record->event.pressed) {
        combo_disable();
        return true;
    }

    if (keycode == CMB_TOG && record->event.pressed) {
        combo_toggle();
        return true;
    }

    if (!is_combo_enabled()) {
        return true;
    }

#if defined(COMBO_PERMISSIVE_HOLD) && (defined(COMBO_MUST_HOLD_MODS) || defined(COMBO_MUST_HOLD_PER_COMBO))
    if (COMBO_PREPARED && timer_elapsed(timer) > COMBO_TERM &&
        record->event.pressed &&
#   if defined(COMBO_MUST_HOLD_PER_COMBO)
        get_combo_must_hold(prepared_combo_index, prepared_combo)
#   elif defined(COMBO_MUST_HOLD_MODS)
        KEYCODE_IS_MOD(prepared_combo->keycode)
#   endif
        ) {
        /* Allow combos resolving to modifier keys to be fired sooner when
         * another key is pressed. */
        fire_combo();
        return true;
    }
#endif

#ifndef COMBO_VARIABLE_LEN
    for (uint16_t idx = 0; idx < COMBO_COUNT; ++idx) {
#else
    for (uint16_t idx = 0; idx < COMBO_LEN; ++idx) {
#endif
        combo_t *combo = &key_combos[idx];
        is_combo_key |= process_single_combo(combo, keycode, record, idx);
        no_combo_keys_pressed = no_combo_keys_pressed && (NO_COMBO_KEYS_ARE_DOWN || combo->active || combo->disabled);
    }

    if (!is_combo_key) {
        /* if no combos claim the key we need to emit the keybuffer */
        dump_key_buffer(true);

    } else if (record->event.pressed && is_active) {
        /* otherwise the key is consumed and placed in the buffer */
        if (!timer) {
            /* only start the timer on first key */
            timer = timer_read();
        }

        if (buffer_size < MAX_COMBO_LENGTH) {
            key_buffer[buffer_size++] = *record;
        }
    }
    if (!record->event.pressed && no_combo_keys_pressed) {
        // reset state if there are no combo keys pressed at all
        dump_key_buffer(true);
        timer     = 0;
        is_active = true;
    }

    return !is_combo_key;
}

void matrix_scan_combo(void) {
    if (!b_combo_enable) { return; }

    uint16_t time = COMBO_TERM;

#if defined(COMBO_TERM_PER_COMBO) || defined(COMBO_MUST_HOLD_MODS) || defined(COMBO_MUST_HOLD_PER_COMBO)
    if (COMBO_PREPARED) {
#   if defined(COMBO_TERM_PER_COMBO)
        time = get_combo_term(prepared_combo_index, prepared_combo);
#   elif defined(COMBO_MUST_HOLD_PER_COMBO)
        if (get_combo_must_hold(prepared_combo_index, prepared_combo)) time = COMBO_MOD_TERM;
#   elif defined(COMBO_MUST_HOLD_MODS)
        if (KEYCODE_IS_MOD(prepared_combo->keycode)) time = COMBO_MOD_TERM;
#   endif
    }
#endif

    if (longest_term > time) {
        time = longest_term;
    }

    if (is_active && timer && timer_elapsed(timer) > time) {
        if (COMBO_PREPARED) {
            fire_combo();
        } else {
            dump_key_buffer(true);
            timer = 0;
            is_active = true;
        }
    }
}

void combo_enable(void) { is_active = b_combo_enable = true; }

void combo_disable(void) {
    b_combo_enable = is_active = false;
    timer                      = 0;
    prepared_combo             = NULL;
    dump_key_buffer(true);
}

void combo_toggle(void) {
    if (b_combo_enable) {
        combo_disable();
    } else {
        combo_enable();
    }
}

bool is_combo_enabled(void) { return b_combo_enable; }
