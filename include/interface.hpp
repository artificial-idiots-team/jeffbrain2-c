#include "main.h"

#ifndef _INTERFACE_HPP_
#define _INTERFACE_HPP_

extern lv_obj_t * main_tabview;
extern lv_obj_t * temp_bar_dbl;
extern lv_obj_t * temp_bar_dbr;
extern lv_obj_t * temp_label_dbl;
extern lv_obj_t * temp_label_dbr;

extern lv_obj_t * temp_bar_int;
extern lv_obj_t * temp_bar_chn;
extern lv_obj_t * temp_label_int;
extern lv_obj_t * temp_label_chn;

// Old motors from Clawbot Jeff 2.
// extern lv_obj_t * temp_bar_arm;
// extern lv_obj_t * temp_bar_clw;
// extern lv_obj_t * temp_label_arm;
// extern lv_obj_t * temp_label_clw;

extern lv_obj_t * config_dropdown;
extern int control_mode;

extern const lv_img_dsc_t creature;

void initialize_interface();
void temp_update_task(void* param);
void legacy_temp_update_task(void* param);

void create_auton_tab(lv_obj_t * parent_tab);
void create_temp_tab(lv_obj_t * parent_tab);

#endif // _INTERFACE_HPP_