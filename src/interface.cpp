#include "interface.hpp"
#include "globals.hpp"

#define M3_ACCENT_COLOR     lv_color_hex(0x6750A4)
#define M3_SURFACE_COLOR    lv_color_hex(0x201A2B)
#define M3_BACKGROUND_COLOR lv_color_hex(0x1C1B1F)
#define M3_RADIUS           20

static lv_style_t style_base;
static lv_style_t style_m3_btn;


lv_obj_t * main_tabview = NULL;
lv_obj_t * temp_bar_dbl = NULL;
lv_obj_t * temp_bar_dbr = NULL;
lv_obj_t * temp_bar_int = NULL;
lv_obj_t * temp_bar_chn = NULL;
lv_obj_t * temp_label_dbl = NULL;
lv_obj_t * temp_label_dbr = NULL;
lv_obj_t * temp_label_arm = NULL;
lv_obj_t * temp_label_clw = NULL;

lv_obj_t * auton_status_label = NULL;
lv_obj_t * toggle_display_image = NULL;
lv_obj_t * config_dropdown = NULL;

int selected_auton = 0;
int control_mode = 0;


static void auton_btn_click_action(lv_event_t * e) {
    lv_obj_t * btn = (lv_obj_t *)lv_event_get_target(e);

    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        uint8_t id = (uint8_t)(intptr_t)lv_obj_get_user_data(btn);

        selected_auton = id;

        const char *auton_names[] = {"None", "RED Left", "RED Right", "BLU Left", "BLU Right", "Skills"};

        if (id >= 0 && id <= 5) {
            static char buffer[64];
            snprintf(buffer, sizeof(buffer), "Selected: %s", auton_names[id]);
            lv_label_set_text(auton_status_label, buffer);
        }

    }
}

void temp_update_task(void* param) {
    pros::Motor *motors[] = {
        &drivebase_left,
        &drivebase_right,
        &int_motor,
        &chn_motor
    };

    lv_obj_t *bars[] = {
        temp_bar_dbl,
        temp_bar_dbr,
        temp_bar_int,
        temp_bar_chn
    };

    const char *names[] = {
        "DBL", "DBR", "INT", "CHN"
    };

    static char buffer[32];

    while (true) {
        for (int i = 0; i < 4; i++) {
            double current_temp = motors[i]->get_temperature();
            // double current_temp = 42.0; <- Declare static value to debug task code.
            lv_obj_t *current_bar = bars[i];
            
            lv_bar_set_value(bars[i], current_temp, LV_ANIM_ON);

            if (current_temp > 999.0) {
                lv_obj_set_style_bg_color(current_bar, lv_palette_main(LV_PALETTE_ORANGE), LV_PART_INDICATOR);
            } else if (current_temp > 55.0) {
                lv_obj_set_style_bg_color(current_bar, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
            } else {
                lv_obj_set_style_bg_color(current_bar, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);
            }
        }

        pros::delay(200);
    }
}

// void temp_update_task(void* param) {
//     pros::Motor *motors[] = {
//         &drivebase_left,
//         &drivebase_right,
//         &arm_motor,
//         &claw_motor
//     };
//
//     lv_obj_t *bars[] = {
//         temp_bar_dbl,
//         temp_bar_dbr,
//         temp_bar_arm,
//         temp_bar_clw
//     };
//
//     const char *names[] = {
//         "DBL", "DBR", "ARM", "CLW"
//     };
//
//     static char buffer[32];
//
//     while (true) {
//         for (int i = 0; i < 4; i++) {
//             double current_temp = motors[i]->get_temperature();
//             // double current_temp = 42.0; <- Declare static value to debug task code.
//             lv_obj_t *current_bar = bars[i];
//            
//             lv_bar_set_value(bars[i], current_temp, LV_ANIM_ON);
//
//             if (current_temp > 999.0) {
//                 lv_obj_set_style_bg_color(current_bar, lv_palette_main(LV_PALETTE_ORANGE), LV_PART_INDICATOR);
//             } else if (current_temp > 55.0) {
//                 lv_obj_set_style_bg_color(current_bar, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
//             } else {
//                 lv_obj_set_style_bg_color(current_bar, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);
//             }
//         }
//
//         pros::delay(200);
//     }
// }

void create_temp_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = lv_obj_create(parent_tab);

    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_set_layout(cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);

    lv_obj_set_style_border_width(cont, 0, 0);
    lv_obj_set_style_pad_all(cont, 0, 0);
    lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_radius(cont, 0, 0);

    temp_bar_dbl = lv_bar_create(cont);
    lv_obj_set_pos(temp_bar_dbl, 10, 10);
    lv_obj_set_size(temp_bar_dbl, 440, 10);
    lv_bar_set_range(temp_bar_dbl, 0, 100);
    
    temp_bar_dbr = lv_bar_create(cont);
    lv_obj_set_pos(temp_bar_dbr, 10, 40);
    lv_obj_set_size(temp_bar_dbr, 440, 10);
    lv_bar_set_range(temp_bar_dbr, 0, 100);
    
    temp_bar_int = lv_bar_create(cont);
    lv_obj_set_pos(temp_bar_int, 10, 70);
    lv_obj_set_size(temp_bar_int, 440, 10);
    lv_bar_set_range(temp_bar_int, 0, 100);
    
    temp_bar_chn = lv_bar_create(cont);
    lv_obj_set_pos(temp_bar_chn, 10, 100);
    lv_obj_set_size(temp_bar_chn, 440, 10);
    lv_bar_set_range(temp_bar_chn, 0, 100);

    lv_obj_set_style_radius(temp_bar_dbl, 8, LV_PART_MAIN);
    lv_obj_set_style_radius(temp_bar_dbr, 8, LV_PART_MAIN);
    lv_obj_set_style_radius(temp_bar_int, 8, LV_PART_MAIN);
    lv_obj_set_style_radius(temp_bar_chn, 8, LV_PART_MAIN);
    lv_obj_set_style_bg_color(temp_bar_dbl, lv_color_hex(0x353139), LV_PART_MAIN);
    lv_obj_set_style_bg_color(temp_bar_dbr, lv_color_hex(0x353139), LV_PART_MAIN);
    lv_obj_set_style_bg_color(temp_bar_int, lv_color_hex(0x353139), LV_PART_MAIN);
    lv_obj_set_style_bg_color(temp_bar_chn, lv_color_hex(0x353139), LV_PART_MAIN);
    lv_obj_set_style_border_width(temp_bar_dbl, 1, LV_PART_MAIN);
    lv_obj_set_style_border_width(temp_bar_dbr, 1, LV_PART_MAIN);
    lv_obj_set_style_border_width(temp_bar_int, 1, LV_PART_MAIN);
    lv_obj_set_style_border_width(temp_bar_chn, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(temp_bar_dbl, lv_color_hex(0x49454F), LV_PART_MAIN);
    lv_obj_set_style_border_color(temp_bar_dbr, lv_color_hex(0x49454F), LV_PART_MAIN);
    lv_obj_set_style_border_color(temp_bar_int, lv_color_hex(0x49454F), LV_PART_MAIN);
    lv_obj_set_style_border_color(temp_bar_chn, lv_color_hex(0x49454F), LV_PART_MAIN);

    lv_obj_set_style_radius(temp_bar_dbl, 8, LV_PART_INDICATOR);
    lv_obj_set_style_radius(temp_bar_dbr, 8, LV_PART_INDICATOR);
    lv_obj_set_style_radius(temp_bar_int, 8, LV_PART_INDICATOR);
    lv_obj_set_style_radius(temp_bar_chn, 8, LV_PART_INDICATOR);

    temp_label_dbl = lv_label_create(cont); // Fixed because I'll probably use these objects later - Aiden
    lv_label_set_text(temp_label_dbl, "Drivebase left");
    lv_obj_set_pos(temp_label_dbl, 10, 25);

    temp_label_dbr = lv_label_create(cont);
    lv_label_set_text(temp_label_dbr, "Drivebase right");
    lv_obj_set_pos(temp_label_dbr, 10, 55);

    temp_label_arm = lv_label_create(cont);
    lv_label_set_text(temp_label_arm, "Arm");
    lv_obj_set_pos(temp_label_arm, 10, 85);

    temp_label_clw = lv_label_create(cont);
    lv_label_set_text(temp_label_clw, "Claw");
    lv_obj_set_pos(temp_label_clw, 10, 115);

    pros::Task temp_task(temp_update_task, (void*)"TEMP_TASK");
}

void create_auton_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = lv_obj_create(parent_tab);

    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_set_layout(cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_style_pad_column(cont, 15, 0);
    lv_obj_set_style_pad_row(cont, 15, 0); 
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);

    lv_obj_set_style_border_width(cont, 0, 0);
    lv_obj_set_style_pad_all(cont, 0, 0);
    lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_radius(cont, 0, 0);

    auton_status_label = lv_label_create(cont);
    lv_label_set_text(auton_status_label, "Selected: None");
    lv_obj_set_pos(auton_status_label, 10, 10);
    lv_obj_set_width(auton_status_label, LV_PCT(90));

    lv_obj_t * none_btn = lv_btn_create(cont);
    lv_obj_set_pos(none_btn, 10, 50);
    lv_obj_set_size(none_btn, LV_PCT(45), 40);
    lv_obj_set_user_data(none_btn, (void*)0);
    lv_obj_add_event_cb(none_btn, auton_btn_click_action, LV_EVENT_CLICKED, NULL);
    lv_obj_add_style(none_btn, &style_m3_btn, 0);
    lv_obj_set_style_bg_color(none_btn, M3_ACCENT_COLOR, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(none_btn, 0, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);

    lv_obj_t * none_label = lv_label_create(none_btn);
    lv_label_set_text(none_label, "None");

    lv_obj_t * skills_btn = lv_btn_create(cont);
    lv_obj_set_pos(skills_btn, 230, 50);
    lv_obj_set_size(skills_btn, LV_PCT(45), 40);
    lv_obj_set_user_data(skills_btn, (void*)5);
    lv_obj_add_event_cb(skills_btn, auton_btn_click_action, LV_EVENT_CLICKED, NULL);
    lv_obj_add_style(skills_btn, &style_m3_btn, 0);
    lv_obj_set_style_bg_color(skills_btn, M3_ACCENT_COLOR, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(skills_btn, 0, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);

    lv_obj_t * skills_label = lv_label_create(skills_btn);
    lv_label_set_text(skills_label, "Skills");

    lv_obj_t * red_left_btn = lv_btn_create(cont);
    lv_obj_set_pos(red_left_btn, 10, 110);
    lv_obj_set_size(red_left_btn, LV_PCT(45), 40);
    lv_obj_set_user_data(red_left_btn, (void*)1);
    lv_obj_add_event_cb(red_left_btn, auton_btn_click_action, LV_EVENT_CLICKED, NULL);
    lv_obj_add_style(red_left_btn, &style_m3_btn, 0);
    lv_obj_set_style_bg_color(red_left_btn, M3_ACCENT_COLOR, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(red_left_btn, 0, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);

    lv_obj_t * red_left_label = lv_label_create(red_left_btn);
    lv_label_set_text(red_left_label, "RED Left");

    lv_obj_t * red_right_btn = lv_btn_create(cont);
    lv_obj_set_pos(red_right_btn, 230, 110);
    lv_obj_set_size(red_right_btn, LV_PCT(45), 40);
    lv_obj_set_user_data(red_right_btn, (void*)2);
    lv_obj_add_event_cb(red_right_btn, auton_btn_click_action, LV_EVENT_CLICKED, NULL);
    lv_obj_add_style(red_right_btn, &style_m3_btn, 0);
    lv_obj_set_style_bg_color(red_right_btn, M3_ACCENT_COLOR, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(red_right_btn, 0, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);
    
    lv_obj_t * red_right_label = lv_label_create(red_right_btn);
    lv_label_set_text(red_right_label, "RED Right");

    lv_obj_t * blu_left_btn = lv_btn_create(cont);
    lv_obj_set_pos(blu_left_btn, 10, 170);
    lv_obj_set_size(blu_left_btn, LV_PCT(45), 40);
    lv_obj_set_user_data(blu_left_btn, (void*)3);
    lv_obj_add_event_cb(blu_left_btn, auton_btn_click_action, LV_EVENT_CLICKED, NULL);
    lv_obj_add_style(blu_left_btn, &style_m3_btn, 0);
    lv_obj_set_style_bg_color(blu_left_btn, M3_ACCENT_COLOR, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(blu_left_btn, 0, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);

    lv_obj_t * blu_left_label = lv_label_create(blu_left_btn);
    lv_label_set_text(blu_left_label, "BLU Left");

    lv_obj_t * blu_right_btn = lv_btn_create(cont);
    lv_obj_set_pos(blu_right_btn, 230, 170);
    lv_obj_set_size(blu_right_btn, LV_PCT(45), 40);
    lv_obj_set_user_data(blu_right_btn, (void*)4);
    lv_obj_add_event_cb(blu_right_btn, auton_btn_click_action, LV_EVENT_CLICKED, NULL);
    lv_obj_add_style(blu_right_btn, &style_m3_btn, 0);
    lv_obj_set_style_bg_color(blu_right_btn, M3_ACCENT_COLOR, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(blu_right_btn, 0, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);

    lv_obj_t * blu_right_label = lv_label_create(blu_right_btn);
    lv_label_set_text(blu_right_label, "BLU Right");
}

static void image_button_action(lv_event_t * e) {
    lv_obj_t * btn = (lv_obj_t *)lv_event_get_target(e); 
    
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) { 
        
        if (lv_obj_has_flag(toggle_display_image, LV_OBJ_FLAG_HIDDEN)) {
            lv_obj_clear_flag(toggle_display_image, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_bg_color(btn, LV_COLOR_MAKE(0, 200, 0), LV_PART_MAIN | LV_STATE_DEFAULT);
        } else {
            lv_obj_add_flag(toggle_display_image, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_bg_color(btn, LV_COLOR_MAKE(255, 0, 0), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
}

void create_image_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = lv_obj_create(parent_tab);

    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_set_layout(cont, LV_LAYOUT_FLEX);

    lv_obj_set_style_border_width(cont, 0, 0);
    lv_obj_set_style_pad_all(cont, 0, 0);
    lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_radius(cont, 0, 0);

    lv_obj_t * image_btn = lv_btn_create(cont);
    lv_obj_set_pos(image_btn, 10, 10);
    lv_obj_set_size(image_btn, 100, 40);
    lv_obj_add_event_cb(image_btn, image_button_action, LV_EVENT_CLICKED, NULL);
    lv_obj_add_style(image_btn, &style_m3_btn, 0);
    lv_obj_set_style_bg_color(image_btn, M3_ACCENT_COLOR, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(image_btn, 0, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);

    lv_obj_t * image_label = lv_label_create(image_btn);
    lv_label_set_text(image_label, "Minnow");

    lv_obj_set_style_bg_color(image_btn, LV_COLOR_MAKE(255, 0, 0), LV_PART_MAIN | LV_STATE_DEFAULT);

    extern const lv_img_dsc_t creature; 

    toggle_display_image = lv_img_create(cont);
    lv_img_set_src(toggle_display_image, &creature);
    lv_obj_move_foreground(toggle_display_image);

    lv_obj_align(toggle_display_image, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(toggle_display_image, LV_OBJ_FLAG_HIDDEN);
}

static void drive_mode_dropdown_action(lv_event_t * e) {
    lv_obj_t * dropdown = (lv_obj_t *)lv_event_get_target(e);
    
    // Check if the event was a value change
    if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) {
        int selected_index = lv_dropdown_get_selected(dropdown);
        control_mode = selected_index;
    }
}

void create_settings_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = lv_obj_create(parent_tab);

    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_set_layout(cont, LV_LAYOUT_FLEX);

    lv_obj_set_style_border_width(cont, 0, 0);
    lv_obj_set_style_pad_all(cont, 0, 0);
    lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_radius(cont, 0, 0);

    lv_obj_t * config_dropdown = lv_dropdown_create(cont);
    lv_obj_set_pos(config_dropdown, 50, 10);
    lv_dropdown_set_options(config_dropdown, "Arcade\nTank");
    lv_obj_add_event_cb(config_dropdown, drive_mode_dropdown_action, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_set_style_radius(config_dropdown, 15, LV_PART_MAIN);
    lv_obj_set_style_bg_color(config_dropdown, lv_color_hex(0x49454F), LV_PART_MAIN);    
    lv_obj_set_style_bg_color(config_dropdown, M3_ACCENT_COLOR, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_color(config_dropdown, lv_color_white(), LV_PART_MAIN);

    lv_obj_t * list = lv_dropdown_get_list(config_dropdown);

    if (list != NULL) {
        lv_obj_set_style_bg_color(list, lv_color_hex(0x49454F), LV_PART_MAIN);
        lv_obj_set_style_radius(list, 15, LV_PART_MAIN);
        lv_obj_set_style_bg_color(list, M3_ACCENT_COLOR, LV_PART_SELECTED);
        lv_obj_set_style_text_color(list, lv_color_white(), LV_PART_SELECTED);
    }
}

void initialize_interface() {
    lv_style_init(&style_base);
    lv_style_set_bg_color(&style_base, M3_BACKGROUND_COLOR);
    lv_style_set_text_color(&style_base, lv_color_white());

    lv_style_init(&style_m3_btn);
    lv_style_set_radius(&style_m3_btn, M3_RADIUS);
    lv_style_set_shadow_width(&style_m3_btn, 8);
    lv_style_set_shadow_opa(&style_m3_btn, LV_OPA_50);
    lv_style_set_bg_color(&style_m3_btn, lv_color_hex(0x49454F));
    lv_style_set_text_color(&style_m3_btn, lv_color_white());

    lv_obj_t * screen = lv_scr_act();
    lv_obj_add_style(screen, &style_base, 0);

    main_tabview = lv_tabview_create(screen);
    lv_obj_t * tab_bar = lv_tabview_get_tab_bar(main_tabview);
    lv_obj_set_height(tab_bar, 40);

    lv_obj_set_style_bg_color(tab_bar, M3_SURFACE_COLOR, LV_PART_ITEMS);
    lv_obj_set_style_text_color(tab_bar, lv_color_hex(0xE6E0E9), LV_PART_ITEMS);
    lv_obj_set_style_border_width(tab_bar, 0, LV_PART_ITEMS);
    lv_obj_set_style_pad_all(tab_bar, 5, LV_PART_ITEMS);

    lv_obj_set_style_radius(tab_bar, M3_RADIUS, LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(tab_bar, M3_ACCENT_COLOR, LV_PART_INDICATOR);
    lv_obj_set_style_text_color(tab_bar, lv_color_hex(0xFFFBFE), LV_STATE_CHECKED | LV_PART_ITEMS);

    lv_obj_set_style_width(tab_bar, LV_PCT(100), LV_STATE_CHECKED | LV_PART_INDICATOR);
    lv_obj_set_style_height(tab_bar, LV_PCT(100), LV_STATE_CHECKED | LV_PART_INDICATOR);

    lv_obj_set_style_bg_color(main_tabview, M3_SURFACE_COLOR, LV_PART_MAIN);
    lv_obj_set_style_pad_all(main_tabview, 0, LV_PART_MAIN);
    
    lv_obj_t * temp_tab = lv_tabview_add_tab(main_tabview, "Monitor");
    create_temp_tab(temp_tab);
    
    lv_obj_t * auton_tab = lv_tabview_add_tab(main_tabview, "Auton");
    create_auton_tab(auton_tab);
    
    lv_obj_t * image_tab = lv_tabview_add_tab(main_tabview, "Images");
    create_image_tab(image_tab);

    lv_obj_t * settings_tab = lv_tabview_add_tab(main_tabview, "Settings");
    create_settings_tab(settings_tab);

    lv_tabview_set_act(main_tabview, 1, LV_ANIM_ON); 
}