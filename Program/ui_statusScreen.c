// ui_statusScreen.c


#include "ui.h"

void ui_statusScreen_screen_init(void)
{
ui_statusScreen = lv_obj_create(NULL);
lv_obj_clear_flag( ui_statusScreen, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_statusScreen, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_statusScreen, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_statusScreen, &ui_img_1234_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_img_opa(ui_statusScreen, 200, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_returnButton3 = lv_btn_create(ui_statusScreen);
lv_obj_set_width( ui_returnButton3, 60);
lv_obj_set_height( ui_returnButton3, 30);
lv_obj_set_x( ui_returnButton3, -125 );
lv_obj_set_y( ui_returnButton3, -100 );
lv_obj_set_align( ui_returnButton3, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_returnButton3, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_returnButton3, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_returnButton3, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_returnButton3, lv_color_hex(0x0D0246), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_returnButton3, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_returnButton3, &ui_img_return_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_width(ui_returnButton3, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_returnButton3, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_x(ui_returnButton3, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_y(ui_returnButton3, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_statusscreenlabel = lv_label_create(ui_statusScreen);
lv_obj_set_width( ui_statusscreenlabel, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_statusscreenlabel, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_statusscreenlabel, 0 );
lv_obj_set_y( ui_statusscreenlabel, -100 );
lv_obj_set_align( ui_statusscreenlabel, LV_ALIGN_CENTER );
lv_label_set_text(ui_statusscreenlabel,"STATUS");
lv_obj_set_style_text_color(ui_statusscreenlabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_statusscreenlabel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_statusscreenlabel, &lv_font_montserrat_18, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_TEMP_Label = lv_label_create(ui_statusScreen);
lv_obj_set_width( ui_TEMP_Label, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_TEMP_Label, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_TEMP_Label, -110 );
lv_obj_set_y( ui_TEMP_Label, 10 );
lv_obj_set_align( ui_TEMP_Label, LV_ALIGN_CENTER );
lv_label_set_text(ui_TEMP_Label,"Temp");
lv_obj_set_style_text_color(ui_TEMP_Label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_TEMP_Label, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_TEMP_Label, &lv_font_montserrat_14, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_ALARM_Label = lv_label_create(ui_statusScreen);
lv_obj_set_width( ui_ALARM_Label, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_ALARM_Label, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_ALARM_Label, 0 );
lv_obj_set_y( ui_ALARM_Label, 10 );
lv_obj_set_align( ui_ALARM_Label, LV_ALIGN_CENTER );
lv_label_set_text(ui_ALARM_Label,"Alarm");
lv_obj_set_style_text_color(ui_ALARM_Label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_ALARM_Label, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_ALARM_Label, &lv_font_montserrat_14, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Status_Label = lv_label_create(ui_statusScreen);
lv_obj_set_width( ui_Status_Label, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Status_Label, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Status_Label, 110 );
lv_obj_set_y( ui_Status_Label, 10 );
lv_obj_set_align( ui_Status_Label, LV_ALIGN_CENTER );
lv_label_set_text(ui_Status_Label,"Status");
lv_obj_set_style_text_color(ui_Status_Label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_Status_Label, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_Status_Label, &lv_font_montserrat_14, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_App_Connection_Label = lv_label_create(ui_statusScreen);
lv_obj_set_width( ui_App_Connection_Label, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_App_Connection_Label, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_App_Connection_Label, -110 );
lv_obj_set_y( ui_App_Connection_Label, 105 );
lv_obj_set_align( ui_App_Connection_Label, LV_ALIGN_CENTER );
lv_label_set_text(ui_App_Connection_Label,"APP");
lv_obj_set_style_text_color(ui_App_Connection_Label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_App_Connection_Label, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_App_Connection_Label, &lv_font_montserrat_14, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_info_Label = lv_label_create(ui_statusScreen);
lv_obj_set_width( ui_info_Label, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_info_Label, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_info_Label, 110 );
lv_obj_set_y( ui_info_Label, 105 );
lv_obj_set_align( ui_info_Label, LV_ALIGN_CENTER );
lv_label_set_text(ui_info_Label,"Info");
lv_obj_set_style_text_color(ui_info_Label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_info_Label, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_info_Label, &lv_font_montserrat_14, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Maintenance_Label = lv_label_create(ui_statusScreen);
lv_obj_set_width( ui_Maintenance_Label, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Maintenance_Label, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Maintenance_Label, 0 );
lv_obj_set_y( ui_Maintenance_Label, 105 );
lv_obj_set_align( ui_Maintenance_Label, LV_ALIGN_CENTER );
lv_label_set_text(ui_Maintenance_Label,"Maintenance");
lv_obj_set_style_text_color(ui_Maintenance_Label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_Maintenance_Label, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_Maintenance_Label, &lv_font_montserrat_14, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_tempStatusButton = lv_btn_create(ui_statusScreen);
lv_obj_set_width( ui_tempStatusButton, 65);
lv_obj_set_height( ui_tempStatusButton, 65);
lv_obj_set_x( ui_tempStatusButton, -110 );
lv_obj_set_y( ui_tempStatusButton, -35 );
lv_obj_set_align( ui_tempStatusButton, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_tempStatusButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_tempStatusButton, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_tempStatusButton, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_tempStatusButton, lv_color_hex(0x00BE10), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_tempStatusButton, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_tempStatusButton, &ui_img_temperature_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_color(ui_tempStatusButton, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_tempStatusButton, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_tempStatusButton, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_width(ui_tempStatusButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_tempStatusButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_x(ui_tempStatusButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_y(ui_tempStatusButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_tempStatusPanel = lv_obj_create(ui_statusScreen);
lv_obj_set_width( ui_tempStatusPanel, 325);
lv_obj_set_height( ui_tempStatusPanel, 245);
lv_obj_set_align( ui_tempStatusPanel, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_tempStatusPanel, LV_OBJ_FLAG_HIDDEN );   /// Flags
lv_obj_clear_flag( ui_tempStatusPanel, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_tempStatusPanel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_tempStatusPanel, lv_color_hex(0x080052), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_tempStatusPanel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_tempStatusPanel, &ui_img_blur_color_background_85551_240x320_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_img_opa(ui_tempStatusPanel, 64, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_returnButton11 = lv_btn_create(ui_tempStatusPanel);
lv_obj_set_width( ui_returnButton11, 60);
lv_obj_set_height( ui_returnButton11, 30);
lv_obj_set_x( ui_returnButton11, -125 );
lv_obj_set_y( ui_returnButton11, -100 );
lv_obj_set_align( ui_returnButton11, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_returnButton11, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_returnButton11, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_returnButton11, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_returnButton11, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_returnButton11, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_returnButton11, &ui_img_return_66_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_width(ui_returnButton11, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_returnButton11, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_x(ui_returnButton11, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_y(ui_returnButton11, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_tempStatusLabel = lv_label_create(ui_tempStatusPanel);
lv_obj_set_width( ui_tempStatusLabel, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_tempStatusLabel, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_tempStatusLabel, 0 );
lv_obj_set_y( ui_tempStatusLabel, -100 );
lv_obj_set_align( ui_tempStatusLabel, LV_ALIGN_CENTER );
lv_label_set_text(ui_tempStatusLabel,"TEMPERATURES");
lv_obj_set_style_text_color(ui_tempStatusLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_tempStatusLabel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_tempStatusLabel, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_inlet_temp_danger = lv_img_create(ui_tempStatusPanel);
lv_img_set_src(ui_inlet_temp_danger, &ui_img_1101012385);
lv_obj_set_width( ui_inlet_temp_danger, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_inlet_temp_danger, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_inlet_temp_danger, 140 );
lv_obj_set_y( ui_inlet_temp_danger, -55 );
lv_obj_set_align( ui_inlet_temp_danger, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_inlet_temp_danger, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_inlet_temp_danger, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_outlet_temp_danger = lv_img_create(ui_tempStatusPanel);
lv_img_set_src(ui_outlet_temp_danger, &ui_img_1101012385);
lv_obj_set_width( ui_outlet_temp_danger, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_outlet_temp_danger, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_outlet_temp_danger, 140 );
lv_obj_set_y( ui_outlet_temp_danger, -10 );
lv_obj_set_align( ui_outlet_temp_danger, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_outlet_temp_danger, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_outlet_temp_danger, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_antifreeze_temp_danger = lv_img_create(ui_tempStatusPanel);
lv_img_set_src(ui_antifreeze_temp_danger, &ui_img_1101012385);
lv_obj_set_width( ui_antifreeze_temp_danger, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_antifreeze_temp_danger, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_antifreeze_temp_danger, 140 );
lv_obj_set_y( ui_antifreeze_temp_danger, 35 );
lv_obj_set_align( ui_antifreeze_temp_danger, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_antifreeze_temp_danger, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_antifreeze_temp_danger, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_filter_temp_danger = lv_img_create(ui_tempStatusPanel);
lv_img_set_src(ui_filter_temp_danger, &ui_img_1101012385);
lv_obj_set_width( ui_filter_temp_danger, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_filter_temp_danger, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_filter_temp_danger, 140 );
lv_obj_set_y( ui_filter_temp_danger, 80 );
lv_obj_set_align( ui_filter_temp_danger, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_filter_temp_danger, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_filter_temp_danger, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_ContainerA1 = lv_obj_create(ui_tempStatusPanel);
lv_obj_remove_style_all(ui_ContainerA1);
lv_obj_set_width( ui_ContainerA1, 300);
lv_obj_set_height( ui_ContainerA1, 150);
lv_obj_set_x( ui_ContainerA1, 0 );
lv_obj_set_y( ui_ContainerA1, 15 );
lv_obj_set_align( ui_ContainerA1, LV_ALIGN_CENTER );
lv_obj_set_flex_flow(ui_ContainerA1,LV_FLEX_FLOW_COLUMN_WRAP);
lv_obj_set_flex_align(ui_ContainerA1, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
lv_obj_clear_flag( ui_ContainerA1, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_Inlet_Temp_status = lv_label_create(ui_ContainerA1);
lv_obj_set_width( ui_Inlet_Temp_status, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Inlet_Temp_status, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Inlet_Temp_status, -105 );
lv_obj_set_y( ui_Inlet_Temp_status, -50 );
lv_obj_set_align( ui_Inlet_Temp_status, LV_ALIGN_CENTER );
lv_label_set_text(ui_Inlet_Temp_status,"INLET: ");
lv_obj_set_style_text_color(ui_Inlet_Temp_status, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_Inlet_Temp_status, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_Inlet_Temp_status, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Outlet_Temp_status = lv_label_create(ui_ContainerA1);
lv_obj_set_width( ui_Outlet_Temp_status, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Outlet_Temp_status, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Outlet_Temp_status, -105 );
lv_obj_set_y( ui_Outlet_Temp_status, -20 );
lv_obj_set_align( ui_Outlet_Temp_status, LV_ALIGN_CENTER );
lv_label_set_text(ui_Outlet_Temp_status,"OUTLET: ");
lv_obj_set_style_text_color(ui_Outlet_Temp_status, lv_color_hex(0x0071FF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_Outlet_Temp_status, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_Outlet_Temp_status, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_antifreeze_temp_status = lv_label_create(ui_ContainerA1);
lv_obj_set_width( ui_antifreeze_temp_status, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_antifreeze_temp_status, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_antifreeze_temp_status, -105 );
lv_obj_set_y( ui_antifreeze_temp_status, 10 );
lv_obj_set_align( ui_antifreeze_temp_status, LV_ALIGN_CENTER );
lv_label_set_text(ui_antifreeze_temp_status,"ANTIFREEZE: ");
lv_obj_set_style_text_color(ui_antifreeze_temp_status, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_antifreeze_temp_status, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_antifreeze_temp_status, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_fan_2_filter_alarm_status = lv_label_create(ui_ContainerA1);
lv_obj_set_width( ui_fan_2_filter_alarm_status, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_fan_2_filter_alarm_status, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_fan_2_filter_alarm_status, -105 );
lv_obj_set_y( ui_fan_2_filter_alarm_status, 40 );
lv_obj_set_align( ui_fan_2_filter_alarm_status, LV_ALIGN_CENTER );
lv_label_set_text(ui_fan_2_filter_alarm_status,"FAN-2: ");
lv_obj_set_style_text_color(ui_fan_2_filter_alarm_status, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_fan_2_filter_alarm_status, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_fan_2_filter_alarm_status, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_alarmStatusButton = lv_btn_create(ui_statusScreen);
lv_obj_set_width( ui_alarmStatusButton, 65);
lv_obj_set_height( ui_alarmStatusButton, 65);
lv_obj_set_x( ui_alarmStatusButton, 0 );
lv_obj_set_y( ui_alarmStatusButton, -35 );
lv_obj_set_align( ui_alarmStatusButton, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_alarmStatusButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_alarmStatusButton, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_alarmStatusButton, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_alarmStatusButton, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_alarmStatusButton, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_alarmStatusButton, &ui_img_sensor_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_color(ui_alarmStatusButton, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_alarmStatusButton, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_alarmStatusButton, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_width(ui_alarmStatusButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_alarmStatusButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_x(ui_alarmStatusButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_y(ui_alarmStatusButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_alarmStatusPanel = lv_obj_create(ui_statusScreen);
lv_obj_set_width( ui_alarmStatusPanel, 325);
lv_obj_set_height( ui_alarmStatusPanel, 245);
lv_obj_set_align( ui_alarmStatusPanel, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_alarmStatusPanel, LV_OBJ_FLAG_HIDDEN );   /// Flags
lv_obj_clear_flag( ui_alarmStatusPanel, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_alarmStatusPanel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_alarmStatusPanel, lv_color_hex(0x080052), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_alarmStatusPanel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_alarmStatusPanel, &ui_img_blur_color_background_85551_240x320_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_img_opa(ui_alarmStatusPanel, 64, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_returnButton2 = lv_btn_create(ui_alarmStatusPanel);
lv_obj_set_width( ui_returnButton2, 60);
lv_obj_set_height( ui_returnButton2, 30);
lv_obj_set_x( ui_returnButton2, -125 );
lv_obj_set_y( ui_returnButton2, -100 );
lv_obj_set_align( ui_returnButton2, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_returnButton2, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_returnButton2, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_returnButton2, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_returnButton2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_returnButton2, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_returnButton2, &ui_img_return_66_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_width(ui_returnButton2, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_returnButton2, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_x(ui_returnButton2, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_y(ui_returnButton2, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_alarmStatusLabel = lv_label_create(ui_alarmStatusPanel);
lv_obj_set_width( ui_alarmStatusLabel, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_alarmStatusLabel, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_alarmStatusLabel, 0 );
lv_obj_set_y( ui_alarmStatusLabel, -100 );
lv_obj_set_align( ui_alarmStatusLabel, LV_ALIGN_CENTER );
lv_label_set_text(ui_alarmStatusLabel,"ALARM-REPORT");
lv_obj_set_style_text_color(ui_alarmStatusLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_alarmStatusLabel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_alarmStatusLabel, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_high_temp_danger = lv_img_create(ui_alarmStatusPanel);
lv_img_set_src(ui_high_temp_danger, &ui_img_1101012385);
lv_obj_set_width( ui_high_temp_danger, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_high_temp_danger, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_high_temp_danger, 140 );
lv_obj_set_y( ui_high_temp_danger, -57 );
lv_obj_set_align( ui_high_temp_danger, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_high_temp_danger, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_high_temp_danger, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_door_danger = lv_img_create(ui_alarmStatusPanel);
lv_img_set_src(ui_door_danger, &ui_img_1101012385);
lv_obj_set_width( ui_door_danger, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_door_danger, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_door_danger, 140 );
lv_obj_set_y( ui_door_danger, -12 );
lv_obj_set_align( ui_door_danger, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_door_danger, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_door_danger, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_filter_danger = lv_img_create(ui_alarmStatusPanel);
lv_img_set_src(ui_filter_danger, &ui_img_1101012385);
lv_obj_set_width( ui_filter_danger, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_filter_danger, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_filter_danger, 140 );
lv_obj_set_y( ui_filter_danger, 33 );
lv_obj_set_align( ui_filter_danger, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_filter_danger, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_filter_danger, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_high_pressure_danger = lv_img_create(ui_alarmStatusPanel);
lv_img_set_src(ui_high_pressure_danger, &ui_img_1101012385);
lv_obj_set_width( ui_high_pressure_danger, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_high_pressure_danger, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_high_pressure_danger, 140 );
lv_obj_set_y( ui_high_pressure_danger, 78 );
lv_obj_set_align( ui_high_pressure_danger, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_high_pressure_danger, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_high_pressure_danger, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_ContainerA2 = lv_obj_create(ui_alarmStatusPanel);
lv_obj_remove_style_all(ui_ContainerA2);
lv_obj_set_width( ui_ContainerA2, 300);
lv_obj_set_height( ui_ContainerA2, 150);
lv_obj_set_x( ui_ContainerA2, 0 );
lv_obj_set_y( ui_ContainerA2, 15 );
lv_obj_set_align( ui_ContainerA2, LV_ALIGN_CENTER );
lv_obj_set_flex_flow(ui_ContainerA2,LV_FLEX_FLOW_COLUMN_WRAP);
lv_obj_set_flex_align(ui_ContainerA2, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
lv_obj_clear_flag( ui_ContainerA2, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_high_Temp_Alarm = lv_label_create(ui_ContainerA2);
lv_obj_set_width( ui_high_Temp_Alarm, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_high_Temp_Alarm, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_high_Temp_Alarm, -112 );
lv_obj_set_y( ui_high_Temp_Alarm, 12 );
lv_obj_set_align( ui_high_Temp_Alarm, LV_ALIGN_CENTER );
lv_label_set_text(ui_high_Temp_Alarm,"TEMP:");
lv_obj_set_style_text_color(ui_high_Temp_Alarm, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_high_Temp_Alarm, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_high_Temp_Alarm, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Door_Alarm = lv_label_create(ui_ContainerA2);
lv_obj_set_width( ui_Door_Alarm, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Door_Alarm, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Door_Alarm, -120 );
lv_obj_set_y( ui_Door_Alarm, -15 );
lv_obj_set_align( ui_Door_Alarm, LV_ALIGN_CENTER );
lv_label_set_text(ui_Door_Alarm,"DOOR:");
lv_obj_set_style_text_color(ui_Door_Alarm, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_Door_Alarm, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_Door_Alarm, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Filter_Alarm = lv_label_create(ui_ContainerA2);
lv_obj_set_width( ui_Filter_Alarm, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Filter_Alarm, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Filter_Alarm, -112 );
lv_obj_set_y( ui_Filter_Alarm, 12 );
lv_obj_set_align( ui_Filter_Alarm, LV_ALIGN_CENTER );
lv_label_set_text(ui_Filter_Alarm,"FILTER:");
lv_obj_set_style_text_color(ui_Filter_Alarm, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_Filter_Alarm, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_Filter_Alarm, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_high_Pressure_Alarm = lv_label_create(ui_ContainerA2);
lv_obj_set_width( ui_high_Pressure_Alarm, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_high_Pressure_Alarm, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_high_Pressure_Alarm, -112 );
lv_obj_set_y( ui_high_Pressure_Alarm, 12 );
lv_obj_set_align( ui_high_Pressure_Alarm, LV_ALIGN_CENTER );
lv_label_set_text(ui_high_Pressure_Alarm,"PRESSURE:");
lv_obj_set_style_text_color(ui_high_Pressure_Alarm, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_high_Pressure_Alarm, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_high_Pressure_Alarm, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_relayStatusButton = lv_btn_create(ui_statusScreen);
lv_obj_set_width( ui_relayStatusButton, 65);
lv_obj_set_height( ui_relayStatusButton, 65);
lv_obj_set_x( ui_relayStatusButton, 110 );
lv_obj_set_y( ui_relayStatusButton, -35 );
lv_obj_set_align( ui_relayStatusButton, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_relayStatusButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_relayStatusButton, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_relayStatusButton, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_relayStatusButton, lv_color_hex(0x0071FF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_relayStatusButton, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_relayStatusButton, &ui_img_120570296, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_color(ui_relayStatusButton, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_relayStatusButton, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_relayStatusButton, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_width(ui_relayStatusButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_relayStatusButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_x(ui_relayStatusButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_y(ui_relayStatusButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_relayStatusPanel = lv_obj_create(ui_statusScreen);
lv_obj_set_width( ui_relayStatusPanel, 325);
lv_obj_set_height( ui_relayStatusPanel, 245);
lv_obj_set_align( ui_relayStatusPanel, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_relayStatusPanel, LV_OBJ_FLAG_HIDDEN );   /// Flags
lv_obj_clear_flag( ui_relayStatusPanel, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_relayStatusPanel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_relayStatusPanel, lv_color_hex(0x080052), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_relayStatusPanel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_relayStatusPanel, &ui_img_blur_color_background_85551_240x320_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_img_opa(ui_relayStatusPanel, 64, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_returnButton7 = lv_btn_create(ui_relayStatusPanel);
lv_obj_set_width( ui_returnButton7, 60);
lv_obj_set_height( ui_returnButton7, 30);
lv_obj_set_x( ui_returnButton7, -125 );
lv_obj_set_y( ui_returnButton7, -100 );
lv_obj_set_align( ui_returnButton7, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_returnButton7, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_returnButton7, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_returnButton7, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_returnButton7, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_returnButton7, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_returnButton7, &ui_img_return_66_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_width(ui_returnButton7, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_returnButton7, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_x(ui_returnButton7, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_y(ui_returnButton7, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_relayStatusLabel = lv_label_create(ui_relayStatusPanel);
lv_obj_set_width( ui_relayStatusLabel, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_relayStatusLabel, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_relayStatusLabel, 0 );
lv_obj_set_y( ui_relayStatusLabel, -100 );
lv_obj_set_align( ui_relayStatusLabel, LV_ALIGN_CENTER );
lv_label_set_text(ui_relayStatusLabel,"STATUS");
lv_obj_set_style_text_color(ui_relayStatusLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_relayStatusLabel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_relayStatusLabel, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_ContainerA3 = lv_obj_create(ui_relayStatusPanel);
lv_obj_remove_style_all(ui_ContainerA3);
lv_obj_set_width( ui_ContainerA3, 300);
lv_obj_set_height( ui_ContainerA3, 150);
lv_obj_set_x( ui_ContainerA3, 0 );
lv_obj_set_y( ui_ContainerA3, 15 );
lv_obj_set_align( ui_ContainerA3, LV_ALIGN_CENTER );
lv_obj_set_flex_flow(ui_ContainerA3,LV_FLEX_FLOW_COLUMN_WRAP);
lv_obj_set_flex_align(ui_ContainerA3, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
lv_obj_clear_flag( ui_ContainerA3, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_evaprartor_Fan_Status = lv_label_create(ui_ContainerA3);
lv_obj_set_width( ui_evaprartor_Fan_Status, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_evaprartor_Fan_Status, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_evaprartor_Fan_Status, 50 );
lv_obj_set_y( ui_evaprartor_Fan_Status, 12 );
lv_obj_set_align( ui_evaprartor_Fan_Status, LV_ALIGN_CENTER );
lv_label_set_text(ui_evaprartor_Fan_Status,"EVAPRATOR:");
lv_obj_set_style_text_color(ui_evaprartor_Fan_Status, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_evaprartor_Fan_Status, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_evaprartor_Fan_Status, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Compressor_Status = lv_label_create(ui_ContainerA3);
lv_obj_set_width( ui_Compressor_Status, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Compressor_Status, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Compressor_Status, 60 );
lv_obj_set_y( ui_Compressor_Status, -15 );
lv_obj_set_align( ui_Compressor_Status, LV_ALIGN_CENTER );
lv_label_set_text(ui_Compressor_Status,"COMPRESSOR:");
lv_obj_set_style_text_color(ui_Compressor_Status, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_Compressor_Status, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_Compressor_Status, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_condensor_Fan_1_Status = lv_label_create(ui_ContainerA3);
lv_obj_set_width( ui_condensor_Fan_1_Status, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_condensor_Fan_1_Status, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_condensor_Fan_1_Status, 50 );
lv_obj_set_y( ui_condensor_Fan_1_Status, 12 );
lv_obj_set_align( ui_condensor_Fan_1_Status, LV_ALIGN_CENTER );
lv_label_set_text(ui_condensor_Fan_1_Status,"CONDENSER:");
lv_obj_set_style_text_color(ui_condensor_Fan_1_Status, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_condensor_Fan_1_Status, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_condensor_Fan_1_Status, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_condensor_Fan_2_Status = lv_label_create(ui_ContainerA3);
lv_obj_set_width( ui_condensor_Fan_2_Status, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_condensor_Fan_2_Status, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_condensor_Fan_2_Status, 50 );
lv_obj_set_y( ui_condensor_Fan_2_Status, 12 );
lv_obj_set_align( ui_condensor_Fan_2_Status, LV_ALIGN_CENTER );
lv_label_set_text(ui_condensor_Fan_2_Status,"FAN-2:");
lv_obj_set_style_text_color(ui_condensor_Fan_2_Status, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_condensor_Fan_2_Status, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_condensor_Fan_2_Status, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_networkStatusButton = lv_btn_create(ui_statusScreen);
lv_obj_set_width( ui_networkStatusButton, 65);
lv_obj_set_height( ui_networkStatusButton, 65);
lv_obj_set_x( ui_networkStatusButton, -110 );
lv_obj_set_y( ui_networkStatusButton, 60 );
lv_obj_set_align( ui_networkStatusButton, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_networkStatusButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_networkStatusButton, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_networkStatusButton, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_networkStatusButton, lv_color_hex(0xFF8800), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_networkStatusButton, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_networkStatusButton, &ui_img_wifi_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_color(ui_networkStatusButton, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_networkStatusButton, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_networkStatusButton, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_width(ui_networkStatusButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_networkStatusButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_x(ui_networkStatusButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_y(ui_networkStatusButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_networkStatusPanel = lv_obj_create(ui_statusScreen);
lv_obj_set_width( ui_networkStatusPanel, 325);
lv_obj_set_height( ui_networkStatusPanel, 245);
lv_obj_set_align( ui_networkStatusPanel, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_networkStatusPanel, LV_OBJ_FLAG_HIDDEN );   /// Flags
lv_obj_clear_flag( ui_networkStatusPanel, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_networkStatusPanel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_networkStatusPanel, lv_color_hex(0x080052), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_networkStatusPanel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_networkStatusPanel, &ui_img_blur_color_background_85551_240x320_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_img_opa(ui_networkStatusPanel, 64, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_returnButton8 = lv_btn_create(ui_networkStatusPanel);
lv_obj_set_width( ui_returnButton8, 60);
lv_obj_set_height( ui_returnButton8, 40);
lv_obj_set_x( ui_returnButton8, -125 );
lv_obj_set_y( ui_returnButton8, -95 );
lv_obj_set_align( ui_returnButton8, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_returnButton8, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_returnButton8, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_returnButton8, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_returnButton8, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_returnButton8, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_returnButton8, &ui_img_return_66_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_width(ui_returnButton8, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_returnButton8, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_x(ui_returnButton8, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_y(ui_returnButton8, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Connect_to_Device_Label = lv_label_create(ui_networkStatusPanel);
lv_obj_set_width( ui_Connect_to_Device_Label, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Connect_to_Device_Label, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Connect_to_Device_Label, 0 );
lv_obj_set_y( ui_Connect_to_Device_Label, -100 );
lv_obj_set_align( ui_Connect_to_Device_Label, LV_ALIGN_CENTER );
lv_label_set_text(ui_Connect_to_Device_Label,"Connect to Device");
lv_obj_set_style_text_color(ui_Connect_to_Device_Label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_Connect_to_Device_Label, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_Connect_to_Device_Label, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_ContainerA4 = lv_obj_create(ui_networkStatusPanel);
lv_obj_remove_style_all(ui_ContainerA4);
lv_obj_set_width( ui_ContainerA4, 300);
lv_obj_set_height( ui_ContainerA4, 170);
lv_obj_set_x( ui_ContainerA4, 0 );
lv_obj_set_y( ui_ContainerA4, 15 );
lv_obj_set_align( ui_ContainerA4, LV_ALIGN_CENTER );
lv_obj_set_flex_flow(ui_ContainerA4,LV_FLEX_FLOW_COLUMN_WRAP);
lv_obj_set_flex_align(ui_ContainerA4, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
lv_obj_clear_flag( ui_ContainerA4, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_text_color(ui_ContainerA4, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_ContainerA4, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_ContainerA4, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_access_Point_wifi = lv_label_create(ui_ContainerA4);
lv_obj_set_width( ui_access_Point_wifi, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_access_Point_wifi, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_access_Point_wifi, -110 );
lv_obj_set_y( ui_access_Point_wifi, 12 );
lv_obj_set_align( ui_access_Point_wifi, LV_ALIGN_CENTER );
lv_label_set_text(ui_access_Point_wifi,"AP-WI-FI:");
lv_obj_set_style_text_color(ui_access_Point_wifi, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_access_Point_wifi, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_access_Point_wifi, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_access_Point_Password = lv_label_create(ui_ContainerA4);
lv_obj_set_width( ui_access_Point_Password, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_access_Point_Password, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_access_Point_Password, -110 );
lv_obj_set_y( ui_access_Point_Password, 12 );
lv_obj_set_align( ui_access_Point_Password, LV_ALIGN_CENTER );
lv_label_set_text(ui_access_Point_Password,"AP-Password:");
lv_obj_set_style_text_color(ui_access_Point_Password, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_access_Point_Password, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_access_Point_Password, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_guidetext = lv_label_create(ui_ContainerA4);
lv_obj_set_width( ui_guidetext, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_guidetext, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_guidetext, -110 );
lv_obj_set_y( ui_guidetext, 12 );
lv_obj_set_align( ui_guidetext, LV_ALIGN_CENTER );
lv_label_set_text(ui_guidetext,"For Locally Connection & Control Device:\n1) First Download Device Application.\n2) Scan QR-Code in Application.");
lv_obj_set_style_text_color(ui_guidetext, lv_color_hex(0xCECECE), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_guidetext, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_letter_space(ui_guidetext, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_line_space(ui_guidetext, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_guidetext, &lv_font_montserrat_12, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_QRcode_Tab = lv_tabview_create(ui_networkStatusPanel, LV_DIR_BOTTOM, 35);
lv_obj_set_width( ui_QRcode_Tab, 310);
lv_obj_set_height( ui_QRcode_Tab, 180);
lv_obj_set_x( ui_QRcode_Tab, 0 );
lv_obj_set_y( ui_QRcode_Tab, 25 );
lv_obj_set_align( ui_QRcode_Tab, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_QRcode_Tab, LV_OBJ_FLAG_HIDDEN );   /// Flags
lv_obj_clear_flag( ui_QRcode_Tab, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_QRcode_Tab, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_QRcode_Tab, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_QRcode_Tab, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_QRcode_Tab, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_outline_width(ui_QRcode_Tab, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_outline_pad(ui_QRcode_Tab, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_radius(ui_QRcode_Tab, 5, LV_PART_MAIN| LV_STATE_CHECKED);
lv_obj_set_style_bg_color(ui_QRcode_Tab, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_CHECKED );
lv_obj_set_style_bg_opa(ui_QRcode_Tab, 255, LV_PART_MAIN| LV_STATE_CHECKED);
lv_obj_set_style_border_width(ui_QRcode_Tab, 0, LV_PART_MAIN| LV_STATE_CHECKED);
lv_obj_set_style_outline_width(ui_QRcode_Tab, 0, LV_PART_MAIN| LV_STATE_CHECKED);
lv_obj_set_style_outline_pad(ui_QRcode_Tab, 0, LV_PART_MAIN| LV_STATE_CHECKED);

lv_obj_set_style_text_color(lv_tabview_get_tab_btns(ui_QRcode_Tab), lv_color_hex(0xFFFFFF),  LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(lv_tabview_get_tab_btns(ui_QRcode_Tab), 255,  LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(lv_tabview_get_tab_btns(ui_QRcode_Tab), lv_color_hex(0xFFFFFF),  LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(lv_tabview_get_tab_btns(ui_QRcode_Tab), 255,  LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_outline_width(lv_tabview_get_tab_btns(ui_QRcode_Tab), 0,  LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_outline_pad(lv_tabview_get_tab_btns(ui_QRcode_Tab), 0,  LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_color(lv_tabview_get_tab_btns(ui_QRcode_Tab), lv_color_hex(0xFFFFFF),  LV_PART_MAIN | LV_STATE_CHECKED );
lv_obj_set_style_text_opa(lv_tabview_get_tab_btns(ui_QRcode_Tab), 255,  LV_PART_MAIN| LV_STATE_CHECKED);
lv_obj_set_style_bg_color(lv_tabview_get_tab_btns(ui_QRcode_Tab), lv_color_hex(0xFFFFFF),  LV_PART_MAIN | LV_STATE_CHECKED );
lv_obj_set_style_bg_opa(lv_tabview_get_tab_btns(ui_QRcode_Tab), 255,  LV_PART_MAIN| LV_STATE_CHECKED);
lv_obj_set_style_border_width(lv_tabview_get_tab_btns(ui_QRcode_Tab), 0,  LV_PART_MAIN| LV_STATE_CHECKED);
lv_obj_set_style_outline_width(lv_tabview_get_tab_btns(ui_QRcode_Tab), 0,  LV_PART_MAIN| LV_STATE_CHECKED);
lv_obj_set_style_outline_pad(lv_tabview_get_tab_btns(ui_QRcode_Tab), 0,  LV_PART_MAIN| LV_STATE_CHECKED);

lv_obj_set_style_text_color(lv_tabview_get_tab_btns(ui_QRcode_Tab), lv_color_hex(0xFFFFFF),  LV_PART_ITEMS | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(lv_tabview_get_tab_btns(ui_QRcode_Tab), 255,  LV_PART_ITEMS| LV_STATE_DEFAULT);
lv_obj_set_style_radius(lv_tabview_get_tab_btns(ui_QRcode_Tab), 5,  LV_PART_ITEMS| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(lv_tabview_get_tab_btns(ui_QRcode_Tab), lv_color_hex(0x0071FF),  LV_PART_ITEMS | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(lv_tabview_get_tab_btns(ui_QRcode_Tab), 255,  LV_PART_ITEMS| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(lv_tabview_get_tab_btns(ui_QRcode_Tab), lv_color_hex(0xFFFFFF),  LV_PART_ITEMS | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(lv_tabview_get_tab_btns(ui_QRcode_Tab), 255,  LV_PART_ITEMS| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(lv_tabview_get_tab_btns(ui_QRcode_Tab), 2,  LV_PART_ITEMS| LV_STATE_DEFAULT);
lv_obj_set_style_outline_color(lv_tabview_get_tab_btns(ui_QRcode_Tab), lv_color_hex(0xFFFFFF),  LV_PART_ITEMS | LV_STATE_DEFAULT );
lv_obj_set_style_outline_opa(lv_tabview_get_tab_btns(ui_QRcode_Tab), 255,  LV_PART_ITEMS| LV_STATE_DEFAULT);
lv_obj_set_style_outline_width(lv_tabview_get_tab_btns(ui_QRcode_Tab), 1,  LV_PART_ITEMS| LV_STATE_DEFAULT);
lv_obj_set_style_outline_pad(lv_tabview_get_tab_btns(ui_QRcode_Tab), 1,  LV_PART_ITEMS| LV_STATE_DEFAULT);
lv_obj_set_style_text_color(lv_tabview_get_tab_btns(ui_QRcode_Tab), lv_color_hex(0xFFFFFF),  LV_PART_ITEMS | LV_STATE_CHECKED );
lv_obj_set_style_text_opa(lv_tabview_get_tab_btns(ui_QRcode_Tab), 255,  LV_PART_ITEMS| LV_STATE_CHECKED);
lv_obj_set_style_radius(lv_tabview_get_tab_btns(ui_QRcode_Tab), 5,  LV_PART_ITEMS| LV_STATE_CHECKED);
lv_obj_set_style_bg_color(lv_tabview_get_tab_btns(ui_QRcode_Tab), lv_color_hex(0x00BE10),  LV_PART_ITEMS | LV_STATE_CHECKED );
lv_obj_set_style_bg_opa(lv_tabview_get_tab_btns(ui_QRcode_Tab), 255,  LV_PART_ITEMS| LV_STATE_CHECKED);
lv_obj_set_style_border_color(lv_tabview_get_tab_btns(ui_QRcode_Tab), lv_color_hex(0xFFFFFF),  LV_PART_ITEMS | LV_STATE_CHECKED );
lv_obj_set_style_border_opa(lv_tabview_get_tab_btns(ui_QRcode_Tab), 255,  LV_PART_ITEMS| LV_STATE_CHECKED);
lv_obj_set_style_border_width(lv_tabview_get_tab_btns(ui_QRcode_Tab), 2,  LV_PART_ITEMS| LV_STATE_CHECKED);
lv_obj_set_style_border_side(lv_tabview_get_tab_btns(ui_QRcode_Tab), LV_BORDER_SIDE_FULL,  LV_PART_ITEMS| LV_STATE_CHECKED);
lv_obj_set_style_outline_color(lv_tabview_get_tab_btns(ui_QRcode_Tab), lv_color_hex(0xFFFFFF),  LV_PART_ITEMS | LV_STATE_CHECKED );
lv_obj_set_style_outline_opa(lv_tabview_get_tab_btns(ui_QRcode_Tab), 255,  LV_PART_ITEMS| LV_STATE_CHECKED);
lv_obj_set_style_outline_width(lv_tabview_get_tab_btns(ui_QRcode_Tab), 1,  LV_PART_ITEMS| LV_STATE_CHECKED);
lv_obj_set_style_outline_pad(lv_tabview_get_tab_btns(ui_QRcode_Tab), 1,  LV_PART_ITEMS| LV_STATE_CHECKED);

ui_appldownload = lv_tabview_add_tab(ui_QRcode_Tab, "Download App");
lv_obj_set_flex_flow(ui_appldownload,LV_FLEX_FLOW_ROW);
lv_obj_set_flex_align(ui_appldownload, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
lv_obj_clear_flag( ui_appldownload, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN );    /// Flags
lv_obj_set_style_bg_color(ui_appldownload, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_appldownload, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Application_QR_Code = lv_obj_create(ui_appldownload);
lv_obj_set_width( ui_Application_QR_Code, 135);
lv_obj_set_height( ui_Application_QR_Code, 135);
lv_obj_set_align( ui_Application_QR_Code, LV_ALIGN_CENTER );
lv_obj_clear_flag( ui_Application_QR_Code, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_Application_QR_Code, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_Application_QR_Code, lv_color_hex(0xFF8900), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_Application_QR_Code, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_Application_QR_Code, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_color(ui_Application_QR_Code, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_opa(ui_Application_QR_Code, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_width(ui_Application_QR_Code, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_Application_QR_Code, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_x(ui_Application_QR_Code, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_y(ui_Application_QR_Code, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

lv_obj_set_style_border_width(ui_Application_QR_Code, 0, LV_PART_SCROLLBAR| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_color(ui_Application_QR_Code, lv_color_hex(0x000000), LV_PART_SCROLLBAR | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_opa(ui_Application_QR_Code, 0, LV_PART_SCROLLBAR| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_width(ui_Application_QR_Code, 0, LV_PART_SCROLLBAR| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_Application_QR_Code, 0, LV_PART_SCROLLBAR| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_x(ui_Application_QR_Code, 0, LV_PART_SCROLLBAR| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_y(ui_Application_QR_Code, 0, LV_PART_SCROLLBAR| LV_STATE_DEFAULT);

ui_download_App_Label = lv_label_create(ui_appldownload);
lv_obj_set_width( ui_download_App_Label, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_download_App_Label, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_download_App_Label, 0 );
lv_obj_set_y( ui_download_App_Label, -70 );
lv_obj_set_align( ui_download_App_Label, LV_ALIGN_CENTER );
lv_label_set_text(ui_download_App_Label,"Scan QR-Code to \ndownload the app.");
lv_obj_set_style_text_color(ui_download_App_Label, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_download_App_Label, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_download_App_Label, &lv_font_montserrat_14, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_connectToapp = lv_tabview_add_tab(ui_QRcode_Tab, "Connect App");
lv_obj_set_flex_flow(ui_connectToapp,LV_FLEX_FLOW_ROW);
lv_obj_set_flex_align(ui_connectToapp, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
lv_obj_clear_flag( ui_connectToapp, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN );    /// Flags
lv_obj_set_style_bg_color(ui_connectToapp, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_connectToapp, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Connection_QR_Code = lv_obj_create(ui_connectToapp);
lv_obj_set_width( ui_Connection_QR_Code, 135);
lv_obj_set_height( ui_Connection_QR_Code, 135);
lv_obj_set_align( ui_Connection_QR_Code, LV_ALIGN_CENTER );
lv_obj_clear_flag( ui_Connection_QR_Code, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_Connection_QR_Code, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_Connection_QR_Code, lv_color_hex(0x080052), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_Connection_QR_Code, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_Connection_QR_Code, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_color(ui_Connection_QR_Code, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_opa(ui_Connection_QR_Code, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_width(ui_Connection_QR_Code, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_Connection_QR_Code, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_x(ui_Connection_QR_Code, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_y(ui_Connection_QR_Code, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

lv_obj_set_style_border_width(ui_Connection_QR_Code, 0, LV_PART_SCROLLBAR| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_color(ui_Connection_QR_Code, lv_color_hex(0x000000), LV_PART_SCROLLBAR | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_opa(ui_Connection_QR_Code, 0, LV_PART_SCROLLBAR| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_width(ui_Connection_QR_Code, 0, LV_PART_SCROLLBAR| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_Connection_QR_Code, 0, LV_PART_SCROLLBAR| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_x(ui_Connection_QR_Code, 0, LV_PART_SCROLLBAR| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_y(ui_Connection_QR_Code, 0, LV_PART_SCROLLBAR| LV_STATE_DEFAULT);

ui_Connect_App_Label = lv_label_create(ui_connectToapp);
lv_obj_set_width( ui_Connect_App_Label, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Connect_App_Label, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Connect_App_Label, 0 );
lv_obj_set_y( ui_Connect_App_Label, -70 );
lv_obj_set_align( ui_Connect_App_Label, LV_ALIGN_CENTER );
lv_label_set_text(ui_Connect_App_Label,"Scan QR-Code to \nConnect the app.");
lv_obj_set_style_text_color(ui_Connect_App_Label, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_Connect_App_Label, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_Connect_App_Label, &lv_font_montserrat_14, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_show_QR_Code_Button = lv_btn_create(ui_networkStatusPanel);
lv_obj_set_width( ui_show_QR_Code_Button, 60);
lv_obj_set_height( ui_show_QR_Code_Button, 40);
lv_obj_set_x( ui_show_QR_Code_Button, 125 );
lv_obj_set_y( ui_show_QR_Code_Button, -96 );
lv_obj_set_align( ui_show_QR_Code_Button, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_show_QR_Code_Button, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_show_QR_Code_Button, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_show_QR_Code_Button, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_show_QR_Code_Button, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_show_QR_Code_Button, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_show_QR_Code_Button, &ui_img_983251955, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_width(ui_show_QR_Code_Button, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_show_QR_Code_Button, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_x(ui_show_QR_Code_Button, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_y(ui_show_QR_Code_Button, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Hide_QR_Code_Button = lv_btn_create(ui_networkStatusPanel);
lv_obj_set_width( ui_Hide_QR_Code_Button, 60);
lv_obj_set_height( ui_Hide_QR_Code_Button, 40);
lv_obj_set_x( ui_Hide_QR_Code_Button, 125 );
lv_obj_set_y( ui_Hide_QR_Code_Button, -96 );
lv_obj_set_align( ui_Hide_QR_Code_Button, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_Hide_QR_Code_Button, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_Hide_QR_Code_Button, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_Hide_QR_Code_Button, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_Hide_QR_Code_Button, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_Hide_QR_Code_Button, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_Hide_QR_Code_Button, &ui_img_close_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_width(ui_Hide_QR_Code_Button, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_Hide_QR_Code_Button, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_x(ui_Hide_QR_Code_Button, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_y(ui_Hide_QR_Code_Button, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_deviceInfoButton = lv_btn_create(ui_statusScreen);
lv_obj_set_width( ui_deviceInfoButton, 65);
lv_obj_set_height( ui_deviceInfoButton, 65);
lv_obj_set_x( ui_deviceInfoButton, 110 );
lv_obj_set_y( ui_deviceInfoButton, 60 );
lv_obj_set_align( ui_deviceInfoButton, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_deviceInfoButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_deviceInfoButton, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_deviceInfoButton, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_deviceInfoButton, lv_color_hex(0xB0B0B0), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_deviceInfoButton, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_deviceInfoButton, &ui_img_1437619218, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_color(ui_deviceInfoButton, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_deviceInfoButton, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_deviceInfoButton, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_width(ui_deviceInfoButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_deviceInfoButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_x(ui_deviceInfoButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_y(ui_deviceInfoButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_deviceInfoPanel = lv_obj_create(ui_statusScreen);
lv_obj_set_width( ui_deviceInfoPanel, 325);
lv_obj_set_height( ui_deviceInfoPanel, 245);
lv_obj_set_align( ui_deviceInfoPanel, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_deviceInfoPanel, LV_OBJ_FLAG_HIDDEN );   /// Flags
lv_obj_clear_flag( ui_deviceInfoPanel, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_deviceInfoPanel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_deviceInfoPanel, lv_color_hex(0x080052), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_deviceInfoPanel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_deviceInfoPanel, &ui_img_blur_color_background_85551_240x320_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_img_opa(ui_deviceInfoPanel, 64, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_returnButton9 = lv_btn_create(ui_deviceInfoPanel);
lv_obj_set_width( ui_returnButton9, 60);
lv_obj_set_height( ui_returnButton9, 30);
lv_obj_set_x( ui_returnButton9, -125 );
lv_obj_set_y( ui_returnButton9, -100 );
lv_obj_set_align( ui_returnButton9, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_returnButton9, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_returnButton9, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_returnButton9, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_returnButton9, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_returnButton9, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_returnButton9, &ui_img_return_66_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_width(ui_returnButton9, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_returnButton9, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_x(ui_returnButton9, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_y(ui_returnButton9, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_deviceInfoLabel = lv_label_create(ui_deviceInfoPanel);
lv_obj_set_width( ui_deviceInfoLabel, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_deviceInfoLabel, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_deviceInfoLabel, 0 );
lv_obj_set_y( ui_deviceInfoLabel, -100 );
lv_obj_set_align( ui_deviceInfoLabel, LV_ALIGN_CENTER );
lv_label_set_text(ui_deviceInfoLabel,"INFO");
lv_obj_set_style_text_color(ui_deviceInfoLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_deviceInfoLabel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_deviceInfoLabel, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_ContainerA5 = lv_obj_create(ui_deviceInfoPanel);
lv_obj_remove_style_all(ui_ContainerA5);
lv_obj_set_width( ui_ContainerA5, 300);
lv_obj_set_height( ui_ContainerA5, 150);
lv_obj_set_x( ui_ContainerA5, 0 );
lv_obj_set_y( ui_ContainerA5, 15 );
lv_obj_set_align( ui_ContainerA5, LV_ALIGN_CENTER );
lv_obj_set_flex_flow(ui_ContainerA5,LV_FLEX_FLOW_COLUMN_WRAP);
lv_obj_set_flex_align(ui_ContainerA5, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
lv_obj_clear_flag( ui_ContainerA5, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_serial_number = lv_label_create(ui_ContainerA5);
lv_obj_set_width( ui_serial_number, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_serial_number, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_serial_number, 45 );
lv_obj_set_y( ui_serial_number, 12 );
lv_obj_set_align( ui_serial_number, LV_ALIGN_CENTER );
lv_label_set_text(ui_serial_number,"SERIAL:");
lv_obj_set_style_text_color(ui_serial_number, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_serial_number, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_serial_number, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_customerID = lv_label_create(ui_ContainerA5);
lv_obj_set_width( ui_customerID, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_customerID, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_customerID, 45 );
lv_obj_set_y( ui_customerID, 12 );
lv_obj_set_align( ui_customerID, LV_ALIGN_CENTER );
lv_label_set_text(ui_customerID,"CUSTOMER-ID:");
lv_obj_set_style_text_color(ui_customerID, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_customerID, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_customerID, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_activationDate = lv_label_create(ui_ContainerA5);
lv_obj_set_width( ui_activationDate, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_activationDate, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_activationDate, 45 );
lv_obj_set_y( ui_activationDate, 12 );
lv_obj_set_align( ui_activationDate, LV_ALIGN_CENTER );
lv_label_set_text(ui_activationDate,"ACTIVATION:");
lv_obj_set_style_text_color(ui_activationDate, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_activationDate, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_activationDate, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_serviceInfoButton = lv_btn_create(ui_statusScreen);
lv_obj_set_width( ui_serviceInfoButton, 65);
lv_obj_set_height( ui_serviceInfoButton, 65);
lv_obj_set_x( ui_serviceInfoButton, 0 );
lv_obj_set_y( ui_serviceInfoButton, 60 );
lv_obj_set_align( ui_serviceInfoButton, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_serviceInfoButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_serviceInfoButton, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_serviceInfoButton, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_serviceInfoButton, lv_color_hex(0xFFD900), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_serviceInfoButton, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_serviceInfoButton, &ui_img_18821776, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_color(ui_serviceInfoButton, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_serviceInfoButton, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_serviceInfoButton, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_width(ui_serviceInfoButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_serviceInfoButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_x(ui_serviceInfoButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_y(ui_serviceInfoButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_serviceInfoPanel = lv_obj_create(ui_statusScreen);
lv_obj_set_width( ui_serviceInfoPanel, 325);
lv_obj_set_height( ui_serviceInfoPanel, 245);
lv_obj_set_align( ui_serviceInfoPanel, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_serviceInfoPanel, LV_OBJ_FLAG_HIDDEN );   /// Flags
lv_obj_clear_flag( ui_serviceInfoPanel, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_serviceInfoPanel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_serviceInfoPanel, lv_color_hex(0x080052), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_serviceInfoPanel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_serviceInfoPanel, &ui_img_blur_color_background_85551_240x320_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_img_opa(ui_serviceInfoPanel, 64, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_returnButton10 = lv_btn_create(ui_serviceInfoPanel);
lv_obj_set_width( ui_returnButton10, 60);
lv_obj_set_height( ui_returnButton10, 30);
lv_obj_set_x( ui_returnButton10, -125 );
lv_obj_set_y( ui_returnButton10, -100 );
lv_obj_set_align( ui_returnButton10, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_returnButton10, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_returnButton10, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_returnButton10, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_returnButton10, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_returnButton10, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_img_src( ui_returnButton10, &ui_img_return_66_png, LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_width(ui_returnButton10, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_returnButton10, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_x(ui_returnButton10, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_y(ui_returnButton10, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_serviceInfoLabel = lv_label_create(ui_serviceInfoPanel);
lv_obj_set_width( ui_serviceInfoLabel, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_serviceInfoLabel, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_serviceInfoLabel, 0 );
lv_obj_set_y( ui_serviceInfoLabel, -100 );
lv_obj_set_align( ui_serviceInfoLabel, LV_ALIGN_CENTER );
lv_label_set_text(ui_serviceInfoLabel,"MAINTENANCE");
lv_obj_set_style_text_color(ui_serviceInfoLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_serviceInfoLabel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_serviceInfoLabel, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_ContainerA6 = lv_obj_create(ui_serviceInfoPanel);
lv_obj_remove_style_all(ui_ContainerA6);
lv_obj_set_width( ui_ContainerA6, 300);
lv_obj_set_height( ui_ContainerA6, 150);
lv_obj_set_x( ui_ContainerA6, 0 );
lv_obj_set_y( ui_ContainerA6, 15 );
lv_obj_set_align( ui_ContainerA6, LV_ALIGN_CENTER );
lv_obj_set_flex_flow(ui_ContainerA6,LV_FLEX_FLOW_COLUMN_WRAP);
lv_obj_set_flex_align(ui_ContainerA6, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
lv_obj_clear_flag( ui_ContainerA6, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_servicesintervals = lv_label_create(ui_ContainerA6);
lv_obj_set_width( ui_servicesintervals, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_servicesintervals, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_servicesintervals, 20 );
lv_obj_set_y( ui_servicesintervals, 23 );
lv_obj_set_align( ui_servicesintervals, LV_ALIGN_CENTER );
lv_label_set_text(ui_servicesintervals,"SERVICES INTERVALS:");
lv_obj_set_style_text_color(ui_servicesintervals, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_servicesintervals, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_servicesintervals, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

lv_obj_add_event_cb(ui_returnButton3, ui_event_returnButton3, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_tempStatusButton, ui_event_tempStatusButton, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_returnButton11, ui_event_returnButton11, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_alarmStatusButton, ui_event_alarmStatusButton, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_returnButton2, ui_event_returnButton2, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_relayStatusButton, ui_event_relayStatusButton, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_returnButton7, ui_event_returnButton7, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_networkStatusButton, ui_event_networkStatusButton, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_returnButton8, ui_event_returnButton8, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_show_QR_Code_Button, ui_event_show_QR_Code_Button, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_Hide_QR_Code_Button, ui_event_Hide_QR_Code_Button, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_deviceInfoButton, ui_event_deviceInfoButton, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_returnButton9, ui_event_returnButton9, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_serviceInfoButton, ui_event_serviceInfoButton, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_returnButton10, ui_event_returnButton10, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_statusScreen, ui_event_statusScreen, LV_EVENT_ALL, NULL);
uic_statusScreen = ui_statusScreen;
uic_returnButton3 = ui_returnButton3;
uic_statusscreenlabel = ui_statusscreenlabel;
uic_TEMP_Label = ui_TEMP_Label;
uic_ALARM_Label = ui_ALARM_Label;
uic_Status_Label = ui_Status_Label;
uic_App_Connection_Label = ui_App_Connection_Label;
uic_info_Label = ui_info_Label;
uic_Maintenance_Label = ui_Maintenance_Label;
uic_tempStatusButton = ui_tempStatusButton;
uic_tempStatusPanel = ui_tempStatusPanel;
uic_returnButton11 = ui_returnButton11;
uic_tempStatusLabel = ui_tempStatusLabel;
uic_inlet_temp_danger = ui_inlet_temp_danger;
uic_outlet_temp_danger = ui_outlet_temp_danger;
uic_antifreeze_temp_danger = ui_antifreeze_temp_danger;
uic_filter_temp_danger = ui_filter_temp_danger;
uic_ContainerA1 = ui_ContainerA1;
uic_Inlet_Temp_status = ui_Inlet_Temp_status;
uic_Outlet_Temp_status = ui_Outlet_Temp_status;
uic_antifreeze_temp_status = ui_antifreeze_temp_status;
uic_fan_2_filter_alarm_status = ui_fan_2_filter_alarm_status;
uic_alarmStatusButton = ui_alarmStatusButton;
uic_alarmStatusPanel = ui_alarmStatusPanel;
uic_returnButton2 = ui_returnButton2;
uic_alarmStatusLabel = ui_alarmStatusLabel;
uic_high_temp_danger = ui_high_temp_danger;
uic_door_danger = ui_door_danger;
uic_filter_danger = ui_filter_danger;
uic_high_pressure_danger = ui_high_pressure_danger;
uic_ContainerA2 = ui_ContainerA2;
uic_high_Temp_Alarm = ui_high_Temp_Alarm;
uic_Door_Alarm = ui_Door_Alarm;
uic_Filter_Alarm = ui_Filter_Alarm;
uic_high_Pressure_Alarm = ui_high_Pressure_Alarm;
uic_relayStatusButton = ui_relayStatusButton;
uic_relayStatusPanel = ui_relayStatusPanel;
uic_returnButton7 = ui_returnButton7;
uic_relayStatusLabel = ui_relayStatusLabel;
uic_ContainerA3 = ui_ContainerA3;
uic_evaprartor_Fan_Status = ui_evaprartor_Fan_Status;
uic_Compressor_Status = ui_Compressor_Status;
uic_condensor_Fan_1_Status = ui_condensor_Fan_1_Status;
uic_condensor_Fan_2_Status = ui_condensor_Fan_2_Status;
uic_networkStatusButton = ui_networkStatusButton;
uic_networkStatusPanel = ui_networkStatusPanel;
uic_returnButton8 = ui_returnButton8;
uic_Connect_to_Device_Label = ui_Connect_to_Device_Label;
uic_ContainerA4 = ui_ContainerA4;
uic_access_Point_wifi = ui_access_Point_wifi;
uic_access_Point_Password = ui_access_Point_Password;
uic_guidetext = ui_guidetext;
uic_QRcode_Tab = ui_QRcode_Tab;
uic_Application_QR_Code = ui_Application_QR_Code;
uic_download_App_Label = ui_download_App_Label;
uic_connectToapp = ui_connectToapp;
uic_Connection_QR_Code = ui_Connection_QR_Code;
uic_Connect_App_Label = ui_Connect_App_Label;
uic_show_QR_Code_Button = ui_show_QR_Code_Button;
uic_Hide_QR_Code_Button = ui_Hide_QR_Code_Button;
uic_deviceInfoButton = ui_deviceInfoButton;
uic_deviceInfoPanel = ui_deviceInfoPanel;
uic_returnButton9 = ui_returnButton9;
uic_deviceInfoLabel = ui_deviceInfoLabel;
uic_ContainerA5 = ui_ContainerA5;
uic_serial_number = ui_serial_number;
uic_customerID = ui_customerID;
uic_activationDate = ui_activationDate;
uic_serviceInfoButton = ui_serviceInfoButton;
uic_serviceInfoPanel = ui_serviceInfoPanel;
uic_returnButton10 = ui_returnButton10;
uic_serviceInfoLabel = ui_serviceInfoLabel;
uic_ContainerA6 = ui_ContainerA6;
uic_servicesintervals = ui_servicesintervals;

}
