/*
Copyright 2019 Russell Troxel <russelltroxel@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include QMK_KEYBOARD_H

#define MODS_SHIFT  (keyboard_report->mods & MOD_BIT(KC_LSHIFT) || keyboard_report->mods & MOD_BIT(KC_RSHIFT))
#define MODS_CTRL  (keyboard_report->mods & MOD_BIT(KC_LCTL) || keyboard_report->mods & MOD_BIT(KC_RCTRL))
#define MODS_ALT  (keyboard_report->mods & MOD_BIT(KC_LALT) || keyboard_report->mods & MOD_BIT(KC_RALT))

enum whitefox_keycodes {
    MD_BOOT = SAFE_RANGE,            //Restart into bootloader after hold timeout
    C_SWAP_GA,
};

typedef union {
  uint32_t raw;
  struct {
    bool alt_gui_swapped :1;
  };
} user_config_t;

user_config_t user_config;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /* Base layer
   * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
   * │Esc│ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │ - │ = │ ` │ \ │PSc│
   * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴───┼───┤
   * │ Tab │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │ [ │ ] │Bspc │Del│
   * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┼───┤
   * │  Fn  │ A │ S │ D │ F │ G │ H │ J │ K │ L │ ; │ ' │ Enter  │PgU│
   * ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────┬───┼───┤
   * │ LShift │ Z │ X │ C │ V │ B │ N │ M │ , │ . │ / │CtlSft│ ↑ │PgD│
   * ├────┬───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬─┬───┼───┼───┤
   * │LCtl│LGui│LAlt│         Space          │AlGu│Caps│ │ ← │ ↓ │ → │
   * └────┴────┴────┴────────────────────────┴────┴────┘ └───┴───┴───┘
   */
  [0] = LAYOUT_truefox(
    KC_ESC, KC_1,   KC_2,   KC_3,   KC_4,   KC_5,   KC_6,   KC_7,   KC_8,   KC_9,   KC_0,   KC_MINS,KC_EQL, KC_GRV, KC_BSLS, KC_PSCR, \
    KC_TAB, KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,   KC_LBRC,KC_RBRC,KC_BSPC,        KC_DEL,  \
    MO(1),  KC_A,   KC_S,   KC_D,   KC_F,   KC_G,   KC_H,   KC_J,   KC_K,   KC_L,   KC_SCLN,KC_QUOT,        KC_ENT,         KC_PGUP, \
    KC_LSFT,        KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,   KC_N,   KC_M,   KC_COMM,KC_DOT, KC_SLSH,KC_RSFT,        KC_UP,  KC_PGDN, \
    KC_LCTL,KC_LALT,KC_LGUI,                        KC_SPC,                 KC_RALT,KC_CAPS,                KC_LEFT,KC_DOWN,KC_RGHT  \
  ),
  [1] = LAYOUT_truefox( \
    KC_GRV, KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,  KC_F7,  KC_F8,  KC_F9,  KC_F10, KC_F11, KC_F12, _______, KC_SLCK,KC_PAUS, \
    _______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,          KC_INS,  \
    _______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,        _______,         KC_VOLU,     \
    _______,        _______,_______,_______,_______,MD_BOOT,_______,_______,_______,_______,_______,_______,         _______, KC_VOLD,  \
    _______,C_SWAP_GA,_______,                        _______,                _______,_______,                 KC_MPRV,KC_MPLY,KC_MNXT   \
  ),
};

const uint16_t PROGMEM fn_actions[] = {};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
  // Call the keymap level matrix init.

  // Read the user config from EEPROM
  user_config.raw = eeconfig_read_user();
};

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint32_t key_timer;

    switch (keycode) {
        case MD_BOOT:
            if (record->event.pressed) {
                key_timer = timer_read32();
            } else {
                if (timer_elapsed32(key_timer) >= 500) {
                    reset_keyboard();
                }
            }
            return false;
        case KC_ESC:
            if (record->event.pressed && MODS_SHIFT) {
                SEND_STRING("~");
                return false;
            }
            // If shift isn't pressed, send KC_ESC events as normal.
            return true;
        case KC_BSPC:
            if (record->event.pressed && MODS_SHIFT) {
                SEND_STRING("|");
                return false;
            }
            return true;
        case C_SWAP_GA:
          if (record->event.pressed) {
            user_config.alt_gui_swapped ^= 1; // Toggles the status
            eeconfig_update_user(user_config.raw); // Writes the new status to EEPROM
          }
          return false;
        case KC_LGUI:
        case KC_LALT: ;
          uint16_t opposite = (keycode == KC_LGUI) ? KC_LALT : KC_LGUI;
          if (user_config.alt_gui_swapped) {
            if (record->event.pressed) {
              register_code(opposite);
            } else {
              unregister_code(opposite);
            }
            return false;
          }
          return true;
        default:
            return true; //Process all other keycodes normally
    }
}
